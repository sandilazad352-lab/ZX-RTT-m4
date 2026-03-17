/*
 * @Author: BigBee-yellowyellow 2956747896@qq.com
 * @Date: 2024-04-11 17:12:39
 * @LastEditors: BigBee-yellowyellow 2956747896@qq.com
 * @LastEditTime: 2024-04-17 13:51:16
 * @FilePath: /weather_clock_700_M3/components/generated/gui_guider.c
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

// 图片路径枚举
char* read_png_path[] = {
// 首页面
	PNG_FILE("main_bottom.png"), 		// 首页面背景图
	PNG_FILE("main_mode_bottom.png"), 	// 模式切换背景图
	PNG_FILE("main_multi_bottom.png"), 	// 音乐背景图
	PNG_FILE("main_wea_bottom.png"), 	// 天气信息展示背景图

	PNG_FILE("wifi_icon.png"), 			// wifi图标

	PNG_FILE("btn_nopress.png"), 		// 按钮未被按下背景图
	PNG_FILE("btn_press.png"),   		// 按钮被按下背景图

	PNG_FILE("wea_clo_big.png"),   		// 多云天气展示图片 -- 大尺寸
	PNG_FILE("wea_rain_big.png"),   	// 多云天气展示图片 -- 大尺寸
	PNG_FILE("wea_sun_big.png"),   		// 多云天气展示图片 -- 大尺寸
	PNG_FILE("wea_clo_small.png"),  	// 多云天气展示图片 -- 小尺寸
	PNG_FILE("wea_rain_small.png"), 	// 多云天气展示图片 -- 小尺寸
	PNG_FILE("wea_sun_small.png"),  	// 多云天气展示图片 -- 小尺寸

	PNG_FILE("light.png"),   		// 灯光选项未被展开时的灯光图标
	PNG_FILE("light_off.png"),   	// 灯光选项被展开时的灯光图标 -- 关闭
	PNG_FILE("light_yellow.png"),   // 灯光选项被展开时的灯光图标 -- 黄色
	PNG_FILE("light_pink.png"),   	// 灯光选项被展开时的灯光图标 -- 粉红色
	PNG_FILE("light_purple.png"),   // 灯光选项被展开时的灯光图标 -- 紫色
	PNG_FILE("light_blue.png"),   	// 灯光选项被展开时的灯光图标 -- 蓝色
	PNG_FILE("sign_color.png"),   	// 灯光选项被展开时的切换灯光图标 -- 彩色 -- 表示可切换颜色页面未展开
	PNG_FILE("sign_yellow.png"),   	// 灯光选项被展开时的切换灯光图标 -- 表示灯光可切换为黄色
	PNG_FILE("sign_pink.png"),   	// 灯光选项被展开时的切换灯光图标 -- 表示灯光可切换为粉色
	PNG_FILE("sign_purple.png"),   	// 灯光选项被展开时的切换灯光图标 --  表示灯光可切换为紫色
	PNG_FILE("sign_blue.png"),   	// 灯光选项被展开时的切换灯光图标 --  表示灯光可切换为蓝色

	PNG_FILE("smart_tv.png"),   	// 电视机选项
	PNG_FILE("smart_air.png"),   	// 空调选项
// 第二页面
	PNG_FILE("second_bottom.png"),  // 第二页面背景图 
	PNG_FILE("second_local.png"),  	// 当地信息展示功能 
	PNG_FILE("second_wea.png"),  	// 天气信息展示功能 
	PNG_FILE("second_test.png"),  	// 其他数据展示功能 
// 测试
	PNG_FILE("Frame34.png"),
	PNG_FILE("Frame35.png"),  
	PNG_FILE("Frame36.png"), 
};

void init_scr_del_flag(lv_ui *ui){
	ui->screen_del = true;
	ui->screen_1_del = true;
	ui->screen_2_del = true;
}

extern void bee_lv_example_png_1(void);

void setup_ui(lv_ui *ui){
	init_scr_del_flag(ui);
	setup_scr_screen(ui);
	setup_scr_screen_1(ui);
	lv_scr_load(ui->screen);
}

char* wea_name[5] = {
	"中雨", "中雨", "多云", "晴", "晴"
};

int8_t wea_tem[5] = {
	23, 19, 25, 29, 29
};

int8_t wea_hum[5] = {
	30, 50, 10, 5, 5
};

int8_t wea_btn_bot[2] = {
	BTN_NOPRESS, BTN_PRESS
};

int8_t wea_img_big[5] = {
	WEA_RAIN_BIG, WEA_RAIN_BIG, WEA_CLO_BIG, WEA_SUN_BIG, WEA_SUN_BIG
};

int8_t wea_img_small[5] = {
	WEA_RAIN_SMALL, WEA_RAIN_SMALL, WEA_CLO_SMALL, WEA_SUN_SMALL, WEA_SUN_SMALL
};

int8_t light_img[4] = {
	LIGHT_YELLOW, LIGHT_PINK, LIGHT_PURPLE, LIGHT_BLUE
};

ui_M3_t global_ui_data;
void ui_data_init(void)
{
	global_ui_data.home_mode_option.mode = 0;// 初始化为待选择模式
	global_ui_data.lift_mode_option.mode = 1;// 初始化为音乐模式
	global_ui_data.week_wea.wea_index = 1;
	global_ui_data.week_wea.weather[0].date = 1;
	global_ui_data.week_wea.weather[0].temp = 23;
	memcpy(global_ui_data.week_wea.weather[0].wea, wea_name[0], strlen(wea_name[0]));
	global_ui_data.week_wea.weather[1].date = 2;
	global_ui_data.week_wea.weather[1].temp = 22;
	memcpy(global_ui_data.week_wea.weather[1].wea, wea_name[1], strlen(wea_name[1]));
	global_ui_data.week_wea.weather[2].date = 3;
	global_ui_data.week_wea.weather[2].temp = 25;
	memcpy(global_ui_data.week_wea.weather[2].wea, wea_name[2], strlen(wea_name[2]));
	global_ui_data.week_wea.weather[3].date = 4;
	global_ui_data.week_wea.weather[3].temp = 29;
	memcpy(global_ui_data.week_wea.weather[3].wea, wea_name[3], strlen(wea_name[3]));
	global_ui_data.week_wea.weather[4].date = 5;
	global_ui_data.week_wea.weather[4].temp = 29;
	memcpy(global_ui_data.week_wea.weather[4].wea, wea_name[4], strlen(wea_name[4]));

	global_ui_data.home_light.option_sign = false;
	global_ui_data.home_light.light_option[0].switch_sign = false;
	global_ui_data.home_light.light_option[0].color = false;
	global_ui_data.home_light.light_option[0].color_sign  = 0;
	global_ui_data.home_light.light_option[1].switch_sign = false;
	global_ui_data.home_light.light_option[1].color = false;
	global_ui_data.home_light.light_option[1].color_sign  = 0;
	global_ui_data.home_light.light_option[2].switch_sign = false;
	global_ui_data.home_light.light_option[2].color = false;
	global_ui_data.home_light.light_option[2].color_sign  = 0;
	global_ui_data.home_light.light_option[3].switch_sign = false;
	global_ui_data.home_light.light_option[3].color = false;
	global_ui_data.home_light.light_option[3].color_sign  = 0;
}

ui_M3_t* get_global_ui_data(void)
{
	return &global_ui_data;
}

void ui_begin(lv_ui* ui)
{
	ui_data_init();
    setup_ui(ui);
    events_init(ui);
}
