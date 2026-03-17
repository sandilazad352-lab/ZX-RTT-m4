
#include <stdint.h>
#include "app_wft_protocol.h"
#include "app_interface.h"
#include <string.h>
#include "bsp_uart.h"

S_APP_WFT_PROTOCOL app_wft_protocol = {
    .f_wft_prl_tx = p_wft_protocol_tx,
    .f_wft_analysis = p_wft_analysis_rx,
    .f_init = p_wft_init};
extern rt_device_t wft_prl_serial;

unsigned char HEAD[2] = {0xFF, 0xA5};
unsigned char TAIL[2] = {0xEE, 0xA6};

#define DATA_ADDR 4
static unsigned char word_cnt = 0, remain_cnt = 0;
static unsigned char remaining_mileage[30] = {0};
unsigned char meter[] = {0xE7, 0xB1, 0xB3, 0xE5, 0x90, 0x8E, 0xE8, 0xBF, 0x9B, 0xE5, 0x85, 0xA5, 0xEF, 0xBC, 0x8C}; // 15Byte 米后进入，

void p_wft_protocol_tx(uint8_t cmd, uint8_t Data_Lenth, uint8_t *code_data_Buff)
{

    uint8_t Protocol_Lenth, check_sum = 0;
    static unsigned char Data_Buff[255] = {0};
    Data_Lenth = Data_Lenth + 1;     // 需要加上指令
    Protocol_Lenth = Data_Lenth + 6; // （ 内容+命令） + 头尾 校验码 长度
    memcpy(&Data_Buff[0], HEAD, 2);
    Data_Buff[2] = Data_Lenth;
    Data_Buff[3] = cmd; // ID
    /**********************************************/
    if (code_data_Buff != NULL)
        memcpy(&Data_Buff[4], code_data_Buff, Data_Lenth); // 将数据放在数据字节位置
    /**********************************************/
    for (int i = 0; i < Data_Lenth + 1; i++)
    {
        check_sum += Data_Buff[2 + i];
    }
    Data_Buff[Protocol_Lenth - 3] = check_sum;
    memcpy(&Data_Buff[Protocol_Lenth - 2], TAIL, 2);
    /*----------------------------------------------*/
    rt_device_write(wft_prl_serial, 0, Data_Buff, Protocol_Lenth);
    /*----------------------------------------------*/
}
/*!
    \brief      报文解析协议
    \param[in]  接收数据缓存
    \param[out] none
    \retval     none
*/
extern struct rt_mailbox ui_data_mb;
void p_wft_analysis_rx(uint8_t *Rx_Data_Buff)
{
    uint8_t Protocol_Lenth = 0, Rx_Check_Sum = 0, Check_Sum_Compute = 0;
    uint8_t Data_Len = 0, cmd = 0;
    ////////////////////////////////////////////////////////////////////////////////////////
    Data_Len = Rx_Data_Buff[2];
    Protocol_Lenth = Data_Len + 6;
    Rx_Check_Sum = Rx_Data_Buff[Protocol_Lenth - 3];

    if ((Rx_Data_Buff[0] == HEAD[0]) &&
        (Rx_Data_Buff[1] == HEAD[1]) &&
        (Rx_Data_Buff[Protocol_Lenth - 1] == TAIL[1]) &&
        (Rx_Data_Buff[Protocol_Lenth - 2] == TAIL[0]))
    {
        for (int i = 0; i < Data_Len + 1; i++)
        {
            Check_Sum_Compute += Rx_Data_Buff[2 + i];
        }
        // rt_kprintf("Rx_Check_Sum: %X \n", Rx_Check_Sum);
        // rt_kprintf("Check_Sum_Compute: %X \n", Check_Sum_Compute);
        if (Rx_Check_Sum == Check_Sum_Compute)
        {
            cmd = Rx_Data_Buff[3];
            // rt_kprintf("cmd: %X \n", cmd);
            switch (cmd) // 指令
            {
            case CMD_HEART_BEAT:
                rt_memcpy(&app_wft_protocol.wft_cmd_1, &Rx_Data_Buff[4], sizeof(S_WFT_CMD_1));
                if (rt_memcmp(&app_wft_protocol.wft_cmd_1_temp, &app_wft_protocol.wft_cmd_1, sizeof(S_WFT_CMD_1)) != 0)
                {
                    // rt_kprintf("rt_mb_send\n");

                    rt_memcpy(&app_wft_protocol.wft_cmd_1_temp, &app_wft_protocol.wft_cmd_1, sizeof(S_WFT_CMD_1));

                    app_interface.ui_data.kinetic_energy_recovery = app_wft_protocol.wft_cmd_1.kinetic_energy_recovery;
                    app_interface.ui_data.eco_energy_saving_display = app_wft_protocol.wft_cmd_1.eco_energy_saving_display;
                    app_interface.ui_data.oil_pot_voltage = app_wft_protocol.wft_cmd_1.oil_pot_voltage;
                    app_interface.ui_data.side_brace = app_wft_protocol.wft_cmd_1.side_brace;                 //  侧撑                    【0:显示关；1:显示开】
                    app_interface.ui_data.high_current_fault = app_wft_protocol.wft_cmd_1.high_current_fault; //  大电流故障              【0:显示关；1:显示开】
                    app_interface.ui_data.undervoltage_fault = app_wft_protocol.wft_cmd_1.undervoltage_fault; //  欠压故障                【0:显示关；1:显示开】
                    app_interface.ui_data.brake_failure = app_wft_protocol.wft_cmd_1.brake_failure;           //  刹车故障                【0:显示关；1:显示开】
                    app_interface.ui_data.trolley_failure = app_wft_protocol.wft_cmd_1.trolley_failure;       //  转把故障                【0:显示关；1:显示开】
                    app_interface.ui_data.ecu_failure = app_wft_protocol.wft_cmd_1.ecu_failure;               //  ECU故障                 【0:显示关；1:显示开】
                    app_interface.ui_data.hall_failure = app_wft_protocol.wft_cmd_1.hall_failure;             //  霍尔故障                【0:显示关；1:显示开】

                    app_interface.ui_data.icon_Turn_L = app_wft_protocol.wft_cmd_1.left_turn_signal;
                    app_interface.ui_data.icon_Turn_R = app_wft_protocol.wft_cmd_1.right_turn_signal;
                    app_interface.ui_data.icon_Ready = app_wft_protocol.wft_cmd_1.prepare;
                    app_interface.ui_data.icon_Cruise = app_wft_protocol.wft_cmd_1.cruise;
                    app_interface.ui_data.icon_Position = app_wft_protocol.wft_cmd_1.position_light;

                    // app_interface.ui_data.icon_Call =       app_wft_protocol.wft_cmd_1. ;
                    // app_interface.ui_data.icon_Msg =        app_wft_protocol.wft_cmd_1. ;
                    app_interface.ui_data.speed_val = app_wft_protocol.wft_cmd_1.speed;
                    app_interface.ui_data.gear = app_wft_protocol.wft_cmd_1.gear;               // gear : 2;        //  档位                    【0:P档  1:前进档  2:倒车档】
                    app_interface.ui_data.memory_gear = app_wft_protocol.wft_cmd_1.three_speed; // three_speed : 4; //  三速                    【0:无档位D   1:低速D1  2:中速D2  3:高速D3  4:运动S】

                 app_interface.ui_data.day_and_night_mode_on = app_wft_protocol.wft_cmd_1.day_and_night_mode_on; ////  昼夜模式开启           【 1:白天  0：黑夜】

                    app_interface.ui_data.run_color = app_wft_protocol.wft_cmd_1.run_color;
                    app_interface.ui_data.arc_speed = app_wft_protocol.wft_cmd_1.arc_speed;
                    app_interface.ui_data.headlight = app_wft_protocol.wft_cmd_1.headlight;

                    app_interface.ui_data.time_display = app_wft_protocol.wft_cmd_1.time_display;
                    app_interface.ui_data.time_hours = app_wft_protocol.wft_cmd_1.time_hours;
                    app_interface.ui_data.time_minutes = app_wft_protocol.wft_cmd_1.time_minutes;

                    app_interface.ui_data.time_display = app_wft_protocol.wft_cmd_1.time_display;
                    app_interface.ui_data.bluetooth = app_wft_protocol.wft_cmd_1.bluetooth;

                    app_interface.ui_data.speed_display = app_wft_protocol.wft_cmd_1.speed_display;
                    app_interface.ui_data.navigate = app_wft_protocol.wft_cmd_1.navigate;

                    app_interface.ui_data.battery_a_power_display = app_wft_protocol.wft_cmd_1.battery_a_power_display;     //  电池A电量显示           【0:显示关；1:显示开】
                    app_interface.ui_data.battery_a_voltage_display = app_wft_protocol.wft_cmd_1.battery_a_voltage_display; //  电池A电压显示           【0:显示关；1:显示开】
                    app_interface.ui_data.battery_b_power_display = app_wft_protocol.wft_cmd_1.battery_b_power_display;     //  电池B电量显示           【0:显示关；1:显示开】
                    app_interface.ui_data.battery_b_voltage_display = app_wft_protocol.wft_cmd_1.battery_b_voltage_display; //  电池B电压显示           【0:显示关；1:显示开】

                    app_interface.ui_data.bt_percent_a = app_wft_protocol.wft_cmd_1.battery_a_power;
                    app_interface.ui_data.bt_percent_b = app_wft_protocol.wft_cmd_1.battery_b_power;
                    app_interface.ui_data.bt_voltage_a = ((app_wft_protocol.wft_cmd_1.battery_a_voltage_byte2 | 0x0000) << 8) |
                                                         ((app_wft_protocol.wft_cmd_1.battery_a_voltage_byte1 | 0x0000) << 0);
                    app_interface.ui_data.bt_voltage_b = ((app_wft_protocol.wft_cmd_1.battery_b_voltage_byte2 | 0x0000) << 8) |
                                                         ((app_wft_protocol.wft_cmd_1.battery_b_voltage_byte1 | 0x0000) << 0);

                    app_interface.ui_data.total_display_status = app_wft_protocol.wft_cmd_1.total_mileage_display_status;
                    app_interface.ui_data.odo_km = ((app_wft_protocol.wft_cmd_1.total_mileage_byte3 | 0x000000) << (2 * 8)) |
                                                   ((app_wft_protocol.wft_cmd_1.total_mileage_byte2 | 0x000000) << (1 * 8)) |
                                                   ((app_wft_protocol.wft_cmd_1.total_mileage_byte1 | 0x000000) << (0 * 8));

                    app_interface.ui_data.trip_display_status = app_wft_protocol.wft_cmd_1.single_trip_display_status;
                    app_interface.ui_data.trip_km = ((app_wft_protocol.wft_cmd_1.single_mileage_byte3 | 0x000000) << (2 * 8)) |
                                                    ((app_wft_protocol.wft_cmd_1.single_mileage_byte2 | 0x000000) << (1 * 8)) |
                                                    ((app_wft_protocol.wft_cmd_1.single_mileage_byte1 | 0x000000) << (0 * 8));

                    rt_mb_send(&ui_data_mb, (rt_uint32_t)&app_interface.ui_data);
                }

                app_wft_protocol.f_wft_prl_tx(0x01, 5, &app_wft_protocol.wft_cmd_1_reply);

                break;
            case CMD_CALL_MSG: // 电话信息（Type：0x02）
                rt_memcpy(&app_wft_protocol.wft_cmd_2, &Rx_Data_Buff[DATA_ADDR + 0], sizeof(S_WFT_CMD_2));
                rt_memcpy(&app_interface.ui_data.msg_call_number, &Rx_Data_Buff[7], app_wft_protocol.wft_cmd_2.call_len + app_wft_protocol.wft_cmd_2.name_len);
                rt_mb_send(&ui_data_mb, (rt_uint32_t)&app_interface.ui_data);
                break;
            case CMD_NA_MSG0: // 导航信息（Type：0x03）
                rt_memcpy(&app_wft_protocol.wft_cmd_3, &Rx_Data_Buff[DATA_ADDR + 0], sizeof(S_WFT_CMD_3));
                if (app_wft_protocol.wft_cmd_3.wft_pakg_msg.pakg_now == 0)
                {
                    app_interface.ui_data.position_base = app_wft_protocol.wft_cmd_3.position_base;
                    app_interface.ui_data.position_nomal = ((app_wft_protocol.wft_cmd_3.position_nomal1 | 0x0000) << (1 * 8)) |
                                                           ((app_wft_protocol.wft_cmd_3.position_nomal0 | 0x0000) << (0 * 8));
                    app_interface.ui_data.mileage_unit = app_wft_protocol.wft_cmd_3.mileage_unit;
                    app_interface.ui_data.current_dir_Remaining_mileage = ((app_wft_protocol.wft_cmd_3.current_dir_remaining_mileage3 | 0x00000000) << (3 * 8)) |
                                                                          ((app_wft_protocol.wft_cmd_3.current_dir_remaining_mileage2 | 0x00000000) << (2 * 8)) |
                                                                          ((app_wft_protocol.wft_cmd_3.current_dir_remaining_mileage1 | 0x00000000) << (1 * 8)) |
                                                                          ((app_wft_protocol.wft_cmd_3.current_dir_remaining_mileage0 | 0x00000000) << (0 * 8));
                    app_interface.ui_data.total_remaining_mileage = ((app_wft_protocol.wft_cmd_3.total_remaining_mileage3 | 0x00000000) << (3 * 8)) |
                                                                    ((app_wft_protocol.wft_cmd_3.total_remaining_mileage2 | 0x00000000) << (2 * 8)) |
                                                                    ((app_wft_protocol.wft_cmd_3.total_remaining_mileage1 | 0x00000000) << (1 * 8)) |
                                                                    ((app_wft_protocol.wft_cmd_3.total_remaining_mileage0 | 0x00000000) << (0 * 8));
                    app_interface.ui_data.estimated_time = ((app_wft_protocol.wft_cmd_3.estimated_time1 | 0x0000) << (1 * 8)) |
                                                           ((app_wft_protocol.wft_cmd_3.estimated_time0 | 0x0000) << (0 * 8));

                    rt_memset(remaining_mileage, 0, 30);
                    rt_sprintf(remaining_mileage, "%d", app_interface.ui_data.current_dir_Remaining_mileage);

                    remain_cnt = rt_strlen(remaining_mileage);
                    rt_memcpy(&remaining_mileage[remain_cnt], meter, 15);
                    remain_cnt += 15;
                    rt_memcpy(&app_interface.ui_data.navigation_description[0], remaining_mileage, remain_cnt);
                    // rt_kprintf("remaining_mileage : %s \r\n", remaining_mileage);
                    // /////////////////////////////////////////
                    // rt_kprintf("pakg_sum : %d \r\n", app_wft_protocol.wft_cmd_3.wft_pakg_msg.pakg_sum);
                    // rt_kprintf("pakg_now : %d \r\n", app_wft_protocol.wft_cmd_3.wft_pakg_msg.pakg_now);
                    // rt_kprintf("position_base : %d \r\n", app_interface.ui_data.position_base);
                    // rt_kprintf("position_nomal : %d \r\n", app_interface.ui_data.position_nomal);
                    // rt_kprintf("mileage_unit : %d \r\n", app_interface.ui_data.mileage_unit);
                    // rt_kprintf("mileage : %d \r\n", app_interface.ui_data.current_dir_Remaining_mileage);
                    // rt_kprintf("total_mileage : %d \r\n", app_interface.ui_data.total_remaining_mileage);
                    // rt_kprintf("estimated_time : %d \r\n", app_interface.ui_data.estimated_time);

                    word_cnt = app_wft_protocol.wft_cmd_3.next_intersection_name_length;
                    /////////////////////////////////////////
                    rt_memcpy(&app_interface.ui_data.navigation_description[remain_cnt], &Rx_Data_Buff[4 + sizeof(S_WFT_CMD_3)], app_wft_protocol.wft_cmd_3.next_intersection_name_length);
                }
                else
                {
                    rt_memcpy(&app_interface.ui_data.navigation_description[remain_cnt + word_cnt], &Rx_Data_Buff[4], app_wft_protocol.wft_cmd_3.next_intersection_name_length);
                }

                // rt_memcpy(&app_interface.ui_data.navigation_description, &Rx_Data_Buff[7], app_wft_protocol.wft_cmd_2.call_len + app_wft_protocol.wft_cmd_2.name_len);
                if (app_wft_protocol.wft_cmd_3.wft_pakg_msg.pakg_sum == (app_wft_protocol.wft_cmd_3.wft_pakg_msg.pakg_now + 1))
                {
                    word_cnt = 0;
                    remain_cnt = 0;
                    rt_mb_send(&ui_data_mb, (rt_uint32_t)&app_interface.ui_data);
                }

                break;

            case CMD_WEATHER:
                rt_memcpy(&app_wft_protocol.wft_cmd_4.wft_pakg_msg, &Rx_Data_Buff[DATA_ADDR + 0], 1);

                if (app_wft_protocol.wft_cmd_4.wft_pakg_msg.pakg_now == 0)
                {
                    rt_memcpy(&app_wft_protocol.wft_cmd_4.address_length, &Rx_Data_Buff[DATA_ADDR + 1], 1);
                    rt_memcpy(&app_interface.ui_data.weather_addr, &Rx_Data_Buff[DATA_ADDR + 2], app_wft_protocol.wft_cmd_4.address_length);
                    rt_memcpy(&app_wft_protocol.wft_cmd_4.wft_cmd_4_f, &Rx_Data_Buff[DATA_ADDR + 2 + app_wft_protocol.wft_cmd_4.address_length], sizeof(S_WFT_CMD_4_F));

                    app_interface.ui_data.current_temperature = app_wft_protocol.wft_cmd_4.wft_cmd_4_f.current_temperature; //  当前温度
                    app_interface.ui_data.maximum_temperature = app_wft_protocol.wft_cmd_4.wft_cmd_4_f.maximum_temperature; //  最高温度
                    app_interface.ui_data.lowest_temperature = app_wft_protocol.wft_cmd_4.wft_cmd_4_f.lowest_temperature;   //  最低温度
                    app_interface.ui_data.weather_code = app_wft_protocol.wft_cmd_4.wft_cmd_4_f.weather_code;               //  天气代码
                    app_interface.ui_data.wind_speed = app_wft_protocol.wft_cmd_4.wft_cmd_4_f.wind_speed;                   //  风速
                    app_interface.ui_data.relative_humidity = app_wft_protocol.wft_cmd_4.wft_cmd_4_f.relative_humidity;     //  相对湿度
                    app_interface.ui_data.visibility = app_wft_protocol.wft_cmd_4.wft_cmd_4_f.visibility;                   //  能见度
                    app_interface.ui_data.uv_intensity = app_wft_protocol.wft_cmd_4.wft_cmd_4_f.uv_intensity;               //  紫外线强度
                    app_interface.ui_data.rainfall = ((app_wft_protocol.wft_cmd_4.wft_cmd_4_f.rainfall_byte2 | 0x0000) << (1 * 8)) |
                                                     ((app_wft_protocol.wft_cmd_4.wft_cmd_4_f.rainfall_byte1 | 0x0000) << (0 * 8)); //  雨量

                    // rt_kprintf("address_length    : %d \r\n", app_wft_protocol.wft_cmd_4.address_length);
                    // rt_kprintf("weather_addr    : %s \r\n", app_interface.ui_data.weather_addr);               //  天气 地址
                    // rt_kprintf("current_temperature    : %d \r\n", app_interface.ui_data.current_temperature); //  当前温度
                    // rt_kprintf("maximum_temperature    : %d \r\n", app_interface.ui_data.maximum_temperature); //  最高温度
                    // rt_kprintf("lowest_temperature     : %d \r\n", app_interface.ui_data.lowest_temperature);  //  最低温度
                    // rt_kprintf("weather_code           : %d \r\n", app_interface.ui_data.weather_code);        //  天气代码
                    // rt_kprintf("wind_speed             : %d \r\n", app_interface.ui_data.wind_speed);          //  风速
                    // rt_kprintf("relative_humidity      : %d \r\n", app_interface.ui_data.relative_humidity);   //  相对湿度
                    // rt_kprintf("visibility             : %d \r\n", app_interface.ui_data.visibility);          //  能见度
                    // rt_kprintf("uv_intensity           : %d \r\n", app_interface.ui_data.uv_intensity);        //  紫外线强度
                    // rt_kprintf("rainfall         : %d \r\n", app_interface.ui_data.rainfall);                  //  雨量 byte1

                    /////////////////////////////////////////
                }
                if (app_wft_protocol.wft_cmd_4.wft_pakg_msg.pakg_sum == (app_wft_protocol.wft_cmd_4.wft_pakg_msg.pakg_now + 1))
                {
                    rt_mb_send(&ui_data_mb, (rt_uint32_t)&app_interface.ui_data);
                }
                break;

                // case CMD_NA_MSG1:
                //     rt_memcpy(&app_wft_protocol.wft_cmd_2, &Rx_Data_Buff[4], sizeof(S_WFT_CMD_2));
                //     rt_memcpy(&app_interface.ui_data.time_destination, &Rx_Data_Buff[7], app_wft_protocol.wft_cmd_2.call_len + app_wft_protocol.wft_cmd_2.name_len);
                //     rt_mb_send(&ui_data_mb, (rt_uint32_t)&app_interface.ui_data);
                //     break;
                // case CMD_NA_MSG2:
                //     rt_memcpy(&app_wft_protocol.wft_cmd_2, &Rx_Data_Buff[4], sizeof(S_WFT_CMD_2));
                //     rt_memcpy(&app_interface.ui_data.time_left, &Rx_Data_Buff[7], app_wft_protocol.wft_cmd_2.call_len + app_wft_protocol.wft_cmd_2.name_len);
                //     rt_mb_send(&ui_data_mb, (rt_uint32_t)&app_interface.ui_data);
                //     break;
                // case CMD_NA_MSG3:
                //     rt_memcpy(&app_wft_protocol.wft_cmd_3, &Rx_Data_Buff[4], sizeof(S_WFT_CMD_3));

                //     app_interface.ui_data.position_nomal = app_wft_protocol.wft_cmd_3.position_nomal;
                //     app_interface.ui_data.position_en = app_wft_protocol.wft_cmd_3.position_en;
                //     app_interface.ui_data.position_all = app_wft_protocol.wft_cmd_3.position_all;

                //     rt_mb_send(&ui_data_mb, (rt_uint32_t)&app_interface.ui_data);
                //     break;

            default:
                break;
            }
        }
    }
}
void p_wft_init(void)
{
    bsp_uart.f_init(p_wft_analysis_rx);
}
/***********************************************************************/
int prl(int argc, char **argv)
{

    rt_kprintf("argc : %d \n", argc);

    for (int i = 0; i < argc; i++)
    {
        rt_kprintf("argv[%d]: %s \n", i, argv[i]);
    }
    app_wft_protocol.wft_cmd_1_reply.run_time_byte0 = 0x11;
    app_wft_protocol.wft_cmd_1_reply.run_time_byte1 = 0x12;
    app_wft_protocol.wft_cmd_1_reply.run_time_byte2 = 0x13;
    app_wft_protocol.wft_cmd_1_reply.run_time_byte3 = 0x14;
    app_wft_protocol.f_wft_prl_tx(0x01, 5, &app_wft_protocol.wft_cmd_1_reply);
}
MSH_CMD_EXPORT(prl, prl sample);

////////////////////////////////////////////////////////////////////////////
