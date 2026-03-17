#include "zx_ui_entry.h"
#include "gui_guider.h"
#include "events_init.h"

lv_ui bee_ui;
static lv_obj_t *img1;

static void jpg_player(lv_timer_t *t)
{
	char _path[64];
	static int i = 1;
	lv_snprintf(_path, sizeof(_path), "L:/rodata/logo/%d.jpg", i++);
	lv_img_set_src(img1, _path);
    if(++i > 119)
    {
        lv_img_cache_set_size(LV_IMG_CACHE_DEF_SIZE);
        lv_img_cache_invalidate_src(NULL);
        extern void ui_begin(lv_ui* ui);
        ui_begin(&bee_ui);
        lv_timer_del(t);
    }
}


void zx_ui_entry(void)
{
    img1 = lv_img_create(lv_scr_act());
    lv_obj_align(img1, LV_ALIGN_CENTER, 0, 0);
    lv_img_cache_set_size(1);
    lv_timer_t *t1 = lv_timer_create(jpg_player, 25, NULL);
    lv_timer_ready(t1);
}