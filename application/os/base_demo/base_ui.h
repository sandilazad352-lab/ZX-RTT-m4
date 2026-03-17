
#ifndef BASE_UI_H
#define BASE_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

#include <stdio.h>
#include <stdlib.h>

#define LVGL_DIR "L:/rodata/"
#define FILE_LIST_PATH "/data/"

#define CONN(x, y) x#y
#define LVGL_PATH(y) CONN(LVGL_DIR, y)
#define LVGL_FILE_LIST_PATH(y) CONN(FILE_LIST_PATH, y)

#define ui_snprintf(fmt, arg...) snprintf(fmt, 127, ##arg)

void base_ui_init();
void zx_ui_entry(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif //BASE_UI_H
