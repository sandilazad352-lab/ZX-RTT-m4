

#ifndef __APP_WFT_PROTOCOL_H_
#define __APP_WFT_PROTOCOL_H_
#include <rtthread.h>

#define CMD_HEART_BEAT 0x01
#define CMD_CALL_MSG 0x02
#define CMD_NA_MSG0 0x03
#define CMD_WEATHER 0x04
extern unsigned char HEAD[2];
extern unsigned char TAIL[2];

////////////////////////////////////////////

////////////////////////////////////////////
// 心跳数据交互（Type：0x01）每50ms发送一次
typedef struct
{
    unsigned char total_mileage_byte1;              //  总里程Byte1
    unsigned char total_mileage_byte2;              //  总里程Byte2
    unsigned char total_mileage_byte3 : 7;          //  总里程Byte3
    unsigned char total_mileage_display_status : 1; //  总里程显示状态          【0:显示关；1:显示开】
    unsigned char single_mileage_byte1;             //  单次里程Byte1
    unsigned char single_mileage_byte2;             //  单次里程Byte2
    unsigned char single_mileage_byte3 : 7;         //  单次里程Byte3
    unsigned char single_trip_display_status : 1;   //  单次程显示状态          【0:显示关；1:显示开】

    unsigned char battery_a_power : 7;           //  电池A电量0-100%
    unsigned char battery_a_charging_status : 1; //  电池A充电状态           【0：未充电；1：充电中】

    unsigned char battery_a_voltage_byte1;       //  电池A电压Byte1          【单位100mv】
    unsigned char battery_a_voltage_byte2 : 6;   //  电池A电压Byte2          【单位100mv】
    unsigned char battery_a_power_display : 1;   //  电池A电量显示           【0:显示关；1:显示开】
    unsigned char battery_a_voltage_display : 1; //  电池A电压显示           【0:显示关；1:显示开】

    unsigned char battery_b_power : 7;           //  电池B电量0-100%
    unsigned char battery_b_charging_status : 1; //  电池B充电状态           【0：未充电；1：充电中】

    unsigned char battery_b_voltage_byte1;       //  电池B电压Byte1          【单位100mv】
    unsigned char battery_b_voltage_byte2 : 6;   //  电池B电压Byte2          【单位100mv】
    unsigned char battery_b_power_display : 1;   //  电池B电量显示           【0:显示关；1:显示开】
    unsigned char battery_b_voltage_display : 1; //  电池B电压显示           【0:显示关；1:显示开】

    unsigned char run_color : 2;                 // 旋转颜色
    unsigned char arc_speed : 2;                 //  旋转速度
    unsigned char kinetic_energy_recovery : 1;   //  动能回收               【0:显示关；1:显示开】
    unsigned char eco_energy_saving_display : 1; //  ECO节能显示            【0:显示关；1:显示开】
    unsigned char day_and_night_mode_on : 1;     //  昼夜模式开启           【 1:白天  0：黑夜】
    unsigned char screencast_prompt_is_on : 1;   //  投屏提示开启            【注意：投屏介绍开始与关闭，只有该位发生变化后才会生效，即边沿触发。如果一直是0或者一直是1不会生效】

    unsigned char time_hours : 5;      //  时间小时
    unsigned char time_display : 1;    //  时间显示                【0:显示关；1:显示开】
    unsigned char oil_pot_voltage : 1; // 油壶电压
    unsigned char reserved_5 : 1;      //  预留

    unsigned char time_minutes : 6; //  时间分钟
    unsigned char navigate : 2;     //  导航

    unsigned char gear : 3;        //  档位                    【0:P档  1:前进档  2:倒车档】
    unsigned char three_speed : 3; //  三速                    【0:无档位D   1:低速D1  2:中速D2  3:高速D3  4:运动S】
    unsigned char charge : 2;      //  充电                    【0:空闲  1:充电中  2:充电完成】

    unsigned char speed_display : 1;      //  速度显示                【0:显示关；1:显示开】
    unsigned char side_brace : 1;         //  侧撑                    【0:显示关；1:显示开】
    unsigned char high_current_fault : 1; //  大电流故障              【0:显示关；1:显示开】
    unsigned char undervoltage_fault : 1; //  欠压故障                【0:显示关；1:显示开】
    unsigned char brake_failure : 1;      //  刹车故障                【0:显示关；1:显示开】
    unsigned char trolley_failure : 1;    //  转把故障                【0:显示关；1:显示开】
    unsigned char ecu_failure : 1;        //  ECU故障                 【0:显示关；1:显示开】
    unsigned char hall_failure : 1;       //  霍尔故障                【0:显示关；1:显示开】

    unsigned char headlight : 1;         //  大灯                    【0:显示关；1:显示开】
    unsigned char left_turn_signal : 1;  //  左转灯                  【0:显示关；1:显示开】
    unsigned char right_turn_signal : 1; //  右转灯                  【0:显示关；1:显示开】
    unsigned char position_light : 1;    //  示廓灯                  【0:显示关；1:显示开】
    unsigned char prepare : 1;           //  准备                    【0:显示关；1:显示开】
    unsigned char bluetooth : 1;         //  蓝牙                    【0:显示关；1:显示开】
    unsigned char brake : 1;             //  刹车                    【0:显示关；1:显示开】
    unsigned char cruise : 1;            //  巡航                    【0:显示关；1:显示开】

    unsigned char speed; //  速度

    unsigned char reserved_byte_0;
    unsigned char reserved_byte_1;
    unsigned char reserved_byte_2;
    unsigned char reserved_byte_3;
    unsigned char reserved_byte_4;
} S_WFT_CMD_1;

