# I2C设备库使用文档

## 简介

I2C设备库提供了一系列接口，用于配置和通信I2C设备。它支持多种I2C操作，包括读写字节、位和多字节数据，同时支持频率设置的调整。

## 配置和API说明

### 初始化I2C设备

在使用I2C设备进行通信之前，首先需要通过`i2c_malloc_device`分配并初始化I2C设备对象。

```c
I2CDevice_t i2c_malloc_device(int i2c_num, int8_t sda, int8_t scl, uint32_t freq, uint8_t device_addr);
```

### 读写操作

- **写入字节到I2C设备**

  ```c
  int i2c_write_byte(I2CDevice_t i2c_device, uint32_t reg_addr, uint8_t data);
  ```

- **从I2C设备读取字节**

  ```c
  int i2c_read_byte(I2CDevice_t i2c_device, uint32_t reg_addr, uint8_t* data);
  ```

- **写入多字节到I2C设备**

  ```c
  int i2c_write_bytes(I2CDevice_t i2c_device, uint32_t reg_addr, uint8_t *data, uint16_t length);
  ```

- **从I2C设备读取多字节**

  ```c
  int i2c_read_bytes(I2CDevice_t i2c_device, uint32_t reg_addr, uint8_t *data, uint16_t length);
  ```

### 更改I2C设备配置

- **更改I2C设备频率**

  ```c
  int i2c_device_change_freq(I2CDevice_t i2c_device, uint32_t freq);
  ```

- **更改I2C设备超时**

  ```c
  int i2c_device_change_timeout(I2CDevice_t i2c_device, int32_t timeout);
  ```

### 示例用法

```c
#include "i2c_device.h"

void app_main(void) {
    // 初始化I2C设备， 引脚是无效参数
    I2CDevice_t i2cDevice = i2c_malloc_device(0, -1, -1, 100000, 0x3C);
    if (i2cDevice == NULL) {
        printf("I2C设备初始化失败\n");
        return;
    }

    // 写入字节到设备
    uint8_t data_to_write = 0x01;
    if (i2c_write_byte(i2cDevice, 0x00, data_to_write) != 0) {
        printf("写入字节失败\n");
    }

    // 从设备读取字节
    uint8_t data_read = 0;
    if (i2c_read_byte(i2cDevice, 0x00, &data_read) != 0) {
        printf("读取字节失败\n");
    }

}
```

> **_NOTE:_** 使用前请确保sda，scl引脚已经配置好复用
