#include "zx_ui_entry.h"

void zx_ui_entry(void)
{
    lv_obj_t *label1 = lv_label_create(lv_scr_act());
    lv_label_set_text(label1, "left: benchmark | right: encode");
    lv_obj_set_style_text_color(label1, lv_color_black(), NULL);
    lv_obj_set_style_text_font(label1, &lv_font_montserrat_20, NULL);
    lv_obj_align(label1, LV_ALIGN_CENTER, 0, 0);
}