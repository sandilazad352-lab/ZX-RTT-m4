#include <lvgl.h>
#include <zx_gui.h>
#include <zx_port.h>
#include <zx_port_indev.h>
#include <aic_dec.h>

#define SLEEP_PERIOD 3

#ifndef CACHE_IMG_NUM
#define CACHE_IMG_NUM 15
#endif

void __attribute__((weak)) zx_ui_entry(void)
{
}

#if defined(KERNEL_RTTHREAD)
#define DBG_TAG    "ZX_GUI"
#define DBG_LVL    DBG_INFO
#include <rtthread.h>
#include <rtdevice.h>
#include <rtdbg.h>

static struct rt_thread __zx_gui_thread;

#ifdef LPKG_USING_FREETYPE
static ALIGN(8) rt_uint8_t __zx_gui_thread_stack[1024 * 256];    // 256K for freetype
#else
static ALIGN(8) rt_uint8_t __zx_gui_thread_stack[1024 * 16];    // 16K
#endif

#define ZX_GUI_MQ_SIZE    256

#define ZX_GUI_MQ_NUM     16

#define ZX_GUI_EVENT_TIMEOUT  2
#define ZX_GUI_MQ_TIMEOUT  2

static rt_mutex_t __zx_gui_mutex = RT_NULL;

rt_event_t __zx_gui_recv_event = RT_NULL;
rt_mq_t __zx_gui_recv_mq = RT_NULL;
static rt_event_t __zx_gui_send_event = RT_NULL;
static rt_mq_t __zx_gui_send_mq = RT_NULL;

static zx_gui_event_cb __zx_e_cb;
static zx_gui_mq_cb __zx_mq_cb;
static zx_gui_loop_cb __zx_l_cb;

#ifdef RT_USING_PM
struct rt_semaphore pm_sem;
void app_notify(rt_uint8_t event, rt_uint8_t mode, void *data)
{
    if (event == RT_PM_ENTER_SLEEP)
    {
        rt_sem_take(&pm_sem, RT_WAITING_FOREVER);
    }
    else if (event == RT_PM_EXIT_SLEEP)
    {
        rt_sem_release(&pm_sem);
    }
}
#endif

