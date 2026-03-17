#include "ui_common.h"

static lv_obj_t *tim_scr;
static int refr_lock = 1;

static void tim_scr_obj_create(void);
static void tim_scr_obj_delete(void);

static lv_obj_t *img_return = NULL;
static lv_obj_t *label_return = NULL;

static lv_obj_t *img_auto = NULL;
static lv_obj_t *label_auto = NULL;
static lv_obj_t *cont_auto = NULL;

static lv_obj_t *img_disinfect = NULL;
static lv_obj_t *label_disinfect = NULL;
static lv_obj_t *cont_disinfect = NULL;

static lv_obj_t *btn_cancel = NULL;
static lv_obj_t *label_cancel = NULL;
static lv_obj_t *btn_determine = NULL;
static lv_obj_t *label_determine = NULL;

static lv_obj_t *cont = NULL;
static lv_obj_t *roller_hour = NULL;
static lv_obj_t *roller_point = NULL;
static lv_obj_t *roller_min = NULL;
static const char * opts_hour = {"00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23"};
static const char * opts_min = {"00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59"};


static int option_sel = 0;

static void tim_scr_load_func(void *arg)
{
    // ... ui_init
    lv_obj_add_flag(lv_layer_top(), LV_OBJ_FLAG_HIDDEN);
    tim_scr_obj_create();
    option_sel = 0;
    refr_lock = 0;
}

static void btn_event_handle(BTN_ID_t which, BTN_STATUS_t status)
{
    switch (which)
    {
    case E_K1:
        if (status == E_BUTTON_SINGLE_CLICK)
            ;
        else
            ;
        break;
    case E_K2:
        if (status == E_BUTTON_SINGLE_CLICK)
            ;
        else
            ;
        break;
    case E_K3:
        if (status == E_BUTTON_SINGLE_CLICK)
            ;
        else
            ;
        break;
    case E_K4:
        if (status == E_BUTTON_SINGLE_CLICK)
            ;
        else
            ;
        break;
    default:
        break;
    }
}

static void tim_scr_refr_func(Event_Data_t *arg)
{
    if(refr_lock)
        return;
    if(arg == NULL)
    {
        
    }
    else
    {
        if(arg->eEventID == E_KEY_EVENT)
            btn_event_handle(arg->lDataArray[0], arg->lDataArray[1]);
    }
}

static void tim_scr_quit_func(void *arg)
{
    refr_lock = 1;
    option_sel = 0;
    // ... ui_del
    tim_scr_obj_delete();
}


static void __return_btn_cb(lv_event_t * e)
{
    scr_load_func(E_HOME_SCR, NULL);
}

static void __cont_auto_cb(lv_event_t * e)
{
    option_sel = 0;
    lv_obj_set_style_border_opa(cont_auto, LV_OPA_100, 0);
    lv_obj_set_style_border_opa(cont_disinfect, LV_OPA_0, 0);
}

static void __cont_disinfect_cb(lv_event_t * e)
{
    option_sel = 1;
    lv_obj_set_style_border_opa(cont_auto, LV_OPA_0, 0);
    lv_obj_set_style_border_opa(cont_disinfect, LV_OPA_100, 0);
}

static void __btn_cancel_cb(lv_event_t * e)
{
    scr_load_func(E_HOME_SCR, NULL);
}

static void __btn_determine_cb(lv_event_t * e)
{
    char buf[64] = {0};

    switch (option_sel)
    {
    case 0: //auto
        E_cur_work_status = E_WORK_TIM_AUTO;
        memset(buf, 0, sizeof(buf));
        lv_roller_get_selected_str(roller_hour, buf, 64);
        user_set_tim_hour(atoi(buf));
        memset(buf, 0, sizeof(buf));
        lv_roller_get_selected_str(roller_min, buf, 64);
        user_set_tim_min(atoi(buf));

        break;
    
    case 1: //disinfect
        E_cur_work_status = E_WORK_TIM_DISINFECT;
        memset(buf, 0, sizeof(buf));
        lv_roller_get_selected_str(roller_hour, buf, 64);
        user_set_tim_hour(atoi(buf));
        memset(buf, 0, sizeof(buf));
        lv_roller_get_selected_str(roller_min, buf, 64);
        user_set_tim_min(atoi(buf));

        break;

    default:
        break;
    }
    scr_load_func(E_HOME_SCR, NULL);
}



