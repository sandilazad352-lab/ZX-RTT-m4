#include "ui_common.h"

static lv_obj_t *home_scr;
static int refr_lock = 1;


static lv_obj_t *cont1 = NULL;
static lv_obj_t *cont2 = NULL;
static lv_obj_t *cont3 = NULL;
static lv_obj_t *cont4 = NULL;
/* 自动 */
static lv_obj_t *img_auto = NULL;
static lv_obj_t *label_auto = NULL;
static lv_obj_t *label_auto_eng = NULL;
static lv_obj_t *label_auto_status = NULL;
static lv_obj_t *label_cont1 = NULL;
static lv_obj_t *label_auto_hour = NULL;
static lv_obj_t *label_auto_point = NULL;
static lv_obj_t *label_auto_min = NULL;
/* 消毒 */
static lv_obj_t *img_disinfect = NULL;
static lv_obj_t *label_disinfect = NULL;
static lv_obj_t *label_disinfect_eng = NULL;
static lv_obj_t *label_disinfect_status = NULL;
static lv_obj_t *label_cont2 = NULL;
static lv_obj_t *label_disinfect_hour = NULL;
static lv_obj_t *label_disinfect_point = NULL;
static lv_obj_t *label_disinfect_min = NULL;
/* 烘干 */
static lv_obj_t *img_dry = NULL;
static lv_obj_t *label_dry = NULL;
static lv_obj_t *label_dry_eng = NULL;
static lv_obj_t *label_dry_status = NULL;
static lv_obj_t *label_cont3 = NULL;
static lv_obj_t *label_dry_hour = NULL;
static lv_obj_t *label_dry_point = NULL;
static lv_obj_t *label_dry_min = NULL;
/* 定时 */
static lv_obj_t *img_tim = NULL;
static lv_obj_t *label_tim = NULL;
static lv_obj_t *label_tim_eng = NULL;

static lv_timer_t *t_home = NULL;

static uint32_t work_disinfect_time = 0;    //消毒工作时长（分钟）
static uint32_t work_dry_time = 0;          //烘干工作时长（分钟）
static uint32_t work_disinfect_sec = 0;    //消毒工作时长（秒）
static uint32_t work_dry_sec = 0;          //烘干工作时长（秒）

static uint32_t work_tim_hour = 0;          //定时时间（时）
static uint32_t work_tim_min = 0;           //定时时间（分）

static uint8_t __work_cnt = 0;

static bool __point_flag = true;

static void home_scr_obj_create(void);
static void home_scr_obj_delete(void);

volatile WORK_STATUS_t E_cur_work_status = E_WORK_NONE;

void user_set_tim_hour(uint32_t tim_hour)
{
    work_tim_hour = tim_hour;
}
void user_set_tim_min(uint32_t tim_min)
{
    work_tim_min = tim_min;
}

