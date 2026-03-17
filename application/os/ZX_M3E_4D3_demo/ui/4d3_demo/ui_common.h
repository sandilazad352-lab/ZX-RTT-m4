#ifndef _UI_COMMON_H_
#define _UI_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lvgl.h"
#include "img_path.h"
/**********************
 *      TYPEDEFS
 **********************/

#define __DEBUG    //日志模块总开关，注释将关闭全局日志输出

#ifdef __DEBUG
    #define APP_DEBUG(format, ...) printf ("\033[41;33m"format"\033[0m\n", ##__VA_ARGS__)
#else
    #define APP_DEBUG(format, ...)
#endif


// 事件类型
typedef enum
{
    E_KEY_EVENT = 0,
    E_EVENT_ID_MAX,
} Event_ID_t;

//按键ID
typedef enum
{
    E_K1 = 0,   // 循环切换
    E_K2,       // 强度循环/往左
    E_K3,       // 时间循环/往右
    E_K4,       // 开机/启动/停止
} BTN_ID_t;

//按键事件类型
typedef enum
{
    E_EBUTTON_PRESS_DOWN = 0,
    E_BUTTON_PRESS_UP,
    E_BUTTON_PRESS_REPEAT,
    E_BUTTON_SINGLE_CLICK,
    E_BUTTON_DOUBLE_CLICK,
    E_BUTTON_LONG_PRESS_START,
    E_BUTTON_LONG_PRESS_HOLD,
    E_BUTTON_EVENT_MAX,
    E_BUTTON_NONE_PRESS
} BTN_STATUS_t;

typedef struct {
    Event_ID_t eEventID;    // 事件类型

    //按键事件参数
    uint32_t lDataArray[2]; // lDataArray[0]-按键编号 lDataArray[1]-按键事件类型(长短按)
} Event_Data_t, *p_Event_Data_t;

//屏幕结构体
typedef struct _SCR_FUNC_TYPE
{
    void (*load_func)(void *args);
    void (*refr_func)(Event_Data_t *args);  // NULL - 定时类显示刷新， 数值波动较大的, 不为NULL - 实时类显示刷新
    void (*quit_func)(void *args);
} SCR_FUNC_TYPE, *p_SCR_FUNC_TYPE;

//屏幕ID
typedef enum {
    E_HOME_SCR = 0,
    E_TIM_SCR,

    E_TOP_LAYER,
    E_MAX_SCR,
} E_SCREEN_ID;



typedef struct _SCR_LIST_TYPE{
    lv_obj_t *scr;              // 当前屏幕
    SCR_FUNC_TYPE *src_ops;     // 屏幕对象 函数接口
} SCR_LIST_TYPE, *p_SCR_LIST_TYPE;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

void scr_add_user_data(lv_obj_t *scr, E_SCREEN_ID id, SCR_FUNC_TYPE *desc);
void scr_load_func(E_SCREEN_ID id, void *args);
void scr_quit_func(void *args);
void scr_refr_func(Event_Data_t *args);
E_SCREEN_ID scr_act_get(void);



//工作类型
typedef enum
{
    E_WORK_NONE = 0,
    E_WORK_AUTOMATIC,           //自动
    E_WORK_DISINFECT,           //消毒
    E_WORK_DRYING,              //烘干

    E_WORK_TIM_AUTO,            //预约(自动)
    E_WORK_TIM_DISINFECT,       //预约(消毒)

    E_WORK_MAX,
} WORK_STATUS_t;



/*********************
 *      DEFINES
 *********************/

extern void home_scr_create(void);
extern void tim_scr_create(void);
extern volatile WORK_STATUS_t E_cur_work_status;
extern void user_set_tim_hour(uint32_t tim_hour);
extern void user_set_tim_min(uint32_t tim_min);
extern void top_layer_create(void);

LV_FONT_DECLARE(MiSans_Regular_14);
LV_FONT_DECLARE(MiSans_Regular_16);
LV_FONT_DECLARE(MiSans_Regular_20);
LV_FONT_DECLARE(MiSans_Regular_22);
LV_FONT_DECLARE(MiSans_Regular_24);
LV_FONT_DECLARE(MiSans_Regular_32);

LV_IMG_DECLARE(Automatic);
LV_IMG_DECLARE(Automatic_s);
LV_IMG_DECLARE(Disinfect);
LV_IMG_DECLARE(Disinfect_s);
LV_IMG_DECLARE(return_img);
LV_IMG_DECLARE(Tim);
LV_IMG_DECLARE(Wifi);
LV_IMG_DECLARE(Drying);

/**********************
 *      MACROS
 **********************/


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*_UI_COMMON_H_*/
