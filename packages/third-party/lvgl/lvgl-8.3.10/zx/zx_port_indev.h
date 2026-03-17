#ifndef __ZX_PORT_INDEV_H
#define __ZX_PORT_INDEV_H

#ifdef __cplusplus
extern "C" {
#endif

#include <lv_hal_indev.h>

void zx_port_indev_init(void);
#ifndef ZX_TOUCH_NONE
void zx_touch_inputevent_cb(rt_int16_t x, rt_int16_t y, rt_uint8_t state);
#endif

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
