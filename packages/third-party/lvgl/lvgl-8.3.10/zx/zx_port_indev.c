/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-10-18     Meco Man     The first version
 * 2021-12-17     Wayne        Add input event
 * 2023-05-02     Ning Fang    Add ZX input event
 */
#include <lvgl.h>
#include <stdbool.h>
#if defined(KERNEL_RTTHREAD)
#include <rtthread.h>
#include <../components/drivers/include/drivers/touch.h>
#endif
#include <zx_tp.h>

#ifndef ZX_TOUCH_NONE
static lv_indev_state_t last_state = LV_INDEV_STATE_REL;
static rt_int16_t last_x = 0;
static rt_int16_t last_y = 0;

static void __input_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    data->point.x = last_x;
    data->point.y = last_y;
    data->state = last_state;
}

void zx_touch_inputevent_cb(rt_int16_t x, rt_int16_t y, rt_uint8_t state)
{
    switch (state)
    {
    case RT_TOUCH_EVENT_UP:
        last_state = LV_INDEV_STATE_RELEASED;
        break;
    case RT_TOUCH_EVENT_MOVE:
    case RT_TOUCH_EVENT_DOWN:
        last_x = x;
        last_y = y;
        last_state = LV_INDEV_STATE_PRESSED;
        break;
    }
}
#endif

#ifdef KERNEL_FREERTOS
static void __input_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data) {
    uint8_t press = 0;
    uint16_t x, y;
    touch_freertos_read(&press, &x, &y);
    if (press) {
        data->point.x = x;
        data->point.y = y;
        data->state = LV_INDEV_STATE_PR;
    } else {
        data->state = LV_INDEV_STATE_REL;
    }
}
#endif

void zx_port_indev_init(void)
{
#ifdef TOUCH_PANEL_DRIVER
    touch_freertos_init();
#endif
 
#if !defined(ZX_TOUCH_NONE) || defined(TOUCH_PANEL_DRIVER)
    static lv_indev_drv_t indev_drv;

    /* Basic initialization */
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = __input_read;

    /* Register the driver in LVGL and save the created input device object */
    lv_indev_drv_register(&indev_drv);
#endif
}
