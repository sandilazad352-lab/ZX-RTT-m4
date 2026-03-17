#include "zx_ui_entry.h"

void zx_ui_entry(void)
{
    lv_obj_t *img = lv_img_create(lv_scr_act());
    lv_img_set_src(img, LVGL_DIR"logo.jpg");
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
}