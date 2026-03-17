#include "touch_freertos.h"
#include "zx_touch.h"
#include "aic_hal_gpio.h"

#ifdef TOUCH_DRIVER_FT5X06
#define TOUCH_DRIVER  (&touch_ft5x06_driver)
#elif defined(TOUCH_DRIVER_GT2863)
#define TOUCH_DRIVER  (&touch_gt2863_driver)
#elif defined(TOUCH_DRIVER_CST816T)
#define TOUCH_DRIVER  (&touch_cst816t_driver)
#elif defined(TOUCH_DRIVER_GT911)
#define TOUCH_DRIVER  (&touch_gt911_driver)
#elif defined(TOUCH_DRIVER_CHSC6540)
#define TOUCH_DRIVER  (&touch_chsc6540_driver)
#elif defined(TOUCH_DRIVER_SP2010)
#define TOUCH_DRIVER  (&touch_sp2010_driver)
#elif defined(TOUCH_DRIVER_CST328)
#define TOUCH_DRIVER  (&touch_cst328_driver)
#endif

#if defined(TOUCH_PANEL_DIRECTION_0)
#define TOUCH_DIRECTION 0
#elif defined(TOUCH_PANEL_DIRECTION_90)
#define TOUCH_DIRECTION TOUCH_MIRROR_X ^ TOUCH_SWAP_X
#elif defined(TOUCH_PANEL_DIRECTION_180)
#define TOUCH_DIRECTION TOUCH_MIRROR_X ^ TOUCH_MIRROR_Y
#elif defined(TOUCH_PANEL_DIRECTION_270)
#define TOUCH_DIRECTION TOUCH_MIRROR_Y ^ TOUCH_SWAP_XY
#endif

void touch_freertos_init() {
    static uint8_t is_init = 0;

#ifndef TOUCH_DRIVER
    return ;
#endif

    if (is_init) {
        return ;
    }
    is_init = 1;

    touch_panel_config_t config;
    config.i2c_num = TOUCH_PANEL_I2C_NUM;
    config.i2c_freq = TOUCH_PANEL_I2C_FREQ;
    config.intr_pin = hal_gpio_name2pin(TOUCH_PANEL_INTR_PIN);
    config.rst_pin = hal_gpio_name2pin(TOUCH_PANEL_RST_PIN);
    config.width = TOUCH_PANEL_WIDTH;                                /*!< touch panel width */
    config.height = TOUCH_PANEL_HEIGHT; 
    config.direction = TOUCH_DIRECTION;                   
    config.task_en = 1;
    config.task_priority = 5;
    config.task_core = 0;
    config.task_stack_size = 3 * 1024;
    touch_init(TOUCH_DRIVER, &config);
}

void touch_freertos_read(uint8_t* press, uint16_t* x, uint16_t* y) {
    touch_panel_points_t points = {
        .point_num = 0,
        .event = TOUCH_EVT_RELEASE,
    };

    touch_read_points(&points);
    *press = points.event == TOUCH_EVT_PRESS;
    *x = points.curx[0];
    *y = points.cury[0];
}
