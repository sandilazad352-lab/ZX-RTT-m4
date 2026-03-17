#include "zx_uart.h"
#include "stdio.h"
#include "aic_hal_clk.h"
#include "aic_clk_id.h"
#include "aic_reset_id.h"
#include "aic_hal_reset.h"
#include "aic_osal.h"
#include "aic_soc.h"
#include "math.h"
#include "float.h"

#ifndef AIC_CLK_UART_MAX_FREQ
#define AIC_CLK_UART_MAX_FREQ 100000000 /* max 100M*/
#endif

typedef struct {
    union {
        __IM uint32_t RBR;  /* Offset: 0x000 (R/ )  Receive buffer register */
        __OM uint32_t THR;  /* Offset: 0x000 ( /W)  Transmission hold register */
        __IOM uint32_t DLL; /* Offset: 0x000 (R/W)  Clock frequency division low section register */
    };
    union {
        __IOM uint32_t DLH; /* Offset: 0x004 (R/W)  Clock frequency division high section register */
        __IOM uint32_t IER; /* Offset: 0x004 (R/W)  Interrupt enable register */
    };
    union {
        __IM uint32_t IIR;  /* Offset: 0x008 (R/ )  Interrupt indicia register */
        __IOM uint32_t FCR; /* Offset: 0x008 (W)    FIFO control register */
    };
    __IOM uint32_t LCR; /* Offset: 0x00C (R/W)  Transmission control register */
    __IOM uint32_t MCR; /* Offset: 0x010 (R/W)  Modem Control register */
    __IM uint32_t LSR;  /* Offset: 0x014 (R/ )  Transmission state register */
    __IM uint32_t MSR;  /* Offset: 0x018 (R/ )  Modem state register */
    uint32_t RESERVED1[24];
    __IM uint32_t USR; /* Offset: 0x07c (R/ )  UART state register */
    __IM uint32_t TFL;  /* Offset: 0x080 (R/ )  UART tx fifo level register */
    __IM uint32_t RFL;  /* Offset: 0x084 (R/ )  UART rx fifo level register */
    __IOM uint32_t HSK; /* Offset: 0x088 (R/W)  UART dma hsk register */
    uint32_t RESERVED3[6];
    __IOM uint32_t HALT; /* Offset: 0x0A4 */
} aic_usart_reg_t;

#define UART_RX_FIFO_SIZE 256
#define UART_TX_FIFO_SIZE 256
#define IER_PTIME_ENABLE (1 << 7)
#define IER_SHIFTER_REG_EMPTY_ENABLE (1 << 5)

typedef struct {
    uint8_t* fifo_data;
    uint16_t start_pos;
    uint16_t end_pos; // pos next write data
    uint8_t overflow_count;
    uint32_t fifo_length;
} rx_fifo_t;

typedef struct {
    aic_usart_priv_t* usart;
    aicos_sem_t tx_sem;
    aicos_sem_t tx_finish_sem;
    aicos_sem_t tx_fifo_ptr_empty_sem;
    aicos_sem_t rx_sem;
    aicos_sem_t rx_available_sem;
    uint8_t* tx_fifo_ptr;
    uint32_t tx_fifo_len;

    rx_fifo_t rx_fifo;
    uint16_t rx_notify_len;

    uint32_t FCR;
} uart_handle_t;

volatile uart_handle_t* g_uart_handle[UART_NUM_MAX];

