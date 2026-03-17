#include <rtthread.h>
#include <rtdevice.h>

#include "zx_button.h"

uint8_t zx_button_gpio_read(uint8_t scan_start, uint8_t wait_press, void* hardware_data) {
    return rt_pin_read((rt_base_t)hardware_data);
}

btn_handle_t zx_button_create_gpio(uint8_t gpio_num, uint8_t mode, uint8_t active_level, void* user_data) {
    if (mode == 1) {
        rt_pin_mode((rt_base_t)gpio_num, PIN_MODE_INPUT_PULLUP);
    } else if (mode == 2) {
        rt_pin_mode((rt_base_t)gpio_num, PIN_MODE_INPUT_PULLDOWN);
    } else {
        rt_pin_mode((rt_base_t)gpio_num, PIN_MODE_INPUT);
    }

    return zx_button_create(zx_button_gpio_read, (void *)(uint32_t)gpio_num, active_level, user_data);
}
