#include "zx_ui_entry.h"

extern void my_mpp_dec_test(char *path, int count);
#include "mpp_fb.h"
#include "aic_dec.h"


LV_FONT_DECLARE(font_30);
LV_FONT_DECLARE(font_50);
LV_FONT_DECLARE(font_70);

FAKE_IMAGE_DECLARE(bg_dark);

typedef struct
{
    lv_obj_t * img_bg;
    lv_obj_t * arc_level;
    lv_obj_t * arc_level_label;
    lv_obj_t * arc_level_label2;

    lv_obj_t * km_label;
    lv_obj_t * km_label2;
    lv_obj_t * km_label3;

    lv_obj_t * label_hours;
    lv_obj_t * label_point;
    lv_obj_t * label_minutes;

    lv_obj_t * arc_km;
    lv_obj_t * arc_km_label;
    lv_obj_t * arc_km_label2;

    lv_obj_t *jpg_img;


    lv_timer_t *timer; //定时器

}user_obj_t;

static user_obj_t user_obj = {0};

static void arc_level_anim_cb(void * obj, int32_t v);
static void arc_km_anim_cb(void * obj, int32_t v);
static void km_label_anim_cb(void * obj, int32_t v);
static void test_timer_cb(lv_timer_t *timer);
static void jpg_player(lv_timer_t *t);