static void refr_src_disp(void)
{
#if 1
    switch (E_cur_work_status)
    {
    case E_WORK_NONE:
        lv_obj_clear_state(cont1, LV_STATE_CHECKED);
        lv_obj_clear_state(cont2, LV_STATE_CHECKED);
        lv_obj_clear_state(cont3, LV_STATE_CHECKED);

        lv_obj_add_flag(label_auto_status, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_auto_hour, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_auto_point, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_auto_min, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_cont1, LV_OBJ_FLAG_HIDDEN);

        lv_obj_add_flag(label_disinfect_status, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_disinfect_hour, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_disinfect_point, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_disinfect_min, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_cont2, LV_OBJ_FLAG_HIDDEN);

        lv_obj_add_flag(label_dry_status, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_dry_hour, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_dry_point, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_dry_min, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_cont3, LV_OBJ_FLAG_HIDDEN);
        break;
    
    case E_WORK_AUTOMATIC:
        lv_obj_add_state(cont1, LV_STATE_CHECKED);
        lv_label_set_text(label_auto_status, "工作中");
        lv_obj_set_style_text_font(label_auto_hour, &MiSans_Regular_24, 0);
        lv_obj_set_style_text_font(label_auto_point, &MiSans_Regular_24, 0);
        lv_obj_set_style_text_font(label_auto_min, &MiSans_Regular_24, 0);
        lv_label_set_text_fmt(label_auto_hour, "%.2d", (work_disinfect_time + work_dry_time) / 60); // 字号 预约16  工作24
        lv_label_set_text_fmt(label_auto_min, "%.2d", work_disinfect_time + work_dry_time); // 字号 预约16  工作24
        lv_obj_set_style_text_opa(label_auto_hour, LV_OPA_100, 0);
        lv_obj_set_style_text_opa(label_auto_point, LV_OPA_100, 0);
        lv_obj_set_style_text_opa(label_auto_min, LV_OPA_100, 0);
        if(__point_flag)
            lv_label_set_text(label_auto_point, ":"); // 字号 预约16  工作24
        else
            lv_label_set_text(label_auto_point, " "); // 字号 预约16  工作24
        lv_obj_align_to(label_auto_hour, label_auto_status, LV_ALIGN_OUT_BOTTOM_LEFT, -12, 10);
        lv_obj_align_to(label_auto_point, label_auto_hour, LV_ALIGN_OUT_RIGHT_MID, 2, -2);
        lv_obj_align_to(label_auto_min, label_auto_hour, LV_ALIGN_OUT_RIGHT_MID, 11, 0);

        lv_obj_clear_flag(label_auto_status, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(label_auto_hour, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(label_auto_point, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(label_auto_min, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_cont1, LV_OBJ_FLAG_HIDDEN);

        lv_obj_add_flag(label_disinfect_status, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_disinfect_hour, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_disinfect_point, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_disinfect_min, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_cont2, LV_OBJ_FLAG_HIDDEN);

        lv_obj_add_flag(label_dry_status, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_dry_hour, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_dry_point, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_dry_min, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_cont3, LV_OBJ_FLAG_HIDDEN);

        lv_obj_clear_state(cont2, LV_STATE_CHECKED);
        lv_obj_clear_state(cont3, LV_STATE_CHECKED);
        break;
    
    case E_WORK_DISINFECT:
        lv_obj_add_state(cont2, LV_STATE_CHECKED);
        lv_label_set_text(label_disinfect_status, "工作中");
        lv_obj_set_style_text_font(label_disinfect_hour, &MiSans_Regular_24, 0);
        lv_obj_set_style_text_font(label_disinfect_point, &MiSans_Regular_24, 0);
        lv_obj_set_style_text_font(label_disinfect_min, &MiSans_Regular_24, 0);
        lv_label_set_text_fmt(label_disinfect_hour, "%.2d", work_disinfect_time / 60); // 字号 预约16  工作24
        lv_label_set_text_fmt(label_disinfect_min, "%.2d", work_disinfect_time); // 字号 预约16  工作24
        lv_obj_set_style_text_opa(label_disinfect_hour, LV_OPA_100, 0);
        lv_obj_set_style_text_opa(label_disinfect_point, LV_OPA_100, 0);
        lv_obj_set_style_text_opa(label_disinfect_min, LV_OPA_100, 0);
        if(__point_flag)
            lv_label_set_text(label_disinfect_point, ":"); // 字号 预约16  工作24
        else
            lv_label_set_text(label_disinfect_point, " "); // 字号 预约16  工作24
        lv_obj_align_to(label_disinfect_hour, label_disinfect_status, LV_ALIGN_OUT_BOTTOM_LEFT, -12, 10);
        lv_obj_align_to(label_disinfect_point, label_disinfect_hour, LV_ALIGN_OUT_RIGHT_MID, 2, -2);
        lv_obj_align_to(label_disinfect_min, label_disinfect_hour, LV_ALIGN_OUT_RIGHT_MID, 11, 0);
        
        lv_obj_add_flag(label_auto_status, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_auto_hour, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_auto_point, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_auto_min, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_cont1, LV_OBJ_FLAG_HIDDEN);

        lv_obj_clear_flag(label_disinfect_status, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(label_disinfect_hour, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(label_disinfect_point, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(label_disinfect_min, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_cont2, LV_OBJ_FLAG_HIDDEN);

        lv_obj_add_flag(label_dry_status, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_dry_hour, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_dry_point, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_dry_min, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_cont3, LV_OBJ_FLAG_HIDDEN);

        lv_obj_clear_state(cont1, LV_STATE_CHECKED);
        lv_obj_clear_state(cont3, LV_STATE_CHECKED);
        break;
    
    case E_WORK_DRYING:
        lv_obj_add_state(cont3, LV_STATE_CHECKED);
        lv_label_set_text(label_dry_status, "工作中");
        lv_obj_set_style_text_font(label_dry_hour, &MiSans_Regular_20, 0);
        lv_obj_set_style_text_font(label_dry_point, &MiSans_Regular_20, 0);
        lv_obj_set_style_text_font(label_dry_min, &MiSans_Regular_20, 0);
        lv_label_set_text(label_dry_hour, "00"); // 字号 预约16  工作24
        lv_label_set_text(label_dry_min, "18"); // 字号 预约16  工作24
        lv_label_set_text_fmt(label_dry_hour, "%.2d", work_dry_time / 60); // 字号 预约16  工作24
        lv_label_set_text_fmt(label_dry_min, "%.2d", work_dry_time); // 字号 预约16  工作24
        lv_obj_set_style_text_opa(label_dry_hour, LV_OPA_100, 0);
        lv_obj_set_style_text_opa(label_dry_point, LV_OPA_100, 0);
        lv_obj_set_style_text_opa(label_dry_min, LV_OPA_100, 0);
        if(__point_flag)
            lv_label_set_text(label_dry_point, ":"); // 字号 预约16  工作24
        else
            lv_label_set_text(label_dry_point, " "); // 字号 预约16  工作24
        lv_obj_align(label_dry_hour, LV_ALIGN_TOP_RIGHT, -45, 10);
        lv_obj_align_to(label_dry_point, label_dry_hour, LV_ALIGN_OUT_RIGHT_MID, 2, -2);
        lv_obj_align_to(label_dry_min, label_dry_hour, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
        
        lv_obj_add_flag(label_auto_status, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_auto_hour, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_auto_point, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_auto_min, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_cont1, LV_OBJ_FLAG_HIDDEN);

        lv_obj_add_flag(label_disinfect_status, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_disinfect_hour, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_disinfect_point, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_disinfect_min, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_cont2, LV_OBJ_FLAG_HIDDEN);

        lv_obj_clear_flag(label_dry_status, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(label_dry_hour, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(label_dry_point, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(label_dry_min, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_cont3, LV_OBJ_FLAG_HIDDEN);

        lv_obj_clear_state(cont1, LV_STATE_CHECKED);
        lv_obj_clear_state(cont2, LV_STATE_CHECKED);
        break;
    
    case E_WORK_TIM_AUTO:
        lv_label_set_text(label_auto_status, "已预约");
        lv_obj_clear_flag(label_cont1, LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_style_text_font(label_auto_hour, &MiSans_Regular_16, 0);
        lv_obj_set_style_text_font(label_auto_point, &MiSans_Regular_16, 0);
        lv_obj_set_style_text_font(label_auto_min, &MiSans_Regular_16, 0);
        lv_label_set_text_fmt(label_auto_hour, "%.2d", work_tim_hour); // 字号 预约16  工作24
        lv_label_set_text_fmt(label_auto_min, "%.2d", work_tim_min); // 字号 预约16  工作24
        lv_obj_set_style_text_opa(label_auto_hour, LV_OPA_60, 0);
        lv_obj_set_style_text_opa(label_auto_point, LV_OPA_60, 0);
        lv_obj_set_style_text_opa(label_auto_min, LV_OPA_60, 0);
        lv_label_set_text(label_auto_point, ":"); // 字号 预约16  工作24
        lv_obj_align_to(label_auto_hour, label_cont1, LV_ALIGN_LEFT_MID, 7, 0);    //预约
        lv_obj_align_to(label_auto_point, label_auto_hour, LV_ALIGN_OUT_RIGHT_MID, 3, -1);
        lv_obj_align_to(label_auto_min, label_auto_hour, LV_ALIGN_OUT_RIGHT_MID, 11, 0);

        lv_obj_clear_flag(label_auto_status, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(label_auto_hour, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(label_auto_point, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(label_auto_min, LV_OBJ_FLAG_HIDDEN);

        lv_obj_add_flag(label_disinfect_status, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_disinfect_hour, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_disinfect_point, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_disinfect_min, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_cont2, LV_OBJ_FLAG_HIDDEN);

        lv_obj_add_flag(label_dry_status, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_dry_hour, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_dry_point, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_dry_min, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_cont3, LV_OBJ_FLAG_HIDDEN);

        lv_obj_clear_state(cont1, LV_STATE_CHECKED);
        lv_obj_clear_state(cont2, LV_STATE_CHECKED);
        lv_obj_clear_state(cont3, LV_STATE_CHECKED);

        break;

    case E_WORK_TIM_DISINFECT:
        lv_label_set_text(label_disinfect_status, "已预约");
        lv_obj_set_style_text_font(label_disinfect_hour, &MiSans_Regular_16, 0);
        lv_obj_set_style_text_font(label_disinfect_point, &MiSans_Regular_16, 0);
        lv_obj_set_style_text_font(label_disinfect_min, &MiSans_Regular_16, 0);
        lv_label_set_text_fmt(label_disinfect_hour, "%.2d", work_tim_hour); // 字号 预约16  工作24
        lv_label_set_text(label_disinfect_point, ":"); // 字号 预约16  工作24
        lv_label_set_text_fmt(label_disinfect_min, "%.2d", work_tim_min); // 字号 预约16  工作24
        lv_obj_set_style_text_opa(label_disinfect_hour, LV_OPA_60, 0);
        lv_obj_set_style_text_opa(label_disinfect_point, LV_OPA_60, 0);
        lv_obj_set_style_text_opa(label_disinfect_min, LV_OPA_60, 0);

        lv_obj_align_to(label_disinfect_hour, label_cont2, LV_ALIGN_LEFT_MID, 7, 0);    //预约
        lv_obj_align_to(label_disinfect_point, label_disinfect_hour, LV_ALIGN_OUT_RIGHT_MID, 3, -1);
        lv_obj_align_to(label_disinfect_min, label_disinfect_hour, LV_ALIGN_OUT_RIGHT_MID, 11, 0);
        
        lv_obj_add_flag(label_auto_status, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_auto_hour, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_auto_point, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_auto_min, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_cont1, LV_OBJ_FLAG_HIDDEN);

        lv_obj_clear_flag(label_disinfect_status, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(label_disinfect_hour, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(label_disinfect_point, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(label_disinfect_min, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(label_cont2, LV_OBJ_FLAG_HIDDEN);

        lv_obj_add_flag(label_dry_status, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_dry_hour, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_dry_point, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_dry_min, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(label_cont3, LV_OBJ_FLAG_HIDDEN);

        lv_obj_clear_state(cont1, LV_STATE_CHECKED);
        lv_obj_clear_state(cont2, LV_STATE_CHECKED);
        lv_obj_clear_state(cont3, LV_STATE_CHECKED);
        break;

    default:
        break;
    }
#endif
}


static void home_scr_load_func(void *arg)
{
    // ... ui_init
    home_scr_obj_create();
    refr_src_disp();
    if(E_cur_work_status != E_WORK_NONE)
    {
        __point_flag = true;
        lv_timer_resume(t_home);
        lv_timer_ready(t_home);
    }
    refr_lock = 0;
}

static void btn_event_handle(BTN_ID_t which, BTN_STATUS_t status)
{
    switch (which)
    {
    case E_K4:
        if (status == E_BUTTON_SINGLE_CLICK)
        {

        }
        break;
    case E_K1:
        if (status == E_BUTTON_SINGLE_CLICK)
        {

        }
        break;
    case E_K2:
        if (status == E_BUTTON_SINGLE_CLICK)
        {

        }
        break;
    case E_K3:
        if (status == E_BUTTON_SINGLE_CLICK)
    
        break;
    default:
        break;
    }
}

//当前屏幕事件处理函数
static void home_scr_refr_func(Event_Data_t *arg)
{
    if(refr_lock)
        return;
    if(arg == NULL)
    {
        // refr_src_disp();
    }
    else
    {
        if(arg->eEventID == E_KEY_EVENT)
            btn_event_handle(arg->lDataArray[0], arg->lDataArray[1]);
    }
}

static void home_scr_quit_func(void *arg)
{
    refr_lock = 1;
    // ... ui_del
    home_scr_obj_delete();
}

//自动
static void __event_auto_cb(lv_event_t * e)
{
    __point_flag = true;
    if(E_cur_work_status != E_WORK_AUTOMATIC)
    {
        E_cur_work_status = E_WORK_AUTOMATIC;
        work_disinfect_time = 12;   //消毒 12min
        work_dry_time = 18;         //烘干 18min
        work_disinfect_time %=  60;
        work_dry_time %= 60;
        work_disinfect_sec = work_disinfect_time * 60;
        work_dry_sec = work_dry_time * 18;
        __work_cnt = 0;
        lv_timer_resume(t_home);
        lv_timer_ready(t_home);
    }else{
        E_cur_work_status = E_WORK_NONE;
        work_disinfect_time = 0;
        work_dry_time = 0;
        lv_timer_pause(t_home);
    }
    refr_src_disp();
}
//消毒
static void __event_disinfect_cb(lv_event_t * e)
{
    __point_flag = true;
    if(E_cur_work_status != E_WORK_DISINFECT)
    {
        E_cur_work_status = E_WORK_DISINFECT;
        work_disinfect_time = 12;   //消毒 12min
        work_dry_time = 0;         //烘干
        work_disinfect_time %=  60;
        work_dry_time %= 60;
        work_disinfect_sec = work_disinfect_time * 60;
        work_dry_sec = work_dry_time * 18;
        __work_cnt = 0;
        lv_timer_resume(t_home);
        lv_timer_ready(t_home);
    }else{
        E_cur_work_status = E_WORK_NONE;
        work_disinfect_time = 0;
        work_dry_time = 0;
        lv_timer_pause(t_home);
    }
    refr_src_disp();
}
//烘干
static void __event_dry_cb(lv_event_t * e)
{
    __point_flag = true;
    if(E_cur_work_status != E_WORK_DRYING)
    {
        E_cur_work_status = E_WORK_DRYING;
        work_disinfect_time = 0;        //消毒 
        work_dry_time = 18;         //烘干 18min
        work_disinfect_time %=  60;
        work_dry_time %= 60;
        work_disinfect_sec = work_disinfect_time * 60;
        work_dry_sec = work_dry_time * 18;
        __work_cnt = 0;
        lv_timer_resume(t_home);
        lv_timer_ready(t_home);
    }else{
        E_cur_work_status = E_WORK_NONE;
        work_disinfect_time = 0;
        work_dry_time = 0;
        lv_timer_pause(t_home);
    }
    refr_src_disp();
}

//定时
static void __event_tim_cb(lv_event_t * e)
{
    lv_timer_pause(t_home);
    scr_load_func(E_TIM_SCR, NULL);
}

//创建当前屏幕 子对象
static void home_scr_obj_create(void)
{
//Automatic
    cont1 = lv_obj_create(home_scr);
    lv_obj_remove_style_all(cont1);
    lv_obj_set_size(cont1, 140, 217);
    lv_obj_set_style_radius(cont1, 5, 0);
    lv_obj_set_style_bg_opa(cont1, LV_OPA_100, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(cont1, lv_color_make(32, 35, 45), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(cont1, lv_color_make(57, 105, 253), LV_STATE_CHECKED);
    lv_obj_set_pos(cont1, 16, 41);
    lv_obj_add_flag(cont1, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(cont1, __event_auto_cb, LV_EVENT_CLICKED, NULL);

    img_auto = lv_img_create(cont1);
    lv_img_set_src(img_auto, U_AUTO_IMG);
    lv_obj_align(img_auto, LV_ALIGN_CENTER, 0, 5);

    label_auto = lv_label_create(cont1);
    lv_obj_set_style_text_font(label_auto, &MiSans_Regular_22, 0);
    lv_obj_set_style_text_color(label_auto, lv_color_make(255, 255, 255), 0);
    lv_obj_set_style_text_opa(label_auto, LV_OPA_80, 0);
    lv_label_set_text(label_auto, "自动");
    lv_obj_align(label_auto, LV_ALIGN_BOTTOM_LEFT, 15, -32);

    label_auto_eng = lv_label_create(cont1);
    lv_obj_set_style_text_font(label_auto_eng, &MiSans_Regular_14, 0);
    lv_obj_set_style_text_color(label_auto_eng, lv_color_make(255, 255, 255), 0);
    lv_obj_set_style_text_opa(label_auto_eng, LV_OPA_40, 0);
    lv_label_set_text(label_auto_eng, "Automatic");
    lv_obj_align(label_auto_eng, LV_ALIGN_BOTTOM_LEFT, 15, -10);

    label_auto_status = lv_label_create(cont1);
    lv_obj_set_style_text_font(label_auto_status, &MiSans_Regular_14, 0);
    lv_obj_set_style_text_color(label_auto_status, lv_color_make(255, 255, 255), 0);
    lv_obj_set_style_text_opa(label_auto_status, LV_OPA_60, 0);
    lv_label_set_text(label_auto_status, "已预约");
    lv_obj_align(label_auto_status, LV_ALIGN_TOP_MID, 0, 12);
    // lv_obj_add_flag(label_auto_status, LV_OBJ_FLAG_HIDDEN);

    label_cont1 = lv_obj_create(cont1);
    lv_obj_remove_style_all(label_cont1);
    lv_obj_set_size(label_cont1, 65, 25);
    lv_obj_set_style_outline_pad(label_cont1, 0, 0);
    lv_obj_set_style_outline_width(label_cont1, 2, 0);
    lv_obj_set_style_outline_color(label_cont1, lv_color_make(59, 64, 78), 0);
    lv_obj_set_style_outline_opa(label_cont1, LV_OPA_100, 0); 
    lv_obj_set_style_radius(label_cont1, 12, LV_PART_MAIN);
    lv_obj_align(label_cont1, LV_ALIGN_TOP_MID, 0, 34);
    // lv_obj_add_flag(label_cont1, LV_OBJ_FLAG_HIDDEN);

    label_auto_hour = lv_label_create(cont1);
    // lv_obj_set_style_clip_corner(label_auto_hour, false, 0);
    // lv_obj_set_style_text_font(label_auto_hour, &MiSans_Regular_16, 0);
    lv_obj_set_style_text_font(label_auto_hour, &MiSans_Regular_24, 0);
    lv_obj_set_style_text_color(label_auto_hour, lv_color_make(255, 255, 255), 0);
    lv_obj_set_style_text_opa(label_auto_hour, LV_OPA_60, 0);
    lv_obj_set_style_text_align(label_auto_hour, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_text(label_auto_hour, "00"); // 字号 预约16  工作24
    // lv_obj_align_to(label_auto_hour, label_cont1, LV_ALIGN_LEFT_MID, 12, 0);    //预约
    lv_obj_align_to(label_auto_hour, label_auto_status, LV_ALIGN_OUT_BOTTOM_LEFT, -8, 0);
    // lv_obj_add_flag(label_auto_hour, LV_OBJ_FLAG_HIDDEN);

    label_auto_point = lv_label_create(cont1);
    // lv_obj_set_style_clip_corner(label_auto_point, false, 0);
    lv_obj_remove_style_all(label_auto_point);
    // lv_obj_set_style_text_font(label_auto_point, &MiSans_Regular_16, 0);
    lv_obj_set_style_text_font(label_auto_point, &MiSans_Regular_24, 0);
    lv_obj_set_style_text_color(label_auto_point, lv_color_make(255, 255, 255), 0);
    lv_obj_set_style_text_opa(label_auto_point, LV_OPA_60, 0);
    lv_obj_set_style_text_align(label_auto_point, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_text(label_auto_point, ":"); // 字号 预约16  工作24
    lv_obj_align_to(label_auto_point, label_auto_hour, LV_ALIGN_OUT_RIGHT_MID, 2, -2);
    // lv_obj_add_flag(label_auto_point, LV_OBJ_FLAG_HIDDEN);

    label_auto_min = lv_label_create(cont1);
    lv_obj_remove_style_all(label_auto_min);
    // lv_obj_set_style_text_font(label_auto_min, &MiSans_Regular_16, 0);
    lv_obj_set_style_text_font(label_auto_min, &MiSans_Regular_24, 0);
    lv_obj_set_style_text_color(label_auto_min, lv_color_make(255, 255, 255), 0);
    lv_obj_set_style_text_opa(label_auto_min, LV_OPA_60, 0);
    lv_obj_set_style_text_align(label_auto_min, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_text(label_auto_min, "30"); // 字号 预约16  工作24
    lv_obj_align_to(label_auto_min, label_auto_hour, LV_ALIGN_OUT_RIGHT_MID, 11, 0);
    // lv_obj_add_flag(label_auto_min, LV_OBJ_FLAG_HIDDEN);

//Disinfect
    cont2 = lv_obj_create(home_scr);
    lv_obj_remove_style_all(cont2);
    lv_obj_set_size(cont2, 140, 217);
    lv_obj_set_style_radius(cont2, 5, 0);
    lv_obj_set_style_bg_opa(cont2, LV_OPA_100, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(cont2, lv_color_make(32, 35, 45), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(cont2, lv_color_make(57, 105, 253), LV_STATE_CHECKED);
    lv_obj_align_to(cont2, cont1, LV_ALIGN_OUT_RIGHT_MID, 14, 0);
    // lv_obj_add_state(cont2, LV_STATE_CHECKED);
    lv_obj_add_flag(cont2, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(cont2, __event_disinfect_cb, LV_EVENT_CLICKED, NULL);

    img_disinfect = lv_img_create(cont2);
    lv_img_set_src(img_disinfect, U_DISINFECT_IMG);
    lv_obj_align(img_disinfect, LV_ALIGN_CENTER, 0, 24);

    label_disinfect = lv_label_create(cont2);
    lv_obj_set_style_text_font(label_disinfect, &MiSans_Regular_22, 0);
    lv_obj_set_style_text_color(label_disinfect, lv_color_make(255, 255, 255), 0);
    lv_obj_set_style_text_opa(label_disinfect, LV_OPA_80, 0);
    lv_label_set_text(label_disinfect, "消毒");
    lv_obj_align(label_disinfect, LV_ALIGN_BOTTOM_LEFT, 15, -32);

    label_disinfect_eng = lv_label_create(cont2);
    lv_obj_set_style_text_font(label_disinfect_eng, &MiSans_Regular_14, 0);
    lv_obj_set_style_text_color(label_disinfect_eng, lv_color_make(255, 255, 255), 0);
    lv_obj_set_style_text_opa(label_disinfect_eng, LV_OPA_40, 0);
    lv_label_set_text(label_disinfect_eng, "Disinfect");
    lv_obj_align(label_disinfect_eng, LV_ALIGN_BOTTOM_LEFT, 15, -10);

    label_disinfect_status = lv_label_create(cont2);
    lv_obj_set_style_text_font(label_disinfect_status, &MiSans_Regular_14, 0);
    lv_obj_set_style_text_color(label_disinfect_status, lv_color_make(255, 255, 255), 0);
    lv_obj_set_style_text_opa(label_disinfect_status, LV_OPA_60, 0);
    lv_label_set_text(label_disinfect_status, "工作中");
    lv_obj_align(label_disinfect_status, LV_ALIGN_TOP_MID, 0, 12);
    // lv_obj_add_flag(label_disinfect_status, LV_OBJ_FLAG_HIDDEN);

    label_cont2 = lv_obj_create(cont2);
    lv_obj_remove_style_all(label_cont2);
    lv_obj_set_size(label_cont2, 65, 25);
    lv_obj_set_style_outline_pad(label_cont2, 0, 0);
    lv_obj_set_style_outline_width(label_cont2, 2, 0);
    lv_obj_set_style_outline_color(label_cont2, lv_color_make(59, 64, 78), 0);
    lv_obj_set_style_outline_opa(label_cont2, LV_OPA_100, 0); 
    lv_obj_set_style_radius(label_cont2, 12, LV_PART_MAIN);
    lv_obj_align(label_cont2, LV_ALIGN_TOP_MID, 0, 34);
    lv_obj_add_flag(label_cont2, LV_OBJ_FLAG_HIDDEN);

    label_disinfect_hour = lv_label_create(cont2);
    // lv_obj_set_style_text_font(label_disinfect_hour, &MiSans_Regular_16, 0);
    lv_obj_set_style_text_font(label_disinfect_hour, &MiSans_Regular_24, 0);
    lv_obj_set_style_text_color(label_disinfect_hour, lv_color_make(255, 255, 255), 0);
    lv_obj_set_style_text_opa(label_disinfect_hour, LV_OPA_60, 0);
    lv_obj_set_style_text_align(label_disinfect_hour, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_text(label_disinfect_hour, "00"); // 字号 预约16  工作24
    // lv_obj_align_to(label_disinfect_hour, label_cont2, LV_ALIGN_LEFT_MID, 12, 0);    //预约
    lv_obj_align_to(label_disinfect_hour, label_disinfect_status, LV_ALIGN_OUT_BOTTOM_LEFT, -8, 0);
    // lv_obj_add_flag(label_disinfect_hour, LV_OBJ_FLAG_HIDDEN);

    label_disinfect_point = lv_label_create(cont2);
    lv_obj_remove_style_all(label_disinfect_point);
    // lv_obj_set_style_text_font(label_disinfect_point, &MiSans_Regular_16, 0);
    lv_obj_set_style_text_font(label_disinfect_point, &MiSans_Regular_24, 0);
    lv_obj_set_style_text_color(label_disinfect_point, lv_color_make(255, 255, 255), 0);
    lv_obj_set_style_text_opa(label_disinfect_point, LV_OPA_60, 0);
    lv_obj_set_style_text_align(label_disinfect_point, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_text(label_disinfect_point, ":"); // 字号 预约16  工作24
    lv_obj_align_to(label_disinfect_point, label_disinfect_hour, LV_ALIGN_OUT_RIGHT_MID, 2, -2);
    // lv_obj_add_flag(label_disinfect_point, LV_OBJ_FLAG_HIDDEN);

    label_disinfect_min = lv_label_create(cont2);
    lv_obj_remove_style_all(label_disinfect_min);
    // lv_obj_set_style_text_font(label_disinfect_min, &MiSans_Regular_16, 0);
    lv_obj_set_style_text_font(label_disinfect_min, &MiSans_Regular_24, 0);
    lv_obj_set_style_text_color(label_disinfect_min, lv_color_make(255, 255, 255), 0);
    lv_obj_set_style_text_opa(label_disinfect_min, LV_OPA_60, 0);
    lv_obj_set_style_text_align(label_disinfect_min, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_text(label_disinfect_min, "30"); // 字号 预约16  工作24
    lv_obj_align_to(label_disinfect_min, label_disinfect_hour, LV_ALIGN_OUT_RIGHT_MID, 11, 0);
    // lv_obj_add_flag(label_disinfect_min, LV_OBJ_FLAG_HIDDEN);

// Drying
    cont3 = lv_obj_create(home_scr);
    lv_obj_remove_style_all(cont3);
    lv_obj_set_size(cont3, 140, 103);
    lv_obj_set_style_radius(cont3, 5, 0);
    lv_obj_set_style_bg_opa(cont3, LV_OPA_100, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(cont3, lv_color_make(32, 35, 45), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(cont3, lv_color_make(57, 105, 253), LV_STATE_CHECKED);
    lv_obj_align_to(cont3, cont2, LV_ALIGN_OUT_RIGHT_TOP, 14, 0);
    lv_obj_add_flag(cont3, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(cont3, __event_dry_cb, LV_EVENT_CLICKED, NULL);

    img_dry = lv_img_create(cont3);
    lv_img_set_src(img_dry, U_DRYING_IMG);
    lv_obj_align(img_dry, LV_ALIGN_TOP_RIGHT, -17, 55);

    label_dry = lv_label_create(cont3);
    lv_obj_set_style_text_font(label_dry, &MiSans_Regular_22, 0);
    lv_obj_set_style_text_color(label_dry, lv_color_make(255, 255, 255), 0);
    lv_obj_set_style_text_opa(label_dry, LV_OPA_80, 0);
    lv_label_set_text(label_dry, "烘干");
    lv_obj_align(label_dry, LV_ALIGN_TOP_LEFT, 15, 48);

    label_dry_eng = lv_label_create(cont3);
    lv_obj_set_style_text_font(label_dry_eng, &MiSans_Regular_14, 0);
    lv_obj_set_style_text_color(label_dry_eng, lv_color_make(255, 255, 255), 0);
    lv_obj_set_style_text_opa(label_dry_eng, LV_OPA_40, 0);
    lv_label_set_text(label_dry_eng, "Drying");
    lv_obj_align(label_dry_eng, LV_ALIGN_TOP_LEFT, 15, 75);


    label_dry_status = lv_label_create(cont3);
    lv_obj_set_style_text_font(label_dry_status, &MiSans_Regular_14, 0);
    lv_obj_set_style_text_color(label_dry_status, lv_color_make(255, 255, 255), 0);
    lv_obj_set_style_text_opa(label_dry_status, LV_OPA_60, 0);
    lv_label_set_text(label_dry_status, "工作中");
    lv_obj_align(label_dry_status, LV_ALIGN_TOP_LEFT, 15, 15);
    // lv_obj_add_flag(label_dry_status, LV_OBJ_FLAG_HIDDEN);

    label_cont3 = lv_obj_create(cont3);
    lv_obj_remove_style_all(label_cont3);
    lv_obj_set_size(label_cont3, 65, 25);
    lv_obj_set_style_outline_pad(label_cont3, 0, 0);
    lv_obj_set_style_outline_width(label_cont3, 2, 0);
    lv_obj_set_style_outline_color(label_cont3, lv_color_make(59, 64, 78), 0);
    lv_obj_set_style_outline_opa(label_cont3, LV_OPA_100, 0); 
    lv_obj_set_style_radius(label_cont3, 12, LV_PART_MAIN);
    lv_obj_align(label_cont3, LV_ALIGN_TOP_RIGHT, -5, 10);
    // lv_obj_add_flag(label_cont3, LV_OBJ_FLAG_HIDDEN);

    label_dry_hour = lv_label_create(cont3);
    lv_obj_set_style_text_font(label_dry_hour, &MiSans_Regular_16, 0);
    // lv_obj_set_style_text_font(label_dry_hour, &MiSans_Regular_20, 0);
    lv_obj_set_style_text_color(label_dry_hour, lv_color_make(255, 255, 255), 0);
    lv_obj_set_style_text_opa(label_dry_hour, LV_OPA_60, 0);
    lv_obj_set_style_text_align(label_dry_hour, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_text(label_dry_hour, "8min"); // 字号 预约16  工作24
    // lv_obj_align(label_dry_hour, LV_ALIGN_TOP_RIGHT, -45, 10);
    lv_obj_align_to(label_dry_hour, label_cont3, LV_ALIGN_CENTER, 0, 0);
    // lv_obj_add_flag(label_dry_hour, LV_OBJ_FLAG_HIDDEN);

    label_dry_point = lv_label_create(cont3);
    lv_obj_remove_style_all(label_dry_point);
    // lv_obj_set_style_text_font(label_dry_point, &MiSans_Regular_16, 0);
    lv_obj_set_style_text_font(label_dry_point, &MiSans_Regular_20, 0);
    lv_obj_set_style_text_color(label_dry_point, lv_color_make(255, 255, 255), 0);
    lv_obj_set_style_text_opa(label_dry_point, LV_OPA_60, 0);
    lv_obj_set_style_text_align(label_dry_point, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_text(label_dry_point, ":"); // 字号 预约16  工作24
    lv_obj_align_to(label_dry_point, label_dry_hour, LV_ALIGN_OUT_RIGHT_MID, 2, -2);
    lv_obj_add_flag(label_dry_point, LV_OBJ_FLAG_HIDDEN);

    label_dry_min = lv_label_create(cont3);
    lv_obj_remove_style_all(label_dry_min);
    // lv_obj_set_style_text_font(label_dry_min, &MiSans_Regular_16, 0);
    lv_obj_set_style_text_font(label_dry_min, &MiSans_Regular_20, 0);
    lv_obj_set_style_text_color(label_dry_min, lv_color_make(255, 255, 255), 0);
    lv_obj_set_style_text_opa(label_dry_min, LV_OPA_60, 0);
    lv_obj_set_style_text_align(label_dry_min, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_text(label_dry_min, "30"); // 字号 预约16  工作24
    lv_obj_align_to(label_dry_min, label_dry_hour, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
    lv_obj_add_flag(label_dry_min, LV_OBJ_FLAG_HIDDEN);

// timing
    cont4 = lv_obj_create(home_scr);
    lv_obj_remove_style_all(cont4);
    lv_obj_set_size(cont4, 140, 103);
    lv_obj_set_style_radius(cont4, 5, 0);
    lv_obj_set_style_bg_opa(cont4, LV_OPA_100, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(cont4, lv_color_make(32, 35, 45), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(cont4, lv_color_make(57, 105, 253), LV_STATE_CHECKED);
    lv_obj_align_to(cont4, cont2, LV_ALIGN_OUT_RIGHT_BOTTOM, 14, 0);
    lv_obj_add_flag(cont4, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(cont4, __event_tim_cb, LV_EVENT_CLICKED, NULL);

    img_tim = lv_img_create(cont4);
    lv_img_set_src(img_tim, U_TIM_IMG);
    lv_obj_align(img_tim, LV_ALIGN_TOP_RIGHT, -17, 40);

    label_tim = lv_label_create(cont4);
    lv_obj_set_style_text_font(label_tim, &MiSans_Regular_22, 0);
    lv_obj_set_style_text_color(label_tim, lv_color_make(255, 255, 255), 0);
    lv_obj_set_style_text_opa(label_tim, LV_OPA_80, 0);
    lv_label_set_text(label_tim, "定时");
    lv_obj_align(label_tim, LV_ALIGN_TOP_LEFT, 15, 40);

    label_tim_eng = lv_label_create(cont4);
    lv_obj_set_style_text_font(label_tim_eng, &MiSans_Regular_14, 0);
    lv_obj_set_style_text_color(label_tim_eng, lv_color_make(255, 255, 255), 0);
    lv_obj_set_style_text_opa(label_tim_eng, LV_OPA_40, 0);
    lv_label_set_text(label_tim_eng, "Timing");
    lv_obj_align(label_tim_eng, LV_ALIGN_TOP_LEFT, 15, 67);

}

static void home_scr_obj_delete(void)
{
    printf("home_scr_obj_delete\n");

    lv_obj_clean(home_scr);
}

void t_home_cb(struct _lv_timer_t *timer)
{
    if(__point_flag)
        __point_flag = false;
    else
        __point_flag = true;

    if(__work_cnt++ == 2)
    {
        __work_cnt = 0;
        switch (E_cur_work_status)
        {
        case E_WORK_AUTOMATIC:
            if(work_disinfect_sec > 0)
            {
                work_disinfect_sec--;
                if(work_disinfect_sec % 60 == 0)
                    work_disinfect_time--;
            }else if(work_dry_sec > 0){
                work_dry_sec--;
                if(work_dry_sec % 60 == 0)
                    work_dry_time--;
            }
            if(work_disinfect_time + work_dry_time == 0)
            {
                E_cur_work_status = E_WORK_NONE;
                work_disinfect_time = 0;
                work_dry_time = 0;
                lv_timer_pause(t_home);
            }
            break;
            
        case E_WORK_DISINFECT:
            if(work_disinfect_sec > 0)
            {
                work_disinfect_sec--;
                if(work_disinfect_sec % 60 == 0)
                    work_disinfect_time--;
            }
            if(work_disinfect_time == 0)
            {
                E_cur_work_status = E_WORK_NONE;
                work_disinfect_time = 0;
                work_dry_time = 0;
                lv_timer_pause(t_home);
            }
            break;

        case E_WORK_DRYING:
            if(work_dry_sec > 0){
                work_dry_sec--;
                if(work_dry_sec % 60 == 0)
                    work_dry_time--;
            }
            if(work_dry_time == 0)
            {
                E_cur_work_status = E_WORK_NONE;
                work_disinfect_time = 0;
                work_dry_time = 0;
                lv_timer_pause(t_home);
            }
            break;

        default:
            break;
        }
    }
    refr_src_disp();
}

void home_scr_create(void)
{
    home_scr = lv_obj_create(NULL);
    lv_obj_set_size(home_scr, LV_HOR_RES, LV_VER_RES);
    lv_obj_center(home_scr);
    lv_obj_set_style_bg_color(home_scr, lv_color_make(0, 0, 0), 0);
    lv_obj_clear_flag(home_scr, LV_OBJ_FLAG_SCROLLABLE);


    SCR_FUNC_TYPE *src_ops = malloc(sizeof(SCR_FUNC_TYPE));
    src_ops->load_func = home_scr_load_func;
    src_ops->refr_func = home_scr_refr_func;
    src_ops->quit_func = home_scr_quit_func;

    if(t_home == NULL)
        t_home = lv_timer_create(t_home_cb, 500, NULL); 
    lv_timer_pause(t_home);
    scr_add_user_data(home_scr, E_HOME_SCR, src_ops);
}
