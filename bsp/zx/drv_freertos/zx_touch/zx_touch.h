#pragma once

#include "stdint.h"
#include "aic_osal.h"
#include "zx_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TOUCH_MAX_POINT_NUMBER (1)

typedef enum {
    TOUCH_MIRROR_X = 0x40, /**< Mirror X-axis */
    TOUCH_MIRROR_Y = 0x20, /**< Mirror Y-axis */
    TOUCH_SWAP_XY  = 0x80, /**< Swap XY axis */
} touch_panel_dir_t;

typedef enum {
    TOUCH_EVT_RELEASE = 0x0,  /*!< Release event */
    TOUCH_EVT_PRESS   = 0x1,  /*!< Press event */
} touch_panel_event_t;

typedef struct {
    touch_panel_event_t event;   /*!< Event of touch */
    uint8_t point_num;           /*!< Touch point number */
    uint16_t curx[TOUCH_MAX_POINT_NUMBER];            /*!< Current x coordinate */
    uint16_t cury[TOUCH_MAX_POINT_NUMBER];            /*!< Current y coordinate */
} touch_panel_points_t;

typedef struct {
    int16_t scl_pin;
    int16_t sda_pin;
    uint8_t i2c_num;
    uint32_t i2c_freq;
    int16_t intr_pin;                               /*-1: disable*/ 
    int16_t rst_pin;                                /*-1: disable*/
    uint16_t width;                                /*!< touch panel width */
    uint16_t height;                               /*!< touch panel height */
    touch_panel_dir_t direction;                   /*!< Rotate direction */
    int8_t task_en;
    int8_t task_priority;
    int8_t task_core;
    uint16_t task_stack_size;
}  touch_panel_config_t;

typedef struct {
    /**
    * @brief Initial touch panel
    * @attention If you have been called function touch_panel_init() that will call this function automatically, and should not be called it again.
    *
    * @param config Pointer to a structure with touch config arguments.
    *
    * @return
    *     - ZX_OK Success
    *     - ZX_FAIL Fail
    */
    ZX_t (*init)(const touch_panel_config_t *config);

    /**
    * @brief Deinitial touch panel
    *
    * @return
    *     - ZX_OK Success
    *     - ZX_FAIL Fail
    */
    ZX_t (*deinit)(void);

    /**
    * @brief Get current touch information, see struct touch_panel_points_t
    *
    * @param point a pointer of touch_panel_points_t contained touch information.
    *
    * @return
    *     - ZX_OK Success
    *     - ZX_FAIL Fail
    */
    ZX_t (*read_point_data)(touch_panel_points_t *point);
} touch_panel_driver_t;

extern touch_panel_driver_t touch_ft5x06_driver;
extern touch_panel_driver_t touch_gt2863_driver;
extern touch_panel_driver_t touch_cst816t_driver;
extern touch_panel_driver_t touch_gt911_driver;
extern touch_panel_driver_t touch_chsc6540_driver;
extern touch_panel_driver_t touch_sp2010_driver;
extern touch_panel_driver_t touch_cst328_driver;

/**
 * Initializes the ZX touch panel driver.
 *
 * This function initializes the ZX touch panel driver with the provided touch panel driver
 * and panel configuration.
 *
 * @param touch_panel A pointer to the touch panel driver.
 * @param panel_config A pointer to the panel configuration.
 * @return The ZX_t value indicating the status of the initialization.
 */
ZX_t touch_init(touch_panel_driver_t* touch_panel, touch_panel_config_t* panel_config);

/**
 * @brief Reads touch points from the touch panel.
 *
 * This function reads touch points from the touch panel and stores them in the provided
 * `touch_panel_points_t` structure.
 *
 * @param point Pointer to the `touch_panel_points_t` structure where the touch points will be stored.
 * @return ZX_t The status code indicating the success or failure of the operation.
 */
ZX_t touch_read_points(touch_panel_points_t *point);

/**
 * @brief Retrieves the number of ticks since the last touch press event.
 *
 * This function returns the number of ticks that have elapsed since the last touch press event occurred.
 * Ticks are a unit of time measurement used by the system.
 *
 * @return The number of ticks since the last touch press event.
 */
uint32_t touch_get_last_press_ticks();

void touch_deinit();

#ifdef __cplusplus
}
#endif
