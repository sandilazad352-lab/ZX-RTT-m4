# PWM Driver Documentation

## Introduction

The PWM (Pulse Width Modulation) driver enables users to configure and control PWM channels to generate pulses of varying frequency and duty cycle. This driver supports multiple PWM channels and signals, offering flexible configuration options, including settings for frequency, duty cycle, and default level.

## Configuration Types

### `pwm_channel_t`

Defines PWM channel numbers:

- `PWM_CH_0`: PWM channel 0
- `PWM_CH_1`: PWM channel 1
- `PWM_CH_2`: PWM channel 2
- `PWM_CH_3`: PWM channel 3
- `PWM_CH_MAX`: Maximum number of PWM channels

### `pwm_signal_num_t`

Defines PWM signal numbers:

- `PWN_SIG_A`: PWM signal A
- `PWN_SIG_B`: PWM signal B
- `PWN_SIG_MAX`: Maximum number of PWM signals

### `pwm_signal_t`

Defines the configuration for a PWM signal, including the default level, enable status, and duty cycle.

### `pwm_config_t`

Defines the configuration for a specific channel, including frequency, signal configuration, and channel enable status.

## API Functions

### Initialization Configuration

- `int pwm_init_config(pwm_config_t* config)`: Initializes PWM configuration.

### Channel Control

- `int pwm_enable(pwm_channel_t channel, pwm_signal_num_t sig, uint8_t enable)`: Enables or disables a PWM channel.
- `int pwm_set_duty(pwm_channel_t channel, pwm_signal_num_t sig, float duty)`: Sets the duty cycle for a PWM channel.
- `int pwm_set_freq(pwm_channel_t channel, uint32_t freq)`: Sets the frequency for a PWM channel.
- `int pwm_set_default_level(pwm_channel_t channel, pwm_signal_num_t sig, uint8_t default_level)`: Sets the default level for a specific PWM signal.

## Example Usage

### Initialize PWM Configuration

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

### Set Duty Cycle

```c
#include "pwm.h"

void setDutyCycle() {
    pwm_set_duty(PWM_CH_0, PWN_SIG_A, 75.0); // Set the duty cycle of PWN_SIG_A signal on PWM_CH_0 to 75%
}
```

### Change Frequency

```c
#include "pwm.h"

void changeFrequency() {
    pwm_set_freq(PWM_CH_0, 2000); // Set the frequency of PWM_CH_0 to 2 kHz
}
```

> **_NOTE:_** Ensure the PWM pins are properly configured for multiplexing before use.