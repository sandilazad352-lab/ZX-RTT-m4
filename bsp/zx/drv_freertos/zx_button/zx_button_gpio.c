#include "zx_button.h"

uint8_t zx_button_gpio_read(uint8_t scan_start, uint8_t wait_press, void* hardware_data) {
    return gpio_get_level((uint32_t)hardware_data);
}

btn_handle_t zx_button_create_gpio(pin_name_t gpio_num, uint8_t active_level, void* user_data) {
    gpio_set_func_as_gpio(gpio_num);
    gpio_set_direction(gpio_num, GPIO_MODE_INPUT);
    if (active_level) {
        gpio_set_pull_mode(gpio_num, GPIO_PULLDOWN);
    } else {
        gpio_set_pull_mode(gpio_num, GPIO_PULLUP);
    }
    return zx_button_create(zx_button_gpio_read, (void *)(uint32_t)gpio_num, active_level, user_data);
}
