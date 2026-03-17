# I2C Device Library Documentation

## Introduction

The I2C device library offers a suite of interfaces for configuring and communicating with I2C devices. It supports a variety of I2C operations, including reading and writing bytes, bits, and multi-byte data, as well as the adjustment of frequency settings.

## Configuration and API Description

### Initialize I2C Device

Before communicating with an I2C device, you first need to allocate and initialize an I2C device object with `i2c_malloc_device`.

```c
I2CDevice_t i2c_malloc_device(int i2c_num, int8_t sda, int8_t scl, uint32_t freq, uint8_t device_addr);
```

### Read and Write Operations

- **Write Byte to I2C Device**

  ```c
  int i2c_write_byte(I2CDevice_t i2c_device, uint32_t reg_addr, uint8_t data);
  ```

- **Read Byte from I2C Device**

  ```c
  int i2c_read_byte(I2CDevice_t i2c_device, uint32_t reg_addr, uint8_t* data);
  ```

- **Write Multiple Bytes to I2C Device**

  ```c
  int i2c_write_bytes(I2CDevice_t i2c_device, uint32_t reg_addr, uint8_t *data, uint16_t length);
  ```

- **Read Multiple Bytes from I2C Device**

  ```c
  int i2c_read_bytes(I2CDevice_t i2c_device, uint32_t reg_addr, uint8_t *data, uint16_t length);
  ```

### Change I2C Device Configuration

- **Change I2C Device Frequency**

  ```c
  int i2c_device_change_freq(I2CDevice_t i2c_device, uint32_t freq);
  ```

- **Change I2C Device Timeout**

  ```c
  int i2c_device_change_timeout(I2CDevice_t i2c_device, int32_t timeout);
  ```

### Example Usage

```c
#include "i2c_device.h"

void app_main(void) {
    // Initialize I2C device, pins are invalid arguments
    I2CDevice_t i2cDevice = i2c_malloc_device(0, -1, -1, 100000, 0x3C);
    if (i2cDevice == NULL) {
        printf("Failed to initialize I2C device\n");
        return;
    }

    // Write byte to device
    uint8_t data_to_write = 0x01;
    if (i2c_write_byte(i2cDevice, 0x00, data_to_write) != 0) {
        printf("Failed to write byte\n");
    }

    // Read byte from device
    uint8_t data_read = 0;
    if (i2c_read_byte(i2cDevice, 0x00, &data_read) != 0) {
        printf("Failed to read byte\n");
    }

}
```

> **_NOTE:_** Ensure the SDA and SCL pins are properly configured for multiplexing before use.