void zx_ui_entry(void)
{

    user_obj.img_bg = lv_obj_create(NULL);
    lv_obj_remove_style_all(user_obj.img_bg);
    lv_obj_set_style_bg_color(user_obj.img_bg, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(user_obj.img_bg, LV_OPA_100, 0);
    lv_obj_center(user_obj.img_bg);

    user_obj.jpg_img = lv_img_create(user_obj.img_bg);


    /*Create an Arc*/
    user_obj.arc_level = lv_arc_create(user_obj.img_bg);
    lv_obj_set_size(user_obj.arc_level, 180, 180);
    lv_arc_set_rotation(user_obj.arc_level, 270);
    lv_arc_set_bg_angles(user_obj.arc_level, 180, 360);
    lv_arc_set_range(user_obj.arc_level, 0, 100);
    lv_arc_set_value(user_obj.arc_level, 10);
    lv_obj_remove_style(user_obj.arc_level, NULL, LV_PART_KNOB);   /*Be sure the knob is not displayed*/
    lv_obj_clear_flag(user_obj.arc_level, LV_OBJ_FLAG_CLICKABLE);  /*To not allow adjusting by click*/
    lv_obj_align(user_obj.arc_level, LV_ALIGN_LEFT_MID, 50, 0);

    user_obj.arc_level_label = lv_label_create(user_obj.img_bg);
    lv_obj_set_style_text_color(user_obj.arc_level_label, lv_color_white(), 0);
    lv_obj_set_style_text_font(user_obj.arc_level_label, &font_50, 0);
    lv_obj_align_to(user_obj.arc_level_label, user_obj.arc_level, LV_ALIGN_RIGHT_MID, -70, 0);
    lv_label_set_text_fmt(user_obj.arc_level_label, "%d", lv_arc_get_value(user_obj.arc_level));

    user_obj.arc_level_label2 = lv_label_create(user_obj.img_bg);
    lv_obj_set_style_text_color(user_obj.arc_level_label2, lv_color_white(), 0);
    lv_obj_set_style_text_font(user_obj.arc_level_label2, &font_30, 0);
    lv_label_set_text(user_obj.arc_level_label2, "%");
    lv_obj_align_to(user_obj.arc_level_label2, user_obj.arc_level_label, LV_ALIGN_OUT_RIGHT_TOP, 0, 0);


    user_obj.km_label = lv_label_create(user_obj.img_bg);
    lv_obj_set_style_text_color(user_obj.km_label, lv_color_white(), 0);
    lv_obj_set_style_text_font(user_obj.km_label, &font_70, 0);
    lv_obj_align(user_obj.km_label, LV_ALIGN_CENTER, 0, -20);
    lv_label_set_text_fmt(user_obj.km_label, "%d", 200);

    user_obj.km_label2 = lv_label_create(user_obj.img_bg);
    lv_obj_set_style_text_color(user_obj.km_label2, lv_color_white(), 0);
    lv_obj_set_style_text_font(user_obj.km_label2, &font_30, 0);
    lv_obj_align_to(user_obj.km_label2, user_obj.km_label, LV_ALIGN_OUT_RIGHT_BOTTOM, 10, -15);
    lv_label_set_text(user_obj.km_label2, "m");

    user_obj.km_label3 = lv_label_create(user_obj.img_bg);
    lv_obj_set_style_text_color(user_obj.km_label3, lv_color_make(133, 167, 240), 0);
    lv_obj_set_style_text_font(user_obj.km_label3, &font_30, 0);
    lv_obj_align(user_obj.km_label3,  LV_ALIGN_CENTER, 10, 60);
    lv_label_set_text(user_obj.km_label3, "来广营东路");


    user_obj.label_hours = lv_label_create(lv_layer_top());
    lv_obj_set_style_text_font(user_obj.label_hours, &font_30, 0); //添加样式
    lv_obj_set_style_text_color(user_obj.label_hours, lv_color_white(), 0);
    lv_label_set_text(user_obj.label_hours, "10");
    lv_obj_align(user_obj.label_hours, LV_ALIGN_TOP_MID, -20, 150);

    user_obj.label_point = lv_label_create(lv_layer_top());
    lv_obj_set_style_text_font(user_obj.label_point, &font_30, 0); //添加样式
    lv_obj_set_style_text_color(user_obj.label_point, lv_color_white(), 0);
    lv_label_set_text(user_obj.label_point, ":");
    lv_obj_align_to(user_obj.label_point, user_obj.label_hours, LV_ALIGN_OUT_RIGHT_BOTTOM, 7, 0);

    user_obj.label_minutes = lv_label_create(lv_layer_top());
    lv_obj_set_style_text_font(user_obj.label_minutes, &font_30, 0); //添加样式
    lv_obj_set_style_text_color(user_obj.label_minutes, lv_color_white(), 0);
    lv_label_set_text(user_obj.label_minutes, "12");
    lv_obj_align_to(user_obj.label_minutes, user_obj.label_point, LV_ALIGN_OUT_RIGHT_BOTTOM, 7, 0);

    /*Create an Arc*/
    user_obj.arc_km = lv_arc_create(user_obj.img_bg);
    lv_obj_set_size(user_obj.arc_km, 180, 180);
    lv_arc_set_rotation(user_obj.arc_km, 270);
    lv_arc_set_bg_angles(user_obj.arc_km, 0, 180);
    lv_arc_set_mode(user_obj.arc_km, LV_ARC_MODE_REVERSE);
    lv_arc_set_range(user_obj.arc_km, 0, 100);
    lv_arc_set_value(user_obj.arc_km, 0);
    lv_obj_remove_style(user_obj.arc_km, NULL, LV_PART_KNOB);   /*Be sure the knob is not displayed*/
    lv_obj_clear_flag(user_obj.arc_km, LV_OBJ_FLAG_CLICKABLE);  /*To not allow adjusting by click*/
    lv_obj_align(user_obj.arc_km, LV_ALIGN_RIGHT_MID, -50, 0);

    user_obj.arc_km_label = lv_label_create(user_obj.img_bg);
    lv_obj_set_style_text_color(user_obj.arc_km_label, lv_color_white(), 0);
    lv_obj_set_style_text_font(user_obj.arc_km_label, &font_50, 0);
    lv_obj_align_to(user_obj.arc_km_label, user_obj.arc_km, LV_ALIGN_LEFT_MID, 60, -20);
    lv_label_set_text_fmt(user_obj.arc_km_label, "%d", 100-lv_arc_get_value(user_obj.arc_km));

    user_obj.arc_km_label2 = lv_label_create(user_obj.img_bg);
    lv_obj_set_style_text_color(user_obj.arc_km_label2, lv_color_white(), 0);
    lv_obj_set_style_text_font(user_obj.arc_km_label2, &font_30, 0);
    lv_label_set_text(user_obj.arc_km_label2, "km/h");
    lv_obj_align_to(user_obj.arc_km_label2, user_obj.arc_km_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);


    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, user_obj.arc_level);
    lv_anim_set_exec_cb(&a, arc_level_anim_cb);
    lv_anim_set_time(&a, 2000);
    lv_anim_set_playback_delay(&a, 300);
    lv_anim_set_playback_time(&a, 2000);//回放动画用的时间
    lv_anim_set_repeat_delay(&a, 300);//延迟回放
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);    /*Just for the demo*/
    lv_anim_set_values(&a, 0, 100);
    lv_anim_start(&a);

    lv_anim_t b;
    lv_anim_init(&b);
    lv_anim_set_var(&b, user_obj.arc_km);
    lv_anim_set_exec_cb(&b, arc_km_anim_cb);
    lv_anim_set_time(&b, 1500);
    lv_anim_set_playback_delay(&b, 300);
    lv_anim_set_playback_time(&b, 1500);//回放动画用的时间
    lv_anim_set_repeat_delay(&b, 300);//延迟回放
    lv_anim_set_repeat_count(&b, LV_ANIM_REPEAT_INFINITE);    /*Just for the demo*/
    lv_anim_set_values(&b, 100, 0);
    lv_anim_start(&b);


    lv_anim_t c;
    lv_anim_init(&c);
    lv_anim_set_var(&c, user_obj.km_label);
    lv_anim_set_exec_cb(&c, km_label_anim_cb);
    lv_anim_set_time(&c, 2000);
    lv_anim_set_playback_delay(&c, 300);
    lv_anim_set_playback_time(&c, 2000);//回放动画用的时间
    lv_anim_set_repeat_delay(&c, 300);//延迟回放
    lv_anim_set_repeat_count(&c, LV_ANIM_REPEAT_INFINITE);    /*Just for the demo*/
    lv_anim_set_values(&c, 0, 250);
    lv_anim_start(&c);

    if (user_obj.timer == NULL)
    {
        user_obj.timer = lv_timer_create(test_timer_cb, 500, NULL);
    }


    // my_mpp_dec_test("/rodata/a.264", -1);
    lv_timer_create(jpg_player, 25, NULL);

    lv_scr_load(user_obj.img_bg);
}
#define SRC2_PATH_HEAD "L:/rodata/"
static void jpg_player(lv_timer_t *t)
{
	char _path[64];
	static int i = 1;
	lv_snprintf(_path, sizeof(_path), SRC2_PATH_HEAD"%03d.jpg", i++);
	lv_img_set_src(user_obj.jpg_img, _path);
    lv_obj_align(user_obj.jpg_img, LV_ALIGN_CENTER, 50, 0);
    if(i == 36)
        i = 1;
}

