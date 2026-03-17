#pragma once

#include "aic_core.h"
#include "aic_osal.h"
#include "hal_i2c.h"

typedef struct _i2c_port_obj_t {
    int8_t i2c_num;     // 0, 1, 2, 3, 4... 
    int8_t scl;         // not used, no effect
    int8_t sda;         // not used, no effect
    uint32_t freq;
    int timeout;        // not used, no effect
    int port;           // i2c handle, returns like i2c_open
} i2c_port_obj_t;

typedef struct _i2c_device_t {
    i2c_port_obj_t* i2c_port;
    uint8_t reg_bit;
    uint8_t addr;
} i2c_device_t;

// I2C_OK define in hal_i2c.h 
// #define I2C_OK                      (0)
#define I2C_FAIL                    (I2C_ERR)
#define I2C_ERR_INVALID_ARG         (10)

// used for thread safe
#define I2C_MUTEX_TYPE_T            aicos_mutex_t
#define I2C_MUTEX_CREATE()          (aicos_mutex_create())
#define I2C_MUTEX_TAKE(x)           (aicos_mutex_take(x, AICOS_WAIT_FOREVER))
#define I2C_MUTEX_GIVE(x)           (aicos_mutex_give(x))          

#define I2C_TIMEOUT_MS (100)
#define I2C_DEVICE_TAG "I2C-Dev"

#ifndef I2C_NUM_MAX
#define I2C_NUM_MAX 3
#endif

// #define CONFIG_I2C_DEVICE_DEBUG_INFO
#define CONFIG_I2C_DEVICE_DEBUG_ERROR
// #define CONFIG_I2C_DEVICE_DEBUG_REG
// #define CONFIG_I2C_DEVICE_DEBUG

#ifdef CONFIG_I2C_DEVICE_DEBUG_INFO
#define i2c_log_i(format, ...) printf("[I]"I2C_DEVICE_TAG": " format "\r\n", ##__VA_ARGS__)
#else
#define i2c_log_i(format...)
#endif

#ifdef CONFIG_I2C_DEVICE_DEBUG_ERROR
#define i2c_log_e(format, ...) printf("[E]"I2C_DEVICE_TAG": " format "\r\n", ##__VA_ARGS__)
#else
#define i2c_log_e(format...)
#endif

#ifdef CONFIG_I2C_DEVICE_DEBUG
#define i2c_log_d(format, ...) printf("[D]"I2C_DEVICE_TAG": " format "\r\n", ##__VA_ARGS__)
#else
#define i2c_log_d(format...)
#endif


#ifdef CONFIG_I2C_DEVICE_DEBUG_REG
#define i2c_log_reg(buffer, buffer_len) \
    do { \
        int i; \
        for (i = 0; i < buffer_len; i++) { \
            printf("0x%02x ", buffer[i]); \
        } \
        printf("\n"); \
    } while (0)
#else
#define i2c_log_reg(buffer, buffer_len)
#endif

#define I2C_PORT_NO_INIT (-1)

/**
 * Write bytes to an I2C device
 * @param i2c_port      the I2C port number to use for communication
 * @param device_addr   the address of the I2C device
 * @param reg_addr      the register address in the device to write to
 * @param reg_len       the length of the register address in bytes
 * @param data          pointer to the data bytes to be written
 * @param length        the number of bytes to write
 * @return              0 on success, non-zero error code on failure
 */

int i2c_dev_write_bytes(int i2c_port, uint8_t device_addr, uint32_t reg_addr, uint8_t reg_len, const uint8_t *data, uint16_t length);

/**
 * Read bytes from an I2C device
 * @param i2c_port      the I2C port number to use for communication
 * @param device_addr   the address of the I2C device
 * @param reg_addr      the register address in the device to read from
 * @param reg_len       the length of the register address in bytes
 * @param data          pointer to a buffer where the read data will be stored
 * @param length        the number of bytes to read
 * @return              0 on success, non-zero error code on failure
 */

int i2c_dev_read_bytes(int i2c_port, uint8_t device_addr, uint32_t reg_addr, uint8_t reg_len, uint8_t *data, uint16_t length);

/**
 * Update the pin configuration of an I2C port
 * @param i2c_port      the I2C port number to update
 * @param select_port   pointer to the new pin configuration object
 * @param old_port      pointer to the old pin configuration object to be updated
 * @return              the updated port number on success, I2C_PORT_NO_INIT (-1) on error
 */

int i2c_dev_update_pins(int i2c_port, i2c_port_obj_t* select_port, i2c_port_obj_t* old_port);

/**
 * Update the frequency of an I2C port
 * @param i2c_port      the I2C port number to update
 * @param port_obj      pointer to the port object containing the new frequency
 * @return              the updated port number on success, I2C_PORT_NO_INIT (-1) on error
 */

int i2c_dev_update_freq(int i2c_port, i2c_port_obj_t* port_obj);

/**
 * Initialize an I2C port
 * @param i2c_num       the I2C port number to initialize
 * @param port_obj      pointer to the port object containing initialization parameters
 * @return              the initialized port number on success, I2C_PORT_NO_INIT (-1) on error
 */

int i2c_dev_init(int i2c_num, i2c_port_obj_t* port_obj);

/**
 * Deinitialize an I2C port
 * @param i2c_port      the I2C port number to deinitialize
 * @return              0 on success, non-zero error code on failure
 */

int i2c_dev_deinit(int i2c_port);