static void tim_scr_obj_create(void)
{
    img_auto = lv_img_create(tim_scr);
    lv_img_set_src(img_auto, U_RETURN_IMG);
    lv_obj_align(img_auto, LV_ALIGN_TOP_LEFT, 24, 17);

    label_auto = lv_label_create(tim_scr);
    lv_obj_set_style_text_font(label_auto, &MiSans_Regular_20, 0);
    lv_obj_set_style_text_color(label_auto, lv_color_make(255, 255, 255), 0);
    lv_obj_set_style_text_opa(label_auto, LV_OPA_100, 0);
    lv_label_set_text(label_auto, "返回");
    lv_obj_align_to(label_auto, img_auto, LV_ALIGN_OUT_RIGHT_MID, 7, 0);
    lv_obj_add_flag(label_auto, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(label_auto, __return_btn_cb, LV_EVENT_CLICKED, 0);

    cont = lv_obj_create(tim_scr);
    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, 374, 128);
    lv_obj_set_style_bg_opa(cont, LV_OPA_100, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(cont, lv_color_make(32, 35, 45), LV_STATE_DEFAULT);
    lv_obj_align(cont, LV_ALIGN_CENTER, 0, 0);

    roller_point = lv_label_create(cont);
    lv_obj_set_style_text_font(roller_point, &MiSans_Regular_32, 0);
    lv_obj_set_style_text_color(roller_point, lv_color_make(255, 255, 255), 0);
    lv_obj_set_style_text_opa(roller_point, LV_OPA_100, 0);
    lv_label_set_text(roller_point, ":");
    lv_obj_align(roller_point, LV_ALIGN_CENTER, 0, -5);

    roller_hour = lv_roller_create(cont);
    lv_obj_set_size(roller_hour, 100, 128);
    lv_roller_set_options(roller_hour, opts_hour, LV_ROLLER_MODE_INFINITE);
    lv_roller_set_visible_row_count(roller_hour, 3);
    lv_obj_set_style_text_font(roller_hour, &MiSans_Regular_22, 0);
    lv_obj_set_style_text_font(roller_hour, &MiSans_Regular_32, LV_PART_SELECTED);
    lv_obj_set_style_text_color(roller_hour, lv_color_make(188, 188, 190), 0);
    lv_obj_set_style_text_color(roller_hour, lv_color_make(255, 255, 255), LV_PART_SELECTED);
    lv_obj_set_style_bg_color(roller_hour, lv_color_make(32, 35, 45), 0);
    lv_obj_set_style_bg_color(roller_hour, lv_color_make(32, 35, 45), LV_PART_SELECTED);
    lv_obj_set_style_border_width(roller_hour, 0, 0);
    lv_obj_set_style_border_width(roller_hour, 1, LV_PART_SELECTED);
    lv_obj_set_style_border_color(roller_hour, lv_color_make(84, 85, 87), 0);
    lv_obj_set_style_border_side(roller_hour, LV_BORDER_SIDE_TOP|LV_BORDER_SIDE_BOTTOM, LV_PART_SELECTED);
    lv_obj_set_style_border_opa(roller_hour, LV_OPA_100, LV_PART_SELECTED);
    lv_roller_set_selected(roller_hour, 0, LV_ANIM_OFF);
    lv_obj_align_to(roller_hour, roller_point, LV_ALIGN_OUT_LEFT_MID, 0, 5);

    roller_min = lv_roller_create(cont);
    lv_obj_set_size(roller_min, 100, 128);
    lv_roller_set_options(roller_min, opts_min, LV_ROLLER_MODE_INFINITE);
    lv_roller_set_visible_row_count(roller_min, 3);
    lv_obj_set_style_text_font(roller_min, &MiSans_Regular_22, 0);
    lv_obj_set_style_text_font(roller_min, &MiSans_Regular_32, LV_PART_SELECTED);
    lv_obj_set_style_text_color(roller_min, lv_color_make(188, 188, 190), 0);
    lv_obj_set_style_text_color(roller_min, lv_color_make(255, 255, 255), LV_PART_SELECTED);
    lv_obj_set_style_bg_color(roller_min, lv_color_make(32, 35, 45), 0);
    lv_obj_set_style_bg_color(roller_min, lv_color_make(32, 35, 45), LV_PART_SELECTED);
    lv_obj_set_style_border_width(roller_min, 0, 0);
    lv_obj_set_style_border_width(roller_min, 1, LV_PART_SELECTED);
    lv_obj_set_style_border_color(roller_min, lv_color_make(84, 85, 87), 0);
    lv_obj_set_style_border_side(roller_min, LV_BORDER_SIDE_TOP|LV_BORDER_SIDE_BOTTOM, LV_PART_SELECTED);
    lv_obj_set_style_border_opa(roller_min, LV_OPA_100, LV_PART_SELECTED);
    lv_roller_set_selected(roller_min, 0, LV_ANIM_OFF);
    lv_obj_align_to(roller_min, roller_point, LV_ALIGN_OUT_RIGHT_MID, 0, 5);

    cont_auto = lv_obj_create(tim_scr);
    lv_obj_remove_style_all(cont_auto);
    lv_obj_set_size(cont_auto, 128, 50);
    lv_obj_set_style_bg_opa(cont_auto, LV_OPA_100, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(cont_auto, lv_color_make(0, 0, 0), 0);
    lv_obj_set_style_border_width(cont_auto, 4, 0);
    lv_obj_set_style_border_side(cont_auto, LV_BORDER_SIDE_BOTTOM, 0);
    lv_obj_set_style_border_color(cont_auto, lv_color_make(58, 123, 254), 0);
    lv_obj_set_style_border_opa(cont_auto, LV_OPA_100, 0);
    lv_obj_add_flag(cont_auto, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(cont_auto, __cont_auto_cb, LV_EVENT_CLICKED, 0);
    lv_obj_align_to(cont_auto, cont, LV_ALIGN_OUT_TOP_LEFT, 40, -5);

    img_auto = lv_img_create(cont_auto);
    lv_img_set_src(img_auto, U_AUTO_S_IMG);
    lv_obj_align(img_auto, LV_ALIGN_LEFT_MID, 10, 0);

    label_auto = lv_label_create(cont_auto);
    lv_obj_set_style_text_font(label_auto, &MiSans_Regular_22, 0);
    lv_obj_set_style_text_color(label_auto, lv_color_make(255, 255, 255), 0);
    lv_obj_set_style_text_opa(label_auto, LV_OPA_100, 0);
    lv_label_set_text(label_auto, "自动");
    lv_obj_align_to(label_auto, img_auto, LV_ALIGN_OUT_RIGHT_MID, 5, -1);


    cont_disinfect = lv_obj_create(tim_scr);
    lv_obj_remove_style_all(cont_disinfect);
    lv_obj_set_size(cont_disinfect, 128, 50);
    lv_obj_set_style_bg_opa(cont_disinfect, LV_OPA_100, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(cont_disinfect, lv_color_make(0, 0, 0), 0);
    lv_obj_set_style_border_width(cont_disinfect, 4, 0);
    lv_obj_set_style_border_side(cont_disinfect, LV_BORDER_SIDE_BOTTOM, 0);
    lv_obj_set_style_border_color(cont_disinfect, lv_color_make(58, 123, 254), 0);
    lv_obj_set_style_border_opa(cont_disinfect, LV_OPA_0, 0);
    lv_obj_add_flag(cont_disinfect, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(cont_disinfect, __cont_disinfect_cb, LV_EVENT_CLICKED, 0);
    lv_obj_align_to(cont_disinfect, cont, LV_ALIGN_OUT_TOP_RIGHT, -40, -5);

    img_disinfect = lv_img_create(cont_disinfect);
    lv_img_set_src(img_disinfect, U_DISINFECT_S_IMG);
    lv_obj_align(img_disinfect, LV_ALIGN_LEFT_MID, 20, 0);

    label_disinfect = lv_label_create(cont_disinfect);
    lv_obj_set_style_text_font(label_disinfect, &MiSans_Regular_22, 0);
    lv_obj_set_style_text_color(label_disinfect, lv_color_make(255, 255, 255), 0);
    lv_obj_set_style_text_opa(label_disinfect, LV_OPA_100, 0);
    lv_label_set_text(label_disinfect, "消毒");
    lv_obj_align_to(label_disinfect, img_disinfect, LV_ALIGN_OUT_RIGHT_MID, 10, -2);

    btn_cancel = lv_obj_create(tim_scr);
    lv_obj_remove_style_all(btn_cancel);
    lv_obj_set_size(btn_cancel, 116, 36);
    lv_obj_set_style_bg_opa(btn_cancel, LV_OPA_100, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(btn_cancel, lv_color_make(32, 35, 45), 0);
    lv_obj_set_style_bg_color(btn_cancel, lv_color_make(58, 140, 254), LV_STATE_PRESSED);
    lv_obj_set_style_radius(btn_cancel, 5, 0);
    lv_obj_add_flag(btn_cancel, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(btn_cancel, __btn_cancel_cb, LV_EVENT_CLICKED, 0);
    lv_obj_align_to(btn_cancel, cont, LV_ALIGN_OUT_BOTTOM_LEFT, 50, 15);

    label_cancel = lv_label_create(btn_cancel);
    lv_obj_set_style_text_font(label_cancel, &MiSans_Regular_20, 0);
    lv_obj_set_style_text_color(label_cancel, lv_color_make(255, 255, 255), 0);
    lv_obj_set_style_text_opa(label_cancel, LV_OPA_100, 0);
    lv_label_set_text(label_cancel, "取消");
    lv_obj_align(label_cancel, LV_ALIGN_CENTER, 0, 0);

    btn_determine = lv_obj_create(tim_scr);
    lv_obj_remove_style_all(btn_determine);
    lv_obj_set_size(btn_determine, 116, 36);
    lv_obj_set_style_bg_opa(btn_determine, LV_OPA_100, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(btn_determine, lv_color_make(32, 35, 45), 0);
    lv_obj_set_style_bg_color(btn_determine, lv_color_make(58, 140, 254), LV_STATE_PRESSED);
    lv_obj_set_style_radius(btn_determine, 5, 0);
    lv_obj_add_flag(btn_determine, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(btn_determine, __btn_determine_cb, LV_EVENT_CLICKED, 0);
    lv_obj_align_to(btn_determine, cont, LV_ALIGN_OUT_BOTTOM_RIGHT, -50, 15);

    label_determine = lv_label_create(btn_determine);
    lv_obj_set_style_text_font(label_determine, &MiSans_Regular_20, 0);
    lv_obj_set_style_text_color(label_determine, lv_color_make(255, 255, 255), 0);
    lv_obj_set_style_text_opa(label_determine, LV_OPA_100, 0);
    lv_label_set_text(label_determine, "确定");
    lv_obj_align(label_determine, LV_ALIGN_CENTER, 0, 0);
}

static void tim_scr_obj_delete(void)
{
    printf("tim_scr_obj_delete\n");
    lv_obj_clean(tim_scr);
    lv_obj_clear_flag(lv_layer_top(), LV_OBJ_FLAG_HIDDEN);
}


void tim_scr_create(void)
{
    tim_scr = lv_obj_create(NULL);
    lv_obj_set_size(tim_scr, LV_HOR_RES, LV_VER_RES);
    lv_obj_center(tim_scr);
    lv_obj_set_style_bg_color(tim_scr, lv_color_make(0, 0, 0), 0);
    lv_obj_clear_flag(tim_scr, LV_OBJ_FLAG_SCROLLABLE);


    SCR_FUNC_TYPE *src_ops = malloc(sizeof(SCR_FUNC_TYPE));
    src_ops->load_func = tim_scr_load_func;
    src_ops->refr_func = tim_scr_refr_func;
    src_ops->quit_func = tim_scr_quit_func;

    scr_add_user_data(tim_scr, E_TIM_SCR, src_ops);
}