static int find_optimal_dividers(uint32_t src_clk, uint32_t dst_clk, uint32_t div_max, uint32_t div_2_max, uint32_t peripherals_clk_max, uint32_t *best_div_1, uint32_t *best_div_2) {
    double min_difference = DBL_MAX;
    int found = -1;

    uint32_t start_div_1 = src_clk / peripherals_clk_max;
    if (start_div_1 < 1) start_div_1 = 1;
    if (start_div_1 > div_max) return -1;

    double total_div = (double)src_clk / dst_clk;

    for (uint32_t div_1 = start_div_1; div_1 <= div_max; div_1++) {
        uint32_t div_2 = (uint32_t)round(total_div / div_1);
        if (div_2 > div_2_max || div_2 < 1) continue;

        double difference = fabs((double)dst_clk - ((double)src_clk / div_1) / div_2);

        if (difference < min_difference) {
            min_difference = difference;
            if (best_div_1) {
                *best_div_1 = div_1;
            }
            if (best_div_2) {
                *best_div_2 = div_2;
            }
            found = 0;
            if (difference == 0) break;
        }
    }

    return found;
}

static irqreturn_t uart_isr_handler(int irq_num, void *arg) {
    uart_num_t uart_num = irq_num - UART0_IRQn;
    volatile uart_handle_t* handle = g_uart_handle[uart_num];
    aic_usart_reg_t *addr = (aic_usart_reg_t *)handle->usart->base;
    uint32_t iir_value = addr->IIR;
    uint8_t irq_type = iir_value & 0x0f;

    // one byte fifo not support len notify
    if (irq_type == 0x04 && ((handle->FCR & FRC_RX_FIFO_SET(3)) == FRC_RX_FIFO_SET(0))) {
        irq_type = 0x0c;
    }

    if (irq_type == 0x04) { // RDA interrupt
        // keep one byte in fifo
        while (addr->RFL > 1) {
            uint16_t pos_now = handle->rx_fifo.end_pos + 1;
            if (pos_now == handle->rx_fifo.fifo_length) {
                pos_now = 0;
            }
            if (pos_now == handle->rx_fifo.start_pos) {
                handle->rx_fifo.overflow_count++;
                volatile uint8_t unused = addr->RBR;
                (void)unused;
                continue ;
            }
            handle->rx_fifo.fifo_data[handle->rx_fifo.end_pos] = addr->RBR & 0xff;
            handle->rx_fifo.end_pos = pos_now;
        }

        // if length  + fifo_length > notify, read all
        int16_t length = handle->rx_fifo.end_pos - handle->rx_fifo.start_pos;
        if (length < 0) {
            length = handle->rx_fifo.fifo_length + length;
        }

        if (length + 1 >= handle->rx_notify_len) {
            irq_type = 0x0c;
        }
    } 
    
    if (irq_type == 0x0C) { // rx timeout interrupt
        while (addr->LSR & LSR_DATA_READY) {
            uint16_t pos_now = handle->rx_fifo.end_pos + 1;
            if (pos_now == handle->rx_fifo.fifo_length) {
                pos_now = 0;
            }
            if (pos_now == handle->rx_fifo.start_pos) {
                handle->rx_fifo.overflow_count++;
                volatile uint8_t unused = addr->RBR;
                (void)unused;
                continue ;
            }
            handle->rx_fifo.fifo_data[handle->rx_fifo.end_pos] = addr->RBR & 0xff;
            handle->rx_fifo.end_pos = pos_now;
        }
        aicos_sem_give(handle->rx_available_sem);
    } else if (irq_type == 0x02 && g_uart_handle[uart_num]->tx_fifo_len != 0) { // THRE interrupt
        uint16_t empty_size = UART_TX_FIFO_SIZE - addr->TFL;
        uint32_t write_size = g_uart_handle[uart_num]->tx_fifo_len > empty_size ? empty_size : g_uart_handle[uart_num]->tx_fifo_len;
        for (uint32_t i = 0; i < write_size; i++) {
            addr->THR = g_uart_handle[uart_num]->tx_fifo_ptr[i];
        }
        g_uart_handle[uart_num]->tx_fifo_len -= write_size;
        if (g_uart_handle[uart_num]->tx_fifo_len) {
            g_uart_handle[uart_num]->tx_fifo_ptr += write_size;
        } else {
            handle->FCR &= ~FRC_TX_FIFO_SET(3);
            handle->FCR |= FRC_TX_FIFO_SET(0);
            addr->FCR = handle->FCR;
            // enable shifter empty irq
            addr->IER |= IER_SHIFTER_REG_EMPTY_ENABLE;
            g_uart_handle[uart_num]->tx_fifo_ptr = NULL;
            aicos_sem_give(g_uart_handle[uart_num]->tx_fifo_ptr_empty_sem);
        }
    }

    if (iir_value & (1 << 4)) {
        // disable shifter empty irq
        addr->IER &= ~IER_SHIFTER_REG_EMPTY_ENABLE;
        aicos_sem_give(g_uart_handle[uart_num]->tx_finish_sem);
    }

    return IRQ_NONE;
}

