#ifndef ZX_UI_ENTRY_H
#define ZX_UI_ENTRY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"
#include "zx_gui.h"

LV_FONT_DECLARE(ali_font_16);

extern lv_obj_t *screen_main;
extern lv_obj_t *screen_main_label;

int screen_main_build(void);
void screen_main_delete(void);
void screen_main_show(void);

void zx_ui_entry(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*ZX_UI_ENTRY_H*/
