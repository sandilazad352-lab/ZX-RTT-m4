/*
 * Copyright 2024 NXP
 * SPDX-License-Identifier: MIT
 * The auto-generated can only be used on NXP devices
 */

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"
#include "guider_fonts.h"

typedef struct
{
	lv_obj_t *screen;
	bool screen_del;
	lv_obj_t *screen_label_39;
	lv_obj_t *screen_label_1;
	lv_obj_t *screen_label_2;
	lv_obj_t *screen_label_3;
	lv_obj_t *screen_label_4;
	lv_obj_t *screen_label_5;
	lv_obj_t *screen_label_6;
	lv_obj_t *screen_label_7;
	lv_obj_t *screen_label_8;
	lv_obj_t *screen_label_9;
	lv_obj_t *screen_label_10;
	lv_obj_t *screen_label_11;
	lv_obj_t *screen_label_12;
	lv_obj_t *screen_label_13;
	lv_obj_t *screen_label_14;
	lv_obj_t *screen_label_15;
	lv_obj_t *screen_label_16;
	lv_obj_t *screen_label_17;
	lv_obj_t *screen_label_18;
	lv_obj_t *screen_label_19;
	lv_obj_t *screen_label_20;
	lv_obj_t *screen_label_21;
	lv_obj_t *screen_label_22;
	lv_obj_t *screen_label_23;
	lv_obj_t *screen_label_24;
	lv_obj_t *screen_label_25;
	lv_obj_t *screen_label_26;
	lv_obj_t *screen_label_27;
	lv_obj_t *screen_label_28;
	lv_obj_t *screen_label_29;
	lv_obj_t *screen_label_30;
	lv_obj_t *screen_label_31;
	lv_obj_t *screen_label_32;
	lv_obj_t *screen_label_33;
	lv_obj_t *screen_label_34;
	lv_obj_t *screen_label_35;
	lv_obj_t *screen_label_36;
	lv_obj_t *screen_label_37;
	lv_obj_t *screen_label_38;
	lv_obj_t *screen_img_1;
	lv_obj_t *screen_img_2;
	lv_obj_t *screen_img_3;
	lv_obj_t *screen_sw_1;
	lv_obj_t *screen_sw_2;
	lv_obj_t *screen_sw_3;
	lv_obj_t *screen_sw_4;
	lv_obj_t *screen_sw_5;
	lv_obj_t *screen_slider_1;
	// lv_obj_t *screen_img_4;

	lv_obj_t *screen_img_m1; // 页面底图
	lv_obj_t *screen_img_m2; // wifi图标
	lv_obj_t *screen_img_m3; // 模式选择背景图片
	lv_obj_t *screen_img_m4; // 音乐背景图片
	lv_obj_t *screen_img_m5; // 天气背景图片
	lv_obj_t *screen_img_m6; // 星期一按钮背景图片
	lv_obj_t *screen_img_m7; // 星期二按钮背景图片
	lv_obj_t *screen_img_m8; // 星期三按钮背景图片
	lv_obj_t *screen_img_m9; // 星期四按钮背景图片
	lv_obj_t *screen_img_m10; // 星期五按钮背景图片
	lv_obj_t *screen_img_m11; // 星期六按钮背景图片
	lv_obj_t *screen_img_m12; // 星期日按钮背景图片
	lv_obj_t *screen_img_m13; // 灯光选项 -- 灯图标
	lv_obj_t *screen_img_m14; // 灯光选项 -- 灯光1
	lv_obj_t *screen_img_m15; // 灯光选项 -- 灯光1 color
	lv_obj_t *screen_img_m16; // 灯光选项 -- 灯光1 yellow 
	lv_obj_t *screen_img_m17; // 灯光选项 -- 灯光1 pink
	lv_obj_t *screen_img_m18; // 灯光选项 -- 灯光1 purpel
	lv_obj_t *screen_img_m19; // 灯光选项 -- 灯光1 blue
	lv_obj_t *screen_img_m20; // 灯光选项 -- 灯光2
	lv_obj_t *screen_img_m21; // 灯光选项 -- 灯光2 color
	lv_obj_t *screen_img_m22; // 灯光选项 -- 灯光2 yellow 
	lv_obj_t *screen_img_m23; // 灯光选项 -- 灯光2 pink
	lv_obj_t *screen_img_m24; // 灯光选项 -- 灯光2 purpel
	lv_obj_t *screen_img_m25; // 灯光选项 -- 灯光2 blue
	lv_obj_t *screen_img_m26; // 灯光选项 -- 灯光3
	lv_obj_t *screen_img_m27; // 灯光选项 -- 灯光3 color
	lv_obj_t *screen_img_m28; // 灯光选项 -- 灯光3 yellow 
	lv_obj_t *screen_img_m29; // 灯光选项 -- 灯光3 pink
	lv_obj_t *screen_img_m30; // 灯光选项 -- 灯光3 purpel
	lv_obj_t *screen_img_m31; // 灯光选项 -- 灯光3 blue
	lv_obj_t *screen_img_m32; // 灯光选项 -- 灯光4
	lv_obj_t *screen_img_m33; // 灯光选项 -- 灯光4 color
	lv_obj_t *screen_img_m34; // 灯光选项 -- 灯光4 yellow 
	lv_obj_t *screen_img_m35; // 灯光选项 -- 灯光4 pink
	lv_obj_t *screen_img_m36; // 灯光选项 -- 灯光4 purpel
	lv_obj_t *screen_img_m37; // 灯光选项 -- 灯光4 blue
	lv_obj_t *screen_img_m38; // 电视选项
	lv_obj_t *screen_img_m39; // 空调选项

	lv_obj_t *screen_img_m40; // 星期一按钮天气图片
	lv_obj_t *screen_img_m41; // 星期二按钮天气图片
	lv_obj_t *screen_img_m42; // 星期三按钮天气图片
	lv_obj_t *screen_img_m43; // 星期四按钮天气图片
	lv_obj_t *screen_img_m44; // 星期五按钮天气图片
	lv_obj_t *screen_img_m45; // 天气图片

	lv_obj_t *screen_img_m46; // 音乐图片
	lv_obj_t *screen_img_m47; // 空调图片
	lv_obj_t *screen_img_m48; // 音乐操作键
	lv_obj_t *screen_img_m49; // 空调操作键

	lv_obj_t *test_mode;
	lv_obj_t *test_music;
	lv_obj_t *test_wea;

	lv_obj_t *screen_1;
	lv_obj_t *screen_1_img_1; // 背景
	lv_obj_t *screen_1_img_2; // 当地信息
	lv_obj_t *screen_1_img_3; // 天气信息
	lv_obj_t *screen_1_img_4; // 数据信息
	bool screen_1_del;
	lv_obj_t *screen_2;
	bool screen_2_del;
	lv_obj_t *screen_2_img_1;
	lv_obj_t *screen_2_img_2;
	lv_obj_t *screen_2_img_3;
	lv_obj_t *screen_2_img_4;
	lv_obj_t *screen_2_img_5;
	lv_obj_t *screen_2_img_6;
	lv_obj_t *screen_2_img_7;
	lv_obj_t *screen_2_img_8;
	lv_obj_t *screen_2_img_9;
	lv_obj_t *screen_2_img_10;
	lv_obj_t *screen_2_label_1;
	lv_obj_t *screen_2_label_2;
	lv_obj_t *screen_2_label_3;
}lv_ui;

