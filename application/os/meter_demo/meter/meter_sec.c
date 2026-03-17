#include <stdio.h>
#include <stdlib.h>
#include "meter_ui.h"

static lv_obj_t *arc_left;
static lv_obj_t *arc_right;
static lv_obj_t *label1, *label3, *label8;
static lv_obj_t *img08, *img09, *img12;
static lv_obj_t *img13, *img14, *img15, *img16, *img17, *img18, *img21, *img22, *img24;

static void arc_left_anim(lv_timer_t *tmr)
{
    static int step_list[] = {30, 20, 100, 80, 160};
    static int t1 = 0, step = 0;
    (void)tmr;

    lv_arc_set_value(arc_left, t1);
    lv_label_set_text_fmt(label1, "%d", t1);
    lv_obj_align_to(label1, img08, LV_ALIGN_CENTER, 0, 0);

    if(t1 > step_list[step])
        t1 = t1 - 2;
    else if(t1 < step_list[step])
        t1 = t1 + 2;
    else
        step = step < sizeof(step_list) / sizeof(int) - 1 ? step + 1 : 0;
}

static void arc_right_anim(lv_timer_t *tmr)
{
    static int step_list[] = {16, 40, 32, 80, 40};
    static int t1 = 0, step = 0;
    char str[128];
    (void)tmr;

    lv_arc_set_value(arc_right, t1);
    // snprintf(str, sizeof(str), "%0.1f", t1 / 10.0);
    snprintf(str, sizeof(str), "%d", t1);
    lv_label_set_text(label3, str);
    lv_obj_align_to(label3, img09, LV_ALIGN_CENTER, 0, 0);

    if(t1 > step_list[step])
        t1 = t1 - 1;
    else if(t1 < step_list[step])
        t1 = t1 + 1;
    else
        step = step < sizeof(step_list) / sizeof(int) - 1 ? step + 1 : 0;
}

static void navigation_anim(lv_timer_t *tmr)
{

    static int step_list[] = {324, 290};
    static int t1 = 324, step = 0;
    (void)tmr;

    lv_obj_set_y(img12, t1);
    lv_label_set_text_fmt(label8, "%dM", t1);

    if(t1 > step_list[step])
        t1 = t1 - 2;
    else if(t1 < step_list[step])
        t1 = t1 + 1;
    else
        step = step < sizeof(step_list) / sizeof(int) - 1 ? step + 1 : 0;
}


static void flash_anim(lv_timer_t *tmr)
{
    static int step_list[] = {1, 2, 4, 3};
    static int t1 = 0, step = 0;
    (void)tmr;
    
    if(t1 > step_list[step])
        t1 = t1 - 1;
    else if(t1 < step_list[step])
        t1 = t1 + 1;
    else
    {
        step = step < sizeof(step_list) / sizeof(int) - 1 ? step + 1 : 0;
        switch (step)
        {
        case 0:
            lv_img_set_src(img21, LVGL_PATH2(icon11.png));
            lv_img_set_src(img22, LVGL_PATH2(icon18.png));
            lv_img_set_src(img13, LVGL_PATH2(icon24.png));
            lv_img_set_src(img15, LVGL_PATH2(icon31.png));
            lv_img_set_src(img16, LVGL_PATH2(icon28.png)); 
            lv_img_set_src(img17, LVGL_PATH2(icon29.png));
            break;
        case 1:
            lv_img_set_src(img21, LVGL_PATH2(icon12.png));
            lv_img_set_src(img22, LVGL_PATH2(icon17.png));
            lv_img_set_src(img13, LVGL_PATH2(icon23.png));
            lv_img_set_src(img15, LVGL_PATH2(icon32.png));
            lv_img_set_src(img18, LVGL_PATH2(icon20.png));
            break;
        case 2:
            lv_img_set_src(img21, LVGL_PATH2(icon13.png));
            lv_img_set_src(img22, LVGL_PATH2(icon16.png));
            lv_img_set_src(img14, LVGL_PATH2(icon26.png));
            break;
        case 3:
            lv_img_set_src(img21, LVGL_PATH2(icon14.png));
            lv_img_set_src(img22, LVGL_PATH2(icon15.png));
            lv_img_set_src(img14, LVGL_PATH2(icon25.png));
            lv_img_set_src(img16, LVGL_PATH2(icon27.png)); 
            lv_img_set_src(img17, LVGL_PATH2(icon30.png));
            lv_img_set_src(img18, LVGL_PATH2(icon19.png));
            break;
        
        default:
            break;
        }
    }
}

