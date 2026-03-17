/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-13     RiceChen     the first version
 * 2023-04-30     Geo          modified for ZX
 */

#include <rtthread.h>
#include <rtdevice.h>

#include <string.h>

#define DBG_TAG "ilitek"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

#include "ilitek.h"

static struct rt_i2c_client ilitek_client;

#define ILITEK_MAX_TOUCH 1
#define REPORT_COUNT_ADDRESS                61
#define ILITEK_SUPPORT_MAX_POINT            40

#define __DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))

#define ILK_MAX_X   1280
#define ILK_MAX_Y   600

static rt_err_t __ilitek_read_regs(struct rt_i2c_client *dev, rt_uint8_t *reg,
                                rt_uint8_t *data, rt_uint8_t len)
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

static int16_t pre_x[ILITEK_MAX_TOUCH] = { -1 };
static int16_t pre_y[ILITEK_MAX_TOUCH] = { -1 };
static int16_t pre_w[ILITEK_MAX_TOUCH] = { -1 };
static rt_uint8_t s_tp_dowm[ILITEK_MAX_TOUCH];
static struct rt_touch_data *read_data;

static void ilitek_touch_up(void *buf, int8_t id)
{
    read_data = (struct rt_touch_data *)buf;

    if (s_tp_dowm[id] == 1) {
        s_tp_dowm[id] = 0;
        read_data[id].event = RT_TOUCH_EVENT_UP;
    } else {
        read_data[id].event = RT_TOUCH_EVENT_NONE;
    }

    read_data[id].timestamp = rt_touch_get_ts();
    read_data[id].width = pre_w[id];
    read_data[id].x_coordinate = pre_x[id];
    read_data[id].y_coordinate = pre_y[id];
    read_data[id].track_id = id;

    pre_x[id] = -1; /* last point is none */
    pre_y[id] = -1;
    pre_w[id] = -1;
}

static void ilitek_touch_down(void *buf, int8_t id, int16_t x, int16_t y,
                             int16_t w)
{
    read_data = (struct rt_touch_data *)buf;

    if (s_tp_dowm[id] == 1) {
        read_data[id].event = RT_TOUCH_EVENT_MOVE;
    } else {
        read_data[id].event = RT_TOUCH_EVENT_DOWN;
        s_tp_dowm[id] = 1;
    }

    read_data[id].timestamp = rt_touch_get_ts();
    read_data[id].width = w;
    read_data[id].x_coordinate = x;
    read_data[id].y_coordinate = y;
    read_data[id].track_id = id;

    pre_x[id] = x; /* save last point */
    pre_y[id] = y;
    pre_w[id] = w;
}

static rt_size_t ilitek_read_point(struct rt_touch_device *touch, void *buf,
                                  rt_size_t read_num)
{
    int packet_len = 5;
    int report_max_point;
    int i, count;
    rt_uint8_t ilt_buf[640];
    unsigned int status, tmp_x, tmp_y, x, y;
    unsigned int tmp;
    rt_err_t error;

    memset(ilt_buf, 0, sizeof(ilt_buf));
    if (__ilitek_read_regs(&ilitek_client, RT_NULL, ilt_buf, 64) != RT_EOK) {
        LOG_I("read point failed\n");
        read_num = 0;
        goto exit_;
    }

    report_max_point = ilt_buf[REPORT_COUNT_ADDRESS];

    if (report_max_point > 10) {
        count = 10;
    } else {
        count = report_max_point;
    }

    LOG_D("count %d\n", count);
    for (i = 1; i < count; i++) {
        memset(ilt_buf, 0, sizeof(ilt_buf));
        error = __ilitek_read_regs(&ilitek_client, RT_NULL, ilt_buf + i * 64, 64);
        if (error) {
            LOG_I("get touch info. failed, cnt:%d, err:%d\n",
                count, error);
            read_num = 0;
            goto exit_;
        }
    }

    for (i = 0; i < report_max_point; i++) {
        status = ilt_buf[i * packet_len + 1] & 0x40;
        if (!status)
            continue;

        tmp = (unsigned int)ilt_buf[i * packet_len + 3];
        x = (tmp << 8) + (unsigned int) ilt_buf[i * packet_len + 2];
        tmp = ilt_buf[i * packet_len + 5];
        y = (tmp << 8) + ilt_buf[i * packet_len + 4];

        //LOG_I("x: %d,  y: %d\n", x, y);
        tmp_x = x*600/16384;
        tmp_y = y*1280/16384;
#ifdef AICFB_ROTATE_0
        y = tmp_y;
        x = tmp_x;
#else
        y = 600-tmp_x;
        x = tmp_y;
#endif


        if (x > ILK_MAX_X || y > ILK_MAX_Y) {
            LOG_D("invalid position, X[%u], Y[%u]\n", x, y);
            continue;
        }
        read_num = 1;
        ilitek_touch_down(buf, 0, x, y, 0);
        LOG_D("x: %u,  y: %u\n", x, y);
        goto exit_;
    }

    ilitek_touch_up(buf, 0);

exit_:

    return read_num;
}

