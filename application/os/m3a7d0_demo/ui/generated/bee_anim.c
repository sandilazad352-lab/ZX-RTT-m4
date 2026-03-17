#include "bee_anim.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

// // extern bee_scr_data_t bee_screen_data;
// extern bee_scr_data_t* get_pet_data(void);

extern char* read_png_path[];

screen_timer_t screen_timer = {
    .screen_timer          = false,
    .screen_1_timer        = false,
    .screen_2_timer        = false,
    .screen_3_timer        = false,
    .screen_4_timer        = false,
    .screen_5_timer        = false,
    .screen_6_timer        = false,
    .screen_7_timer        = false,
    .screen_8_timer        = false,
    .screen_9_timer        = false,
    .screen_10_timer       = false,
    .screen_timer_index    = 0,
    .screen_1_timer_index  = 0,
    .screen_2_timer_index  = 0,
    .screen_3_timer_index  = 0,
    .screen_4_timer_index  = 0,
    .screen_5_timer_index  = 0,
    .screen_6_timer_index  = 0,
    .screen_7_timer_index  = 0,
    .screen_8_timer_index  = 0,
    .screen_9_timer_index  = 0,
    .screen_10_timer_index = 0,
};

static lv_obj_t *bk_scr = NULL;

void set_bk_scr(lv_obj_t *scr)
{
    bk_scr = scr;
}

void back_scr_ready_cb(struct _lv_anim_t *a)
{
    if(bk_scr != NULL)
        lv_scr_load(bk_scr);
    lv_anim_del_all();
    lv_img_cache_invalidate_src(NULL);
}

/*加载屏幕*/
void back_scr_delay(lv_obj_t* var_obj, int time){
    lv_anim_t ANIM_obj;
    lv_anim_init(&ANIM_obj);
    lv_anim_set_time(&ANIM_obj, time);
    lv_anim_set_values(&ANIM_obj, 0, 1);
    lv_anim_set_path_cb(&ANIM_obj, lv_anim_path_overshoot);
    lv_anim_set_delay(&ANIM_obj, 10);/*开启动画前的延时时间*/
    lv_anim_set_playback_time(&ANIM_obj, 0);     /*动画返回时时间*/
    lv_anim_set_playback_delay(&ANIM_obj, 0);    /*动画执行返回时延时时间*/
    lv_anim_set_repeat_count(&ANIM_obj, 0);      /*动画重复次数*/
    lv_anim_set_repeat_delay(&ANIM_obj, 0);      /*在重复动画之前设置延迟。*/
    lv_anim_set_early_apply(&ANIM_obj, true);    /*设置是应立即应用动画还是仅在延迟过期时应用动画。*/
    lv_anim_set_ready_cb(&ANIM_obj, back_scr_ready_cb);    
    lv_anim_set_var(&ANIM_obj, var_obj);
    lv_anim_start(&ANIM_obj);
}

/*动画由回弹效果，动画次数：1次*/
void obj_overshoot_anim(lv_obj_t* var_obj, int start_data, int end_data, int time, void* cb_function){
    lv_anim_t ANIM_obj;
    lv_anim_init(&ANIM_obj);
    lv_anim_set_time(&ANIM_obj, time);
    lv_anim_set_values(&ANIM_obj, start_data, end_data);
    lv_anim_set_path_cb(&ANIM_obj, lv_anim_path_overshoot);
    lv_anim_set_delay(&ANIM_obj, 10);/*开启动画前的延时时间*/
    lv_anim_set_playback_time(&ANIM_obj, 0);     /*动画返回时时间*/
    lv_anim_set_playback_delay(&ANIM_obj, 0);    /*动画执行返回时延时时间*/
    lv_anim_set_repeat_count(&ANIM_obj, 0);      /*动画重复次数*/
    lv_anim_set_repeat_delay(&ANIM_obj, 0);      /*在重复动画之前设置延迟。*/
    lv_anim_set_early_apply(&ANIM_obj, true);    /*设置是应立即应用动画还是仅在延迟过期时应用动画。*/
    
    lv_anim_set_var(&ANIM_obj, var_obj);
    lv_anim_set_exec_cb(&ANIM_obj, (lv_anim_exec_xcb_t)cb_function);
    lv_anim_start(&ANIM_obj);
}

/*动画由回弹效果，动画次数：1次, 有延时*/
void obj_overshoot_delay_anim(lv_obj_t* var_obj, int start_data, int end_data, int time, int delay, void* cb_function){
    lv_anim_t ANIM_obj;
    lv_anim_init(&ANIM_obj);
    lv_anim_set_time(&ANIM_obj, time);
    lv_anim_set_values(&ANIM_obj, start_data, end_data);
    lv_anim_set_path_cb(&ANIM_obj, lv_anim_path_overshoot);
    lv_anim_set_delay(&ANIM_obj, delay);/*开启动画前的延时时间*/
    lv_anim_set_playback_time(&ANIM_obj, 0);     /*动画返回时时间*/
    lv_anim_set_playback_delay(&ANIM_obj, 0);    /*动画执行返回时延时时间*/
    lv_anim_set_repeat_count(&ANIM_obj, 0);      /*动画重复次数*/
    lv_anim_set_repeat_delay(&ANIM_obj, 0);      /*在重复动画之前设置延迟。*/
    lv_anim_set_early_apply(&ANIM_obj, true);    /*设置是应立即应用动画还是仅在延迟过期时应用动画。*/
    
    lv_anim_set_var(&ANIM_obj, var_obj);
    lv_anim_set_exec_cb(&ANIM_obj, (lv_anim_exec_xcb_t)cb_function);
    lv_anim_start(&ANIM_obj);
}

/*动画无回弹效果，动画次数：1次*/
void obj_linear_anim(lv_obj_t* var_obj, int start_data, int end_data, int time, void* cb_function){
    lv_anim_t ANIM_obj;
    lv_anim_init(&ANIM_obj);
    lv_anim_set_time(&ANIM_obj, time);
    lv_anim_set_values(&ANIM_obj, start_data, end_data);
    lv_anim_set_path_cb(&ANIM_obj, lv_anim_path_linear);
    lv_anim_set_delay(&ANIM_obj, 100);/*开启动画前的延时时间*/
    lv_anim_set_playback_time(&ANIM_obj, 0);     /*动画返回时时间*/
    lv_anim_set_playback_delay(&ANIM_obj, 0);    /*动画执行返回时延时时间*/
    lv_anim_set_repeat_count(&ANIM_obj, 0);      /*动画重复次数*/
    lv_anim_set_repeat_delay(&ANIM_obj, 0);      /*在重复动画之前设置延迟。*/
    lv_anim_set_early_apply(&ANIM_obj, true);    /*设置是应立即应用动画还是仅在延迟过期时应用动画。*/
    
    lv_anim_set_var(&ANIM_obj, var_obj);
    lv_anim_set_exec_cb(&ANIM_obj, (lv_anim_exec_xcb_t)cb_function);
    lv_anim_start(&ANIM_obj);
}

/*动画无回弹效果，动画次数：来回各1次， 有延时*/
void obj_linear_round_trip_anim(lv_obj_t* var_obj, int start_data, int end_data, int time, int delay_time, void* cb_function){
    lv_anim_t ANIM_obj;
    lv_anim_init(&ANIM_obj);
    lv_anim_set_time(&ANIM_obj, time);
    lv_anim_set_values(&ANIM_obj, start_data, end_data);
    lv_anim_set_path_cb(&ANIM_obj, lv_anim_path_linear);
    lv_anim_set_delay(&ANIM_obj, delay_time);/*开启动画前的延时时间*/
    lv_anim_set_playback_time(&ANIM_obj, time);     /*动画返回时时间*/
    lv_anim_set_playback_delay(&ANIM_obj, 0);    /*动画执行返回时延时时间*/
    lv_anim_set_repeat_count(&ANIM_obj, 0);      /*动画重复次数*/
    lv_anim_set_repeat_delay(&ANIM_obj, 0);      /*在重复动画之前设置延迟。*/
    lv_anim_set_early_apply(&ANIM_obj, true);    /*设置是应立即应用动画还是仅在延迟过期时应用动画。*/
    
    lv_anim_set_var(&ANIM_obj, var_obj);
    lv_anim_set_exec_cb(&ANIM_obj, (lv_anim_exec_xcb_t)cb_function);
    lv_anim_start(&ANIM_obj);
}

