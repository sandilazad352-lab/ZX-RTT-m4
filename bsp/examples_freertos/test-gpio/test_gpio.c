/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <console.h>
#include <getopt.h>
#include "gpio.h" // Make sure this matches the name of your GPIO driver header file.

#define TEST_PIN_DEFAULT ("PD.15") // Default test pin.

static void cmd_gpio_usage(void)
{
    printf("Compile time: %s %s\n", __DATE__, __TIME__);
    printf("Usage: gpio_test [options]\n");
    printf("gpio_test dir <PIN> <MODE>       : Set direction of PIN as MODE. MODE can be 'input' or 'output'.\n");
    printf("gpio_test pull <PIN> <MODE>      : Set pull mode of PIN. MODE can be 'up', 'down', or 'floating'.\n");
    printf("gpio_test read <PIN>             : Read current level of PIN.\n");
    printf("gpio_test write <PIN> <LEVEL>    : Set level of PIN. LEVEL can be '0' or '1'.\n");
    printf("gpio_test help                   : Get this help.\n");
    printf("\nExample: gpio_test dir PD.4 output\n");
}

static pin_name_t pin_name_parser(char *arg_pin)
{
    pin_name_t pin;
    if (arg_pin == NULL || strlen(arg_pin) == 0) {
        printf("PIN set default PD.15\n");
        pin = hal_gpio_name2pin(TEST_PIN_DEFAULT);
    } else {
        pin = hal_gpio_name2pin(arg_pin);
    }

    return pin;
}

static gpio_mode_t mode_parser(char *arg_mode)
{
    if (strcmp(arg_mode, "input") == 0) {
        return GPIO_MODE_INPUT;
    } else if (strcmp(arg_mode, "output") == 0) {
        return GPIO_MODE_OUTPUT;
    } else {
        printf("Invalid mode. Using default 'input'.\n");
        return GPIO_MODE_INPUT;
    }
}

static gpio_pull_mode_t pull_mode_parser(char *arg_pull)
{
    if (strcmp(arg_pull, "up") == 0) {
        return GPIO_PULLUP;
    } else if (strcmp(arg_pull, "down") == 0) {
        return GPIO_PULLDOWN;
    } else if (strcmp(arg_pull, "floating") == 0) {
        return GPIO_FLOATING;
    } else {
        printf("Invalid pull mode. Using default 'floating'.\n");
        return GPIO_FLOATING;
    }
}

static int cmd_gpio_direction(int argc, char **argv)
{
    pin_name_t pin = pin_name_parser(argv[1]);
    gpio_mode_t mode = mode_parser(argv[2]);

    gpio_set_func_as_gpio(pin);

    if (gpio_set_direction(pin, mode) != 0) {
        printf("Failed to set direction for PIN %s.\n", argv[1]);
    } else {
        printf("Direction for PIN %s set to %s.\n", argv[1], argv[2]);
    }

    return 0;
}

static int cmd_gpio_pull(int argc, char **argv)
{
    pin_name_t pin = pin_name_parser(argv[1]);
    gpio_pull_mode_t pull = pull_mode_parser(argv[2]);

    if (gpio_set_pull_mode(pin, pull) != 0) {
        printf("Failed to set pull mode for PIN %s.\n", argv[1]);
    } else {
        printf("Pull mode for PIN %s set to %s.\n", argv[1], argv[2]);
    }

    return 0;
}

static int cmd_gpio_read(int argc, char **argv)
{
    pin_name_t pin = pin_name_parser(argv[1]);
    int level;

    level = gpio_get_level(pin);
    if (level < 0) {
        printf("Failed to read level for PIN %s.\n", argv[1]);
    } else {
        printf("Current level for PIN %s is %d.\n", argv[1], level);
    }

    return 0;
}

static int cmd_gpio_write(int argc, char **argv)
{
    pin_name_t pin = pin_name_parser(argv[1]);
    uint32_t level = (uint32_t)atoi(argv[2]);

    if (gpio_set_level(pin, level) != 0) {
        printf("Failed to set level for PIN %s.\n", argv[1]);
    } else {
        printf("Level for PIN %s set to %ld.\n", argv[1], level);
    }

    return 0;
}

static int cmd_gpio_test(int argc, char *argv[])
{
    if (argc < 2) {
        cmd_gpio_usage();
        return 0;
    }

    if (!strcmp(argv[1], "dir")) {
        if (argc != 4) {
            printf("Invalid number of arguments for direction command.\n");
            return -1;
        }
        return cmd_gpio_direction(argc - 1, &argv[1]);
    }

    if (!strcmp(argv[1], "pull")) {
        if (argc != 4) {
            printf("Invalid number of arguments for pull command.\n");
            return -1;
        }
        return cmd_gpio_pull(argc - 1, &argv[1]);
    }

    if (!strcmp(argv[1], "read")) {
        if (argc != 3) {
            printf("Invalid number of arguments for read command.\n");
            return -1;
        }
        return cmd_gpio_read(argc - 1, &argv[1]);
    }

    if (!strcmp(argv[1], "write")) {
        if (argc != 4) {
            printf("Invalid number of arguments for write command.\n");
            return -1;
        }
        return cmd_gpio_write(argc - 1, &argv[1]);
    }

    cmd_gpio_usage();
    return 0;
}

CONSOLE_CMD(gpio_test, cmd_gpio_test, "GPIO test commands");
