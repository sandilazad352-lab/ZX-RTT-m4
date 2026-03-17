/*
 * Copyright 2024 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"

#include "bee_anim.h"
#include "bee_ui_init.h"

extern char* read_png_path[];

void screen_anim(lv_ui* ui);

void lv_timer_cb(lv_timer_t* t){
    lv_ui* ui = t->user_data;
	static int time_index = 0;
	static bool index_sign = true;

	lv_obj_invalidate(ui->screen_img_1);
	lv_img_set_antialias(ui->screen_img_1, true);
	lv_img_set_size_mode(ui->screen_img_1, LV_IMG_SIZE_MODE_REAL);
	lv_img_set_zoom(ui->screen_img_1, LV_IMG_ZOOM_NONE - time_index);
	// lv_obj_set_pos(ui->screen_img_4, 0, 0);
	// lv_obj_set_pos(ui->screen_img_1, 20, 20);

	if(index_sign){
		time_index++;
	}else{
		time_index--;
	}

	if(time_index >= 105){
		time_index = 105;
		index_sign = false;
	}else if(time_index <= 0){
		time_index = 0;
		index_sign = true;
	}
}

void screen_flag_init(lv_ui* ui){
	// //对屏幕的操作 
	lv_obj_clear_flag(ui->screen, LV_OBJ_FLAG_OVERFLOW_VISIBLE);
    lv_obj_clear_flag(ui->screen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_clear_flag(ui->screen, LV_OBJ_FLAG_SCROLL_ELASTIC);
    lv_obj_clear_flag(ui->screen, LV_OBJ_FLAG_SCROLL_ONE);
    lv_obj_clear_flag(ui->screen, LV_OBJ_FLAG_SNAPPABLE);
    lv_obj_clear_flag(ui->screen, LV_OBJ_FLAG_FLOATING);	

	lv_obj_add_flag(ui->screen_img_1, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(ui->screen_img_2, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(ui->screen_img_3, LV_OBJ_FLAG_CLICKABLE);

	lv_obj_add_flag(ui->screen_img_m6, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(ui->screen_img_m7, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(ui->screen_img_m8, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(ui->screen_img_m9, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(ui->screen_img_m10, LV_OBJ_FLAG_CLICKABLE);

	lv_obj_add_flag(ui->screen_img_m4, LV_OBJ_FLAG_CLICKABLE);

	// lv_obj_add_flag(ui->screen_img_m14, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(ui->screen_img_m15, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(ui->screen_img_m16, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(ui->screen_img_m17, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(ui->screen_img_m18, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(ui->screen_img_m19, LV_OBJ_FLAG_CLICKABLE);
	// lv_obj_add_flag(ui->screen_img_m20, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(ui->screen_img_m21, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(ui->screen_img_m22, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(ui->screen_img_m23, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(ui->screen_img_m24, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(ui->screen_img_m25, LV_OBJ_FLAG_CLICKABLE);
	// lv_obj_add_flag(ui->screen_img_m26, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(ui->screen_img_m27, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(ui->screen_img_m28, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(ui->screen_img_m29, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(ui->screen_img_m30, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(ui->screen_img_m31, LV_OBJ_FLAG_CLICKABLE);
	// lv_obj_add_flag(ui->screen_img_m32, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(ui->screen_img_m33, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(ui->screen_img_m34, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(ui->screen_img_m35, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(ui->screen_img_m36, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(ui->screen_img_m37, LV_OBJ_FLAG_CLICKABLE);

	lv_obj_add_flag(ui->screen_label_39, LV_OBJ_FLAG_CLICKABLE);

	lv_obj_clear_flag(ui->screen_sw_1, LV_OBJ_FLAG_CLICKABLE);
}

void setup_scr_screen(lv_ui *ui){

	//Write codes screen
	ui->screen = lv_obj_create(NULL);
	lv_obj_set_scrollbar_mode(ui->screen, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_main_main_default
	static lv_style_t style_screen_main_main_default;
	if (style_screen_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_main_main_default);
	else
		lv_style_init(&style_screen_main_main_default);
	lv_style_set_bg_color(&style_screen_main_main_default, lv_color_hex(0x454C7E));
	lv_style_set_bg_opa(&style_screen_main_main_default, 255);
	lv_obj_add_style(ui->screen, &style_screen_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

/* ====================== 解码读取PNG -- begin*/
	// //Write codes screen_img_m1
	// ui->screen_img_m1 = lv_img_create(ui->screen);
	// lv_obj_set_pos(ui->screen_img_m1, 0, 0);
	// // lv_obj_set_scrollbar_mode(ui->screen_img_m1, LV_SCROLLBAR_MODE_OFF);
	// lv_img_set_src(ui->screen_img_m1,read_png_path[MAIN_BOTTOM]);
	// lv_obj_set_style_img_opa(ui->screen_img_m1, 255, 0);

	//Write codes screen_img_m2
	ui->screen_img_m2 = lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m2, 340, 27);
	lv_img_set_src(ui->screen_img_m2,read_png_path[WIFI_ICON]);

	// //Write codes screen_img_m3
	// ui->screen_img_m3 = lv_img_create(ui->screen);
	// // lv_obj_set_pos(ui->screen_img_m3, 428, 20);
	// // lv_img_set_src(ui->screen_img_m3,read_png_path[MAIN_MODE_BOTTOM]);
	// lv_obj_set_pos(ui->screen_img_m3, 463, 20);
	// lv_img_set_src(ui->screen_img_m3,read_png_path[TEST_MODE]);
		//Write codes screen_img_m3
	ui->screen_img_m3 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m3, 463, 20);
	lv_obj_set_size(ui->screen_img_m3, 213, 321);
	lv_obj_set_scrollbar_mode(ui->screen_img_m3, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_img_m3, "");
	lv_label_set_long_mode(ui->screen_img_m3, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_img_m3_main_main_default
	static lv_style_t style_screen_img_m3_main_main_default;
	if (style_screen_img_m3_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_img_m3_main_main_default);
	else
		lv_style_init(&style_screen_img_m3_main_main_default);
	lv_style_set_radius(&style_screen_img_m3_main_main_default, 20);
	lv_style_set_bg_color(&style_screen_img_m3_main_main_default, lv_color_hex(0x312C74));
	lv_style_set_bg_grad_color(&style_screen_img_m3_main_main_default, lv_color_hex(0x312C74));
	lv_style_set_bg_grad_dir(&style_screen_img_m3_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_img_m3_main_main_default, 255);
	lv_style_set_text_color(&style_screen_img_m3_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_letter_space(&style_screen_img_m3_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_img_m3_main_main_default, 0);
	lv_style_set_text_align(&style_screen_img_m3_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_img_m3_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_img_m3_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_img_m3_main_main_default, 8);
	lv_style_set_pad_bottom(&style_screen_img_m3_main_main_default, 0);
	lv_obj_add_style(ui->screen_img_m3, &style_screen_img_m3_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	// //Write codes screen_img_m4
	// ui->screen_img_m4 = lv_img_create(ui->screen);
	// lv_obj_set_pos(ui->screen_img_m4, 685, 19);
	// // lv_obj_set_size(ui->screen_img_m4, 115, 120);
	// // lv_obj_set_scrollbar_mode(ui->screen_img_m4, LV_SCROLLBAR_MODE_OFF);
	// lv_img_set_src(ui->screen_img_m4,read_png_path[TEST_MUSIC]);
		//Write codes screen_img_m4
	ui->screen_img_m4 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m4, 685, 19);
	lv_obj_set_size(ui->screen_img_m4, 335, 321);
	lv_obj_set_scrollbar_mode(ui->screen_img_m4, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_img_m4, "");
	lv_label_set_long_mode(ui->screen_img_m4, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_img_m4_main_main_default
	static lv_style_t style_screen_img_m4_main_main_default;
	if (style_screen_img_m4_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_img_m4_main_main_default);
	else
		lv_style_init(&style_screen_img_m4_main_main_default);
	lv_style_set_radius(&style_screen_img_m4_main_main_default, 20);
	lv_style_set_bg_color(&style_screen_img_m4_main_main_default, lv_color_hex(0x312C74));
	lv_style_set_bg_grad_color(&style_screen_img_m4_main_main_default, lv_color_hex(0x312C74));
	lv_style_set_bg_grad_dir(&style_screen_img_m4_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_img_m4_main_main_default, 255);
	lv_style_set_text_color(&style_screen_img_m4_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_letter_space(&style_screen_img_m4_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_img_m4_main_main_default, 0);
	lv_style_set_text_align(&style_screen_img_m4_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_img_m4_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_img_m4_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_img_m4_main_main_default, 8);
	lv_style_set_pad_bottom(&style_screen_img_m4_main_main_default, 0);
	lv_obj_add_style(ui->screen_img_m4, &style_screen_img_m4_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	// //Write codes screen_img_m5
	// ui->screen_img_m5 = lv_img_create(ui->screen);
	// lv_obj_set_pos(ui->screen_img_m5, 20, 283);
	// lv_img_set_src(ui->screen_img_m5,read_png_path[TEST_WEA]);
		//Write codes screen_img_m5
	ui->screen_img_m5 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m5, 20, 283);
	lv_obj_set_size(ui->screen_img_m5, 433, 311);
	lv_obj_set_scrollbar_mode(ui->screen_img_m5, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_img_m5, "");
	lv_label_set_long_mode(ui->screen_img_m5, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_img_m5_main_main_default
	static lv_style_t style_screen_img_m5_main_main_default;
	if (style_screen_img_m5_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_img_m5_main_main_default);
	else
		lv_style_init(&style_screen_img_m5_main_main_default);
	lv_style_set_radius(&style_screen_img_m5_main_main_default, 20);
	lv_style_set_bg_color(&style_screen_img_m5_main_main_default, lv_color_hex(0x312C74));
	lv_style_set_bg_grad_color(&style_screen_img_m5_main_main_default, lv_color_hex(0x312C74));
	lv_style_set_bg_grad_dir(&style_screen_img_m5_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_img_m5_main_main_default, 255);
	lv_style_set_text_color(&style_screen_img_m5_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_letter_space(&style_screen_img_m5_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_img_m5_main_main_default, 0);
	lv_style_set_text_align(&style_screen_img_m5_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_img_m5_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_img_m5_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_img_m5_main_main_default, 8);
	lv_style_set_pad_bottom(&style_screen_img_m5_main_main_default, 0);
	lv_obj_add_style(ui->screen_img_m5, &style_screen_img_m5_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	// //Write codes test_mode
	// ui->test_mode = lv_img_create(ui->screen);
	// lv_obj_set_pos(ui->test_mode, 463, 20);
	// lv_img_set_src(ui->test_mode,read_png_path[TEST_MODE]);

	// //Write codes test_music
	// ui->test_music = lv_img_create(ui->screen);
	// lv_obj_set_pos(ui->test_music, 685, 19);
	// lv_img_set_src(ui->test_music,read_png_path[TEST_MUSIC]);

	// //Write codes test_wea
	// ui->test_wea = lv_img_create(ui->screen);
	// lv_obj_set_pos(ui->test_wea, 20, 283);
	// lv_img_set_src(ui->test_wea,read_png_path[TEST_WEA]);


	//Write codes screen_label_39
	ui->screen_label_39 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_39, 470, 354);
	lv_obj_set_size(ui->screen_label_39, 189, 234);
	lv_obj_set_scrollbar_mode(ui->screen_label_39, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_39, "");
	lv_label_set_long_mode(ui->screen_label_39, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_39_main_main_default
	static lv_style_t style_screen_label_39_main_main_default;
	if (style_screen_label_39_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_39_main_main_default);
	else
		lv_style_init(&style_screen_label_39_main_main_default);
	lv_style_set_radius(&style_screen_label_39_main_main_default, 20);
	lv_style_set_bg_color(&style_screen_label_39_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_label_39_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_39_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_39_main_main_default, 255);
	lv_style_set_text_color(&style_screen_label_39_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_letter_space(&style_screen_label_39_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_label_39_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_39_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_39_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_39_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_39_main_main_default, 8);
	lv_style_set_pad_bottom(&style_screen_label_39_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_39, &style_screen_label_39_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);


	//Write codes screen_img_m38
	ui->screen_img_m38 = lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m38, 661, 354);
	lv_img_set_src(ui->screen_img_m38,read_png_path[SMART_TV]);

	//Write codes screen_img_m39
	ui->screen_img_m39 = lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m39, 852, 354);
	lv_img_set_src(ui->screen_img_m39,read_png_path[SMART_AIR]);

	//Write codes screen_img_m6
	ui->screen_img_m6 = lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m6, 50, 427);
	lv_img_set_src(ui->screen_img_m6,read_png_path[BTN_NOPRESS]);
	//Write codes screen_img_m7
	ui->screen_img_m7 = lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m7, 127, 427);
	lv_img_set_src(ui->screen_img_m7,read_png_path[BTN_NOPRESS]);
	//Write codes screen_img_m8
	ui->screen_img_m8 = lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m8, 204, 427);
	lv_img_set_src(ui->screen_img_m8,read_png_path[BTN_NOPRESS]);
	//Write codes screen_img_m9
	ui->screen_img_m9 = lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m9, 281, 427);
	lv_img_set_src(ui->screen_img_m9,read_png_path[BTN_NOPRESS]);
	//Write codes screen_img_m10
	ui->screen_img_m10= lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m10, 358, 427);
	lv_img_set_src(ui->screen_img_m10,read_png_path[BTN_NOPRESS]);

	//Write codes screen_img_m40
	ui->screen_img_m40= lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m40, 44, 452);
	lv_img_set_src(ui->screen_img_m40,read_png_path[WEA_RAIN_SMALL]);
	//Write codes screen_img_m41
	ui->screen_img_m41= lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m41, 121, 452);
	lv_img_set_src(ui->screen_img_m41,read_png_path[WEA_RAIN_SMALL]);
	//Write codes screen_img_m42
	ui->screen_img_m42= lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m42, 198, 453);
	lv_img_set_src(ui->screen_img_m42,read_png_path[WEA_CLO_SMALL]);
	//Write codes screen_img_m43
	ui->screen_img_m43= lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m43, 295, 474);
	lv_img_set_src(ui->screen_img_m43,read_png_path[WEA_SUN_SMALL]);
	//Write codes screen_img_m44
	ui->screen_img_m44= lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m44, 372, 474);
	lv_img_set_src(ui->screen_img_m44,read_png_path[WEA_SUN_SMALL]);
	// //Write codes screen_img_m45
	// ui->screen_img_m45= lv_img_create(ui->screen);
	// lv_obj_set_pos(ui->screen_img_m45, 250, 247);
	// lv_img_set_src(ui->screen_img_m45,read_png_path[WEA_SUN_BIG]);

	//Write codes screen_img_m45
	ui->screen_img_m45= lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m45, 250, 247);
	lv_img_set_src(ui->screen_img_m45,read_png_path[WEA_SUN_BIG]);

	//Write codes screen_img_m46
	ui->screen_img_m46= lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m46, 690, 18);
	lv_img_set_src(ui->screen_img_m46,&_music_icon_318x277);
	//Write codes screen_img_m47
	ui->screen_img_m47= lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m47, 794, 90);
	lv_img_set_src(ui->screen_img_m47,&_curtain_icon_115x115);

	//Write codes screen_img_m48
	ui->screen_img_m48= lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m48, 757, 296);
	lv_img_set_src(ui->screen_img_m48,&_music_btn_190x33);
	//Write codes screen_img_m49
	ui->screen_img_m49= lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m49, 742, 296);
	lv_img_set_src(ui->screen_img_m49,&_cur_btn_225x25);

	// //Write codes screen_img_m13
	// ui->screen_img_m13 = lv_img_create(ui->screen);
	// lv_obj_set_pos(ui->screen_img_m13, 488, 376);
	// lv_img_set_src(ui->screen_img_m13,read_png_path[LIGHT]);

	// //Write codes screen_img_m20
	// ui->screen_img_m20 = lv_img_create(ui->screen);
	// lv_obj_set_pos(ui->screen_img_m20, 865, 387);
	// lv_img_set_src(ui->screen_img_m20,read_png_path[LIGHT_OFF]);

	//Write codes screen_img_m14
	ui->screen_img_m14 = lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m14, 717, 387);
	lv_img_set_src(ui->screen_img_m14,read_png_path[LIGHT_OFF]);
	//Write codes screen_img_m15
	ui->screen_img_m15 = lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m15, 768, 378);
	lv_img_set_src(ui->screen_img_m15,read_png_path[SIGN_COLOR]);	
	//Write codes screen_img_m16
	ui->screen_img_m16 = lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m16, 773, 383);
	lv_img_set_src(ui->screen_img_m16,read_png_path[SIGN_YELLOW]);	
	//Write codes screen_img_m17
	ui->screen_img_m17 = lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m17, 778, 408);
	lv_img_set_src(ui->screen_img_m17,read_png_path[SIGN_PINK]);	
	//Write codes screen_img_m18
	ui->screen_img_m18 = lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m18, 778, 428);
	lv_img_set_src(ui->screen_img_m18,read_png_path[SIGN_PURPLE]);	
	//Write codes screen_img_m19
	ui->screen_img_m19 = lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m19, 778, 448);
	lv_img_set_src(ui->screen_img_m19,read_png_path[SIGN_BLUE]);
	//Write codes screen_img_m20
	ui->screen_img_m20 = lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m20, 865, 387);
	lv_img_set_src(ui->screen_img_m20,read_png_path[LIGHT_OFF]);
	//Write codes screen_img_m21
	ui->screen_img_m21 = lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m21, 916, 378);
	lv_img_set_src(ui->screen_img_m21,read_png_path[SIGN_COLOR]);	
	//Write codes screen_img_m22
	ui->screen_img_m22 = lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m22, 921, 383);
	lv_img_set_src(ui->screen_img_m22,read_png_path[SIGN_YELLOW]);	
	//Write codes screen_img_m23
	ui->screen_img_m23 = lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m23, 926, 403);
	lv_img_set_src(ui->screen_img_m23,read_png_path[SIGN_PINK]);	
	//Write codes screen_img_m24
	ui->screen_img_m24 = lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m24, 926, 423);
	lv_img_set_src(ui->screen_img_m24,read_png_path[SIGN_PURPLE]);	
	//Write codes screen_img_m25
	ui->screen_img_m25 = lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m25, 926, 443);
	lv_img_set_src(ui->screen_img_m25,read_png_path[SIGN_BLUE]);
	//Write codes screen_img_m26
	ui->screen_img_m26 = lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m26, 717, 489);
	lv_img_set_src(ui->screen_img_m26,read_png_path[LIGHT_OFF]);
	//Write codes screen_img_m27
	ui->screen_img_m27 = lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m27, 768, 480);
	lv_img_set_src(ui->screen_img_m27,read_png_path[SIGN_COLOR]);	
	//Write codes screen_img_m28
	ui->screen_img_m28 = lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m28, 773, 485);
	lv_img_set_src(ui->screen_img_m28,read_png_path[SIGN_YELLOW]);	
	//Write codes screen_img_m29
	ui->screen_img_m29 = lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m29, 778, 510);
	lv_img_set_src(ui->screen_img_m29,read_png_path[SIGN_PINK]);	
	//Write codes screen_img_m30
	ui->screen_img_m30 = lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m30, 778, 530);
	lv_img_set_src(ui->screen_img_m30,read_png_path[SIGN_PURPLE]);	
	//Write codes screen_img_m31
	ui->screen_img_m31 = lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m31, 778, 550);
	lv_img_set_src(ui->screen_img_m31,read_png_path[SIGN_BLUE]);
	//Write codes screen_img_m32
	ui->screen_img_m32 = lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m32, 865, 489);
	lv_img_set_src(ui->screen_img_m32,read_png_path[LIGHT_OFF]);
	//Write codes screen_img_m33
	ui->screen_img_m33 = lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m33, 916, 480);
	lv_img_set_src(ui->screen_img_m33,read_png_path[SIGN_COLOR]);	
	//Write codes screen_img_m34
	ui->screen_img_m34 = lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m34, 921, 485);
	lv_img_set_src(ui->screen_img_m34,read_png_path[SIGN_YELLOW]);	
	//Write codes screen_img_m35
	ui->screen_img_m35 = lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m35, 926, 510);
	lv_img_set_src(ui->screen_img_m35,read_png_path[SIGN_PINK]);	
	//Write codes screen_img_m36
	ui->screen_img_m36 = lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m36, 926, 530);
	lv_img_set_src(ui->screen_img_m36,read_png_path[SIGN_PURPLE]);	
	//Write codes screen_img_m37
	ui->screen_img_m37 = lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m37, 926, 550);
	lv_img_set_src(ui->screen_img_m37,read_png_path[SIGN_BLUE]);