static void __zx_gui_entry(void *parameter)
{
    __zx_gui_mutex = rt_mutex_create("zx_gui_mutex", RT_IPC_FLAG_FIFO);
    if (__zx_gui_mutex == RT_NULL) {
        rt_kprintf("create zx_gui_mutex mutex failed.\n");
        goto err_out;
    }

    __zx_gui_recv_event = rt_event_create("zx_gui_recv_event", RT_IPC_FLAG_FIFO);
    if (__zx_gui_recv_event == RT_NULL) {
        rt_kprintf("create zx_gui_recv_event event failed.\n");
        goto err_out;
    }

    __zx_gui_recv_mq = rt_mq_create("zx_gui_recv_mq", ZX_GUI_MQ_SIZE, ZX_GUI_MQ_NUM, RT_IPC_FLAG_FIFO);
    if (__zx_gui_recv_mq == RT_NULL) {
        rt_kprintf("create zx_gui_recv_mq event failed.\n");
        goto err_out;
    }

    __zx_gui_send_event = rt_event_create("zx_gui_send_event", RT_IPC_FLAG_FIFO);
    if (__zx_gui_send_event == RT_NULL) {
        rt_kprintf("create zx_gui_send_event event failed.\n");
        goto err_out;
    }

    __zx_gui_send_mq = rt_mq_create("zx_gui_send_mq", ZX_GUI_MQ_SIZE, ZX_GUI_MQ_NUM, RT_IPC_FLAG_FIFO);
    if (__zx_gui_send_mq == RT_NULL) {
        rt_kprintf("create zx_gui_send_mq event failed.\n");
        goto err_out;
    }

    zx_gui_lock(RT_WAITING_FOREVER);

    lv_init();
    zx_port_disp_init();
    zx_port_indev_init();
    lv_img_cache_set_size(CACHE_IMG_NUM);
    aic_dec_create();   // png/jpg dec

    zx_ui_entry();
    zx_gui_unlock();
#ifdef RT_USING_PM
    rt_sem_init(&pm_sem, "pm_sem", 1, RT_IPC_FLAG_PRIO);
    rt_pm_notify_set(app_notify, NULL);
#endif
    /* handle the tasks of LVGL */
    while(1)
    {
        rt_err_t err;
        int sleep_timeout = SLEEP_PERIOD;
        rt_uint32_t event_recv = 0;
        char mq_buf[ZX_GUI_MQ_SIZE];

        event_recv = 0;
        memset(mq_buf, 0, sizeof(mq_buf));
#ifdef RT_USING_PM
        rt_int32_t tick_event = rt_tick_from_millisecond(0);
        rt_int32_t tick_mq = rt_tick_from_millisecond(0);
#else
        rt_int32_t tick_event = rt_tick_from_millisecond(1);
        rt_int32_t tick_mq = rt_tick_from_millisecond(1);
#endif

#ifdef RT_USING_PM
        rt_sem_take(&pm_sem, RT_WAITING_FOREVER);
        rt_pm_module_request(PM_MAIN_ID, PM_SLEEP_MODE_NONE);
#endif

        zx_gui_lock(RT_WAITING_FOREVER);
        lv_task_handler();
        err = rt_event_recv(__zx_gui_recv_event, 0xFFFFFFFF, RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR, tick_event, &event_recv);
        if (err == RT_ETIMEOUT) {
            sleep_timeout -= ZX_GUI_EVENT_TIMEOUT;
        } else if (err == RT_EOK) {
            if (__zx_e_cb) {
                __zx_e_cb(event_recv);
            }
        }
        err = rt_mq_recv(__zx_gui_recv_mq, &mq_buf, sizeof(mq_buf), tick_mq);
        if (err == RT_ETIMEOUT) {
            sleep_timeout -= ZX_GUI_EVENT_TIMEOUT;
        } else if (err == RT_EOK) {
            if (__zx_mq_cb) {
                __zx_mq_cb(mq_buf, sizeof(mq_buf));
            }
        }
        if (__zx_l_cb) {
            __zx_l_cb();
        }

        zx_gui_unlock();
#ifdef RT_USING_PM
        rt_pm_module_release(PM_MAIN_ID, PM_SLEEP_MODE_NONE);
        rt_sem_release(&pm_sem);
#endif

#ifdef RT_USING_PM
        rt_uint32_t status = rt_pm_module_get_status();
        if (!(status & (1 << PM_POWER_ID)))
        {
            /* 没有模块请求保持唤醒，可以延迟让系统进入低功耗 */
            rt_thread_mdelay(sleep_timeout);
        }
        else
        {
            if (sleep_timeout > 0)
                rt_thread_mdelay(1);  /* 至少延迟1ms让idle线程有机会执行 */
        }
#else
        rt_thread_mdelay(sleep_timeout);
#endif
    }

err_out:
    if (__zx_gui_mutex != RT_NULL) {
        rt_mutex_delete(__zx_gui_mutex);
        __zx_gui_mutex = RT_NULL;
    }



    if (__zx_gui_recv_event != RT_NULL) {
        rt_event_delete(__zx_gui_recv_event);
        __zx_gui_recv_event = RT_NULL;
    }

    if (__zx_gui_recv_mq != RT_NULL) {
        rt_mq_delete(__zx_gui_recv_mq);
        __zx_gui_recv_mq = RT_NULL;
    }

    if (__zx_gui_send_event != RT_NULL) {
        rt_event_delete(__zx_gui_send_event);
        __zx_gui_send_event = RT_NULL;
    }

    if (__zx_gui_send_mq != RT_NULL) {
        rt_mq_delete(__zx_gui_send_mq);
        __zx_gui_send_mq = RT_NULL;
    }
}

/**
 * @brief    This function will get zx gui lock.
 *
 * @note     Executing this function will task zx gui lock.
 *
 * @param    timeout is a timeout ms (unit: ms).
 *
 * @return   Return the operation status. 0 is ok, -1 is not ok.
 *
 */
int zx_gui_lock(rt_int32_t timeout)
{
    rt_int32_t tick;

    if (__zx_gui_mutex == RT_NULL)
        return -1;

    tick = rt_tick_from_millisecond(timeout);
    if (RT_EOK == rt_mutex_take(__zx_gui_mutex, tick)) {
        return 0;
    } else {
        return -1;
    }
}

/**
 * @brief    This function will release zx gui lock.
 *
 * @note     Executing this function will release zx gui lock.
 *
 *
 * @return   Return the operation status. 0 is ok, -1 is not ok.
 *
 */
int zx_gui_unlock(void)
{
    if (__zx_gui_mutex == RT_NULL)
        return -1;

    if (RT_EOK == rt_mutex_release(__zx_gui_mutex)) {
        return 0;
    } else {
        return 1;
    }
}

/**
 * @brief    This function will set zx gui event callback.
 *
 * @note     Executing this function will set zx gui event callback.
 *
 * @param    event_cb is a callback.
 *
 * @return   
 *
 */
void zx_gui_set_event_cb(zx_gui_event_cb event_cb)
{
    zx_gui_lock(RT_WAITING_FOREVER);
    __zx_e_cb = event_cb;
    zx_gui_unlock();
}

/**
 * @brief    This function will set zx gui mq callback.
 *
 * @note     Executing this function will set zx gui mq callback.
 *
 * @param    mq_cb is a callback.
 *
 * @return   
 *
 */
void zx_gui_set_mq_cb(zx_gui_mq_cb mq_cb)
{
    zx_gui_lock(RT_WAITING_FOREVER);
    __zx_mq_cb = mq_cb;
    zx_gui_unlock();
}

/**
 * @brief    This function will set zx gui loop callback.
 *
 * @note     Executing this function will set zx lgui oop callback.
 *
 * @param    loop_cb is a callback.
 *
 * @return   
 *
 */
