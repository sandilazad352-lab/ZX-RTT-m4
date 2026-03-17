#include "zx_ui_entry.h"

LV_FONT_DECLARE(font_BOLD_30);
LV_FONT_DECLARE(font_BOLD_95);
LV_FONT_DECLARE(font_12);
LV_FONT_DECLARE(font_15);
LV_FONT_DECLARE(font_18);
LV_FONT_DECLARE(font_20);
LV_FONT_DECLARE(font_40);

static void btn_curtain_suspend_cb(lv_event_t *e);
static void btn_curtain_open_cb(lv_event_t *e);
static void btn_curtain_close_cb(lv_event_t *e);

static void btn_air_condition_left_cb(lv_event_t *e);
static void btn_air_condition_right_cb(lv_event_t *e);

static void btn_air_condition_cold_cb(lv_event_t *e);
static void btn_air_condition_hot_cb(lv_event_t *e);
static void btn_air_condition_wet_cb(lv_event_t *e);
static void disp_check_tiemr_cb(struct _lv_timer_t * timer);

static void test_tv_cb(lv_event_t *e);
static lv_timer_t *disp_check_tiemr = NULL;

static lv_obj_t *top_black = NULL;

typedef struct test_demo
{
    lv_obj_t *img_top_src1;
    lv_obj_t *img_top_src2;

    lv_obj_t *img_background_first;
    lv_obj_t *test_tv;
    lv_timer_t *timer_first; //定时器

    lv_obj_t *page_first;
    lv_obj_t *label_time_hours_first; //时间     xx:xx
    lv_obj_t *label_time_minutes_first;
    lv_obj_t *label_time_point_first;
    lv_obj_t *label_date_first;  //日期     xx月xx日
    lv_obj_t *label_week_first;  //星期     星期x
    lv_obj_t *img_weather_first; //天气
    lv_obj_t *label_tp_first;    //温度

    lv_obj_t *back_first_2;
    lv_obj_t *img_ht_first;
    lv_obj_t *label_ht_1;
    lv_obj_t *label_ht_2;
    lv_obj_t *img_air_first;
    lv_obj_t *label_air_first_1;
    lv_obj_t *label_air_first_2;

    lv_obj_t *btn_clean;                   //清洁
    lv_obj_t *btn_not_disturb_first;       //勿扰
    lv_obj_t *btn_more_first;              //更多
    lv_obj_t *img_clean_first;             //清洁
    lv_obj_t *img_not_disturb_first;       //勿扰
    lv_obj_t *img_more_first;              //更多
    lv_obj_t *small_img_clean_first;       //清洁
    lv_obj_t *small_img_not_disturb_first; //勿扰
    lv_obj_t *small_img_more_first;        //更多
    lv_obj_t *label_clean_firsy;           //清洁
    lv_obj_t *label_not_disturb_first;     //勿扰
    lv_obj_t *label_more_first;            //更多

    //第二页面
    lv_obj_t *page_second;

    lv_obj_t *btn2_clean_second;             //清洁
    lv_obj_t *btn2_not_disturb_second;       //勿扰
    lv_obj_t *img2_clean_second;             //清洁
    lv_obj_t *img2_not_disturb_second;       //勿扰
    lv_obj_t *small2_img_clean_second;       //清洁
    lv_obj_t *small2_img_not_disturb_second; //勿扰
    lv_obj_t *label2_clean_second;           //清洁
    lv_obj_t *label2_not_disturb_second;     //勿扰

    lv_obj_t *img_tubelamp_second;     //筒灯背景
    lv_obj_t *obj_tubelamp_second;     //筒灯文字背景2
    lv_obj_t *label_tubelamp_second;   //筒灯文字
    lv_obj_t *img_tubelamp2_second;    //筒灯图像
    lv_obj_t *switch_tubelamp2_second; //筒灯开关

    lv_obj_t *img_corridorlamp_second;     //廊灯背景
    lv_obj_t *obj_corridorlamp_second;     //廊灯文字背景2
    lv_obj_t *label_corridorlamp_second;   //廊灯文字
    lv_obj_t *img_corridorlamp2_second;    //廊灯图像
    lv_obj_t *switch_corridorlamp2_second; //廊灯开关

    lv_obj_t *img_curtain_second;           //窗帘背景
    lv_obj_t *bar_curtain_second;           //窗帘背景
    lv_obj_t *label_curtain_second;         //窗帘文字
    lv_obj_t *label2_curtain_second;        //窗帘文字提示
    lv_obj_t *img_curtain2_second;          //窗帘图像 小图标
    lv_obj_t *btn_curtain_suspend_second;   //窗帘暂停
    lv_obj_t *label_curtain_suspend_second; //窗帘暂停
    lv_obj_t *btn_curtain_open_second;      //窗帘打开
    lv_obj_t *img_curtain_open_second;      //窗帘打开
    lv_obj_t *btn_curtain_close_second;     //窗帘关闭
    lv_obj_t *img_curtain_close_second;     //窗帘打开
    lv_timer_t *timer_curtain_second;       //定时器

    lv_obj_t *img_air_condition_second;       //空调背景
    lv_obj_t *img_air_condition2_second;      //空调图像 小图标
    lv_obj_t *label_air_condition_second;     //空调
    lv_obj_t *label_air_condition_tp_second;  //空调温度
    lv_obj_t *btn_air_condition_left_second;  //空调减小
    lv_obj_t *btn_air_condition_right_second; //空调增大
    lv_obj_t *img_air_condition_left_second;  //空调减小
    lv_obj_t *img_air_condition_right_second; //空调增大
    lv_obj_t *btn_air_condition_cold_second;  //空调制冷
    lv_obj_t *img_air_condition_cold_second;  //空调制冷
    lv_obj_t *btn_air_condition_hot_second;   //空调制热
    lv_obj_t *img_air_condition_hot_second;   //空调制热
    lv_obj_t *btn_air_condition_wet_second;   //空调除湿
    lv_obj_t *img_air_condition_wet_second;   //空调除湿

} test_demo_obj;

static test_demo_obj test_obj = {0};