int zx_uart_init(uart_num_t uart_num, uart_config_t* config) {
    if (uart_num >= UART_NUM_MAX || config == NULL) {
        return -1;
    }
    
    if (config->baud_rate > 3000000) {
        return -2;
    }

    uint32_t clk_id = CLK_UART0 + uart_num;
    uint32_t parent_clk_id = hal_clk_get_parent(clk_id);
    uint32_t parent_freq   = hal_clk_get_freq(parent_clk_id);
    uint32_t div_max = 32;
    #if defined(AIC_CMU_DRV_V12)
    div_max = 16;
    #endif

    uint32_t best_div_1 = 0;
    uint32_t best_div_2 = 0;
    // uart baud rate divider spec
    uint8_t div_spec = 16;
    int ret = find_optimal_dividers(parent_freq, config->baud_rate * div_spec, div_max, 65535, AIC_CLK_UART_MAX_FREQ, &best_div_1, &best_div_2);
    if (ret != 0) {
        return ret;
    }
    best_div_2 = best_div_2 * div_spec;
    uint32_t peripherals_clk = parent_freq / best_div_1;
    if (abs(peripherals_clk / best_div_2 - config->baud_rate) > 0.05 * config->baud_rate) {
        return -3;
    }

    hal_clk_set_freq(clk_id, peripherals_clk);
    hal_clk_enable(clk_id);
    hal_reset_assert(RESET_UART0 + uart_num);
    aic_udelay(500);
    hal_reset_deassert(RESET_UART0 + uart_num);
    
    usart_handle_t uart = hal_usart_initialize(uart_num, NULL, NULL);
    uint8_t function = config->mode == UART_RS485_MODE ? 2 : 0;
    ret = hal_usart_config(uart, config->baud_rate, USART_MODE_ASYNCHRONOUS, config->parity, config->stop_bits, config->data_bits, function);
    if (ret != 0) {
        return ret;
    }

    uart_handle_t* handle = (uart_handle_t *)calloc(1, sizeof(uart_handle_t));
    if (handle == NULL) {
        return -3;
    }
    handle->usart = (aic_usart_priv_t *)uart;
    // TX config
    handle->tx_sem = aicos_sem_create(1);
    handle->tx_finish_sem = aicos_sem_create(0);
    handle->tx_fifo_ptr_empty_sem = aicos_sem_create(0);
    handle->tx_fifo_ptr = NULL;
    handle->tx_fifo_len = 0;
    
    // RX Config
    handle->rx_sem = aicos_sem_create(1);
    handle->rx_available_sem = aicos_sem_create(0);
    handle->rx_fifo.fifo_data = (uint8_t *)malloc(config->rx_fifo_len);
    if (handle->rx_fifo.fifo_data == NULL) {
        ret = -4;
        goto err;
    }
    handle->rx_fifo.fifo_length = config->rx_fifo_len;
    handle->rx_notify_len = config->rx_notify_len;
    
    aic_usart_reg_t *addr = (aic_usart_reg_t *)handle->usart->base;
    // disable write irq 
    addr->IER &= ~IER_THRE_INT_ENABLE;
    handle->FCR = FCR_FIFO_EN;
    handle->FCR &= ~FRC_RX_FIFO_SET(3);
    handle->FCR |= FRC_RX_FIFO_SET((config->rx_irq_type));
    // Enable rx fifo, irq
    addr->FCR = handle->FCR;
    addr->IER |= IER_RDA_INT_ENABLE;

    aicos_request_irq(handle->usart->irq, uart_isr_handler, 0, "uart", NULL);
    aicos_irq_enable(handle->usart->irq);
    g_uart_handle[uart_num] = handle;
    return 0;

err:
    if (handle->tx_sem) {
        aicos_sem_delete(handle->tx_sem);
    }
    if (handle->tx_finish_sem) {
        aicos_sem_delete(handle->tx_finish_sem);
    }
    if (handle->tx_fifo_ptr_empty_sem) {
        aicos_sem_delete(handle->tx_fifo_ptr_empty_sem);
    }
    if (handle->rx_sem) {
        aicos_sem_delete(handle->rx_sem);
    }
    if (handle->rx_available_sem) {
        aicos_sem_delete(handle->rx_available_sem);
    }
    if (handle->rx_fifo.fifo_data) {
        free(handle->rx_fifo.fifo_data);
    }
    return ret;
}

