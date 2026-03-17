# SDK + FreeRTOS 开发指南

## FreeRTOS 简介

[FreeRTOS](https://www.freertos.org/) 是一个市场领先的实时操作系统（RTOS）为微控制器提供小型，低功耗的解决方案。它非常适合用于低成本和低处理能力的设备，如IoT设备、可穿戴设备和嵌入式系统。

### 默认配置

- **版本号**：V10.4.3 LTS Patch 3
- **Heap管理方式**：Heap_5
- **系统时钟频率**：500Hz
- **调度策略**：抢占式调度

### 内存申请

- 使用`pvPortMalloc`函数进行动态内存分配，分配的内存位于SRAM区域。

## SDK 基于 FreeRTOS 的驱动支持

### HAL 层驱动

| 驱动类型 | 说明     |
|----------|----------|
| GPIO     | 通用输入输出端口 |
| CAN      | 控制器局域网络 |
| GPAI     | 通用并行AI接口 |
| PWM      | 脉冲宽度调制 |
| I2C      | 线程安全的串行总线 |
| UART     | 线程安全的串行通讯 |

### Bare 层驱动

- **Audio**：音频播放和录制
- **EFuse**：电子保险丝，用于存储设备配置
- **HRTimer**：高分辨率定时器
- 更多驱动详见`drv_bare`文件夹。

### 其他支持

- **Button**：支持ADC Button和GPIO Button。
- **Touch**：支持多种触摸屏控制器，如GT911和FT5X06等。

## 工程开始

1. **查看配置文件**

   执行`Scons --list-def`查看所有可用的配置文件。

2. **选择对应的 FreeRTOS 配置**

   使用`Scons --apply-def=ZXM37D0_freertos_defconfig`选择适合的FreeRTOS配置。

3. **编译**

   执行`Scons`开始编译项目。

