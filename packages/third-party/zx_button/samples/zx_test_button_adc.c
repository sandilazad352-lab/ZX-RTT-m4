/*
 * SPDX-License-Identifier: Apache-2.0
 */
#include <rtthread.h>
#include "zx_gui.h"
#include <stdio.h>
#include "string.h"
#include <aic_core.h>
#include <aic_hal.h>

#ifdef LPKG_USING_ZX_BUTTON
    #ifdef PKG_USING_ZX_ADC_BUTTON

#include "zx_button.h"

static const char* press_event[] = {"PRESS_DOWN", "PRESS_UP", "PRESS_REPEAT", "SINGLE_CLICK", "DOUBLE_CLICK", "LONG_PRESS_START", "LONG_PRESS_HOLD", "BUTTON_EVENT_MAX", "NONE_PRESS"};

static void btn_callback(btn_handle_t handle, void* user_data) {
    rt_kprintf("BTN %s: %s\r\n", (char *)user_data, press_event[zx_button_get_event(handle)]);
    zx_gui_send_event(zx_button_get_event(handle), true);
}

void zx_button_start_adc(void)
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
    
    static bool init_flag = false;
    if(init_flag == false)
    {
        init_flag = true;
        zx_button_init(&config);
    }

    rt_kprintf("start test\n");
    //1829   1760
    //1433  1397
    //819   766
    //409   399
    btn_handle_t* btn1 = zx_button_create_adc(ADC_BTN_CHANNEL, 1829, 200, 0, "key1");
    if(btn1 == NULL)
    {
        rt_kprintf("The channel or button has been initialized\n");
        return;
    }
    zx_button_register_cb(btn1, BUTTON_PRESS_DOWN,       btn_callback);
    zx_button_register_cb(btn1, BUTTON_PRESS_UP,         btn_callback);
    zx_button_register_cb(btn1, BUTTON_PRESS_REPEAT,     btn_callback);
    zx_button_register_cb(btn1, BUTTON_SINGLE_CLICK,     btn_callback);
    zx_button_register_cb(btn1, BUTTON_DOUBLE_CLICK,     btn_callback);
    zx_button_register_cb(btn1, BUTTON_LONG_PRESS_START, btn_callback);
    zx_button_register_cb(btn1, BUTTON_LONG_PRESS_HOLD,  btn_callback);
    zx_button_start(btn1);

    btn_handle_t* btn2 = zx_button_create_adc(ADC_BTN_CHANNEL, 1433, 100, 0, "KEY2");
    if(btn2 == NULL)
    {
        rt_kprintf("The channel or button has been initialized\n");
        return;
    }
    zx_button_register_cb(btn2, BUTTON_PRESS_DOWN,       btn_callback);
    zx_button_register_cb(btn2, BUTTON_PRESS_UP,         btn_callback);
    zx_button_register_cb(btn2, BUTTON_PRESS_REPEAT,     btn_callback);
    zx_button_register_cb(btn2, BUTTON_SINGLE_CLICK,     btn_callback);
    zx_button_register_cb(btn2, BUTTON_DOUBLE_CLICK,     btn_callback);
    zx_button_register_cb(btn2, BUTTON_LONG_PRESS_START, btn_callback);
    zx_button_register_cb(btn2, BUTTON_LONG_PRESS_HOLD,  btn_callback);
    zx_button_start(btn2);


    btn_handle_t* btn3 = zx_button_create_adc(ADC_BTN_CHANNEL, 819, 100, 0, "KEY3");
    if(btn3 == NULL)
    {
        rt_kprintf("The channel or button has been initialized\n");
        return;
    }
    zx_button_register_cb(btn3, BUTTON_PRESS_DOWN,       btn_callback);
    zx_button_register_cb(btn3, BUTTON_PRESS_UP,         btn_callback);
    zx_button_register_cb(btn3, BUTTON_PRESS_REPEAT,     btn_callback);
    zx_button_register_cb(btn3, BUTTON_SINGLE_CLICK,     btn_callback);
    zx_button_register_cb(btn3, BUTTON_DOUBLE_CLICK,     btn_callback);
    zx_button_register_cb(btn3, BUTTON_LONG_PRESS_START, btn_callback);
    zx_button_register_cb(btn3, BUTTON_LONG_PRESS_HOLD,  btn_callback);
    zx_button_start(btn3);


    btn_handle_t* btn4 = zx_button_create_adc(ADC_BTN_CHANNEL, 409, 100, 0, "KEY4");
    if(btn4 == NULL)
    {
        rt_kprintf("The channel or button has been initialized\n");
        return;
    }
    zx_button_register_cb(btn4, BUTTON_PRESS_DOWN,       btn_callback);
    zx_button_register_cb(btn4, BUTTON_PRESS_UP,         btn_callback);
    zx_button_register_cb(btn4, BUTTON_PRESS_REPEAT,     btn_callback);
    zx_button_register_cb(btn4, BUTTON_SINGLE_CLICK,     btn_callback);
    zx_button_register_cb(btn4, BUTTON_DOUBLE_CLICK,     btn_callback);
    zx_button_register_cb(btn4, BUTTON_LONG_PRESS_START, btn_callback);
    zx_button_register_cb(btn4, BUTTON_LONG_PRESS_HOLD,  btn_callback);
    zx_button_start(btn4);
}

    #endif
#endif
