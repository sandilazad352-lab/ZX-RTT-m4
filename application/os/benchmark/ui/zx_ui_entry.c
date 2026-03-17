#include "zx_ui_entry.h"

extern void lv_demo_benchmark(void);

#ifdef ZX_LVGL_MJPEG_DECODE_DEMO

static lv_obj_t *jpg_img;
static lv_obj_t *label1;
#define SRC2_PATH_HEAD "L:/rodata/lvgl_data/"

void jpg_player(lv_timer_t *t)
{
	char _path[64];
	static int i = 0;
	lv_snprintf(_path, sizeof(_path), SRC2_PATH_HEAD"a_%03d.jpg", i);
	lv_img_set_src(jpg_img, _path);
	lv_label_set_text_fmt(label1, "%d", i);

	if(++i == 210)
	{
		lv_obj_clean(lv_scr_act());
		lv_demo_benchmark();
		lv_timer_del(t);
	}
}

void zx_ui_entry(void)
{
	jpg_img = lv_img_create(lv_scr_act());
	lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), 0);
	lv_img_set_src(jpg_img, SRC2_PATH_HEAD"a_001.jpg");
	lv_obj_center(jpg_img);

	label1 = lv_label_create(lv_scr_act());
	lv_label_set_text(label1, "1");
	lv_obj_set_style_text_font(label1, &lv_font_montserrat_40, 0);
	lv_obj_set_style_text_color(label1, lv_color_make(0, 0, 255), 0);
	lv_obj_center(label1);
	
	lv_timer_create(jpg_player, 30, NULL);
}

#else

void zx_ui_entry(void)
{
	lv_demo_benchmark();
}

#endif