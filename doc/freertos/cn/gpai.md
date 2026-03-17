# 通用模拟输入（GPAI）驱动使用文档

## 简介

GPAI（General Purpose Analog Input，通用模拟输入）驱动提供了一套API，允许用户配置和读取模拟输入通道的值。该驱动支持多通道输入，适用于需要模拟信号采集的应用场景。

## 配置和API说明

### 初始化GPAI系统

在使用任何其他GPAI功能之前，必须先调用以下函数来准备GPAI硬件：

```c
int gpai_init(void);
```

### 配置GPAI通道

可以通过以下函数启用或禁用指定的GPAI通道，并配置其采样模式和采样周期：

```c
int gpai_enabled(uint32_t ch, uint8_t enabled, enum aic_gpai_mode mode, uint32_t period_ms);
```

参数说明：

- `ch`：要配置的通道号。
- `enabled`：设置为1以启用通道，设置为0以禁用通道。
- `mode`：采样模式。
- `period_ms`：采样周期，以毫秒为单位。

### 读取模拟输入值

可以通过以下函数从指定的GPAI通道读取当前的模拟值，并将其转换为毫伏值：

```c
int gpai_read_mv(uint32_t ch);
```

参数说明：

- `ch`：要读取的通道号。

### 获取GPAI系统分辨率

通过以下函数获取GPAI系统的分辨率：

```c
int gpai_get_resolution();
```

## 示例用法

### 初始化GPAI

在程序开始时，首先初始化GPAI系统：

```c
#include "gpai.h"

int main() {
    gpai_init();
    // 之后可以配置GPAI通道和读取值
}
```

### 配置并读取GPAI通道

启用一个GPAI通道，并读取其模拟值：

```c
#include "gpai.h"

int main() {
    // 初始化GPAI系统
    gpai_init();
    
    // 启用通道0，设置为单次采样模式，采样周期为100ms
    gpai_enabled(0, 1, AIC_GPAI_MODE_SINGLE, 100);
    
    // 读取并打印通道0的模拟值（毫伏）
    int mv = gpai_read_mv(0);
    printf("Channel 0 reads %d mV\n", mv);
}
```

### 循环读取GPAI通道

在启用一个GPAI通道后，可以在一个循环中持续读取其模拟值：

```c
#include "gpai.h"
#include <unistd.h> // 为了使用sleep函数

int main() {
    // 初始化GPAI系统
    gpai_init();
    
    // 启用通道0，设置为连续采样模式，采样周期为100ms
    gpai_enabled(0, 1, AIC_GPAI_MODE_CONTINUOUS, 100);
    
    // 持续读取并打印通道0的模拟值（毫伏）
    while(1) {
        int mv = gpai_read_mv(0);
        printf("Channel 0 reads %d mV\n", mv);
        vTaskDelay(pdMS_TO_TICKS(100)); // 每100ms读取一次
    }
}
```

> **_NOTE:_** 使用前请确保GPAI引脚已经配置好复用
