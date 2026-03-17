

#include "app_interface.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../ui.h"
S_APP_API app_interface = {
    .f_init = p_app_init,
    .wft_protocol = &app_wft_protocol,
    .font = &app_font};

unsigned char WEATHER_STRING[][20] = {
    "未知  ",
    "晴天  ",
    "多云  ",
    "阴天  ",
    "小雨  ",
    "中雨  ",
    "大雨  ",
    "阵雨  ",
    "暴雨  ",
    "雷雨  ",
    "雨夹雪",
    "小雪  ",
    "中雪  ",
    "大雪  ",
    "暴雪  ",
    "雾    ",
    "沙尘暴",
    "雾霾  "};

//  信号
struct rt_semaphore sem_evasive;
struct rt_semaphore sem_refresh_rate; // 刷新
/* 邮箱控制块 */
struct rt_mailbox ui_data_mb;
/* 用于放邮件的内存池 */
static char ui_data_mb_pool[128];

S_UI_DATA *recv_ui_data = NULL;
static uint8_t disp_flag = 0;
static uint8_t disp_ui = 0;
unsigned char *ui_d, *ui_d_temp;
void ui_change_funtion(lv_timer_t *t)
{
    static char temp_str[300];
    static int temp_data_1 = 0;
    static int temp_data_2 = 0;
    // static int temp_data_3 = 0;
    // static int temp_data_4 = 0;
    static uint16_t range_cnt = 0;
    /* 从邮箱中收取邮件 */
    if (rt_mb_recv(&ui_data_mb, (rt_uint32_t *)&recv_ui_data, RT_WAITING_NO) == RT_EOK)
    {

        if (recv_ui_data->navigate == 0) // Nomal
        {
            if (disp_ui != 1)
            {
                disp_ui = 1;
                lv_img_cache_invalidate_src(NULL);
                if (ui_wft_ui2)
                {
                    lv_obj_del(ui_wft_ui2);
                    ui_wft_ui2 = NULL;
                }
                if (ui_wft_ui1 == NULL)
                {
                    ui_wft_ui1_screen_init();
                    lv_disp_load_scr(ui_wft_ui1);
                }
                // lv_obj_invalidate(lv_scr_act());//重绘屏幕
                lv_img_cache_invalidate_src(NULL);
                lv_disp_load_scr(ui_wft_ui1);

                //  更新所有控件
                ui_d = recv_ui_data;
                ui_d_temp = &app_interface.ui_data_temp;
                for (int i = 0; i < (sizeof(S_UI_DATA)); i++)
                {
                    ui_d_temp[i] = ~(*(ui_d + i));
                }
            }
        }
        else // navigate
        {
            if (disp_ui != 2)
            {
                disp_ui = 2;
                lv_img_cache_invalidate_src(NULL);
                if (ui_wft_ui1)
                {
                    lv_obj_del(ui_wft_ui1);
                    ui_wft_ui1 = NULL;
                }
                if (ui_wft_ui2 == NULL)
                {

                    ui_wft_ui2_screen_init();
                    lv_disp_load_scr(ui_wft_ui2);
                }
                // lv_obj_invalidate(lv_scr_act());//重绘屏幕

                lv_img_cache_invalidate_src(NULL);
                lv_disp_load_scr(ui_wft_ui2);

                //  更新所有控件
                ui_d = recv_ui_data;
                ui_d_temp = &app_interface.ui_data_temp;
                for (int i = 0; i < (sizeof(S_UI_DATA)); i++)
                {
                    ui_d_temp[i] = ~(*(ui_d + i));
                }
            }
        }

        ////////////////////////////////////////////////////////////////
        //     rt_kprintf("recv_ui_data.bar_val: %d:\n", recv_ui_data->speed_val);
        if (recv_ui_data->navigate == 0) // Nomal
        {                                /*                  recv_ui_data->navigate == 0)        */

            // 【0:P档  1:前进档  2:倒车档】
            switch (recv_ui_data->gear)
            {
            case 0: // P
                // recv_ui_data->speed_val = 0;
                if ((recv_ui_data->gear != app_interface.ui_data_temp.gear) || (recv_ui_data->memory_gear != app_interface.ui_data_temp.memory_gear))
                {
                    app_interface.ui_data_temp.gear = recv_ui_data->gear;
                    app_interface.ui_data_temp.memory_gear = recv_ui_data->memory_gear;
                    switch (recv_ui_data->memory_gear)
                    {
                    case 0:
                        lv_img_set_src(ui_Image10, &ui_img_d_hui_png);
                        break;
                    case 1:
                        lv_img_set_src(ui_Image10, &ui_img_d1_hui_png);
                        break;
                    case 2:
                        lv_img_set_src(ui_Image10, &ui_img_d2_hui_png);
                        break;
                    case 3:
                        lv_img_set_src(ui_Image10, &ui_img_d3_hui_png);
                        break;
                    case 4:
                        lv_img_set_src(ui_Image10, &ui_img_s_hui_png);
                        break;
                    default:
                        lv_img_set_src(ui_Image10, &ui_img_d1_hui_png);
                        break;
                    }

                    lv_img_set_src(ui_Image7, &ui_img_p_png);
                    lv_img_set_src(ui_Image9, &ui_img_r_hui_png);
                    lv_obj_set_x(ui_Image8, -80);
                }
                break;

            case 1: // 前进挡
                    // 【0:无档位D   1:低速D1  2:中速D2  3:高速D3  4:运动S】
                if ((recv_ui_data->gear != app_interface.ui_data_temp.gear) || (recv_ui_data->memory_gear != app_interface.ui_data_temp.memory_gear))
                {
                    app_interface.ui_data_temp.gear = recv_ui_data->gear;
                    app_interface.ui_data_temp.memory_gear = recv_ui_data->memory_gear;
                    switch (recv_ui_data->memory_gear)
                    {
                    case 0: // 无档位D
                        // lv_img_cache_invalidate_src(NULL);
                        lv_img_set_src(ui_Image10, &ui_img_d_png);
                        lv_img_set_src(ui_Image7, &ui_img_p_hui_png);
                        lv_img_set_src(ui_Image9, &ui_img_r_hui_png);
                        break;
                    case 1: // D1

                        // lv_img_cache_invalidate_src(NULL);
                        lv_img_set_src(ui_Image10, &ui_img_d1_png);
                        lv_img_set_src(ui_Image7, &ui_img_p_hui_png);
                        lv_img_set_src(ui_Image9, &ui_img_r_hui_png);
                        break;
                    case 2: // D2
                        // lv_img_cache_invalidate_src(NULL);
                        lv_img_set_src(ui_Image10, &ui_img_d2_png);
                        lv_img_set_src(ui_Image7, &ui_img_p_hui_png);
                        lv_img_set_src(ui_Image9, &ui_img_r_hui_png);
                        break;
                    case 3: // D3
                        // lv_img_cache_invalidate_src(NULL);
                        lv_img_set_src(ui_Image10, &ui_img_d3_png);
                        lv_img_set_src(ui_Image7, &ui_img_p_hui_png);
                        lv_img_set_src(ui_Image9, &ui_img_r_hui_png);
                        break;
                    case 4: // S

                        // lv_img_cache_invalidate_src(NULL);
                        lv_img_set_src(ui_Image10, &ui_img_s_png);
                        lv_img_set_src(ui_Image7, &ui_img_p_hui_png);
                        lv_img_set_src(ui_Image9, &ui_img_r_hui_png);
                        break;
                    default:
                        break;
                    }
                    lv_obj_set_x(ui_Image8, 80);
                }
                break;

            case 2: // R
                    // recv_ui_data->speed_val = 0;
                if ((recv_ui_data->gear != app_interface.ui_data_temp.gear) || (recv_ui_data->memory_gear != app_interface.ui_data_temp.memory_gear))
                {
                    app_interface.ui_data_temp.gear = recv_ui_data->gear;
                    app_interface.ui_data_temp.memory_gear = recv_ui_data->memory_gear;
                    switch (recv_ui_data->memory_gear)
                    {
                    case 0:
                        lv_img_set_src(ui_Image10, &ui_img_d_hui_png);
                        break;
                    case 1:
                        lv_img_set_src(ui_Image10, &ui_img_d1_hui_png);
                        break;
                    case 2:
                        lv_img_set_src(ui_Image10, &ui_img_d2_hui_png);
                        break;
                    case 3:
                        lv_img_set_src(ui_Image10, &ui_img_d3_hui_png);
                        break;
                    case 4:
                        lv_img_set_src(ui_Image10, &ui_img_s_hui_png);
                        break;
                    default:
                        lv_img_set_src(ui_Image10, &ui_img_d1_hui_png);
                        break;
                    }
                    lv_img_set_src(ui_Image7, &ui_img_p_hui_png);
                    lv_img_set_src(ui_Image9, &ui_img_r_png);
                    lv_obj_set_x(ui_Image8, 0);
                }
                break;
            default:
                break;
            }

            lv_slider_set_value(ui_ui1_Slider_bat_left, recv_ui_data->bt_percent_a, LV_ANIM_OFF);
            lv_slider_set_value(ui_ui1_Slider_bat_right, recv_ui_data->bt_percent_b, LV_ANIM_OFF);

            if (rt_memcmp(&app_interface.ui_data_temp.msg_call_number, &recv_ui_data->msg_call_number, 250) != 0)
            {
                rt_memcpy(app_interface.ui_data_temp.msg_call_number, recv_ui_data->msg_call_number, 250);
                lv_label_set_text(ui_Label2, recv_ui_data->msg_call_number);
                ENABLE_FLAG_HIDDEN(ui_Label2, 1);
            }

            // if ((app_interface.ui_data_temp.maximum_temperature != recv_ui_data->maximum_temperature) |
            //     (app_interface.ui_data_temp.weather_code != recv_ui_data->weather_code) |
            //     (app_interface.ui_data_temp.lowest_temperature != recv_ui_data->lowest_temperature))
            // {

            //     app_interface.ui_data_temp.maximum_temperature = recv_ui_data->maximum_temperature;
            //     app_interface.ui_data_temp.lowest_temperature = recv_ui_data->lowest_temperature;
            //     app_interface.ui_data_temp.weather_code = recv_ui_data->weather_code;

            //     if ((recv_ui_data->maximum_temperature & 0x80) == 0x80)
            //     {
            //         temp_data_2 = 0 - (recv_ui_data->maximum_temperature & 0x7f);
            //     }
            //     else
            //     {
            //         temp_data_2 = recv_ui_data->maximum_temperature;
            //     }
            //     if ((recv_ui_data->lowest_temperature & 0x80) == 0x80)
            //     {
            //         temp_data_1 = 0 - (recv_ui_data->lowest_temperature & 0x7f);
            //     }
            //     else
            //     {
            //         temp_data_1 = recv_ui_data->lowest_temperature;
            //     }
            //     rt_memset(temp_str, 0, 300);
            //     rt_sprintf(temp_str, "%s %s %d℃~%d℃", recv_ui_data->weather_addr, WEATHER_STRING[recv_ui_data->weather_code], temp_data_1, temp_data_2);
            //     // rt_kprintf("temp_str : %s", temp_str);
            //     lv_label_set_text(ui_Label13, temp_str);
            //     ENABLE_FLAG_HIDDEN(ui_Label13, 1);
            // }

#if 0 // AUTO
            if (recv_ui_data->speed_val >= 75)
            {
                if (disp_flag != 1)
                {
                    disp_flag = 1;

                    lv_img_cache_invalidate_src(NULL);
                    // lv_img_cache_set_size(5);
                    lv_img_set_src(ui_ui1_Image_bg1, SRC2_PATH_HEAD "red_1.jpg");
                    lv_img_set_src(ui_ui1_Image_bg2, SRC2_PATH_HEAD "red_2.png");
                }
            }
            else if (recv_ui_data->speed_val >= 60)
            {
                if (disp_flag != 2)
                {
                    disp_flag = 2;

                    lv_img_cache_invalidate_src(NULL);
                    // lv_img_cache_set_size(5);

                    lv_img_set_src(ui_ui1_Image_bg1, SRC2_PATH_HEAD "orange_1.jpg");
                    lv_img_set_src(ui_ui1_Image_bg2, SRC2_PATH_HEAD "orange_2.png");
                }
            }
            else if (recv_ui_data->speed_val >= 30)
            {
                if (disp_flag != 3)
                {
                    disp_flag = 3;

                    lv_img_cache_invalidate_src(NULL);
                    // lv_img_cache_set_size(5);
                    lv_img_set_src(ui_ui1_Image_bg1, SRC2_PATH_HEAD "blue_1.jpg");
                    lv_img_set_src(ui_ui1_Image_bg2, SRC2_PATH_HEAD "blue_2.png");
                }
            }
            else
            {
                if (disp_flag != 4)
                {
                    disp_flag = 4;

                    lv_img_cache_invalidate_src(NULL);
                    // lv_img_cache_set_size(5);
                    lv_img_set_src(ui_ui1_Image_bg1, SRC2_PATH_HEAD "green_1.jpg");
                    lv_img_set_src(ui_ui1_Image_bg2, SRC2_PATH_HEAD "green_2.png");
                }
            }
#else
            if (app_interface.ui_data_temp.run_color != recv_ui_data->run_color)
            {
                app_interface.ui_data_temp.run_color = recv_ui_data->run_color;
                switch (recv_ui_data->run_color)
                {
                case 0:
                    lv_img_cache_invalidate_src(NULL);
                    lv_img_set_src(ui_ui1_Image_bg1, SRC2_PATH_HEAD "green_1.jpg");
                    lv_img_set_src(ui_ui1_Image_bg2, SRC2_PATH_HEAD "green_2.png");
                    lv_obj_set_style_shadow_color(ui_Panel1, lv_color_hex(0x00ff00), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(ui_Panel2, lv_color_hex(0x00ff00), LV_PART_MAIN | LV_STATE_DEFAULT);

                    break;
                case 1:
                    lv_img_cache_invalidate_src(NULL);
                    lv_img_set_src(ui_ui1_Image_bg1, SRC2_PATH_HEAD "blue_1.jpg");
                    lv_img_set_src(ui_ui1_Image_bg2, SRC2_PATH_HEAD "blue_2.png");
                    lv_obj_set_style_shadow_color(ui_Panel1, lv_color_hex(0x0000ff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(ui_Panel2, lv_color_hex(0x0000ff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    break;
                case 2:
                    lv_img_cache_invalidate_src(NULL);
                    lv_img_set_src(ui_ui1_Image_bg1, SRC2_PATH_HEAD "orange_1.jpg");
                    lv_img_set_src(ui_ui1_Image_bg2, SRC2_PATH_HEAD "orange_2.png");
                    lv_obj_set_style_shadow_color(ui_Panel1, lv_color_hex(0xED7911), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(ui_Panel2, lv_color_hex(0xED7911), LV_PART_MAIN | LV_STATE_DEFAULT);
                    break;
                case 3:
                    lv_img_cache_invalidate_src(NULL);
                    lv_img_set_src(ui_ui1_Image_bg1, SRC2_PATH_HEAD "red_1.jpg");
                    lv_img_set_src(ui_ui1_Image_bg2, SRC2_PATH_HEAD "red_2.png");
                    lv_obj_set_style_shadow_color(ui_Panel1, lv_color_hex(0xff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(ui_Panel2, lv_color_hex(0xff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    break;
                default:
                    lv_img_cache_invalidate_src(NULL);
                    lv_img_set_src(ui_ui1_Image_bg1, SRC2_PATH_HEAD "green_1.jpg");
                    lv_img_set_src(ui_ui1_Image_bg2, SRC2_PATH_HEAD "green_2.png");
                    lv_obj_set_style_shadow_color(ui_Panel1, lv_color_hex(0x00ff00), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(ui_Panel2, lv_color_hex(0x00ff00), LV_PART_MAIN | LV_STATE_DEFAULT);
                    break;
                }
            }
#endif

            if (app_interface.ui_data_temp.day_and_night_mode_on != recv_ui_data->day_and_night_mode_on)
            {
                app_interface.ui_data_temp.day_and_night_mode_on = recv_ui_data->day_and_night_mode_on;

                if (recv_ui_data->day_and_night_mode_on == 1)
                {
                    lv_obj_set_style_bg_color(ui_wft_ui1, lv_color_hex(0x01153D), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(ui_wft_ui1, 100 + recv_ui_data->speed_val, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                else
                {
                    lv_obj_set_style_bg_color(ui_wft_ui1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(ui_wft_ui1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }

            ///////////////////////////   图标控制  ////////////////////////////

            if (app_interface.ui_data_temp.oil_pot_voltage != recv_ui_data->oil_pot_voltage)
            {
                app_interface.ui_data_temp.oil_pot_voltage = recv_ui_data->oil_pot_voltage;
                ENABLE_FLAG_HIDDEN(ui_Image48, recv_ui_data->oil_pot_voltage);
            }

            if (app_interface.ui_data_temp.side_brace != recv_ui_data->side_brace)
            {
                app_interface.ui_data_temp.side_brace = recv_ui_data->side_brace;
                ENABLE_FLAG_HIDDEN(ui_Image46, recv_ui_data->side_brace);
            }

            if (app_interface.ui_data_temp.high_current_fault != recv_ui_data->high_current_fault)
            {
                app_interface.ui_data_temp.high_current_fault = recv_ui_data->high_current_fault;
                ENABLE_FLAG_HIDDEN(ui_Image45, recv_ui_data->high_current_fault);
            }

            if (app_interface.ui_data_temp.undervoltage_fault != recv_ui_data->undervoltage_fault)
            {
                app_interface.ui_data_temp.undervoltage_fault = recv_ui_data->undervoltage_fault;
                ENABLE_FLAG_HIDDEN(ui_Image28, recv_ui_data->undervoltage_fault);
            }

            if (app_interface.ui_data_temp.brake_failure != recv_ui_data->brake_failure)
            {
                app_interface.ui_data_temp.brake_failure = recv_ui_data->brake_failure;
                ENABLE_FLAG_HIDDEN(ui_Image24, recv_ui_data->brake_failure);
            }

            if (app_interface.ui_data_temp.trolley_failure != recv_ui_data->trolley_failure)
            {
                app_interface.ui_data_temp.trolley_failure = recv_ui_data->trolley_failure;
                ENABLE_FLAG_HIDDEN(ui_Image25, recv_ui_data->trolley_failure);
            }
            if (app_interface.ui_data_temp.ecu_failure != recv_ui_data->ecu_failure)
            {
                app_interface.ui_data_temp.ecu_failure = recv_ui_data->ecu_failure;
                ENABLE_FLAG_HIDDEN(ui_Image27, recv_ui_data->ecu_failure);
            }

            if (app_interface.ui_data_temp.hall_failure != recv_ui_data->hall_failure)
            {
                app_interface.ui_data_temp.hall_failure = recv_ui_data->hall_failure;
                ENABLE_FLAG_HIDDEN(ui_Image3, recv_ui_data->hall_failure);
            }
            if (app_interface.ui_data_temp.eco_energy_saving_display != recv_ui_data->eco_energy_saving_display)
            {
                app_interface.ui_data_temp.eco_energy_saving_display = recv_ui_data->eco_energy_saving_display;
                ENABLE_FLAG_HIDDEN(ui_Image47, recv_ui_data->eco_energy_saving_display);
            }

            if (app_interface.ui_data_temp.icon_Ready != recv_ui_data->icon_Ready)
            {
                app_interface.ui_data_temp.icon_Ready = recv_ui_data->icon_Ready;
                ENABLE_FLAG_HIDDEN(ui_Image18, recv_ui_data->icon_Ready);
            }

            if (app_interface.ui_data_temp.bluetooth != recv_ui_data->bluetooth)
            {
                app_interface.ui_data_temp.bluetooth = recv_ui_data->bluetooth;
                ENABLE_FLAG_HIDDEN(ui_Image4, recv_ui_data->bluetooth);
            }

            if (app_interface.ui_data_temp.headlight != recv_ui_data->headlight)
            {
                app_interface.ui_data_temp.headlight = recv_ui_data->headlight;
                ENABLE_FLAG_HIDDEN(ui_Image2, recv_ui_data->headlight);
            }
            if (app_interface.ui_data_temp.icon_Cruise != recv_ui_data->icon_Cruise)
            {
                app_interface.ui_data_temp.icon_Cruise = recv_ui_data->icon_Cruise;
                ENABLE_FLAG_HIDDEN(ui_Image16, recv_ui_data->icon_Cruise);
            }

            if (app_interface.ui_data_temp.icon_Position != recv_ui_data->icon_Position)
            {
                app_interface.ui_data_temp.icon_Position = recv_ui_data->icon_Position;
                ENABLE_FLAG_HIDDEN(ui_Image17, recv_ui_data->icon_Position);
            }

            if (app_interface.ui_data_temp.icon_Turn_L != recv_ui_data->icon_Turn_L)
            {
                app_interface.ui_data_temp.icon_Turn_L = recv_ui_data->icon_Turn_L;
                ENABLE_FLAG_HIDDEN(ui_Image5, recv_ui_data->icon_Turn_L);
            }

            if (app_interface.ui_data_temp.icon_Turn_R != recv_ui_data->icon_Turn_R)
            {
                app_interface.ui_data_temp.icon_Turn_R = recv_ui_data->icon_Turn_R;
                ENABLE_FLAG_HIDDEN(ui_Image6, recv_ui_data->icon_Turn_R);
            }
            if (app_interface.ui_data_temp.icon_Call != recv_ui_data->icon_Call)
            {
                app_interface.ui_data_temp.icon_Call = recv_ui_data->icon_Call;
                ENABLE_FLAG_HIDDEN(ui_Image11, recv_ui_data->icon_Call);
            }
            if (app_interface.ui_data_temp.icon_Msg != recv_ui_data->icon_Msg)
            {
                app_interface.ui_data_temp.icon_Msg = recv_ui_data->icon_Msg;
                ENABLE_FLAG_HIDDEN(ui_Image13, recv_ui_data->icon_Msg);
            }

            if ((recv_ui_data->speed_val != app_interface.ui_data_temp.speed_val) || (app_interface.ui_data_temp.speed_display != recv_ui_data->speed_display))
            {
                app_interface.ui_data_temp.speed_val = recv_ui_data->speed_val;
                app_interface.ui_data_temp.speed_display = recv_ui_data->speed_display;
                rt_memset(temp_str, 0, 300);
                sprintf(temp_str, "%d", (int)recv_ui_data->speed_val);
                lv_label_set_text(ui_ui1_Label_bgnum, temp_str);
                ENABLE_FLAG_HIDDEN(ui_ui1_Label_bgnum, recv_ui_data->speed_display);
                ENABLE_FLAG_HIDDEN(ui_ui1_Label_kmh, recv_ui_data->speed_display);
            }

            if ((recv_ui_data->bt_percent_b != app_interface.ui_data_temp.bt_percent_b) ||
                (recv_ui_data->bt_voltage_b != app_interface.ui_data_temp.bt_voltage_b) ||
                (recv_ui_data->battery_b_power_display != app_interface.ui_data_temp.battery_b_power_display) ||
                (recv_ui_data->battery_b_voltage_display != app_interface.ui_data_temp.battery_b_voltage_display)) // BT2
            {
                app_interface.ui_data_temp.bt_percent_b = recv_ui_data->bt_percent_b;
                app_interface.ui_data_temp.bt_voltage_b = recv_ui_data->bt_voltage_b;
                app_interface.ui_data_temp.battery_b_power_display = recv_ui_data->battery_b_power_display;
                app_interface.ui_data_temp.battery_b_voltage_display = recv_ui_data->battery_b_voltage_display;

                if (recv_ui_data->battery_b_power_display)
                {
                    rt_memset(temp_str, 0, 300);
                    sprintf(temp_str, "%d%%", (int)recv_ui_data->bt_percent_b);
                    ENABLE_FLAG_HIDDEN(ui_Label4, 1);
                }
                else if (recv_ui_data->battery_b_voltage_display)
                {
                    rt_memset(temp_str, 0, 300);
                    rt_sprintf(temp_str, "%2d", recv_ui_data->bt_voltage_b / 10);
                    temp_str[2] = '.';
                    rt_sprintf(&temp_str[3], "%1d", recv_ui_data->bt_voltage_b % 10);
                    temp_str[4] = 'V';
                    ENABLE_FLAG_HIDDEN(ui_Label4, 1);
                }
                else
                {
                    rt_memset(temp_str, 0, 300);
                    rt_sprintf(&temp_str, " ");
                    ENABLE_FLAG_HIDDEN(ui_Label4, 0);
                }
                lv_label_set_text(ui_Label4, temp_str);
            }

            if ((recv_ui_data->bt_percent_a != app_interface.ui_data_temp.bt_percent_a) ||
                (recv_ui_data->bt_voltage_a != app_interface.ui_data_temp.bt_voltage_a) ||
                (recv_ui_data->battery_a_power_display != app_interface.ui_data_temp.battery_a_power_display) ||
                (recv_ui_data->battery_a_voltage_display != app_interface.ui_data_temp.battery_a_voltage_display)) // BT1
            {
                app_interface.ui_data_temp.bt_percent_a = recv_ui_data->bt_percent_a;
                app_interface.ui_data_temp.bt_voltage_a = recv_ui_data->bt_voltage_a;
                app_interface.ui_data_temp.battery_a_power_display = recv_ui_data->battery_a_power_display;
                app_interface.ui_data_temp.battery_a_voltage_display = recv_ui_data->battery_a_voltage_display;

                if (recv_ui_data->battery_a_power_display)
                {
                    rt_memset(temp_str, 0, 300);
                    sprintf(temp_str, "%d%%", (int)recv_ui_data->bt_percent_a);
                    ENABLE_FLAG_HIDDEN(ui_Label3, 1);
                }
                else if (recv_ui_data->battery_a_voltage_display)
                {
                    rt_memset(temp_str, 0, 300);
                    rt_sprintf(temp_str, "%2d", recv_ui_data->bt_voltage_a / 10);
                    temp_str[2] = '.';
                    rt_sprintf(&temp_str[3], "%1d", recv_ui_data->bt_voltage_a % 10);
                    temp_str[4] = 'V';
                    ENABLE_FLAG_HIDDEN(ui_Label3, 1);
                }
                else
                {
                    rt_memset(temp_str, 0, 300);
                    rt_sprintf(&temp_str, " ");
                    ENABLE_FLAG_HIDDEN(ui_Label3, 0);
                }
                lv_label_set_text(ui_Label3, temp_str);
            }

            if ((app_interface.ui_data_temp.time_hours != recv_ui_data->time_hours) ||
                (app_interface.ui_data_temp.time_minutes != recv_ui_data->time_minutes) ||
                (app_interface.ui_data_temp.time_display != recv_ui_data->time_display))
            {
                app_interface.ui_data_temp.time_hours = recv_ui_data->time_hours;
                app_interface.ui_data_temp.time_minutes = recv_ui_data->time_minutes;
                app_interface.ui_data_temp.time_display = recv_ui_data->time_display;

                if (recv_ui_data->time_display == 1) // 显示时间
                {
                    lv_obj_clear_flag(ui_Label9, LV_OBJ_FLAG_HIDDEN);
                }
                else
                {
                    lv_obj_add_flag(ui_Label9, LV_OBJ_FLAG_HIDDEN);
                }
                rt_memset(temp_str, 0, 300);
                sprintf(temp_str, "%d:%02d", (int)recv_ui_data->time_hours, (int)recv_ui_data->time_minutes);
                lv_label_set_text(ui_Label9, temp_str);
            }

            if (recv_ui_data->odo_km != app_interface.ui_data_temp.odo_km ||
                recv_ui_data->total_display_status != app_interface.ui_data_temp.total_display_status)
            {

                app_interface.ui_data_temp.odo_km = recv_ui_data->odo_km;
                app_interface.ui_data_temp.total_display_status = recv_ui_data->total_display_status;
                if (recv_ui_data->total_display_status == 1) // 总里程显示状态          【0:显示关；1:显示开】
                {
                    lv_obj_clear_flag(ui_Label5, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_clear_flag(ui_Image19, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_clear_flag(ui_Image21, LV_OBJ_FLAG_HIDDEN);
                }
                else
                {
                    lv_obj_add_flag(ui_Label5, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_add_flag(ui_Image19, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_add_flag(ui_Image21, LV_OBJ_FLAG_HIDDEN);
                }

                rt_memset(temp_str, 0, 300);
                sprintf(temp_str, "%d", (int)recv_ui_data->odo_km);
                lv_label_set_text(ui_Label5, temp_str);
            }

            if (recv_ui_data->trip_km != app_interface.ui_data_temp.trip_km ||
                recv_ui_data->trip_display_status != app_interface.ui_data_temp.trip_display_status)
            {
                if (recv_ui_data->trip_display_status == 1) //  单次程显示状态          【0:显示关；1:显示开】
                {
                    lv_obj_clear_flag(ui_Label6, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_clear_flag(ui_Image20, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_clear_flag(ui_Image22, LV_OBJ_FLAG_HIDDEN);
                }
                else
                {
                    lv_obj_add_flag(ui_Label6, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_add_flag(ui_Image20, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_add_flag(ui_Image22, LV_OBJ_FLAG_HIDDEN);
                }
                app_interface.ui_data_temp.trip_km = recv_ui_data->trip_km;
                app_interface.ui_data_temp.trip_display_status = recv_ui_data->trip_display_status;
                rt_memset(temp_str, 0, 300);
                sprintf(temp_str, "%d", (int)recv_ui_data->trip_km);
                lv_label_set_text(ui_Label6, temp_str);
            }

        } /*******************           recv_ui_data->navigate == 0        ****************************/
        else
        { /*******************           recv_ui_data->navigate == 1        ****************************/
            // 【0:P档  1:前进档  2:倒车档】
            switch (recv_ui_data->gear)
            {
            case 0: // P
                if ((recv_ui_data->gear != app_interface.ui_data_temp.gear) || (recv_ui_data->memory_gear != app_interface.ui_data_temp.memory_gear))
                {
                    app_interface.ui_data_temp.gear = recv_ui_data->gear;
                    app_interface.ui_data_temp.memory_gear = recv_ui_data->memory_gear;
                    switch (recv_ui_data->memory_gear)
                    {
                    case 0:
                        lv_img_set_src(ui_Image40, &ui_img_d_hui_png);
                        break;
                    case 1:
                        lv_img_set_src(ui_Image40, &ui_img_d1_hui_png);
                        break;
                    case 2:
                        lv_img_set_src(ui_Image40, &ui_img_d2_hui_png);
                        break;
                    case 3:
                        lv_img_set_src(ui_Image40, &ui_img_d3_hui_png);
                        break;
                    case 4:
                        lv_img_set_src(ui_Image40, &ui_img_s_hui_png);
                        break;
                    default:
                        lv_img_set_src(ui_Image40, &ui_img_d1_hui_png);
                        break;
                    }

                    lv_img_set_src(ui_Image31, &ui_img_p_png);
                    lv_img_set_src(ui_Image44, &ui_img_r_hui_png);
                    lv_obj_set_x(ui_Image43, -80);
                }
                break;

            case 1: // 前进挡
                    // 【0:无档位D   1:低速D1  2:中速D2  3:高速D3  4:运动S】
                if ((recv_ui_data->gear != app_interface.ui_data_temp.gear) || (recv_ui_data->memory_gear != app_interface.ui_data_temp.memory_gear))
                {
                    app_interface.ui_data_temp.gear = recv_ui_data->gear;
                    app_interface.ui_data_temp.memory_gear = recv_ui_data->memory_gear;
                    switch (recv_ui_data->memory_gear)
                    {
                    case 0: // 无档位D
                        lv_img_cache_invalidate_src(NULL);
                        lv_img_set_src(ui_Image40, &ui_img_d_png);
                        lv_img_set_src(ui_Image31, &ui_img_p_hui_png);
                        lv_img_set_src(ui_Image44, &ui_img_r_hui_png);
                        break;
                    case 1: // D1

                        lv_img_cache_invalidate_src(NULL);
                        lv_img_set_src(ui_Image40, &ui_img_d1_png);
                        lv_img_set_src(ui_Image31, &ui_img_p_hui_png);
                        lv_img_set_src(ui_Image44, &ui_img_r_hui_png);
                        break;
                    case 2: // D2
                        lv_img_cache_invalidate_src(NULL);
                        lv_img_set_src(ui_Image40, &ui_img_d2_png);
                        lv_img_set_src(ui_Image31, &ui_img_p_hui_png);
                        lv_img_set_src(ui_Image44, &ui_img_r_hui_png);
                        break;
                    case 3: // D3
                        lv_img_cache_invalidate_src(NULL);
                        lv_img_set_src(ui_Image40, &ui_img_d3_png);
                        lv_img_set_src(ui_Image31, &ui_img_p_hui_png);
                        lv_img_set_src(ui_Image44, &ui_img_r_hui_png);
                        break;
                    case 4: // S

                        lv_img_cache_invalidate_src(NULL);
                        lv_img_set_src(ui_Image40, &ui_img_s_png);
                        lv_img_set_src(ui_Image31, &ui_img_p_hui_png);
                        lv_img_set_src(ui_Image44, &ui_img_r_hui_png);
                        break;
                    default:
                        break;
                    }
                    lv_obj_set_x(ui_Image43, 80);
                }
                break;

            case 2: // R
                if ((recv_ui_data->gear != app_interface.ui_data_temp.gear) || (recv_ui_data->memory_gear != app_interface.ui_data_temp.memory_gear))
                {
                    app_interface.ui_data_temp.gear = recv_ui_data->gear;
                    app_interface.ui_data_temp.memory_gear = recv_ui_data->memory_gear;
                    switch (recv_ui_data->memory_gear)
                    {
                    case 0:
                        lv_img_set_src(ui_Image40, &ui_img_d_hui_png);
                        break;
                    case 1:
                        lv_img_set_src(ui_Image40, &ui_img_d1_hui_png);
                        break;
                    case 2:
                        lv_img_set_src(ui_Image40, &ui_img_d2_hui_png);
                        break;
                    case 3:
                        lv_img_set_src(ui_Image40, &ui_img_d3_hui_png);
                        break;
                    case 4:
                        lv_img_set_src(ui_Image40, &ui_img_s_hui_png);
                        break;
                    default:
                        lv_img_set_src(ui_Image40, &ui_img_d1_hui_png);
                        break;
                    }
                    lv_img_set_src(ui_Image31, &ui_img_p_hui_png);
                    lv_img_set_src(ui_Image44, &ui_img_r_png);
                    lv_obj_set_x(ui_Image43, 0);
                }
                break;
            default:
                break;
            }

            // if (rt_memcmp(&app_interface.ui_data_temp.msg_call_number, &recv_ui_data->msg_call_number, 250) != 0)
            // {
            //     rt_memcpy(app_interface.ui_data_temp.msg_call_number, recv_ui_data->msg_call_number, 250);
            //     lv_label_set_text(ui_Label20, recv_ui_data->msg_call_number);
            //     ENABLE_FLAG_HIDDEN(ui_Label20, 1);
            // }

            // if ((app_interface.ui_data_temp.maximum_temperature != recv_ui_data->maximum_temperature) |
            //     (app_interface.ui_data_temp.weather_code != recv_ui_data->weather_code) |
            //     (app_interface.ui_data_temp.lowest_temperature != recv_ui_data->lowest_temperature))
            // {

            //     app_interface.ui_data_temp.maximum_temperature = recv_ui_data->maximum_temperature;
            //     app_interface.ui_data_temp.lowest_temperature = recv_ui_data->lowest_temperature;
            //     app_interface.ui_data_temp.weather_code = recv_ui_data->weather_code;

            //     if ((recv_ui_data->maximum_temperature & 0x80) == 0x80)
            //     {
            //         temp_data_2 = 0 - (recv_ui_data->maximum_temperature & 0x7f);
            //     }
            //     else
            //     {
            //         temp_data_2 = recv_ui_data->maximum_temperature;
            //     }
            //     if ((recv_ui_data->lowest_temperature & 0x80) == 0x80)
            //     {
            //         temp_data_1 = 0 - (recv_ui_data->lowest_temperature & 0x7f);
            //     }
            //     else
            //     {
            //         temp_data_1 = recv_ui_data->lowest_temperature;
            //     }

            //     rt_memset(temp_str, 0, 300);
            //     rt_sprintf(temp_str, "%s %s %d℃~%d℃", recv_ui_data->weather_addr, WEATHER_STRING[recv_ui_data->weather_code], temp_data_1, temp_data_2);

            //     // rt_kprintf("temp_str : %s", temp_str);
            //     lv_label_set_text(ui_Label12, temp_str);
            //     ENABLE_FLAG_HIDDEN(ui_Label12, 1);
            // }

            if (rt_memcmp(&app_interface.ui_data_temp.navigation_description, &recv_ui_data->navigation_description, 250) != 0)
            {
                rt_memcpy(app_interface.ui_data_temp.navigation_description, recv_ui_data->navigation_description, 250);
                lv_label_set_text(ui_Label1, recv_ui_data->navigation_description);
                ENABLE_FLAG_HIDDEN(ui_Label1, 1);
            }

            if (app_interface.ui_data_temp.estimated_time != recv_ui_data->estimated_time)
            {
                app_interface.ui_data_temp.estimated_time = recv_ui_data->estimated_time;
                rt_sprintf(app_interface.ui_data_temp.time_left, "预计%d分钟到达", recv_ui_data->estimated_time);
                lv_label_set_text(ui_Label10, app_interface.ui_data_temp.time_left);
                ENABLE_FLAG_HIDDEN(ui_Label10, 1);
            }
            if (app_interface.ui_data_temp.total_remaining_mileage != recv_ui_data->total_remaining_mileage)
            {
                app_interface.ui_data_temp.total_remaining_mileage = recv_ui_data->total_remaining_mileage;
                rt_sprintf(app_interface.ui_data_temp.total_left_mileage, "剩余%dKm", recv_ui_data->total_remaining_mileage);
                lv_label_set_text(ui_Label8, app_interface.ui_data_temp.total_left_mileage);
                ENABLE_FLAG_HIDDEN(ui_Label8, 1);
            }
            // if (rt_memcmp(&app_interface.ui_data_temp.time_destination, &recv_ui_data->time_destination, 250) != 0)
            // {
            //     rt_memcpy(app_interface.ui_data_temp.time_destination, recv_ui_data->time_destination, 250);
            //     lv_label_set_text(ui_Label8, recv_ui_data->time_destination);
            //     ENABLE_FLAG_HIDDEN(ui_Label8, 1);
            // }
            // if (rt_memcmp(&app_interface.ui_data_temp.time_left, &recv_ui_data->time_left, 250) != 0)
            // {
            //     rt_memcpy(app_interface.ui_data_temp.time_left, recv_ui_data->time_left, 250);
            //     lv_label_set_text(ui_Label10, recv_ui_data->time_left);
            //     ENABLE_FLAG_HIDDEN(ui_Label10, 1);
            // }

            if (app_interface.ui_data_temp.run_color != recv_ui_data->run_color)
            {
                app_interface.ui_data_temp.run_color = recv_ui_data->run_color;
                switch (recv_ui_data->run_color)
                {
                case 0:
                    lv_img_cache_invalidate_src(NULL);
                    lv_img_set_src(ui_ui2_Image_bg1, SRC2_PATH_HEAD "green_1.jpg");
                    lv_img_set_src(ui_ui2_Image_bg2, SRC2_PATH_HEAD "green_2.png");
                    break;
                case 1:
                    lv_img_cache_invalidate_src(NULL);
                    lv_img_set_src(ui_ui2_Image_bg1, SRC2_PATH_HEAD "blue_1.jpg");
                    lv_img_set_src(ui_ui2_Image_bg2, SRC2_PATH_HEAD "blue_2.png");
                    break;
                case 2:
                    lv_img_cache_invalidate_src(NULL);
                    lv_img_set_src(ui_ui2_Image_bg1, SRC2_PATH_HEAD "orange_1.jpg");
                    lv_img_set_src(ui_ui2_Image_bg2, SRC2_PATH_HEAD "orange_2.png");
                    break;
                case 3:
                    lv_img_cache_invalidate_src(NULL);
                    lv_img_set_src(ui_ui2_Image_bg1, SRC2_PATH_HEAD "red_1.jpg");
                    lv_img_set_src(ui_ui2_Image_bg2, SRC2_PATH_HEAD "red_2.png");
                    break;
                default:
                    lv_img_cache_invalidate_src(NULL);
                    lv_img_set_src(ui_ui2_Image_bg1, SRC2_PATH_HEAD "green_1.jpg");
                    lv_img_set_src(ui_ui2_Image_bg2, SRC2_PATH_HEAD "green_2.png");
                    break;
                }
            }

            if (app_interface.ui_data_temp.position_nomal != recv_ui_data->position_nomal ||
                app_interface.ui_data_temp.position_base != recv_ui_data->position_base ||
                app_interface.ui_data_temp.position_nomal_base != recv_ui_data->position_nomal_base)
            {
                app_interface.ui_data_temp.position_nomal = recv_ui_data->position_nomal;
                app_interface.ui_data_temp.position_base = recv_ui_data->position_base;
                app_interface.ui_data_temp.position_nomal_base = recv_ui_data->position_nomal_base;

                // if (recv_ui_data->position_nomal_base)
                if (recv_ui_data->position_base == 0)
                {
                    switch (recv_ui_data->position_nomal)
                    {
                    case 0:
                        ENABLE_FLAG_HIDDEN(ui_Image1, 0);
                        break;

                    default:
                        break;
                    }
                }
                else
                {
                    switch (recv_ui_data->position_base)
                    {
                    case 0:
                        ENABLE_FLAG_HIDDEN(ui_Image1, 0);
                        break;
                    case 1:
                        lv_img_set_src(ui_Image1, &ui_img_zhixing_png);
                        ENABLE_FLAG_HIDDEN(ui_Image1, 1);
                        break;
                    case 2:
                        lv_img_set_src(ui_Image1, &ui_img_diaotou_png);
                        ENABLE_FLAG_HIDDEN(ui_Image1, 1);
                        break;
                    case 3:
                        lv_img_set_src(ui_Image1, &ui_img_zuozhuan_png);
                        ENABLE_FLAG_HIDDEN(ui_Image1, 1);
                        break;
                    case 4:
                        lv_img_set_src(ui_Image1, &ui_img_youzhuan_png);
                        ENABLE_FLAG_HIDDEN(ui_Image1, 1);
                        break;
                    case 5:
                        lv_img_set_src(ui_Image1, &ui_img_zuoqian_png);
                        ENABLE_FLAG_HIDDEN(ui_Image1, 1);
                        break;
                    default:
                        break;
                    }
                }
            }

            if (app_interface.ui_data_temp.day_and_night_mode_on != recv_ui_data->day_and_night_mode_on)
            {
                app_interface.ui_data_temp.day_and_night_mode_on = recv_ui_data->day_and_night_mode_on;

                if (recv_ui_data->day_and_night_mode_on == 1)
                {
                    lv_obj_set_style_bg_color(ui_wft_ui2, lv_color_hex(0x01153D), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(ui_wft_ui2, 100 + recv_ui_data->speed_val, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                else
                {
                    lv_obj_set_style_bg_color(ui_wft_ui2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(ui_wft_ui2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
            /////////////////////////// 导航  图标控制  ////////////////////////////

            if (app_interface.ui_data_temp.oil_pot_voltage != recv_ui_data->oil_pot_voltage)
            {
                app_interface.ui_data_temp.oil_pot_voltage = recv_ui_data->oil_pot_voltage;
                ENABLE_FLAG_HIDDEN(ui_Image59, recv_ui_data->oil_pot_voltage);
            }

            if (app_interface.ui_data_temp.side_brace != recv_ui_data->side_brace)
            {
                app_interface.ui_data_temp.side_brace = recv_ui_data->side_brace;
                ENABLE_FLAG_HIDDEN(ui_Image52, recv_ui_data->side_brace);
            }

            if (app_interface.ui_data_temp.high_current_fault != recv_ui_data->high_current_fault)
            {
                app_interface.ui_data_temp.high_current_fault = recv_ui_data->high_current_fault;
                ENABLE_FLAG_HIDDEN(ui_Image53, recv_ui_data->high_current_fault);
            }

            if (app_interface.ui_data_temp.undervoltage_fault != recv_ui_data->undervoltage_fault)
            {
                app_interface.ui_data_temp.undervoltage_fault = recv_ui_data->undervoltage_fault;
                ENABLE_FLAG_HIDDEN(ui_Image54, recv_ui_data->undervoltage_fault);
            }

            if (app_interface.ui_data_temp.brake_failure != recv_ui_data->brake_failure)
            {
                app_interface.ui_data_temp.brake_failure = recv_ui_data->brake_failure;
                ENABLE_FLAG_HIDDEN(ui_Image58, recv_ui_data->brake_failure);
            }

            if (app_interface.ui_data_temp.trolley_failure != recv_ui_data->trolley_failure)
            {
                app_interface.ui_data_temp.trolley_failure = recv_ui_data->trolley_failure;
                ENABLE_FLAG_HIDDEN(ui_Image51, recv_ui_data->trolley_failure);
            }
            if (app_interface.ui_data_temp.ecu_failure != recv_ui_data->ecu_failure)
            {
                app_interface.ui_data_temp.ecu_failure = recv_ui_data->ecu_failure;
                ENABLE_FLAG_HIDDEN(ui_Image55, recv_ui_data->ecu_failure);
            }

            if (app_interface.ui_data_temp.hall_failure != recv_ui_data->hall_failure)
            {
                app_interface.ui_data_temp.hall_failure = recv_ui_data->hall_failure;
                ENABLE_FLAG_HIDDEN(ui_Image57, recv_ui_data->hall_failure);
            }
            if (app_interface.ui_data_temp.eco_energy_saving_display != recv_ui_data->eco_energy_saving_display)
            {
                app_interface.ui_data_temp.eco_energy_saving_display = recv_ui_data->eco_energy_saving_display;
                ENABLE_FLAG_HIDDEN(ui_Image60, recv_ui_data->eco_energy_saving_display);
            }

            if (app_interface.ui_data_temp.icon_Ready != recv_ui_data->icon_Ready)
            {
                app_interface.ui_data_temp.icon_Ready = recv_ui_data->icon_Ready;
                ENABLE_FLAG_HIDDEN(ui_Image29, recv_ui_data->icon_Ready);
            }

            if (app_interface.ui_data_temp.bluetooth != recv_ui_data->bluetooth)
            {
                app_interface.ui_data_temp.bluetooth = recv_ui_data->bluetooth;
                ENABLE_FLAG_HIDDEN(ui_Image49, recv_ui_data->bluetooth);
            }

            if (app_interface.ui_data_temp.headlight != recv_ui_data->headlight)
            {
                app_interface.ui_data_temp.headlight = recv_ui_data->headlight;
                ENABLE_FLAG_HIDDEN(ui_Image23, recv_ui_data->headlight);
            }
            if (app_interface.ui_data_temp.icon_Cruise != recv_ui_data->icon_Cruise)
            {
                app_interface.ui_data_temp.icon_Cruise = recv_ui_data->icon_Cruise;
                ENABLE_FLAG_HIDDEN(ui_Image30, recv_ui_data->icon_Cruise);
            }

            if (app_interface.ui_data_temp.icon_Position != recv_ui_data->icon_Position)
            {
                app_interface.ui_data_temp.icon_Position = recv_ui_data->icon_Position;
                ENABLE_FLAG_HIDDEN(ui_Image36, recv_ui_data->icon_Position);
            }

            if (app_interface.ui_data_temp.icon_Turn_L != recv_ui_data->icon_Turn_L)
            {
                app_interface.ui_data_temp.icon_Turn_L = recv_ui_data->icon_Turn_L;
                ENABLE_FLAG_HIDDEN(ui_Image41, recv_ui_data->icon_Turn_L);
            }

            if (app_interface.ui_data_temp.icon_Turn_R != recv_ui_data->icon_Turn_R)
            {
                app_interface.ui_data_temp.icon_Turn_R = recv_ui_data->icon_Turn_R;
                ENABLE_FLAG_HIDDEN(ui_Image42, recv_ui_data->icon_Turn_R);
            }
            if (app_interface.ui_data_temp.icon_Call != recv_ui_data->icon_Call)
            {
                app_interface.ui_data_temp.icon_Call = recv_ui_data->icon_Call;
                ENABLE_FLAG_HIDDEN(ui_Image39, recv_ui_data->icon_Call);
            }
            if (app_interface.ui_data_temp.icon_Msg != recv_ui_data->icon_Msg)
            {
                app_interface.ui_data_temp.icon_Msg = recv_ui_data->icon_Msg;
                ENABLE_FLAG_HIDDEN(ui_Image37, recv_ui_data->icon_Msg);
            }

            if ((recv_ui_data->speed_val != app_interface.ui_data_temp.speed_val) || (app_interface.ui_data_temp.speed_display != recv_ui_data->speed_display))
            {
                app_interface.ui_data_temp.speed_val = recv_ui_data->speed_val;
                app_interface.ui_data_temp.speed_display = recv_ui_data->speed_display;

                rt_memset(temp_str, 0, 300);
                sprintf(temp_str, "%d", (int)recv_ui_data->speed_val);
                lv_label_set_text(ui_ui2_Label_bgnum, temp_str);
                ENABLE_FLAG_HIDDEN(ui_ui2_Label_bgnum, recv_ui_data->speed_display);
                ENABLE_FLAG_HIDDEN(ui_ui2_Label_kmh, recv_ui_data->speed_display);
            }
            //
            if (recv_ui_data->time_display == 1) // 显示时间
            {
                if ((app_interface.ui_data_temp.time_hours != recv_ui_data->time_hours) || (app_interface.ui_data_temp.time_minutes != recv_ui_data->time_minutes))
                {
                    app_interface.ui_data_temp.time_hours = recv_ui_data->time_hours;
                    app_interface.ui_data_temp.time_minutes = recv_ui_data->time_minutes;

                    rt_memset(temp_str, 0, 300);
                    sprintf(temp_str, "%d:%02d", (int)recv_ui_data->time_hours, (int)recv_ui_data->time_minutes);
                    lv_label_set_text(ui_Label11, temp_str);
                }
                lv_obj_clear_flag(ui_Label11, LV_OBJ_FLAG_HIDDEN);
            }
            else
            {
                lv_obj_add_flag(ui_Label11, LV_OBJ_FLAG_HIDDEN);
            }

            if (recv_ui_data->total_display_status == 1) // 总里程显示状态          【0:显示关；1:显示开】
            {
                if (recv_ui_data->odo_km != app_interface.ui_data_temp.odo_km)
                {

                    app_interface.ui_data_temp.odo_km = recv_ui_data->odo_km;
                    rt_memset(temp_str, 0, 300);
                    sprintf(temp_str, "%d", (int)recv_ui_data->odo_km);
                    lv_label_set_text(ui_Label14, temp_str);
                }
                lv_obj_clear_flag(ui_Label14, LV_OBJ_FLAG_HIDDEN);
                lv_obj_clear_flag(ui_Image35, LV_OBJ_FLAG_HIDDEN);
                lv_obj_clear_flag(ui_Image33, LV_OBJ_FLAG_HIDDEN);
            }
            else
            {
                lv_obj_add_flag(ui_Label14, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(ui_Image35, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(ui_Image33, LV_OBJ_FLAG_HIDDEN);
            }

            if (recv_ui_data->trip_display_status == 1) //  单次程显示状态          【0:显示关；1:显示开】
            {
                if (recv_ui_data->trip_km != app_interface.ui_data_temp.trip_km)
                {
                    lv_img_cache_invalidate_src(NULL);
                    app_interface.ui_data_temp.trip_km = recv_ui_data->trip_km;
                    rt_memset(temp_str, 0, 300);
                    sprintf(temp_str, "%d", (int)recv_ui_data->trip_km);
                    lv_label_set_text(ui_Label19, temp_str);
                }
                lv_obj_clear_flag(ui_Label19, LV_OBJ_FLAG_HIDDEN);
                lv_obj_clear_flag(ui_Image34, LV_OBJ_FLAG_HIDDEN);
                lv_obj_clear_flag(ui_Image32, LV_OBJ_FLAG_HIDDEN);
            }
            else
            {
                lv_obj_add_flag(ui_Label19, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(ui_Image34, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(ui_Image32, LV_OBJ_FLAG_HIDDEN);
            }

        } /*******************           recv_ui_data->navigate == 1        ****************************/
    }     ///////* 从邮箱中收取邮件 */
    /////////////////////////           一直需要调用
    if (recv_ui_data->navigate == 0)
    {
        if (recv_ui_data != NULL)
        {
            switch (recv_ui_data->arc_speed)
            {
            case 0:
                // range_cnt = 0; // (range_cnt + 0) % 360;
                break;
            case 1:
                range_cnt = (range_cnt + 1) % 360;
                break;
            case 2:
                range_cnt = (range_cnt + 3) % 360;
                break;
            case 3:
                range_cnt = (range_cnt + 6) % 360;
                break;

            default:
                range_cnt = (range_cnt + 0) % 360;
                break;
            }

            if (rt_sem_take(&sem_refresh_rate, RT_WAITING_NO) == RT_EOK)
            {
                lv_img_set_angle(ui_ui1_Image_bg1, range_cnt * 10);
                lv_img_set_angle(ui_ui1_Image_bg2, 3600 - range_cnt * 10);
            }
        }

        // if (rt_sem_take(&sem_evasive, RT_WAITING_NO) == RT_EOK) //  需要闪烁的控件
        // {
        //     if (recv_ui_data != NULL)
        //     {
        //         ////
        //     }
        // }
    }
    else
    { /*******************           recv_ui_data->navigate == 1        ****************************/

        if (recv_ui_data != NULL)
        {
            switch (recv_ui_data->arc_speed)
            {
            case 0:
                // range_cnt = 0; // (range_cnt + 0) % 360;
                break;
            case 1:
                range_cnt = (range_cnt + 1) % 360;
                break;
            case 2:
                range_cnt = (range_cnt + 3) % 360;
                break;
            case 3:
                range_cnt = (range_cnt + 6) % 360;
                break;

            default:
                range_cnt = (range_cnt + 0) % 360;
                break;
            }

            if (rt_sem_take(&sem_refresh_rate, RT_WAITING_NO) == RT_EOK)
            {
                lv_img_set_angle(ui_ui2_Image_bg1, range_cnt * 10);
                lv_img_set_angle(ui_ui2_Image_bg2, 3600 - range_cnt * 10);
            }
        }
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

// /**************************************/
// rt_kprintf("lv_disp_load_scr \r\n");

// lv_timer_create(bar_test, 30, NULL);
// /**************************************/

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

/* 定时器的控制块 */
static struct rt_timer timer_evasive_t;

/* 定时器 1 超时函数 */
static void timeout_evasive1(void *parameter)
{
    static int time_cnt = 0;
    time_cnt++;

    if ((time_cnt % 50) == 0)
    {
        rt_sem_release(&sem_refresh_rate);
    }

    if (time_cnt >= 500)
    {
        if (app_interface.ui_data.sta_evasive_bit == 1)
            app_interface.ui_data.sta_evasive_bit = 0;
        else
            app_interface.ui_data.sta_evasive_bit = 1;
        rt_sem_release(&sem_evasive);
        time_cnt = 0;
    }
}

int timer_evasive1_sample(void)
{
    /* 初始化定时器 */
    rt_timer_init(&timer_evasive_t, "t_evasive", /* 定时器名字是 timer1 */
                  timeout_evasive1,              /* 超时时回调的处理函数 */
                  RT_NULL,                       /* 超时函数的入口参数 */
                  1,                             /* 定时长度，以 OS Tick 为单位，即 10 个 OS Tick */
                  RT_TIMER_FLAG_PERIODIC);       /* 周期性定时器 */
    /* 启动定时器 */
    rt_timer_start(&timer_evasive_t);
    return 0;
}

#define THREAD_PRIORITY 24
#define THREAD_STACK_SIZE 1024
#define THREAD_TIMESLICE 5

ALIGN(RT_ALIGN_SIZE)
static char thread_stack[THREAD_STACK_SIZE];
static struct rt_thread thread;

static void thread_entry(void *param)
{
    static unsigned int run_time = 0;
    while (1)
    {
        run_time++;

        app_wft_protocol.wft_cmd_1_reply.run_time_byte0 = (unsigned char)(run_time >> (0 * 8));
        app_wft_protocol.wft_cmd_1_reply.run_time_byte1 = (unsigned char)(run_time >> (1 * 8));
        app_wft_protocol.wft_cmd_1_reply.run_time_byte2 = (unsigned char)(run_time >> (2 * 8));
        app_wft_protocol.wft_cmd_1_reply.run_time_byte3 = (unsigned char)(run_time >> (3 * 8));
        rt_thread_mdelay(50);
    }
}

int p_app_init(void)
{

    rt_err_t result;

    /* 初始化一个 mailbox */
    result = rt_mb_init(&ui_data_mb,
                        "ui_mb",                     /* 名称是 mbt */
                        &ui_data_mb_pool[0],         /* 邮箱用到的内存池是 mb_pool */
                        sizeof(ui_data_mb_pool) / 4, /* 邮箱中的邮件数目，因为一封邮件占 4 字节 */
                        RT_IPC_FLAG_FIFO);           /* 采用 FIFO 方式进行线程等待 */
    if (result != RT_EOK)
    {
        rt_kprintf("init mailbox failed.\n");
    }
    else
    {
        rt_kprintf("init mailbox ok !\n");
        rt_memset(&app_interface.ui_data, 0, sizeof(S_UI_DATA));
        rt_memset(&app_interface.ui_data_temp, 0xFF, sizeof(S_UI_DATA));

        rt_mb_send(&ui_data_mb, (rt_uint32_t)&app_interface.ui_data);
    }

    rt_sem_init(&sem_evasive, "sem_evasive", 0, RT_IPC_FLAG_PRIO);
    rt_sem_init(&sem_refresh_rate, "sem_refresh_rate", 0, RT_IPC_FLAG_PRIO);
    timer_evasive1_sample();
    rt_thread_init(&thread,
                   "user_app",
                   thread_entry,
                   RT_NULL,
                   &thread_stack[0],
                   THREAD_STACK_SIZE,
                   THREAD_PRIORITY,
                   THREAD_TIMESLICE);
    rt_thread_startup(&thread);
    rt_kprintf("rt_thread_init ok ! \r\n");

    // lv_timer_create(ui_change_funtion, 30, NULL);

    ///////////////////////////////////////////////////////
    app_interface.wft_protocol->f_init();
    return 0;
}
// MSH_CMD_EXPORT(bar_set, bar_set sample);
// INIT_LATE_APP_EXPORT(bar_set);

////////////////////////////////////////////////////
// int mb_set(int argc, char **argv)
// {
//     rt_kprintf("mb_set argc : %d \n", argc);

//     for (int i = 0; i < argc; i++)
//     {
//         rt_kprintf("argv[%d]: %s \n", i, argv[i]);
//     }

//     if (argv[1] != NULL)
//     {
//         app_interface.ui_data.speed_val = atoi(argv[1]);
//         // app_interface.ui_data.bt_percent = atoi(argv[2]);
//     }
//     if (argv[2] != NULL)
//     {
//         app_interface.ui_data.gear = atoi(argv[2]);
//         // app_interface.ui_data.bt_percent = atoi(argv[2]);
//     }

//     if (argv[3] != NULL)
//     {
//         app_interface.ui_data.icon_Turn_L = atoi(argv[3]);
//         app_interface.ui_data.icon_Turn_L = app_interface.ui_data.icon_Turn_L;
//         app_interface.ui_data.icon_Turn_R = app_interface.ui_data.icon_Turn_L;
//         app_interface.ui_data.icon_Ready = app_interface.ui_data.icon_Turn_L;
//         app_interface.ui_data.icon_Cruise = app_interface.ui_data.icon_Turn_L;
//         app_interface.ui_data.icon_Position = app_interface.ui_data.icon_Turn_L;
//         app_interface.ui_data.icon_Call = app_interface.ui_data.icon_Turn_L;
//         app_interface.ui_data.icon_Msg = app_interface.ui_data.icon_Turn_L;
//     }
//     // if (argv[4] != NULL)
//     // {
//     //     app_interface.ui_data.trip_km = atoi(argv[4]);
//     // }
//     // if (argv[5] != NULL)
//     // {
//     //     app_interface.ui_data.gear = atoi(argv[5]);
//     // }

//     // if (argv[6] != NULL)
//     // {
//     //     app_interface.ui_data.navigate = atoi(argv[6]);
//     // }

//     // send_ui_data.arc_val = 100 - send_ui_data.bar_val;
//     // rt_kprintf("send_ui_data.bar_val : %d \n", send_ui_data.bar_val);
//     rt_mb_send(&ui_data_mb, (rt_uint32_t)&app_interface.ui_data);
// }
// MSH_CMD_EXPORT(mb_set, mb_set sample);

////////////////////////////////////////////////////
// int mb_call(int argc, char **argv)
// {
//     rt_kprintf("mb_call  argc : %d \n", argc);

//     for (int i = 0; i < argc; i++)
//     {
//         rt_kprintf("argv[%d]: %s \n", i, argv[i]);
//     }

//     memcpy(app_interface.ui_data.msg_call_number, argv[1], strlen(argv[1]));

//     // memcpy(app_interface.ui_data.navigation_description, argv[1], strlen(argv[1]));
//     // memcpy(app_interface.ui_data.time_destination, argv[1], strlen(argv[1]));
//     // memcpy(app_interface.ui_data.time_left, argv[1], strlen(argv[1]));

//     // rt_kprintf("send_ui_data.bar_val : %d \n", send_ui_data.bar_val);
//     rt_mb_send(&ui_data_mb, (rt_uint32_t)&app_interface.ui_data);
// }
// MSH_CMD_EXPORT(mb_call, mb_call sample);

//  E4 B8 A4 E4 B8 AA E9 BB 84 E9 B9 82 E9 B8 A3 E7 BF A0 E6 9F B3 EF BC 8C E4 B8 80 E8 A1 8C E7 99 BD E9 B9 AD E4 B8 8A E9 9D 92 E5 A4 A9 E3 80 82
