#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "i2c_device_hal.h"

/**
 * @brief Used when the I2C peripheral does not use registers 
 * for read/write operations.
 * 
 * This macro is used in situations when the I2C peripheral
 * connected to port A does not use registers to perform read
 * or write operations to it. Since register addresses are
 * uint8, this 32-bit value exceeds the range of possible 
 * register addresses and is used to trigger logic to bypass 
 * the write to the register address.
 *
 */
/* @[declare_i2c_no_reg] */
#define I2C_REG_8BIT            0
#define I2C_REG_16BIT_BIG       1
#define I2C_REG_16BIT_LITTLE    2
#define I2C_NO_REG              3
/* @[declare_i2c_no_reg] */

/**
 * @brief A pointer to the I2C peripheral configuration.
 * 
 * Contains configuration related to the peripheral in 
 * the following structure:
 * - addr       (device address)
 * - i2c_port
 *  - port     (I2C port number — 0 or 1)
 *  - sda      (I2C port's SDA pin)
 *  - scl      (I2C port's SCL pin)
 *  - freq     (I2C port frequency)
 *
 */
/* @[declare_i2cdevice_t] */
typedef void * I2CDevice_t;
/* @[declare_i2cdevice_t] */


I2CDevice_t i2c_malloc_device(int i2c_num, int8_t sda, int8_t scl, uint32_t freq, uint8_t device_addr);

void i2c_free_device(I2CDevice_t i2c_device);

int i2c_apply_bus(I2CDevice_t i2c_device);

int i2c_free_bus(I2CDevice_t i2c_device);

/**
 * @brief Changes the frequency of the I2C device.
 *
 * This function allows you to change the frequency of the I2C device.
 *
 * @param i2c_device The I2C device to change the frequency for.
 * @param freq The new frequency to set for the I2C device.
 * @return 0 if successful, otherwise an error code.
 */
int i2c_device_change_freq(I2CDevice_t i2c_device, uint32_t freq);

int i2c_device_change_timeout(I2CDevice_t i2c_device, int32_t timeout);

/**
 * @brief Sets the specified register bits of an I2C device.
 *
 * This function sets the specified register bits of the given I2C device.
 *
 * @param i2c_device The I2C device to operate on.
 * @param reg_bit The register bits to set. [I2C_REG_8BIT, I2C_REG_16BIT_BIG, I2C_REG_16BIT_LITTLE, I2C_NO_REG]
 * @return 0 if successful, otherwise an error code.
 */
int i2c_device_set_reg_bits(I2CDevice_t i2c_device, uint32_t reg_bit);

/**
 * @brief Reads multiple bytes from an I2C device.
 *
 * This function reads multiple bytes from the specified I2C device starting from the given register address.
 *
 * @param i2c_device The I2C device to read from.
 * @param reg_addr The register address to start reading from.
 * @param data Pointer to the buffer where the read data will be stored.
 * @param length The number of bytes to read.
 * @return 0 on success, or a negative error code on failure.
 */
int i2c_read_bytes(I2CDevice_t i2c_device, uint32_t reg_addr, uint8_t *data, uint16_t length);

/**
 * @brief Reads a byte from the specified register address of an I2C device.
 *
 * This function reads a byte from the specified register address of the given I2C device.
 *
 * @param i2c_device The I2C device to read from.
 * @param reg_addr The register address to read from.
 * @param data Pointer to store the read byte.
 * @return 0 if successful, otherwise an error code.
 */
int i2c_read_byte(I2CDevice_t i2c_device, uint32_t reg_addr, uint8_t* data);

/**
 * @brief Reads a single bit from a register of an I2C device.
 *
 * This function reads a single bit from the specified register address of the given I2C device.
 *
 * @param i2c_device The I2C device to read from.
 * @param reg_addr The address of the register to read from.
 * @param data Pointer to store the read bit value.
 * @param bit_pos The position of the bit to read (0-7).
 * @return 0 if successful, otherwise an error code.
 */
int i2c_read_bit(I2CDevice_t i2c_device, uint32_t reg_addr, uint8_t *data, uint8_t bit_pos);

