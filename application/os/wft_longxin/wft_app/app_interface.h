
#ifndef __APP_INTERFACE_H_
#define __APP_INTERFACE_H_
// #include <rtthread.h>
#include "ui.h"
/////////////////////////////////////////
#include "app_font.h"
#include "app_wft_protocol.h"
#define PNG_PATH "L:/rodata/lvgl_data/"

#define ENABLE_FLAG_HIDDEN(A, B) (B) ? lv_obj_clear_flag((A), LV_OBJ_FLAG_HIDDEN) : lv_obj_add_flag((A), LV_OBJ_FLAG_HIDDEN)

#define MAX_SPEED 100
#define UNIT_SPEED 25

extern unsigned char WEATHER_STRING[][20];

typedef struct
{
    uint8_t icon_Turn_L : 1;
    uint8_t icon_Turn_R : 1;
    uint8_t icon_Ready : 1;
    uint8_t icon_Cruise : 1;   // 定数巡航
    uint8_t icon_Position : 1; // 示廓灯
    uint8_t icon_Call : 1;     // 电话来电图标
    uint8_t icon_Msg : 1;      // 电话来电图标
    uint8_t navigate : 1;      // 导航
    uint8_t headlight : 1;     // 大灯 远光
    uint8_t speed_display : 1; // 速度显示
    uint8_t bluetooth : 1;     // 蓝牙

    uint8_t time_display : 1;              //  时间显示                【0:显示关；1:显示开】
    uint8_t trip_display_status : 1;       //  单次程显示状态          【0:显示关；1:显示开】
    uint8_t total_display_status : 1;      //  总里程显示状态          【0:显示关；1:显示开】
    uint8_t battery_a_power_display : 1;   //  电池A电量显示           【0:显示关；1:显示开】
    uint8_t battery_a_voltage_display : 1; //  电池A电压显示           【0:显示关；1:显示开】
    uint8_t battery_b_power_display : 1;   //  电池B电量显示           【0:显示关；1:显示开】
    uint8_t battery_b_voltage_display : 1; //  电池B电压显示           【0:显示关；1:显示开】

    uint8_t oil_pot_voltage : 1;           // 油壶电压
    uint8_t side_brace : 1;                //  侧撑                    【0:显示关；1:显示开】
    uint8_t high_current_fault : 1;        //  大电流故障              【0:显示关；1:显示开】
    uint8_t undervoltage_fault : 1;        //  欠压故障                【0:显示关；1:显示开】
    uint8_t brake_failure : 1;             //  刹车故障                【0:显示关；1:显示开】
    uint8_t trolley_failure : 1;           //  转把故障                【0:显示关；1:显示开】
    uint8_t ecu_failure : 1;               //  ECU故障                 【0:显示关；1:显示开】
    uint8_t hall_failure : 1;              //  霍尔故障                【0:显示关；1:显示开】
    uint8_t kinetic_energy_recovery : 1;   //  动能回收               【0:显示关；1:显示开】
    uint8_t eco_energy_saving_display : 1; //  ECO节能显示            【0:显示关；1:显示开】

    uint8_t day_and_night_mode_on : 1;   

    uint8_t position_nomal_base : 1; // 显示基础方向还是全向
    /*****      bool val     ******/
    uint8_t speed_val;
    uint8_t time_hours;   //  时间小时
    uint8_t time_minutes; //  时间分钟
    uint8_t gear;         // 档位     档位                    【0:P档  1:前进档  2:倒车档】
    uint8_t memory_gear;  // 记忆档位 三速                    【0:无档位D   1:低速D1  2:中速D2  3:高速D3  4:运动S】
    uint8_t run_color;    // 旋转颜色
    uint8_t arc_speed;    //  旋转速度
    uint8_t bt_percent_a;
    uint8_t bt_percent_b;
    uint16_t bt_voltage_a;
    uint16_t bt_voltage_b;

    uint8_t sta_evasive_bit;
    uint8_t msg_call_number[250];

    uint8_t position_base;                  // 基础导航方向代码
    uint16_t position_nomal;                // 基础导航方向代码
    uint8_t mileage_unit;                   // 里程单位
    uint32_t current_dir_Remaining_mileage; // 当前方向剩余里程
    uint32_t total_remaining_mileage;       // 总剩余里程
    uint16_t estimated_time;                // 预计花费时间(分钟)
    uint8_t navigation_description[300];    ///  导航描述
    uint8_t total_left_mileage[250];        /// 总剩余里程
    uint8_t time_left[250];                 ///  剩余时间

    uint8_t weather_addr[99];    ///  天气 地址
    uint8_t current_temperature; //  当前温度
    uint8_t maximum_temperature; //  最高温度
    uint8_t lowest_temperature;  //  最低温度
    uint8_t weather_code;        //  天气代码
    uint8_t wind_speed;          //  风速
    uint8_t relative_humidity;   //  相对湿度
    uint8_t visibility;          //  能见度
    uint8_t uv_intensity;        //  紫外线强度
    uint16_t rainfall;           //  雨量

    uint32_t odo_km;
    uint32_t trip_km;

} S_UI_DATA;

typedef struct
{
    S_UI_DATA ui_data;
    S_UI_DATA ui_data_temp; // 防止重复刷新
    S_APP_FONT *font;

    S_APP_WFT_PROTOCOL *wft_protocol;

    void (*f_init)(void);
} S_APP_API;
extern S_APP_API app_interface;

void ui_change_funtion(lv_timer_t *t);
int p_app_init(void);
#endif