/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <console.h>
#include <getopt.h>
#include "gpai.h" // Include the GPAI driver header file.
#include "gpio.h" // Include GPIO driver header file for gpio_set_func.

static void cmd_gpai_usage(void)
{
    printf("Compile time: %s %s\n", __DATE__, __TIME__);
    printf("Usage: gpai_test [options]\n");
    printf("gpai_test enable <CH> <MODE> <PERIOD_MS> : Enable GPAI channel CH with MODE and sampling period PERIOD_MS.\n");
    printf("gpai_test disable <CH>                   : Disable GPAI channel CH.\n");
    printf("gpai_test read <CH>                      : Read millivolt value from GPAI channel CH.\n");
    printf("gpai_test help                           : Get this help.\n");
    printf("\nExample: gpai_test enable 1 single\n");
    printf("\nExample: gpai_test enable 1 period 500\n");
}

static int cmd_gpai_enable(int argc, char **argv)
{
    uint32_t ch = (uint32_t)atoi(argv[1]);
    enum aic_gpai_mode mode = (strcmp(argv[2], "single") == 0) ? AIC_GPAI_MODE_SINGLE : AIC_GPAI_MODE_PERIOD;
    uint32_t period_ms = 0;
    if (mode == AIC_GPAI_MODE_SINGLE) {
        period_ms = 0;
    } else {
        period_ms = (uint32_t)atoi(argv[3]);
    }

    if (gpai_enabled(ch, 1, mode, period_ms) != 0) {
        printf("Failed to enable GPAI channel %ld.\n", ch);
    } else {
        printf("GPAI channel %ld enabled with mode %s and period %ld ms.\n", ch, argv[2], period_ms);
    }

    return 0;
}

static int cmd_gpai_disable(int argc, char **argv)
{
    uint32_t ch = (uint32_t)atoi(argv[1]);

    if (gpai_enabled(ch, 0, AIC_GPAI_MODE_SINGLE, 0) != 0) {
        printf("Failed to disable GPAI channel %ld.\n", ch);
    } else {
        printf("GPAI channel %ld disabled.\n", ch);
    }

    return 0;
}

static int cmd_gpai_read(int argc, char **argv)
{
    uint32_t ch = (uint32_t)atoi(argv[1]);
    int mv = gpai_read_mv(ch);

    if (mv == 0) {
        printf("Failed to read or channel %ld not available.\n", ch);
    } else {
        printf("Read %d mV from GPAI channel %ld.\n", mv, ch);
    }

    return 0;
}

static int cmd_gpai_test(int argc, char *argv[])
{
    // Automatically call gpai_init at the start of the command.
    if (gpai_init() != 0) {
        printf("GPAI system initialization failed.\n");
        return -1;
    }

    if (argc < 2) {
        cmd_gpai_usage();
        return 0;
    }

    if (!strcmp(argv[1], "enable")) {
        if (argc != 5 || argc != 4) {
            printf("Invalid number of arguments for enable command.\n");
            return -1;
        }
        return cmd_gpai_enable(argc - 1, &argv[1]);
    }

    if (!strcmp(argv[1], "disable")) {
        if (argc != 3) {
            printf("Invalid number of arguments for disable command.\n");
            return -1;
        }
        return cmd_gpai_disable(argc - 1, &argv[1]);
    }

    if (!strcmp(argv[1], "read")) {
        if (argc != 3) {
            printf("Invalid number of arguments for read command.\n");
            return -1;
        }
        return cmd_gpai_read(argc - 1, &argv[1]);
    }

    cmd_gpai_usage();
    return 0;
}
CONSOLE_CMD(gpai_test, cmd_gpai_test, "GPAI test commands");
