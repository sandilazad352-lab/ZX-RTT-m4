#ifndef _ZX_GUI_H
#define _ZX_GUI_H

#include "lvgl.h"

#ifdef __cplusplus
extern "C"
{
#endif

#if defined(KERNEL_RTTHREAD)
typedef void (*zx_gui_event_cb)(rt_uint32_t event);
typedef void (*zx_gui_mq_cb)(const void *buffer, rt_size_t size);
typedef void (*zx_gui_loop_cb)(void);

int zx_gui_init(void);
int zx_gui_lock(rt_int32_t timeout);
int zx_gui_unlock(void);

void zx_gui_set_event_cb(zx_gui_event_cb event_cb);
void zx_gui_set_mq_cb(zx_gui_mq_cb mq_cb);
void zx_gui_set_loop_cb(zx_gui_loop_cb l_cb);
rt_err_t zx_gui_send_event(rt_uint32_t event, bool toui);
rt_err_t zx_gui_send_mq(const void *buffer, rt_size_t size, bool toui);
#else
int zx_gui_init(void);
int zx_gui_lock(uint32_t timeout);
int zx_gui_unlock(void);
#endif

#ifdef __cplusplus
}
#endif

#endif /* _ZX_GUI_H */
