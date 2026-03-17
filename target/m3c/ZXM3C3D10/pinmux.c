/*
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#include <aic_core.h>
#include <aic_hal.h>
#include "board.h"

struct aic_pinmux
{
    unsigned char       func;
    unsigned char       bias;
    unsigned char       drive;
    char *              name;
};

struct aic_pinmux aic_pinmux_config[] = {
#ifdef AIC_USING_UART0
    /* uart0 */
    {5, PIN_PULL_DIS, 3, "PA.0"},
    {5, PIN_PULL_UP, 3, "PA.1"},
#endif
#ifdef AIC_USING_UART1
    /* uart1 */
    {5, PIN_PULL_DIS, 3, "PA.2"},
    {5, PIN_PULL_DIS, 3, "PA.3"},
#endif
#ifdef AIC_USING_UART2
    /* uart2 */
    {5, PIN_PULL_DIS, 3, "PD.4"},
    {5, PIN_PULL_DIS, 3, "PD.5"},
#endif
#ifdef AIC_USING_UART3
    /* uart3 */
    {5, PIN_PULL_DIS, 3, "PB.10"},
    {5, PIN_PULL_DIS, 3, "PB.11"},
#endif
#ifdef AIC_USING_I2C0
    {4, PIN_PULL_DIS, 3, "PA.8"},  // SCK
    {4, PIN_PULL_DIS, 3, "PA.9"},  // SDA
#endif
#ifdef AIC_USING_I2C1
    {4, PIN_PULL_UP, 3, "PC.4"},  // SCK
    {4, PIN_PULL_UP, 3, "PC.5"},  // SDA
#endif
#ifdef AIC_USING_QSPI0
    /* qspi0 */
    {2, PIN_PULL_DIS, 3, "PB.0"},
    {2, PIN_PULL_DIS, 3, "PB.1"},
    {2, PIN_PULL_DIS, 3, "PB.2"},
    {2, PIN_PULL_DIS, 3, "PB.3"},
    {2, PIN_PULL_DIS, 3, "PB.4"},
    {2, PIN_PULL_DIS, 3, "PB.5"},
#endif
#ifdef AIC_PANEL_ENABLE_GPIO
    {1, PIN_PULL_DIS, 3, AIC_PANEL_ENABLE_GPIO},
#endif
#ifdef AIC_PANEL_DBI_ST77903
    /* spi 4 sda */
    //{6, PIN_PULL_DIS, 3, "PC.6"},   // DE_TE
    {2, PIN_PULL_DIS, 3, "PD.20"},
    {2, PIN_PULL_DIS, 3, "PD.21"},
    {2, PIN_PULL_DIS, 3, "PD.22"},
    {2, PIN_PULL_DIS, 3, "PD.23"},
    {2, PIN_PULL_DIS, 3, "PD.24"},
    {2, PIN_PULL_DIS, 3, "PD.26"},
    {2, PIN_PULL_DIS, 3, "PD.27"},
#endif

#ifdef AIC_USING_PWM0
    {3, PIN_PULL_DIS, 3, "PE.13"},
#endif
#ifdef AIC_USING_PWM1
    {3, PIN_PULL_DIS, 3, "PC.7"},
#endif
#ifdef AIC_USING_AUDIO
#ifdef AIC_AUDIO_PLAYBACK
    {5, PIN_PULL_DIS, 3, "PE.12"},
    {1, PIN_PULL_DIS, 3, AIC_AUDIO_PA_ENABLE_GPIO},
#endif
#endif

    {1, PIN_PULL_DIS, 3, "PD.16"},  // LCD_RESET
    {1, PIN_PULL_DIS, 3, "PC.2"},  // ESP_EN
};

void aic_board_pinmux_init(void)
{
    uint32_t i = 0;
    long pin = 0;
    unsigned int g;
    unsigned int p;

    for (i=0; i<ARRAY_SIZE(aic_pinmux_config); i++) {
        pin = hal_gpio_name2pin(aic_pinmux_config[i].name);
        if (pin < 0)
            continue;
        g = GPIO_GROUP(pin);
        p = GPIO_GROUP_PIN(pin);
        hal_gpio_set_func(g, p, aic_pinmux_config[i].func);
        hal_gpio_set_bias_pull(g, p, aic_pinmux_config[i].bias);
        hal_gpio_set_drive_strength(g, p, aic_pinmux_config[i].drive);
    }

    pin = hal_gpio_name2pin("PC.2");
    g = GPIO_GROUP(pin);
    p = GPIO_GROUP_PIN(pin);
    hal_gpio_direction_input(g, p);

    pin = hal_gpio_name2pin("PD.16");
    g = GPIO_GROUP(pin);
    p = GPIO_GROUP_PIN(pin);
    hal_gpio_direction_output(g, p);
#ifndef AIC_BOOTLOADER
    hal_gpio_set_output(g,p);
    aic_mdelay(20);
    hal_gpio_clr_output(g,p);
    aic_mdelay(120);
    hal_gpio_set_output(g,p);
#else
    hal_gpio_clr_output(g,p);
#endif
}
