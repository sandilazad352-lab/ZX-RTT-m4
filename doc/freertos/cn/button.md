# ZX_BUTTON库使用文档

## 简介

ZX_BUTTON库为嵌入式系统提供了一套灵活的按钮处理机制，支持通过GPIO和ADC读取按钮状态，能够检测并区分多种按钮事件，如按下、释放、单击、双击和长按等。该库旨在简化物理按钮的事件处理流程，适用于需要对物理按钮进行多种交互操作的场景。

## 配置和API说明

### 初始化按钮库

在使用按钮库之前，首先需要初始化库，配置基本的参数。

```c
void zx_button_init(zx_button_config_t* config);
```

### 创建按钮实例

根据您的需求，可以创建基于GPIO或ADC的按钮实例。

- **GPIO按钮实例创建**

  ```c
  btn_handle_t zx_button_create_gpio(pin_name_t gpio_num, uint8_t active_level, void* user_data);
  ```

- **ADC按钮实例创建**

  ```c
  btn_handle_t zx_button_create_adc(uint32_t ch, uint16_t middle_volt_mv, uint16_t diff_volt_mv, void* user_data);
  ```

### 注册事件回调函数

为特定的按钮事件注册回调函数，当相应事件发生时，系统将调用这些函数。

```c
void zx_button_register_cb(btn_handle_t handle, press_event_t event, btn_callback_t cb);
```

### 更新按钮状态

在系统的主循环或定时任务中调用此函数，以便库能够更新按钮的状态并触发相应的事件。

```c
void zx_button_update(void);
```

## 示例用法

### 初始化按钮库并创建按钮实例

下面的示例展示了如何初始化按钮库、创建一个GPIO按钮实例，并为其注册回调函数。

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

> **_NOTE:_** 如果使用ADC button，请确保ADC引脚复用被正确配置