int zx_uart_write(uart_num_t num, uint8_t* data, uint32_t len) {
    if (num >= UART_NUM_MAX || data == NULL || len == 0) {
        return -1;
    }
    volatile uart_handle_t* handle = g_uart_handle[num];
    if (handle == NULL) {
        return -2;
    }

    aic_usart_reg_t *addr = (aic_usart_reg_t *)handle->usart->base;
    // clear finish sem
    aicos_sem_take(handle->tx_finish_sem, 0);
    aicos_sem_take(handle->tx_sem, AICOS_WAIT_FOREVER);
    // data not finish write, write last data into fifo
    if (handle->tx_fifo_ptr) {
        uint8_t wait_mask = 0;
        addr->IER &= ~IER_THRE_INT_ENABLE;
        if (handle->tx_fifo_ptr) {
            wait_mask = 1;
            aicos_sem_take(handle->tx_fifo_ptr_empty_sem, 0);
        }
        addr->IER |= IER_THRE_INT_ENABLE;
        // wait ptr empty
        if (wait_mask) {
            aicos_sem_take(handle->tx_fifo_ptr_empty_sem, AICOS_WAIT_FOREVER);
        }
    }

    // clear tx sem
    aicos_sem_take(handle->tx_sem, 0);
    // disable intr first
    addr->IER &= ~(IER_THRE_INT_ENABLE | IER_SHIFTER_REG_EMPTY_ENABLE);

    uint16_t empty_size = UART_TX_FIFO_SIZE - addr->TFL;
    uint32_t write_size = len > empty_size ? empty_size : len;
    for (uint32_t i = 0; i < write_size; i++) {
        addr->THR = data[i];
    }
    len -= write_size;

    handle->FCR &= ~FRC_TX_FIFO_SET(3);
    if (len) {
        // if not write all data to fifo, save the left data to handle
        handle->FCR |= FRC_TX_FIFO_SET(2);
        handle->tx_fifo_ptr = data + write_size;
        handle->tx_fifo_len = len;
    } else {
        handle->tx_fifo_ptr = NULL;
        handle->tx_fifo_len = 0;
        handle->FCR |= FRC_TX_FIFO_SET(0);
        addr->IER |= IER_SHIFTER_REG_EMPTY_ENABLE;
    }
    addr->FCR = handle->FCR;
    // enable isr
    addr->IER |= IER_PTIME_ENABLE | IER_THRE_INT_ENABLE;
    aicos_sem_give(handle->tx_sem);

    return write_size;
}

