#include "ui_common.h"

static lv_obj_t *top_layer;
static void top_scr_obj_create(void);

static lv_obj_t* img_wifi = NULL;
static lv_obj_t* label_AM = NULL;
static lv_obj_t* label_hour = NULL;
static lv_obj_t* label_point = NULL;
static lv_obj_t* label_second = NULL;
static lv_timer_t *t_top = NULL;


static void refr_src_disp(void)
{
    static bool flag = false;
    if(label_point)
    {
        if(flag)
        {
            flag = false;
            lv_label_set_text(label_point, ":");
        }
        else
        {
            flag = true;
            lv_label_set_text(label_point, " ");
        }
    }
}

static void top_layer_load_func(void *arg)
{
    printf("top_layer_load_func\n");

    lv_timer_resume(t_top);
}

static void top_layer_refr_func(Event_Data_t *arg)
{
    if(arg == NULL)
    {

    }
    else
    {
        if(arg->eEventID == E_KEY_EVENT)
            ;
    }
}

static void top_layer_quit_func(void *arg)
{
    printf("top_layer_quit_func\n");
    lv_obj_clean(lv_layer_top());
    lv_timer_pause(t_top);
}


static void top_scr_obj_create(void)
{
    img_wifi = lv_img_create(lv_layer_top());
    lv_img_set_src(img_wifi, U_WIFI_IMG);
    lv_obj_clear_flag(img_wifi, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_align(img_wifi, LV_ALIGN_TOP_RIGHT, -38, 12);

    label_AM = lv_label_create(lv_layer_top());
    lv_obj_set_style_text_font(label_AM, &MiSans_Regular_14, 0);
    lv_obj_set_style_text_color(label_AM, lv_color_make(255, 255, 255), 0);
    lv_obj_set_style_text_opa(label_AM, LV_OPA_60, 0);
    lv_label_set_text(label_AM, "AM");
    lv_obj_align(label_AM, LV_ALIGN_TOP_LEFT, 16, 15);

    label_hour = lv_label_create(lv_layer_top());
    lv_obj_set_style_text_font(label_hour, &MiSans_Regular_16, 0);
    lv_obj_set_style_text_color(label_hour, lv_color_make(255, 255, 255), 0);
    lv_obj_set_style_text_opa(label_hour, LV_OPA_80, 0);
    lv_label_set_text(label_hour, "12");
    lv_obj_align(label_hour, LV_ALIGN_TOP_LEFT, 45, 13);

    label_point = lv_label_create(lv_layer_top());
    lv_obj_set_style_text_font(label_point, &MiSans_Regular_16, 0);
    lv_obj_set_style_text_color(label_point, lv_color_make(255, 255, 255), 0);
    lv_obj_set_style_text_opa(label_point, LV_OPA_80, 0);
    lv_label_set_text(label_point, ":");
    lv_obj_align_to(label_point, label_hour, LV_ALIGN_OUT_RIGHT_MID, 2, 0);

    label_second = lv_label_create(lv_layer_top());
    lv_obj_set_style_text_font(label_second, &MiSans_Regular_16, 0);
    lv_obj_set_style_text_color(label_second, lv_color_make(255, 255, 255), 0);
    lv_obj_set_style_text_opa(label_second, LV_OPA_80, 0);
    lv_label_set_text(label_second, "40");
    lv_obj_align_to(label_second, label_point, LV_ALIGN_OUT_RIGHT_MID, 2, 0);
    
}
void top_layer_create(void)
{
    top_scr_obj_create();

    SCR_FUNC_TYPE *src_ops = malloc(sizeof(SCR_FUNC_TYPE));
    src_ops->load_func = top_layer_load_func;
    src_ops->refr_func = top_layer_refr_func; 
    src_ops->quit_func = top_layer_quit_func;

    if(t_top == NULL)
        t_top = lv_timer_create(refr_src_disp, 500, NULL); 
    lv_timer_pause(t_top);

    scr_add_user_data(top_layer, E_TOP_LAYER, src_ops);
}

