#include "event_handle.h"

#include "bee_anim.h"


/* ============================= screen ============================= */
void screen_1_load_fun(lv_ui* ui)
{
    lv_indev_wait_release(lv_indev_get_act());/*Do nothing until the next release 在下一次发布之前不执行任何操作*/
    // setup_scr_screen_1(ui);
    lv_scr_load(ui->screen_1);
}

void screen_event_handler(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_ui* bee = lv_event_get_user_data(e);

    // if(code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT){
    //     lv_indev_wait_release(lv_indev_get_act());/*Do nothing until the next release 在下一次发布之前不执行任何操作*/
    //     setup_scr_screen_1(bee);
    //     // lv_scr_load_anim(bee->screen_2, LV_SCR_LOAD_ANIM_FADE_IN, 0, 0, true);
    //     lv_scr_load_anim(bee->screen_1, LV_SCR_LOAD_ANIM_FADE_IN, 500, 10, true);
    // }
    // if(code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT){
    //     // lv_indev_wait_release(lv_indev_get_act());/*Do nothing until the next release 在下一次发布之前不执行任何操作*/
    //     // main_3_to_sign = false;
    //     // setup_scr_screen_4(bee);
    //     // lv_scr_load_anim(bee->screen_4, LV_SCR_LOAD_ANIM_FADE_IN, 500, 10, true);
    // }

    if(code == LV_EVENT_SCREEN_UNLOAD_START){
        // screen_anim_end(bee);
        // screen_unload_anim(bee);
    }

    if(code == LV_EVENT_SCREEN_LOAD_START){
        // screen_load_anim(bee);
    }

    if(code == LV_EVENT_DRAW_POST_END){
    }
}

void screen_img_1_event_handler(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_ui* ui = lv_event_get_user_data(e);

    if(code == LV_EVENT_CLICKED){
        lv_indev_wait_release(lv_indev_get_act());
        ui_M3_t* ui_data = get_global_ui_data();
        if(ui_data->home_mode_option.mode == 0){
            // 放大
            screen_img_1_zoom(ui, true);
            ui_data->home_mode_option.mode = 1;
        }else if(ui_data->home_mode_option.mode == 1){
            // 缩小
            screen_img_1_zoom(ui, false);
            ui_data->home_mode_option.mode = 0;
        }
    }
}

void screen_img_2_event_handler(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_ui* ui = lv_event_get_user_data(e);

    if(code == LV_EVENT_CLICKED){
        lv_indev_wait_release(lv_indev_get_act());
        ui_M3_t* ui_data = get_global_ui_data();
        if(ui_data->home_mode_option.mode == 0){
            // 放大
            screen_img_2_zoom(ui, true);
            ui_data->home_mode_option.mode = 2;
        }else if(ui_data->home_mode_option.mode == 2){
            // 缩小
            screen_img_2_zoom(ui, false);
            ui_data->home_mode_option.mode = 0;
        }
    }
}

void screen_img_3_event_handler(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_ui* ui = lv_event_get_user_data(e);

    if(code == LV_EVENT_CLICKED){
        lv_indev_wait_release(lv_indev_get_act());
        ui_M3_t* ui_data = get_global_ui_data();
        if(ui_data->home_mode_option.mode == 0){
            // 放大
            screen_img_3_zoom(ui, true);
            ui_data->home_mode_option.mode = 3;
        }else if(ui_data->home_mode_option.mode == 3){
            // 缩小
            screen_img_3_zoom(ui, false);
            ui_data->home_mode_option.mode = 0;
        }
    }
}

void screen_img_m4_event_handler(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_ui* ui = lv_event_get_user_data(e);

    if(code == LV_EVENT_CLICKED){
        lv_indev_wait_release(lv_indev_get_act());
        ui_M3_t* ui_data = get_global_ui_data();
        if(ui_data->lift_mode_option.mode == 1){
            // 从音乐转向窗帘
            ui_data->lift_mode_option.mode = 2;
        }else if(ui_data->lift_mode_option.mode == 2){
            // 从窗帘转向音乐
            ui_data->lift_mode_option.mode = 1;
        }
        screen_img_m4_anim(ui, ui_data->lift_mode_option.mode);
    }
}

