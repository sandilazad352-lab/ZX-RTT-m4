/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-13     RiceChen     the first version
 */

#include <rtthread.h>
#include <rtdevice.h>

#include <string.h>

#define DBG_TAG "ft5x06"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

#include "ft5x06.h"

static struct rt_i2c_client ft5x06_client;

static rt_err_t __ft5x06_read_reg(struct rt_i2c_client *dev, rt_uint8_t *reg, rt_uint8_t *data, rt_uint8_t len)
{
    struct rt_i2c_msg msgs[2];

    msgs[0].addr = dev->client_addr;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf = reg;
    msgs[0].len = 1;

    msgs[1].addr = dev->client_addr;
    msgs[1].flags = RT_I2C_RD;
    msgs[1].buf = data;
    msgs[1].len = len;

    if (rt_i2c_transfer(dev->bus, msgs, 2) == 2) {
        return RT_EOK;
    } else {
        return -RT_ERROR;
    }
}

static rt_err_t __ft5x06_write_reg(struct rt_i2c_client *dev, rt_uint8_t *data,
                                rt_uint8_t len)
{
    struct rt_i2c_msg msgs;

    msgs.addr = dev->client_addr;
    msgs.flags = RT_I2C_WR;
    msgs.buf = data;
    msgs.len = len;

    if (rt_i2c_transfer(dev->bus, &msgs, 1) == 1) {
        return RT_EOK;
    } else {
        return -RT_ERROR;
    }
}

static struct rt_touch_data *read_data;

static void __ft5x06_touch_up(void *buf, int8_t id, int16_t x, int16_t y, int16_t w)
{
    read_data = (struct rt_touch_data *)buf;

    read_data[id].event = RT_TOUCH_EVENT_UP;

    read_data[id].timestamp = rt_touch_get_ts();
    read_data[id].width = w;
    read_data[id].x_coordinate = x;
    read_data[id].y_coordinate = y;
    read_data[id].track_id = id;
}

static void __ft5x06_touch_down(void *buf, int8_t id, int16_t x, int16_t y,
                             int16_t w)
{
    read_data = (struct rt_touch_data *)buf;

    read_data[id].event = RT_TOUCH_EVENT_DOWN;
    read_data[id].timestamp = rt_touch_get_ts();
    read_data[id].width = w;
    read_data[id].x_coordinate = x;
    read_data[id].y_coordinate = y;
    read_data[id].track_id = id;
}

static int __init_ft5x06(struct rt_i2c_client *dev)
{
    uint8_t cmd[8];
    // Enter Command mode
    cmd[0] = FT5X0X_REG_THGROUP;
    cmd[1] = 0x16;

    // Init default values. (From NHD-3.5-320240MF-ATXL-CTP-1 datasheet)
    // Valid touching detect threshold
    __ft5x06_write_reg(dev, cmd, 2);

    cmd[0] = FT5X0X_REG_THPEAK;
    cmd[1] = 0x3C;
    // valid touching peak detect threshold
    __ft5x06_write_reg(dev, cmd, 0x3C);

    cmd[0] = FT5X0X_REG_THCAL;
    cmd[1] = 0xE9;
    // Touch focus threshold
    __ft5x06_write_reg(dev, cmd, 0xE9);

    cmd[0] = FT5X0X_REG_THWATER;
    cmd[1] = 0x01;
    // threshold when there is surface water
    __ft5x06_write_reg(dev, cmd, 0x01);

    cmd[0] = FT5X0X_REG_THTEMP;
    cmd[1] = 0x01;
    // threshold of temperature compensation
    __ft5x06_write_reg(dev, cmd, 0x01);

    cmd[0] = FT5X0X_REG_THDIFF;
    cmd[1] = 0xA0;
    // Touch difference threshold
    __ft5x06_write_reg(dev, cmd, 0xA0);

    cmd[0] = FT5X0X_REG_THDIFF;
    cmd[1] = 0x0A;
    // Delay to enter 'Monitor' status (s)
    __ft5x06_write_reg(dev, cmd, 2);

    cmd[0] = FT5X0X_REG_THDIFF;
    cmd[1] = 0x06;
    // Period of 'Active' status (ms)
    __ft5x06_write_reg(dev, cmd, 2);

    cmd[0] = FT5X0X_REG_PERIODMONITOR;
    cmd[1] = 0x28;
    // Timer to enter 'idle' when in 'Monitor' (ms)
    __ft5x06_write_reg(dev, cmd, 2);

    return 1;
}

static rt_size_t ft5x06_read_point(struct rt_touch_device *touch, void *buf,
                                  rt_size_t read_num)
{
    uint8_t point_num;
    uint8_t data[4] = {0};
    uint16_t input_x = 0, input_y = 0;
    rt_uint8_t reg[1];

    reg[0] = (rt_uint8_t)(FT5x06_TOUCH_POINTS);

    __ft5x06_read_reg(&ft5x06_client, reg, &point_num, 1);
    point_num &= 0x07;

    if (point_num > 0 && point_num <= TOUCH_MAX_POINT_NUMBER) {
        uint16_t x[TOUCH_MAX_POINT_NUMBER];
        uint16_t y[TOUCH_MAX_POINT_NUMBER];

        for (size_t i = 0; i < point_num; i++) {
            reg[0] = (rt_uint8_t)(FT5x06_TOUCH1_XH + i * 6);
            __ft5x06_read_reg(&ft5x06_client, reg, data, 2);
            x[i] = 0x0fff & ((uint16_t)(data[0]) << 8 | data[1]);
            input_x = x[i];
            reg[0] = (rt_uint8_t)(FT5x06_TOUCH1_YH + i * 6);
            __ft5x06_read_reg(&ft5x06_client, reg, data, 2);
            y[i] = ((uint16_t)(data[0]) << 8 | data[1]);
            input_y = y[i];
        }

        __ft5x06_touch_down(buf, 0, input_x, input_y, 0);
    } else {
        __ft5x06_touch_up(buf, 0, input_x, input_y, 0);
    }

    return 1;
}