static void test_timer_cb(lv_timer_t *timer)
{
    static bool flag = false;
    if (flag)
    {
        flag = false;
        lv_obj_add_flag(user_obj.label_point, LV_OBJ_FLAG_HIDDEN); // 18:23
    }
    else
    {
        flag = true;
        lv_obj_clear_flag(user_obj.label_point, LV_OBJ_FLAG_HIDDEN); // 18:23
    }
}

static void arc_level_anim_cb(void * obj, int32_t v)
{
    lv_arc_set_value(user_obj.arc_level, v);

    lv_label_set_text_fmt(user_obj.arc_level_label, "%d", lv_arc_get_value(user_obj.arc_level));
    lv_obj_align_to(user_obj.arc_level_label, user_obj.arc_level, LV_ALIGN_RIGHT_MID, -70, 0);

    lv_obj_align_to(user_obj.arc_level_label2, user_obj.arc_level_label, LV_ALIGN_OUT_RIGHT_TOP, 0, 0);
}
static void arc_km_anim_cb(void * obj, int32_t v)
{
    lv_arc_set_value(user_obj.arc_km, v);

    lv_label_set_text_fmt(user_obj.arc_km_label, "%d", lv_arc_get_value(user_obj.arc_km));
    lv_obj_align_to(user_obj.arc_km_label, user_obj.arc_km, LV_ALIGN_LEFT_MID, 60, -20);

    lv_obj_align_to(user_obj.arc_km_label2, user_obj.arc_km_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
}
static void km_label_anim_cb(void * obj, int32_t v)
{
    lv_label_set_text_fmt(user_obj.km_label, "%d", v);
    lv_obj_align(user_obj.km_label, LV_ALIGN_CENTER, 0, -20);

    lv_obj_align_to(user_obj.km_label2, user_obj.km_label, LV_ALIGN_OUT_RIGHT_BOTTOM, 10, -15);
}

    
    
    