void screen_img_m6_event_handler(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_ui* ui = lv_event_get_user_data(e);

    if(code == LV_EVENT_CLICKED){
        lv_indev_wait_release(lv_indev_get_act());
        ui_M3_t* ui_data = get_global_ui_data();

        int8_t wea_index = 1;
        if(wea_index == ui_data->week_wea.wea_index){
            // 将页面切换到下一个页面
            screen_1_load_fun(ui);
        }else{
            ui_data->week_wea.wea_index = wea_index;
            // 信息改变动画
            screen_wea_anim(ui, wea_index);
        }
    }
}
void screen_img_m7_event_handler(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_ui* ui = lv_event_get_user_data(e);

    if(code == LV_EVENT_CLICKED){
        lv_indev_wait_release(lv_indev_get_act());
        ui_M3_t* ui_data = get_global_ui_data();

        int8_t wea_index = 2;
        if(wea_index == ui_data->week_wea.wea_index){
            // 将页面切换到下一个页面
            screen_1_load_fun(ui);
        }else{
            ui_data->week_wea.wea_index = wea_index;
            // 信息改变动画
            screen_wea_anim(ui, wea_index);
        }
    }
}
void screen_img_m8_event_handler(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_ui* ui = lv_event_get_user_data(e);

    if(code == LV_EVENT_CLICKED){
        lv_indev_wait_release(lv_indev_get_act());
        ui_M3_t* ui_data = get_global_ui_data();

        int8_t wea_index = 3;
        if(wea_index == ui_data->week_wea.wea_index){
            // 将页面切换到下一个页面
            screen_1_load_fun(ui);
        }else{
            ui_data->week_wea.wea_index = wea_index;
            // 信息改变动画
            screen_wea_anim(ui, wea_index);
        }
    }
}
void screen_img_m9_event_handler(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_ui* ui = lv_event_get_user_data(e);

    if(code == LV_EVENT_CLICKED){
        lv_indev_wait_release(lv_indev_get_act());
        ui_M3_t* ui_data = get_global_ui_data();

        int8_t wea_index = 4;
        if(wea_index == ui_data->week_wea.wea_index){
            // 将页面切换到下一个页面
            screen_1_load_fun(ui);
        }else{
            ui_data->week_wea.wea_index = wea_index;
            // 信息改变动画
            screen_wea_anim(ui, wea_index);
        }
    }
}
void screen_img_m10_event_handler(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_ui* ui = lv_event_get_user_data(e);

    if(code == LV_EVENT_CLICKED){
        lv_indev_wait_release(lv_indev_get_act());
        ui_M3_t* ui_data = get_global_ui_data();

        int8_t wea_index = 5;
        if(wea_index == ui_data->week_wea.wea_index){
            // 将页面切换到下一个页面
            screen_1_load_fun(ui);
        }else{
            ui_data->week_wea.wea_index = wea_index;
            // 信息改变动画
            screen_wea_anim(ui, wea_index);
        }
    }
}

