/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <console.h>
#include "pwm.h"

static uint8_t g_init = 0;

static void cmd_pwm_usage(void)
{
    printf("Usage: pwm_test [command] [options]\n");
    printf("Commands:\n");
    printf("  init <channel> <frequency>                   - Initialize PWM channel with frequency\n");
    printf("  set_freq <channel> <frequency>                - Set frequency for PWM channel\n");
    printf("  set_duty <channel> <signal> <duty>            - Set duty cycle for PWM channel and signal\n");
    printf("  enable <channel> <signal> <enable>            - Enable/disable PWM channel and signal\n");
    printf("  set_default_level <channel> <signal> <level>  - Set default level for PWM signal\n");
    printf("\n");
    printf("channel: 0-3, signal: A=0, B=1, enable: 0=disable, 1=enable, level: 0=low, 1=high\n");
    printf("Example: pwm_test init 1 5000                  - Initializes channel 1 with 5000 Hz, default settings\n");
}

static int cmd_pwm_init(int argc, char **argv)
{
    if (argc != 3) {
        printf("Invalid number of arguments for init.\n");
        return -1;
    }

    pwm_config_t config;
    pwm_channel_t channel = atoi(argv[1]);
    uint32_t freq = atoi(argv[2]);

    // Initialize the structure with default settings
    config.channel = channel;
    config.freq = freq;
    config.enable = 1; // Enable the channel
    for (int i = 0; i < PWN_SIG_MAX; ++i) {
        config.signal[i].duty = 0.0; // Default duty cycle
        config.signal[i].enable = 0; // Disabled by default
        config.signal[i].default_level = 1; // Default level high
    }

    if (pwm_init_config(&config) != 0) {
        printf("Failed to initialize PWM channel %d.\n", channel);
    } else {
        printf("PWM channel %d initialized with frequency %ld Hz.\n", channel, freq);
    }
    g_init = 1;
    return 0;
}

static int cmd_pwm_set_freq(int argc, char **argv)
{
    if (argc != 3) {
        printf("Invalid number of arguments for set_freq.\n");
        return -1;
    }
    pwm_channel_t channel = atoi(argv[1]);
    uint32_t freq = atoi(argv[2]);

    if (pwm_set_freq(channel, freq) != 0) {
        printf("Failed to set frequency for channel %d.\n", channel);
    } else {
        printf("Frequency for channel %d set to %ld Hz.\n", channel, freq);
    }

    return 0;
}

static int cmd_pwm_set_duty(int argc, char **argv)
{
    if (argc != 4) {
        printf("Invalid number of arguments for set_duty.\n");
        return -1;
    }

    pwm_channel_t channel = atoi(argv[1]);
    pwm_signal_num_t signal = atoi(argv[2]);
    float duty = atof(argv[3]);

    if (pwm_set_duty(channel, signal, duty) != 0) {
        printf("Failed to set duty cycle for channel %d, signal %d.\n", channel, signal);
    } else {
        printf("Duty cycle for channel %d, signal %d set to %.2f%%.\n", channel, signal, duty);
    }

    return 0;
}

static int cmd_pwm_enable(int argc, char **argv)
{
    if (argc != 4) {
        printf("Invalid number of arguments for enable.\n");
        return -1;
    }

    pwm_channel_t channel = atoi(argv[1]);
    pwm_signal_num_t signal = atoi(argv[2]);
    uint8_t enable = atoi(argv[3]);

    if (pwm_enable(channel, signal, enable) != 0) {
        printf("Failed to %s channel %d, signal %d.\n", enable ? "enable" : "disable", channel, signal);
    } else {
        printf("Channel %d, signal %d %s.\n", channel, signal, enable ? "enabled" : "disabled");
    }

    return 0;
}

static int cmd_pwm_set_default_level(int argc, char **argv)
{
    if (argc != 4) {
        printf("Invalid number of arguments for set_default_level.\n");
        return -1;
    }

    pwm_channel_t channel = atoi(argv[1]);
    pwm_signal_num_t signal = atoi(argv[2]);
    uint8_t default_level = atoi(argv[3]);

    if (pwm_set_default_level(channel, signal, default_level) != 0) {
        printf("Failed to set default level for channel %d, signal %d.\n", channel, signal);
    } else {
        printf("Default level for channel %d, signal %d set to %d.\n", channel, signal, default_level);
    }

    return 0;
}

static int cmd_pwm_test(int argc, char *argv[])
{
    if (argc < 2) {
        cmd_pwm_usage();
        return 0;
    }
    if (strcmp(argv[1], "init") == 0) {
        return cmd_pwm_init(argc - 1, &argv[1]);
    }
    
    if (g_init == 0) {
        printf("Pls Init first\r\n");
        return -1;
    }
    if (strcmp(argv[1], "set_freq") == 0) {
        return cmd_pwm_set_freq(argc - 1, &argv[1]);
    } else if (strcmp(argv[1], "set_duty") == 0) {
        return cmd_pwm_set_duty(argc - 1, &argv[1]);
    } else if (strcmp(argv[1], "enable") == 0) {
        return cmd_pwm_enable(argc - 1, &argv[1]);
    } else if (strcmp(argv[1], "set_default_level") == 0) {
        return cmd_pwm_set_default_level(argc - 1, &argv[1]);
    }

    cmd_pwm_usage();
    return -1;
}
CONSOLE_CMD(pwm_test, cmd_pwm_test, "PWM test commands");