static rt_err_t ilitek_get_info(struct rt_i2c_client *dev,
                               struct rt_touch_info *info)
{
    info->range_x = ILK_MAX_X;
    info->range_y = ILK_MAX_Y;
    info->point_num = 1;

    return RT_EOK;
}

static rt_err_t ilitek_control(struct rt_touch_device *touch, int cmd, void *arg)
{
    if (cmd == RT_TOUCH_CTRL_GET_INFO) {
        return ilitek_get_info(&ilitek_client, arg);
    }

    return -RT_ERROR;
}

static struct rt_touch_ops ilitek_touch_ops = {
    .touch_readpoint = ilitek_read_point,
    .touch_control = ilitek_control,
};

static int rt_hw_ilitek_init(const char *name, struct rt_touch_config *cfg)
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

    ilitek_client.bus =
        (struct rt_i2c_bus_device *)rt_device_find(cfg->dev_name);

    if (ilitek_client.bus == RT_NULL) {
        LOG_E("Can't find %s device", cfg->dev_name);
        return -RT_ERROR;
    }

    if (rt_device_open((rt_device_t)ilitek_client.bus, RT_DEVICE_FLAG_RDWR) !=
        RT_EOK) {
        LOG_E("open %s device failed", cfg->dev_name);
        return -RT_ERROR;
    }

    ilitek_client.client_addr = ILITEK_ADDRESS;

    /* register touch device */
    touch_device->info.type = RT_TOUCH_TYPE_CAPACITANCE;
    touch_device->info.vendor = RT_TOUCH_VENDOR_GT;
    rt_memcpy(&touch_device->config, cfg, sizeof(struct rt_touch_config));
    touch_device->ops = &ilitek_touch_ops;

    if (RT_EOK != rt_hw_touch_register(touch_device, name, RT_DEVICE_FLAG_INT_RX, RT_NULL)) {
        LOG_E("touch device ilitek init failed !!!");
        return -RT_ERROR;
    }

    LOG_I("touch device ilitek init success");
    return RT_EOK;
}

static int rt_ilitek_gpio_cfg()
{
    unsigned int g, p;
    long pin;

    // RST
    pin = drv_pin_get(ILITEK_RST_PIN);
    g = GPIO_GROUP(pin);
    p = GPIO_GROUP_PIN(pin);
    hal_gpio_direction_input(g, p);

    // INT
    pin = drv_pin_get(ILITEK_INT_PIN);
    g = GPIO_GROUP(pin);
    p = GPIO_GROUP_PIN(pin);
    hal_gpio_direction_input(g, p);
    hal_gpio_set_irq_mode(g, p, 0);

    return 0;
}

static int rt_hw_litek_port(void)
{
    struct rt_touch_config cfg;
    rt_uint8_t rst_pin;

    rt_ilitek_gpio_cfg();

    rst_pin = drv_pin_get(ILITEK_RST_PIN);
    cfg.dev_name = ILITEK_I2C_CHAN;
    cfg.irq_pin.pin = drv_pin_get(ILITEK_INT_PIN);
    cfg.irq_pin.mode = PIN_MODE_INPUT_PULLUP;
    cfg.user_data = &rst_pin;

    rt_hw_ilitek_init("ilitek", &cfg);

    return 0;
}

INIT_DEVICE_EXPORT(rt_hw_litek_port);
