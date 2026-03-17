#define LOG_TAG "I2C"

#include <inttypes.h>
#include "i2c_device_hal.h"
#include "i2c_device.h"
#include <hal_i2c.h>

// Return i2c handle for read and write, -1 mean error
int i2c_dev_init(int i2c_num, i2c_port_obj_t* port_obj) {
    int ret = -1;

    ret = aic_i2c_init(i2c_num);
    if (ret) {
        i2c_log_e("i2c init %d failed: %d", i2c_num, ret);
        return ret;
    }
    if (i2c_num > 3) {
        i2c_log_e("i2c %d not support", i2c_num);
        return -1;
    }
    int i2c_base_list[] = { I2C0_BASE, I2C1_BASE, I2C2_BASE, I2C3_BASE };
    int i2c_base = i2c_base_list[i2c_num];
    ret = aic_i2c_set_master_slave_mode(i2c_base, true);
    if (ret) {
        i2c_log_e("i2c %d set master mode failed: %d", i2c_num, ret);
        return -1;
    }
    hal_i2c_set_hold(i2c_base, 10);
    aic_i2c_master_10bit_addr(i2c_base, false);
    aic_i2c_slave_10bit_addr(i2c_base, false);
    aic_i2c_master_enable_transmit_irq(i2c_base);
    aic_i2c_master_enable_receive_irq(i2c_base);

    port_obj->port = i2c_base;
    i2c_dev_update_freq(i2c_num, port_obj);

    i2c_log_i("i2c %d init success", i2c_num);
    return port_obj->port;
}

int i2c_dev_update_pins(int i2c_num, i2c_port_obj_t* select_port, i2c_port_obj_t* old_port) {
    i2c_log_e("Not support update pins");
    return select_port->port;
}

int i2c_dev_update_freq(int i2c_num, i2c_port_obj_t* port_obj) {
    int i2c_base = port_obj->port;
    i2c_log_i("i2c update freq: %"PRIu32, port_obj->freq);
    if (port_obj->freq == 400000) {
        aic_i2c_speed_mode_select(i2c_base, I2C_DEFALT_CLOCK, true);
    } else if (port_obj->freq == 100000) {
        aic_i2c_speed_mode_select(i2c_base, I2C_DEFALT_CLOCK, false);
    } else {
        i2c_log_e("Only support 100K or 400K freq");
    }
    return port_obj->port;
}

int i2c_dev_deinit(int i2c_port) {
    // not support now
    i2c_log_e("Not support deinit now");
    return -1;
}

static int32_t aic_i2c_wait_iic_transmit(unsigned long reg_base, uint32_t timeout) {
    int32_t ret = I2C_OK;

    do {
        uint64_t timecount = timeout + aic_get_time_ms();

        while ((aic_i2c_get_transmit_fifo_num(reg_base) != 0U) && (ret == EOK)) {
            if (aic_get_time_ms() >= timecount) {
                ret = I2C_TIMEOUT;
            }
        }
    } while (0);

    return ret;
}


static int32_t aic_i2c_write(unsigned long reg_base, const uint8_t* buffer, uint16_t len, uint8_t level_fifo, uint16_t timeout, uint8_t stop_bit) {
    if (len == 0) {
        return I2C_OK;
    }

    int32_t ret = I2C_OK;
    uint16_t send_len = 0;
    uint32_t stop_time = 0;

    if (stop_bit) {
        len -= 1;
    }
    uint8_t fifo_length = level_fifo > len ? len : level_fifo;

    while (len) {
        while (fifo_length--) {
            aic_i2c_transmit_data(reg_base, buffer[send_len++]);
            len--;
        }
        if (fifo_length == 0) {
            ret = aic_i2c_wait_iic_transmit(reg_base, timeout);
            if (ret != I2C_OK) {
                i2c_log_d("write wait ack timeout");
                return I2C_TIMEOUT;
            }
        }
        fifo_length = I2C_FIFO_DEPTH > len ? len : I2C_FIFO_DEPTH;
    }
    
    if (stop_bit) {
        aic_i2c_transmit_data_with_stop_bit(reg_base, buffer[send_len++]);

        while (1) {
            uint32_t reg_val = aic_i2c_get_raw_interrupt_state(reg_base);
            if (reg_val & I2C_INTR_STOP_DET) {
                if (reg_val & I2C_INTR_TX_ABRT) {
                    return I2C_FAIL;
                } else {
                    break;
                }
            }
            stop_time++;
            if (stop_time > I2C_TIMEOUT_DEF_VAL) {
                i2c_log_d("write wait stop timeout");
                return I2C_TIMEOUT;
            }
        }
    }

    return I2C_OK;
}

// reg len (byte): 0, 1, 2
// data: NULL or data
int i2c_dev_write_bytes(int i2c_port, uint8_t device_addr, uint32_t reg_addr, uint8_t reg_len, const uint8_t *data, uint16_t length) {
    if (i2c_port == -1 || (length > 0 && data == NULL)) {
        return I2C_FAIL;
    }

    int reg_base = i2c_port;

    int32_t ret = I2C_OK;
    uint16_t size = reg_len + length;
    uint32_t timeout = 1000;
    uint32_t reg_val;

    aic_i2c_module_disable(i2c_port);
    aic_i2c_target_addr(i2c_port, device_addr);
    aic_i2c_module_enable(i2c_port);

    if (!size) {
        aic_i2c_transfer_stop_bit(reg_base);
        while (1) {
            reg_val = aic_i2c_get_raw_interrupt_state(reg_base);
            if (reg_val & I2C_INTR_STOP_DET) {
                if (reg_val & I2C_INTR_TX_ABRT) {
                    return I2C_FAIL;
                } else {
                    return I2C_OK;
                }
            }
        }
    }

    uint8_t fifo_len = I2C_FIFO_DEPTH;
    ret = aic_i2c_write(reg_base, (uint8_t *)&reg_addr, reg_len, fifo_len, timeout, length == 0);
    if (ret != I2C_OK) {
        return ret;
    }

    fifo_len -= reg_len;
    ret = aic_i2c_write(reg_base, (const uint8_t *)data, length, fifo_len, timeout, 1);

    return ret;
}

int i2c_dev_read_bytes(int i2c_port, uint8_t device_addr, uint32_t reg_addr, uint8_t reg_len, uint8_t *data, uint16_t length) {
    if (i2c_port == -1 || (length > 0 && data == NULL)) {
        return I2C_FAIL;
    }

    int reg_base = i2c_port;
    int ret = I2C_FAIL;

    if (reg_len) {
        ret = i2c_dev_write_bytes(i2c_port, device_addr, reg_addr, reg_len, NULL, 0);
        if (ret != I2C_OK) {
            return ret;
        }
    }

    struct aic_i2c_msg msg = {
        .addr = device_addr,
        .flags = 0,
        .buf = data,
        .len = length,
    };
    int32_t bytes_cnt = aic_i2c_master_receive_msg(reg_base, &msg, 1);
    
    return bytes_cnt == length ? I2C_OK : I2C_FAIL;
}
