#pragma once

#include "stdint.h"
#include "string.h"

typedef void *btn_handle_t;
typedef void (*btn_callback_t)(btn_handle_t handle, void* user_data);
typedef uint8_t (*btn_get_level_fun_t)(uint8_t scan_start, uint8_t wait_press, void* hardware_data);

typedef enum {
    BUTTON_PRESS_DOWN = 0,
    BUTTON_PRESS_UP,
    BUTTON_PRESS_REPEAT,
    BUTTON_SINGLE_CLICK,
    BUTTON_DOUBLE_CLICK,
    BUTTON_LONG_PRESS_START,
    BUTTON_LONG_PRESS_HOLD,
    BUTTON_EVENT_MAX,
    BUTTON_NONE_PRESS
} press_event_t;

typedef struct {
	uint8_t ticks_interval_ms;
	uint8_t debounce_ticks;     
	uint16_t long_ticks;
	uint16_t short_ticks;
	struct {
		uint8_t en: 1;
		uint8_t priority: 7;
		int8_t core;
	} update_task;
} zx_button_config_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 函数功能: 按键初始化，配置线程相关参数
 * config: 线程相关参数,长按时间，短按时间
 */
void zx_button_init(zx_button_config_t* config);

/**
 * 创建按键
 * get_level: 获取按键状态的函数指针
 * hardware_data: 硬件数据信息(例adc参考 btn_hw_info_t)
 * active_level: 按键按下时的电平状态 (adc参考 zx_button_adc_read 函数的返回值)
 * user_data
 */
btn_handle_t zx_button_create(btn_get_level_fun_t get_level, void* hardware_data, uint8_t active_level, void* user_data);

/**
 * 创建gpio按键
 * gpio_num: pin脚
 * mode: 模式配置
 * active_level: 按键按下时的电平状态 (adc参考 zx_button_adc_read 函数的返回值)
 * user_data
 */
btn_handle_t zx_button_create_gpio(uint8_t gpio_num, uint8_t mode, uint8_t active_level, void* user_data);

/**
 * 创建adc按键
 * adc_channel:adc 通道
 * middle_volt_mv: 参考电压(按键按下时的电压值)
 * diff_volt_mv: 电压浮动范围
 * active_level: 按键按下时的zx_button_adc_read的返回值 (adc参考 zx_button_adc_read 函数的返回值)
 * user_data
 */
btn_handle_t zx_button_create_adc(int adc_channel, uint16_t middle_volt_mv, uint16_t diff_volt_mv,  uint8_t active_level, void* user_data);

/**
 * 按键注册回调
 * handle: 按键句柄()
 * event: 事件类型
 * cb：回调函数
 */
void zx_button_register_cb(btn_handle_t handle, press_event_t event, btn_callback_t cb);

/**
 * 注销按键回调
 * handle: 按键句柄()
 * event: 事件类型
 */
void zx_button_unregister_cb(btn_handle_t handle,  press_event_t event);

/**
 * 获取按键事件
 * handle: 按键句柄()
 */
press_event_t zx_button_get_event(btn_handle_t handle);

/**
 * 获取按键重复次数
 * handle: 按键句柄()
 */
uint8_t zx_button_get_repeat(btn_handle_t btn_handle);

/**
 * 获取按键状态
 * handle: 按键句柄()
 */
uint8_t zx_button_get_level(btn_handle_t btn_handle);

/**
 * 加入状态机链表
 * handle: 按键句柄()
 */
int  zx_button_start(btn_handle_t handle);

/**
 * 移除状态机链表
 * handle: 按键句柄()
 */
void zx_button_stop(btn_handle_t handle);

/**
 * 检查状态机(检查按键状态)
 */
void zx_button_update(void);

#ifdef __cplusplus
}
#endif