void page_second(void);
void page_first(void);

static void test_timer_cb(lv_timer_t *timer_first)
{
    static bool flag = false;
    if (flag)
    {
        flag = false;
        lv_obj_add_flag(test_obj.label_time_point_first, LV_OBJ_FLAG_HIDDEN); // 18:23
    }
    else
    {
        flag = true;
        lv_obj_clear_flag(test_obj.label_time_point_first, LV_OBJ_FLAG_HIDDEN); // 18:23
    }
}

static volatile int g_flag_curtain = 0;   //-1:打开   0:暂停    1:关闭
static int g_value_curtain = 50; //进度
static bool label_curtain_flag = false;

static void test_timer_curtain_cb(lv_timer_t *timer)
{
    switch (g_flag_curtain)
    {
    case -1:
        if (g_value_curtain >= 0 && g_value_curtain < 100)
        {
            if(label_curtain_flag == false)
                label_curtain_flag = true;

            lv_label_set_text(test_obj.label_curtain_second, "正在打开窗帘");
            lv_bar_set_value(test_obj.bar_curtain_second, ++g_value_curtain, LV_ANIM_ON);
        }
        else
        {
            lv_timer_pause(test_obj.timer_curtain_second); //暂停定时器
            lv_label_set_text(test_obj.label_curtain_second, "");
            label_curtain_flag = false;
        }
        break;
    case 0:
        break;
    case 1:
        if (g_value_curtain <= 100 && g_value_curtain > 0)
        {
            if(label_curtain_flag == false)
                label_curtain_flag = true;
            
            lv_label_set_text(test_obj.label_curtain_second, "正在关闭窗帘");
            lv_bar_set_value(test_obj.bar_curtain_second, --g_value_curtain, LV_ANIM_ON);
        }
        else
        {
            lv_timer_pause(test_obj.timer_curtain_second); //暂停定时器
            lv_label_set_text(test_obj.label_curtain_second, "");
            label_curtain_flag = false;
        }
        break;
    default:
        break;
    }
}

static lv_timer_t * timer_jpg = NULL;
static lv_obj_t* img_player = NULL;
#define SRC2_PATH_HEAD "L:/rodata/"
static void jpg_player(lv_timer_t *t)
{
    if(img_player)
    {
        char _path[64];
        static int i = 0;
        lv_snprintf(_path, sizeof(_path), SRC2_PATH_HEAD"fresh_air_jpg/%03d.jpg", i++);
        lv_img_set_src(img_player, _path);
        lv_obj_align(img_player, LV_ALIGN_CENTER, 0, 0);
        if(i == 61)
            i = 0;
    }
}


static void btn_clean_first_cb(lv_event_t *e) //清洁
{
  
}

static void btn_not_disturb_first_cb(lv_event_t *e) //勿扰
{
    lv_obj_t *new_scr;
    lv_obj_t *old_scr;
    old_scr = lv_scr_act();
    new_scr = lv_obj_create(NULL);
    lv_timer_del(test_obj.timer_first);
    lv_timer_del(test_obj.timer_curtain_second);
    lv_timer_del(disp_check_tiemr);
    lv_scr_load(new_scr);
    lv_obj_del_async(old_scr);
    lv_obj_clean(lv_layer_top());
    lv_obj_set_style_bg_color(new_scr, lv_color_black(), 0);
    img_player = lv_img_create(new_scr);
    lv_img_cache_set_size(3);
    timer_jpg = lv_timer_create(jpg_player, 25, NULL);
    
    rt_kprintf("test\n");
}

static void btn_more_first_cb(lv_event_t *e) //更多
{
    extern void lv_demo_benchmark(void);
    lv_obj_t *new_scr;
    lv_obj_t *old_scr;
    old_scr = lv_scr_act();
    new_scr = lv_obj_create(NULL);
    lv_timer_del(test_obj.timer_first);
    lv_timer_del(test_obj.timer_curtain_second);
    lv_timer_del(disp_check_tiemr);
    lv_scr_load(new_scr);
    lv_obj_del_async(old_scr);
    lv_obj_clean(lv_layer_top());
    lv_demo_benchmark();
    rt_kprintf("test\n");
}
static void btn_curtain_suspend_cb(lv_event_t *e) //暂停
{
    label_curtain_flag = false;
    lv_label_set_text(test_obj.label_curtain_second, "");
    lv_timer_pause(test_obj.timer_curtain_second);
    g_flag_curtain = 0;
}
static void btn_curtain_open_cb(lv_event_t *e) //打开
{
    lv_timer_pause(test_obj.timer_curtain_second);
    g_flag_curtain = -1;
    lv_timer_resume(test_obj.timer_curtain_second);
}
static void btn_curtain_close_cb(lv_event_t *e) //关闭
{
    lv_timer_pause(test_obj.timer_curtain_second);
    g_flag_curtain = 1;
    lv_timer_resume(test_obj.timer_curtain_second);
}

static int g_value_air_condition = 26; //空调温度
static void btn_air_condition_left_cb(lv_event_t *e)
{
    if (g_value_air_condition > 16)
        lv_label_set_text_fmt(test_obj.label_air_condition_tp_second, "%d°", --g_value_air_condition);
}

static void btn_air_condition_right_cb(lv_event_t *e)
{
    if (g_value_air_condition < 32)
        lv_label_set_text_fmt(test_obj.label_air_condition_tp_second, "%d°", ++g_value_air_condition);
}

static void btn_air_condition_cold_cb(lv_event_t *e)
{
    lv_obj_set_style_border_opa(test_obj.btn_air_condition_wet_second, LV_OPA_0, 0);
    lv_obj_set_style_border_opa(test_obj.btn_air_condition_cold_second, LV_OPA_100, 0);
    lv_obj_set_style_border_opa(test_obj.btn_air_condition_hot_second, LV_OPA_0, 0);
}
static void btn_air_condition_hot_cb(lv_event_t *e)
{
    lv_obj_set_style_border_opa(test_obj.btn_air_condition_wet_second, LV_OPA_0, 0);
    lv_obj_set_style_border_opa(test_obj.btn_air_condition_cold_second, LV_OPA_0, 0);
    lv_obj_set_style_border_opa(test_obj.btn_air_condition_hot_second, LV_OPA_100, 0);
}
static void btn_air_condition_wet_cb(lv_event_t *e)
{
    lv_obj_set_style_border_opa(test_obj.btn_air_condition_wet_second, LV_OPA_100, 0);
    lv_obj_set_style_border_opa(test_obj.btn_air_condition_cold_second, LV_OPA_0, 0);
    lv_obj_set_style_border_opa(test_obj.btn_air_condition_hot_second, LV_OPA_0, 0);
}

