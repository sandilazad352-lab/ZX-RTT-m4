#include "bee_ui_init.h"



void screen_mode_ui_init(int8_t mode_sign, lv_ui* ui)
{
    switch (mode_sign)
    {
    case 0:{
        lv_img_zoom(ui->screen_img_1, 151);
        lv_obj_set_pos(ui->screen_img_1, 433, 28);
        lv_img_zoom(ui->screen_img_2, 151);
        lv_obj_set_pos(ui->screen_img_2, 546, 28);
        lv_img_zoom(ui->screen_img_3, 151);
        lv_obj_set_pos(ui->screen_img_3, 488, 118);
        break;}
    case 1:{
        lv_img_zoom(ui->screen_img_1, 256);
        lv_obj_set_pos(ui->screen_img_1, 475, 40);
        lv_img_zoom(ui->screen_img_2, 151);
        lv_obj_set_pos(ui->screen_img_2, 546, 28);
        lv_img_zoom(ui->screen_img_3, 151);
        lv_obj_set_pos(ui->screen_img_3, 488, 118);
        break;}
    case 2:{
        lv_img_zoom(ui->screen_img_1, 151);
        lv_obj_set_pos(ui->screen_img_1, 433, 28);
        lv_img_zoom(ui->screen_img_2, 256);
        lv_obj_set_pos(ui->screen_img_2, 475, 40);
        lv_img_zoom(ui->screen_img_3, 151);
        lv_obj_set_pos(ui->screen_img_3, 488, 118);
        break;}
    case 3:{
        lv_img_zoom(ui->screen_img_1, 151);
        lv_obj_set_pos(ui->screen_img_1, 433, 28);
        lv_img_zoom(ui->screen_img_2, 151);
        lv_obj_set_pos(ui->screen_img_2, 546, 28);
        lv_img_zoom(ui->screen_img_3, 256);
        lv_obj_set_pos(ui->screen_img_3, 475, 40);
        break;}
    default:
        break;
    }
}

void screen_music_ui_init(int8_t music_sign, lv_ui* ui)
{
    switch (music_sign)
    {
    case 1:{
        // 音乐
        lv_obj_set_y(ui->screen_img_m47, 0);
        lv_obj_set_style_img_opa(ui->screen_img_m47, 0, 0);
        lv_obj_set_y(ui->screen_img_m46, 18);
        lv_obj_set_style_img_opa(ui->screen_img_m46, 255, 0);
        lv_obj_set_y(ui->screen_img_m49, 320);
        lv_obj_set_style_img_opa(ui->screen_img_m49, 0, 0);
        lv_obj_set_y(ui->screen_img_m48, 296);
        lv_obj_set_style_img_opa(ui->screen_img_m48, 255, 0);
        lv_obj_set_y(ui->screen_label_32, 320);
        lv_obj_set_style_text_opa(ui->screen_label_32, 0, 0);
        lv_obj_set_y(ui->screen_slider_1, 270);
        lv_obj_set_style_opa(ui->screen_slider_1, 255, 0);
        lv_obj_set_y(ui->screen_label_30, 190);
        lv_obj_set_y(ui->screen_label_31, 215);
        lv_label_set_text(ui->screen_label_30, "Sharpen Edge");
        lv_label_set_text(ui->screen_label_31, "Gallant");
        break;}
    case 2:{
        // 空调
        lv_obj_set_y(ui->screen_img_m47, 90);
        lv_obj_set_style_img_opa(ui->screen_img_m47, 255, 0);
        lv_obj_set_y(ui->screen_img_m46, -70);
        lv_obj_set_style_img_opa(ui->screen_img_m46, 0, 0);
        lv_obj_set_y(ui->screen_img_m49, 296);
        lv_obj_set_style_img_opa(ui->screen_img_m49, 255, 0);
        lv_obj_set_y(ui->screen_img_m48, 320);
        lv_obj_set_style_img_opa(ui->screen_img_m48, 0, 0);
        lv_obj_set_y(ui->screen_label_32, 298);
        lv_obj_set_style_text_opa(ui->screen_label_32, 255, 0);
        lv_obj_set_y(ui->screen_slider_1, 320);
        lv_obj_set_style_opa(ui->screen_slider_1, 0, 0);
        lv_obj_set_y(ui->screen_label_30, 230);
        lv_obj_set_y(ui->screen_label_31, 255);
        lv_label_set_text(ui->screen_label_30, "卧室窗帘");
        lv_label_set_text(ui->screen_label_31, "Living Room");
        break;}
    default:
        break;
    }
}

