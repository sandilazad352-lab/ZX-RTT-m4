#pragma once

#include "stdint.h"
#include "string.h"
#include <aic_osal.h>
#include "gpai.h"
#include "gpio.h"

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

#define ZX_BUTTON_DEFAULT_CONFIG {\
    .ticks_interval_ms = 10,\
    .debounce_ticks = 2,\
    .short_ticks = 200 / 10,\
    .long_ticks = 1000 / 10,\
    .update_task = {\
        .en = 1,\
        .core = 1,\
        .priority = 1,\
    }\
}

#ifdef __cplusplus
extern "C" {
#endif

void zx_button_init(zx_button_config_t* config);

btn_handle_t zx_button_create(btn_get_level_fun_t get_level, void* hardware_data, uint8_t active_level, void* user_data);

btn_handle_t zx_button_create_gpio(pin_name_t gpio_num, uint8_t active_level, void* user_data);

btn_handle_t zx_button_create_adc(uint32_t ch, uint16_t middle_volt_mv, uint16_t diff_volt_mv, void* user_data);

void zx_button_register_cb(btn_handle_t handle, press_event_t event, btn_callback_t cb);

void zx_button_unregister_cb(btn_handle_t handle,  press_event_t event);

press_event_t zx_button_get_event(btn_handle_t handle);

uint8_t zx_button_get_repeat(btn_handle_t btn_handle);

uint8_t zx_button_get_level(btn_handle_t btn_handle);

int  zx_button_start(btn_handle_t handle);

void zx_button_stop(btn_handle_t handle);

void zx_button_update(void);

void zx_button_reset_event(btn_handle_t btn_handle);

bool zx_button_wait_event(btn_handle_t btn_handle, press_event_t event, uint32_t ticks_ms);

#ifdef __cplusplus
}
#endif