void screen_label_39_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_ui* ui = lv_event_get_user_data(e);

    if(code == LV_EVENT_CLICKED){
        lv_indev_wait_release(lv_indev_get_act());
        ui_M3_t* ui_data = get_global_ui_data();

        if(ui_data->home_light.option_sign){
            // printf("灯光关闭\r\n");
            ui_data->home_light.option_sign = false;
            // lv_obj_clear_state(ui->screen_sw_1, LV_STATE_CHECKED);
        }else{
            // printf("灯光打开\r\n");
            ui_data->home_light.option_sign = true;
            // lv_obj_add_state(ui->screen_sw_1, LV_STATE_CHECKED);
        }
        screen_light_anim(ui);
    }
}
// void screen_light_change_color(lv_ui* ui, int8_t sw_sign);
#if 1//小灯图标事件回调函数
void screen_img_m15_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    // printf("code: %d\r\n", e->code);
    if(e->code == LV_EVENT_CLICKED){
        // printf("======\r\n");
        lv_ui* ui = lv_event_get_user_data(e);

        lv_indev_wait_release(lv_indev_get_act());
        ui_M3_t* ui_data = get_global_ui_data();

        if(ui_data->home_light.option_sign == true){
            if(ui_data->home_light.light_option[0].color){
                ui_data->home_light.light_option[0].color = false;
            }else{
                ui_data->home_light.light_option[0].color = true;
                screen_light_open(ui, 0);
            }
            // screen_light_open(ui, 0);
        }
    }
}
void screen_img_m16_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(e->code == LV_EVENT_CLICKED){
        lv_ui* ui = lv_event_get_user_data(e);

        lv_indev_wait_release(lv_indev_get_act());
        ui_M3_t* ui_data = get_global_ui_data();  
        
        if(ui_data->home_light.option_sign == true && ui_data->home_light.light_option[0].switch_sign == true){
            ui_data->home_light.light_option[0].color_sign = 0;
            screen_light_change_color(ui, 0);
        }
    }
}
void screen_img_m17_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(e->code == LV_EVENT_CLICKED){
        lv_ui* ui = lv_event_get_user_data(e);

        lv_indev_wait_release(lv_indev_get_act());
        ui_M3_t* ui_data = get_global_ui_data();  
        
        if(ui_data->home_light.option_sign == true && ui_data->home_light.light_option[0].switch_sign == true){
            ui_data->home_light.light_option[0].color_sign = 1;
            screen_light_change_color(ui, 0);
        }
    }
}
void screen_img_m18_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(e->code == LV_EVENT_CLICKED){
        lv_ui* ui = lv_event_get_user_data(e);

        lv_indev_wait_release(lv_indev_get_act());
        ui_M3_t* ui_data = get_global_ui_data();  
        
        if(ui_data->home_light.option_sign == true && ui_data->home_light.light_option[0].switch_sign == true){
            ui_data->home_light.light_option[0].color_sign = 2;
            screen_light_change_color(ui, 0);
        }
    }
}
void screen_img_m19_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(e->code == LV_EVENT_CLICKED){
        lv_ui* ui = lv_event_get_user_data(e);

        lv_indev_wait_release(lv_indev_get_act());
        ui_M3_t* ui_data = get_global_ui_data();  
        
        if(ui_data->home_light.option_sign == true && ui_data->home_light.light_option[0].switch_sign == true){
            ui_data->home_light.light_option[0].color_sign = 3;
            screen_light_change_color(ui, 0);
        }
    }
}
void screen_img_m21_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(e->code == LV_EVENT_CLICKED){
    lv_ui* ui = lv_event_get_user_data(e);

    lv_indev_wait_release(lv_indev_get_act());
    ui_M3_t* ui_data = get_global_ui_data();

    if(ui_data->home_light.option_sign == true){
        if(ui_data->home_light.light_option[1].color){
            ui_data->home_light.light_option[1].color = false;
        }else{
            ui_data->home_light.light_option[1].color = true;
            screen_light_open(ui, 1);
        }
    }
    }
}
void screen_img_m22_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(e->code == LV_EVENT_CLICKED){
        lv_ui* ui = lv_event_get_user_data(e);

        lv_indev_wait_release(lv_indev_get_act());
        ui_M3_t* ui_data = get_global_ui_data();  
        
        if(ui_data->home_light.option_sign == true && ui_data->home_light.light_option[1].switch_sign == true){
            ui_data->home_light.light_option[1].color_sign = 0;
            screen_light_change_color(ui, 1);
        }
    }
}
void screen_img_m23_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(e->code == LV_EVENT_CLICKED){
        lv_ui* ui = lv_event_get_user_data(e);

        lv_indev_wait_release(lv_indev_get_act());
        ui_M3_t* ui_data = get_global_ui_data();  
        
        if(ui_data->home_light.option_sign == true && ui_data->home_light.light_option[1].switch_sign == true){
            ui_data->home_light.light_option[1].color_sign = 1;
            screen_light_change_color(ui, 1);
        }
    }
}
void screen_img_m24_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(e->code == LV_EVENT_CLICKED){
        lv_ui* ui = lv_event_get_user_data(e);

        lv_indev_wait_release(lv_indev_get_act());
        ui_M3_t* ui_data = get_global_ui_data();  
        
        if(ui_data->home_light.option_sign == true && ui_data->home_light.light_option[1].switch_sign == true){
            ui_data->home_light.light_option[1].color_sign = 2;
            screen_light_change_color(ui, 1);
        }
    }
}
void screen_img_m25_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(e->code == LV_EVENT_CLICKED){
        lv_ui* ui = lv_event_get_user_data(e);

        lv_indev_wait_release(lv_indev_get_act());
        ui_M3_t* ui_data = get_global_ui_data();  
        
        if(ui_data->home_light.option_sign == true && ui_data->home_light.light_option[1].switch_sign == true){
            ui_data->home_light.light_option[1].color_sign = 3;
            screen_light_change_color(ui, 1);
        }
    }
}
void screen_img_m27_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(e->code == LV_EVENT_CLICKED){
    lv_ui* ui = lv_event_get_user_data(e);

    lv_indev_wait_release(lv_indev_get_act());
    ui_M3_t* ui_data = get_global_ui_data();

    if(ui_data->home_light.option_sign == true){
        if(ui_data->home_light.light_option[2].color){
            ui_data->home_light.light_option[2].color = false;
        }else{
            ui_data->home_light.light_option[2].color = true;
            screen_light_open(ui, 2);
        }
    }
    }
}
void screen_img_m28_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(e->code == LV_EVENT_CLICKED){
        lv_ui* ui = lv_event_get_user_data(e);

        lv_indev_wait_release(lv_indev_get_act());
        ui_M3_t* ui_data = get_global_ui_data();  
        
        if(ui_data->home_light.option_sign == true && ui_data->home_light.light_option[2].switch_sign == true){
            ui_data->home_light.light_option[2].color_sign = 0;
            screen_light_change_color(ui, 2);
        }
    }
}
void screen_img_m29_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(e->code == LV_EVENT_CLICKED){
        lv_ui* ui = lv_event_get_user_data(e);

        lv_indev_wait_release(lv_indev_get_act());
        ui_M3_t* ui_data = get_global_ui_data();  
        
        if(ui_data->home_light.option_sign == true && ui_data->home_light.light_option[2].switch_sign == true){
            ui_data->home_light.light_option[2].color_sign = 1;
            screen_light_change_color(ui, 2);
        }
    }
}
void screen_img_m30_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(e->code == LV_EVENT_CLICKED){
        lv_ui* ui = lv_event_get_user_data(e);

        lv_indev_wait_release(lv_indev_get_act());
        ui_M3_t* ui_data = get_global_ui_data();  
        
        if(ui_data->home_light.option_sign == true && ui_data->home_light.light_option[2].switch_sign == true){
            ui_data->home_light.light_option[2].color_sign = 2;
            screen_light_change_color(ui, 2);
        }
    }
}
void screen_img_m31_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(e->code == LV_EVENT_CLICKED){
        lv_ui* ui = lv_event_get_user_data(e);

        lv_indev_wait_release(lv_indev_get_act());
        ui_M3_t* ui_data = get_global_ui_data();  
        
        if(ui_data->home_light.option_sign == true && ui_data->home_light.light_option[2].switch_sign == true){
            ui_data->home_light.light_option[2].color_sign = 3;
            screen_light_change_color(ui, 2);
        }
    }
}
void screen_img_m33_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(e->code == LV_EVENT_CLICKED){
    lv_ui* ui = lv_event_get_user_data(e);

    lv_indev_wait_release(lv_indev_get_act());
    ui_M3_t* ui_data = get_global_ui_data();

    if(ui_data->home_light.option_sign == true){
        if(ui_data->home_light.light_option[3].color){
            ui_data->home_light.light_option[3].color = false;
        }else{
            ui_data->home_light.light_option[3].color = true;
            screen_light_open(ui, 3);
        }
    }
    }
}
void screen_img_m34_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(e->code == LV_EVENT_CLICKED){
        lv_ui* ui = lv_event_get_user_data(e);

        lv_indev_wait_release(lv_indev_get_act());
        ui_M3_t* ui_data = get_global_ui_data();  
        
        if(ui_data->home_light.option_sign == true && ui_data->home_light.light_option[3].switch_sign == true){
            ui_data->home_light.light_option[3].color_sign = 0;
            screen_light_change_color(ui, 3);
        }
    }
}
void screen_img_m35_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(e->code == LV_EVENT_CLICKED){
        lv_ui* ui = lv_event_get_user_data(e);

        lv_indev_wait_release(lv_indev_get_act());
        ui_M3_t* ui_data = get_global_ui_data();  
        
        if(ui_data->home_light.option_sign == true && ui_data->home_light.light_option[3].switch_sign == true){
            ui_data->home_light.light_option[3].color_sign = 1;
            screen_light_change_color(ui, 3);
        }
    }
}
void screen_img_m36_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(e->code == LV_EVENT_CLICKED){
        lv_ui* ui = lv_event_get_user_data(e);

        lv_indev_wait_release(lv_indev_get_act());
        ui_M3_t* ui_data = get_global_ui_data();  
        
        if(ui_data->home_light.option_sign == true && ui_data->home_light.light_option[3].switch_sign == true){
            ui_data->home_light.light_option[3].color_sign = 2;
            screen_light_change_color(ui, 3);
        }
    }
}
void screen_img_m37_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(e->code == LV_EVENT_CLICKED){
        lv_ui* ui = lv_event_get_user_data(e);

        lv_indev_wait_release(lv_indev_get_act());
        ui_M3_t* ui_data = get_global_ui_data();  
        
        if(ui_data->home_light.option_sign == true && ui_data->home_light.light_option[3].switch_sign == true){
            ui_data->home_light.light_option[3].color_sign = 3;
            screen_light_change_color(ui, 3);
        }
    }
}

