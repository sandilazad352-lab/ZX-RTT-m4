#include "pwm.h"
#include "pwm_hal.h"

typedef struct {
    uint32_t period_ns;
    uint32_t sig_duty_ns[PWN_SIG_MAX];
} pwm_time_ns_t;


static pwm_config_t g_pwm_config[PWM_CH_MAX];
static pwm_time_ns_t g_pwm_time_ns[PWM_CH_MAX];

static int pwm_set_duty_ns(pwm_channel_t channel, pwm_signal_num_t sig, uint32_t duty_ns);
static int pwm_set_period_ns(pwm_channel_t channel, uint32_t period_ns);

int pwm_signal_update_config(pwm_channel_t channel, pwm_signal_num_t sig, pwm_signal_t* sig_config) {
    pwm_set_default_level(channel, sig, sig_config->default_level);
    pwm_set_duty(channel, sig, sig_config->duty);
    pwm_enable(channel, sig, sig_config->enable);
    return 0;
}

int pwm_init_config(pwm_config_t* config) {
    static uint8_t _pwm_hal_init = 0;
    if (!_pwm_hal_init) {
        pwm_hal_init();
        _pwm_hal_init = 1;
    }

    if (config->channel >= PWM_CH_MAX) {
        return -1;
    }

    g_pwm_config[config->channel] = *config;
    
    pwm_hal_ch_init(config->channel, PWM_MODE_UP_COUNT);
    int ret = pwm_set_freq(config->channel, config->freq);
    if (ret != 0) {
        return ret;
    }
    ret = pwm_signal_update_config(config->channel, PWN_SIG_A, &config->signal[0]);
    ret |= pwm_signal_update_config(config->channel, PWN_SIG_B, &config->signal[1]);
    if (ret != 0) {
        return ret;
    }

    pwm_hal_enable(config->channel, config->enable);
    return 0;
}

int pwm_enable(pwm_channel_t channel, pwm_signal_num_t sig, uint8_t enable) {
    if (channel >= PWM_CH_MAX) {
        return -1;
    }

    if (sig >= PWN_SIG_MAX) {
        return -1 ;
    }
    g_pwm_config[channel].signal[sig].enable = enable;
    pwm_hal_signal_enable(channel, sig, enable);
    return 0;
}

int pwm_set_duty(pwm_channel_t channel, pwm_signal_num_t sig, float duty) {
    if (channel >= PWM_CH_MAX) {
        return -1;
    }
    if (sig >= PWN_SIG_MAX) {
        return -1 ;
    }
    if (duty > 100.0) {
        duty = 100;
    }
    uint32_t duty_ns = duty * g_pwm_time_ns[channel].period_ns / 100;
    int ret = pwm_set_duty_ns(channel, sig, duty_ns);
    if (ret == 0) {
        g_pwm_config[channel].signal[sig].duty = duty;
    }
    return 0;
}

int pwm_set_default_level(pwm_channel_t channel, pwm_signal_num_t sig, uint8_t default_level) {
    if (channel >= PWM_CH_MAX) {
        return -1;
    }
    if (sig >= PWN_SIG_MAX) {
        return -1 ;
    }
    pwm_hal_signal_set_default_level(channel, sig, default_level);
    g_pwm_config[channel].signal[sig].default_level = default_level;
    return 0;
}

int pwm_set_freq(pwm_channel_t channel, uint32_t freq) {
    if (channel >= PWM_CH_MAX) {
        return -1;
    }
    uint32_t period_ns = 1000000000 / freq;
    int ret = pwm_set_period_ns(channel, period_ns);
    if (ret == 0) {
        g_pwm_config[channel].freq = freq;
    }
    return 0;
}

static int pwm_set_duty_ns(pwm_channel_t channel, pwm_signal_num_t sig, uint32_t duty_ns) {
    if (channel >= PWM_CH_MAX) {
        return -1;
    }
    if (sig >= PWN_SIG_MAX) {
        return -1 ;
    }

    if (duty_ns > g_pwm_time_ns[channel].period_ns) {
        duty_ns = g_pwm_time_ns[channel].period_ns;
    }
    int ret = pwm_hal_signal_set_duty_ns(channel, sig, duty_ns);
    if (ret == 0) {
        g_pwm_time_ns[channel].sig_duty_ns[sig] = duty_ns;
    }
    return ret;
}

static int pwm_set_period_ns(pwm_channel_t channel, uint32_t period_ns) {
    if (channel >= PWM_CH_MAX) {
        return -1;
    }
    uint32_t duty_a_ns = g_pwm_config[channel].signal[0].duty * period_ns / 100;
    uint32_t duty_b_ns = g_pwm_config[channel].signal[1].duty * period_ns / 100;
    int ret = pwm_hal_set_period_ns(channel, period_ns);
    if (ret == 0) {
        pwm_hal_signal_set_duty_ns(channel, PWN_SIG_A, duty_a_ns);
        pwm_hal_signal_set_duty_ns(channel, PWN_SIG_B, duty_b_ns);
        g_pwm_time_ns[channel].sig_duty_ns[0] = duty_a_ns;
        g_pwm_time_ns[channel].sig_duty_ns[1] = duty_b_ns;
        g_pwm_time_ns[channel].period_ns = period_ns;
    }
    return ret;
}