void screen_light_ui_init(lv_ui* ui)
{
    ui_M3_t* ui_data = get_global_ui_data();

    // ui_data->home_light.option_sign
    if(ui_data->home_light.option_sign){
        // 打开了
        // lv_obj_add_state(ui->screen_sw_1, LV_STATE_CHECKED);
        lv_obj_set_width(ui->screen_label_39, 545);
        lv_obj_set_x(ui->screen_img_m38, 1037);
        lv_obj_set_x(ui->screen_img_m39, 1228);

        lv_obj_set_style_opa(ui->screen_sw_2, 255, 0);
        lv_obj_set_style_opa(ui->screen_sw_3, 255, 0);
        lv_obj_set_style_opa(ui->screen_sw_4, 255, 0);
        lv_obj_set_style_opa(ui->screen_sw_5, 255, 0);

        lv_obj_set_x(ui->screen_sw_2, 718);
        lv_obj_set_x(ui->screen_sw_3, 866);
        lv_obj_set_x(ui->screen_sw_4, 718);
        lv_obj_set_x(ui->screen_sw_5, 866);

        lv_obj_set_style_opa(ui->screen_sw_1, 0, 0);
        lv_obj_set_style_img_opa(ui->screen_img_m13, 0, 0);
        lv_obj_set_style_text_opa(ui->screen_label_33, 0, 0);
        lv_obj_set_style_text_opa(ui->screen_label_34, 255, 0);

        lv_obj_set_style_img_opa(ui->screen_img_m14, 255, 0);
        // lv_obj_set_style_img_opa(ui->screen_img_m15, 255, 0);
        // lv_obj_set_style_img_opa(ui->screen_img_m16, 255, 0);
        // lv_obj_set_style_img_opa(ui->screen_img_m17, 255, 0);
        // lv_obj_set_style_img_opa(ui->screen_img_m18, 255, 0);
        // lv_obj_set_style_img_opa(ui->screen_img_m19, 255, 0);
        lv_obj_set_style_img_opa(ui->screen_img_m20, 255, 0);
        // lv_obj_set_style_img_opa(ui->screen_img_m21, 255, 0);
        // lv_obj_set_style_img_opa(ui->screen_img_m22, 255, 0);
        // lv_obj_set_style_img_opa(ui->screen_img_m23, 255, 0);
        // lv_obj_set_style_img_opa(ui->screen_img_m24, 255, 0);
        // lv_obj_set_style_img_opa(ui->screen_img_m25, 255, 0);
        lv_obj_set_style_img_opa(ui->screen_img_m26, 255, 0);
        // lv_obj_set_style_img_opa(ui->screen_img_m27, 255, 0);
        // lv_obj_set_style_img_opa(ui->screen_img_m28, 255, 0);
        // lv_obj_set_style_img_opa(ui->screen_img_m29, 255, 0);
        // lv_obj_set_style_img_opa(ui->screen_img_m30, 255, 0);
        // lv_obj_set_style_img_opa(ui->screen_img_m31, 255, 0);
        lv_obj_set_style_img_opa(ui->screen_img_m32, 255, 0);
        // lv_obj_set_style_img_opa(ui->screen_img_m33, 255, 0);
        // lv_obj_set_style_img_opa(ui->screen_img_m34, 255, 0);
        // lv_obj_set_style_img_opa(ui->screen_img_m35, 255, 0);
        // lv_obj_set_style_img_opa(ui->screen_img_m36, 255, 0);
        // lv_obj_set_style_img_opa(ui->screen_img_m37, 255, 0);

        lv_obj_set_x(ui->screen_img_m14, 717);
        lv_obj_set_x(ui->screen_img_m20, 865);
        lv_obj_set_x(ui->screen_img_m26, 717);
        lv_obj_set_x(ui->screen_img_m32, 865);

        if(ui_data->home_light.light_option[0].color == false){
            // 未展开颜色选项
            lv_obj_set_y(ui->screen_img_m15, 378);
            lv_obj_set_y(ui->screen_img_m16, 383+600);
            lv_obj_set_y(ui->screen_img_m17, 388+600);
            lv_obj_set_y(ui->screen_img_m18, 388+600);
            lv_obj_set_y(ui->screen_img_m19, 388+600);
            lv_obj_set_style_img_opa(ui->screen_img_m15, 255, 0);
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
            lv_obj_set_style_img_opa(ui->screen_img_m16, 255, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m17, 255, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m18, 255, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m19, 255, 0);
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
            lv_obj_set_style_img_opa(ui->screen_img_m21, 255, 0);
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
            lv_obj_set_style_img_opa(ui->screen_img_m22, 255, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m23, 255, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m24, 255, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m25, 255, 0);
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
            lv_obj_set_style_img_opa(ui->screen_img_m27, 255, 0);
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
            lv_obj_set_style_img_opa(ui->screen_img_m28, 255, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m29, 255, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m30, 255, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m31, 255, 0);
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
            lv_obj_set_style_img_opa(ui->screen_img_m33, 255, 0);
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
            lv_obj_set_style_img_opa(ui->screen_img_m34, 255, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m35, 255, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m36, 255, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m37, 255, 0);
            lv_obj_clear_flag(ui->screen_img_m33, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m34, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m35, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m36, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(ui->screen_img_m37, LV_OBJ_FLAG_CLICKABLE);
        }
    }else{
        // 未打开
        // lv_obj_clear_state(ui->screen_sw_1, LV_STATE_CHECKED);
        lv_obj_set_width(ui->screen_label_39, 189);
        lv_obj_set_x(ui->screen_img_m38, 661);
        lv_obj_set_x(ui->screen_img_m39, 852);

        lv_obj_set_style_opa(ui->screen_sw_2, 0, 0);
        lv_obj_set_style_opa(ui->screen_sw_3, 0, 0);
        lv_obj_set_style_opa(ui->screen_sw_4, 0, 0);
        lv_obj_set_style_opa(ui->screen_sw_5, 0, 0);

        lv_obj_set_x(ui->screen_sw_2, 628);
        lv_obj_set_x(ui->screen_sw_3, 776);
        lv_obj_set_x(ui->screen_sw_4, 628);
        lv_obj_set_x(ui->screen_sw_5, 776);

        lv_obj_set_style_opa(ui->screen_sw_1, 255, 0);
        lv_obj_set_style_img_opa(ui->screen_img_m13, 255, 0);
        lv_obj_set_style_text_opa(ui->screen_label_33, 255, 0);
        lv_obj_set_style_text_opa(ui->screen_label_34, 0, 0);

        lv_obj_set_style_img_opa(ui->screen_img_m14, 0, 0);
        // lv_obj_set_style_img_opa(ui->screen_img_m15, 0, 0);
        // lv_obj_set_style_img_opa(ui->screen_img_m16, 0, 0);
        // lv_obj_set_style_img_opa(ui->screen_img_m17, 0, 0);
        // lv_obj_set_style_img_opa(ui->screen_img_m18, 0, 0);
        // lv_obj_set_style_img_opa(ui->screen_img_m19, 0, 0);
        lv_obj_set_style_img_opa(ui->screen_img_m20, 0, 0);
        // lv_obj_set_style_img_opa(ui->screen_img_m21, 0, 0);
        // lv_obj_set_style_img_opa(ui->screen_img_m22, 0, 0);
        // lv_obj_set_style_img_opa(ui->screen_img_m23, 0, 0);
        // lv_obj_set_style_img_opa(ui->screen_img_m24, 0, 0);
        // lv_obj_set_style_img_opa(ui->screen_img_m25, 0, 0);
        lv_obj_set_style_img_opa(ui->screen_img_m26, 0, 0);
        // lv_obj_set_style_img_opa(ui->screen_img_m27, 0, 0);
        // lv_obj_set_style_img_opa(ui->screen_img_m28, 0, 0);
        // lv_obj_set_style_img_opa(ui->screen_img_m29, 0, 0);
        // lv_obj_set_style_img_opa(ui->screen_img_m30, 0, 0);
        // lv_obj_set_style_img_opa(ui->screen_img_m31, 0, 0);
        lv_obj_set_style_img_opa(ui->screen_img_m32, 0, 0);
        // lv_obj_set_style_img_opa(ui->screen_img_m33, 0, 0);
        // lv_obj_set_style_img_opa(ui->screen_img_m34, 0, 0);
        // lv_obj_set_style_img_opa(ui->screen_img_m35, 0, 0);
        // lv_obj_set_style_img_opa(ui->screen_img_m36, 0, 0);
        // lv_obj_set_style_img_opa(ui->screen_img_m37, 0, 0);

        lv_obj_set_x(ui->screen_img_m14, 630);
        lv_obj_set_x(ui->screen_img_m20, 778);
        lv_obj_set_x(ui->screen_img_m26, 630);
        lv_obj_set_x(ui->screen_img_m32, 778);

        if(ui_data->home_light.light_option[0].color == false){
            // 灯颜色选项未展开
            lv_obj_set_pos(ui->screen_img_m15, 678, 378+600);
            lv_obj_set_pos(ui->screen_img_m16, 683, 383+600);
            lv_obj_set_pos(ui->screen_img_m17, 688, 388+600);
            lv_obj_set_pos(ui->screen_img_m18, 688, 388+600);
            lv_obj_set_pos(ui->screen_img_m19, 688, 388+600);
            lv_obj_set_style_img_opa(ui->screen_img_m15, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m16, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m17, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m18, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m19, 0, 0);
            lv_obj_clear_flag(ui->screen_img_m15, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m16, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m17, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m18, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m19, LV_OBJ_FLAG_CLICKABLE);
        }else if(ui_data->home_light.light_option[0].color == true){
            lv_obj_set_pos(ui->screen_img_m15, 678, 378+600);
            lv_obj_set_pos(ui->screen_img_m16, 683, 383+600);
            lv_obj_set_pos(ui->screen_img_m17, 688, 408+600);
            lv_obj_set_pos(ui->screen_img_m18, 688, 428+600);
            lv_obj_set_pos(ui->screen_img_m19, 688, 448+600);
            lv_obj_set_style_img_opa(ui->screen_img_m15, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m16, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m17, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m18, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m19, 0, 0);
            lv_obj_clear_flag(ui->screen_img_m15, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m16, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m17, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m18, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m19, LV_OBJ_FLAG_CLICKABLE);
        }

        if(ui_data->home_light.light_option[1].color == false){
            // 灯的开关已经打开
            lv_obj_set_pos(ui->screen_img_m21, 916, 378+600);
            lv_obj_set_pos(ui->screen_img_m22, 921, 383+600);
            lv_obj_set_pos(ui->screen_img_m23, 926, 388+600);
            lv_obj_set_pos(ui->screen_img_m24, 926, 388+600);
            lv_obj_set_pos(ui->screen_img_m25, 926, 388+600);
            lv_obj_set_style_img_opa(ui->screen_img_m21, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m22, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m23, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m24, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m25, 0, 0);
            lv_obj_clear_flag(ui->screen_img_m21, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m22, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m23, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m24, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m25, LV_OBJ_FLAG_CLICKABLE);
        }else if(ui_data->home_light.light_option[1].color == true){
            lv_obj_set_pos(ui->screen_img_m21, 916, 378+600);
            lv_obj_set_pos(ui->screen_img_m22, 921, 383+600);
            lv_obj_set_pos(ui->screen_img_m23, 926, 408+600);
            lv_obj_set_pos(ui->screen_img_m24, 926, 428+600);
            lv_obj_set_pos(ui->screen_img_m25, 926, 448+600);
            lv_obj_set_style_img_opa(ui->screen_img_m21, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m22, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m23, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m24, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m25, 0, 0);
            lv_obj_clear_flag(ui->screen_img_m21, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m22, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m23, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m24, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m25, LV_OBJ_FLAG_CLICKABLE);
        }

        if(ui_data->home_light.light_option[2].color == false){
            lv_obj_set_pos(ui->screen_img_m27, 678, 480+600);
            lv_obj_set_pos(ui->screen_img_m28, 683, 485+600);
            lv_obj_set_pos(ui->screen_img_m29, 688, 490+600);
            lv_obj_set_pos(ui->screen_img_m30, 688, 490+600);
            lv_obj_set_pos(ui->screen_img_m31, 688, 490+600);
            lv_obj_set_style_img_opa(ui->screen_img_m27, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m28, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m29, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m30, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m31, 0, 0);
            lv_obj_clear_flag(ui->screen_img_m27, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m28, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m29, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m30, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m31, LV_OBJ_FLAG_CLICKABLE);
        }else if(ui_data->home_light.light_option[2].color == true){
            lv_obj_set_pos(ui->screen_img_m27, 678, 480+600);
            lv_obj_set_pos(ui->screen_img_m28, 683, 485+600);
            lv_obj_set_pos(ui->screen_img_m29, 688, 510+600);
            lv_obj_set_pos(ui->screen_img_m30, 688, 530+600);
            lv_obj_set_pos(ui->screen_img_m31, 688, 550+600);
            lv_obj_set_style_img_opa(ui->screen_img_m27, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m28, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m29, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m30, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m31, 0, 0);
            lv_obj_clear_flag(ui->screen_img_m27, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m28, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m29, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m30, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m31, LV_OBJ_FLAG_CLICKABLE);
        }

        if(ui_data->home_light.light_option[3].color == false){
            lv_obj_set_pos(ui->screen_img_m33, 916, 480+600);
            lv_obj_set_pos(ui->screen_img_m34, 921, 485+600);
            lv_obj_set_pos(ui->screen_img_m35, 926, 490+600);
            lv_obj_set_pos(ui->screen_img_m36, 926, 490+600);
            lv_obj_set_pos(ui->screen_img_m37, 926, 490+600);
            lv_obj_set_style_img_opa(ui->screen_img_m33, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m34, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m35, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m36, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m37, 0, 0);
            lv_obj_clear_flag(ui->screen_img_m33, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m34, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m35, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m36, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m37, LV_OBJ_FLAG_CLICKABLE);
        }else if(ui_data->home_light.light_option[3].color == true){
            lv_obj_set_pos(ui->screen_img_m33, 916, 480+600);
            lv_obj_set_pos(ui->screen_img_m34, 921, 485+600);
            lv_obj_set_pos(ui->screen_img_m35, 926, 510+600);
            lv_obj_set_pos(ui->screen_img_m36, 926, 530+600);
            lv_obj_set_pos(ui->screen_img_m37, 926, 550+600);
            lv_obj_set_style_img_opa(ui->screen_img_m33, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m34, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m35, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m36, 0, 0);
            lv_obj_set_style_img_opa(ui->screen_img_m37, 0, 0);
            lv_obj_clear_flag(ui->screen_img_m33, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m34, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m35, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m36, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_clear_flag(ui->screen_img_m37, LV_OBJ_FLAG_CLICKABLE);
        }
    }

    // lv_obj_set_ext_click_area(ui->screen_img_m15, 10);
    // lv_obj_set_ext_click_area(ui->screen_img_m16, 10);
    // lv_obj_set_ext_click_area(ui->screen_img_m17, 10);
    // lv_obj_set_ext_click_area(ui->screen_img_m18, 10);
    // lv_obj_set_ext_click_area(ui->screen_img_m19, 10);
    // lv_obj_set_ext_click_area(ui->screen_img_m21, 10);
    // lv_obj_set_ext_click_area(ui->screen_img_m22, 10);
    // lv_obj_set_ext_click_area(ui->screen_img_m23, 10);
    // lv_obj_set_ext_click_area(ui->screen_img_m24, 10);
    // lv_obj_set_ext_click_area(ui->screen_img_m25, 10);
    // lv_obj_set_ext_click_area(ui->screen_img_m27, 10);
    // lv_obj_set_ext_click_area(ui->screen_img_m28, 10);
    // lv_obj_set_ext_click_area(ui->screen_img_m29, 10);
    // lv_obj_set_ext_click_area(ui->screen_img_m30, 10);
    // lv_obj_set_ext_click_area(ui->screen_img_m31, 10);
    // lv_obj_set_ext_click_area(ui->screen_img_m33, 10);
    // lv_obj_set_ext_click_area(ui->screen_img_m34, 10);
    // lv_obj_set_ext_click_area(ui->screen_img_m35, 10);
    // lv_obj_set_ext_click_area(ui->screen_img_m36, 10);
    // lv_obj_set_ext_click_area(ui->screen_img_m37, 10);
}