void page_first(void)
{

    test_obj.img_background_first = lv_obj_create(test_obj.page_first);
    lv_obj_set_size(test_obj.img_background_first, 480, 480);
    lv_obj_set_style_bg_color(test_obj.img_background_first, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(test_obj.img_background_first, 255, 0);
    lv_obj_set_style_border_width(test_obj.img_background_first, 0, 0);
    lv_obj_set_style_radius(test_obj.img_background_first, 0, 0);
    
    lv_obj_center(test_obj.img_background_first);

    test_obj.label_time_hours_first = lv_label_create(test_obj.img_background_first);
    lv_obj_set_style_text_font(test_obj.label_time_hours_first, &font_BOLD_95, 0); //添加样式
    lv_obj_set_style_text_color(test_obj.label_time_hours_first, lv_color_white(), 0);
    lv_label_set_text(test_obj.label_time_hours_first, "10");
    lv_obj_align(test_obj.label_time_hours_first, LV_ALIGN_TOP_LEFT, 50, 70);

    test_obj.label_time_point_first = lv_label_create(test_obj.img_background_first);
    lv_obj_set_style_text_font(test_obj.label_time_point_first, &font_BOLD_95, 0); //添加样式
    lv_obj_set_style_text_color(test_obj.label_time_point_first, lv_color_white(), 0);
    lv_label_set_text(test_obj.label_time_point_first, ":");
    lv_obj_align_to(test_obj.label_time_point_first, test_obj.label_time_hours_first, LV_ALIGN_OUT_RIGHT_BOTTOM, 7, 0);

    test_obj.label_time_minutes_first = lv_label_create(test_obj.img_background_first);
    lv_obj_set_style_text_font(test_obj.label_time_minutes_first, &font_BOLD_95, 0); //添加样式
    lv_obj_set_style_text_color(test_obj.label_time_minutes_first, lv_color_white(), 0);
    lv_label_set_text(test_obj.label_time_minutes_first, "12");
    lv_obj_align_to(test_obj.label_time_minutes_first, test_obj.label_time_point_first, LV_ALIGN_OUT_RIGHT_BOTTOM, 7, 0);

    test_obj.label_date_first = lv_label_create(test_obj.img_background_first);
    lv_obj_set_style_text_font(test_obj.label_date_first, &font_BOLD_30, 0); //添加样式
    lv_obj_set_style_text_color(test_obj.label_date_first, lv_color_white(), 0);
    lv_label_set_text(test_obj.label_date_first, "8月12日");
    lv_obj_align_to(test_obj.label_date_first, test_obj.label_time_hours_first, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 25);

    test_obj.label_week_first = lv_label_create(test_obj.img_background_first);
    lv_obj_set_style_text_font(test_obj.label_week_first, &font_BOLD_30, 0); //添加样式
    lv_obj_set_style_text_color(test_obj.label_week_first, lv_color_white(), 0);
    lv_label_set_text(test_obj.label_week_first, "星期二");
    lv_obj_align_to(test_obj.label_week_first, test_obj.label_date_first, LV_ALIGN_OUT_RIGHT_MID, 40, 0);

    test_obj.img_weather_first = lv_img_create(test_obj.page_first);
    lv_img_set_src(test_obj.img_weather_first, IMG_WEATHER_FIRST);
    lv_obj_align(test_obj.img_weather_first, LV_ALIGN_TOP_RIGHT, -50, 80);

    test_obj.label_tp_first = lv_label_create(test_obj.img_background_first);
    lv_obj_set_style_text_font(test_obj.label_tp_first, &font_BOLD_30, 0); //添加样式
    lv_obj_set_style_text_color(test_obj.label_tp_first, lv_color_white(), 0);
    lv_label_set_text(test_obj.label_tp_first, "26°");
    lv_obj_align_to(test_obj.label_tp_first, test_obj.img_weather_first, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    test_obj.back_first_2 = lv_obj_create(test_obj.page_first);
    lv_obj_remove_style_all(test_obj.back_first_2);
    lv_obj_set_style_bg_opa(test_obj.back_first_2, LV_OPA_COVER, 0); // 0不透明
    lv_obj_set_size(test_obj.back_first_2, 290, 35);
    lv_obj_set_style_bg_color(test_obj.back_first_2, lv_color_black(), 0);
    lv_obj_set_style_radius(test_obj.back_first_2, 50, 0);
    lv_obj_align_to(test_obj.back_first_2, test_obj.label_time_hours_first, LV_ALIGN_OUT_BOTTOM_LEFT, -15, 100);
    lv_obj_set_scrollbar_mode(test_obj.back_first_2, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(test_obj.back_first_2, LV_OBJ_FLAG_SCROLLABLE);

    test_obj.img_ht_first = lv_img_create(test_obj.back_first_2);
    lv_img_set_src(test_obj.img_ht_first, IMG_HT_FIRST);
    lv_obj_align(test_obj.img_ht_first, LV_ALIGN_LEFT_MID, 20, 0);

    test_obj.label_ht_1 = lv_label_create(test_obj.back_first_2);
    lv_obj_set_style_text_font(test_obj.label_ht_1, &font_15, 0); //添加样式
    lv_obj_set_style_text_color(test_obj.label_ht_1, lv_color_white(), 0);
    lv_label_set_text(test_obj.label_ht_1, "湿度");
    lv_obj_align_to(test_obj.label_ht_1, test_obj.img_ht_first, LV_ALIGN_OUT_RIGHT_MID, 8, 0);

    test_obj.label_ht_2 = lv_label_create(test_obj.back_first_2);
    lv_obj_set_style_text_font(test_obj.label_ht_2, &font_20, 0); //添加样式
    lv_obj_set_style_text_color(test_obj.label_ht_2, lv_color_white(), 0);
    lv_label_set_text(test_obj.label_ht_2, "60\%");
    lv_obj_align_to(test_obj.label_ht_2, test_obj.label_ht_1, LV_ALIGN_OUT_RIGHT_MID, 12, -2);

    test_obj.img_air_first = lv_img_create(test_obj.back_first_2);
    lv_img_set_src(test_obj.img_air_first, IMG_AIR_FIRST);
    lv_obj_align(test_obj.img_air_first, LV_ALIGN_LEFT_MID, 150, 0);

    test_obj.label_air_first_1 = lv_label_create(test_obj.back_first_2);
    lv_obj_set_style_text_font(test_obj.label_air_first_1, &font_15, 0); //添加样式
    lv_obj_set_style_text_color(test_obj.label_air_first_1, lv_color_white(), 0);
    lv_label_set_text(test_obj.label_air_first_1, "空气质量");
    lv_obj_align_to(test_obj.label_air_first_1, test_obj.img_air_first, LV_ALIGN_OUT_RIGHT_MID, 8, 0);

    test_obj.label_air_first_2 = lv_label_create(test_obj.back_first_2);
    lv_obj_set_style_text_font(test_obj.label_air_first_2, &font_20, 0); //添加样式
    lv_obj_set_style_text_color(test_obj.label_air_first_2, lv_color_white(), 0);
    lv_label_set_text(test_obj.label_air_first_2, "33");
    lv_obj_align_to(test_obj.label_air_first_2, test_obj.label_air_first_1, LV_ALIGN_OUT_RIGHT_MID, 12, -2);

    test_obj.btn_clean = lv_btn_create(test_obj.page_first);
    lv_obj_remove_style_all(test_obj.btn_clean);
    lv_obj_set_size(test_obj.btn_clean, 120, 120);
    lv_obj_set_style_bg_opa(test_obj.btn_clean, LV_OPA_0, 0); //透明
    lv_obj_set_style_radius(test_obj.btn_clean, 30, 0);
    lv_obj_set_scrollbar_mode(test_obj.btn_clean, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(test_obj.btn_clean, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_align(test_obj.btn_clean, LV_ALIGN_BOTTOM_LEFT, 40, -50);

    test_obj.img_clean_first = lv_img_create(test_obj.btn_clean);
    lv_img_set_src(test_obj.img_clean_first, IMG_CLEAN_FIRST);
    lv_obj_align(test_obj.img_clean_first, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(test_obj.img_clean_first, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(test_obj.img_clean_first, btn_clean_first_cb, LV_EVENT_CLICKED, NULL);


    test_obj.btn_not_disturb_first = lv_btn_create(test_obj.page_first);
    lv_obj_remove_style_all(test_obj.btn_not_disturb_first);
    lv_obj_set_size(test_obj.btn_not_disturb_first, 120, 120);
    lv_obj_set_style_bg_opa(test_obj.btn_not_disturb_first, LV_OPA_0, 0); //透明
    lv_obj_set_style_radius(test_obj.btn_not_disturb_first, 30, 0);
    lv_obj_set_scrollbar_mode(test_obj.btn_not_disturb_first, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(test_obj.btn_not_disturb_first, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_align_to(test_obj.btn_not_disturb_first, test_obj.btn_clean, LV_ALIGN_OUT_RIGHT_MID, 20, 0);

    test_obj.img_not_disturb_first = lv_img_create(test_obj.btn_not_disturb_first);
    lv_img_set_src(test_obj.img_not_disturb_first, IMG_NTO_DISTURB_FIRST);
    lv_obj_align(test_obj.img_not_disturb_first, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(test_obj.img_not_disturb_first, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(test_obj.img_not_disturb_first, btn_not_disturb_first_cb, LV_EVENT_CLICKED, NULL);



    test_obj.btn_more_first = lv_btn_create(test_obj.page_first);
    lv_obj_remove_style_all(test_obj.btn_more_first);
    lv_obj_set_size(test_obj.btn_more_first, 120, 120);
    lv_obj_set_style_bg_opa(test_obj.btn_more_first, LV_OPA_0, 0); //透明
    lv_obj_set_style_radius(test_obj.btn_more_first, 30, 0);
    lv_obj_set_scrollbar_mode(test_obj.btn_more_first, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(test_obj.btn_more_first, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_align_to(test_obj.btn_more_first, test_obj.btn_not_disturb_first, LV_ALIGN_OUT_RIGHT_MID, 20, 0);

    test_obj.img_more_first = lv_img_create(test_obj.btn_more_first);
    lv_img_set_src(test_obj.img_more_first, IMG_MORE_FIRST);
    lv_obj_align(test_obj.img_more_first, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(test_obj.img_more_first, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(test_obj.img_more_first, btn_more_first_cb, LV_EVENT_CLICKED, NULL);


    if (test_obj.timer_first == NULL)
    {
        test_obj.timer_first = lv_timer_create(test_timer_cb, 500, NULL);
    }
}
void page_second(void)
{
    /* 清洁 */
    test_obj.btn2_clean_second = lv_btn_create(test_obj.page_second);
    lv_obj_remove_style_all(test_obj.btn2_clean_second);
    lv_obj_set_size(test_obj.btn2_clean_second, 120, 120);
    lv_obj_set_style_bg_opa(test_obj.btn2_clean_second, LV_OPA_0, 0); //透明
    lv_obj_set_style_radius(test_obj.btn2_clean_second, 30, 0);
    lv_obj_set_scrollbar_mode(test_obj.btn2_clean_second, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(test_obj.btn2_clean_second, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_align(test_obj.btn2_clean_second, LV_ALIGN_TOP_LEFT, 40, 40);

    test_obj.img2_clean_second = lv_img_create(test_obj.btn2_clean_second);
    lv_img_set_src(test_obj.img2_clean_second, IMG2_CLEAN_SECOND);
    lv_obj_align(test_obj.img2_clean_second, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(test_obj.img2_clean_second, LV_OBJ_FLAG_CLICKABLE);


    /* 勿扰 */
    test_obj.btn2_not_disturb_second = lv_btn_create(test_obj.page_second);
    lv_obj_remove_style_all(test_obj.btn2_not_disturb_second);
    lv_obj_set_size(test_obj.btn2_not_disturb_second, 120, 120);
    lv_obj_set_style_bg_opa(test_obj.btn2_not_disturb_second, LV_OPA_0, 0); //透明
    lv_obj_set_style_radius(test_obj.btn2_not_disturb_second, 30, 0);
    lv_obj_set_scrollbar_mode(test_obj.btn2_not_disturb_second, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(test_obj.btn2_not_disturb_second, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_align_to(test_obj.btn2_not_disturb_second, test_obj.btn2_clean_second, LV_ALIGN_OUT_RIGHT_MID, -2, 0);

    test_obj.img2_not_disturb_second = lv_img_create(test_obj.btn2_not_disturb_second);
    lv_img_set_src(test_obj.img2_not_disturb_second, IMG2_NTO_DISTURB_SECOND);
    lv_obj_align(test_obj.img2_not_disturb_second, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(test_obj.img2_not_disturb_second, LV_OBJ_FLAG_CLICKABLE);


    /* 筒灯 */
    test_obj.img_tubelamp_second = lv_img_create(test_obj.page_second);
    lv_img_set_src(test_obj.img_tubelamp_second, IMG_TUBELAMP_SECOND);
    lv_obj_align_to(test_obj.img_tubelamp_second, test_obj.btn2_clean_second, LV_ALIGN_OUT_BOTTOM_LEFT, 7, 10);

    test_obj.obj_tubelamp_second = lv_btn_create(test_obj.img_tubelamp_second);
    lv_obj_remove_style_all(test_obj.obj_tubelamp_second);
    lv_obj_set_size(test_obj.obj_tubelamp_second, 75, 30);
    lv_obj_set_style_bg_opa(test_obj.obj_tubelamp_second, LV_OPA_20, 0); //透明
    lv_obj_set_style_radius(test_obj.obj_tubelamp_second, 30, 0);
    lv_obj_set_style_bg_color(test_obj.obj_tubelamp_second, lv_color_black(), 0);
    lv_obj_set_scrollbar_mode(test_obj.obj_tubelamp_second, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(test_obj.obj_tubelamp_second, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_align(test_obj.obj_tubelamp_second, LV_ALIGN_TOP_LEFT, 15, 15);

    test_obj.label_tubelamp_second = lv_label_create(test_obj.obj_tubelamp_second);
    lv_obj_set_style_text_font(test_obj.label_tubelamp_second, &font_18, 0); //添加样式
    lv_obj_set_style_text_color(test_obj.label_tubelamp_second, lv_color_white(), 0);
    lv_label_set_text(test_obj.label_tubelamp_second, "筒灯");
    lv_obj_align(test_obj.label_tubelamp_second, LV_ALIGN_CENTER, 0, 0);

    test_obj.switch_tubelamp2_second = lv_switch_create(test_obj.img_tubelamp_second);
    lv_obj_set_size(test_obj.switch_tubelamp2_second, 43, 22);
    lv_obj_align(test_obj.switch_tubelamp2_second, LV_ALIGN_BOTTOM_LEFT, 30, -18);

    // test_obj.img_tubelamp2_second = lv_img_create(test_obj.img_tubelamp_second);
    // lv_img_set_src(test_obj.img_tubelamp2_second, IMG_LAMP);
    // lv_obj_align(test_obj.img_tubelamp2_second, LV_ALIGN_TOP_RIGHT, -20, 20);

    /* 廊灯 */
    test_obj.img_corridorlamp_second = lv_img_create(test_obj.page_second);
    lv_img_set_src(test_obj.img_corridorlamp_second, IMG_CORRIDORLAMP_SECOND);
    lv_obj_align_to(test_obj.img_corridorlamp_second, test_obj.img_tubelamp_second, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);

    test_obj.obj_corridorlamp_second = lv_btn_create(test_obj.img_corridorlamp_second);
    lv_obj_remove_style_all(test_obj.obj_corridorlamp_second);
    lv_obj_set_size(test_obj.obj_corridorlamp_second, 75, 30);
    lv_obj_set_style_bg_opa(test_obj.obj_corridorlamp_second, LV_OPA_20, 0); //透明
    lv_obj_set_style_radius(test_obj.obj_corridorlamp_second, 30, 0);
    lv_obj_set_style_bg_color(test_obj.obj_corridorlamp_second, lv_color_black(), 0);
    lv_obj_set_scrollbar_mode(test_obj.obj_corridorlamp_second, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(test_obj.obj_corridorlamp_second, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_align(test_obj.obj_corridorlamp_second, LV_ALIGN_TOP_LEFT, 15, 15);

    test_obj.label_corridorlamp_second = lv_label_create(test_obj.obj_corridorlamp_second);
    lv_obj_set_style_text_font(test_obj.label_corridorlamp_second, &font_18, 0); //添加样式
    lv_obj_set_style_text_color(test_obj.label_corridorlamp_second, lv_color_white(), 0);
    lv_label_set_text(test_obj.label_corridorlamp_second, "廊灯");
    lv_obj_align(test_obj.label_corridorlamp_second, LV_ALIGN_CENTER, 0, 0);

    test_obj.switch_corridorlamp2_second = lv_switch_create(test_obj.img_corridorlamp_second);
    lv_obj_set_size(test_obj.switch_corridorlamp2_second, 43, 22);
    lv_obj_align(test_obj.switch_corridorlamp2_second, LV_ALIGN_BOTTOM_LEFT, 30, -18);



    /* 窗帘 */
    test_obj.img_curtain_second = lv_img_create(test_obj.page_second);
    lv_img_set_src(test_obj.img_curtain_second, IMG_CURTAIN_SECOND);
    lv_obj_align(test_obj.img_curtain_second, LV_ALIGN_TOP_RIGHT, -20, 45);

    /* 进度条 */
    test_obj.bar_curtain_second = lv_bar_create(test_obj.img_curtain_second);
    lv_bar_set_range(test_obj.bar_curtain_second, 0, 100);
    lv_obj_set_size(test_obj.bar_curtain_second, 143, 30);
    lv_obj_set_style_radius(test_obj.bar_curtain_second, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(test_obj.bar_curtain_second, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(test_obj.bar_curtain_second, lv_color_make(255, 165, 37), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(test_obj.bar_curtain_second, lv_color_black(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(test_obj.bar_curtain_second, LV_OPA_40, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(test_obj.bar_curtain_second, LV_ALIGN_CENTER, 0, 5);
    lv_bar_set_value(test_obj.bar_curtain_second, g_value_curtain, LV_ANIM_OFF);

    test_obj.label_curtain_second = lv_label_create(test_obj.img_curtain_second);
    lv_obj_set_style_text_font(test_obj.label_curtain_second, &font_12, 0); //添加样式
    lv_obj_set_style_text_color(test_obj.label_curtain_second, lv_color_white(), 0);
    lv_label_set_text(test_obj.label_curtain_second, "");
    lv_obj_align_to(test_obj.label_curtain_second, test_obj.bar_curtain_second, LV_ALIGN_OUT_TOP_LEFT, 0, -10);



    /* 暂停 */
    test_obj.btn_curtain_suspend_second = lv_btn_create(test_obj.img_curtain_second);
    lv_obj_remove_style_all(test_obj.btn_curtain_suspend_second);
    lv_obj_set_size(test_obj.btn_curtain_suspend_second, 35, 35);
    lv_obj_set_style_bg_opa(test_obj.btn_curtain_suspend_second, LV_OPA_0, 0); //透明
    lv_obj_set_style_bg_opa(test_obj.btn_curtain_suspend_second, LV_OPA_40, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_radius(test_obj.btn_curtain_suspend_second, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(test_obj.btn_curtain_suspend_second, lv_color_make(211, 211, 211), 0);
    lv_obj_set_scrollbar_mode(test_obj.btn_curtain_suspend_second, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(test_obj.btn_curtain_suspend_second, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_align(test_obj.btn_curtain_suspend_second, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_add_event_cb(test_obj.btn_curtain_suspend_second, btn_curtain_suspend_cb, LV_EVENT_CLICKED, NULL);

    test_obj.label_curtain_suspend_second = lv_label_create(test_obj.btn_curtain_suspend_second);
    lv_obj_set_style_text_font(test_obj.label_curtain_suspend_second, &lv_font_montserrat_14, 0); //添加样式
    lv_obj_set_style_text_color(test_obj.label_curtain_suspend_second, lv_color_white(), 0);
    lv_label_set_text(test_obj.label_curtain_suspend_second, LV_SYMBOL_PAUSE);
    lv_obj_align(test_obj.label_curtain_suspend_second, LV_ALIGN_CENTER, 0, 0);

    /* 打开 */
    test_obj.btn_curtain_open_second = lv_btn_create(test_obj.img_curtain_second);
    lv_obj_remove_style_all(test_obj.btn_curtain_open_second);
    lv_obj_set_size(test_obj.btn_curtain_open_second, 35, 35);
    lv_obj_set_style_bg_opa(test_obj.btn_curtain_open_second, LV_OPA_0, 0); //透明
    lv_obj_set_style_bg_opa(test_obj.btn_curtain_open_second, LV_OPA_40, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_radius(test_obj.btn_curtain_open_second, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(test_obj.btn_curtain_open_second, lv_color_make(211, 211, 211), 0);
    lv_obj_set_scrollbar_mode(test_obj.btn_curtain_open_second, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(test_obj.btn_curtain_open_second, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_align_to(test_obj.btn_curtain_open_second, test_obj.btn_curtain_suspend_second, LV_ALIGN_OUT_LEFT_MID, -15, 0);
    lv_obj_add_event_cb(test_obj.btn_curtain_open_second, btn_curtain_open_cb, LV_EVENT_CLICKED, NULL);

    test_obj.img_curtain_open_second = lv_img_create(test_obj.btn_curtain_open_second);
    lv_img_set_src(test_obj.img_curtain_open_second, IMG_CURTAIN_OPEN_SECOND);
    lv_obj_align(test_obj.img_curtain_open_second, LV_ALIGN_CENTER, 0, 0);

    /* 关闭 */
    test_obj.btn_curtain_close_second = lv_btn_create(test_obj.img_curtain_second);
    lv_obj_remove_style_all(test_obj.btn_curtain_close_second);
    lv_obj_set_size(test_obj.btn_curtain_close_second, 35, 35);
    lv_obj_set_style_bg_opa(test_obj.btn_curtain_close_second, LV_OPA_0, 0); //透明
    lv_obj_set_style_bg_opa(test_obj.btn_curtain_close_second, LV_OPA_40, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_radius(test_obj.btn_curtain_close_second, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(test_obj.btn_curtain_close_second, lv_color_make(211, 211, 211), 0);
    lv_obj_set_scrollbar_mode(test_obj.btn_curtain_close_second, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(test_obj.btn_curtain_close_second, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_align_to(test_obj.btn_curtain_close_second, test_obj.btn_curtain_suspend_second, LV_ALIGN_OUT_RIGHT_MID, 15, 0);
    lv_obj_add_event_cb(test_obj.btn_curtain_close_second, btn_curtain_close_cb, LV_EVENT_CLICKED, NULL);

    test_obj.img_curtain_close_second = lv_img_create(test_obj.btn_curtain_close_second);
    lv_img_set_src(test_obj.img_curtain_close_second, IMG_CURTAIN_CLOSE_SECOND);
    lv_obj_align(test_obj.img_curtain_close_second, LV_ALIGN_CENTER, 0, 0);

    if (test_obj.timer_curtain_second == NULL)
    {
        test_obj.timer_curtain_second = lv_timer_create(test_timer_curtain_cb, 10, NULL);
        lv_timer_pause(test_obj.timer_curtain_second); //暂停定时器
    }

    /* 空调 */
    test_obj.img_air_condition_second = lv_img_create(test_obj.page_second);
    lv_img_set_src(test_obj.img_air_condition_second, IMG_AIR_CONDITION_SECOND);
    lv_obj_align_to(test_obj.img_air_condition_second, test_obj.img_curtain_second, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);


    test_obj.label_air_condition_tp_second = lv_label_create(test_obj.img_air_condition_second);
    lv_obj_set_style_text_font(test_obj.label_air_condition_tp_second, &font_40, 0); //添加样式
    lv_obj_set_style_text_color(test_obj.label_air_condition_tp_second, lv_color_white(), 0);
    lv_label_set_text_fmt(test_obj.label_air_condition_tp_second, "%d°", g_value_air_condition);
    lv_obj_align(test_obj.label_air_condition_tp_second, LV_ALIGN_CENTER, 5, -10);

    /* 减小 */
    test_obj.btn_air_condition_left_second = lv_btn_create(test_obj.img_air_condition_second);
    lv_obj_remove_style_all(test_obj.btn_air_condition_left_second);
    lv_obj_set_size(test_obj.btn_air_condition_left_second, 35, 35);
    lv_obj_set_style_radius(test_obj.btn_air_condition_left_second, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_opa(test_obj.btn_air_condition_left_second, LV_OPA_0, 0); //透明
    lv_obj_set_style_bg_opa(test_obj.btn_air_condition_left_second, LV_OPA_40, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(test_obj.btn_air_condition_left_second, lv_color_make(211, 211, 211), 0);
    lv_obj_align_to(test_obj.btn_air_condition_left_second, test_obj.label_air_condition_tp_second, LV_ALIGN_OUT_LEFT_MID, -15, 3);
    lv_obj_add_event_cb(test_obj.btn_air_condition_left_second, btn_air_condition_left_cb, LV_EVENT_CLICKED, NULL);

    test_obj.img_air_condition_left_second = lv_img_create(test_obj.btn_air_condition_left_second);
    lv_img_set_src(test_obj.img_air_condition_left_second, IMG_AIR_CONDITION_LEFT_SECOND);
    lv_obj_align(test_obj.img_air_condition_left_second, LV_ALIGN_CENTER, 0, 0);

    /* 增大 */
    test_obj.btn_air_condition_right_second = lv_btn_create(test_obj.img_air_condition_second);
    lv_obj_remove_style_all(test_obj.btn_air_condition_right_second);
    lv_obj_set_size(test_obj.btn_air_condition_right_second, 35, 35);
    lv_obj_set_style_radius(test_obj.btn_air_condition_right_second, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_opa(test_obj.btn_air_condition_right_second, LV_OPA_0, 0); //透明
    lv_obj_set_style_bg_opa(test_obj.btn_air_condition_right_second, LV_OPA_40, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(test_obj.btn_air_condition_right_second, lv_color_make(211, 211, 211), 0);
    lv_obj_align_to(test_obj.btn_air_condition_right_second, test_obj.label_air_condition_tp_second, LV_ALIGN_OUT_RIGHT_MID, 15, 3);
    lv_obj_add_event_cb(test_obj.btn_air_condition_right_second, btn_air_condition_right_cb, LV_EVENT_CLICKED, NULL);

    test_obj.img_air_condition_right_second = lv_img_create(test_obj.btn_air_condition_right_second);
    lv_img_set_src(test_obj.img_air_condition_right_second, IMG_AIR_CONDITION_RIGHT_SECOND);
    lv_obj_align(test_obj.img_air_condition_right_second, LV_ALIGN_CENTER, 0, 0);

    /* 制热*/
    test_obj.btn_air_condition_hot_second = lv_btn_create(test_obj.img_air_condition_second);
    lv_obj_remove_style_all(test_obj.btn_air_condition_hot_second);
    lv_obj_set_size(test_obj.btn_air_condition_hot_second, 45, 45);
    lv_obj_set_style_radius(test_obj.btn_air_condition_hot_second, 5, 0);
    lv_obj_set_style_bg_opa(test_obj.btn_air_condition_hot_second, LV_OPA_0, 0); //透明
    lv_obj_set_style_border_opa(test_obj.btn_air_condition_hot_second, LV_OPA_0, 0);
    lv_obj_set_style_border_color(test_obj.btn_air_condition_hot_second, lv_color_white(), 0);
    lv_obj_set_style_border_width(test_obj.btn_air_condition_hot_second, 1, 0);
    lv_obj_align(test_obj.btn_air_condition_hot_second, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_obj_add_event_cb(test_obj.btn_air_condition_hot_second, btn_air_condition_hot_cb, LV_EVENT_CLICKED, NULL);

    test_obj.img_air_condition_hot_second = lv_img_create(test_obj.btn_air_condition_hot_second);
    lv_img_set_src(test_obj.img_air_condition_hot_second, IMG_AIR_CONDITION_HOT_SECOND);
    lv_obj_align(test_obj.img_air_condition_hot_second, LV_ALIGN_CENTER, 0, 0);

    /* 制冷 */
    test_obj.btn_air_condition_cold_second = lv_btn_create(test_obj.img_air_condition_second);
    lv_obj_remove_style_all(test_obj.btn_air_condition_cold_second);
    lv_obj_set_size(test_obj.btn_air_condition_cold_second, 45, 45);
    lv_obj_set_style_radius(test_obj.btn_air_condition_cold_second, 5, 0);
    lv_obj_set_style_border_opa(test_obj.btn_air_condition_cold_second, LV_OPA_0, 0);
    lv_obj_set_style_border_color(test_obj.btn_air_condition_cold_second, lv_color_white(), 0);
    lv_obj_set_style_border_width(test_obj.btn_air_condition_cold_second, 1, 0);
    lv_obj_set_style_bg_opa(test_obj.btn_air_condition_cold_second, LV_OPA_0, 0); //透明
    lv_obj_align_to(test_obj.btn_air_condition_cold_second, test_obj.btn_air_condition_hot_second, LV_ALIGN_OUT_LEFT_MID, -5, 0);
    lv_obj_add_event_cb(test_obj.btn_air_condition_cold_second, btn_air_condition_cold_cb, LV_EVENT_CLICKED, NULL);

    test_obj.img_air_condition_cold_second = lv_img_create(test_obj.btn_air_condition_cold_second);
    lv_img_set_src(test_obj.img_air_condition_cold_second, IMG_AIR_CONDITION_COLD_SECOND);
    lv_obj_align(test_obj.img_air_condition_cold_second, LV_ALIGN_CENTER, 0, 0);

    /* 除湿 */
    test_obj.btn_air_condition_wet_second = lv_btn_create(test_obj.img_air_condition_second);
    lv_obj_remove_style_all(test_obj.btn_air_condition_wet_second);
    lv_obj_set_size(test_obj.btn_air_condition_wet_second, 45, 45);
    lv_obj_set_style_radius(test_obj.btn_air_condition_wet_second, 5, 0);
    lv_obj_set_style_border_opa(test_obj.btn_air_condition_wet_second, LV_OPA_0, 0);
    lv_obj_set_style_border_color(test_obj.btn_air_condition_wet_second, lv_color_white(), 0);
    lv_obj_set_style_border_width(test_obj.btn_air_condition_wet_second, 1, 0);
    lv_obj_set_style_bg_opa(test_obj.btn_air_condition_wet_second, LV_OPA_0, 0); //透明
    lv_obj_align_to(test_obj.btn_air_condition_wet_second, test_obj.btn_air_condition_hot_second, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    lv_obj_add_event_cb(test_obj.btn_air_condition_wet_second, btn_air_condition_wet_cb, LV_EVENT_CLICKED, NULL);

    test_obj.img_air_condition_wet_second = lv_img_create(test_obj.btn_air_condition_wet_second);
    lv_img_set_src(test_obj.img_air_condition_wet_second, IMG_AIR_CONDITION_WET_SECOND);
    lv_obj_align(test_obj.img_air_condition_wet_second, LV_ALIGN_CENTER, 0, 0);
}

static void test_tv_cb(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_VALUE_CHANGED)
    {
        if (lv_tileview_get_tile_act(obj) == test_obj.page_first)
        {
            lv_img_set_src(test_obj.img_top_src1, IMG_SWICH_Y);
            lv_img_set_src(test_obj.img_top_src2, IMG_SWICH_N);
        }
        else
        {
            lv_img_set_src(test_obj.img_top_src1, IMG_SWICH_N);
            lv_img_set_src(test_obj.img_top_src2, IMG_SWICH_Y);
        }
    }
}
void test_demo(void)
{
    static lv_style_t style; //创建样式变量
    static bool style_flag = false;
    if (style_flag == false)
    {
        lv_style_init(&style);                             //初始化样式
        lv_style_set_bg_color(&style, lv_color_black());   //设置背景颜色
        lv_style_set_text_color(&style, lv_color_white()); //设置字体颜色
        lv_style_set_border_width(&style, 0);              //设置边框宽度
        lv_style_set_pad_all(&style, 0);                   //设置边距
        style_flag = true;
    }

    

    test_obj.test_tv = lv_tileview_create(lv_scr_act());
    lv_obj_add_style(test_obj.test_tv, &style, 0); //添加样式
    lv_obj_set_scrollbar_mode(test_obj.test_tv, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_event_cb(test_obj.test_tv, test_tv_cb, LV_EVENT_ALL, NULL);

    /*首屏*/
    test_obj.page_first = lv_tileview_add_tile(test_obj.test_tv, 0, 0, LV_DIR_HOR);
    lv_obj_set_style_bg_color(test_obj.page_first, lv_color_black(), 0);
    page_first();

    /*次屏*/
    test_obj.page_second = lv_tileview_add_tile(test_obj.test_tv, 1, 0, LV_DIR_HOR);
    page_second();

    lv_obj_set_scrollbar_mode(test_obj.page_first, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scrollbar_mode(test_obj.page_second, LV_SCROLLBAR_MODE_OFF);

    test_obj.img_top_src1 = lv_img_create(lv_layer_top());
    lv_img_set_src(test_obj.img_top_src1, IMG_SWICH_Y);
    lv_obj_align(test_obj.img_top_src1, LV_ALIGN_BOTTOM_MID, -15, -15);

    test_obj.img_top_src2 = lv_img_create(lv_layer_top());
    lv_img_set_src(test_obj.img_top_src2, IMG_SWICH_N);
    lv_obj_align(test_obj.img_top_src2, LV_ALIGN_BOTTOM_MID, 15, -15);

    top_black = lv_obj_create(lv_layer_top());
    lv_obj_remove_style_all(top_black);
    lv_obj_set_size(top_black, 480, 480);
    lv_obj_set_style_bg_color(top_black, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(top_black, LV_OPA_100, 0);
    lv_obj_add_flag(top_black, LV_OBJ_FLAG_HIDDEN);

    disp_check_tiemr = lv_timer_create(disp_check_tiemr_cb, 1000, NULL);
}

static void disp_check_tiemr_cb(struct _lv_timer_t * timer)
{
    long pin = 0;
    unsigned int g;
    unsigned int p;
    pin = hal_gpio_name2pin("PC.7");
    g = GPIO_GROUP(pin);
    p = GPIO_GROUP_PIN(pin);

    uint32_t tick = lv_disp_get_inactive_time(lv_disp_get_default());
    // rt_kprintf("tick:%u\n", tick);
    static bool disp_flag = false;
    if(10000 >= tick)   //熄屏
    {
        if(disp_flag == false)
        {
            disp_flag = true;
            hal_gpio_set_output(g, p);
            lv_obj_add_flag(top_black, LV_OBJ_FLAG_HIDDEN);
        }
    }else{
        if(disp_flag == true)
        {
            disp_flag = false;
            hal_gpio_clr_output(g, p);
            lv_obj_clear_flag(top_black, LV_OBJ_FLAG_HIDDEN);
        }
    }
}
void zx_ui_entry(void)
{
    test_demo();

}