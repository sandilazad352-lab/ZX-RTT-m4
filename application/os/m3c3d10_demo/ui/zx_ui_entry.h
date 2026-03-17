#ifndef __ZX_UI_ENTRY_H__
#define __ZX_UI_ENTRY_H__

#include "lvgl.h"
#include <stdio.h>
#include <rtconfig.h>

#ifdef LPKG_USING_RAMDISK
#define LVGL_DIR "L:/ram/"
#else
#define LVGL_DIR "L:/rodata/"
#endif

#endif  //__ZX_UI_ENTRY_H__
