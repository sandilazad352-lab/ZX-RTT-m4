# GPIO Driver Documentation

## Introduction

The GPIO (General Purpose Input/Output) driver offers a suite of functions to control GPIO pins on microcontrollers. These capabilities include setting pin modes, reading and writing pin levels, configuring pull-up/pull-down resistors, setting drive strength, and handling GPIO interrupts.

## API Reference

### GPIO Modes

- **GPIO_MODE_DISABLE**: Disables input/output.
- **GPIO_MODE_INPUT**: Input-only mode.
- **GPIO_MODE_OUTPUT**: Output-only mode.
- **GPIO_MODE_INPUT_OUTPUT**: Input and output mode.

### GPIO Pull-up/Pull-down Modes

- **GPIO_PULLUP**: Enables internal pull-up resistor.
- **GPIO_PULLDOWN**: Enables internal pull-down resistor.
- **GPIO_FLOATING**: No pull-up or pull-down resistor.

### GPIO Drive Strength Options

- **GPIO_DRV_180_OHM** to **GPIO_DRV_23_OHM**: Specifies drive strength from 180 ohms to 23 ohms.

### GPIO Interrupt Modes

- **GPIO_IRQ_MODE_FALLING_EDGE**: Interrupt on falling edge.
- **GPIO_IRQ_MODE_RISING_EDGE**: Interrupt on rising edge.
- **GPIO_IRQ_MODE_LOW_LEVEL**: Interrupt on low level.
- **GPIO_IRQ_MODE_HIGH_LEVEL**: Interrupt on high level.
- **GPIO_IRQ_MODE_BOTH_EDGE**: Interrupt on both edges.

### Functions

- `int gpio_set_level(pin_name_t pin, uint32_t level)`: Sets the output level of a GPIO pin.
- `int gpio_toggle_level(pin_name_t pin)`: Toggles the output level of a GPIO pin.
- `int gpio_get_level(pin_name_t pin)`: Reads the current level of a GPIO pin.
- `int gpio_set_direction(pin_name_t pin, gpio_mode_t mode)`: Sets the direction of a GPIO pin.
- `int gpio_set_pull_mode(pin_name_t pin, gpio_pull_mode_t pull)`: Sets the pull-up/pull-down mode of a GPIO pin.
- `int gpio_set_drive_strength(pin_name_t pin, gpio_drv_strength_t strength)`: Sets the drive strength of a GPIO pin.
- `int gpio_irq_enable(pin_name_t pin, uint8_t enable)`: Enables or disables an interrupt for a GPIO pin.
- `int gpio_irq_attach(pin_name_t pin, gpio_irq_mode_t mode, gpio_irq_handler_t handler, void* arg)`: Attaches an interrupt handler to a GPIO pin.

## Usage

Include `gpio.h` in your project and ensure your build system includes paths to the header and source files.

## Example Usage

### Basic GPIO Output

```c
#include "gpio.h"

int main() {
    // Initialize GPIO pin for GPIO functionality
    gpio_set_func_as_gpio(PE0);
    
    // Set GPIO pin direction to output
    gpio_set_direction(PE0, GPIO_MODE_OUTPUT);
    
    // Set GPIO pin to high level
    gpio_set_level(PE0, 1);
    
    return 0;
}
```

### Reading GPIO Input

```c
#include "gpio.h"

int main() {
    // Initialize GPIO pin for GPIO functionality
    gpio_set_func_as_gpio(PE0);
    
    // Set GPIO pin direction to input
    gpio_set_direction(PE0, GPIO_MODE_INPUT);
    
    // Read GPIO pin level
    int level = gpio_get_level(PE0);
    
    return 0;
}
```

### Interrupt Handling

```c
#include "gpio.h"

// Interrupt handler function
void gpio_irq_handler(void *arg) {
    // Handle the interrupt
}

int main() {
    // Initialize GPIO pin for GPIO functionality
    gpio_set_func_as_gpio(PE0);
    
    // Enable interrupt for GPIO pin
    gpio_irq_enable(PE0, 1);

    // Set GPIO pin direction to input
    gpio_set_direction(PE0, GPIO_MODE_INPUT);
    
    // Attach interrupt handler
    gpio_irq_attach(PE0, GPIO_IRQ_MODE_RISING_EDGE, gpio_irq_handler, NULL);
    
    return 0;
}
```