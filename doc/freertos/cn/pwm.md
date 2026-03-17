# PWM 驱动使用文档

## 简介

PWM（脉冲宽度调制）驱动允许用户配置和控制PWM通道，以生成不同频率和占空比的脉冲。该驱动支持多个PWM通道和信号，提供了灵活的配置选项，包括频率、占空比和默认电平的设置。

## 配置类型

### `pwm_channel_t`

定义PWM通道号：

- `PWM_CH_0`：PWM通道0
- `PWM_CH_1`：PWM通道1
- `PWM_CH_2`：PWM通道2
- `PWM_CH_3`：PWM通道3
- `PWM_CH_MAX`：最大PWM通道数

### `pwm_signal_num_t`

定义PWM信号编号：

- `PWN_SIG_A`：PWM信号A
- `PWN_SIG_B`：PWM信号B
- `PWN_SIG_MAX`：最大PWM信号数

### `pwm_signal_t`

定义PWM信号配置，包括默认电平、使能状态和占空比。

### `pwm_config_t`

定义特定通道的PWM配置，包括频率、信号配置和通道使能状态。

## API 函数

### 初始化配置

- `int pwm_init_config(pwm_config_t* config)`：初始化PWM配置。

### 通道控制

- `int pwm_enable(pwm_channel_t channel, pwm_signal_num_t sig, uint8_t enable)`：启用或禁用PWM通道。
- `int pwm_set_duty(pwm_channel_t channel, pwm_signal_num_t sig, float duty)`：设置PWM通道的占空比。
- `int pwm_set_freq(pwm_channel_t channel, uint32_t freq)`：设置PWM通道的频率。
- `int pwm_set_default_level(pwm_channel_t channel, pwm_signal_num_t sig, uint8_t default_level)`：设置特定PWM信号的默认电平。

## 示例用法

### 初始化PWM配置

```c
#include "pwm.h"

int main() {
    pwm_config_t pwmConfig = {
        .channel = PWM_CH_0,
        .freq = 1000, // 1 kHz
        .signal = {
            {.default_level = 0, .enable = 1, .duty = 50.0},
            {.default_level = 0, .enable = 0, .duty = 0.0}
        },
        .enable = 1
    };

    pwm_init_config(&pwmConfig);

    return 0;
}
```

### 设置占空比

```c
#include "pwm.h"

void setDutyCycle() {
    pwm_set_duty(PWM_CH_0, PWN_SIG_A, 75.0); // 设置PWM_CH_0的PWN_SIG_A信号占空比为75%
}
```

### 修改频率

```c
#include "pwm.h"

void changeFrequency() {
    pwm_set_freq(PWM_CH_0, 2000); // 将PWM_CH_0的频率设置为2 kHz
}
```

> **_NOTE:_** 使用前请确保PWM引脚已经配置好复用