static void meter_sec_anim_start(void)
{
    lv_obj_clear_flag(arc_left, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(arc_right, LV_OBJ_FLAG_CLICKABLE);

    lv_arc_set_value(arc_left, 0);
    lv_arc_set_value(arc_right, 0);
    lv_label_set_text(label1, "0");
    lv_label_set_text(label3, "0");

    lv_timer_create(arc_left_anim, 30, NULL);
    lv_timer_create(arc_right_anim, 30, NULL);
    lv_timer_create(navigation_anim, 60, NULL);
    lv_timer_create(flash_anim, 500, NULL);
}

LV_IMG_DECLARE(icon05);
LV_IMG_DECLARE(icon06);

void meter_sec_ui_init(lv_obj_t *par)
{
    extern void _lv_hw_png_src_add(lv_ll_t *_lv_sw_img_ll, char *src);
    _lv_hw_png_src_add(&_lv_sw_img_ll, LVGL_PATH2(icon05.png));
    _lv_hw_png_src_add(&_lv_sw_img_ll, LVGL_PATH2(icon06.png));
    lv_img_dsc_t *_icon05 = _lv_get_sw_png_src_index(&_lv_sw_img_ll, 0);
    lv_img_dsc_t *_icon06 = _lv_get_sw_png_src_index(&_lv_sw_img_ll, 1);

    lv_obj_t *bg01 = lv_obj_create(par);
    lv_obj_set_style_bg_color(bg01, lv_color_black(), 0);
    lv_obj_set_style_border_width(bg01, 0, 0);
    lv_obj_set_style_radius(bg01, 0, 0);
    lv_obj_set_size(bg01, 800, 480);
    lv_obj_set_style_pad_all(bg01, 0, 0);
    lv_obj_align(bg01, LV_ALIGN_CENTER, 0, 0);
    lv_obj_clear_flag(bg01, LV_OBJ_FLAG_SCROLLABLE);

    // lv_obj_t *img01 = lv_img_create(bg01);
    // lv_img_set_src(img01, LVGL_PATH2(icon39.png));
    // lv_obj_align(img01, LV_ALIGN_CENTER, -10, -65);
 
    lv_obj_t *img02 = lv_img_create(bg01);
    lv_img_set_src(img02, LVGL_PATH2(icon40.png));
    lv_obj_set_pos(img02, 203, 126);

    lv_obj_t *img03 = lv_img_create(bg01);
    lv_img_set_src(img03, LVGL_PATH2(icon42.png));
    lv_obj_set_pos(img03, 258, 130);

    // lv_obj_t *img04 = lv_img_create(bg01);
    // lv_img_set_src(img04, LVGL_PATH2(icon41.png));
    // lv_obj_set_pos(img04, 0, 375);

    // lv_obj_t *img05 = lv_img_create(bg01);
    // lv_img_set_src(img05, LVGL_PATH2(icon43.png));
    // lv_obj_align(img05, LV_ALIGN_TOP_MID, 0, -5);

    lv_obj_t *img06 = lv_img_create(bg01);
    lv_img_set_src(img06, LVGL_PATH2(icon01.png));
    lv_obj_align(img06, LV_ALIGN_TOP_LEFT, 17, 113);

    lv_obj_t *img07 = lv_img_create(bg01);
    lv_img_set_src(img07, LVGL_PATH2(icon02.png));
    lv_obj_align(img07, LV_ALIGN_TOP_RIGHT, -17, 113);
    
    arc_left = lv_arc_create(bg01);
    lv_obj_set_size(arc_left, 290, 290);
    lv_arc_set_rotation(arc_left, 90 + (360 - 300) / 2);
    lv_arc_set_bg_angles(arc_left, 0, 300);
    lv_arc_set_range(arc_left, 0, 160);
    lv_arc_set_value(arc_left, 80);
	lv_obj_set_style_bg_opa(arc_left, 0, LV_PART_KNOB);
	lv_obj_set_style_arc_width(arc_left, 90, LV_PART_MAIN);
	lv_obj_set_style_arc_width(arc_left, 90, LV_PART_INDICATOR);
	lv_obj_set_style_arc_rounded(arc_left, 0, LV_PART_MAIN);
	lv_obj_set_style_arc_rounded(arc_left, 0, LV_PART_INDICATOR);
	lv_obj_set_style_arc_opa(arc_left, 0, LV_PART_MAIN);
	// lv_obj_set_style_arc_img_src(arc_left, LVGL_PATH2(icon06.png), LV_PART_INDICATOR);
	lv_obj_set_style_arc_img_src(arc_left, _icon06, LV_PART_INDICATOR);
	lv_obj_set_style_arc_opa(arc_left, 255, LV_PART_INDICATOR);
 
    arc_right = lv_arc_create(bg01);
    lv_obj_set_size(arc_right, 290, 290);
    lv_arc_set_rotation(arc_right, 90 + (360 - 300) / 2);
    lv_arc_set_bg_angles(arc_right, 0, 300);
    lv_arc_set_range(arc_right, 0, 80);
    lv_arc_set_value(arc_right, 40);
	lv_obj_set_style_bg_opa(arc_right, 0, LV_PART_KNOB);
	lv_obj_set_style_arc_width(arc_right, 90, LV_PART_MAIN);
	lv_obj_set_style_arc_width(arc_right, 90, LV_PART_INDICATOR);
	lv_obj_set_style_arc_rounded(arc_right, 0, LV_PART_MAIN);
	lv_obj_set_style_arc_rounded(arc_right, 0, LV_PART_INDICATOR);
	lv_obj_set_style_arc_opa(arc_right, 0, LV_PART_MAIN);
	// lv_obj_set_style_arc_img_src(arc_right, LVGL_PATH2(icon05.png), LV_PART_INDICATOR);
	lv_obj_set_style_arc_img_src(arc_right, _icon05, LV_PART_INDICATOR);
	lv_obj_set_style_arc_opa(arc_right, 255, LV_PART_INDICATOR);


    img08 = lv_img_create(bg01);
    lv_img_set_src(img08, LVGL_PATH2(icon04.png));
    lv_obj_align_to(img08, img06, LV_ALIGN_TOP_LEFT, 39, 23);

    img09 = lv_img_create(bg01);
    lv_img_set_src(img09, LVGL_PATH2(icon03.png));
    lv_obj_align_to(img09, img07, LV_ALIGN_TOP_LEFT, 29, 24);

    lv_obj_align_to(arc_left, img08, LV_ALIGN_CENTER, 0, -10);
    lv_obj_align_to(arc_right, img09, LV_ALIGN_CENTER, 0, -10);

    lv_obj_t *img10 = lv_img_create(bg01);
    lv_img_set_src(img10, LVGL_PATH2(icon44.png));
    lv_obj_align(img10, LV_ALIGN_TOP_MID, 0, 18);
    
    lv_obj_t *img11 = lv_img_create(bg01);
    lv_img_set_src(img11, LVGL_PATH2(icon07.png));
    lv_obj_align_to(img11, img10, LV_ALIGN_CENTER, 0, 0);
    
    img12 = lv_img_create(bg01);
    lv_img_set_src(img12, LVGL_PATH2(icon46.png));
    lv_obj_set_pos(img12, 381, 324);
    
    img13 = lv_img_create(bg01);
    lv_img_set_src(img13, LVGL_PATH2(icon23.png));
    lv_obj_align_to(img13, img10, LV_ALIGN_LEFT_MID, -60, 0);

    img14 = lv_img_create(bg01);
    lv_img_set_src(img14, LVGL_PATH2(icon25.png));
    lv_obj_align_to(img14, img10, LV_ALIGN_RIGHT_MID, 60, 0);

    img15 = lv_img_create(bg01);
    lv_img_set_src(img15, LVGL_PATH2(icon31.png));
    lv_obj_set_pos(img15, 131, 42);
   
    img16 = lv_img_create(bg01);
    lv_img_set_src(img16, LVGL_PATH2(icon27.png));
    lv_obj_align_to(img16, img15, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
 
    img17 = lv_img_create(bg01);
    lv_img_set_src(img17, LVGL_PATH2(icon29.png));
    lv_obj_align_to(img17, img15, LV_ALIGN_OUT_RIGHT_MID, 435, 0);
    
    img18 = lv_img_create(bg01);
    lv_img_set_src(img18, LVGL_PATH2(icon20.png));
    lv_obj_align_to(img18, img17, LV_ALIGN_OUT_RIGHT_MID, 20, 0);

    lv_obj_t *img19 = lv_img_create(bg01);
    lv_img_set_src(img19, LVGL_PATH2(icon22.png));
    lv_obj_set_pos(img19, 39, 350);

    lv_obj_t *img20 = lv_img_create(bg01);
    lv_img_set_src(img20, LVGL_PATH2(icon21.png));
    lv_obj_set_pos(img20, 741, 352);

    img21 = lv_img_create(bg01);
    lv_img_set_src(img21, LVGL_PATH2(icon11.png));
    lv_obj_set_pos(img21, 53, 365);

    img22 = lv_img_create(bg01);
    lv_img_set_src(img22, LVGL_PATH2(icon15.png));
    lv_obj_set_pos(img22, 642, 365);

    lv_obj_t *img23 = lv_img_create(bg01);
    lv_img_set_src(img23, LVGL_PATH2(icon45.png));
    lv_obj_set_pos(img23, 385, 440);

    img24 = lv_img_create(bg01);
    lv_img_set_src(img24, LVGL_PATH2(icon36.png));
    lv_obj_set_pos(img24, 342, 155);

    LV_FONT_DECLARE(cn_r32x);
    LV_FONT_DECLARE(cn_r18x);
    LV_FONT_DECLARE(probert_b15x);
    LV_FONT_DECLARE(probert_b18x);
    LV_FONT_DECLARE(probert_b40x);


    label1 = lv_label_create(bg01);
    lv_obj_set_style_text_font(label1, &probert_b40x, 0);
    lv_obj_set_style_text_color(label1, lv_color_white(), 0);
    lv_obj_set_style_text_letter_space(label1, 2, 0);
    lv_label_set_text(label1, "86");
    lv_obj_align_to(label1, img08, LV_ALIGN_CENTER, 0, 0);
    
    lv_obj_t *label2 = lv_label_create(bg01);
    lv_obj_set_style_text_font(label2, &probert_b15x, 0);
    lv_obj_set_style_text_color(label2, lv_color_make(133, 134, 136), 0);
    lv_label_set_text(label2, "KM/H");
    lv_obj_align_to(label2, label1, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);

    label3 = lv_label_create(bg01);
    lv_obj_set_style_text_font(label3, &probert_b40x, 0);
    lv_obj_set_style_text_color(label3, lv_color_white(), 0);
    lv_obj_set_style_text_letter_space(label3, 2, 0);
    lv_label_set_text(label3, "1.2");
    lv_obj_align_to(label3, img09, LV_ALIGN_CENTER, 0, 0);
    
    lv_obj_t *label4 = lv_label_create(bg01);
    lv_obj_set_style_text_font(label4, &probert_b15x, 0);
    lv_obj_set_style_text_color(label4, lv_color_make(133, 134, 136), 0);
    lv_label_set_text(label4, "1minx1000");
    lv_obj_align_to(label4, label3, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);

    lv_obj_t *label5 = lv_label_create(bg01);
    lv_obj_set_style_text_font(label5, &probert_b40x, 0);
    lv_obj_set_style_text_color(label5, lv_color_white(), 0);
    lv_obj_set_style_text_letter_space(label5, 2, 0);
    lv_label_set_text(label5, "15:20");
    lv_obj_align(label5, LV_ALIGN_BOTTOM_MID, 0, -40);
    
    lv_obj_t *label6 = lv_label_create(bg01);
    lv_obj_set_style_text_font(label6, &probert_b18x, 0);
    lv_obj_set_style_text_color(label6, lv_color_make(133, 134, 136), 0);
    lv_obj_set_style_text_letter_space(label6, 2, 0);
    lv_label_set_text(label6, "10.31");
    lv_obj_align_to(label6, img23, LV_ALIGN_OUT_LEFT_MID, -10, 0);

    lv_obj_t *label7 = lv_label_create(bg01);
    lv_obj_set_style_text_font(label7, &probert_b18x, 0);
    lv_obj_set_style_text_color(label7, lv_color_make(133, 134, 136), 0);
    lv_obj_set_style_text_letter_space(label7, 2, 0);
    lv_label_set_text(label7, "20*");
    lv_obj_align_to(label7, img23, LV_ALIGN_OUT_RIGHT_MID, 10, 0);

    label8 = lv_label_create(bg01);
    lv_obj_set_style_text_font(label8, &cn_r32x, 0);
    lv_obj_set_style_text_color(label8, lv_color_white(), 0);
    lv_label_set_text(label8, "138M");
    lv_obj_set_pos(label8, 385, 141);

    lv_obj_t *label9 = lv_label_create(bg01);
    lv_obj_set_style_text_font(label9, &cn_r18x, 0);
    lv_obj_set_style_text_color(label9, lv_color_make(150, 160, 188), 0);
    lv_label_set_text(label9, "南坪快速");
    lv_obj_align_to(label9, label8, LV_ALIGN_OUT_BOTTOM_MID, 0, -5);

    meter_sec_anim_start();
}
