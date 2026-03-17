/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-13     RiceChen     the first version
 */
#include "zx_tp.h"
#include "zx_port_indev.h"
#include "zx_gui.h"

#if defined(KERNEL_RTTHREAD)
#include <rtdevice.h>
#include <rtthread.h>

#ifndef ZX_TOUCH_NONE
#define THREAD_PRIORITY   24
#define THREAD_STACK_SIZE 4096
#define THREAD_TIMESLICE  5

static rt_thread_t  touch_thread = RT_NULL;
static rt_sem_t     touch_sem = RT_NULL;
static rt_device_t  dev = RT_NULL;
static struct rt_touch_data *read_data;
static struct rt_touch_info info;

extern void zx_touch_inputevent_cb(rt_int16_t x, rt_int16_t y, rt_uint8_t state);

static void __touch_entry(void *parameter)
{
    rt_device_control(dev, RT_TOUCH_CTRL_GET_INFO, &info);

    read_data = (struct rt_touch_data *)rt_malloc(sizeof(struct rt_touch_data) * info.point_num);

    while (1)
    {
        rt_sem_take(touch_sem, RT_WAITING_FOREVER);
        int num = 0;
        num = rt_device_read(dev, 0, read_data, info.point_num);
        if (num == info.point_num)
        {
            for (rt_uint8_t i = 0; i < info.point_num; i++)
            {
                if (read_data[i].event == RT_TOUCH_EVENT_DOWN
                        || read_data[i].event == RT_TOUCH_EVENT_UP
                        || read_data[i].event == RT_TOUCH_EVENT_MOVE)
                {
                    rt_uint16_t  u16X, u16Y;

                    u16X = read_data[i].x_coordinate;
                    u16Y = read_data[i].y_coordinate;

                    // rt_kprintf("t: %d/%d\n", u16X, u16Y);
                    zx_gui_lock(RT_WAITING_FOREVER);
                    zx_touch_inputevent_cb(u16X, u16Y, read_data[i].event);
                    zx_gui_unlock();
                }
            }
        }
        rt_device_control(dev, RT_TOUCH_CTRL_ENABLE_INT, RT_NULL);
    }
}

static rt_err_t rx_callback(rt_device_t dev, rt_size_t size)
{
#ifdef AIC_PM_POWER_TOUCH_WAKEUP
    rt_uint8_t sleep_mode;
#endif
    rt_sem_release(touch_sem);
    rt_device_control(dev, RT_TOUCH_CTRL_DISABLE_INT, RT_NULL);
#ifdef AIC_PM_POWER_TOUCH_WAKEUP
    sleep_mode = rt_pm_get_sleep_mode();
    if (sleep_mode != PM_SLEEP_MODE_NONE && !wakeup_triggered)
    {
        rt_pm_module_request(PM_POWER_ID, PM_SLEEP_MODE_NONE);
        wakeup_triggered = 1;
    }
    /* touch timer restart */
    rt_timer_start(touch_timer);
#endif
    return 0;
}

int tpc_run(const char *name)
{
    dev = rt_device_find(name);
    if (dev == RT_NULL)
    {
        rt_kprintf("can't find device:%s\n", name);
        return -1;
    }

    if (rt_device_open(dev, RT_DEVICE_FLAG_INT_RX) != RT_EOK)
    {
        rt_kprintf("open device failed!");
        return -1;
    }

    touch_sem = rt_sem_create("dsem", 0, RT_IPC_FLAG_FIFO);

    if (touch_sem == RT_NULL)
    {
        rt_kprintf("create dynamic semaphore failed.\n");
        return -1;
    }

    rt_device_set_rx_indicate(dev, rx_callback);

    touch_thread = rt_thread_create("touch",
                                     __touch_entry,
                                     RT_NULL,
                                     THREAD_STACK_SIZE,
                                     THREAD_PRIORITY,
                                     THREAD_TIMESLICE);

    if (touch_thread != RT_NULL)
        rt_thread_startup(touch_thread);

    return 0;
}
#endif
#else
int tpc_run(const char *name)
{
    return 0;
}
#endif
