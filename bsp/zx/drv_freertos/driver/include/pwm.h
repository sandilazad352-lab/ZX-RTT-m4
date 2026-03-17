#ifndef __ZX_PWM_DRIVER_H__
#define __ZX_PWM_DRIVER_H__

#include "stdio.h"

/**
 * @enum pwm_channel_t
 * @brief Defines PWM channel numbers.
 */
typedef enum {
    PWM_CH_0 = 0x00, /**< PWM channel 0 */
    PWM_CH_1,        /**< PWM channel 1 */
    PWM_CH_2,        /**< PWM channel 2 */
    PWM_CH_3,        /**< PWM channel 3 */
    PWM_CH_MAX       /**< Maximum PWM channels */
} pwm_channel_t;

/**
 * @enum pwm_signal_num_t
 * @brief Defines PWM signal numbers.
 */
typedef enum {
    PWN_SIG_A = 0x00, /**< PWM signal A */
    PWN_SIG_B,        /**< PWM signal B */
    PWN_SIG_MAX,      /**< Maximum PWM signals */
} pwm_signal_num_t;

/**
 * @struct pwm_signal_t
 * @brief Defines PWM signal configuration.
 */
typedef struct {
    uint8_t default_level; /**< PWM signal output default level */
    uint8_t enable;        /**< Enables the PWM signal if set to 1 */
    float duty;            /**< Duty cycle of the PWM signal, range: 0.0 to 100.0 */
} pwm_signal_t ;

/**
 * @struct pwm_config_t
 * @brief Defines PWM configuration for a specific channel.
 */
typedef struct {
    pwm_channel_t channel;             /**< PWM channel */
    uint32_t freq;                     /**< Frequency of PWM signal */
    pwm_signal_t signal[PWN_SIG_MAX];  /**< PWM signal configurations */
    uint8_t enable;                    /**< PWM Channel enable*/
} pwm_config_t;

/**
 * @brief Initializes PWM configuration.
 * @param config Pointer to PWM configuration struct.
 * @return 0 if successful, -1 on failure.
 */
int pwm_init_config(pwm_config_t* config);

/**
 * @brief Enables or disables a PWM channel.
 * @param channel PWM channel to configure.
 * @param sig PWM signal to configure.
 * @param enable 1 to enable, 0 to disable.
 * @return 0 if successful, -1 on failure.
 */
int pwm_enable(pwm_channel_t channel, pwm_signal_num_t sig, uint8_t enable);

/**
 * @brief Sets the duty cycle for a PWM channel.
 * @param channel PWM channel to configure.
 * @param sig PWM signal to configure.
 * @param duty Duty cycle value (range: 0.0 to 100.0).
 * @return 0 if successful, -1 on failure.
 */
int pwm_set_duty(pwm_channel_t channel, pwm_signal_num_t sig, float duty);

/**
 * @brief Sets the frequency for a PWM channel.
 * @param channel PWM channel to configure.
 * @param freq Frequency in Hertz.
 * @return 0 if successful, -1 on failure.
 */
int pwm_set_freq(pwm_channel_t channel, uint32_t freq);

/**
 * @brief Sets the default level for a specific PWM signal on a given channel.
 *
 * @param channel The PWM channel to set the default level for.
 * @param sig The PWM signal number to set the default level for.
 * @param default_level The default level to set for the PWM signal (0-1).
 * @return Returns 0 on success, or a negative error code on failure.
 */
int pwm_set_default_level(pwm_channel_t channel, pwm_signal_num_t sig, uint8_t default_level);

#endif