/*动画无回弹效果，动画次数：1次, 有延时*/
void obj_linear_delay_anim(lv_obj_t* var_obj, int start_data, int end_data, int time, int delay, void* cb_function){
    lv_anim_t ANIM_obj;
    lv_anim_init(&ANIM_obj);
    lv_anim_set_time(&ANIM_obj, time);
    lv_anim_set_values(&ANIM_obj, start_data, end_data);
    lv_anim_set_path_cb(&ANIM_obj, lv_anim_path_linear);
    lv_anim_set_delay(&ANIM_obj, delay);/*开启动画前的延时时间*/
    lv_anim_set_playback_time(&ANIM_obj, 0);     /*动画返回时时间*/
    lv_anim_set_playback_delay(&ANIM_obj, 0);    /*动画执行返回时延时时间*/
    lv_anim_set_repeat_count(&ANIM_obj, 0);      /*动画重复次数*/
    lv_anim_set_repeat_delay(&ANIM_obj, 0);      /*在重复动画之前设置延迟。*/
    lv_anim_set_early_apply(&ANIM_obj, true);    /*设置是应立即应用动画还是仅在延迟过期时应用动画。*/
    
    lv_anim_set_var(&ANIM_obj, var_obj);
    lv_anim_set_exec_cb(&ANIM_obj, (lv_anim_exec_xcb_t)cb_function);
    lv_anim_start(&ANIM_obj);
}
    
/*动画无回弹效果，动画次数：无限*/
void obj_linear_infinite_anim(lv_obj_t* var_obj, int start_data, int end_data, int time, void* cb_function){
    lv_anim_t ANIM_obj;
    lv_anim_init(&ANIM_obj);
    lv_anim_set_time(&ANIM_obj, time);
    lv_anim_set_values(&ANIM_obj, start_data, end_data);
    lv_anim_set_path_cb(&ANIM_obj, lv_anim_path_linear);
    lv_anim_set_delay(&ANIM_obj, 10);/*开启动画前的延时时间*/
    lv_anim_set_playback_time(&ANIM_obj, time);     /*动画返回时时间*/
    lv_anim_set_playback_delay(&ANIM_obj, 10);    /*动画执行返回时延时时间*/
    lv_anim_set_repeat_count(&ANIM_obj, LV_ANIM_REPEAT_INFINITE);      /*动画重复次数*/
    lv_anim_set_repeat_delay(&ANIM_obj, 0);      /*在重复动画之前设置延迟。*/
    lv_anim_set_early_apply(&ANIM_obj, true);    /*设置是应立即应用动画还是仅在延迟过期时应用动画。*/
    
    lv_anim_set_var(&ANIM_obj, var_obj);
    lv_anim_set_exec_cb(&ANIM_obj, (lv_anim_exec_xcb_t)cb_function);
    lv_anim_start(&ANIM_obj);
}

/*动画无回弹效果，动画次数：无限（无来回）*/
void obj_linear_play_infinite_anim(lv_obj_t* var_obj, int start_data, int end_data, int time, void* cb_function){
    lv_anim_t ANIM_obj;
    lv_anim_init(&ANIM_obj);
    lv_anim_set_time(&ANIM_obj, time);
    lv_anim_set_values(&ANIM_obj, start_data, end_data);
    lv_anim_set_path_cb(&ANIM_obj, lv_anim_path_linear);
    lv_anim_set_delay(&ANIM_obj, 0);/*开启动画前的延时时间*/
    lv_anim_set_playback_time(&ANIM_obj, 0);     /*动画返回时时间*/
    lv_anim_set_playback_delay(&ANIM_obj, 0);    /*动画执行返回时延时时间*/
    lv_anim_set_repeat_count(&ANIM_obj, LV_ANIM_REPEAT_INFINITE);      /*动画重复次数*/
    lv_anim_set_repeat_delay(&ANIM_obj, 0);      /*在重复动画之前设置延迟。*/
    lv_anim_set_early_apply(&ANIM_obj, true);    /*设置是应立即应用动画还是仅在延迟过期时应用动画。*/
    
    lv_anim_set_var(&ANIM_obj, var_obj);
    lv_anim_set_exec_cb(&ANIM_obj, (lv_anim_exec_xcb_t)cb_function);
    lv_anim_start(&ANIM_obj);
}

/*动画无回弹效果，动画次数：无限，有延时*/
void obj_linear_delay_infinite_anim(lv_obj_t* var_obj, int start_data, int end_data, int time,int delay_time, void* cb_function){
    lv_anim_t ANIM_obj;
    lv_anim_init(&ANIM_obj);
    lv_anim_set_time(&ANIM_obj, time);
    lv_anim_set_values(&ANIM_obj, start_data, end_data);
    lv_anim_set_path_cb(&ANIM_obj, lv_anim_path_linear);
    lv_anim_set_delay(&ANIM_obj, delay_time);/*开启动画前的延时时间*/
    lv_anim_set_playback_time(&ANIM_obj, time);     /*动画返回时时间*/
    lv_anim_set_playback_delay(&ANIM_obj, 10);    /*动画执行返回时延时时间*/
    lv_anim_set_repeat_count(&ANIM_obj, LV_ANIM_REPEAT_INFINITE);      /*动画重复次数*/
    lv_anim_set_repeat_delay(&ANIM_obj, 0);      /*在重复动画之前设置延迟。*/
    lv_anim_set_early_apply(&ANIM_obj, true);    /*设置是应立即应用动画还是仅在延迟过期时应用动画。*/
    
    lv_anim_set_var(&ANIM_obj, var_obj);
    lv_anim_set_exec_cb(&ANIM_obj, (lv_anim_exec_xcb_t)cb_function);
    lv_anim_start(&ANIM_obj);
}


/*================== screen ====================*/
void lv_obj_set_img_opa(lv_obj_t* obj, int values)
{
	lv_obj_set_style_img_opa(obj, (uint8_t)values, 0);
}
void lv_obj_set_text_opa(lv_obj_t* obj, int values)
{
	lv_obj_set_style_text_opa(obj, (uint8_t)values, 0);
}
void lv_obj_set_obj_opa(lv_obj_t* obj, int values)
{
	lv_obj_set_style_opa(obj, (uint8_t)values, 0);
}

void lv_img_zoom(lv_obj_t* obj, int32_t values)
{
	lv_obj_invalidate(obj);
	lv_img_set_antialias(obj, true);
	lv_img_set_size_mode(obj, LV_IMG_SIZE_MODE_VIRTUAL);
	lv_img_set_zoom(obj, values);
}

void screen_anim(lv_ui* ui)
{
    ui_M3_t* ui_data = get_global_ui_data();
}
void screen_load_anim(lv_ui* ui)
{
    
}
/**
 * @brief screen_img_1 控件放大或者缩小
 * 
 * @param zoom_sign true: 放大 false: 缩小
 */
