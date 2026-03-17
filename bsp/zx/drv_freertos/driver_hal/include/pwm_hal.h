/*
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */


// ToDo: not true hal driver

#ifndef _ZX_PWM_HAL_H_
#define _ZX_PWM_HAL_H_

#include "aic_common.h"

#define AIC_PWM_NAME        "aic-pwm"

#ifdef CONFIG_FPGA_BOARD_ZX
#define PWM_CLK_RATE        24000000 /* 24 MHz */
#else
#define PWM_CLK_RATE        48000000 /* 48 MHz */
#endif
#define PWM_TB_CLK_RATE     24000000 /* 24 MHz */

enum pwm_polarity {
    PWM_POLARITY_NORMAL,
    PWM_POLARITY_INVERSED,
};

enum aic_pwm_mode {
    PWM_MODE_UP_COUNT = 0,
    PWM_MODE_DOWN_COUNT,
    PWM_MODE_UP_DOWN_COUNT,
    PWM_MODE_NUM
};

enum aic_pwm_action_type {
    PWM_ACT_NONE = 0,
    PWM_ACT_LOW,
    PWM_ACT_HIGH,
    PWM_ACT_INVERSE,
    PWM_ACT_NUM
};

struct aic_pwm_arg {
    u16 available;
    u16 id;
    enum aic_pwm_mode mode;
    uint32_t tb_clk_rate;
    uint32_t freq;
    uint32_t period;
    uint32_t period_ns;
    uint32_t duty;
    s32 def_level;
    enum pwm_polarity polarity;
};

enum aic_pwm_int_event {
    PWM_CMPA_UP = 0,
    PWM_CMPA_DOWN,
    PWM_CMPB_UP,
    PWM_CMPB_DOWN
};

int pwm_hal_ch_init(uint32_t ch, enum aic_pwm_mode mode);

int pwm_hal_set_polarity(uint32_t ch, enum pwm_polarity polarity);

int pwm_hal_enable(uint32_t ch, uint8_t enable);

uint32_t pwm_hal_int_sts(void);

void pwm_hal_clr_int(uint32_t stat);

void pwm_hal_int_config(uint32_t ch, u8 irq_mode, u8 enable);

int pwm_hal_init(void);

int pwm_hal_deinit(void);


/**
 * @brief Enables or disables a specific signal for a given PWM channel.
 *
 * This function allows you to enable or disable a specific signal for a given PWM channel.
 *
 * @param ch The PWM channel number.
 * @param signal The signal number to enable or disable.
 * @param enable Set to 1 to enable the signal, or 0 to disable it.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int pwm_hal_signal_enable(uint32_t ch, uint8_t signal, uint8_t enable);

/**
 * Sets the duty cycle of a specific signal in nanoseconds for a given PWM channel.
 *
 * @param ch The PWM channel number.
 * @param signal The signal number within the PWM channel.
 * @param duty_ns The duty cycle in nanoseconds.
 * @return 0 on success, or a negative error code on failure.
 */
int pwm_hal_signal_set_duty_ns(uint32_t ch, uint8_t signal, uint32_t duty_ns);

/**
 * Sets the default level for a specific signal of a PWM channel.
 *
 * @param ch The PWM channel number.
 * @param signal The signal number.
 * @param level The default level to be set for the signal.
 * @return 0 on success, or a negative error code on failure.
 */
int pwm_hal_signal_set_default_level(uint32_t ch, uint32_t signal, uint32_t level);

/**
 * @brief Sets the period of the PWM signal in nanoseconds for a specific channel.
 *
 * This function sets the period of the PWM signal in nanoseconds for the specified channel.
 *
 * @param ch The channel number of the PWM signal.
 * @param ns The period of the PWM signal in nanoseconds.
 * @return 0 on success, or a negative error code on failure.
 */
int pwm_hal_set_period_ns(uint32_t ch, uint32_t ns);

#endif // end of _ZX_PWM_HAL_H_
