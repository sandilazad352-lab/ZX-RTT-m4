/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2021-10-18     Meco Man      First version
 */

#ifndef LV_CONF_H
#define LV_CONF_H

#include <rtconfig.h>

#ifndef LV_COLOR_DEPTH
#define LV_COLOR_DEPTH          32
#endif

#define LV_HOR_RES_MAX          BSP_LCD_WIDTH
#define LV_VER_RES_MAX          BSP_LCD_HEIGHT
#define LV_IMG_CACHE_DEF_SIZE 5
#define LV_USE_MEM_MONITOR 0
#define LV_INDEV_DEF_READ_PERIOD 5 //10

#ifdef AIC_LVGL_BENCHMARK_DEMO
#define LV_USE_PERF_MONITOR 1
#endif

#if defined(KERNEL_BAREMETAL) || defined(KERNEL_FREERTOS)
#define LV_TICK_CUSTOM 1
#define LV_TICK_CUSTOM_INCLUDE <aic_time.h>
#define LV_TICK_CUSTOM_SYS_TIME_EXPR (aic_get_time_ms())    /*Expression evaluating to current system time in ms*/
#define LV_DISP_DEF_REFR_PERIOD 10
#endif

#define LV_USE_FS_POSIX 1
#if LV_USE_FS_POSIX
    #define LV_FS_POSIX_LETTER 'L'     /*Set an upper cased letter on which the drive will accessible (e.g. 'A')*/
    #define LV_FS_POSIX_PATH ""         /*Set the working directory. File/directory paths will be appended to it.*/
    #define LV_FS_POSIX_CACHE_SIZE 0    /*>0 to cache this number of bytes in lv_fs_read()*/
#endif

/*PNG decoder library*/
#define LV_USE_PNG 0

/*BMP decoder library*/
#define LV_USE_BMP 0

/* JPG + split JPG decoder library.
 * Split JPG is a custom format optimized for embedded systems. */
#define LV_USE_SJPG 0

/*GIF decoder library*/
#define LV_USE_GIF 0

/*QR code library*/
#define LV_USE_QRCODE 0

#ifdef ZX_LVGL_BENCHMARK_DEMO
#define LV_FONT_MONTSERRAT_12       1
#define LV_FONT_MONTSERRAT_16       1
#define LV_FONT_MONTSERRAT_40       1
#endif

#ifdef LPKG_USING_FREETYPE
/*FreeType library*/
#define LV_USE_FREETYPE 1
#if LV_USE_FREETYPE
    /*Memory used by FreeType to cache characters [bytes] (-1: no caching)*/
    #define LV_FREETYPE_CACHE_SIZE (16 * 1024)
    #if LV_FREETYPE_CACHE_SIZE >= 0
        /* 1: bitmap cache use the sbit cache, 0:bitmap cache use the image cache. */
        /* sbit cache:it is much more memory efficient for small bitmaps(font size < 256) */
        /* if font size >= 256, must be configured as image cache */
        #define LV_FREETYPE_SBIT_CACHE 0
        /* Maximum number of opened FT_Face/FT_Size objects managed by this cache instance. */
        /* (0:use system defaults) */
        #define LV_FREETYPE_CACHE_FT_FACES 0
        #define LV_FREETYPE_CACHE_FT_SIZES 0
    #endif
#endif
#endif

#ifdef AIC_XIP
#define LV_ATTRIBUTE_FAST_MEM   __attribute__((section(".ram.code")))
#endif

#define LV_COLOR_SCREEN_TRANSP 0

#endif
