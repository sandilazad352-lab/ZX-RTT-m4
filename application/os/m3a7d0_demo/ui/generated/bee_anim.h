/*
 * @Descripttion: 
 * @version: 
 * @Author: 输入自己姓名
 * @Date: 2023-10-31 17:09:38
 * @LastEditors: BigBee-yellowyellow 2956747896@qq.com
 * @LastEditTime: 2024-04-16 17:04:18
 */
#ifndef _BEE_ANIM_H_
#define _BEE_ANIM_H_
#include "gui_guider.h"

typedef struct _screen_timer_t{
    bool screen_timer;
    bool screen_1_timer;
    bool screen_2_timer;
    bool screen_3_timer;
    bool screen_4_timer;
    bool screen_5_timer;
    bool screen_6_timer;
    bool screen_7_timer;
    bool screen_8_timer;
    bool screen_9_timer;
    bool screen_10_timer;
    int  screen_timer_index;
    int  screen_1_timer_index;
    int  screen_2_timer_index;
    int  screen_3_timer_index;
    int  screen_4_timer_index;
    int  screen_5_timer_index;
    int  screen_6_timer_index;
    int  screen_7_timer_index;
    int  screen_8_timer_index;
    int  screen_9_timer_index;
    int  screen_10_timer_index;
}screen_timer_t;

void obj_linear_infinite_anim(lv_obj_t* var_obj, int start_data, int end_data, int time, void* cb_function);

/*======================== screen ========================*/
void lv_screen_load(lv_ui* ui);
void lv_screen_unload(lv_ui* ui);
void screen_img_1_zoom(lv_ui* ui, bool zoom_sign);
void screen_img_2_zoom(lv_ui* ui, bool zoom_sign);
void screen_img_3_zoom(lv_ui* ui, bool zoom_sign);
void screen_img_m4_anim(lv_ui* ui, int8_t mode_sign);
void screen_wea_anim(lv_ui* ui, int8_t wea_sign);
void screen_light_anim(lv_ui* ui);
void screen_light_change_color(lv_ui* ui, int8_t sw_sign);
void screen_light_open(lv_ui* ui, int8_t sw_sign);
/*======================== screen_1 ========================*/
void lv_screen_1_load(lv_ui* ui);
void lv_screen_1_unload(lv_ui* ui);
void set_bk_scr(lv_obj_t *scr);


void lv_img_zoom(lv_obj_t* obj, int32_t values);

#endif