void init_scr_del_flag(lv_ui *ui);
void setup_ui(lv_ui *ui);
extern lv_ui guider_ui;
void setup_scr_screen(lv_ui *ui);
void setup_scr_screen_1(lv_ui *ui);
void setup_scr_screen_2(lv_ui *ui);
LV_IMG_DECLARE(_mode_sleep_big_194x204);
LV_IMG_DECLARE(_mode_leave_small_116x120);
LV_IMG_DECLARE(_mode_leave_big_193x206);
LV_IMG_DECLARE(_mode_fun_small_115x120);
LV_IMG_DECLARE(_mode_leave_small_115x120);
LV_IMG_DECLARE(_music_btn_190x33);
LV_IMG_DECLARE(_curtain_icon_115x115);
LV_IMG_DECLARE(_mode_fun_big_193x203);
LV_IMG_DECLARE(_cur_btn_225x25);
LV_IMG_DECLARE(_mode_sleep_small_114x120);
LV_IMG_DECLARE(_mode_sleep_small_115x120);
LV_IMG_DECLARE(_music_icon_318x277);
LV_IMG_DECLARE(_mode_sleep_big_193x203);
LV_IMG_DECLARE(_mode_leave_big_193x203);

#define PNG_PATH "L:/rodata/"
#define PNG_FILE(file_name) PNG_PATH file_name

