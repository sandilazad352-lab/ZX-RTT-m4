
# GPIO 驱动文档

## 简介

GPIO（通用输入/输出）驱动提供了一组控制微控制器上GPIO引脚的函数。这些功能允许设置引脚模式、读取和写入引脚电平、配置上拉/下拉电阻、设置驱动强度和处理GPIO中断。

## API 参考

### GPIO 模式

- **GPIO_MODE_DISABLE**: 禁用输入/输出。
- **GPIO_MODE_INPUT**: 仅输入模式。
- **GPIO_MODE_OUTPUT**: 仅输出模式。
- **GPIO_MODE_INPUT_OUTPUT**: 输入和输出模式。

### GPIO 上拉/下拉模式

- **GPIO_PULLUP**: 启用内部上拉电阻。
- **GPIO_PULLDOWN**: 启用内部下拉电阻。
- **GPIO_FLOATING**: 无上拉或下拉电阻。

### GPIO 驱动强度选项

- **GPIO_DRV_180_OHM** 至 **GPIO_DRV_23_OHM**: 指定从180欧姆到23欧姆的驱动强度。

### GPIO 中断模式

- **GPIO_IRQ_MODE_FALLING_EDGE**: 下降沿触发中断。
- **GPIO_IRQ_MODE_RISING_EDGE**: 上升沿触发中断。
- **GPIO_IRQ_MODE_LOW_LEVEL**: 低电平触发中断。
- **GPIO_IRQ_MODE_HIGH_LEVEL**: 高电平触发中断。
- **GPIO_IRQ_MODE_BOTH_EDGE**: 双边沿触发中断。

### 函数

- `int gpio_set_level(pin_name_t pin, uint32_t level)`: 设置GPIO引脚的输出电平。
- `int gpio_toggle_level(pin_name_t pin)`: 切换GPIO引脚的输出电平。
- `int gpio_get_level(pin_name_t pin)`: 读取GPIO引脚的当前电平。
- `int gpio_set_direction(pin_name_t pin, gpio_mode_t mode)`: 设置GPIO引脚的方向。
- `int gpio_set_pull_mode(pin_name_t pin, gpio_pull_mode_t pull)`: 设置GPIO引脚的上拉/下拉模式。
- `int gpio_set_drive_strength(pin_name_t pin, gpio_drv_strength_t strength)`: 设置GPIO引脚的驱动强度。
- `int gpio_irq_enable(pin_name_t pin, uint8_t enable)`: 为GPIO引脚启用或禁用中断。
- `int gpio_irq_attach(pin_name_t pin, gpio_irq_mode_t mode, gpio_irq_handler_t handler, void* arg)`: 将中断处理程序附加到GPIO引脚。

## 使用

在项目中包含`gpio.h`并确保构建系统包括对头文件和源文件的路径。

## 示例用法

### 基本GPIO输出

```c
#include "gpio.h"

int main() {
    // 初始化GPIO引脚为GPIO功能
    gpio_set_func_as_gpio(PE0);
    
    // 设置GPIO引脚方向为输出
    gpio_set_direction(PE0, GPIO_MODE_OUTPUT);
    
    // 将GPIO引脚设置为高电平
    gpio_set_level(PE0, 1);
    
    return 0;
}
```

### 读取GPIO输入

```c
#include "gpio.h"

int main() {
    // 初始化GPIO引脚为GPIO功能
    gpio_set_func_as_gpio(PE0);
    
    // 设置GPIO引脚方向为输入
    gpio_set_direction(PE0, GPIO_MODE_INPUT);
    
    // 读取GPIO引脚电平
    int level = gpio_get_level(PE0);
    
    return 0;
}
```

### 中断处理

```c
#include "gpio.h"

// 中断处理函数
void gpio_irq_handler(void *arg) {
    // 处理中断
}

int main() {
    // 初始化GPIO引脚为GPIO功能
    gpio_set_func_as_gpio(PE0);
    
    // 为GPIO引脚启用中断
    gpio_irq_enable(PE0, 1);

    // 设置GPIO引脚方向为输入
    gpio_set_direction(PE0, GPIO_MODE_INPUT);
    
    // 附加中断处理程序
    gpio_irq_attach(PE0, GPIO_IRQ_MODE_RISING_EDGE, gpio_irq_handler, NULL);
    
    return 0;
}
```