int zx_uart_write_wait_done(uart_num_t num) {
    if (num >= UART_NUM_MAX) {
        return -1;
    }
    volatile uart_handle_t* handle = g_uart_handle[num];
    if (handle == NULL) {
        return -1;
    }
    aic_usart_reg_t *addr = (aic_usart_reg_t *)handle->usart->base;
    if ((addr->LSR & AIC_LSR_TRANS_EMPTY)) {
        return 0;
    }
    aicos_sem_take(g_uart_handle[num]->tx_finish_sem, AICOS_WAIT_FOREVER);
    return 0;
}

int zx_uart_write_all(uart_num_t num, uint8_t* data, uint32_t len) {
    if (num >= UART_NUM_MAX) {
        return -1;
    }
    zx_uart_write(num, data, len);
    zx_uart_write_wait_done(num);
    return len;
}

int zx_uart_write_byte(uart_num_t num, uint8_t byte) {
    return zx_uart_write_all(num, &byte, 1);
}

uint32_t zx_uart_rx_available(uart_num_t num) {
    if (num >= UART_NUM_MAX) {
        return 0;
    }
    volatile uart_handle_t* handle = g_uart_handle[num];
    if (handle == NULL) {
        return 0;
    }
    int32_t length = handle->rx_fifo.end_pos - handle->rx_fifo.start_pos;
    if (length < 0) {
        length = handle->rx_fifo.fifo_length + length;
    }
    return length;
}

uint32_t zx_uart_read(uart_num_t num, uint8_t* data, uint32_t len) {
    if (num >= UART_NUM_MAX || data == NULL || len == 0) {
        return 0;
    }
    volatile uart_handle_t* handle = g_uart_handle[num];
    if (handle == NULL) {
        return 0;
    }
    uint32_t read_size = 0;
    aicos_sem_take(handle->rx_sem, AICOS_WAIT_FOREVER);
    while (read_size < len && handle->rx_fifo.start_pos != handle->rx_fifo.end_pos) {
        data[read_size] = handle->rx_fifo.fifo_data[handle->rx_fifo.start_pos];
        handle->rx_fifo.start_pos++;
        if (handle->rx_fifo.start_pos == handle->rx_fifo.fifo_length) {
            handle->rx_fifo.start_pos = 0;
        }
        read_size++;
    }
    aicos_sem_give(handle->rx_sem);
    return read_size;
}

uint32_t zx_uart_wait_rx_data(uart_num_t num, uint32_t timeout_ms) {
    if (num >= UART_NUM_MAX) {
        return 0;
    }
    volatile uart_handle_t* handle = g_uart_handle[num];
    if (handle == NULL) {
        return 0;
    }
    uint32_t length = zx_uart_rx_available(num);
    if (length >= handle->rx_notify_len) {
        aicos_sem_take(handle->rx_available_sem, 0);
        return length;
    }
    aicos_sem_take(handle->rx_available_sem, timeout_ms);
    return zx_uart_rx_available(num);
}

int zx_uart_clean_rx_fifo_overflow(uart_num_t num) {
    if (num >= UART_NUM_MAX) {
        return -1;
    }
    volatile uart_handle_t* handle = g_uart_handle[num];
    if (handle == NULL) {
        return -1;
    }
    aicos_sem_take(handle->rx_sem, AICOS_WAIT_FOREVER);
    handle->rx_fifo.overflow_count = 0;
    aicos_sem_give(handle->rx_sem);
    return 0;
}

int zx_uart_clean_rx_fifo(uart_num_t num) {
    if (num >= UART_NUM_MAX) {
        return -1;
    }
    volatile uart_handle_t* handle = g_uart_handle[num];
    if (handle == NULL) {
        return -1;
    }
    aicos_sem_take(handle->rx_sem, AICOS_WAIT_FOREVER);
    handle->rx_fifo.start_pos = handle->rx_fifo.end_pos;
    handle->rx_fifo.overflow_count = 0;
    aicos_sem_give(handle->rx_sem);
    return 0;
}