void zx_gui_set_loop_cb(zx_gui_loop_cb loop_cb)
{
    zx_gui_lock(RT_WAITING_FOREVER);
    __zx_l_cb = loop_cb;
    zx_gui_unlock();
}

/**
 * @brief    This function will send evnet to zx gui task.
 *
 * @note     Executing this function will set event to zx gui task.
 *
 * @param    event is a flag that you will set for this event's flag.
 *           You can set an event flag, or you can set multiple flags through OR logic operation.
 * @param    toui is a flag that indicate send event to zx gui task or send event from zx gui task.
 *
 * @return   Return the operation status. When the return value is RT_EOK, the operation is successful.
 *           If the return value is any other values, it means that the event detach failed.
 *
 */
rt_err_t zx_gui_send_event(rt_uint32_t event, bool toui)
{
    if (toui) {
        if (__zx_gui_recv_event != RT_NULL) {
            return rt_event_send(__zx_gui_recv_event, event);
        } else {
            return RT_ERROR;
        }
    } else {
        if (__zx_gui_send_event != RT_NULL) {
            return rt_event_send(__zx_gui_send_event, event);
        } else {
            return RT_ERROR;
        }
    }
}

/**
 * @brief    This function will send a message to zx gui task.
 *
 * @note     When using this function to send a message to zx gui task
 *
 *
 * @param    buffer is the content of the message.
 *
 * @param    size is the length of the message(Unit: Byte).
 *
 * @return   Return the operation status. When the return value is RT_EOK, the operation is successful.
 *           If the return value is any other values, it means that the messagequeue detach failed.
 *
 */
rt_err_t zx_gui_send_mq(const void *buffer, rt_size_t size, bool toui)
{
    if (toui) {
        if (__zx_gui_recv_mq != RT_NULL) {
            return rt_mq_send(__zx_gui_recv_mq, buffer, size);
        } else {
            return RT_ERROR;
        }
    } else {
        if (__zx_gui_send_mq != RT_NULL) {
            return rt_mq_send(__zx_gui_send_mq, buffer, size);
        } else {
            return RT_ERROR;
        }        
    }
}
#else
#include <FreeRTOS.h>
#include <semphr.h>
#include <timers.h>
#include <event_groups.h>

static SemaphoreHandle_t g_zx_gui_sem = NULL;

static void __zx_gui_entry(void *parameter)
{
    zx_gui_lock(portMAX_DELAY);

    lv_init();
    zx_port_disp_init();
    zx_port_indev_init();
    lv_img_cache_set_size(CACHE_IMG_NUM);
    aic_dec_create();   // png/jpg dec

    zx_ui_entry();
    zx_gui_unlock();

    /* handle the tasks of LVGL */
    while(1)
    {
        zx_gui_lock(portMAX_DELAY);
        lv_task_handler();
        zx_gui_unlock();

        vTaskDelay(pdMS_TO_TICKS(SLEEP_PERIOD));
    }

    vTaskDelete(NULL);
}

/**
 * @brief    This function will get zx gui lock.
 *
 * @note     Executing this function will task zx gui lock.
 *
 * @param    timeout is a timeout ms (unit: ms).
 *
 * @return   Return the operation status. 0 is ok, -1 is not ok.
 *
 */
int zx_gui_lock(uint32_t timeout)
{
    if (!g_zx_gui_sem)
        return -1;
    if (pdTRUE == xSemaphoreTake(g_zx_gui_sem, timeout)) {
        return 0;
    } else {
        return -1;
    }
}

/**
 * @brief    This function will release zx gui lock.
 *
 * @note     Executing this function will release zx gui lock.
 *
 *
 * @return   Return the operation status. 0 is ok, -1 is not ok.
 *
 */
int zx_gui_unlock(void)
{
    if (!g_zx_gui_sem)
        return -1;
    xSemaphoreGive(g_zx_gui_sem);
    return 0;
}
#endif

/**
 * @brief    This function will init zx gui.
 *
 * @note     Executing this function will init zx gui task.
 *
 *
 * @return   
 *
 */
int zx_gui_init(void)
{
#if defined(KERNEL_RTTHREAD)
    rt_err_t err;

    err = rt_thread_init(&__zx_gui_thread, "zx_gui", __zx_gui_entry, RT_NULL,
           &__zx_gui_thread_stack[0], sizeof(__zx_gui_thread_stack), 25, 10);
    if(err != RT_EOK)
    {
        LOG_E("Failed to create zx gui thread");
        return -1;
    }
    rt_thread_startup(&__zx_gui_thread);
#else
    g_zx_gui_sem = xSemaphoreCreateMutex();
#ifdef LPKG_USING_FREETYPE
    xTaskCreate(__zx_gui_entry, "zx_gui", 128 * 1024, NULL, 5, NULL);
#else
    xTaskCreate(__zx_gui_entry, "zx_gui", 16 * 1024, NULL, 5, NULL);
#endif
#endif

    return 0;
}

