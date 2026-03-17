/*
 * @Author: BigBee-yellowyellow 2956747896@qq.com
 * @Date: 2024-04-11 17:12:39
 * @LastEditors: BigBee-yellowyellow 2956747896@qq.com
 * @LastEditTime: 2024-04-16 16:34:12
 * @FilePath: /weather_clock_700_M3/components/generated/events_init.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/*
 * Copyright 2024 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"

#include "event_handle.h"

void events_init(lv_ui *ui)
{
    
}

void screen_add_event_handler(lv_ui* bee){
    // screen_timer(bee);
    lv_obj_add_event_cb(bee->screen, screen_event_handler, LV_EVENT_ALL, bee);

    lv_obj_add_event_cb(bee->screen_img_1, screen_img_1_event_handler, LV_EVENT_ALL, bee);
    lv_obj_add_event_cb(bee->screen_img_2, screen_img_2_event_handler, LV_EVENT_ALL, bee);
    lv_obj_add_event_cb(bee->screen_img_3, screen_img_3_event_handler, LV_EVENT_ALL, bee);
    // screen_img_m4
    lv_obj_add_event_cb(bee->screen_img_m4, screen_img_m4_event_handler, LV_EVENT_ALL, bee);
    // screen_img_m6
    lv_obj_add_event_cb(bee->screen_img_m6, screen_img_m6_event_handler, LV_EVENT_ALL, bee);
    lv_obj_add_event_cb(bee->screen_img_m7, screen_img_m7_event_handler, LV_EVENT_ALL, bee);
    lv_obj_add_event_cb(bee->screen_img_m8, screen_img_m8_event_handler, LV_EVENT_ALL, bee);
    lv_obj_add_event_cb(bee->screen_img_m9, screen_img_m9_event_handler, LV_EVENT_ALL, bee);
    lv_obj_add_event_cb(bee->screen_img_m10,screen_img_m10_event_handler, LV_EVENT_ALL, bee);

    lv_obj_add_event_cb(bee->screen_label_39,screen_label_39_event_handler, LV_EVENT_ALL, bee);

    lv_obj_add_event_cb(bee->screen_img_m15,screen_img_m15_event_handler, LV_EVENT_ALL, bee);
    lv_obj_add_event_cb(bee->screen_img_m16,screen_img_m16_event_handler, LV_EVENT_ALL, bee);
    lv_obj_add_event_cb(bee->screen_img_m17,screen_img_m17_event_handler, LV_EVENT_ALL, bee);
    lv_obj_add_event_cb(bee->screen_img_m18,screen_img_m18_event_handler, LV_EVENT_ALL, bee);
    lv_obj_add_event_cb(bee->screen_img_m19,screen_img_m19_event_handler, LV_EVENT_ALL, bee);
    // lv_obj_add_event_cb(bee->screen_img_m20,screen_img_m10_event_handler, LV_EVENT_ALL, bee);
    lv_obj_add_event_cb(bee->screen_img_m21,screen_img_m21_event_handler, LV_EVENT_ALL, bee);
    lv_obj_add_event_cb(bee->screen_img_m22,screen_img_m22_event_handler, LV_EVENT_ALL, bee);
    lv_obj_add_event_cb(bee->screen_img_m23,screen_img_m23_event_handler, LV_EVENT_ALL, bee);
    lv_obj_add_event_cb(bee->screen_img_m24,screen_img_m24_event_handler, LV_EVENT_ALL, bee);
    lv_obj_add_event_cb(bee->screen_img_m25,screen_img_m25_event_handler, LV_EVENT_ALL, bee);
    // lv_obj_add_event_cb(bee->screen_img_m26,screen_img_m10_event_handler, LV_EVENT_ALL, bee);
    lv_obj_add_event_cb(bee->screen_img_m27,screen_img_m27_event_handler, LV_EVENT_ALL, bee);
    lv_obj_add_event_cb(bee->screen_img_m28,screen_img_m28_event_handler, LV_EVENT_ALL, bee);
    lv_obj_add_event_cb(bee->screen_img_m29,screen_img_m29_event_handler, LV_EVENT_ALL, bee);
    lv_obj_add_event_cb(bee->screen_img_m30,screen_img_m30_event_handler, LV_EVENT_ALL, bee);
    lv_obj_add_event_cb(bee->screen_img_m31,screen_img_m31_event_handler, LV_EVENT_ALL, bee);
    // lv_obj_add_event_cb(bee->screen_img_m32,screen_img_m10_event_handler, LV_EVENT_ALL, bee);/
    lv_obj_add_event_cb(bee->screen_img_m33,screen_img_m33_event_handler, LV_EVENT_ALL, bee);
    lv_obj_add_event_cb(bee->screen_img_m34,screen_img_m34_event_handler, LV_EVENT_ALL, bee);
    lv_obj_add_event_cb(bee->screen_img_m35,screen_img_m35_event_handler, LV_EVENT_ALL, bee);
    lv_obj_add_event_cb(bee->screen_img_m36,screen_img_m36_event_handler, LV_EVENT_ALL, bee);
    lv_obj_add_event_cb(bee->screen_img_m37,screen_img_m37_event_handler, LV_EVENT_ALL, bee);

    lv_obj_add_event_cb(bee->screen_sw_2,screen_sw_2_event_handler, LV_EVENT_ALL, bee);
    lv_obj_add_event_cb(bee->screen_sw_3,screen_sw_3_event_handler, LV_EVENT_ALL, bee);
    lv_obj_add_event_cb(bee->screen_sw_4,screen_sw_4_event_handler, LV_EVENT_ALL, bee);
    lv_obj_add_event_cb(bee->screen_sw_5,screen_sw_5_event_handler, LV_EVENT_ALL, bee);
}

void screen_1_add_event_handler(lv_ui* bee){
    // screen_1_timer(bee);
    lv_obj_add_event_cb(bee->screen_1, screen_1_event_handler, LV_EVENT_ALL, bee);
    lv_obj_add_event_cb(bee->screen_1, screen_img_m2_event_handler, LV_EVENT_ALL, bee);
}