void screen_img_1_zoom(lv_ui* ui, bool zoom_sign)
{
    if(zoom_sign){
        lv_obj_clear_flag(ui->screen_img_2, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(ui->screen_img_3, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_move_foreground(ui->screen_img_2);
        lv_obj_move_foreground(ui->screen_img_3);

        obj_linear_anim(ui->screen_img_1, 151, 256, 500, lv_img_zoom);
        obj_linear_anim(ui->screen_img_1, 433, 472, 500, lv_obj_set_x);
        obj_linear_anim(ui->screen_img_1, 28,   40, 500, lv_obj_set_y);

        obj_linear_anim(ui->screen_img_2, 546, 492, 500, lv_obj_set_x);
        obj_linear_anim(ui->screen_img_2, 28,   66, 500, lv_obj_set_y);
        obj_linear_anim(ui->screen_img_2, 255,   0, 500, lv_obj_set_img_opa);
        obj_linear_anim(ui->screen_img_3, 118,  65, 500, lv_obj_set_y);
        obj_linear_anim(ui->screen_img_3, 255,   0, 500, lv_obj_set_img_opa);

        obj_linear_anim(ui->screen_label_28, 281, 261, 500, lv_obj_set_y);
        obj_linear_anim(ui->screen_label_29, 310, 280, 500, lv_obj_set_y);
        lv_label_set_text(ui->screen_label_28, "娱乐模式");
        lv_label_set_text(ui->screen_label_29, "Ent. mode");
    }else{
        lv_obj_move_foreground(ui->screen_img_2);
        lv_obj_move_foreground(ui->screen_img_3);

        obj_linear_anim(ui->screen_img_1, 256, 151, 500, lv_img_zoom);
        obj_linear_anim(ui->screen_img_1, 472, 433, 500, lv_obj_set_x);
        obj_linear_anim(ui->screen_img_1, 40,   28, 500, lv_obj_set_y);

        obj_linear_anim(ui->screen_img_2, 492, 546, 500, lv_obj_set_x);
        obj_linear_anim(ui->screen_img_2, 66,   28, 500, lv_obj_set_y);
        obj_linear_anim(ui->screen_img_2, 0,   255, 500, lv_obj_set_img_opa);
        obj_linear_anim(ui->screen_img_3, 65,  118, 500, lv_obj_set_y);
        obj_linear_anim(ui->screen_img_3, 0,   255, 500, lv_obj_set_img_opa);

        lv_obj_add_flag(ui->screen_img_2, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(ui->screen_img_3, LV_OBJ_FLAG_CLICKABLE);

        obj_linear_anim(ui->screen_label_28, 261, 281, 500, lv_obj_set_y);
        obj_linear_anim(ui->screen_label_29, 280, 310, 500, lv_obj_set_y);
        lv_label_set_text(ui->screen_label_28, "模式选择");
        lv_label_set_text(ui->screen_label_29, "Model choose");
    }
}
void screen_img_2_zoom(lv_ui* ui, bool zoom_sign)
{
    if(zoom_sign){
        lv_obj_clear_flag(ui->screen_img_1, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(ui->screen_img_3, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_move_foreground(ui->screen_img_1);
        lv_obj_move_foreground(ui->screen_img_3);

        obj_linear_anim(ui->screen_img_2, 151, 256, 500, lv_img_zoom);
        obj_linear_anim(ui->screen_img_2, 546, 474, 500, lv_obj_set_x);
        obj_linear_anim(ui->screen_img_2, 28,   40, 500, lv_obj_set_y);

        obj_linear_anim(ui->screen_img_1, 433, 487, 500, lv_obj_set_x);
        obj_linear_anim(ui->screen_img_1, 27,   65, 500, lv_obj_set_y);
        obj_linear_anim(ui->screen_img_1, 255,   0, 500, lv_obj_set_img_opa);
        obj_linear_anim(ui->screen_img_3, 118,  65, 500, lv_obj_set_y);
        obj_linear_anim(ui->screen_img_3, 255,   0, 500, lv_obj_set_img_opa);

        obj_linear_anim(ui->screen_label_28, 281, 261, 500, lv_obj_set_y);
        obj_linear_anim(ui->screen_label_29, 310, 280, 500, lv_obj_set_y);
        lv_label_set_text(ui->screen_label_28, "睡眠模式");
        lv_label_set_text(ui->screen_label_29, "Sleep Mode");
    }else{
        lv_obj_move_foreground(ui->screen_img_1);
        lv_obj_move_foreground(ui->screen_img_3);

        obj_linear_anim(ui->screen_img_2, 256, 151, 500, lv_img_zoom);
        obj_linear_anim(ui->screen_img_2, 474, 546, 500, lv_obj_set_x);
        obj_linear_anim(ui->screen_img_2,   40, 28, 500, lv_obj_set_y);

        obj_linear_anim(ui->screen_img_1, 487, 433, 500, lv_obj_set_x);
        obj_linear_anim(ui->screen_img_1,   65, 27, 500, lv_obj_set_y);
        obj_linear_anim(ui->screen_img_1,   0, 255, 500, lv_obj_set_img_opa);
        obj_linear_anim(ui->screen_img_3,  65, 118, 500, lv_obj_set_y);
        obj_linear_anim(ui->screen_img_3,   0, 255, 500, lv_obj_set_img_opa);

        lv_obj_add_flag(ui->screen_img_1, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(ui->screen_img_3, LV_OBJ_FLAG_CLICKABLE);

        obj_linear_anim(ui->screen_label_28, 261, 281, 500, lv_obj_set_y);
        obj_linear_anim(ui->screen_label_29, 280, 310, 500, lv_obj_set_y);
        lv_label_set_text(ui->screen_label_28, "模式选择");
        lv_label_set_text(ui->screen_label_29, "Model choose");
    }
}
void screen_img_3_zoom(lv_ui* ui, bool zoom_sign)
{
    if(zoom_sign){
        lv_obj_clear_flag(ui->screen_img_1, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(ui->screen_img_2, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_move_foreground(ui->screen_img_1);
        lv_obj_move_foreground(ui->screen_img_2);

        obj_linear_anim(ui->screen_img_3, 151, 256, 500, lv_img_zoom);
        obj_linear_anim(ui->screen_img_3, 488, 473, 500, lv_obj_set_x);
        obj_linear_anim(ui->screen_img_3, 118,  40, 500, lv_obj_set_y);

        obj_linear_anim(ui->screen_img_1, 433, 487, 500, lv_obj_set_x);
        obj_linear_anim(ui->screen_img_1, 27,   65, 500, lv_obj_set_y);
        obj_linear_anim(ui->screen_img_1, 255,   0, 500, lv_obj_set_img_opa);
        obj_linear_anim(ui->screen_img_2, 546, 492, 500, lv_obj_set_x);
        obj_linear_anim(ui->screen_img_2, 28,   66, 500, lv_obj_set_y);
        obj_linear_anim(ui->screen_img_2, 255,   0, 500, lv_obj_set_img_opa);

        obj_linear_anim(ui->screen_label_28, 281, 261, 500, lv_obj_set_y);
        obj_linear_anim(ui->screen_label_29, 310, 280, 500, lv_obj_set_y);
        lv_label_set_text(ui->screen_label_28, "离家模式");
        lv_label_set_text(ui->screen_label_29, "Away Mode");
    }else{
        lv_obj_move_foreground(ui->screen_img_1);
        lv_obj_move_foreground(ui->screen_img_2);

        obj_linear_anim(ui->screen_img_3, 256, 151, 500, lv_img_zoom);
        obj_linear_anim(ui->screen_img_3, 473, 488, 500, lv_obj_set_x);
        obj_linear_anim(ui->screen_img_3,  40, 118, 500, lv_obj_set_y);

        obj_linear_anim(ui->screen_img_1, 487, 433, 500, lv_obj_set_x);
        obj_linear_anim(ui->screen_img_1,   65, 27, 500, lv_obj_set_y);
        obj_linear_anim(ui->screen_img_1,   0, 255, 500, lv_obj_set_img_opa);
        obj_linear_anim(ui->screen_img_2, 492, 546, 500, lv_obj_set_x);
        obj_linear_anim(ui->screen_img_2,   66, 28, 500, lv_obj_set_y);
        obj_linear_anim(ui->screen_img_2, 0  ,  255,500, lv_obj_set_img_opa);

        lv_obj_add_flag(ui->screen_img_1, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(ui->screen_img_2, LV_OBJ_FLAG_CLICKABLE);

        obj_linear_anim(ui->screen_label_28, 261, 281, 500, lv_obj_set_y);
        obj_linear_anim(ui->screen_label_29, 280, 310, 500, lv_obj_set_y);
        lv_label_set_text(ui->screen_label_28, "模式选择");
        lv_label_set_text(ui->screen_label_29, "Model choose");
    }
}

void screen_img_m4_anim(lv_ui* ui, int8_t mode_sign)
{
    if(mode_sign == 2){
        obj_linear_anim(ui->screen_img_m47,   0,  90, 500, lv_obj_set_y);
        obj_linear_anim(ui->screen_img_m47,   0, 255, 500, lv_obj_set_img_opa);
        obj_linear_anim(ui->screen_img_m46,  18, -70, 500, lv_obj_set_y);
        obj_linear_anim(ui->screen_img_m46, 255,   0, 500, lv_obj_set_img_opa);

        obj_linear_anim(ui->screen_img_m49, 320, 296, 500, lv_obj_set_y);
        obj_linear_anim(ui->screen_img_m49,   0, 255, 500, lv_obj_set_img_opa);
        obj_linear_anim(ui->screen_img_m48, 296, 320, 500, lv_obj_set_y);
        obj_linear_anim(ui->screen_img_m48, 255,   0, 500, lv_obj_set_img_opa);

        obj_linear_anim(ui->screen_label_32, 320, 298, 500, lv_obj_set_y);
        obj_linear_anim(ui->screen_label_32,   0, 255, 500, lv_obj_set_text_opa);
        obj_linear_anim(ui->screen_slider_1, 270, 320, 500, lv_obj_set_y);
        obj_linear_anim(ui->screen_slider_1, 255,   0, 500, lv_obj_set_obj_opa);

        obj_linear_anim(ui->screen_label_30, 190, 230, 500, lv_obj_set_y);
        obj_linear_anim(ui->screen_label_31, 215, 255, 500, lv_obj_set_y);
        lv_label_set_text(ui->screen_label_30, "卧室窗帘");
        lv_label_set_text(ui->screen_label_31, "Living Room");
    }else if(mode_sign == 1){
        obj_linear_anim(ui->screen_img_m47,  90,   0, 500, lv_obj_set_y);
        obj_linear_anim(ui->screen_img_m47, 255,   0, 500, lv_obj_set_img_opa);
        obj_linear_anim(ui->screen_img_m46, -70,  18, 500, lv_obj_set_y);
        obj_linear_anim(ui->screen_img_m46,   0, 255, 500, lv_obj_set_img_opa);

        obj_linear_anim(ui->screen_img_m49, 296, 320, 500, lv_obj_set_y);
        obj_linear_anim(ui->screen_img_m49, 255,   0, 500, lv_obj_set_img_opa);
        obj_linear_anim(ui->screen_img_m48, 320, 296, 500, lv_obj_set_y);
        obj_linear_anim(ui->screen_img_m48,   0, 255, 500, lv_obj_set_img_opa);

        obj_linear_anim(ui->screen_label_32, 298, 320, 500, lv_obj_set_y);
        obj_linear_anim(ui->screen_label_32, 255,   0, 500, lv_obj_set_text_opa);
        obj_linear_anim(ui->screen_slider_1, 320, 270, 500, lv_obj_set_y);
        obj_linear_anim(ui->screen_slider_1,   0, 255, 500, lv_obj_set_obj_opa);

        obj_linear_anim(ui->screen_label_30, 230, 190, 500, lv_obj_set_y);
        obj_linear_anim(ui->screen_label_31, 255, 215, 500, lv_obj_set_y);
        lv_label_set_text(ui->screen_label_30, "Sharpen Edge");
        lv_label_set_text(ui->screen_label_31, "Gallant");
    }
}

extern char* wea_name[5];
extern int8_t wea_tem[5];
extern int8_t wea_hum[5];
extern int8_t wea_btn_bot[2];
extern int8_t wea_img_big[5];

void screen_wea_anim(lv_ui* ui, int8_t wea_sign)
{
    lv_obj_t* ui_img[] = {
        ui->screen_img_m6, ui->screen_img_m7, ui->screen_img_m8,
        ui->screen_img_m9, ui->screen_img_m10,
    };
    for(int i = 1; i <= 5; i++){
        if(wea_sign == i){
            lv_img_set_src(ui_img[i-1], read_png_path[wea_btn_bot[1]]);
        }else{
            lv_img_set_src(ui_img[i-1], read_png_path[wea_btn_bot[0]]);
        }
    }

    int8_t sign = wea_sign-1;
    lv_label_set_text_fmt(ui->screen_label_8 , "%02d°", wea_tem[sign]);
    lv_label_set_text_fmt(ui->screen_label_12, "%s", wea_name[sign]);
    lv_img_set_src(ui->screen_img_m45, read_png_path[wea_img_big[sign]]);
}

void screen_light_anim(lv_ui* ui)
{
    ui_M3_t* ui_data = get_global_ui_data();

    if(ui_data->home_light.option_sign){
        // printf("从未打开-->打开动画\r\n");
        // 从未打开-->打开动画
        // lv_obj_add_state(ui->screen_sw_1, LV_STATE_CHECKED);
        obj_linear_anim(ui->screen_sw_1, 255, 0, 250,lv_obj_set_obj_opa);
        obj_linear_anim(ui->screen_label_39, 189, 545, 250, lv_obj_set_width);
        obj_linear_anim(ui->screen_img_m38, 661, 1037, 250, lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m39, 852, 1228, 250, lv_obj_set_x);

        obj_linear_anim(ui->screen_img_m13, 255, 0, 250,lv_obj_set_img_opa);
        obj_linear_anim(ui->screen_label_33, 255, 0, 250,lv_obj_set_text_opa);
        obj_linear_anim(ui->screen_label_34, 0, 255, 250,lv_obj_set_text_opa);

        obj_linear_anim(ui->screen_img_m14, 0, 255, 250,lv_obj_set_img_opa);
        // obj_linear_anim(ui->screen_img_m15, 0, 255, 250,lv_obj_set_img_opa);
        // obj_linear_anim(ui->screen_img_m16, 0, 255, 250,lv_obj_set_img_opa);
        // obj_linear_anim(ui->screen_img_m17, 0, 255, 250,lv_obj_set_img_opa);
        // obj_linear_anim(ui->screen_img_m18, 0, 255, 250,lv_obj_set_img_opa);
        // obj_linear_anim(ui->screen_img_m19, 0, 255, 250,lv_obj_set_img_opa);
        obj_linear_anim(ui->screen_img_m20, 0, 255, 250,lv_obj_set_img_opa);
        // obj_linear_anim(ui->screen_img_m21, 0, 255, 250,lv_obj_set_img_opa);
        // obj_linear_anim(ui->screen_img_m22, 0, 255, 250,lv_obj_set_img_opa);
        // obj_linear_anim(ui->screen_img_m23, 0, 255, 250,lv_obj_set_img_opa);
        // obj_linear_anim(ui->screen_img_m24, 0, 255, 250,lv_obj_set_img_opa);
        // obj_linear_anim(ui->screen_img_m25, 0, 255, 250,lv_obj_set_img_opa);
        obj_linear_anim(ui->screen_img_m26, 0, 255, 250,lv_obj_set_img_opa);
        // obj_linear_anim(ui->screen_img_m27, 0, 255, 250,lv_obj_set_img_opa);
        // obj_linear_anim(ui->screen_img_m28, 0, 255, 250,lv_obj_set_img_opa);
        // obj_linear_anim(ui->screen_img_m29, 0, 255, 250,lv_obj_set_img_opa);
        // obj_linear_anim(ui->screen_img_m30, 0, 255, 250,lv_obj_set_img_opa);
        // obj_linear_anim(ui->screen_img_m31, 0, 255, 250,lv_obj_set_img_opa);
        obj_linear_anim(ui->screen_img_m32, 0, 255, 250,lv_obj_set_img_opa);
        // obj_linear_anim(ui->screen_img_m33, 0, 255, 250,lv_obj_set_img_opa);
        // obj_linear_anim(ui->screen_img_m34, 0, 255, 250,lv_obj_set_img_opa);
        // obj_linear_anim(ui->screen_img_m35, 0, 255, 250,lv_obj_set_img_opa);
        // obj_linear_anim(ui->screen_img_m36, 0, 255, 250,lv_obj_set_img_opa);
        // obj_linear_anim(ui->screen_img_m37, 0, 255, 250,lv_obj_set_img_opa);

        obj_linear_anim(ui->screen_img_m14, (717 - 90), 717, 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m15, (768 - 90), 768, 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m16, (773 - 90), 773, 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m17, (778 - 90), 778, 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m18, (778 - 90), 778, 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m19, (778 - 90), 778, 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m20, (865 - 90), 865, 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m21, (916 - 90), 916, 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m22, (921 - 90), 921, 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m23, (926 - 90), 926, 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m24, (926 - 90), 926, 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m25, (926 - 90), 926, 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m26, (717 - 90), 717, 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m27, (768 - 90), 768, 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m28, (773 - 90), 773, 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m29, (778 - 90), 778, 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m30, (778 - 90), 778, 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m31, (778 - 90), 778, 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m32, (865 - 90), 865, 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m33, (916 - 90), 916, 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m34, (921 - 90), 921, 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m35, (926 - 90), 926, 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m36, (926 - 90), 926, 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m37, (926 - 90), 926, 250,lv_obj_set_x);

        // obj_linear_anim(ui->screen_img_m14, 630, 717, 250,lv_obj_set_x);
        // obj_linear_anim(ui->screen_img_m20, 778, 865, 250,lv_obj_set_x);
        // obj_linear_anim(ui->screen_img_m26, 630, 717, 250,lv_obj_set_x);
        // obj_linear_anim(ui->screen_img_m32, 778, 865, 250,lv_obj_set_x);

        obj_linear_anim(ui->screen_sw_2, 0, 255, 250, lv_obj_set_obj_opa);
        obj_linear_anim(ui->screen_sw_3, 0, 255, 250, lv_obj_set_obj_opa);
        obj_linear_anim(ui->screen_sw_4, 0, 255, 250, lv_obj_set_obj_opa);
        obj_linear_anim(ui->screen_sw_5, 0, 255, 250, lv_obj_set_obj_opa);
        obj_linear_anim(ui->screen_sw_2, (718 - 90), 718, 250, lv_obj_set_x);
        obj_linear_anim(ui->screen_sw_3, (866 - 90), 866, 250, lv_obj_set_x);
        obj_linear_anim(ui->screen_sw_4, (718 - 90), 718, 250, lv_obj_set_x);
        obj_linear_anim(ui->screen_sw_5, (866 - 90), 866, 250, lv_obj_set_x);

        if(ui_data->home_light.light_option[0].color == false){
            // 未展开颜色选项
            lv_obj_set_y(ui->screen_img_m15, 378);
            lv_obj_set_y(ui->screen_img_m16, 383+600);
            lv_obj_set_y(ui->screen_img_m17, 388+600);
            lv_obj_set_y(ui->screen_img_m18, 388+600);
            lv_obj_set_y(ui->screen_img_m19, 388+600);
            obj_linear_anim(ui->screen_img_m15, 0, 255, 250,lv_obj_set_img_opa);
            lv_obj_set_style_img_opa(ui->screen_img_m16, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m17, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m18, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m19, 0, 0);
            lv_obj_add_flag(ui->screen_img_m15, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m16, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m17, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m18, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m19, LV_OBJ_FLAG_CLICKABLE);
        }else if(ui_data->home_light.light_option[0].color == true){
            // 展开颜色选项
            lv_obj_set_y(ui->screen_img_m15, 378+600);
            lv_obj_set_y(ui->screen_img_m16, 383);
            lv_obj_set_y(ui->screen_img_m17, 408);
            lv_obj_set_y(ui->screen_img_m18, 428);
            lv_obj_set_y(ui->screen_img_m19, 448);
            lv_obj_set_style_img_opa(ui->screen_img_m15, 0, 0);
            obj_linear_anim(ui->screen_img_m16, 0, 255, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m17, 0, 255, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m18, 0, 255, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m19, 0, 255, 250,lv_obj_set_img_opa);
            lv_obj_clear_flag(ui->screen_img_m15, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m16, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m17, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m18, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m19, LV_OBJ_FLAG_CLICKABLE);
        }

        if(ui_data->home_light.light_option[1].color == false){
            // 未展开颜色选项
            lv_obj_set_y(ui->screen_img_m21, 378);
            lv_obj_set_y(ui->screen_img_m22, 383+600);
            lv_obj_set_y(ui->screen_img_m23, 388+600);
            lv_obj_set_y(ui->screen_img_m24, 388+600);
            lv_obj_set_y(ui->screen_img_m25, 388+600);
            obj_linear_anim(ui->screen_img_m21, 0, 255, 250,lv_obj_set_img_opa);
            lv_obj_set_style_img_opa(ui->screen_img_m22, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m23, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m24, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m25, 0, 0);
            lv_obj_add_flag(ui->screen_img_m21, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m22, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m23, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m24, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m25, LV_OBJ_FLAG_CLICKABLE);
        }else if(ui_data->home_light.light_option[1].color == true){
            // 展开颜色选项
            lv_obj_set_y(ui->screen_img_m21, 378+600);
            lv_obj_set_y(ui->screen_img_m22, 383);
            lv_obj_set_y(ui->screen_img_m23, 408);
            lv_obj_set_y(ui->screen_img_m24, 428);
            lv_obj_set_y(ui->screen_img_m25, 448);
            lv_obj_set_style_img_opa(ui->screen_img_m21, 0, 0);
            obj_linear_anim(ui->screen_img_m22, 0, 255, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m23, 0, 255, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m24, 0, 255, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m25, 0, 255, 250,lv_obj_set_img_opa);
            lv_obj_clear_flag(ui->screen_img_m21, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m22, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m23, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m24, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m25, LV_OBJ_FLAG_CLICKABLE);
        }

        if(ui_data->home_light.light_option[2].color == false){
            lv_obj_set_y(ui->screen_img_m27, 480);
            lv_obj_set_y(ui->screen_img_m28, 485+600);
            lv_obj_set_y(ui->screen_img_m29, 490+600);
            lv_obj_set_y(ui->screen_img_m30, 490+600);
            lv_obj_set_y(ui->screen_img_m31, 490+600);
            obj_linear_anim(ui->screen_img_m27, 0, 255, 250,lv_obj_set_img_opa);
            lv_obj_set_style_img_opa(ui->screen_img_m28, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m29, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m30, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m31, 0, 0);
            lv_obj_add_flag(ui->screen_img_m27, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m28, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m29, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m30, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m31, LV_OBJ_FLAG_CLICKABLE);
        }else if(ui_data->home_light.light_option[2].color == true){
            lv_obj_set_y(ui->screen_img_m27, 480+600);
            lv_obj_set_y(ui->screen_img_m28, 485);
            lv_obj_set_y(ui->screen_img_m29, 510);
            lv_obj_set_y(ui->screen_img_m30, 530);
            lv_obj_set_y(ui->screen_img_m31, 550);
            lv_obj_set_style_img_opa(ui->screen_img_m27, 0, 0);
            obj_linear_anim(ui->screen_img_m28, 0, 255, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m29, 0, 255, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m30, 0, 255, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m31, 0, 255, 250,lv_obj_set_img_opa);
            lv_obj_clear_flag(ui->screen_img_m27, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m28, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m29, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m30, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m31, LV_OBJ_FLAG_CLICKABLE);
        }

        if(ui_data->home_light.light_option[3].color == false){
            lv_obj_set_y(ui->screen_img_m33, 480);
            lv_obj_set_y(ui->screen_img_m34, 485+600);
            lv_obj_set_y(ui->screen_img_m35, 490+600);
            lv_obj_set_y(ui->screen_img_m36, 490+600);
            lv_obj_set_y(ui->screen_img_m37, 490+600);
            obj_linear_anim(ui->screen_img_m33, 0, 255, 250,lv_obj_set_img_opa);
            lv_obj_set_style_img_opa(ui->screen_img_m34, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m35, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m36, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m37, 0, 0);
            lv_obj_add_flag(ui->screen_img_m33, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m34, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m35, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m36, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m37, LV_OBJ_FLAG_CLICKABLE);
        }else if(ui_data->home_light.light_option[3].color == true){
            lv_obj_set_y(ui->screen_img_m33, 480+600);
            lv_obj_set_y(ui->screen_img_m34, 485);
            lv_obj_set_y(ui->screen_img_m35, 510);
            lv_obj_set_y(ui->screen_img_m36, 530);
            lv_obj_set_y(ui->screen_img_m37, 550);
            lv_obj_set_style_img_opa(ui->screen_img_m33, 0, 0);
            obj_linear_anim(ui->screen_img_m34, 0, 255, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m35, 0, 255, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m36, 0, 255, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m37, 0, 255, 250,lv_obj_set_img_opa);
            lv_obj_clear_flag(ui->screen_img_m33, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m34, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m35, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m36, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m37, LV_OBJ_FLAG_CLICKABLE);
        }

        lv_obj_add_flag(ui->screen_sw_2, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(ui->screen_sw_3, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(ui->screen_sw_4, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_flag(ui->screen_sw_5, LV_OBJ_FLAG_CLICKABLE);
    }else{
        // printf(" 从打开-->未打开动画\r\n");
        // 从打开-->未打开动画
        // lv_obj_clear_state(ui->screen_sw_1, LV_STATE_CHECKED);
        obj_linear_anim(ui->screen_sw_1, 0, 255, 250,lv_obj_set_obj_opa);
        obj_linear_anim(ui->screen_label_39, 545, 189, 250, lv_obj_set_width);
        obj_linear_anim(ui->screen_img_m38, 1037, 661, 250, lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m39, 1228, 852, 250, lv_obj_set_x);

        obj_linear_anim(ui->screen_img_m13, 0, 255, 250,lv_obj_set_img_opa);
        obj_linear_anim(ui->screen_label_33, 0, 255, 250,lv_obj_set_text_opa);
        obj_linear_anim(ui->screen_label_34, 255, 0, 250,lv_obj_set_text_opa);

        obj_linear_anim(ui->screen_img_m14, 255, 0, 250,lv_obj_set_img_opa);
        // obj_linear_anim(ui->screen_img_m15, 255, 0, 250,lv_obj_set_img_opa);
        // obj_linear_anim(ui->screen_img_m16, 255, 0, 250,lv_obj_set_img_opa);
        // obj_linear_anim(ui->screen_img_m17, 255, 0, 250,lv_obj_set_img_opa);
        // obj_linear_anim(ui->screen_img_m18, 255, 0, 250,lv_obj_set_img_opa);
        // obj_linear_anim(ui->screen_img_m19, 255, 0, 250,lv_obj_set_img_opa);
        obj_linear_anim(ui->screen_img_m20, 255, 0, 250,lv_obj_set_img_opa);
        // obj_linear_anim(ui->screen_img_m21, 255, 0, 250,lv_obj_set_img_opa);
        // obj_linear_anim(ui->screen_img_m22, 255, 0, 250,lv_obj_set_img_opa);
        // obj_linear_anim(ui->screen_img_m23, 255, 0, 250,lv_obj_set_img_opa);
        // obj_linear_anim(ui->screen_img_m24, 255, 0, 250,lv_obj_set_img_opa);
        // obj_linear_anim(ui->screen_img_m25, 255, 0, 250,lv_obj_set_img_opa);
        obj_linear_anim(ui->screen_img_m26, 255, 0, 250,lv_obj_set_img_opa);
        // obj_linear_anim(ui->screen_img_m27, 255, 0, 250,lv_obj_set_img_opa);
        // obj_linear_anim(ui->screen_img_m28, 255, 0, 250,lv_obj_set_img_opa);
        // obj_linear_anim(ui->screen_img_m29, 255, 0, 250,lv_obj_set_img_opa);
        // obj_linear_anim(ui->screen_img_m30, 255, 0, 250,lv_obj_set_img_opa);
        // obj_linear_anim(ui->screen_img_m31, 255, 0, 250,lv_obj_set_img_opa);
        obj_linear_anim(ui->screen_img_m32, 255, 0, 250,lv_obj_set_img_opa);
        // obj_linear_anim(ui->screen_img_m33, 255, 0, 250,lv_obj_set_img_opa);
        // obj_linear_anim(ui->screen_img_m34, 255, 0, 250,lv_obj_set_img_opa);
        // obj_linear_anim(ui->screen_img_m35, 255, 0, 250,lv_obj_set_img_opa);
        // obj_linear_anim(ui->screen_img_m36, 255, 0, 250,lv_obj_set_img_opa);
        // obj_linear_anim(ui->screen_img_m37, 255, 0, 250,lv_obj_set_img_opa);

        obj_linear_anim(ui->screen_img_m14, 717, (717 - 90), 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m15, 768, (768 - 90), 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m16, 773, (773 - 90), 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m17, 778, (778 - 90), 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m18, 778, (778 - 90), 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m19, 778, (778 - 90), 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m20, 865, (865 - 90), 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m21, 916, (916 - 90), 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m22, 921, (921 - 90), 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m23, 926, (926 - 90), 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m24, 926, (926 - 90), 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m25, 926, (926 - 90), 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m26, 717, (717 - 90), 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m27, 768, (768 - 90), 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m28, 773, (773 - 90), 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m29, 778, (778 - 90), 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m30, 778, (778 - 90), 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m31, 778, (778 - 90), 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m32, 865, (865 - 90), 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m33, 916, (916 - 90), 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m34, 921, (921 - 90), 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m35, 926, (926 - 90), 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m36, 926, (926 - 90), 250,lv_obj_set_x);
        obj_linear_anim(ui->screen_img_m37, 926, (926 - 90), 250,lv_obj_set_x);

        // obj_linear_anim(ui->screen_img_m14, 717, 630, 250,lv_obj_set_x);
        // obj_linear_anim(ui->screen_img_m20, 865, 778, 250,lv_obj_set_x);
        // obj_linear_anim(ui->screen_img_m26, 717, 630, 250,lv_obj_set_x);
        // obj_linear_anim(ui->screen_img_m32, 865, 778, 250,lv_obj_set_x);

        obj_linear_anim(ui->screen_sw_2, 255, 0, 250, lv_obj_set_obj_opa);
        obj_linear_anim(ui->screen_sw_3, 255, 0, 250, lv_obj_set_obj_opa);
        obj_linear_anim(ui->screen_sw_4, 255, 0, 250, lv_obj_set_obj_opa);
        obj_linear_anim(ui->screen_sw_5, 255, 0, 250, lv_obj_set_obj_opa);
        obj_linear_anim(ui->screen_sw_2, 718, (718 - 90), 250, lv_obj_set_x);
        obj_linear_anim(ui->screen_sw_3, 866, (866 - 90), 250, lv_obj_set_x);
        obj_linear_anim(ui->screen_sw_4, 718, (718 - 90), 250, lv_obj_set_x);
        obj_linear_anim(ui->screen_sw_5, 866, (866 - 90), 250, lv_obj_set_x);

        if(ui_data->home_light.light_option[0].color == false){
            // 未展开颜色选项
            lv_obj_set_y(ui->screen_img_m15, 378);
            lv_obj_set_y(ui->screen_img_m16, 383+600);
            lv_obj_set_y(ui->screen_img_m17, 388+600);
            lv_obj_set_y(ui->screen_img_m18, 388+600);
            lv_obj_set_y(ui->screen_img_m19, 388+600);
            obj_linear_anim(ui->screen_img_m15, 255, 0, 250,lv_obj_set_img_opa);
            lv_obj_set_style_img_opa(ui->screen_img_m16, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m17, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m18, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m19, 0, 0);
            lv_obj_add_flag(ui->screen_img_m15, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m16, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m17, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m18, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m19, LV_OBJ_FLAG_CLICKABLE);
        }else if(ui_data->home_light.light_option[0].color == true){
            // 展开颜色选项
            lv_obj_set_y(ui->screen_img_m15, 378+600);
            lv_obj_set_y(ui->screen_img_m16, 383);
            lv_obj_set_y(ui->screen_img_m17, 408);
            lv_obj_set_y(ui->screen_img_m18, 428);
            lv_obj_set_y(ui->screen_img_m19, 448);
            lv_obj_set_style_img_opa(ui->screen_img_m15, 0, 0);
            obj_linear_anim(ui->screen_img_m16, 255, 0, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m17, 255, 0, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m18, 255, 0, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m19, 255, 0, 250,lv_obj_set_img_opa);
            lv_obj_clear_flag(ui->screen_img_m15, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m16, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m17, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m18, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m19, LV_OBJ_FLAG_CLICKABLE);
        }

        if(ui_data->home_light.light_option[1].color == false){
            // 未展开颜色选项
            lv_obj_set_y(ui->screen_img_m21, 378);
            lv_obj_set_y(ui->screen_img_m22, 383+600);
            lv_obj_set_y(ui->screen_img_m23, 388+600);
            lv_obj_set_y(ui->screen_img_m24, 388+600);
            lv_obj_set_y(ui->screen_img_m25, 388+600);
            obj_linear_anim(ui->screen_img_m21, 255, 0, 250,lv_obj_set_img_opa);
            lv_obj_set_style_img_opa(ui->screen_img_m22, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m23, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m24, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m25, 0, 0);
            lv_obj_add_flag(ui->screen_img_m21, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m22, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m23, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m24, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m25, LV_OBJ_FLAG_CLICKABLE);
        }else if(ui_data->home_light.light_option[1].color == true){
            // 展开颜色选项
            lv_obj_set_y(ui->screen_img_m21, 378+600);
            lv_obj_set_y(ui->screen_img_m22, 383);
            lv_obj_set_y(ui->screen_img_m23, 408);
            lv_obj_set_y(ui->screen_img_m24, 428);
            lv_obj_set_y(ui->screen_img_m25, 448);
            lv_obj_set_style_img_opa(ui->screen_img_m21, 0, 0);
            obj_linear_anim(ui->screen_img_m22, 255, 0, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m23, 255, 0, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m24, 255, 0, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m25, 255, 0, 250,lv_obj_set_img_opa);
            lv_obj_clear_flag(ui->screen_img_m21, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m22, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m23, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m24, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m25, LV_OBJ_FLAG_CLICKABLE);
        }

        if(ui_data->home_light.light_option[2].color == false){
            lv_obj_set_y(ui->screen_img_m27, 480);
            lv_obj_set_y(ui->screen_img_m28, 485+600);
            lv_obj_set_y(ui->screen_img_m29, 490+600);
            lv_obj_set_y(ui->screen_img_m30, 490+600);
            lv_obj_set_y(ui->screen_img_m31, 490+600);
            obj_linear_anim(ui->screen_img_m27, 255, 0, 250,lv_obj_set_img_opa);
            lv_obj_set_style_img_opa(ui->screen_img_m28, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m29, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m30, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m31, 0, 0);
            lv_obj_add_flag(ui->screen_img_m27, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m28, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m29, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m30, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m31, LV_OBJ_FLAG_CLICKABLE);
        }else if(ui_data->home_light.light_option[2].color == true){
            lv_obj_set_y(ui->screen_img_m27, 480+600);
            lv_obj_set_y(ui->screen_img_m28, 485);
            lv_obj_set_y(ui->screen_img_m29, 510);
            lv_obj_set_y(ui->screen_img_m30, 530);
            lv_obj_set_y(ui->screen_img_m31, 550);
            lv_obj_set_style_img_opa(ui->screen_img_m27, 0, 0);
            obj_linear_anim(ui->screen_img_m28, 255, 0, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m29, 255, 0, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m30, 255, 0, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m31, 255, 0, 250,lv_obj_set_img_opa);
            lv_obj_clear_flag(ui->screen_img_m27, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m28, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m29, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m30, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m31, LV_OBJ_FLAG_CLICKABLE);
        }

        if(ui_data->home_light.light_option[3].color == false){
            lv_obj_set_y(ui->screen_img_m33, 480);
            lv_obj_set_y(ui->screen_img_m34, 485+600);
            lv_obj_set_y(ui->screen_img_m35, 490+600);
            lv_obj_set_y(ui->screen_img_m36, 490+600);
            lv_obj_set_y(ui->screen_img_m37, 490+600);
            obj_linear_anim(ui->screen_img_m33, 255, 0, 250,lv_obj_set_img_opa);
            lv_obj_set_style_img_opa(ui->screen_img_m34, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m35, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m36, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m37, 0, 0);
            lv_obj_add_flag(ui->screen_img_m33, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m34, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m35, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m36, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m37, LV_OBJ_FLAG_CLICKABLE);
        }else if(ui_data->home_light.light_option[3].color == true){
            lv_obj_set_y(ui->screen_img_m33, 480+600);
            lv_obj_set_y(ui->screen_img_m34, 485);
            lv_obj_set_y(ui->screen_img_m35, 510);
            lv_obj_set_y(ui->screen_img_m36, 530);
            lv_obj_set_y(ui->screen_img_m37, 550);
            lv_obj_set_style_img_opa(ui->screen_img_m33, 0, 0);
            obj_linear_anim(ui->screen_img_m34, 255, 0, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m35, 255, 0, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m36, 255, 0, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m37, 255, 0, 250,lv_obj_set_img_opa);
            lv_obj_clear_flag(ui->screen_img_m33, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m34, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m35, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m36, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m37, LV_OBJ_FLAG_CLICKABLE);
        }

        lv_obj_clear_flag(ui->screen_sw_2, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(ui->screen_sw_3, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(ui->screen_sw_4, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_clear_flag(ui->screen_sw_5, LV_OBJ_FLAG_CLICKABLE);
    }

    lv_obj_set_ext_click_area(ui->screen_img_m15, 10);
    lv_obj_set_ext_click_area(ui->screen_img_m16, 10);
    lv_obj_set_ext_click_area(ui->screen_img_m17, 10);
    lv_obj_set_ext_click_area(ui->screen_img_m18, 10);
    lv_obj_set_ext_click_area(ui->screen_img_m19, 10);
    lv_obj_set_ext_click_area(ui->screen_img_m21, 10);
    lv_obj_set_ext_click_area(ui->screen_img_m22, 10);
    lv_obj_set_ext_click_area(ui->screen_img_m23, 10);
    lv_obj_set_ext_click_area(ui->screen_img_m24, 10);
    lv_obj_set_ext_click_area(ui->screen_img_m25, 10);
    lv_obj_set_ext_click_area(ui->screen_img_m27, 10);
    lv_obj_set_ext_click_area(ui->screen_img_m28, 10);
    lv_obj_set_ext_click_area(ui->screen_img_m29, 10);
    lv_obj_set_ext_click_area(ui->screen_img_m30, 10);
    lv_obj_set_ext_click_area(ui->screen_img_m31, 10);
    lv_obj_set_ext_click_area(ui->screen_img_m33, 10);
    lv_obj_set_ext_click_area(ui->screen_img_m34, 10);
    lv_obj_set_ext_click_area(ui->screen_img_m35, 10);
    lv_obj_set_ext_click_area(ui->screen_img_m36, 10);
    lv_obj_set_ext_click_area(ui->screen_img_m37, 10);
}

void screen_light_open(lv_ui* ui, int8_t sw_sign)
{
    ui_M3_t* ui_data = get_global_ui_data();
    switch (sw_sign)
    {
    case 0:
        if(ui_data->home_light.light_option[0].color == true){
            // 展开颜色选项
            // lv_obj_set_y(ui->screen_img_m15, 378);
            // lv_obj_set_y(ui->screen_img_m16, 383);
            // lv_obj_set_y(ui->screen_img_m17, 408);
            // lv_obj_set_y(ui->screen_img_m18, 428);
            // lv_obj_set_y(ui->screen_img_m19, 448);
            obj_linear_anim(ui->screen_img_m16, 383, 383, 250, lv_obj_set_y);
            obj_linear_anim(ui->screen_img_m17, 388, 408, 250, lv_obj_set_y);
            obj_linear_anim(ui->screen_img_m18, 388, 428, 250, lv_obj_set_y);
            obj_linear_anim(ui->screen_img_m19, 388, 448, 250, lv_obj_set_y);
            lv_obj_set_style_img_opa(ui->screen_img_m15, 0, 0);
            obj_linear_anim(ui->screen_img_m16, 0, 255, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m17, 0, 255, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m18, 0, 255, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m19, 0, 255, 250,lv_obj_set_img_opa);

            lv_obj_clear_flag(ui->screen_img_m15, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m16, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m17, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m18, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m19, LV_OBJ_FLAG_CLICKABLE);
        }else{
            lv_obj_set_pos(ui->screen_img_m15, 768, 378);
            obj_linear_anim(ui->screen_img_m16, 383, 383, 250, lv_obj_set_y);
            obj_linear_anim(ui->screen_img_m17, 408, 388, 250, lv_obj_set_y);
            obj_linear_anim(ui->screen_img_m18, 428, 388, 250, lv_obj_set_y);
            obj_linear_anim(ui->screen_img_m19, 448, 388, 250, lv_obj_set_y);
            obj_linear_anim(ui->screen_img_m15, 0, 255, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m16, 255, 0, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m17, 255, 0, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m18, 255, 0, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m19, 255, 0, 250,lv_obj_set_img_opa);

            lv_obj_add_flag(ui->screen_img_m15, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m16, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m17, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m18, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m19, LV_OBJ_FLAG_CLICKABLE);
        }
        break;
    case 1:
        if(ui_data->home_light.light_option[1].color == true){
            // 展开颜色选项
            obj_linear_anim(ui->screen_img_m22, 383, 383, 250, lv_obj_set_y);
            obj_linear_anim(ui->screen_img_m23, 388, 408, 250, lv_obj_set_y);
            obj_linear_anim(ui->screen_img_m24, 388, 428, 250, lv_obj_set_y);
            obj_linear_anim(ui->screen_img_m25, 388, 448, 250, lv_obj_set_y);
            lv_obj_set_style_img_opa(ui->screen_img_m21, 0, 0);
            obj_linear_anim(ui->screen_img_m22, 0, 255, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m23, 0, 255, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m24, 0, 255, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m25, 0, 255, 250,lv_obj_set_img_opa);

            lv_obj_clear_flag(ui->screen_img_m21, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m22, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m23, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m24, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m25, LV_OBJ_FLAG_CLICKABLE);
        }else{
            lv_obj_set_pos(ui->screen_img_m21, 916, 378);
            obj_linear_anim(ui->screen_img_m22, 383, 383, 250, lv_obj_set_y);
            obj_linear_anim(ui->screen_img_m23, 408, 388, 250, lv_obj_set_y);
            obj_linear_anim(ui->screen_img_m24, 428, 388, 250, lv_obj_set_y);
            obj_linear_anim(ui->screen_img_m25, 448, 388, 250, lv_obj_set_y);
            obj_linear_anim(ui->screen_img_m21, 0, 255, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m22, 255, 0, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m23, 255, 0, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m24, 255, 0, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m25, 255, 0, 250,lv_obj_set_img_opa);

            lv_obj_add_flag(ui->screen_img_m21, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m22, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m23, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m24, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m25, LV_OBJ_FLAG_CLICKABLE);
        }
        break;
    case 2:
        if(ui_data->home_light.light_option[2].color == true){
            // 展开颜色选项
            obj_linear_anim(ui->screen_img_m28, 485, 485, 250, lv_obj_set_y);
            obj_linear_anim(ui->screen_img_m29, 490, 510, 250, lv_obj_set_y);
            obj_linear_anim(ui->screen_img_m30, 490, 530, 250, lv_obj_set_y);
            obj_linear_anim(ui->screen_img_m31, 490, 550, 250, lv_obj_set_y);
            lv_obj_set_style_img_opa(ui->screen_img_m27, 0, 0);
            obj_linear_anim(ui->screen_img_m28, 0, 255, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m29, 0, 255, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m30, 0, 255, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m31, 0, 255, 250,lv_obj_set_img_opa);

            lv_obj_clear_flag(ui->screen_img_m27, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m28, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m29, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m30, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m31, LV_OBJ_FLAG_CLICKABLE);
        }else{
            lv_obj_set_pos(ui->screen_img_m27, 768, 480);
            obj_linear_anim(ui->screen_img_m28, 485, 485, 250, lv_obj_set_y);
            obj_linear_anim(ui->screen_img_m29, 510, 490, 250, lv_obj_set_y);
            obj_linear_anim(ui->screen_img_m30, 530, 490, 250, lv_obj_set_y);
            obj_linear_anim(ui->screen_img_m31, 550, 490, 250, lv_obj_set_y);
            obj_linear_anim(ui->screen_img_m27, 0, 255, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m28, 255, 0, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m29, 255, 0, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m30, 255, 0, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m31, 255, 0, 250,lv_obj_set_img_opa);

            lv_obj_add_flag(ui->screen_img_m27, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m28, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m29, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m30, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m31, LV_OBJ_FLAG_CLICKABLE);
        }
        break;
    case 3:
        if(ui_data->home_light.light_option[3].color == true){
            // 展开颜色选项
            obj_linear_anim(ui->screen_img_m34, 485, 485, 250, lv_obj_set_y);
            obj_linear_anim(ui->screen_img_m35, 490, 510, 250, lv_obj_set_y);
            obj_linear_anim(ui->screen_img_m36, 490, 530, 250, lv_obj_set_y);
            obj_linear_anim(ui->screen_img_m37, 490, 550, 250, lv_obj_set_y);
            lv_obj_set_style_img_opa(ui->screen_img_m33, 0, 0);
            obj_linear_anim(ui->screen_img_m34, 0, 255, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m35, 0, 255, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m36, 0, 255, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m37, 0, 255, 250,lv_obj_set_img_opa);

            lv_obj_clear_flag(ui->screen_img_m33, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m34, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m35, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m36, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m37, LV_OBJ_FLAG_CLICKABLE);
        }else{
            lv_obj_set_pos(ui->screen_img_m33, 916, 480);
            obj_linear_anim(ui->screen_img_m34, 485, 485, 250, lv_obj_set_y);
            obj_linear_anim(ui->screen_img_m35, 510, 490, 250, lv_obj_set_y);
            obj_linear_anim(ui->screen_img_m36, 530, 490, 250, lv_obj_set_y);
            obj_linear_anim(ui->screen_img_m37, 550, 490, 250, lv_obj_set_y);
            obj_linear_anim(ui->screen_img_m33, 0, 255, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m34, 255, 0, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m35, 255, 0, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m36, 255, 0, 250,lv_obj_set_img_opa);
            obj_linear_anim(ui->screen_img_m37, 255, 0, 250,lv_obj_set_img_opa);

            lv_obj_add_flag(ui->screen_img_m33, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m34, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m35, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m36, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m37, LV_OBJ_FLAG_CLICKABLE);
        }
        break;
    default:
        break;
    }
}

int8_t light_color[5] = {
    LIGHT_YELLOW,LIGHT_PINK,LIGHT_PURPLE,LIGHT_BLUE,LIGHT_OFF
};

void screen_light_change_color(lv_ui* ui, int8_t sw_sign)
{
    bool sw_switch = false;
    ui_M3_t* ui_data = get_global_ui_data();
    switch (sw_sign)
    {
    case 0:
        // if(lv_obj_get_state(ui->screen_sw_2) == LV_STATE_CHECKED){
        // if(lv_obj_get_state(ui->screen_sw_2) == 3){
        if(ui_data->home_light.light_option[0].switch_sign){
            sw_switch = true;
            printf("screen_sw_2: light %d\r\n", ui_data->home_light.light_option[0].color_sign);
            lv_img_set_src(ui->screen_img_m14, read_png_path[light_color[ui_data->home_light.light_option[0].color_sign]]);
            // if(ui_data->home_light.light_option[0].color){
            //     screen_light_open(ui, 0);
            // }
        }else{
            printf("screen_sw_2: nolight\r\n");
            lv_img_set_src(ui->screen_img_m14, read_png_path[light_color[4]]);
            if(ui_data->home_light.light_option[0].color){
                ui_data->home_light.light_option[0].color = false;
                screen_light_open(ui, 0);
            }
        }
        break;
    case 1:
        // if(lv_obj_get_state(ui->screen_sw_3) == LV_STATE_CHECKED){
        // if(lv_obj_get_state(ui->screen_sw_3) == 3){
        if(ui_data->home_light.light_option[1].switch_sign){
            sw_switch = true;
            printf("screen_sw_3: light %d\r\n", ui_data->home_light.light_option[1].color_sign);
            lv_img_set_src(ui->screen_img_m20, read_png_path[light_color[ui_data->home_light.light_option[1].color_sign]]);
            // if(ui_data->home_light.light_option[1].color){
            //     screen_light_open(ui, 1);
            // }
        }else{
            lv_img_set_src(ui->screen_img_m20, read_png_path[light_color[4]]);
            if(ui_data->home_light.light_option[1].color){
                ui_data->home_light.light_option[1].color = false;
                screen_light_open(ui, 1);
            }
        }
        break;
    case 2:
        // if(lv_obj_get_state(ui->screen_sw_4) == LV_STATE_CHECKED){
        // if(lv_obj_get_state(ui->screen_sw_4) == 3){
        if(ui_data->home_light.light_option[2].switch_sign){
            sw_switch = true;
            printf("screen_sw_4: light %d\r\n", ui_data->home_light.light_option[2].color_sign);
            lv_img_set_src(ui->screen_img_m26, read_png_path[light_color[ui_data->home_light.light_option[2].color_sign]]);
            // if(ui_data->home_light.light_option[2].color){
            //     screen_light_open(ui, 2);
            // }
        }else{
            lv_img_set_src(ui->screen_img_m26, read_png_path[light_color[4]]);
            if(ui_data->home_light.light_option[2].color){
                ui_data->home_light.light_option[2].color = false;
                screen_light_open(ui, 2);
            }
        }
        break;
    case 3:
        // if(lv_obj_get_state(ui->screen_sw_5) == LV_STATE_CHECKED){
        // if(lv_obj_get_state(ui->screen_sw_5) == 3){
        if(ui_data->home_light.light_option[3].switch_sign){
            sw_switch = true;
            printf("screen_sw_5: light %d\r\n", ui_data->home_light.light_option[3].color_sign);
            lv_img_set_src(ui->screen_img_m32, read_png_path[light_color[ui_data->home_light.light_option[3].color_sign]]);
            // if(ui_data->home_light.light_option[3].color){
            //     screen_light_open(ui, 3);
            // }
        }else{
            lv_img_set_src(ui->screen_img_m32, read_png_path[light_color[4]]);
            if(ui_data->home_light.light_option[3].color){
                ui_data->home_light.light_option[3].color = false;
                screen_light_open(ui, 3);
            }
        }
        break;
    default:
        break;
    }
}
/*================== screen_1 ====================*/

void lv_screen_1_load(lv_ui* ui)
{
    obj_overshoot_anim(ui->screen_1_img_2, -277, -5, 500, lv_obj_set_x);
    obj_overshoot_anim(ui->screen_1_img_3, -247, 8, 500, lv_obj_set_y);
    obj_overshoot_anim(ui->screen_1_img_4, (253+346+20), 253, 500, lv_obj_set_y);
}

void lv_screen_1_unload(lv_ui* ui)
{
    obj_overshoot_anim(ui->screen_1_img_2, -5, -277, 500, lv_obj_set_x);
    obj_overshoot_anim(ui->screen_1_img_3, 8, -247, 500, lv_obj_set_y);
    obj_overshoot_anim(ui->screen_1_img_4, 253, (253+346+20), 500, lv_obj_set_y);
    back_scr_delay(NULL, 400);
}