enum{
// 首页面
	MAIN_BOTTOM,		// 首页面背景图
	MAIN_MODE_BOTTOM,	// 模式切换背景图
	MAIN_MULTI_BOTOM,	// 音乐背景图
	MAIN_WEA_BOTTOM,	// 天气信息展示背景图
	WIFI_ICON,			// wifi图标
	BTN_NOPRESS,		// 按钮未被按下背景图
	BTN_PRESS,			// 按钮被按下背景图
	WEA_CLO_BIG,		// 多云天气展示图片 -- 大尺寸
	WEA_RAIN_BIG,		// 中雨天气展示图片 -- 大尺寸
	WEA_SUN_BIG,		// 晴天天气展示图片 -- 大尺寸
	WEA_CLO_SMALL,		// 多云天气展示图片 -- 小尺寸
	WEA_RAIN_SMALL,		// 中雨天气展示图片 -- 小尺寸
	WEA_SUN_SMALL,		// 晴天天气展示图片 -- 小尺寸
	LIGHT,				// 灯光选项未被展开时的灯光图标
	LIGHT_OFF,			// 灯光选项被展开时的灯光图标 -- 关闭
	LIGHT_YELLOW,		// 灯光选项被展开时的灯光图标 -- 黄色
	LIGHT_PINK,			// 灯光选项被展开时的灯光图标 -- 粉红色
	LIGHT_PURPLE,		// 灯光选项被展开时的灯光图标 -- 紫色
	LIGHT_BLUE,			// 灯光选项被展开时的灯光图标 -- 蓝色
	SIGN_COLOR,			// 灯光选项被展开时的切换灯光图标 -- 彩色 -- 表示可切换颜色
	SIGN_YELLOW,		// 灯光选项被展开时的切换灯光图标 -- 表示灯光可切换为黄色
	SIGN_PINK,			// 灯光选项被展开时的切换灯光图标 -- 表示灯光可切换为粉色
	SIGN_PURPLE,		// 灯光选项被展开时的切换灯光图标 --  表示灯光可切换为紫色
	SIGN_BLUE,			// 灯光选项被展开时的切换灯光图标 --  表示灯光可切换为蓝色
	SMART_TV,			// 电视机选项
	SMART_AIR,			// 空调选项
// 第二页面
	SECOND_BOTTOM,		// 第二页面背景图 
	SECOND_LOCAL,		// 当地信息展示功能
	SECOND_WEA,			// 天气信息展示功能
	SECOND_TEST,			// 其他数据展示功能
// 测试
	TEST_MODE,
	TEST_MUSIC,
	TEST_WEA,
};

typedef struct _ui_mode_t {
	int8_t mode;//0: 未选择模式; 1: 娱乐模式; 2:睡眠模式; 3:离家模式
} ui_mode_t;

typedef struct _ui_music_t {
	int8_t mode;// 1: 音乐模式; 2: 窗帘模式
} ui_music_t;

typedef struct _ui_weather_day_t {
	int8_t date;// 1: 表示星期一,以此类推
	int8_t temp;// 表示温度
	char   wea[10];// 里面的字符串表示天气
}ui_weather_day_t;

typedef struct _ui_weather_t {
	ui_weather_day_t weather[5];
	int8_t wea_index;//0: 表示当前没有按下任何星期按钮, 1:代表按下星期一按钮,以此类推
} ui_weather_t;

typedef struct _ui_light_option_t {
	bool switch_sign;  // true: 表示该选项已经打开; false: 表示该选项未被打开
	bool color;		   // true: 需要展开颜色， false: 不需要展开颜色
	int8_t color_sign; //0: 黄色; 1: 粉色;2: 紫色;3: 蓝色;
} ui_light_option_t;

typedef struct _ui_light_t {
	bool option_sign;// true: 表示该选项已经打开; false: 表示该选项未被打开
	ui_light_option_t light_option[4];//存储四个灯具的信息
} ui_light_t;

typedef struct _ui_M3_t {
	ui_mode_t 	 home_mode_option;// 家庭模式【娱乐、离家、睡眠】
	ui_music_t	 lift_mode_option;// 生活模式【音乐、窗帘】
	ui_weather_t week_wea;		  // 记录一个星期的天气情况
	ui_light_t	 home_light;	  // 家庭灯具信息
} ui_M3_t;

void ui_data_init(void);

ui_M3_t* get_global_ui_data(void);

#ifdef __cpluspluss
}
#endif
#endif
