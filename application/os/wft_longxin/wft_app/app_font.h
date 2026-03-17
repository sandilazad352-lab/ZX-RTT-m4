
#ifndef __APP_FONT_H_
#define __APP_FONT_H_
#include "lvgl.h"

#include <rtthread.h>
/////////////////////////////////////////
typedef struct
{
    lv_ft_info_t info;
    lv_style_t style;

    lv_style_t style_font_16;

    void (*f_init)(void);
} S_APP_FONT;
extern S_APP_FONT app_font;
void p_font_init(void);
#endif