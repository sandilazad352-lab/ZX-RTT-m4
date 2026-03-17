/*
 * Copyright 2024 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"



void setup_scr_screen_2(lv_ui *ui){

	//Write codes screen_2
	ui->screen_2 = lv_obj_create(NULL);
	lv_obj_set_scrollbar_mode(ui->screen_2, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_2_main_main_default
	static lv_style_t style_screen_2_main_main_default;
	if (style_screen_2_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_2_main_main_default);
	else
		lv_style_init(&style_screen_2_main_main_default);
	lv_style_set_bg_color(&style_screen_2_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_opa(&style_screen_2_main_main_default, 255);
	lv_obj_add_style(ui->screen_2, &style_screen_2_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_2_img_1
	ui->screen_2_img_1 = lv_img_create(ui->screen_2);
	lv_obj_set_pos(ui->screen_2_img_1, 23, 22);
	lv_obj_set_size(ui->screen_2_img_1, 115, 115);
	lv_obj_set_scrollbar_mode(ui->screen_2_img_1, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_2_img_1_main_main_default
	static lv_style_t style_screen_2_img_1_main_main_default;
	if (style_screen_2_img_1_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_2_img_1_main_main_default);
	else
		lv_style_init(&style_screen_2_img_1_main_main_default);
	lv_style_set_img_recolor(&style_screen_2_img_1_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_2_img_1_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_2_img_1_main_main_default, 255);
	lv_obj_add_style(ui->screen_2_img_1, &style_screen_2_img_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_2_img_1, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_2_img_1,&_curtain_icon_115x115);
	lv_img_set_pivot(ui->screen_2_img_1, 50,50);
	lv_img_set_angle(ui->screen_2_img_1, 0);

	//Write codes screen_2_img_2
	ui->screen_2_img_2 = lv_img_create(ui->screen_2);
	lv_obj_set_pos(ui->screen_2_img_2, 225, 25);
	lv_obj_set_size(ui->screen_2_img_2, 225, 25);
	lv_obj_set_scrollbar_mode(ui->screen_2_img_2, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_2_img_2_main_main_default
	static lv_style_t style_screen_2_img_2_main_main_default;
	if (style_screen_2_img_2_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_2_img_2_main_main_default);
	else
		lv_style_init(&style_screen_2_img_2_main_main_default);
	lv_style_set_img_recolor(&style_screen_2_img_2_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_2_img_2_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_2_img_2_main_main_default, 255);
	lv_obj_add_style(ui->screen_2_img_2, &style_screen_2_img_2_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_2_img_2, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_2_img_2,&_cur_btn_225x25);
	lv_img_set_pivot(ui->screen_2_img_2, 50,50);
	lv_img_set_angle(ui->screen_2_img_2, 0);

	//Write codes screen_2_img_3
	ui->screen_2_img_3 = lv_img_create(ui->screen_2);
	lv_obj_set_pos(ui->screen_2_img_3, 337, 22);
	lv_obj_set_size(ui->screen_2_img_3, 193, 203);
	lv_obj_set_scrollbar_mode(ui->screen_2_img_3, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_2_img_3_main_main_default
	static lv_style_t style_screen_2_img_3_main_main_default;
	if (style_screen_2_img_3_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_2_img_3_main_main_default);
	else
		lv_style_init(&style_screen_2_img_3_main_main_default);
	lv_style_set_img_recolor(&style_screen_2_img_3_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_2_img_3_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_2_img_3_main_main_default, 255);
	lv_obj_add_style(ui->screen_2_img_3, &style_screen_2_img_3_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_2_img_3, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_2_img_3,&_mode_fun_big_193x203);
	lv_img_set_pivot(ui->screen_2_img_3, 50,50);
	lv_img_set_angle(ui->screen_2_img_3, 0);

	//Write codes screen_2_img_4
	ui->screen_2_img_4 = lv_img_create(ui->screen_2);
	lv_obj_set_pos(ui->screen_2_img_4, 477, 22);
	lv_obj_set_size(ui->screen_2_img_4, 115, 120);
	lv_obj_set_scrollbar_mode(ui->screen_2_img_4, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_2_img_4_main_main_default
	static lv_style_t style_screen_2_img_4_main_main_default;
	if (style_screen_2_img_4_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_2_img_4_main_main_default);
	else
		lv_style_init(&style_screen_2_img_4_main_main_default);
	lv_style_set_img_recolor(&style_screen_2_img_4_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_2_img_4_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_2_img_4_main_main_default, 255);
	lv_obj_add_style(ui->screen_2_img_4, &style_screen_2_img_4_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_2_img_4, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_2_img_4,&_mode_fun_small_115x120);
	lv_img_set_pivot(ui->screen_2_img_4, 50,50);
	lv_img_set_angle(ui->screen_2_img_4, 0);

	//Write codes screen_2_img_5
	ui->screen_2_img_5 = lv_img_create(ui->screen_2);
	lv_obj_set_pos(ui->screen_2_img_5, 570, 22);
	lv_obj_set_size(ui->screen_2_img_5, 193, 206);
	lv_obj_set_scrollbar_mode(ui->screen_2_img_5, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_2_img_5_main_main_default
	static lv_style_t style_screen_2_img_5_main_main_default;
	if (style_screen_2_img_5_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_2_img_5_main_main_default);
	else
		lv_style_init(&style_screen_2_img_5_main_main_default);
	lv_style_set_img_recolor(&style_screen_2_img_5_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_2_img_5_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_2_img_5_main_main_default, 255);
	lv_obj_add_style(ui->screen_2_img_5, &style_screen_2_img_5_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_2_img_5, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_2_img_5,&_mode_leave_big_193x206);
	lv_img_set_pivot(ui->screen_2_img_5, 50,50);
	lv_img_set_angle(ui->screen_2_img_5, 0);

	//Write codes screen_2_img_6
	ui->screen_2_img_6 = lv_img_create(ui->screen_2);
	lv_obj_set_pos(ui->screen_2_img_6, 23, 320);
	lv_obj_set_size(ui->screen_2_img_6, 116, 120);
	lv_obj_set_scrollbar_mode(ui->screen_2_img_6, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_2_img_6_main_main_default
	static lv_style_t style_screen_2_img_6_main_main_default;
	if (style_screen_2_img_6_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_2_img_6_main_main_default);
	else
		lv_style_init(&style_screen_2_img_6_main_main_default);
	lv_style_set_img_recolor(&style_screen_2_img_6_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_2_img_6_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_2_img_6_main_main_default, 255);
	lv_obj_add_style(ui->screen_2_img_6, &style_screen_2_img_6_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_2_img_6, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_2_img_6,&_mode_leave_small_116x120);
	lv_img_set_pivot(ui->screen_2_img_6, 50,50);
	lv_img_set_angle(ui->screen_2_img_6, 0);

	//Write codes screen_2_img_7
	ui->screen_2_img_7 = lv_img_create(ui->screen_2);
	lv_obj_set_pos(ui->screen_2_img_7, 182, 313);
	lv_obj_set_size(ui->screen_2_img_7, 194, 204);
	lv_obj_set_scrollbar_mode(ui->screen_2_img_7, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_2_img_7_main_main_default
	static lv_style_t style_screen_2_img_7_main_main_default;
	if (style_screen_2_img_7_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_2_img_7_main_main_default);
	else
		lv_style_init(&style_screen_2_img_7_main_main_default);
	lv_style_set_img_recolor(&style_screen_2_img_7_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_2_img_7_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_2_img_7_main_main_default, 255);
	lv_obj_add_style(ui->screen_2_img_7, &style_screen_2_img_7_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_2_img_7, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_2_img_7,&_mode_sleep_big_194x204);
	lv_img_set_pivot(ui->screen_2_img_7, 50,50);
	lv_img_set_angle(ui->screen_2_img_7, 0);

	//Write codes screen_2_img_8
	ui->screen_2_img_8 = lv_img_create(ui->screen_2);
	lv_obj_set_pos(ui->screen_2_img_8, 362, 320);
	lv_obj_set_size(ui->screen_2_img_8, 114, 120);
	lv_obj_set_scrollbar_mode(ui->screen_2_img_8, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_2_img_8_main_main_default
	static lv_style_t style_screen_2_img_8_main_main_default;
	if (style_screen_2_img_8_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_2_img_8_main_main_default);
	else
		lv_style_init(&style_screen_2_img_8_main_main_default);
	lv_style_set_img_recolor(&style_screen_2_img_8_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_2_img_8_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_2_img_8_main_main_default, 255);
	lv_obj_add_style(ui->screen_2_img_8, &style_screen_2_img_8_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_2_img_8, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_2_img_8,&_mode_sleep_small_114x120);
	lv_img_set_pivot(ui->screen_2_img_8, 50,50);
	lv_img_set_angle(ui->screen_2_img_8, 0);

	//Write codes screen_2_img_9
	ui->screen_2_img_9 = lv_img_create(ui->screen_2);
	lv_obj_set_pos(ui->screen_2_img_9, 520, 320);
	lv_obj_set_size(ui->screen_2_img_9, 190, 33);
	lv_obj_set_scrollbar_mode(ui->screen_2_img_9, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_2_img_9_main_main_default
	static lv_style_t style_screen_2_img_9_main_main_default;
	if (style_screen_2_img_9_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_2_img_9_main_main_default);
	else
		lv_style_init(&style_screen_2_img_9_main_main_default);
	lv_style_set_img_recolor(&style_screen_2_img_9_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_2_img_9_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_2_img_9_main_main_default, 255);
	lv_obj_add_style(ui->screen_2_img_9, &style_screen_2_img_9_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_2_img_9, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_2_img_9,&_music_btn_190x33);
	lv_img_set_pivot(ui->screen_2_img_9, 50,50);
	lv_img_set_angle(ui->screen_2_img_9, 0);

	//Write codes screen_2_img_10
	ui->screen_2_img_10 = lv_img_create(ui->screen_2);
	lv_obj_set_pos(ui->screen_2_img_10, 697, 313);
	lv_obj_set_size(ui->screen_2_img_10, 318, 277);
	lv_obj_set_scrollbar_mode(ui->screen_2_img_10, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_2_img_10_main_main_default
	static lv_style_t style_screen_2_img_10_main_main_default;
	if (style_screen_2_img_10_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_2_img_10_main_main_default);
	else
		lv_style_init(&style_screen_2_img_10_main_main_default);
	lv_style_set_img_recolor(&style_screen_2_img_10_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_2_img_10_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_2_img_10_main_main_default, 255);
	lv_obj_add_style(ui->screen_2_img_10, &style_screen_2_img_10_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_2_img_10, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_2_img_10,&_music_icon_318x277);
	lv_img_set_pivot(ui->screen_2_img_10, 50,50);
	lv_img_set_angle(ui->screen_2_img_10, 0);

	//Write codes screen_2_label_1
	ui->screen_2_label_1 = lv_label_create(ui->screen_2);
	lv_obj_set_pos(ui->screen_2_label_1, 34, 195);
	lv_obj_set_size(ui->screen_2_label_1, 230, 47);
	lv_obj_set_scrollbar_mode(ui->screen_2_label_1, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_2_label_1, "星期一二三四五六日");
	lv_label_set_long_mode(ui->screen_2_label_1, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_2_label_1_main_main_default
	static lv_style_t style_screen_2_label_1_main_main_default;
	if (style_screen_2_label_1_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_2_label_1_main_main_default);
	else
		lv_style_init(&style_screen_2_label_1_main_main_default);
	lv_style_set_radius(&style_screen_2_label_1_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_2_label_1_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_2_label_1_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_2_label_1_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_2_label_1_main_main_default, 0);
	lv_style_set_text_color(&style_screen_2_label_1_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_2_label_1_main_main_default, &lv_font_MiSans_Demibold_16);
	lv_style_set_text_letter_space(&style_screen_2_label_1_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_2_label_1_main_main_default, 0);
	lv_style_set_text_align(&style_screen_2_label_1_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_2_label_1_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_2_label_1_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_2_label_1_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_2_label_1_main_main_default, 0);
	lv_obj_add_style(ui->screen_2_label_1, &style_screen_2_label_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_2_label_2
	ui->screen_2_label_2 = lv_label_create(ui->screen_2);
	lv_obj_set_pos(ui->screen_2_label_2, 34, 476);
	lv_obj_set_size(ui->screen_2_label_2, 58, 41);
	lv_obj_set_scrollbar_mode(ui->screen_2_label_2, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_2_label_2, "中雨大小多云晴");
	lv_label_set_long_mode(ui->screen_2_label_2, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_2_label_2_main_main_default
	static lv_style_t style_screen_2_label_2_main_main_default;
	if (style_screen_2_label_2_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_2_label_2_main_main_default);
	else
		lv_style_init(&style_screen_2_label_2_main_main_default);
	lv_style_set_radius(&style_screen_2_label_2_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_2_label_2_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_2_label_2_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_2_label_2_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_2_label_2_main_main_default, 0);
	lv_style_set_text_color(&style_screen_2_label_2_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_2_label_2_main_main_default, &lv_font_MiSans_Demibold_13);
	lv_style_set_text_letter_space(&style_screen_2_label_2_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_2_label_2_main_main_default, 0);
	lv_style_set_text_align(&style_screen_2_label_2_main_main_default, LV_TEXT_ALIGN_LEFT);
	lv_style_set_pad_left(&style_screen_2_label_2_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_2_label_2_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_2_label_2_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_2_label_2_main_main_default, 0);
	lv_obj_add_style(ui->screen_2_label_2, &style_screen_2_label_2_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_2_label_3
	ui->screen_2_label_3 = lv_label_create(ui->screen_2);
	lv_obj_set_pos(ui->screen_2_label_3, 282, 228);
	lv_obj_set_size(ui->screen_2_label_3, 121, 81);
	lv_obj_set_scrollbar_mode(ui->screen_2_label_3, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_2_label_3, "模式选择娱乐离家睡眠");
	lv_label_set_long_mode(ui->screen_2_label_3, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_2_label_3_main_main_default
	static lv_style_t style_screen_2_label_3_main_main_default;
	if (style_screen_2_label_3_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_2_label_3_main_main_default);
	else
		lv_style_init(&style_screen_2_label_3_main_main_default);
	lv_style_set_radius(&style_screen_2_label_3_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_2_label_3_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_2_label_3_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_2_label_3_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_2_label_3_main_main_default, 0);
	lv_style_set_text_color(&style_screen_2_label_3_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_2_label_3_main_main_default, &lv_font_MiSans_Demibold_20);
	lv_style_set_text_letter_space(&style_screen_2_label_3_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_2_label_3_main_main_default, 0);
	lv_style_set_text_align(&style_screen_2_label_3_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_2_label_3_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_2_label_3_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_2_label_3_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_2_label_3_main_main_default, 0);
	lv_obj_add_style(ui->screen_2_label_3, &style_screen_2_label_3_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
}