void screen_sw_2_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(e->code == LV_EVENT_CLICKED){
        lv_ui* ui = lv_event_get_user_data(e);
        ui_M3_t* ui_data = get_global_ui_data();

        printf("sw_2 state: %d\r\n", lv_obj_get_state(ui->screen_sw_2));

        if(lv_obj_get_state(ui->screen_sw_2) == 3){
            ui_data->home_light.light_option[0].switch_sign = true;
        }else{
            ui_data->home_light.light_option[0].switch_sign = false;
        }

        screen_light_change_color(ui, 0);
    }
}
void screen_sw_3_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(e->code == LV_EVENT_CLICKED){
        lv_ui* ui = lv_event_get_user_data(e);
        ui_M3_t* ui_data = get_global_ui_data();

        if(lv_obj_get_state(ui->screen_sw_3) == 3){
            ui_data->home_light.light_option[1].switch_sign = true;
        }else{
            ui_data->home_light.light_option[1].switch_sign = false;
        }

        screen_light_change_color(ui, 1);
    }
}
void screen_sw_4_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(e->code == LV_EVENT_CLICKED){
        lv_ui* ui = lv_event_get_user_data(e);
        ui_M3_t* ui_data = get_global_ui_data();

        if(lv_obj_get_state(ui->screen_sw_4) == 3){
            ui_data->home_light.light_option[2].switch_sign = true;
        }else{
            ui_data->home_light.light_option[2].switch_sign = false;
        }

        screen_light_change_color(ui, 2);
    }
}
void screen_sw_5_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(e->code == LV_EVENT_CLICKED){
        lv_ui* ui = lv_event_get_user_data(e);
        ui_M3_t* ui_data = get_global_ui_data();

        if(lv_obj_get_state(ui->screen_sw_5) == 3){
            ui_data->home_light.light_option[3].switch_sign = true;
        }else{
            ui_data->home_light.light_option[3].switch_sign = false;
        }

        screen_light_change_color(ui, 3);
    }
}
#endif