typedef struct
{
    unsigned char pakg_now : 4;
    unsigned char pakg_sum : 4;
} S_WFT_PAKG_MSG;

// 电话信息（Type：0x02）
typedef struct
{
    unsigned char pakg_now : 4;
    unsigned char pakg_sum : 4;
    unsigned char name_len;
    unsigned char call_len;
} S_WFT_CMD_2;

// 导航信息（Type：0x03）

typedef struct
{
    S_WFT_PAKG_MSG wft_pakg_msg;
    unsigned char position_base;                  // 基础导航方向代码
    unsigned char position_nomal0;                // 基础导航方向代码
    unsigned char position_nomal1;                // 基础导航方向代码
    unsigned char mileage_unit;                   // 里程单位
    unsigned char current_dir_remaining_mileage0; // 当前方向剩余里程byte1
    unsigned char current_dir_remaining_mileage1; // 当前方向剩余里程byte2
    unsigned char current_dir_remaining_mileage2; // 当前方向剩余里程byte3
    unsigned char current_dir_remaining_mileage3; // 当前方向剩余里程byte4
    unsigned char total_remaining_mileage0;       // 总剩余里程byte1
    unsigned char total_remaining_mileage1;       // 总剩余里程byte2
    unsigned char total_remaining_mileage2;       // 总剩余里程byte3
    unsigned char total_remaining_mileage3;       // 总剩余里程byte4
    unsigned char estimated_time0;                // 预计花费时间(分钟)byte1
    unsigned char estimated_time1;                // 预计花费时间(分钟)byte2
    unsigned char next_intersection_name_length;  // 下个路口名称长度
} S_WFT_CMD_3;
// 天气信息（Type：0x04）
typedef struct
{

    unsigned char current_temperature; //  当前温度
    unsigned char maximum_temperature; //  最高温度
    unsigned char lowest_temperature;  //  最低温度
    unsigned char weather_code;        //  天气代码
    unsigned char wind_speed;          //  风速
    unsigned char relative_humidity;   //  相对湿度
    unsigned char visibility;          //  能见度
    unsigned char uv_intensity;        //  紫外线强度
    unsigned char rainfall_byte1;      //  雨量 byte1
    unsigned char rainfall_byte2;      //  雨量 byte2
    unsigned char reserved0;           //  预留
    unsigned char reserved1;           //  预留
    unsigned char reserved2;           //  预留
    unsigned char reserved3;           //  预留
    unsigned char reserved4;           //  预留
} S_WFT_CMD_4_F;

typedef struct
{
    S_WFT_PAKG_MSG wft_pakg_msg;
    unsigned char address_length;
    S_WFT_CMD_4_F wft_cmd_4_f;
} S_WFT_CMD_4;

typedef struct
{
    unsigned char run_time_byte0;
    unsigned char run_time_byte1;
    unsigned char run_time_byte2;
    unsigned char run_time_byte3;
    unsigned char reserved_0 : 1;
    unsigned char reserved_1 : 1;
    unsigned char reserved_2 : 1;
    unsigned char reserved_3 : 1;
    unsigned char reserved_4 : 1;
    unsigned char reserved_5 : 1;
    unsigned char Screen_tatus : 1;
    unsigned char day_ight_state : 1;
} S_WFT_CMD_1_REPLY;

//  长数据包回复
typedef struct
{
    unsigned char Data_reception_status; // 数据接收状态	【0:发送下一包(只有一包表示成功)；1:数据错误，请求重发】
    unsigned char Package_number;        // 包号	【接收到的数据包编号】
} S_WFT_CMD_LEN_REPLY;

typedef struct
{
    S_WFT_CMD_1 wft_cmd_1; // 心跳数据交互（Type：0x01）每50ms发送一次
    S_WFT_CMD_1 wft_cmd_1_temp;
    S_WFT_CMD_1_REPLY wft_cmd_1_reply;

    S_WFT_CMD_2 wft_cmd_2; // 电话信息（Type：0x02）
    S_WFT_CMD_3 wft_cmd_3; // 导航信息（Type：0x03）
    S_WFT_CMD_4 wft_cmd_4; // 天气信息（Type：0x04）
    
    S_WFT_CMD_LEN_REPLY wft_cmd_len_reply;
    void (*f_wft_prl_tx)(uint8_t cmd, uint8_t Data_Lenth, uint8_t *code_data_Buff);
    void (*f_wft_analysis)(uint8_t *Rx_Data_Buff);
    void (*f_init)(void);
} S_APP_WFT_PROTOCOL;
extern S_APP_WFT_PROTOCOL app_wft_protocol;
/////////////////////////////////
void p_wft_analysis_rx(uint8_t *Rx_Data_Buff);
void p_wft_protocol_tx(uint8_t cmd, uint8_t Data_Lenth, uint8_t *code_data_Buff);
void p_wft_init(void);
#endif //__APP_WFT_PROTOCOL_H_