/* ====================== 解码读取PNG -- end*/

	//Write codes screen_label_1
	ui->screen_label_1 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_1, 30, 24);
	lv_obj_set_size(ui->screen_label_1, 250, 40);
	lv_obj_set_scrollbar_mode(ui->screen_label_1, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_1, "Shenzhen,China");
	lv_label_set_long_mode(ui->screen_label_1, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_1_main_main_default
	static lv_style_t style_screen_label_1_main_main_default;
	if (style_screen_label_1_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_1_main_main_default);
	else
		lv_style_init(&style_screen_label_1_main_main_default);
	lv_style_set_radius(&style_screen_label_1_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_1_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_1_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_1_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_1_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_1_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_1_main_main_default, &lv_font_SF_Pro_Display_Medium_34);
	lv_style_set_text_letter_space(&style_screen_label_1_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_label_1_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_1_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_1_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_1_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_1_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_1_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_1, &style_screen_label_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_2
	ui->screen_label_2 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_2, 29, 68);
	lv_obj_set_size(ui->screen_label_2, 372, 132);
	lv_obj_set_scrollbar_mode(ui->screen_label_2, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_2, "20:00");
	lv_label_set_long_mode(ui->screen_label_2, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_2_main_main_default
	static lv_style_t style_screen_label_2_main_main_default;
	if (style_screen_label_2_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_2_main_main_default);
	else
		lv_style_init(&style_screen_label_2_main_main_default);
	lv_style_set_radius(&style_screen_label_2_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_2_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_2_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_2_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_2_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_2_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_2_main_main_default, &lv_font_SF_Pro_Display_Medium_136);
	lv_style_set_text_letter_space(&style_screen_label_2_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_label_2_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_2_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_2_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_2_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_2_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_2_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_2, &style_screen_label_2_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_3
	ui->screen_label_3 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_3, 35, 208);
	lv_obj_set_size(ui->screen_label_3, 115, 28);
	lv_obj_set_scrollbar_mode(ui->screen_label_3, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_3, "Mostly Clear");
	lv_label_set_long_mode(ui->screen_label_3, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_3_main_main_default
	static lv_style_t style_screen_label_3_main_main_default;
	if (style_screen_label_3_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_3_main_main_default);
	else
		lv_style_init(&style_screen_label_3_main_main_default);
	lv_style_set_radius(&style_screen_label_3_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_3_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_3_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_3_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_3_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_3_main_main_default, lv_color_make(0xA6, 0xA9, 0xC0));
	lv_style_set_text_font(&style_screen_label_3_main_main_default, &lv_font_SF_Pro_Display_Medium_20);
	lv_style_set_text_letter_space(&style_screen_label_3_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_label_3_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_3_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_3_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_3_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_3_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_3_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_3, &style_screen_label_3_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_4
	ui->screen_label_4 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_4, 27, 233);
	lv_obj_set_size(ui->screen_label_4, 72, 22);
	lv_obj_set_scrollbar_mode(ui->screen_label_4, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_4, "H:24°");
	lv_label_set_long_mode(ui->screen_label_4, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_4_main_main_default
	static lv_style_t style_screen_label_4_main_main_default;
	if (style_screen_label_4_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_4_main_main_default);
	else
		lv_style_init(&style_screen_label_4_main_main_default);
	lv_style_set_radius(&style_screen_label_4_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_4_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_4_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_4_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_4_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_4_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_4_main_main_default, &lv_font_SF_Pro_Display_Medium_20);
	lv_style_set_text_letter_space(&style_screen_label_4_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_label_4_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_4_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_4_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_4_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_4_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_4_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_4, &style_screen_label_4_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_5
	ui->screen_label_5 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_5, 95, 233);
	lv_obj_set_size(ui->screen_label_5, 62, 21);
	lv_obj_set_scrollbar_mode(ui->screen_label_5, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_5, "L:18°");
	lv_label_set_long_mode(ui->screen_label_5, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_5_main_main_default
	static lv_style_t style_screen_label_5_main_main_default;
	if (style_screen_label_5_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_5_main_main_default);
	else
		lv_style_init(&style_screen_label_5_main_main_default);
	lv_style_set_radius(&style_screen_label_5_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_5_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_5_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_5_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_5_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_5_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_5_main_main_default, &lv_font_SF_Pro_Display_Medium_20);
	lv_style_set_text_letter_space(&style_screen_label_5_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_label_5_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_5_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_5_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_5_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_5_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_5_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_5, &style_screen_label_5_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_6
	ui->screen_label_6 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_6, 167, 213);
	lv_obj_set_size(ui->screen_label_6, 58, 18);
	lv_obj_set_scrollbar_mode(ui->screen_label_6, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_6, "星期二");
	lv_label_set_long_mode(ui->screen_label_6, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_6_main_main_default
	static lv_style_t style_screen_label_6_main_main_default;
	if (style_screen_label_6_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_6_main_main_default);
	else
		lv_style_init(&style_screen_label_6_main_main_default);
	lv_style_set_radius(&style_screen_label_6_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_6_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_6_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_6_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_6_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_6_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_6_main_main_default, &lv_font_MiSans_Demibold_16);
	lv_style_set_text_letter_space(&style_screen_label_6_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_label_6_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_6_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_6_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_6_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_6_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_6_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_6, &style_screen_label_6_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_7
	ui->screen_label_7 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_7, 164, 233);
	lv_obj_set_size(ui->screen_label_7, 120, 21);
	lv_obj_set_scrollbar_mode(ui->screen_label_7, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_7, "2023/09/16");
	lv_label_set_long_mode(ui->screen_label_7, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_7_main_main_default
	static lv_style_t style_screen_label_7_main_main_default;
	if (style_screen_label_7_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_7_main_main_default);
	else
		lv_style_init(&style_screen_label_7_main_main_default);
	lv_style_set_radius(&style_screen_label_7_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_7_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_7_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_7_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_7_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_7_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_7_main_main_default, &lv_font_SF_Pro_Display_Medium_20);
	lv_style_set_text_letter_space(&style_screen_label_7_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_label_7_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_7_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_7_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_7_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_7_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_7_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_7, &style_screen_label_7_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_8
	ui->screen_label_8 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_8, 36, 294);
	lv_obj_set_size(ui->screen_label_8, 119, 75);
	lv_obj_set_scrollbar_mode(ui->screen_label_8, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_8, "99°");
	lv_label_set_long_mode(ui->screen_label_8, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_8_main_main_default
	static lv_style_t style_screen_label_8_main_main_default;
	if (style_screen_label_8_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_8_main_main_default);
	else
		lv_style_init(&style_screen_label_8_main_main_default);
	lv_style_set_radius(&style_screen_label_8_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_8_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_8_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_8_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_8_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_8_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_8_main_main_default, &lv_font_SF_Pro_Display_Medium_74);
	lv_style_set_text_letter_space(&style_screen_label_8_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_label_8_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_8_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_8_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_8_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_8_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_8_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_8, &style_screen_label_8_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_9
	ui->screen_label_9 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_9, 36, 371);
	lv_obj_set_size(ui->screen_label_9, 57, 14);
	lv_obj_set_scrollbar_mode(ui->screen_label_9, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_9, "H:24°");
	lv_label_set_long_mode(ui->screen_label_9, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_9_main_main_default
	static lv_style_t style_screen_label_9_main_main_default;
	if (style_screen_label_9_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_9_main_main_default);
	else
		lv_style_init(&style_screen_label_9_main_main_default);
	lv_style_set_radius(&style_screen_label_9_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_9_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_9_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_9_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_9_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_9_main_main_default, lv_color_make(0xA6, 0xA9, 0xC0));
	lv_style_set_text_font(&style_screen_label_9_main_main_default, &lv_font_SF_Pro_Display_Medium_14);
	lv_style_set_text_letter_space(&style_screen_label_9_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_label_9_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_9_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_9_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_9_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_9_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_9_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_9, &style_screen_label_9_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_10
	ui->screen_label_10 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_10, 78, 371);
	lv_obj_set_size(ui->screen_label_10, 57, 14);
	lv_obj_set_scrollbar_mode(ui->screen_label_10, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_10, "L:18°");
	lv_label_set_long_mode(ui->screen_label_10, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_10_main_main_default
	static lv_style_t style_screen_label_10_main_main_default;
	if (style_screen_label_10_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_10_main_main_default);
	else
		lv_style_init(&style_screen_label_10_main_main_default);
	lv_style_set_radius(&style_screen_label_10_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_10_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_10_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_10_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_10_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_10_main_main_default, lv_color_make(0xA6, 0xA9, 0xC0));
	lv_style_set_text_font(&style_screen_label_10_main_main_default, &lv_font_SF_Pro_Display_Medium_14);
	lv_style_set_text_letter_space(&style_screen_label_10_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_label_10_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_10_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_10_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_10_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_10_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_10_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_10, &style_screen_label_10_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_11
	ui->screen_label_11 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_11, 41, 391);
	lv_obj_set_size(ui->screen_label_11, 168, 26);
	lv_obj_set_scrollbar_mode(ui->screen_label_11, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_11, "Nanshan, Shenzhen");
	lv_label_set_long_mode(ui->screen_label_11, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_11_main_main_default
	static lv_style_t style_screen_label_11_main_main_default;
	if (style_screen_label_11_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_11_main_main_default);
	else
		lv_style_init(&style_screen_label_11_main_main_default);
	lv_style_set_radius(&style_screen_label_11_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_11_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_11_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_11_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_11_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_11_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_11_main_main_default, &lv_font_SF_Pro_Display_Medium_18);
	lv_style_set_text_letter_space(&style_screen_label_11_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_label_11_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_11_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_11_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_11_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_11_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_11_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_11, &style_screen_label_11_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_12
	ui->screen_label_12 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_12, 221, 393);
	lv_obj_set_size(ui->screen_label_12, 58, 18);
	lv_obj_set_scrollbar_mode(ui->screen_label_12, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_12, "中雨");
	lv_label_set_long_mode(ui->screen_label_12, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_12_main_main_default
	static lv_style_t style_screen_label_12_main_main_default;
	if (style_screen_label_12_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_12_main_main_default);
	else
		lv_style_init(&style_screen_label_12_main_main_default);
	lv_style_set_radius(&style_screen_label_12_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_12_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_12_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_12_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_12_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_12_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_12_main_main_default, &lv_font_MiSans_Demibold_13);
	lv_style_set_text_letter_space(&style_screen_label_12_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_label_12_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_12_main_main_default, LV_TEXT_ALIGN_LEFT);
	lv_style_set_pad_left(&style_screen_label_12_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_12_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_12_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_12_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_12, &style_screen_label_12_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_13
	ui->screen_label_13 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_13, 58, 445);
	lv_obj_set_size(ui->screen_label_13, 42, 16);
	lv_obj_set_scrollbar_mode(ui->screen_label_13, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_13, "Mon");
	lv_label_set_long_mode(ui->screen_label_13, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_13_main_main_default
	static lv_style_t style_screen_label_13_main_main_default;
	if (style_screen_label_13_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_13_main_main_default);
	else
		lv_style_init(&style_screen_label_13_main_main_default);
	lv_style_set_radius(&style_screen_label_13_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_13_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_13_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_13_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_13_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_13_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_13_main_main_default, &lv_font_SF_Pro_Display_Medium_15);
	lv_style_set_text_letter_space(&style_screen_label_13_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_label_13_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_13_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_13_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_13_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_13_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_13_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_13, &style_screen_label_13_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_14
	ui->screen_label_14 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_14, 134, 445);
	lv_obj_set_size(ui->screen_label_14, 42, 16);
	lv_obj_set_scrollbar_mode(ui->screen_label_14, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_14, "Tue");
	lv_label_set_long_mode(ui->screen_label_14, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_14_main_main_default
	static lv_style_t style_screen_label_14_main_main_default;
	if (style_screen_label_14_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_14_main_main_default);
	else
		lv_style_init(&style_screen_label_14_main_main_default);
	lv_style_set_radius(&style_screen_label_14_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_14_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_14_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_14_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_14_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_14_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_14_main_main_default, &lv_font_SF_Pro_Display_Medium_15);
	lv_style_set_text_letter_space(&style_screen_label_14_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_label_14_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_14_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_14_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_14_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_14_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_14_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_14, &style_screen_label_14_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_15
	ui->screen_label_15 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_15, 214, 445);
	lv_obj_set_size(ui->screen_label_15, 42, 16);
	lv_obj_set_scrollbar_mode(ui->screen_label_15, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_15, "Wed");
	lv_label_set_long_mode(ui->screen_label_15, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_15_main_main_default
	static lv_style_t style_screen_label_15_main_main_default;
	if (style_screen_label_15_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_15_main_main_default);
	else
		lv_style_init(&style_screen_label_15_main_main_default);
	lv_style_set_radius(&style_screen_label_15_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_15_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_15_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_15_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_15_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_15_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_15_main_main_default, &lv_font_SF_Pro_Display_Medium_15);
	lv_style_set_text_letter_space(&style_screen_label_15_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_label_15_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_15_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_15_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_15_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_15_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_15_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_15, &style_screen_label_15_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_16
	ui->screen_label_16 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_16, 289, 446);
	lv_obj_set_size(ui->screen_label_16, 42, 16);
	lv_obj_set_scrollbar_mode(ui->screen_label_16, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_16, "Thu");
	lv_label_set_long_mode(ui->screen_label_16, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_16_main_main_default
	static lv_style_t style_screen_label_16_main_main_default;
	if (style_screen_label_16_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_16_main_main_default);
	else
		lv_style_init(&style_screen_label_16_main_main_default);
	lv_style_set_radius(&style_screen_label_16_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_16_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_16_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_16_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_16_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_16_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_16_main_main_default, &lv_font_SF_Pro_Display_Medium_15);
	lv_style_set_text_letter_space(&style_screen_label_16_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_label_16_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_16_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_16_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_16_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_16_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_16_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_16, &style_screen_label_16_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_17
	ui->screen_label_17 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_17, 365, 446);
	lv_obj_set_size(ui->screen_label_17, 42, 16);
	lv_obj_set_scrollbar_mode(ui->screen_label_17, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_17, "Fri");
	lv_label_set_long_mode(ui->screen_label_17, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_17_main_main_default
	static lv_style_t style_screen_label_17_main_main_default;
	if (style_screen_label_17_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_17_main_main_default);
	else
		lv_style_init(&style_screen_label_17_main_main_default);
	lv_style_set_radius(&style_screen_label_17_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_17_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_17_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_17_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_17_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_17_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_17_main_main_default, &lv_font_SF_Pro_Display_Medium_15);
	lv_style_set_text_letter_space(&style_screen_label_17_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_label_17_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_17_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_17_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_17_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_17_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_17_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_17, &style_screen_label_17_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_18
	ui->screen_label_18 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_18, 60, 506);
	lv_obj_set_size(ui->screen_label_18, 42, 16);
	lv_obj_set_scrollbar_mode(ui->screen_label_18, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_18, "30%");
	lv_label_set_long_mode(ui->screen_label_18, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_18_main_main_default
	static lv_style_t style_screen_label_18_main_main_default;
	if (style_screen_label_18_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_18_main_main_default);
	else
		lv_style_init(&style_screen_label_18_main_main_default);
	lv_style_set_radius(&style_screen_label_18_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_18_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_18_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_18_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_18_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_18_main_main_default, lv_color_make(0x42, 0xA0, 0xF0));
	lv_style_set_text_font(&style_screen_label_18_main_main_default, &lv_font_SF_Pro_Display_Medium_15);
	lv_style_set_text_letter_space(&style_screen_label_18_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_label_18_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_18_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_18_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_18_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_18_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_18_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_18, &style_screen_label_18_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_19
	ui->screen_label_19 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_19, 137, 506);
	lv_obj_set_size(ui->screen_label_19, 42, 16);
	lv_obj_set_scrollbar_mode(ui->screen_label_19, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_19, "30%");
	lv_label_set_long_mode(ui->screen_label_19, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_19_main_main_default
	static lv_style_t style_screen_label_19_main_main_default;
	if (style_screen_label_19_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_19_main_main_default);
	else
		lv_style_init(&style_screen_label_19_main_main_default);
	lv_style_set_radius(&style_screen_label_19_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_19_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_19_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_19_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_19_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_19_main_main_default, lv_color_make(0x42, 0xA0, 0xF0));
	lv_style_set_text_font(&style_screen_label_19_main_main_default, &lv_font_SF_Pro_Display_Medium_15);
	lv_style_set_text_letter_space(&style_screen_label_19_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_label_19_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_19_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_19_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_19_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_19_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_19_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_19, &style_screen_label_19_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_20
	ui->screen_label_20 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_20, 213, 506);
	lv_obj_set_size(ui->screen_label_20, 42, 16);
	lv_obj_set_scrollbar_mode(ui->screen_label_20, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_20, "10%");
	lv_label_set_long_mode(ui->screen_label_20, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_20_main_main_default
	static lv_style_t style_screen_label_20_main_main_default;
	if (style_screen_label_20_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_20_main_main_default);
	else
		lv_style_init(&style_screen_label_20_main_main_default);
	lv_style_set_radius(&style_screen_label_20_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_20_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_20_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_20_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_20_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_20_main_main_default, lv_color_make(0x42, 0xA0, 0xF0));
	lv_style_set_text_font(&style_screen_label_20_main_main_default, &lv_font_SF_Pro_Display_Medium_15);
	lv_style_set_text_letter_space(&style_screen_label_20_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_label_20_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_20_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_20_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_20_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_20_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_20_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_20, &style_screen_label_20_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_21
	ui->screen_label_21 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_21, 289, 506);
	lv_obj_set_size(ui->screen_label_21, 42, 16);
	lv_obj_set_scrollbar_mode(ui->screen_label_21, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_21, "5%");
	lv_label_set_long_mode(ui->screen_label_21, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_21_main_main_default
	static lv_style_t style_screen_label_21_main_main_default;
	if (style_screen_label_21_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_21_main_main_default);
	else
		lv_style_init(&style_screen_label_21_main_main_default);
	lv_style_set_radius(&style_screen_label_21_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_21_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_21_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_21_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_21_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_21_main_main_default, lv_color_make(0x42, 0xA0, 0xF0));
	lv_style_set_text_font(&style_screen_label_21_main_main_default, &lv_font_SF_Pro_Display_Medium_15);
	lv_style_set_text_letter_space(&style_screen_label_21_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_label_21_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_21_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_21_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_21_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_21_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_21_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_21, &style_screen_label_21_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_22
	ui->screen_label_22 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_22, 367, 506);
	lv_obj_set_size(ui->screen_label_22, 42, 16);
	lv_obj_set_scrollbar_mode(ui->screen_label_22, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_22, "5%");
	lv_label_set_long_mode(ui->screen_label_22, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_22_main_main_default
	static lv_style_t style_screen_label_22_main_main_default;
	if (style_screen_label_22_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_22_main_main_default);
	else
		lv_style_init(&style_screen_label_22_main_main_default);
	lv_style_set_radius(&style_screen_label_22_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_22_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_22_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_22_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_22_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_22_main_main_default, lv_color_make(0x42, 0xA0, 0xF0));
	lv_style_set_text_font(&style_screen_label_22_main_main_default, &lv_font_SF_Pro_Display_Medium_15);
	lv_style_set_text_letter_space(&style_screen_label_22_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_label_22_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_22_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_22_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_22_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_22_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_22_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_22, &style_screen_label_22_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_23
	ui->screen_label_23 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_23, 60, 540);
	lv_obj_set_size(ui->screen_label_23, 42, 22);
	lv_obj_set_scrollbar_mode(ui->screen_label_23, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_23, "23°");
	lv_label_set_long_mode(ui->screen_label_23, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_23_main_main_default
	static lv_style_t style_screen_label_23_main_main_default;
	if (style_screen_label_23_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_23_main_main_default);
	else
		lv_style_init(&style_screen_label_23_main_main_default);
	lv_style_set_radius(&style_screen_label_23_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_23_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_23_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_23_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_23_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_23_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_23_main_main_default, &lv_font_SF_Pro_Display_Medium_20);
	lv_style_set_text_letter_space(&style_screen_label_23_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_label_23_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_23_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_23_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_23_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_23_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_23_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_23, &style_screen_label_23_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_24
	ui->screen_label_24 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_24, 138, 540);
	lv_obj_set_size(ui->screen_label_24, 42, 22);
	lv_obj_set_scrollbar_mode(ui->screen_label_24, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_24, "22°");
	lv_label_set_long_mode(ui->screen_label_24, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_24_main_main_default
	static lv_style_t style_screen_label_24_main_main_default;
	if (style_screen_label_24_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_24_main_main_default);
	else
		lv_style_init(&style_screen_label_24_main_main_default);
	lv_style_set_radius(&style_screen_label_24_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_24_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_24_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_24_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_24_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_24_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_24_main_main_default, &lv_font_SF_Pro_Display_Medium_20);
	lv_style_set_text_letter_space(&style_screen_label_24_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_label_24_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_24_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_24_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_24_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_24_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_24_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_24, &style_screen_label_24_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_25
	ui->screen_label_25 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_25, 215, 540);
	lv_obj_set_size(ui->screen_label_25, 42, 22);
	lv_obj_set_scrollbar_mode(ui->screen_label_25, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_25, "25°");
	lv_label_set_long_mode(ui->screen_label_25, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_25_main_main_default
	static lv_style_t style_screen_label_25_main_main_default;
	if (style_screen_label_25_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_25_main_main_default);
	else
		lv_style_init(&style_screen_label_25_main_main_default);
	lv_style_set_radius(&style_screen_label_25_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_25_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_25_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_25_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_25_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_25_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_25_main_main_default, &lv_font_SF_Pro_Display_Medium_20);
	lv_style_set_text_letter_space(&style_screen_label_25_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_label_25_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_25_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_25_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_25_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_25_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_25_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_25, &style_screen_label_25_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_26
	ui->screen_label_26 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_26, 292, 540);
	lv_obj_set_size(ui->screen_label_26, 42, 22);
	lv_obj_set_scrollbar_mode(ui->screen_label_26, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_26, "29°");
	lv_label_set_long_mode(ui->screen_label_26, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_26_main_main_default
	static lv_style_t style_screen_label_26_main_main_default;
	if (style_screen_label_26_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_26_main_main_default);
	else
		lv_style_init(&style_screen_label_26_main_main_default);
	lv_style_set_radius(&style_screen_label_26_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_26_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_26_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_26_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_26_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_26_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_26_main_main_default, &lv_font_SF_Pro_Display_Medium_20);
	lv_style_set_text_letter_space(&style_screen_label_26_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_label_26_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_26_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_26_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_26_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_26_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_26_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_26, &style_screen_label_26_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_27
	ui->screen_label_27 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_27, 369, 540);
	lv_obj_set_size(ui->screen_label_27, 42, 22);
	lv_obj_set_scrollbar_mode(ui->screen_label_27, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_27, "29°");
	lv_label_set_long_mode(ui->screen_label_27, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_27_main_main_default
	static lv_style_t style_screen_label_27_main_main_default;
	if (style_screen_label_27_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_27_main_main_default);
	else
		lv_style_init(&style_screen_label_27_main_main_default);
	lv_style_set_radius(&style_screen_label_27_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_27_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_27_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_27_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_27_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_27_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_27_main_main_default, &lv_font_SF_Pro_Display_Medium_20);
	lv_style_set_text_letter_space(&style_screen_label_27_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_label_27_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_27_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_27_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_27_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_27_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_27_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_27, &style_screen_label_27_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_28
	ui->screen_label_28 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_28, 526, 281);
	lv_obj_set_size(ui->screen_label_28, 80, 22);
	lv_obj_set_scrollbar_mode(ui->screen_label_28, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_28, "模式选择");
	lv_label_set_long_mode(ui->screen_label_28, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_28_main_main_default
	static lv_style_t style_screen_label_28_main_main_default;
	if (style_screen_label_28_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_28_main_main_default);
	else
		lv_style_init(&style_screen_label_28_main_main_default);
	lv_style_set_radius(&style_screen_label_28_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_28_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_28_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_28_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_28_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_28_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_28_main_main_default, &lv_font_MiSans_Demibold_20);
	lv_style_set_text_letter_space(&style_screen_label_28_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_label_28_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_28_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_28_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_28_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_28_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_28_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_28, &style_screen_label_28_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_29
	ui->screen_label_29 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_29, 501, 310);
	lv_obj_set_size(ui->screen_label_29, 130, 28);
	lv_obj_set_scrollbar_mode(ui->screen_label_29, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_29, "Model choose");
	lv_label_set_long_mode(ui->screen_label_29, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_29_main_main_default
	static lv_style_t style_screen_label_29_main_main_default;
	if (style_screen_label_29_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_29_main_main_default);
	else
		lv_style_init(&style_screen_label_29_main_main_default);
	lv_style_set_radius(&style_screen_label_29_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_29_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_29_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_29_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_29_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_29_main_main_default, lv_color_make(0xA6, 0xA9, 0xC0));
	lv_style_set_text_font(&style_screen_label_29_main_main_default, &lv_font_SF_Pro_Display_Medium_14);
	lv_style_set_text_letter_space(&style_screen_label_29_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_label_29_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_29_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_29_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_29_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_29_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_29_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_29, &style_screen_label_29_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_30
	ui->screen_label_30 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_30, 753, 230);
	lv_obj_set_size(ui->screen_label_30, 200, 22);
	lv_obj_set_scrollbar_mode(ui->screen_label_30, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_30, "卧室窗帘");
	lv_label_set_long_mode(ui->screen_label_30, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_30_main_main_default
	static lv_style_t style_screen_label_30_main_main_default;
	if (style_screen_label_30_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_30_main_main_default);
	else
		lv_style_init(&style_screen_label_30_main_main_default);
	lv_style_set_radius(&style_screen_label_30_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_30_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_30_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_30_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_30_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_30_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_30_main_main_default, &lv_font_MiSans_Demibold_20);
	lv_style_set_text_letter_space(&style_screen_label_30_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_label_30_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_30_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_30_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_30_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_30_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_30_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_30, &style_screen_label_30_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_31
	ui->screen_label_31 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_31, 787, 255);
	lv_obj_set_size(ui->screen_label_31, 130, 28);
	lv_obj_set_scrollbar_mode(ui->screen_label_31, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_31, "Living Room");
	lv_label_set_long_mode(ui->screen_label_31, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_31_main_main_default
	static lv_style_t style_screen_label_31_main_main_default;
	if (style_screen_label_31_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_31_main_main_default);
	else
		lv_style_init(&style_screen_label_31_main_main_default);
	lv_style_set_radius(&style_screen_label_31_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_31_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_31_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_31_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_31_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_31_main_main_default, lv_color_make(0xA6, 0xA9, 0xC0));
	lv_style_set_text_font(&style_screen_label_31_main_main_default, &lv_font_SF_Pro_Display_Medium_14);
	lv_style_set_text_letter_space(&style_screen_label_31_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_label_31_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_31_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_31_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_31_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_31_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_31_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_31, &style_screen_label_31_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_32
	ui->screen_label_32 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_32, 824, 298);
	lv_obj_set_size(ui->screen_label_32, 60, 21);
	lv_obj_set_scrollbar_mode(ui->screen_label_32, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_32, "45%");
	lv_label_set_long_mode(ui->screen_label_32, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_32_main_main_default
	static lv_style_t style_screen_label_32_main_main_default;
	if (style_screen_label_32_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_32_main_main_default);
	else
		lv_style_init(&style_screen_label_32_main_main_default);
	lv_style_set_radius(&style_screen_label_32_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_32_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_32_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_32_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_32_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_32_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_32_main_main_default, &lv_font_SF_Pro_Display_Medium_20);
	lv_style_set_text_letter_space(&style_screen_label_32_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_label_32_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_32_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_32_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_32_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_32_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_32_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_32, &style_screen_label_32_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_33
	ui->screen_label_33 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_33, 562, 392);
	lv_obj_set_size(ui->screen_label_33, 80, 22);
	lv_obj_set_scrollbar_mode(ui->screen_label_33, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_33, "智能灯具");
	lv_label_set_long_mode(ui->screen_label_33, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_33_main_main_default
	static lv_style_t style_screen_label_33_main_main_default;
	if (style_screen_label_33_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_33_main_main_default);
	else
		lv_style_init(&style_screen_label_33_main_main_default);
	lv_style_set_radius(&style_screen_label_33_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_33_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_33_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_33_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_33_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_33_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_33_main_main_default, &lv_font_MiSans_Demibold_17);
	lv_style_set_text_letter_space(&style_screen_label_33_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_label_33_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_33_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_33_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_33_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_33_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_33_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_33, &style_screen_label_33_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_34
	ui->screen_label_34 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_34, 491, 379);
	lv_obj_set_size(ui->screen_label_34, 128, 35);
	lv_obj_set_scrollbar_mode(ui->screen_label_34, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_34, "智能灯具");
	lv_label_set_long_mode(ui->screen_label_34, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_34_main_main_default
	static lv_style_t style_screen_label_34_main_main_default;
	if (style_screen_label_34_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_34_main_main_default);
	else
		lv_style_init(&style_screen_label_34_main_main_default);
	lv_style_set_radius(&style_screen_label_34_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_34_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_34_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_34_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_34_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_34_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_34_main_main_default, &lv_font_MiSans_Demibold_32);
	lv_style_set_text_letter_space(&style_screen_label_34_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_label_34_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_34_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_34_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_34_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_34_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_34_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_34, &style_screen_label_34_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_35
	ui->screen_label_35 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_35, 493, 444);
	lv_obj_set_size(ui->screen_label_35, 82, 43);
	lv_obj_set_scrollbar_mode(ui->screen_label_35, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_35, "Philips \nLighting");
	lv_label_set_long_mode(ui->screen_label_35, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_35_main_main_default
	static lv_style_t style_screen_label_35_main_main_default;
	if (style_screen_label_35_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_35_main_main_default);
	else
		lv_style_init(&style_screen_label_35_main_main_default);
	lv_style_set_radius(&style_screen_label_35_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_35_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_35_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_35_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_35_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_35_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_35_main_main_default, &lv_font_SF_Pro_Display_Medium_20);
	lv_style_set_text_letter_space(&style_screen_label_35_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_label_35_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_35_main_main_default, LV_TEXT_ALIGN_LEFT);
	lv_style_set_pad_left(&style_screen_label_35_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_35_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_35_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_35_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_35, &style_screen_label_35_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_36
	ui->screen_label_36 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_36, 493, 503);
	lv_obj_set_size(ui->screen_label_36, 82, 22);
	lv_obj_set_scrollbar_mode(ui->screen_label_36, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_36, "4  lamps");
	lv_label_set_long_mode(ui->screen_label_36, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_36_main_main_default
	static lv_style_t style_screen_label_36_main_main_default;
	if (style_screen_label_36_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_36_main_main_default);
	else
		lv_style_init(&style_screen_label_36_main_main_default);
	lv_style_set_radius(&style_screen_label_36_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_36_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_36_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_36_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_36_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_36_main_main_default, lv_color_make(0xA6, 0xA9, 0xC0));
	lv_style_set_text_font(&style_screen_label_36_main_main_default, &lv_font_SF_Pro_Display_Medium_14);
	lv_style_set_text_letter_space(&style_screen_label_36_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_label_36_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_36_main_main_default, LV_TEXT_ALIGN_LEFT);
	lv_style_set_pad_left(&style_screen_label_36_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_36_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_36_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_36_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_36, &style_screen_label_36_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_37
	ui->screen_label_37 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_37, 493, 540);
	lv_obj_set_size(ui->screen_label_37, 30, 21);
	lv_obj_set_scrollbar_mode(ui->screen_label_37, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_37, "On");
	lv_label_set_long_mode(ui->screen_label_37, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_37_main_main_default
	static lv_style_t style_screen_label_37_main_main_default;
	if (style_screen_label_37_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_37_main_main_default);
	else
		lv_style_init(&style_screen_label_37_main_main_default);
	lv_style_set_radius(&style_screen_label_37_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_37_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_color(&style_screen_label_37_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_37_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_37_main_main_default, 0);
	lv_style_set_text_color(&style_screen_label_37_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_text_font(&style_screen_label_37_main_main_default, &lv_font_SF_Pro_Display_Medium_20);
	lv_style_set_text_letter_space(&style_screen_label_37_main_main_default, 0);
	lv_style_set_text_line_space(&style_screen_label_37_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_37_main_main_default, LV_TEXT_ALIGN_LEFT);
	lv_style_set_pad_left(&style_screen_label_37_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_37_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_37_main_main_default, 0);
	lv_style_set_pad_bottom(&style_screen_label_37_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_37, &style_screen_label_37_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_label_38
	ui->screen_label_38 = lv_label_create(ui->screen);
	lv_obj_set_pos(ui->screen_label_38, 562, 526);
	lv_obj_set_size(ui->screen_label_38, 87, 49);
	lv_obj_set_scrollbar_mode(ui->screen_label_38, LV_SCROLLBAR_MODE_OFF);
	lv_label_set_text(ui->screen_label_38, "");
	lv_label_set_long_mode(ui->screen_label_38, LV_LABEL_LONG_WRAP);

	//Write style state: LV_STATE_DEFAULT for style_screen_label_38_main_main_default
	static lv_style_t style_screen_label_38_main_main_default;
	if (style_screen_label_38_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_label_38_main_main_default);
	else
		lv_style_init(&style_screen_label_38_main_main_default);
	lv_style_set_radius(&style_screen_label_38_main_main_default, 0);
	lv_style_set_bg_color(&style_screen_label_38_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_label_38_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_label_38_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_label_38_main_main_default, 255);
	lv_style_set_text_color(&style_screen_label_38_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_text_letter_space(&style_screen_label_38_main_main_default, 2);
	lv_style_set_text_line_space(&style_screen_label_38_main_main_default, 0);
	lv_style_set_text_align(&style_screen_label_38_main_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_left(&style_screen_label_38_main_main_default, 0);
	lv_style_set_pad_right(&style_screen_label_38_main_main_default, 0);
	lv_style_set_pad_top(&style_screen_label_38_main_main_default, 8);
	lv_style_set_pad_bottom(&style_screen_label_38_main_main_default, 0);
	lv_obj_add_style(ui->screen_label_38, &style_screen_label_38_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_sw_1
	ui->screen_sw_1 = lv_switch_create(ui->screen);
	lv_obj_set_pos(ui->screen_sw_1, 585, 536);
	lv_obj_set_size(ui->screen_sw_1, 61, 32);
	lv_obj_set_scrollbar_mode(ui->screen_sw_1, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_sw_1_main_main_default
	static lv_style_t style_screen_sw_1_main_main_default;
	if (style_screen_sw_1_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_sw_1_main_main_default);
	else
		lv_style_init(&style_screen_sw_1_main_main_default);
	lv_style_set_radius(&style_screen_sw_1_main_main_default, 100);
	lv_style_set_bg_color(&style_screen_sw_1_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_sw_1_main_main_default, lv_color_make(0xe6, 0xe2, 0xe6));
	lv_style_set_bg_grad_dir(&style_screen_sw_1_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_sw_1_main_main_default, 255);
	lv_style_set_border_color(&style_screen_sw_1_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_border_width(&style_screen_sw_1_main_main_default, 2);
	lv_style_set_border_opa(&style_screen_sw_1_main_main_default, 255);
	lv_obj_add_style(ui->screen_sw_1, &style_screen_sw_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_FOCUSED for style_screen_sw_1_main_main_focused
	static lv_style_t style_screen_sw_1_main_main_focused;
	if (style_screen_sw_1_main_main_focused.prop_cnt > 1)
		lv_style_reset(&style_screen_sw_1_main_main_focused);
	else
		lv_style_init(&style_screen_sw_1_main_main_focused);
	lv_style_set_radius(&style_screen_sw_1_main_main_focused, 100);
	lv_style_set_bg_color(&style_screen_sw_1_main_main_focused, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_sw_1_main_main_focused, lv_color_make(0xe6, 0xe2, 0xe6));
	lv_style_set_bg_grad_dir(&style_screen_sw_1_main_main_focused, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_sw_1_main_main_focused, 255);
	lv_style_set_border_color(&style_screen_sw_1_main_main_focused, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_border_width(&style_screen_sw_1_main_main_focused, 2);
	lv_style_set_border_opa(&style_screen_sw_1_main_main_focused, 255);
	lv_obj_add_style(ui->screen_sw_1, &style_screen_sw_1_main_main_focused, LV_PART_MAIN|LV_STATE_FOCUSED);

	//Write style state: LV_STATE_DISABLED for style_screen_sw_1_main_main_disabled
	static lv_style_t style_screen_sw_1_main_main_disabled;
	if (style_screen_sw_1_main_main_disabled.prop_cnt > 1)
		lv_style_reset(&style_screen_sw_1_main_main_disabled);
	else
		lv_style_init(&style_screen_sw_1_main_main_disabled);
	lv_style_set_radius(&style_screen_sw_1_main_main_disabled, 100);
	lv_style_set_bg_color(&style_screen_sw_1_main_main_disabled, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_sw_1_main_main_disabled, lv_color_make(0xe6, 0xe2, 0xe6));
	lv_style_set_bg_grad_dir(&style_screen_sw_1_main_main_disabled, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_sw_1_main_main_disabled, 255);
	lv_style_set_border_color(&style_screen_sw_1_main_main_disabled, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_border_width(&style_screen_sw_1_main_main_disabled, 2);
	lv_style_set_border_opa(&style_screen_sw_1_main_main_disabled, 255);
	lv_obj_add_style(ui->screen_sw_1, &style_screen_sw_1_main_main_disabled, LV_PART_MAIN|LV_STATE_DISABLED);

	//Write style state: LV_STATE_CHECKED for style_screen_sw_1_main_indicator_checked
	static lv_style_t style_screen_sw_1_main_indicator_checked;
	if (style_screen_sw_1_main_indicator_checked.prop_cnt > 1)
		lv_style_reset(&style_screen_sw_1_main_indicator_checked);
	else
		lv_style_init(&style_screen_sw_1_main_indicator_checked);
	lv_style_set_bg_color(&style_screen_sw_1_main_indicator_checked, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_sw_1_main_indicator_checked, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_sw_1_main_indicator_checked, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_sw_1_main_indicator_checked, 255);
	lv_style_set_border_color(&style_screen_sw_1_main_indicator_checked, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_border_width(&style_screen_sw_1_main_indicator_checked, 0);
	lv_style_set_border_opa(&style_screen_sw_1_main_indicator_checked, 255);
	lv_obj_add_style(ui->screen_sw_1, &style_screen_sw_1_main_indicator_checked, LV_PART_INDICATOR|LV_STATE_CHECKED);

	//Write style state: LV_STATE_FOCUSED for style_screen_sw_1_main_indicator_focused
	static lv_style_t style_screen_sw_1_main_indicator_focused;
	if (style_screen_sw_1_main_indicator_focused.prop_cnt > 1)
		lv_style_reset(&style_screen_sw_1_main_indicator_focused);
	else
		lv_style_init(&style_screen_sw_1_main_indicator_focused);
	lv_style_set_bg_color(&style_screen_sw_1_main_indicator_focused, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_sw_1_main_indicator_focused, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_sw_1_main_indicator_focused, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_sw_1_main_indicator_focused, 255);
	lv_style_set_border_color(&style_screen_sw_1_main_indicator_focused, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_border_width(&style_screen_sw_1_main_indicator_focused, 2);
	lv_style_set_border_opa(&style_screen_sw_1_main_indicator_focused, 255);
	lv_obj_add_style(ui->screen_sw_1, &style_screen_sw_1_main_indicator_focused, LV_PART_INDICATOR|LV_STATE_FOCUSED);

	//Write style state: LV_STATE_DISABLED for style_screen_sw_1_main_indicator_disabled
	static lv_style_t style_screen_sw_1_main_indicator_disabled;
	if (style_screen_sw_1_main_indicator_disabled.prop_cnt > 1)
		lv_style_reset(&style_screen_sw_1_main_indicator_disabled);
	else
		lv_style_init(&style_screen_sw_1_main_indicator_disabled);
	lv_style_set_bg_color(&style_screen_sw_1_main_indicator_disabled, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_sw_1_main_indicator_disabled, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_sw_1_main_indicator_disabled, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_sw_1_main_indicator_disabled, 255);
	lv_style_set_border_color(&style_screen_sw_1_main_indicator_disabled, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_border_width(&style_screen_sw_1_main_indicator_disabled, 2);
	lv_style_set_border_opa(&style_screen_sw_1_main_indicator_disabled, 255);
	lv_obj_add_style(ui->screen_sw_1, &style_screen_sw_1_main_indicator_disabled, LV_PART_INDICATOR|LV_STATE_DISABLED);
	lv_obj_add_style(ui->screen_sw_1, &style_screen_sw_1_main_main_disabled, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_add_style(ui->screen_sw_1, &style_screen_sw_1_main_main_disabled, LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_add_style(ui->screen_sw_1, &style_screen_sw_1_main_main_disabled, LV_PART_INDICATOR|LV_STATE_FOCUSED);
	lv_obj_add_style(ui->screen_sw_1, &style_screen_sw_1_main_main_disabled, LV_PART_INDICATOR|LV_STATE_FOCUS_KEY);
	lv_obj_add_style(ui->screen_sw_1, &style_screen_sw_1_main_main_disabled, LV_PART_INDICATOR|LV_STATE_EDITED);
	lv_obj_add_style(ui->screen_sw_1, &style_screen_sw_1_main_main_disabled, LV_PART_INDICATOR|LV_STATE_HOVERED);
	lv_obj_add_style(ui->screen_sw_1, &style_screen_sw_1_main_main_disabled, LV_PART_INDICATOR|LV_STATE_PRESSED);
	lv_obj_add_style(ui->screen_sw_1, &style_screen_sw_1_main_main_disabled, LV_PART_INDICATOR|LV_STATE_SCROLLED);
	lv_obj_add_style(ui->screen_sw_1, &style_screen_sw_1_main_main_disabled, LV_PART_INDICATOR|LV_STATE_DISABLED);

	//Write style state: LV_STATE_DEFAULT for style_screen_sw_1_main_knob_default
	static lv_style_t style_screen_sw_1_main_knob_default;
	if (style_screen_sw_1_main_knob_default.prop_cnt > 1)
		lv_style_reset(&style_screen_sw_1_main_knob_default);
	else
		lv_style_init(&style_screen_sw_1_main_knob_default);
	lv_style_set_radius(&style_screen_sw_1_main_knob_default, 100);
	lv_style_set_bg_color(&style_screen_sw_1_main_knob_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_sw_1_main_knob_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_sw_1_main_knob_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_sw_1_main_knob_default, 255);
	lv_style_set_border_color(&style_screen_sw_1_main_knob_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_border_width(&style_screen_sw_1_main_knob_default, 0);
	lv_style_set_border_opa(&style_screen_sw_1_main_knob_default, 0);
	lv_obj_add_style(ui->screen_sw_1, &style_screen_sw_1_main_knob_default, LV_PART_KNOB|LV_STATE_DEFAULT);

	//Write codes screen_sw_2
	ui->screen_sw_2 = lv_switch_create(ui->screen);
	lv_obj_set_pos(ui->screen_sw_2, 718, 445);
	lv_obj_set_size(ui->screen_sw_2, 30, 18);
	lv_obj_set_scrollbar_mode(ui->screen_sw_2, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_sw_2_main_main_default
	static lv_style_t style_screen_sw_2_main_main_default;
	if (style_screen_sw_2_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_sw_2_main_main_default);
	else
		lv_style_init(&style_screen_sw_2_main_main_default);
	lv_style_set_radius(&style_screen_sw_2_main_main_default, 100);
	lv_style_set_bg_color(&style_screen_sw_2_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_sw_2_main_main_default, lv_color_make(0xe6, 0xe2, 0xe6));
	lv_style_set_bg_grad_dir(&style_screen_sw_2_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_sw_2_main_main_default, 255);
	lv_style_set_border_color(&style_screen_sw_2_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_border_width(&style_screen_sw_2_main_main_default, 2);
	lv_style_set_border_opa(&style_screen_sw_2_main_main_default, 255);
	lv_obj_add_style(ui->screen_sw_2, &style_screen_sw_2_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_FOCUSED for style_screen_sw_2_main_main_focused
	static lv_style_t style_screen_sw_2_main_main_focused;
	if (style_screen_sw_2_main_main_focused.prop_cnt > 1)
		lv_style_reset(&style_screen_sw_2_main_main_focused);
	else
		lv_style_init(&style_screen_sw_2_main_main_focused);
	lv_style_set_radius(&style_screen_sw_2_main_main_focused, 100);
	lv_style_set_bg_color(&style_screen_sw_2_main_main_focused, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_sw_2_main_main_focused, lv_color_make(0xe6, 0xe2, 0xe6));
	lv_style_set_bg_grad_dir(&style_screen_sw_2_main_main_focused, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_sw_2_main_main_focused, 255);
	lv_style_set_border_color(&style_screen_sw_2_main_main_focused, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_border_width(&style_screen_sw_2_main_main_focused, 2);
	lv_style_set_border_opa(&style_screen_sw_2_main_main_focused, 255);
	lv_obj_add_style(ui->screen_sw_2, &style_screen_sw_2_main_main_focused, LV_PART_MAIN|LV_STATE_FOCUSED);

	//Write style state: LV_STATE_DISABLED for style_screen_sw_2_main_main_disabled
	static lv_style_t style_screen_sw_2_main_main_disabled;
	if (style_screen_sw_2_main_main_disabled.prop_cnt > 1)
		lv_style_reset(&style_screen_sw_2_main_main_disabled);
	else
		lv_style_init(&style_screen_sw_2_main_main_disabled);
	lv_style_set_radius(&style_screen_sw_2_main_main_disabled, 100);
	lv_style_set_bg_color(&style_screen_sw_2_main_main_disabled, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_sw_2_main_main_disabled, lv_color_make(0xe6, 0xe2, 0xe6));
	lv_style_set_bg_grad_dir(&style_screen_sw_2_main_main_disabled, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_sw_2_main_main_disabled, 255);
	lv_style_set_border_color(&style_screen_sw_2_main_main_disabled, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_border_width(&style_screen_sw_2_main_main_disabled, 2);
	lv_style_set_border_opa(&style_screen_sw_2_main_main_disabled, 255);
	lv_obj_add_style(ui->screen_sw_2, &style_screen_sw_2_main_main_disabled, LV_PART_MAIN|LV_STATE_DISABLED);
	lv_obj_add_style(ui->screen_sw_2, &style_screen_sw_2_main_main_disabled, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_style(ui->screen_sw_2, &style_screen_sw_2_main_main_disabled, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_add_style(ui->screen_sw_2, &style_screen_sw_2_main_main_disabled, LV_PART_MAIN|LV_STATE_FOCUSED);
	lv_obj_add_style(ui->screen_sw_2, &style_screen_sw_2_main_main_disabled, LV_PART_MAIN|LV_STATE_FOCUS_KEY);
	lv_obj_add_style(ui->screen_sw_2, &style_screen_sw_2_main_main_disabled, LV_PART_MAIN|LV_STATE_EDITED);
	lv_obj_add_style(ui->screen_sw_2, &style_screen_sw_2_main_main_disabled, LV_PART_MAIN|LV_STATE_HOVERED);
	lv_obj_add_style(ui->screen_sw_2, &style_screen_sw_2_main_main_disabled, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_add_style(ui->screen_sw_2, &style_screen_sw_2_main_main_disabled, LV_PART_MAIN|LV_STATE_SCROLLED);
	lv_obj_add_style(ui->screen_sw_2, &style_screen_sw_2_main_main_disabled, LV_PART_MAIN|LV_STATE_DISABLED);

	//Write style state: LV_STATE_CHECKED for style_screen_sw_2_main_indicator_checked
	static lv_style_t style_screen_sw_2_main_indicator_checked;
	if (style_screen_sw_2_main_indicator_checked.prop_cnt > 1)
		lv_style_reset(&style_screen_sw_2_main_indicator_checked);
	else
		lv_style_init(&style_screen_sw_2_main_indicator_checked);
	lv_style_set_bg_color(&style_screen_sw_2_main_indicator_checked, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_sw_2_main_indicator_checked, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_sw_2_main_indicator_checked, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_sw_2_main_indicator_checked, 255);
	lv_style_set_border_color(&style_screen_sw_2_main_indicator_checked, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_border_width(&style_screen_sw_2_main_indicator_checked, 0);
	lv_style_set_border_opa(&style_screen_sw_2_main_indicator_checked, 255);
	lv_obj_add_style(ui->screen_sw_2, &style_screen_sw_2_main_indicator_checked, LV_PART_INDICATOR|LV_STATE_CHECKED);

	//Write style state: LV_STATE_FOCUSED for style_screen_sw_2_main_indicator_focused
	static lv_style_t style_screen_sw_2_main_indicator_focused;
	if (style_screen_sw_2_main_indicator_focused.prop_cnt > 1)
		lv_style_reset(&style_screen_sw_2_main_indicator_focused);
	else
		lv_style_init(&style_screen_sw_2_main_indicator_focused);
	lv_style_set_bg_color(&style_screen_sw_2_main_indicator_focused, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_sw_2_main_indicator_focused, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_sw_2_main_indicator_focused, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_sw_2_main_indicator_focused, 255);
	lv_style_set_border_color(&style_screen_sw_2_main_indicator_focused, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_border_width(&style_screen_sw_2_main_indicator_focused, 2);
	lv_style_set_border_opa(&style_screen_sw_2_main_indicator_focused, 255);
	lv_obj_add_style(ui->screen_sw_2, &style_screen_sw_2_main_indicator_focused, LV_PART_INDICATOR|LV_STATE_FOCUSED);

	//Write style state: LV_STATE_DISABLED for style_screen_sw_2_main_indicator_disabled
	static lv_style_t style_screen_sw_2_main_indicator_disabled;
	if (style_screen_sw_2_main_indicator_disabled.prop_cnt > 1)
		lv_style_reset(&style_screen_sw_2_main_indicator_disabled);
	else
		lv_style_init(&style_screen_sw_2_main_indicator_disabled);
	lv_style_set_bg_color(&style_screen_sw_2_main_indicator_disabled, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_sw_2_main_indicator_disabled, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_sw_2_main_indicator_disabled, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_sw_2_main_indicator_disabled, 255);
	lv_style_set_border_color(&style_screen_sw_2_main_indicator_disabled, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_border_width(&style_screen_sw_2_main_indicator_disabled, 2);
	lv_style_set_border_opa(&style_screen_sw_2_main_indicator_disabled, 255);
	lv_obj_add_style(ui->screen_sw_2, &style_screen_sw_2_main_indicator_disabled, LV_PART_INDICATOR|LV_STATE_DISABLED);
	lv_obj_add_style(ui->screen_sw_2, &style_screen_sw_2_main_main_disabled, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_add_style(ui->screen_sw_2, &style_screen_sw_2_main_main_disabled, LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_add_style(ui->screen_sw_2, &style_screen_sw_2_main_main_disabled, LV_PART_INDICATOR|LV_STATE_FOCUSED);
	lv_obj_add_style(ui->screen_sw_2, &style_screen_sw_2_main_main_disabled, LV_PART_INDICATOR|LV_STATE_FOCUS_KEY);
	lv_obj_add_style(ui->screen_sw_2, &style_screen_sw_2_main_main_disabled, LV_PART_INDICATOR|LV_STATE_EDITED);
	lv_obj_add_style(ui->screen_sw_2, &style_screen_sw_2_main_main_disabled, LV_PART_INDICATOR|LV_STATE_HOVERED);
	lv_obj_add_style(ui->screen_sw_2, &style_screen_sw_2_main_main_disabled, LV_PART_INDICATOR|LV_STATE_PRESSED);
	lv_obj_add_style(ui->screen_sw_2, &style_screen_sw_2_main_main_disabled, LV_PART_INDICATOR|LV_STATE_SCROLLED);
	lv_obj_add_style(ui->screen_sw_2, &style_screen_sw_2_main_main_disabled, LV_PART_INDICATOR|LV_STATE_DISABLED);

	//Write style state: LV_STATE_DEFAULT for style_screen_sw_2_main_knob_default
	static lv_style_t style_screen_sw_2_main_knob_default;
	if (style_screen_sw_2_main_knob_default.prop_cnt > 1)
		lv_style_reset(&style_screen_sw_2_main_knob_default);
	else
		lv_style_init(&style_screen_sw_2_main_knob_default);
	lv_style_set_radius(&style_screen_sw_2_main_knob_default, 100);
	lv_style_set_bg_color(&style_screen_sw_2_main_knob_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_sw_2_main_knob_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_sw_2_main_knob_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_sw_2_main_knob_default, 255);
	lv_style_set_border_color(&style_screen_sw_2_main_knob_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_border_width(&style_screen_sw_2_main_knob_default, 0);
	lv_style_set_border_opa(&style_screen_sw_2_main_knob_default, 0);
	lv_obj_add_style(ui->screen_sw_2, &style_screen_sw_2_main_knob_default, LV_PART_KNOB|LV_STATE_DEFAULT);

	//Write codes screen_sw_3
	ui->screen_sw_3 = lv_switch_create(ui->screen);
	lv_obj_set_pos(ui->screen_sw_3, 866, 445);
	lv_obj_set_size(ui->screen_sw_3, 30, 18);
	lv_obj_set_scrollbar_mode(ui->screen_sw_3, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_sw_3_main_main_default
	static lv_style_t style_screen_sw_3_main_main_default;
	if (style_screen_sw_3_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_sw_3_main_main_default);
	else
		lv_style_init(&style_screen_sw_3_main_main_default);
	lv_style_set_radius(&style_screen_sw_3_main_main_default, 100);
	lv_style_set_bg_color(&style_screen_sw_3_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_sw_3_main_main_default, lv_color_make(0xe6, 0xe2, 0xe6));
	lv_style_set_bg_grad_dir(&style_screen_sw_3_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_sw_3_main_main_default, 255);
	lv_style_set_border_color(&style_screen_sw_3_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_border_width(&style_screen_sw_3_main_main_default, 2);
	lv_style_set_border_opa(&style_screen_sw_3_main_main_default, 255);
	lv_obj_add_style(ui->screen_sw_3, &style_screen_sw_3_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_FOCUSED for style_screen_sw_3_main_main_focused
	static lv_style_t style_screen_sw_3_main_main_focused;
	if (style_screen_sw_3_main_main_focused.prop_cnt > 1)
		lv_style_reset(&style_screen_sw_3_main_main_focused);
	else
		lv_style_init(&style_screen_sw_3_main_main_focused);
	lv_style_set_radius(&style_screen_sw_3_main_main_focused, 100);
	lv_style_set_bg_color(&style_screen_sw_3_main_main_focused, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_sw_3_main_main_focused, lv_color_make(0xe6, 0xe2, 0xe6));
	lv_style_set_bg_grad_dir(&style_screen_sw_3_main_main_focused, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_sw_3_main_main_focused, 255);
	lv_style_set_border_color(&style_screen_sw_3_main_main_focused, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_border_width(&style_screen_sw_3_main_main_focused, 2);
	lv_style_set_border_opa(&style_screen_sw_3_main_main_focused, 255);
	lv_obj_add_style(ui->screen_sw_3, &style_screen_sw_3_main_main_focused, LV_PART_MAIN|LV_STATE_FOCUSED);

	//Write style state: LV_STATE_DISABLED for style_screen_sw_3_main_main_disabled
	static lv_style_t style_screen_sw_3_main_main_disabled;
	if (style_screen_sw_3_main_main_disabled.prop_cnt > 1)
		lv_style_reset(&style_screen_sw_3_main_main_disabled);
	else
		lv_style_init(&style_screen_sw_3_main_main_disabled);
	lv_style_set_radius(&style_screen_sw_3_main_main_disabled, 100);
	lv_style_set_bg_color(&style_screen_sw_3_main_main_disabled, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_sw_3_main_main_disabled, lv_color_make(0xe6, 0xe2, 0xe6));
	lv_style_set_bg_grad_dir(&style_screen_sw_3_main_main_disabled, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_sw_3_main_main_disabled, 255);
	lv_style_set_border_color(&style_screen_sw_3_main_main_disabled, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_border_width(&style_screen_sw_3_main_main_disabled, 2);
	lv_style_set_border_opa(&style_screen_sw_3_main_main_disabled, 255);
	lv_obj_add_style(ui->screen_sw_3, &style_screen_sw_3_main_main_disabled, LV_PART_MAIN|LV_STATE_DISABLED);

	//Write style state: LV_STATE_CHECKED for style_screen_sw_3_main_indicator_checked
	static lv_style_t style_screen_sw_3_main_indicator_checked;
	if (style_screen_sw_3_main_indicator_checked.prop_cnt > 1)
		lv_style_reset(&style_screen_sw_3_main_indicator_checked);
	else
		lv_style_init(&style_screen_sw_3_main_indicator_checked);
	lv_style_set_bg_color(&style_screen_sw_3_main_indicator_checked, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_sw_3_main_indicator_checked, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_sw_3_main_indicator_checked, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_sw_3_main_indicator_checked, 255);
	lv_style_set_border_color(&style_screen_sw_3_main_indicator_checked, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_border_width(&style_screen_sw_3_main_indicator_checked, 0);
	lv_style_set_border_opa(&style_screen_sw_3_main_indicator_checked, 255);
	lv_obj_add_style(ui->screen_sw_3, &style_screen_sw_3_main_indicator_checked, LV_PART_INDICATOR|LV_STATE_CHECKED);

	//Write style state: LV_STATE_FOCUSED for style_screen_sw_3_main_indicator_focused
	static lv_style_t style_screen_sw_3_main_indicator_focused;
	if (style_screen_sw_3_main_indicator_focused.prop_cnt > 1)
		lv_style_reset(&style_screen_sw_3_main_indicator_focused);
	else
		lv_style_init(&style_screen_sw_3_main_indicator_focused);
	lv_style_set_bg_color(&style_screen_sw_3_main_indicator_focused, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_sw_3_main_indicator_focused, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_sw_3_main_indicator_focused, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_sw_3_main_indicator_focused, 255);
	lv_style_set_border_color(&style_screen_sw_3_main_indicator_focused, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_border_width(&style_screen_sw_3_main_indicator_focused, 2);
	lv_style_set_border_opa(&style_screen_sw_3_main_indicator_focused, 255);
	lv_obj_add_style(ui->screen_sw_3, &style_screen_sw_3_main_indicator_focused, LV_PART_INDICATOR|LV_STATE_FOCUSED);

	//Write style state: LV_STATE_DISABLED for style_screen_sw_3_main_indicator_disabled
	static lv_style_t style_screen_sw_3_main_indicator_disabled;
	if (style_screen_sw_3_main_indicator_disabled.prop_cnt > 1)
		lv_style_reset(&style_screen_sw_3_main_indicator_disabled);
	else
		lv_style_init(&style_screen_sw_3_main_indicator_disabled);
	lv_style_set_bg_color(&style_screen_sw_3_main_indicator_disabled, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_sw_3_main_indicator_disabled, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_sw_3_main_indicator_disabled, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_sw_3_main_indicator_disabled, 255);
	lv_style_set_border_color(&style_screen_sw_3_main_indicator_disabled, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_border_width(&style_screen_sw_3_main_indicator_disabled, 2);
	lv_style_set_border_opa(&style_screen_sw_3_main_indicator_disabled, 255);
	lv_obj_add_style(ui->screen_sw_3, &style_screen_sw_3_main_indicator_disabled, LV_PART_INDICATOR|LV_STATE_DISABLED);
	lv_obj_add_style(ui->screen_sw_3, &style_screen_sw_3_main_main_disabled, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_add_style(ui->screen_sw_3, &style_screen_sw_3_main_main_disabled, LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_add_style(ui->screen_sw_3, &style_screen_sw_3_main_main_disabled, LV_PART_INDICATOR|LV_STATE_FOCUSED);
	lv_obj_add_style(ui->screen_sw_3, &style_screen_sw_3_main_main_disabled, LV_PART_INDICATOR|LV_STATE_FOCUS_KEY);
	lv_obj_add_style(ui->screen_sw_3, &style_screen_sw_3_main_main_disabled, LV_PART_INDICATOR|LV_STATE_EDITED);
	lv_obj_add_style(ui->screen_sw_3, &style_screen_sw_3_main_main_disabled, LV_PART_INDICATOR|LV_STATE_HOVERED);
	lv_obj_add_style(ui->screen_sw_3, &style_screen_sw_3_main_main_disabled, LV_PART_INDICATOR|LV_STATE_PRESSED);
	lv_obj_add_style(ui->screen_sw_3, &style_screen_sw_3_main_main_disabled, LV_PART_INDICATOR|LV_STATE_SCROLLED);
	lv_obj_add_style(ui->screen_sw_3, &style_screen_sw_3_main_main_disabled, LV_PART_INDICATOR|LV_STATE_DISABLED);

	//Write style state: LV_STATE_DEFAULT for style_screen_sw_3_main_knob_default
	static lv_style_t style_screen_sw_3_main_knob_default;
	if (style_screen_sw_3_main_knob_default.prop_cnt > 1)
		lv_style_reset(&style_screen_sw_3_main_knob_default);
	else
		lv_style_init(&style_screen_sw_3_main_knob_default);
	lv_style_set_radius(&style_screen_sw_3_main_knob_default, 100);
	lv_style_set_bg_color(&style_screen_sw_3_main_knob_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_sw_3_main_knob_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_sw_3_main_knob_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_sw_3_main_knob_default, 255);
	lv_style_set_border_color(&style_screen_sw_3_main_knob_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_border_width(&style_screen_sw_3_main_knob_default, 0);
	lv_style_set_border_opa(&style_screen_sw_3_main_knob_default, 0);
	lv_obj_add_style(ui->screen_sw_3, &style_screen_sw_3_main_knob_default, LV_PART_KNOB|LV_STATE_DEFAULT);

	//Write codes screen_sw_4
	ui->screen_sw_4 = lv_switch_create(ui->screen);
	lv_obj_set_pos(ui->screen_sw_4, 718, 545);
	lv_obj_set_size(ui->screen_sw_4, 30, 18);
	lv_obj_set_scrollbar_mode(ui->screen_sw_4, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_sw_4_main_main_default
	static lv_style_t style_screen_sw_4_main_main_default;
	if (style_screen_sw_4_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_sw_4_main_main_default);
	else
		lv_style_init(&style_screen_sw_4_main_main_default);
	lv_style_set_radius(&style_screen_sw_4_main_main_default, 100);
	lv_style_set_bg_color(&style_screen_sw_4_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_sw_4_main_main_default, lv_color_make(0xe6, 0xe2, 0xe6));
	lv_style_set_bg_grad_dir(&style_screen_sw_4_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_sw_4_main_main_default, 255);
	lv_style_set_border_color(&style_screen_sw_4_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_border_width(&style_screen_sw_4_main_main_default, 2);
	lv_style_set_border_opa(&style_screen_sw_4_main_main_default, 255);
	lv_obj_add_style(ui->screen_sw_4, &style_screen_sw_4_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_FOCUSED for style_screen_sw_4_main_main_focused
	static lv_style_t style_screen_sw_4_main_main_focused;
	if (style_screen_sw_4_main_main_focused.prop_cnt > 1)
		lv_style_reset(&style_screen_sw_4_main_main_focused);
	else
		lv_style_init(&style_screen_sw_4_main_main_focused);
	lv_style_set_radius(&style_screen_sw_4_main_main_focused, 100);
	lv_style_set_bg_color(&style_screen_sw_4_main_main_focused, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_sw_4_main_main_focused, lv_color_make(0xe6, 0xe2, 0xe6));
	lv_style_set_bg_grad_dir(&style_screen_sw_4_main_main_focused, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_sw_4_main_main_focused, 255);
	lv_style_set_border_color(&style_screen_sw_4_main_main_focused, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_border_width(&style_screen_sw_4_main_main_focused, 2);
	lv_style_set_border_opa(&style_screen_sw_4_main_main_focused, 255);
	lv_obj_add_style(ui->screen_sw_4, &style_screen_sw_4_main_main_focused, LV_PART_MAIN|LV_STATE_FOCUSED);

	//Write style state: LV_STATE_DISABLED for style_screen_sw_4_main_main_disabled
	static lv_style_t style_screen_sw_4_main_main_disabled;
	if (style_screen_sw_4_main_main_disabled.prop_cnt > 1)
		lv_style_reset(&style_screen_sw_4_main_main_disabled);
	else
		lv_style_init(&style_screen_sw_4_main_main_disabled);
	lv_style_set_radius(&style_screen_sw_4_main_main_disabled, 100);
	lv_style_set_bg_color(&style_screen_sw_4_main_main_disabled, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_sw_4_main_main_disabled, lv_color_make(0xe6, 0xe2, 0xe6));
	lv_style_set_bg_grad_dir(&style_screen_sw_4_main_main_disabled, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_sw_4_main_main_disabled, 255);
	lv_style_set_border_color(&style_screen_sw_4_main_main_disabled, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_border_width(&style_screen_sw_4_main_main_disabled, 2);
	lv_style_set_border_opa(&style_screen_sw_4_main_main_disabled, 255);
	lv_obj_add_style(ui->screen_sw_4, &style_screen_sw_4_main_main_disabled, LV_PART_MAIN|LV_STATE_DISABLED);

	//Write style state: LV_STATE_CHECKED for style_screen_sw_4_main_indicator_checked
	static lv_style_t style_screen_sw_4_main_indicator_checked;
	if (style_screen_sw_4_main_indicator_checked.prop_cnt > 1)
		lv_style_reset(&style_screen_sw_4_main_indicator_checked);
	else
		lv_style_init(&style_screen_sw_4_main_indicator_checked);
	lv_style_set_bg_color(&style_screen_sw_4_main_indicator_checked, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_sw_4_main_indicator_checked, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_sw_4_main_indicator_checked, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_sw_4_main_indicator_checked, 255);
	lv_style_set_border_color(&style_screen_sw_4_main_indicator_checked, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_border_width(&style_screen_sw_4_main_indicator_checked, 0);
	lv_style_set_border_opa(&style_screen_sw_4_main_indicator_checked, 255);
	lv_obj_add_style(ui->screen_sw_4, &style_screen_sw_4_main_indicator_checked, LV_PART_INDICATOR|LV_STATE_CHECKED);

	//Write style state: LV_STATE_FOCUSED for style_screen_sw_4_main_indicator_focused
	static lv_style_t style_screen_sw_4_main_indicator_focused;
	if (style_screen_sw_4_main_indicator_focused.prop_cnt > 1)
		lv_style_reset(&style_screen_sw_4_main_indicator_focused);
	else
		lv_style_init(&style_screen_sw_4_main_indicator_focused);
	lv_style_set_bg_color(&style_screen_sw_4_main_indicator_focused, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_sw_4_main_indicator_focused, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_sw_4_main_indicator_focused, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_sw_4_main_indicator_focused, 255);
	lv_style_set_border_color(&style_screen_sw_4_main_indicator_focused, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_border_width(&style_screen_sw_4_main_indicator_focused, 2);
	lv_style_set_border_opa(&style_screen_sw_4_main_indicator_focused, 255);
	lv_obj_add_style(ui->screen_sw_4, &style_screen_sw_4_main_indicator_focused, LV_PART_INDICATOR|LV_STATE_FOCUSED);

	//Write style state: LV_STATE_DISABLED for style_screen_sw_4_main_indicator_disabled
	static lv_style_t style_screen_sw_4_main_indicator_disabled;
	if (style_screen_sw_4_main_indicator_disabled.prop_cnt > 1)
		lv_style_reset(&style_screen_sw_4_main_indicator_disabled);
	else
		lv_style_init(&style_screen_sw_4_main_indicator_disabled);
	lv_style_set_bg_color(&style_screen_sw_4_main_indicator_disabled, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_sw_4_main_indicator_disabled, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_sw_4_main_indicator_disabled, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_sw_4_main_indicator_disabled, 255);
	lv_style_set_border_color(&style_screen_sw_4_main_indicator_disabled, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_border_width(&style_screen_sw_4_main_indicator_disabled, 2);
	lv_style_set_border_opa(&style_screen_sw_4_main_indicator_disabled, 255);
	lv_obj_add_style(ui->screen_sw_4, &style_screen_sw_4_main_indicator_disabled, LV_PART_INDICATOR|LV_STATE_DISABLED);
	lv_obj_add_style(ui->screen_sw_4, &style_screen_sw_4_main_main_disabled, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_add_style(ui->screen_sw_4, &style_screen_sw_4_main_main_disabled, LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_add_style(ui->screen_sw_4, &style_screen_sw_4_main_main_disabled, LV_PART_INDICATOR|LV_STATE_FOCUSED);
	lv_obj_add_style(ui->screen_sw_4, &style_screen_sw_4_main_main_disabled, LV_PART_INDICATOR|LV_STATE_FOCUS_KEY);
	lv_obj_add_style(ui->screen_sw_4, &style_screen_sw_4_main_main_disabled, LV_PART_INDICATOR|LV_STATE_EDITED);
	lv_obj_add_style(ui->screen_sw_4, &style_screen_sw_4_main_main_disabled, LV_PART_INDICATOR|LV_STATE_HOVERED);
	lv_obj_add_style(ui->screen_sw_4, &style_screen_sw_4_main_main_disabled, LV_PART_INDICATOR|LV_STATE_PRESSED);
	lv_obj_add_style(ui->screen_sw_4, &style_screen_sw_4_main_main_disabled, LV_PART_INDICATOR|LV_STATE_SCROLLED);
	lv_obj_add_style(ui->screen_sw_4, &style_screen_sw_4_main_main_disabled, LV_PART_INDICATOR|LV_STATE_DISABLED);

	//Write style state: LV_STATE_DEFAULT for style_screen_sw_4_main_knob_default
	static lv_style_t style_screen_sw_4_main_knob_default;
	if (style_screen_sw_4_main_knob_default.prop_cnt > 1)
		lv_style_reset(&style_screen_sw_4_main_knob_default);
	else
		lv_style_init(&style_screen_sw_4_main_knob_default);
	lv_style_set_radius(&style_screen_sw_4_main_knob_default, 100);
	lv_style_set_bg_color(&style_screen_sw_4_main_knob_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_sw_4_main_knob_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_sw_4_main_knob_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_sw_4_main_knob_default, 255);
	lv_style_set_border_color(&style_screen_sw_4_main_knob_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_border_width(&style_screen_sw_4_main_knob_default, 0);
	lv_style_set_border_opa(&style_screen_sw_4_main_knob_default, 0);
	lv_obj_add_style(ui->screen_sw_4, &style_screen_sw_4_main_knob_default, LV_PART_KNOB|LV_STATE_DEFAULT);

	//Write codes screen_sw_5
	ui->screen_sw_5 = lv_switch_create(ui->screen);
	lv_obj_set_pos(ui->screen_sw_5, 866, 545);
	lv_obj_set_size(ui->screen_sw_5, 30, 18);
	lv_obj_set_scrollbar_mode(ui->screen_sw_5, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_sw_5_main_main_default
	static lv_style_t style_screen_sw_5_main_main_default;
	if (style_screen_sw_5_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_sw_5_main_main_default);
	else
		lv_style_init(&style_screen_sw_5_main_main_default);
	lv_style_set_radius(&style_screen_sw_5_main_main_default, 100);
	lv_style_set_bg_color(&style_screen_sw_5_main_main_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_sw_5_main_main_default, lv_color_make(0xe6, 0xe2, 0xe6));
	lv_style_set_bg_grad_dir(&style_screen_sw_5_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_sw_5_main_main_default, 255);
	lv_style_set_border_color(&style_screen_sw_5_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_border_width(&style_screen_sw_5_main_main_default, 2);
	lv_style_set_border_opa(&style_screen_sw_5_main_main_default, 255);
	lv_obj_add_style(ui->screen_sw_5, &style_screen_sw_5_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_FOCUSED for style_screen_sw_5_main_main_focused
	static lv_style_t style_screen_sw_5_main_main_focused;
	if (style_screen_sw_5_main_main_focused.prop_cnt > 1)
		lv_style_reset(&style_screen_sw_5_main_main_focused);
	else
		lv_style_init(&style_screen_sw_5_main_main_focused);
	lv_style_set_radius(&style_screen_sw_5_main_main_focused, 100);
	lv_style_set_bg_color(&style_screen_sw_5_main_main_focused, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_sw_5_main_main_focused, lv_color_make(0xe6, 0xe2, 0xe6));
	lv_style_set_bg_grad_dir(&style_screen_sw_5_main_main_focused, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_sw_5_main_main_focused, 255);
	lv_style_set_border_color(&style_screen_sw_5_main_main_focused, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_border_width(&style_screen_sw_5_main_main_focused, 2);
	lv_style_set_border_opa(&style_screen_sw_5_main_main_focused, 255);
	lv_obj_add_style(ui->screen_sw_5, &style_screen_sw_5_main_main_focused, LV_PART_MAIN|LV_STATE_FOCUSED);

	//Write style state: LV_STATE_DISABLED for style_screen_sw_5_main_main_disabled
	static lv_style_t style_screen_sw_5_main_main_disabled;
	if (style_screen_sw_5_main_main_disabled.prop_cnt > 1)
		lv_style_reset(&style_screen_sw_5_main_main_disabled);
	else
		lv_style_init(&style_screen_sw_5_main_main_disabled);
	lv_style_set_radius(&style_screen_sw_5_main_main_disabled, 100);
	lv_style_set_bg_color(&style_screen_sw_5_main_main_disabled, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_sw_5_main_main_disabled, lv_color_make(0xe6, 0xe2, 0xe6));
	lv_style_set_bg_grad_dir(&style_screen_sw_5_main_main_disabled, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_sw_5_main_main_disabled, 255);
	lv_style_set_border_color(&style_screen_sw_5_main_main_disabled, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_border_width(&style_screen_sw_5_main_main_disabled, 2);
	lv_style_set_border_opa(&style_screen_sw_5_main_main_disabled, 255);
	lv_obj_add_style(ui->screen_sw_5, &style_screen_sw_5_main_main_disabled, LV_PART_MAIN|LV_STATE_DISABLED);

	//Write style state: LV_STATE_CHECKED for style_screen_sw_5_main_indicator_checked
	static lv_style_t style_screen_sw_5_main_indicator_checked;
	if (style_screen_sw_5_main_indicator_checked.prop_cnt > 1)
		lv_style_reset(&style_screen_sw_5_main_indicator_checked);
	else
		lv_style_init(&style_screen_sw_5_main_indicator_checked);
	lv_style_set_bg_color(&style_screen_sw_5_main_indicator_checked, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_sw_5_main_indicator_checked, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_sw_5_main_indicator_checked, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_sw_5_main_indicator_checked, 255);
	lv_style_set_border_color(&style_screen_sw_5_main_indicator_checked, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_border_width(&style_screen_sw_5_main_indicator_checked, 0);
	lv_style_set_border_opa(&style_screen_sw_5_main_indicator_checked, 255);
	lv_obj_add_style(ui->screen_sw_5, &style_screen_sw_5_main_indicator_checked, LV_PART_INDICATOR|LV_STATE_CHECKED);

	//Write style state: LV_STATE_FOCUSED for style_screen_sw_5_main_indicator_focused
	static lv_style_t style_screen_sw_5_main_indicator_focused;
	if (style_screen_sw_5_main_indicator_focused.prop_cnt > 1)
		lv_style_reset(&style_screen_sw_5_main_indicator_focused);
	else
		lv_style_init(&style_screen_sw_5_main_indicator_focused);
	lv_style_set_bg_color(&style_screen_sw_5_main_indicator_focused, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_sw_5_main_indicator_focused, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_sw_5_main_indicator_focused, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_sw_5_main_indicator_focused, 255);
	lv_style_set_border_color(&style_screen_sw_5_main_indicator_focused, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_border_width(&style_screen_sw_5_main_indicator_focused, 2);
	lv_style_set_border_opa(&style_screen_sw_5_main_indicator_focused, 255);
	lv_obj_add_style(ui->screen_sw_5, &style_screen_sw_5_main_indicator_focused, LV_PART_INDICATOR|LV_STATE_FOCUSED);

	//Write style state: LV_STATE_DISABLED for style_screen_sw_5_main_indicator_disabled
	static lv_style_t style_screen_sw_5_main_indicator_disabled;
	if (style_screen_sw_5_main_indicator_disabled.prop_cnt > 1)
		lv_style_reset(&style_screen_sw_5_main_indicator_disabled);
	else
		lv_style_init(&style_screen_sw_5_main_indicator_disabled);
	lv_style_set_bg_color(&style_screen_sw_5_main_indicator_disabled, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_bg_grad_color(&style_screen_sw_5_main_indicator_disabled, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_sw_5_main_indicator_disabled, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_sw_5_main_indicator_disabled, 255);
	lv_style_set_border_color(&style_screen_sw_5_main_indicator_disabled, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_border_width(&style_screen_sw_5_main_indicator_disabled, 2);
	lv_style_set_border_opa(&style_screen_sw_5_main_indicator_disabled, 255);
	lv_obj_add_style(ui->screen_sw_5, &style_screen_sw_5_main_indicator_disabled, LV_PART_INDICATOR|LV_STATE_DISABLED);
	lv_obj_add_style(ui->screen_sw_5, &style_screen_sw_5_main_main_disabled, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_add_style(ui->screen_sw_5, &style_screen_sw_5_main_main_disabled, LV_PART_INDICATOR|LV_STATE_CHECKED);
	lv_obj_add_style(ui->screen_sw_5, &style_screen_sw_5_main_main_disabled, LV_PART_INDICATOR|LV_STATE_FOCUSED);
	lv_obj_add_style(ui->screen_sw_5, &style_screen_sw_5_main_main_disabled, LV_PART_INDICATOR|LV_STATE_FOCUS_KEY);
	lv_obj_add_style(ui->screen_sw_5, &style_screen_sw_5_main_main_disabled, LV_PART_INDICATOR|LV_STATE_EDITED);
	lv_obj_add_style(ui->screen_sw_5, &style_screen_sw_5_main_main_disabled, LV_PART_INDICATOR|LV_STATE_HOVERED);
	lv_obj_add_style(ui->screen_sw_5, &style_screen_sw_5_main_main_disabled, LV_PART_INDICATOR|LV_STATE_PRESSED);
	lv_obj_add_style(ui->screen_sw_5, &style_screen_sw_5_main_main_disabled, LV_PART_INDICATOR|LV_STATE_SCROLLED);
	lv_obj_add_style(ui->screen_sw_5, &style_screen_sw_5_main_main_disabled, LV_PART_INDICATOR|LV_STATE_DISABLED);

	//Write style state: LV_STATE_DEFAULT for style_screen_sw_5_main_knob_default
	static lv_style_t style_screen_sw_5_main_knob_default;
	if (style_screen_sw_5_main_knob_default.prop_cnt > 1)
		lv_style_reset(&style_screen_sw_5_main_knob_default);
	else
		lv_style_init(&style_screen_sw_5_main_knob_default);
	lv_style_set_radius(&style_screen_sw_5_main_knob_default, 100);
	lv_style_set_bg_color(&style_screen_sw_5_main_knob_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_sw_5_main_knob_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_sw_5_main_knob_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_sw_5_main_knob_default, 255);
	lv_style_set_border_color(&style_screen_sw_5_main_knob_default, lv_color_make(0x00, 0x00, 0x00));
	lv_style_set_border_width(&style_screen_sw_5_main_knob_default, 0);
	lv_style_set_border_opa(&style_screen_sw_5_main_knob_default, 0);
	lv_obj_add_style(ui->screen_sw_5, &style_screen_sw_5_main_knob_default, LV_PART_KNOB|LV_STATE_DEFAULT);
// LV_STATE_PRESSED

	//Write codes screen_slider_1
	ui->screen_slider_1 = lv_slider_create(ui->screen);
	lv_obj_set_pos(ui->screen_slider_1, 728, 270);
	lv_obj_set_size(ui->screen_slider_1, 250, 5);
	lv_obj_set_scrollbar_mode(ui->screen_slider_1, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_slider_1_main_main_default
	static lv_style_t style_screen_slider_1_main_main_default;
	if (style_screen_slider_1_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_slider_1_main_main_default);
	else
		lv_style_init(&style_screen_slider_1_main_main_default);
	lv_style_set_radius(&style_screen_slider_1_main_main_default, 50);
	lv_style_set_bg_color(&style_screen_slider_1_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_slider_1_main_main_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_slider_1_main_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_slider_1_main_main_default, 100);
	lv_style_set_outline_color(&style_screen_slider_1_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_outline_width(&style_screen_slider_1_main_main_default, 0);
	lv_style_set_outline_opa(&style_screen_slider_1_main_main_default, 0);
	lv_obj_add_style(ui->screen_slider_1, &style_screen_slider_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_screen_slider_1_main_indicator_default
	static lv_style_t style_screen_slider_1_main_indicator_default;
	if (style_screen_slider_1_main_indicator_default.prop_cnt > 1)
		lv_style_reset(&style_screen_slider_1_main_indicator_default);
	else
		lv_style_init(&style_screen_slider_1_main_indicator_default);
	lv_style_set_radius(&style_screen_slider_1_main_indicator_default, 50);
	lv_style_set_bg_color(&style_screen_slider_1_main_indicator_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_slider_1_main_indicator_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_slider_1_main_indicator_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_slider_1_main_indicator_default, 255);
	lv_obj_add_style(ui->screen_slider_1, &style_screen_slider_1_main_indicator_default, LV_PART_INDICATOR|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for style_screen_slider_1_main_knob_default
	static lv_style_t style_screen_slider_1_main_knob_default;
	if (style_screen_slider_1_main_knob_default.prop_cnt > 1)
		lv_style_reset(&style_screen_slider_1_main_knob_default);
	else
		lv_style_init(&style_screen_slider_1_main_knob_default);
	lv_style_set_radius(&style_screen_slider_1_main_knob_default, 50);
	lv_style_set_bg_color(&style_screen_slider_1_main_knob_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_slider_1_main_knob_default, lv_color_make(0x21, 0x95, 0xf6));
	lv_style_set_bg_grad_dir(&style_screen_slider_1_main_knob_default, LV_GRAD_DIR_NONE);
	lv_style_set_bg_opa(&style_screen_slider_1_main_knob_default, 255);
	lv_obj_add_style(ui->screen_slider_1, &style_screen_slider_1_main_knob_default, LV_PART_KNOB|LV_STATE_DEFAULT);
	lv_slider_set_range(ui->screen_slider_1,0, 100);
	lv_slider_set_value(ui->screen_slider_1,50,false);

	//Write codes screen_img_m13
	ui->screen_img_m13 = lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_m13, 488, 376);
	lv_img_set_src(ui->screen_img_m13,read_png_path[LIGHT]);

	//Write codes screen_img_1
	ui->screen_img_1 = lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_1, 453, 47);
	// lv_obj_set_size(ui->screen_img_1, 193, 203);
	lv_obj_set_scrollbar_mode(ui->screen_img_1, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_img_1_main_main_default
	static lv_style_t style_screen_img_1_main_main_default;
	if (style_screen_img_1_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_img_1_main_main_default);
	else
		lv_style_init(&style_screen_img_1_main_main_default);
	lv_style_set_img_recolor(&style_screen_img_1_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_img_1_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_img_1_main_main_default, 255);
	lv_obj_add_style(ui->screen_img_1, &style_screen_img_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_img_1, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_img_1,&_mode_fun_big_193x203);
	lv_img_set_pivot(ui->screen_img_1, 50,50);
	lv_img_set_angle(ui->screen_img_1, 0);

	lv_obj_invalidate(ui->screen_img_1);
	lv_img_set_antialias(ui->screen_img_1, true);
	lv_img_set_size_mode(ui->screen_img_1, LV_IMG_SIZE_MODE_VIRTUAL);
	lv_img_set_zoom(ui->screen_img_1, LV_IMG_ZOOM_NONE - 105);
	lv_obj_set_pos(ui->screen_img_1, 433, 27); /*未点击时，在左上角的坐标*/
	// lv_obj_set_pos(ui->screen_img_1, 487, 65);    /*位于中心点时的坐标*/

	//Write codes screen_img_2
	ui->screen_img_2 = lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_2, 566, 48);
	// lv_obj_set_size(ui->screen_img_2, 193, 203);
	lv_obj_set_scrollbar_mode(ui->screen_img_2, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_img_2_main_main_default
	static lv_style_t style_screen_img_2_main_main_default;
	if (style_screen_img_2_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_img_2_main_main_default);
	else
		lv_style_init(&style_screen_img_2_main_main_default);
	lv_style_set_img_recolor(&style_screen_img_2_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_img_2_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_img_2_main_main_default, 255);
	lv_obj_add_style(ui->screen_img_2, &style_screen_img_2_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_img_2, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_img_2,&_mode_sleep_big_193x203);
	lv_img_set_pivot(ui->screen_img_2, 50,50); 	
	lv_img_set_angle(ui->screen_img_2, 0);   	

	lv_obj_invalidate(ui->screen_img_2);
	lv_img_set_antialias(ui->screen_img_2, true);
	lv_img_set_size_mode(ui->screen_img_2, LV_IMG_SIZE_MODE_VIRTUAL);
	lv_img_set_zoom(ui->screen_img_2, LV_IMG_ZOOM_NONE - 105);
	lv_obj_set_pos(ui->screen_img_2, 546, 28);	/*未点击时，在左上角的坐标*/
	// lv_obj_set_pos(ui->screen_img_2, 492, 66);		/*位于中心点时的坐标*/

	//Write codes screen_img_3
	ui->screen_img_3 = lv_img_create(ui->screen);
	lv_obj_set_pos(ui->screen_img_3, 508, 142);
	// lv_obj_set_size(ui->screen_img_3, 193, 203);
	lv_obj_set_scrollbar_mode(ui->screen_img_3, LV_SCROLLBAR_MODE_OFF);

	//Write style state: LV_STATE_DEFAULT for style_screen_img_3_main_main_default
	static lv_style_t style_screen_img_3_main_main_default;
	if (style_screen_img_3_main_main_default.prop_cnt > 1)
		lv_style_reset(&style_screen_img_3_main_main_default);
	else
		lv_style_init(&style_screen_img_3_main_main_default);
	lv_style_set_img_recolor(&style_screen_img_3_main_main_default, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_img_recolor_opa(&style_screen_img_3_main_main_default, 0);
	lv_style_set_img_opa(&style_screen_img_3_main_main_default, 255);
	lv_obj_add_style(ui->screen_img_3, &style_screen_img_3_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_flag(ui->screen_img_3, LV_OBJ_FLAG_CLICKABLE);
	lv_img_set_src(ui->screen_img_3,&_mode_leave_big_193x203);
	lv_img_set_pivot(ui->screen_img_3, 50,50);	
	lv_img_set_angle(ui->screen_img_3, 0);

	lv_obj_invalidate(ui->screen_img_3);
	lv_img_set_antialias(ui->screen_img_3, true);
	lv_img_set_size_mode(ui->screen_img_3, LV_IMG_SIZE_MODE_VIRTUAL);
	lv_img_set_zoom(ui->screen_img_3, LV_IMG_ZOOM_NONE - 105);
	lv_obj_set_pos(ui->screen_img_3, 488, 118);	/*未点击时，在左上角的坐标*/
	// lv_obj_set_pos(ui->screen_img_3, 488, 65);	/*位于中心点时的坐标*/

	// lv_obj_set_ext_click_area(arc, 5);

	ui_M3_t* ui_data = get_global_ui_data();
	screen_mode_ui_init(ui_data->home_mode_option.mode, ui);
	screen_music_ui_init(ui_data->lift_mode_option.mode, ui);
	screen_wea_anim(ui, ui_data->week_wea.wea_index);
	screen_light_ui_init(ui);


	screen_flag_init(ui);
	screen_add_event_handler(ui);
}

// 小尺寸
// 原位置						中心位置
	// fun:  (433, 27)			(487, 65)	
	// sleep:(546, 28)			(492, 66)	
	// leave:(488, 118)			(488, 65)	

// 大尺寸
// 原位置						中心位置
	// fun:  (433, 27)			(475， 40)	
	// sleep:(546, 28)			(475， 40)	
	// leave:(488, 118)			(475， 40)	
	