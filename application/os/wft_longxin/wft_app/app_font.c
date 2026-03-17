#include "app_font.h"

#define FONT_NAME "/rodata/fonts/heitijian.ttf"
S_APP_FONT app_font={

    
    .f_init = p_font_init
};
void p_font_init(void)
{
	app_font.info.name = FONT_NAME;
	app_font.info.weight = 32;
	app_font.info.style = FT_FONT_STYLE_NORMAL;
	app_font.info.mem = NULL;
	rt_kprintf("font_init() :FONT_NAME[%s]\r\n" , FONT_NAME);
	if (!lv_ft_font_init(&app_font.info))
	{
		LV_LOG_ERROR("create failed.");
	}
	/*Create style with the new font*/
	lv_style_init(&app_font.style);
	lv_style_set_text_color(&app_font.style,  lv_color_make(0, 0, 255) );
	
	lv_style_set_text_font(&app_font.style, app_font.info.font);
	lv_style_set_text_align(&app_font.style, LV_TEXT_ALIGN_CENTER);

	app_font.info.name = FONT_NAME;
	app_font.info.weight = 20;
	app_font.info.style = FT_FONT_STYLE_NORMAL;
	app_font.info.mem = NULL;
	if (!lv_ft_font_init(&app_font.info))
	{
		LV_LOG_ERROR("create failed.");
	}
	/*Create style with the new font*/
	lv_style_init(&app_font.style_font_16);
	lv_style_set_text_color(&app_font.style_font_16,  lv_color_make(255, 255, 255) );
	lv_style_set_text_font(&app_font.style_font_16, app_font.info.font);
	lv_style_set_text_align(&app_font.style_font_16, LV_TEXT_ALIGN_CENTER);
}