/*
    Read bits from 8 bit reg
    bit_pos = 4, bit_length = 3
    read ->  0b|1|0|1|0|1|1|0|0| 
             0b|-|x|x|x|-|-|-|-|   
    data = 0b00000010
*/
int i2c_read_bits(I2CDevice_t i2c_device, uint32_t reg_addr, uint8_t *data, uint8_t bit_pos, uint8_t bit_length);

/**
 * @brief Writes a sequence of bytes to an I2C device.
 *
 * This function writes a sequence of bytes to the specified I2C device.
 *
 * @param i2c_device The I2C device to write to.
 * @param reg_addr The register address to write to.
 * @param data Pointer to the data buffer containing the bytes to write.
 * @param length The number of bytes to write.
 * @return Returns 0 on success, or a negative error code on failure.
 */
int i2c_write_bytes(I2CDevice_t i2c_device, uint32_t reg_addr, uint8_t *data, uint16_t length);

/**
 * @brief Reads multiple bytes from an I2C device without sending a stop condition.
 *
 * This function reads `length` bytes from the I2C device specified by `i2c_device`,
 * starting from the register address `reg_addr`. The read data is stored in the buffer
 * pointed to by `data`.
 *
 * @param i2c_device The I2C device to read from.
 * @param reg_addr The register address to start reading from.
 * @param data Pointer to the buffer where the read data will be stored.
 * @param length The number of bytes to read.
 * @return The number of bytes read, or a negative error code if an error occurred.
 */
int i2c_read_bytes_no_stop(I2CDevice_t i2c_device, uint32_t reg_addr, uint8_t *data, uint16_t length);

/**
 * @brief Writes a byte of data to the specified register address of an I2C device.
 *
 * This function writes a single byte of data to the specified register address of the
 * I2C device identified by the `i2c_device` parameter.
 *
 * @param i2c_device The I2C device to write to.
 * @param reg_addr The register address to write the data to.
 * @param data The byte of data to write.
 * @return 0 if the write operation is successful, otherwise an error code.
 */
int i2c_write_byte(I2CDevice_t i2c_device, uint32_t reg_addr, uint8_t data);

/**
 * @brief Writes a single bit to a specific register address of an I2C device.
 *
 * This function writes a single bit to the specified register address of the given I2C device.
 *
 * @param i2c_device The I2C device to write to.
 * @param reg_addr The register address to write the bit to.
 * @param data The data value to write (0 or 1).
 * @param bit_pos The position of the bit to write (0 to 7).
 * @return Returns 0 on success, or a negative error code on failure.
 */
int i2c_write_bit(I2CDevice_t i2c_device, uint32_t reg_addr, uint8_t data, uint8_t bit_pos);

/*
    Read before bits from 8 bit reg, then update write bits
    1. Read data 0b10101100
    2. write, 0b0101, bit_pos = 4, bit_length = 3 
    read ->  0b|1|0|1|0|1|1|0|0| 
             0b|-|x|x|x|-|-|-|-| 
    write -> 0b|1|1|0|1|1|1|0|0|  
    data = 0b00000101
*/
int i2c_write_bits(I2CDevice_t i2c_device, uint32_t reg_addr, uint8_t data, uint8_t bit_pos, uint8_t bit_length);

/**
 * @brief Checks if the given I2C device is valid.
 *
 * This function checks if the provided I2C device is valid and can be used for communication.
 *
 * @param i2c_device The I2C device to be checked.
 * @return true if the I2C device is valid, false otherwise.
 */
int i2c_device_valid(I2CDevice_t i2c_device);

/**
 * Scans the I2C bus for connected devices and prints the results.
 *
 * @param i2c_device The I2C device to use for scanning.
 * @return The number of devices found on the I2C bus.
 */
int i2c_bus_scan_print(I2CDevice_t i2c_device);

/**
 * Scans the I2C bus and prints the detected devices by performing read operations.
 *
 * @param i2c_device The I2C device to use for scanning.
 * @return The number of devices detected on the I2C bus.
 */
int i2c_bus_scan_print_by_read(I2CDevice_t i2c_device);

#ifdef __cplusplus
}
#endif