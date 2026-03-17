# ZX_BUTTON Library Documentation

## Introduction

The ZX_BUTTON library offers a flexible button handling mechanism for embedded systems. It supports reading button states through GPIO and ADC, detecting various button events like press, release, single click, double click, and long press. The library aims to simplify the process of handling physical button events, suitable for scenarios requiring multiple interactions with physical buttons.

## Configuration and API Description

### Initialize the Button Library

Before using the button library, initialize it with basic parameters.

```c
void zx_button_init(zx_button_config_t* config);
```

### Create Button Instance

Create a button instance based on GPIO or ADC according to your needs.

- **GPIO Button Instance**

  ```c
  btn_handle_t zx_button_create_gpio(pin_name_t gpio_num, uint8_t active_level, void* user_data);
  ```

- **ADC Button Instance**

  ```c
  btn_handle_t zx_button_create_adc(uint32_t ch, uint16_t middle_volt_mv, uint16_t diff_volt_mv, void* user_data);
  ```

### Register Event Callback Function

Register callback functions for specific button events. These functions are called when the events occur.

```c
void zx_button_register_cb(btn_handle_t handle, press_event_t event, btn_callback_t cb);
```

### Update Button State

Call this function in the main loop or a timed task to update button states and trigger events.

```c
void zx_button_update(void);
```

## Example Usage

### Initialize Button Library and Create Button Instance

This example shows how to initialize the button library, create a GPIO button instance, and register callback functions for it.

```c
#include "zx_button.h"

void btn_callback(btn_handle_t handle, void* user_data) {
    const char* press_event[] = {
        "PRESS_DOWN", "PRESS_UP", "PRESS_REPEAT", "SINGLE_CLICK", "DOUBLE_CLICK", "LONG_PRESS_START", 
        "LONG_PRESS_HOLD", "BUTTON_EVENT_MAX", "NONE_PRESS"
    };
    printf("BTN %s\r\n", press_event[zx_button_get_event(handle)]); 
}

void app_main(void)
{   
    zx_button_config_t config = ZX_BUTTON_DEFAULT_CONFIG;
    zx_button_init(&config);
    
    // USE button GPIO0  
    btn_handle_t* btn0 = zx_button_create_gpio(PA0, 0, NULL);
    // btn_handle_t* btn0 = zx_button_create_adc(0, 1630, 200, NULL);

    zx_button_register_cb(btn0, BUTTON_PRESS_DOWN,       btn_callback);
    zx_button_register_cb(btn0, BUTTON_PRESS_UP,         btn_callback);
    zx_button_register_cb(btn0, BUTTON_PRESS_REPEAT,     btn_callback);
    zx_button_register_cb(btn0, BUTTON_SINGLE_CLICK,     btn_callback);
    zx_button_register_cb(btn0, BUTTON_DOUBLE_CLICK,     btn_callback);
    zx_button_register_cb(btn0, BUTTON_LONG_PRESS_START, btn_callback);
    zx_button_register_cb(btn0, BUTTON_LONG_PRESS_HOLD,  btn_callback);
    zx_button_start(btn0);

    for (;;) {
        if (zx_button_wait_event(btn0, BUTTON_SINGLE_CLICK, 0)) {
            printf("___ SINGLE_CLICK ___\r\n");
        } else if (zx_button_wait_event(btn0, BUTTON_DOUBLE_CLICK, 0)) {
            printf("___ DOUBLE_CLICK ___\r\n");
        }
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}
```

> **_NOTE:_** If using an ADC button, ensure the ADC pin is correctly configured.