/* ============================= screen_1 ============================= */
void screen_1_event_handler(lv_event_t *e){
    lv_event_code_t code = lv_event_get_code(e);
    lv_ui* bee = lv_event_get_user_data(e);

    // if(code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT){
    //     lv_indev_wait_release(lv_indev_get_act());/*Do nothing until the next release 在下一次发布之前不执行任何操作*/
    //     setup_scr_screen(bee);
    //     // lv_scr_load_anim(bee->screen_2, LV_SCR_LOAD_ANIM_FADE_IN, 0, 0, true);
    //     lv_scr_load_anim(bee->screen, LV_SCR_LOAD_ANIM_FADE_IN, 500, 10, true);
    // }
    // if(code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT){
    //     // lv_indev_wait_release(lv_indev_get_act());/*Do nothing until the next release 在下一次发布之前不执行任何操作*/
    //     // main_3_to_sign = false;
    //     // setup_scr_screen_4(bee);
    //     // lv_scr_load_anim(bee->screen_4, LV_SCR_LOAD_ANIM_FADE_IN, 500, 10, true);
    // }

    if(code == LV_EVENT_SCREEN_UNLOAD_START){
        // screen_anim_end(bee);
        // screen_unload_anim(bee);
        // lv_screen_1_unload(bee);
    }

    if(code == LV_EVENT_SCREEN_LOAD_START){
        // screen_load_anim(bee);
        lv_screen_1_load(bee);
        lv_img_cache_invalidate_src(NULL);
    }

    if(code == LV_EVENT_DRAW_POST_END){

    }
}

void screen_img_m2_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_ui* ui = lv_event_get_user_data(e);

    if(e->code == LV_EVENT_CLICKED){
        lv_indev_wait_release(lv_indev_get_act());/*Do nothing until the next release 在下一次发布之前不执行任何操作*/
        // setup_scr_screen(ui);
        if(lv_anim_count_running())
            return;
        set_bk_scr(ui->screen);
        lv_screen_1_unload(ui);
    }
}
