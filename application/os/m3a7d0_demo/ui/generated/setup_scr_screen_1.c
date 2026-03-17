/*
 * @Author: BigBee-yellowyellow 2956747896@qq.com
 * @Date: 2024-04-11 17:12:39
 * @LastEditors: BigBee-yellowyellow 2956747896@qq.com
 * @LastEditTime: 2024-04-17 13:32:05
 * @FilePath: /weather_clock_700_M3/components/generated/setup_scr_screen_1.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/*
 * Copyright 2024 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"

extern char* read_png_path[];

void screen_1_flag_init(lv_ui* ui){
	// //对屏幕的操作 
	lv_obj_clear_flag(ui->screen_1, LV_OBJ_FLAG_OVERFLOW_VISIBLE);
    lv_obj_clear_flag(ui->screen_1, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_clear_flag(ui->screen_1, LV_OBJ_FLAG_SCROLL_ELASTIC);
    lv_obj_clear_flag(ui->screen_1, LV_OBJ_FLAG_SCROLL_ONE);
    lv_obj_clear_flag(ui->screen_1, LV_OBJ_FLAG_SNAPPABLE);
    lv_obj_clear_flag(ui->screen_1, LV_OBJ_FLAG_FLOATING);	

	lv_obj_add_flag(ui->screen_1_img_2, LV_OBJ_FLAG_FLOATING);	
}

void setup_scr_screen_1(lv_ui *ui){

	//Write codes screen_1
	ui->screen_1 = lv_obj_create(NULL);
	lv_obj_set_scrollbar_mode(ui->screen_1, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_1_main_main_default
	static lv_style_t style_screen_1_main_main_default;
	if (style_screen_1_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_1_main_main_default);
	else
		lv_style_init(&style_screen_1_main_main_default);
	// lv_style_set_bg_color(&style_screen_1_main_main_default, lv_color_hex(0x1e2030));
	lv_style_set_bg_color(&style_screen_1_main_main_default, lv_color_hex(0x272B50));
	lv_style_set_bg_opa(&style_screen_1_main_main_default, 255);
	lv_obj_add_style(ui->screen_1, &style_screen_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);


	// //Write codes screen_1_img_1
	// ui->screen_1_img_1 = lv_img_create(ui->screen_1);
	// lv_obj_set_pos(ui->screen_1_img_1, 0, 0);
	// lv_img_set_src(ui->screen_1_img_1,read_png_path[SECOND_BOTTOM]);
	//Write codes screen_1_img_2
	ui->screen_1_img_2 = lv_img_create(ui->screen_1);
	lv_obj_set_pos(ui->screen_1_img_2, -5, 0);
	lv_img_set_src(ui->screen_1_img_2,read_png_path[SECOND_LOCAL]);
	//Write codes screen_1_img_3
	ui->screen_1_img_3 = lv_img_create(ui->screen_1);
	lv_obj_set_pos(ui->screen_1_img_3, 272, 8);
	lv_img_set_src(ui->screen_1_img_3,read_png_path[SECOND_WEA]);
	//Write codes screen_1_img_4
	ui->screen_1_img_4 = lv_img_create(ui->screen_1);
	lv_obj_set_pos(ui->screen_1_img_4, 260, 253);
	lv_img_set_src(ui->screen_1_img_4,read_png_path[SECOND_TEST]);

	screen_1_flag_init(ui);

	screen_1_add_event_handler(ui);
}