#ifndef __ZX_TOUCH_FREERTOS_H__
#define __ZX_TOUCH_FREERTOS_H__

#include "stdio.h"

void touch_freertos_init();

void touch_freertos_read(uint8_t* press, uint16_t* x, uint16_t* y);

#endif
