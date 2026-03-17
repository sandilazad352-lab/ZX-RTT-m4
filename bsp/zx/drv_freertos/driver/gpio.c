#include "gpio.h"
#include "aic_io.h"
#include "aic_soc.h"
#include "aic_osal.h"

#define GROUP_FACTOR    0x100
#define PIN_CFG_REG     0x80
#define PIN_FACTOR      0x4

/* PIN_CFG register field */
#define PIN_DIR_IN_SHIFT    16
#define PIN_DIR_OUT_SHIFT   17

#define gen_reg(_g, _offset) (volatile void*)((_g * GROUP_FACTOR) + _offset + GPIO_BASE)
#define cfg_reg(_g, _p) gen_reg(_g, (_p * PIN_FACTOR) + PIN_CFG_REG)
#define pin_cfg_reg(pin) cfg_reg(GPIO_GROUP(pin), GPIO_GROUP_PIN(pin))
#define pin_cfg_reg_write(pin, offset, mask, value)                                                                                                                                \
    do {                                                                                                                                                                           \
        unsigned int reg = readl(pin_cfg_reg(pin));                                                                                                                                \
        reg &= ~(mask << offset);                                                                                                                                                  \
        reg |= (value & mask) << offset;                                                                                                                                           \
        writel(reg, pin_cfg_reg(pin));                                                                                                                                             \
    } while (0)

typedef struct _gpio_irq_node_t {
    uint8_t pin_mask;
    struct _gpio_irq_node_t* next;
} gpio_irq_node_t;

gpio_irq_node_t gpio_group_node[GPIO_GROUP_MAX];

static irqreturn_t drv_gpio_group_irqhandler(int irq, void* data);

static inline void hal_gpio_direction_input_en(pin_name_t pin) {
    pin_cfg_reg_write(pin, PIN_DIR_IN_SHIFT, 1, 1);
}

static inline void hal_gpio_direction_input_dis(pin_name_t pin) {
    pin_cfg_reg_write(pin, PIN_DIR_IN_SHIFT, 1, 0);
}

static inline void hal_gpio_direction_output_dis(pin_name_t pin) {
    pin_cfg_reg_write(pin, PIN_DIR_OUT_SHIFT, 1, 0);
}

static inline void hal_gpio_direction_output_en(pin_name_t pin) {
    pin_cfg_reg_write(pin, PIN_DIR_OUT_SHIFT, 1, 1);
}

int gpio_set_level(pin_name_t pin, uint32_t level) {
    unsigned int g = GPIO_GROUP(pin);
    unsigned int p = GPIO_GROUP_PIN(pin);
    if (level) {
        hal_gpio_set_output(g, p);
    } else {
        hal_gpio_clr_output(g, p);
    }
    return 0;
}

int gpio_toggle_level(pin_name_t pin) {
    unsigned int g = GPIO_GROUP(pin);
    unsigned int p = GPIO_GROUP_PIN(pin);
    hal_gpio_toggle_output(g, p);
    return 0;
}

int gpio_get_level(pin_name_t pin) {
    unsigned int g = GPIO_GROUP(pin);
    unsigned int p = GPIO_GROUP_PIN(pin);
    unsigned int value = 0;
    hal_gpio_get_value(g, p, &value);
    return value;
}

int gpio_set_direction(pin_name_t pin, gpio_mode_t mode) {
    if (mode & GPIO_MODE_DEF_INPUT) {
        hal_gpio_direction_input_en(pin);
    } else {
        hal_gpio_direction_input_dis(pin);
    }

    if (mode & GPIO_MODE_DEF_OUTPUT) {
        hal_gpio_direction_output_en(pin);
    } else {
        hal_gpio_direction_output_dis(pin);
    }

    if (mode == GPIO_MODE_DISABLE) {
        hal_gpio_direction_input_dis(pin);
        hal_gpio_direction_output_dis(pin);
    }

    return 0;
}

int gpio_set_pull_mode(pin_name_t pin, gpio_pull_mode_t pull) {
    unsigned int g = GPIO_GROUP(pin);
    unsigned int p = GPIO_GROUP_PIN(pin);

    if (pull == GPIO_PULLDOWN) {
        hal_gpio_set_bias_pull(g, p, PIN_PULL_DOWN);
    } else if (pull == GPIO_PULLUP) {
        hal_gpio_set_bias_pull(g, p, PIN_PULL_UP);
    } else if (pull == GPIO_FLOATING) {
        hal_gpio_set_bias_pull(g, p, PIN_PULL_DIS);
    }
    return 0;
}

