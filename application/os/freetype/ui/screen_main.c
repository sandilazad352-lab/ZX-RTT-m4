#include <rtthread.h>
#include <rtconfig.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include "zx_ui_entry.h"

lv_obj_t* screen_main;

int screen_main_build(void)
{
    screen_main = lv_obj_create(NULL);
    if (screen_main) {
        /*Create a font*/
        static lv_ft_info_t info;

        /*FreeType uses C standard file system, so no driver letter is required.*/
        info.name = "/ram/font/font.ttf";
        info.weight = 24;
        info.style = FT_FONT_STYLE_NORMAL;
        info.mem = NULL;

        if(!lv_ft_font_init(&info)) {
            LV_LOG_ERROR("create failed.");
        }

        /*Create style with the new font*/
        static lv_style_t style;
        lv_style_init(&style);
        lv_style_set_text_font(&style, info.font);
        lv_style_set_text_align(&style, LV_TEXT_ALIGN_CENTER);

        /*Create a label with the new style*/
        lv_obj_t * label = lv_label_create(screen_main);
        lv_obj_add_style(label, &style, 0);
        lv_label_set_text(label, "启明智显");
        lv_obj_center(label);
    }

    return -1;
}

void screen_main_delete(void)
{
    if (screen_main) {
        lv_obj_del(screen_main);
        screen_main = NULL;
    }
}

void screen_main_show(void)
{
    if (screen_main) {
        lv_scr_load(screen_main);
    }
}
