#ifndef __ZX_DRIVER_UART_PROTOCOL_H_
#define __ZX_DRIVER_UART_PROTOCOL_H_

#include "stdio.h"
#include "aic_common.h"
#include "aic_hal_uart.h"

typedef enum {
    UART_NUM_0 = 0x0,
    UART_NUM_1,
    UART_NUM_2,
    UART_NUM_3,
    UART_NUM_4,
#if AIC_UART_DEV_NUM == 8
    UART_NUM_5,
    UART_NUM_6,
    UART_NUM_7,
    UART_NUM_8,
#endif
    UART_NUM_MAX,
} uart_num_t;

typedef enum {
    UART_DEFAULT_MODE = 0x0,
    UART_RS485_MODE,
} uart_mode_t;

typedef enum {
    IRQ_WHEN_1_BYTE = 0x0,      // not suggest, may cause high CPU usage
    IRQ_WHEN_QUARTER_FIFO,
    IRQ_WHEN_HALF_FIFO,
    IRQ_WHEN_FULL_FIFO,
} uart_rx_irq_type_t;

/**
 * @brief UART configuration structure.
 *
 * This structure defines the configuration parameters for a UART device.
 */
typedef struct {
    uint32_t baud_rate;             /**< Baud rate of the UART. */
    usart_data_bits_e data_bits;    /**< Number of data bits. */
    usart_stop_bits_e stop_bits;    /**< Number of stop bits. */
    usart_parity_e parity;          /**< Parity mode. */
    uart_mode_t mode;               /**< UART mode. */
    uint32_t rx_fifo_len;           /**< Length of the RX FIFO. */
    uint32_t rx_notify_len;         /**< Length at which RX notifications are triggered. */
    uart_rx_irq_type_t rx_irq_type; /**< RX IRQ type. */
} uart_config_t;

#define UART_DEFAULT_CONIFG { \
    .baud_rate = 115200, \
    .data_bits = USART_DATA_BITS_8, \
    .stop_bits = USART_STOP_BITS_1, \
    .parity = USART_PARITY_NONE, \
    .mode = UART_DEFAULT_MODE, \
    .rx_fifo_len = 1024, \
    .rx_notify_len = 128, \
    .rx_irq_type = IRQ_WHEN_QUARTER_FIFO, \
}

/**
 * @brief Initializes the UART with specified configurations.
 *
 * This function initializes the UART module with the given configurations, including baud rate, data bits, stop bits, and parity. 
 * It sets up the underlying hardware parameters and prepares the UART module for data transmission and reception.
 *
 * @param uart_num The UART number to initialize.
 * @param config Pointer to the uart_config_t structure containing the desired configurations.
 * @return Returns 0 on success, and a negative error code on failure.
 */
int zx_uart_init(uart_num_t uart_num, uart_config_t* config);

/**
 * @brief Writes data to the UART.
 *
 * This function writes data to the UART, sending the specified number of bytes from the provided data buffer. 
 * This is a non-blocking call that may return before all the data is transmitted if the FIFO gets filled. 
 * If the returned number of bytes is less than the length specified, it indicates the FIFO was full and not all data could be sent immediately. 
 * In such cases, to ensure all data is transmitted before freeing or altering the buffer, 
 * wait for the transmission to complete by checking or waiting on the appropriate transmission complete signal or event.
 *
 * @param num The UART number to write data to.
 * @param data Pointer to the data buffer.
 * @param len The number of bytes to write.
 * @return Returns the number of bytes written to the FIFO. If this number is less than 'len', the remaining data was not sent due to FIFO capacity constraints.
 */
int zx_uart_write(uart_num_t num, uint8_t* data, uint32_t len);

/**
 * @brief Waits until all queued UART write operations are done.
 *
 * This function blocks until all data previously queued for transmission by zx_uart_write() has been sent out of the UART.
 * It ensures that the UART's transmit buffer is empty and all data has been sent.
 *
 * @param num The UART number.
 * @return Returns 0 on success, and a negative error code on failure.
 */
int zx_uart_write_wait_done(uart_num_t num);

/**
 * @brief Writes all specified data to the UART, blocking until completion.
 *
 * This function writes data to the UART and does not return until all data has been transmitted. It is a blocking call that ensures all specified data is sent out.
 *
 * @param num The UART number to write data to.
 * @param data Pointer to the data buffer.
 * @param len The number of bytes to write.
 * @return Returns the number of bytes written.
 */
int zx_uart_write_all(uart_num_t num, uint8_t* data, uint32_t len);

/**
 * @brief Writes a single byte to the UART.
 *
 * This function writes a single byte to the UART. It is a convenience wrapper around zx_uart_write_all() for sending just one byte.
 *
 * @param num The UART number to write the byte to.
 * @param byte The byte to write.
 * @return Returns 0 on success, and a negative error code on failure.
 */
int zx_uart_write_byte(uart_num_t num, uint8_t byte);

/**
 * @brief Returns the number of bytes available to read from the UART.
 *
 * This function checks how many bytes have been received by the UART and are currently available to be read.
 *
 * @param num The UART number to check.
 * @return Returns the number of bytes available to read.
 */
uint32_t zx_uart_rx_available(uart_num_t num);

/**
 * @brief Reads data from the UART.
 *
 * This function reads up to the specified number of bytes from the UART into the provided buffer. It may return fewer bytes than requested if fewer are available.
 *
 * @param num The UART number to read data from.
 * @param data Pointer to the buffer where read data should be stored.
 * @param len The maximum number of bytes to read.
 * @return Returns the number of bytes actually read.
 */
uint32_t zx_uart_read(uart_num_t num, uint8_t* data, uint32_t len);

/**
 * @brief Waits for a specified amount of data to be available on the UART.
 *
 * This function blocks until at least a certain number of bytes are available to read from the UART or a timeout occurs.
 *
 * @param num The UART number to monitor.
 * @param timeout_ms The maximum time to wait for the specified amount of data, in milliseconds.
 * @return Returns the number of bytes available to read.
 */
uint32_t zx_uart_wait_rx_data(uart_num_t num, uint32_t timeout_ms);

/**
 * @brief Clears the UART RX FIFO overflow counter.
 *
 * This function resets the internal counter that tracks the number of times the receive FIFO has overflowed. Overflow occurs when incoming data is lost because the FIFO is full and new data continues to arrive.
 *
 * @param num The UART number.
 * @return Returns 0 on success, and a negative error code on failure.
 */
int zx_uart_clean_rx_fifo_overflow(uart_num_t num);

/**
 * @brief Clears the UART RX FIFO buffer.
 *
 * This function clears the receive FIFO, discarding any data that has been received but not yet read.
 *
 * @param num The UART number.
 * @return Returns 0 on success, and a negative error code on failure.
 */
int zx_uart_clean_rx_fifo(uart_num_t num);

#endif
