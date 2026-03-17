#ifndef __ZX_UI_ENTRY_H__
#define __ZX_UI_ENTRY_H__

#include "lvgl.h"
#include <stdio.h>
#include <rtconfig.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <aic_core.h>
#include <aic_hal.h>
#ifdef LPKG_USING_RAMDISK
#define LVGL_DIR "L:/ram/"
#else
#define LVGL_DIR "L:/rodata/"
#endif



#endif  //__ZX_UI_ENTRY_H__