static rt_err_t __ft5x06_get_info(struct rt_i2c_client *dev,
                               struct rt_touch_info *info)
{
    info->range_x = 480;
    info->range_y = 480;
    info->point_num = 1;

    return RT_EOK;
}

static rt_err_t ft5x06_control(struct rt_touch_device *touch, int cmd, void *arg)
{
    if (cmd == RT_TOUCH_CTRL_GET_INFO) {
        return __ft5x06_get_info(&ft5x06_client, arg);
    }

    return -RT_ERROR;
}

static struct rt_touch_ops ft5x06_touch_ops = {
    .touch_readpoint = ft5x06_read_point,
    .touch_control = ft5x06_control,
};

static rt_uint8_t ft5x06_read_fw_ver(struct rt_i2c_client *dev)
{
    rt_uint8_t reg[1];
    rt_uint8_t ver;

    reg[0] = (rt_uint8_t)(FT5X0X_REG_FIRMID);

    __ft5x06_read_reg(dev, reg, &ver, 1);

    return (ver);
}

static int rt_hw_ft5x06_init(const char *name, struct rt_touch_config *cfg)
{
    struct rt_touch_device *touch_device = RT_NULL;

    touch_device =
        (struct rt_touch_device *)rt_malloc(sizeof(struct rt_touch_device));
    if (touch_device == RT_NULL) {
        LOG_E("touch device malloc fail");
        return -RT_ERROR;
    }
    rt_memset((void *)touch_device, 0, sizeof(struct rt_touch_device));

    /* hw init*/
    // rst output 0
    rt_pin_mode(*(rt_uint8_t *)cfg->user_data, PIN_MODE_OUTPUT);
    rt_pin_write(*(rt_uint8_t *)cfg->user_data, PIN_LOW);
    rt_thread_delay(10);

    // irq output 0
    rt_pin_mode(cfg->irq_pin.pin, PIN_MODE_OUTPUT);
    rt_pin_write(cfg->irq_pin.pin, PIN_LOW);

    rt_thread_delay(2);
    // rst output 1
    rt_pin_mode(*(rt_uint8_t *)cfg->user_data, PIN_MODE_OUTPUT);
    rt_pin_write(*(rt_uint8_t *)cfg->user_data, PIN_HIGH);

    rt_thread_delay(5);
    // rst input
    rt_pin_mode(*(rt_uint8_t *)cfg->user_data, PIN_MODE_INPUT);

    //irq output 0
    rt_pin_mode(cfg->irq_pin.pin, PIN_MODE_OUTPUT);
    rt_pin_write(cfg->irq_pin.pin, PIN_LOW);

    rt_thread_delay(50);

    rt_pin_mode(cfg->irq_pin.pin, PIN_MODE_INPUT_PULLUP);

    ft5x06_client.bus =
        (struct rt_i2c_bus_device *)rt_device_find(cfg->dev_name);

    if (ft5x06_client.bus == RT_NULL) {
        LOG_E("Can't find %s device", cfg->dev_name);
        return -RT_ERROR;
    }

    if (rt_device_open((rt_device_t)ft5x06_client.bus, RT_DEVICE_FLAG_RDWR) !=
        RT_EOK) {
        LOG_E("open %s device failed", cfg->dev_name);
        return -RT_ERROR;
    }

    ft5x06_client.client_addr = FT5X06_ADDRESS;

    /* register touch device */
    touch_device->info.type = RT_TOUCH_TYPE_CAPACITANCE;
    touch_device->info.vendor = RT_TOUCH_VENDOR_GT;
    rt_memcpy(&touch_device->config, cfg, sizeof(struct rt_touch_config));
    touch_device->ops = &ft5x06_touch_ops;

    if (RT_EOK != rt_hw_touch_register(touch_device, name, RT_DEVICE_FLAG_INT_RX, RT_NULL)) {
        LOG_E("touch device ft5x06 init failed !!!");
        return -RT_ERROR;
    }

    LOG_I("FT5x06 frameware version [%x]", ft5x06_read_fw_ver(&ft5x06_client));

    __init_ft5x06(&ft5x06_client);
    LOG_I("touch device ft5x06 init success");
    return RT_EOK;
}

static int rt_ft5x06_gpio_cfg()
{
    unsigned int g, p;
    long pin;

    // RST
    pin = drv_pin_get(FT5X06_RST_PIN);
    g = GPIO_GROUP(pin);
    p = GPIO_GROUP_PIN(pin);
    hal_gpio_direction_input(g, p);

    // INT
    pin = drv_pin_get(FT5X06_INT_PIN);
    g = GPIO_GROUP(pin);
    p = GPIO_GROUP_PIN(pin);
    hal_gpio_direction_input(g, p);
    hal_gpio_set_irq_mode(g, p, 0);

    return 0;
}

static int rt_hw_ft5x06_port(void)
{
    struct rt_touch_config cfg;
    rt_uint8_t rst_pin;

    rt_ft5x06_gpio_cfg();

    rst_pin = drv_pin_get(FT5X06_RST_PIN);
    cfg.dev_name = FT5X06_I2C_CHAN;
    cfg.irq_pin.pin = drv_pin_get(FT5X06_INT_PIN);
    cfg.irq_pin.mode = PIN_MODE_INPUT_PULLUP;
    cfg.user_data = &rst_pin;

    rt_hw_ft5x06_init("ft5x06", &cfg);

    return 0;
}

INIT_DEVICE_EXPORT(rt_hw_ft5x06_port);
