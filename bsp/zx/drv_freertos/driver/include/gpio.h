/**
 * @file gpio.h
 * @brief This file contains the definitions and function prototypes for the GPIO driver.
 */

#ifndef __ZX_GPIO_DRIVER_H__
#define __ZX_GPIO_DRIVER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdio.h"
#include "aic_hal_gpio.h"

/**
 * @brief Type definition for GPIO interrupt handler function pointer.
 * @param arg Argument passed to the interrupt handler.
 */
typedef void (*gpio_irq_handler_t)(void *arg);

#define GPIO_MODE_DEF_DISABLE         (0)
#define GPIO_MODE_DEF_INPUT           (1 << 0)
#define GPIO_MODE_DEF_OUTPUT          (1 << 1)

/**
 * @brief Enumerates GPIO modes.
 */
typedef enum {
    GPIO_MODE_DISABLE = GPIO_MODE_DEF_DISABLE,             /*!< Disabled input/output. */
    GPIO_MODE_INPUT = GPIO_MODE_DEF_INPUT,                 /*!< Input-only mode. */
    GPIO_MODE_OUTPUT = GPIO_MODE_DEF_OUTPUT,               /*!< Output-only mode. */
    GPIO_MODE_INPUT_OUTPUT = GPIO_MODE_DEF_INPUT | GPIO_MODE_DEF_OUTPUT, /*!< Input and output mode. */
} gpio_mode_t;

/**
 * @brief Enumerates GPIO pull modes.
 */
typedef enum {
    GPIO_PULLUP,               /*!< Enable internal pull-up resistor. */
    GPIO_PULLDOWN,             /*!< Enable internal pull-down resistor. */
    GPIO_FLOATING,             /*!< No pull-up or pull-down. */
} gpio_pull_mode_t;

/**
 * @brief Enumerates GPIO drive strength options.
 */
typedef enum {
    GPIO_DRV_180_OHM,          /*!< 180 ohm drive strength. */
    GPIO_DRV_90_OHM,           /*!< 90 ohm drive strength. */
    GPIO_DRV_60_OHM,           /*!< 60 ohm drive strength. */
    GPIO_DRV_45_OHM,           /*!< 45 ohm drive strength. */
    GPIO_DRV_36_OHM,           /*!< 36 ohm drive strength. */
    GPIO_DRV_30_OHM,           /*!< 30 ohm drive strength. */
    GPIO_DRV_26_OHM,           /*!< 26 ohm drive strength. */
    GPIO_DRV_23_OHM,           /*!< 23 ohm drive strength. */
} gpio_drv_strength_t;

/**
 * @brief Enumerates GPIO interrupt modes.
 */
typedef enum {
    GPIO_IRQ_MODE_FALLING_EDGE,    /*!< Interrupt on falling edge. */
    GPIO_IRQ_MODE_RISING_EDGE,     /*!< Interrupt on rising edge. */
    GPIO_IRQ_MODE_LOW_LEVEL,       /*!< Interrupt on low level. */
    GPIO_IRQ_MODE_HIGH_LEVEL,      /*!< Interrupt on high level. */
    GPIO_IRQ_MODE_BOTH_EDGE,       /*!< Interrupt on both edges. */
} gpio_irq_mode_t;

/**
 * @brief Sets the output level of a GPIO pin.
 * @param pin GPIO pin number.
 * @param level Output level (0 or 1).
 * @return 0 if successful, -1 on failure.
 */
int gpio_set_level(pin_name_t pin, uint32_t level);

/**
 * @brief Toggles the output level of a GPIO pin.
 * @param pin GPIO pin number.
 * @return 0 if successful, -1 on failure.
 */
int gpio_toggle_level(pin_name_t pin);

/**
 * @brief Reads the current level of a GPIO pin.
 * @param pin GPIO pin number.
 * @return Current level of the pin (0 or 1).
 */
int gpio_get_level(pin_name_t pin);

/**
 * @brief Sets the direction of a GPIO pin.
 * @param pin GPIO pin number.
 * @param mode GPIO mode to set.
 * @return 0 if successful, -1 on failure.
 */
int gpio_set_direction(pin_name_t pin, gpio_mode_t mode);

/**
 * @brief Sets the pull mode of a GPIO pin.
 * @param pin GPIO pin number.
 * @param pull Pull mode to set.
 * @return 0 if successful, -1 on failure.
 */
int gpio_set_pull_mode(pin_name_t pin, gpio_pull_mode_t pull);

/**
 * @brief Enables the pull-up resistor for a GPIO pin.
 * @param pin GPIO pin number.
 * @return 0 if successful, -1 on failure.
 */
int gpio_pullup_en(pin_name_t pin);

/**
 * @brief Enables the pull-down resistor for a GPIO pin.
 * @param pin GPIO pin number.
 * @return 0 if successful, -1 on failure.
 */
int gpio_pulldown_en(pin_name_t pin);

/**
 * @brief Disables the pull resistor for a GPIO pin.
 * @param pin GPIO pin number.
 * @return 0 ifsuccessful, -1 on failure.
 */
int gpio_pull_dis(pin_name_t pin);

/**
 * @brief Sets the drive strength of a GPIO pin.
 * @param pin GPIO pin number.
 * @param strength Drive strength to set.
 * @return 0 if successful, -1 on failure.
 */
int gpio_set_drive_strength(pin_name_t pin, gpio_drv_strength_t strength);

/**
 * @brief Configures the function of a GPIO pin.
 * @param pin GPIO pin number.
 * @param func Function to set for the pin.
 * @return 0 if successful, -1 on failure.
 */
int gpio_set_func(pin_name_t pin, unsigned int func);

/**
 * @brief Configures a GPIO pin's function to default GPIO.
 * @param pin GPIO pin number.
 * @return 0 if successful, -1 on failure.
 */
int gpio_set_func_as_gpio(pin_name_t pin);

/**
 * @brief Disables a specific function of a GPIO pin, setting it to a default state.
 * @param pin GPIO pin number.
 * @return 0 if successful, -1 on failure.
 */
int gpio_set_func_disable(pin_name_t pin);

/**
 * @brief Enables or disables interrupts for a GPIO pin.
 * @param pin GPIO pin number.
 * @param enable 1 to enable interrupts, 0 to disable.
 * @return 0 if successful, -1 on failure.
 */
int gpio_irq_enable(pin_name_t pin, uint8_t enable);

/**
 * @brief Attaches an interrupt handler to a GPIO pin for specified interrupt conditions.
 * @param pin GPIO pin number.
 * @param mode Interrupt mode to set.
 * @param handler Interrupt handler function to attach.
 * @param arg Argument passed to the interrupt handler.
 * @return 0 if successful, -1 on failure.
 */
int gpio_irq_attach(pin_name_t pin, gpio_irq_mode_t mode, gpio_irq_handler_t handler, void* arg);

#ifdef __cplusplus
}
#endif

#endif /* __GPIO_DRIVER_H__ */
