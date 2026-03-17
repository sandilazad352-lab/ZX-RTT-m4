#include "zx_ui_entry.h"
#include "app_interface.h"
static lv_obj_t *jpg_img;
#define PATH_HEAD "L:/rodata/logo/"



void jpg_player(lv_timer_t *t)
{
    char _path[64];
    static int i = 2;
    lv_img_cache_invalidate_src(NULL);
    lv_snprintf(_path, sizeof(_path), PATH_HEAD "zb_%03d.jpg", i);
    // rt_kprintf("_path : %s" , _path); 
    lv_img_set_src(jpg_img, _path);
    lv_obj_center(jpg_img);

    if (++i == 82)
    {
        lv_img_cache_set_size(1);
        lv_img_cache_invalidate_src(NULL);
        lv_obj_clean(lv_scr_act());
        app_interface.font->f_init();
        ui_init();
        app_interface.f_init();
        lv_timer_del(t);
    }
}

void zx_ui_entry(void)
{
    lv_img_cache_set_size(1);
    jpg_img = lv_img_create(lv_scr_act());
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), 0);
    lv_img_set_src(jpg_img, PATH_HEAD "zb_080.jpg");
    lv_obj_center(jpg_img);

	// label1 = lv_label_create(lv_scr_act());
	// lv_label_set_text(label1, "1");
	// lv_obj_set_style_text_font(label1, &lv_font_montserrat_40, 0);
	// lv_obj_set_style_text_color(label1, lv_color_make(0, 0, 255), 0);
	// lv_obj_center(label1);

    lv_timer_create(jpg_player, 30, NULL);
}

// void zx_ui_entry(void)
// {
//     app_interface.font->f_init();
//     ui_init();

//     app_interface.f_init();
// }
