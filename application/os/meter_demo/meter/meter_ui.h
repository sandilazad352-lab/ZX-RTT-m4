#ifndef METER_UI_H
#define METER_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

#if 1 // tmp
    #define UI_SNPRINTF_LEN 256
    #define LVGL_DIR1 "L:/rodata/meter_firs/"
    #define LVGL_DIR2 "L:/rodata/meter_sec/"
    #define LVGL_DIR3 "L:/rodata/meter_thir/"
    #define CONN(x, y) x#y
    #define LVGL_PATH1(y) CONN(LVGL_DIR1, y)
    #define LVGL_PATH2(y) CONN(LVGL_DIR2, y)
    #define LVGL_PATH3(y) CONN(LVGL_DIR3, y)

// bg_red.png - bg_red.jpg
#endif

// 待完善CPU + FPS
void meter_sec_ui_init(lv_obj_t *par);
void meter_ui_init(void);

typedef struct _lv_ll_img_dsc_t {
    int index;
    lv_img_dsc_t img_dsc;
} lv_ll_img_dsc_t;

void _lv_sw_png_init(lv_ll_t *_lv_sw_img_ll);
void _lv_sw_png_src_add(lv_ll_t *_lv_sw_img_ll, char *src);
void _lv_sw_png_src_free(lv_ll_t *_lv_sw_img_ll);
lv_img_dsc_t *_lv_get_sw_png_src_index(lv_ll_t *_lv_sw_img_ll, int index);

extern lv_ll_t _lv_sw_img_ll;


#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif //METER_UI_H
