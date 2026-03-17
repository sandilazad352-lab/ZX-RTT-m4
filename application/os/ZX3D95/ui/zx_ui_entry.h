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

#define IMG_SWICH_N            LVGL_DIR"first_screen/switch_n.png"
#define IMG_SWICH_Y            LVGL_DIR"first_screen/switch_y.png"

#define IMG_BACKGROUND_FIRST            LVGL_DIR"first_screen/img_background_first.png"
#define IMG_WEATHER_FIRST               LVGL_DIR"first_screen/img_weather_first.png"
#define IMG_HT_FIRST                    LVGL_DIR"first_screen/img_ht_first.png"
#define IMG_AIR_FIRST                   LVGL_DIR"first_screen/img_air_first.png"
#define IMG_CLEAN_FIRST                 LVGL_DIR"first_screen/img_clean_first.png"
#define SMALL_IMG_CLEAN_FIRST           LVGL_DIR"first_screen/small_img_clean_first.png"
#define IMG_NTO_DISTURB_FIRST           LVGL_DIR"first_screen/img_not_disturb_first.png"
#define SMALL_IMG_NTO_DISTURB_FIRST     LVGL_DIR"first_screen/small_img_not_disturb_first.png"
#define IMG_MORE_FIRST                  LVGL_DIR"first_screen/img_more_first.png"
#define SMALL_IMG_MORE_FIRST            LVGL_DIR"first_screen/small_img_more_first.png"


#define IMG2_CLEAN_SECOND               LVGL_DIR"second_screen/img2_clean_second.png"
#define IMG2_NTO_DISTURB_SECOND         LVGL_DIR"second_screen/img2_not_disturb_second.png"
#define IMG_TUBELAMP_SECOND             LVGL_DIR"second_screen/img_tubelamp_second.png"
#define IMG_LAMP                        LVGL_DIR"second_screen/img_lamp.png"

#define IMG_CORRIDORLAMP_SECOND         LVGL_DIR"second_screen/img_corridorlamp_second.png"
#define IMG_CURTAIN_SECOND              LVGL_DIR"second_screen/img_curtain_second.png"
#define IMG_CURTAIN2_SECOND             LVGL_DIR"second_screen/img_curtain2_second.png"
#define IMG_CURTAIN_OPEN_SECOND         LVGL_DIR"second_screen/img_curtain_open_second.png"
#define IMG_CURTAIN_CLOSE_SECOND        LVGL_DIR"second_screen/img_curtain_close_second.png"

#define IMG_AIR_CONDITION_SECOND        LVGL_DIR"second_screen/img_air_condition_second.png"
#define IMG_AIR_CONDITION2_SECOND       LVGL_DIR"second_screen/img_air_condition2_second.png"
#define IMG_AIR_CONDITION_LEFT_SECOND   LVGL_DIR"second_screen/img_air_condition_left_second.png"
#define IMG_AIR_CONDITION_RIGHT_SECOND  LVGL_DIR"second_screen/img_air_condition_right_second.png"
#define IMG_AIR_CONDITION_HOT_SECOND    LVGL_DIR"second_screen/img_air_condition_hot_second.png"
#define IMG_AIR_CONDITION_COLD_SECOND   LVGL_DIR"second_screen/img_air_condition_cold_second.png"
#define IMG_AIR_CONDITION_WET_SECOND    LVGL_DIR"second_screen/img_air_condition_wet_second.png"

#endif  //__ZX_UI_ENTRY_H__
