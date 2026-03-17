/*
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <rtthread.h>
#include "zx_gui.h"
#include <stdio.h>
#include "string.h"
#include <aic_core.h>
#include <aic_hal.h>

#ifdef LPKG_USING_ZX_BUTTON
#include "zx_button.h"

#define KEY1_GPIO_NAME    "PD.9"
#define KEY2_GPIO_NAME    "PD.10"
#define KEY3_GPIO_NAME    "PD.11"

const char* press_event[] = {"PRESS_DOWN", "PRESS_UP", "PRESS_REPEAT", "SINGLE_CLICK", "DOUBLE_CLICK", "LONG_PRESS_START", "LONG_PRESS_HOLD", "BUTTON_EVENT_MAX", "NONE_PRESS"};
void btn_callback(btn_handle_t handle, void* user_data) {
    rt_kprintf("BTN %s: %s\r\n", (char *)user_data, press_event[zx_button_get_event(handle)]);

    rt_uint32_t event = 0;
    if(!strcmp((char *)user_data, "key1")) {
        event |= (0x01 << 8);
    }
    else if(!strcmp((char *)user_data, "key2")) {
        event |= (0x02 << 8);
    }
    else if(!strcmp((char *)user_data, "key3")) {
        event |= (0x04 << 8);
    }
    event = event | (zx_button_get_event(handle) & 0xff);
    zx_gui_send_event(event, true);
}

void zx_button_start_gpio(void)
{   
    zx_button_config_t config = {
        .ticks_interval_ms = 10,
        .debounce_ticks = 2,
        .short_ticks = 200 / 10,
        .long_ticks = 1000 / 10,
        .update_task = {
            .en = 1,
            .core = 1,
            .priority = 5,
        }
    };
    zx_button_init(&config);

    long pin = hal_gpio_name2pin(KEY1_GPIO_NAME);
    rt_kprintf("start test\n");
    btn_handle_t* btn1 = zx_button_create_gpio(pin, 0, 0, "key1");
    zx_button_register_cb(btn1, BUTTON_SINGLE_CLICK,     btn_callback);
    zx_button_start(btn1);

    pin = hal_gpio_name2pin(KEY2_GPIO_NAME);
    btn_handle_t* btn2 = zx_button_create_gpio(pin, 0, 0, "key2");
    zx_button_register_cb(btn2, BUTTON_SINGLE_CLICK,     btn_callback);
    zx_button_start(btn2);

    pin = hal_gpio_name2pin(KEY3_GPIO_NAME);
    btn_handle_t* btn3 = zx_button_create_gpio(pin, 0, 0, "key3");
    zx_button_register_cb(btn3, BUTTON_SINGLE_CLICK,     btn_callback);
    zx_button_start(btn3);
}
#endif