int gpio_pullup_en(pin_name_t pin) {
    unsigned int g = GPIO_GROUP(pin);
    unsigned int p = GPIO_GROUP_PIN(pin);
    hal_gpio_set_bias_pull(g, p, PIN_PULL_UP);
    return 0;
}

int gpio_pulldown_en(pin_name_t pin) {
    unsigned int g = GPIO_GROUP(pin);
    unsigned int p = GPIO_GROUP_PIN(pin);
    hal_gpio_set_bias_pull(g, p, PIN_PULL_DOWN);
    return 0;
}

int gpio_pull_dis(pin_name_t pin) {
    unsigned int g = GPIO_GROUP(pin);
    unsigned int p = GPIO_GROUP_PIN(pin);
    hal_gpio_set_bias_pull(g, p, PIN_PULL_DIS);
    return 0;
}

int gpio_set_drive_strength(pin_name_t pin, gpio_drv_strength_t strength) {
    unsigned int g = GPIO_GROUP(pin);
    unsigned int p = GPIO_GROUP_PIN(pin);
    hal_gpio_set_drive_strength(g, p, strength);
    return 0;
}

int gpio_set_func(pin_name_t pin, unsigned int func) {
    unsigned int g = GPIO_GROUP(pin);
    unsigned int p = GPIO_GROUP_PIN(pin);
    hal_gpio_set_func(g, p, func);
    return 0;
}

int gpio_set_func_as_gpio(pin_name_t pin) {
    gpio_set_func(pin, 1);
    return 0;
}

int gpio_set_func_disable(pin_name_t pin) {
    gpio_set_func(pin, 0);
    return 0;
}

static int gpio_irq_dis(pin_name_t pin) {
    unsigned int g = GPIO_GROUP(pin);
    unsigned int p = GPIO_GROUP_PIN(pin);
    gpio_irq_node_t* node = &gpio_group_node[g];
    gpio_irq_node_t* prev = node;
    node = node->next;

    hal_gpio_disable_irq(g, p);
    while (node) {
        if (node->pin_mask == p) {
            prev->next = node->next;
            free(node);
            return 0;
        }
        prev = node;
        node = node->next;
    }
    return 0;
}

static int gpio_irq_en(pin_name_t pin) {
    static uint16_t pin_group_irq_en = 0;
    unsigned int g = GPIO_GROUP(pin);
    unsigned int p = GPIO_GROUP_PIN(pin);
    gpio_irq_node_t* node = &gpio_group_node[g];
    gpio_irq_node_t* new_node = NULL;

    // check if the pin is already in the list, found last node
    while (node->next) {
        if (node->next->pin_mask == p) {
            return 0;
        }
        node = node->next;
    }

    // add new node
    new_node = (gpio_irq_node_t*)malloc(sizeof(gpio_irq_node_t));
    if (!new_node) {
        return -1;
    }
    new_node->pin_mask = p;
    new_node->next = NULL;
    node->next = new_node;

    // enable group irq
    if (!(pin_group_irq_en & (1 << g))) {
        aicos_request_irq(GPIO_IRQn + g, drv_gpio_group_irqhandler, 0, "pin_group", NULL);
        aicos_irq_enable(GPIO_IRQn + g);
        pin_group_irq_en |= (1 << g);
    }

    // enable pin irq
    hal_gpio_enable_irq(g, p);

    return 0;
}

int gpio_irq_enable(pin_name_t pin, uint8_t enable) {
    return enable ? gpio_irq_en(pin) : gpio_irq_dis(pin);
}

int gpio_irq_attach(pin_name_t pin, gpio_irq_mode_t mode, gpio_irq_handler_t handler, void* arg) {
    unsigned int g = GPIO_GROUP(pin);
    unsigned int p = GPIO_GROUP_PIN(pin);
    hal_gpio_disable_irq(g, p);

    hal_gpio_set_irq_mode(g, p, mode);
    aicos_request_irq(AIC_GPIO_TO_IRQ(pin), (irq_handler_t)handler, 0, "pin", arg);
    return 0;
}

static irqreturn_t drv_gpio_group_irqhandler(int irq, void* data) {
    unsigned int g = irq - GPIO_IRQn;
    unsigned int stat = 0;

    hal_gpio_group_get_irq_stat(g, &stat);
    gpio_irq_node_t* node = (&gpio_group_node[g])->next;
    while (node) {
        if (stat & (1U << node->pin_mask)) {
            drv_irq_call_isr(AIC_GPIO_TO_IRQ(g * GPIO_GROUP_SIZE + node->pin_mask));
        }
        node = node->next;
    }

    hal_gpio_group_set_irq_stat(g, 0xFFFFFFFF);

    return IRQ_HANDLED;
}
