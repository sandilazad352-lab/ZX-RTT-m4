#include <rtthread.h>
#include <rtdevice.h>

#include <string.h>

#define DBG_TAG "cst3240"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

#include "cst3240.h"


static struct rt_i2c_client cst3240_client;
static struct rt_touch_data *read_data;


static rt_err_t __cst3240_read_data(struct rt_i2c_client *dev, rt_uint8_t *data, rt_uint8_t len)
{
    struct rt_i2c_msg msgs;

    msgs.addr = dev->client_addr;
    msgs.flags = RT_I2C_RD;
    msgs.buf = data;
    msgs.len = len;

    if (rt_i2c_transfer(dev->bus, &msgs, 1) == 1) {
        return RT_EOK;
    } else {
        rt_kprintf("__cst3240_read_data failed[%d]\n", __LINE__);
        return -RT_ERROR;
    }
}
static rt_err_t __cst3240_write_data(struct rt_i2c_client *dev, rt_uint8_t *data,
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
        rt_kprintf("__cst3240_write_data failed[%d]\n", __LINE__);
        return -RT_ERROR;
    }
}

static rt_err_t __cst3240_read_reg(struct rt_i2c_client *dev, rt_uint16_t reg_addr, 
                            rt_uint8_t *buf, rt_uint8_t len)
{
    struct rt_i2c_msg msg = {0};
    uint8_t addr_buf[2] ={0};
    addr_buf[0] = (reg_addr >> 8) & 0xFF;
    addr_buf[1] = reg_addr & 0xFF;

    msg.addr = dev->client_addr;
    msg.flags = RT_I2C_WR;
    msg.buf = addr_buf;
    msg.len = 2;
    if (rt_i2c_transfer(dev->bus, &msg, 1) != 1) {
        rt_kprintf("__cst3240_read_reg failed[%d]\n", __LINE__);
        return -RT_ERROR;
    }

    msg.addr = dev->client_addr;
    msg.flags = RT_I2C_RD;
    msg.buf = buf;
    msg.len = len;
    if (rt_i2c_transfer(dev->bus, &msg, 1) != 1) {
        rt_kprintf("__cst3240_read_reg failed[%d]\n", __LINE__);
        return -RT_ERROR;
    }
    return RT_EOK;
}
static rt_err_t __cst3240_write_reg(struct rt_i2c_client *dev, rt_uint16_t reg_addr, 
                                rt_uint8_t *buf, rt_uint8_t len)
{
    struct rt_i2c_msg msgs[2] = {0};

    uint8_t addr_buf[2] ={0};
    addr_buf[0] = (reg_addr >> 8) & 0xFF;
    addr_buf[1] = reg_addr & 0xFF;

    msgs[0].addr = dev->client_addr;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf = addr_buf;
    msgs[0].len = 2;

    msgs[1].addr = dev->client_addr;
    msgs[1].flags = RT_I2C_WR | RT_I2C_NO_START;
    msgs[1].buf = buf;
    msgs[1].len = len;

    if (rt_i2c_transfer(dev->bus, msgs, 2) != 2) {
        rt_kprintf("__cst3240_write_reg failed[%d]\n", __LINE__);
        return -RT_ERROR;
    }
    return RT_EOK;
}

static void __cst3240_touch_up(void *buf, int8_t id, int16_t x, int16_t y, int16_t w)

{
    read_data = (struct rt_touch_data *)buf;

    read_data[id].event = RT_TOUCH_EVENT_UP;
    read_data[id].timestamp = rt_touch_get_ts();
    read_data[id].width = w;
    read_data[id].x_coordinate = x;
    read_data[id].y_coordinate = y;
    read_data[id].track_id = id;
}


static void __cst3240_touch_down(void *buf, int8_t id, int16_t x, int16_t y,
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


static rt_size_t cst3240_read_point(struct rt_touch_device *touch, void *buf,
                                  rt_size_t read_num)
{
    read_num = 0;
    uint16_t input_x = 0, input_y = 0;
    rt_uint8_t status = 0x00;

    rt_uint8_t data_r[7] = {0x0};
    rt_uint16_t reg_addr = 0xD000;

    __cst3240_read_reg(&cst3240_client, reg_addr, data_r, 7);

    status = data_r[0] & 0x0F;
    read_num = 1;
    input_x = (data_r[1] << 4) | ((data_r[3] >> 4 ) & 0x0F);
    input_y = (data_r[2] << 4) | (data_r[3] & 0x0F);
    rt_uint8_t data_w[2] = {0};
    data_w[0] = 0xAB;
    __cst3240_write_reg(&cst3240_client, 0xD000, data_w, 1);

    // rt_kprintf("%u/%u/%u/%u\n", input_x, input_y, read_num, status);
    if (status == TOUCH_EVT_RELEASE)
        __cst3240_touch_up(buf, 0, input_x, input_y, 0);
    else if(status == TOUCH_EVT_PRESS)
        __cst3240_touch_down(buf, 0, input_x, input_y, 0);

    return read_num;
}

static rt_err_t __cst3240_get_info(struct rt_i2c_client *dev,
                               struct rt_touch_info *info)
{
    info->point_num = 1;

    return RT_EOK;
}
static rt_err_t cst3240_control(struct rt_touch_device *touch, int cmd, void *arg)
{
    if (cmd == RT_TOUCH_CTRL_GET_INFO) {
        return __cst3240_get_info(&cst3240_client, arg);
    }

    return -RT_ERROR;
}

static struct rt_touch_ops cst3240_touch_ops = {
    .touch_readpoint = cst3240_read_point,
    .touch_control = cst3240_control,
};

static int rt_hw_cst3240_init(const char *name, struct rt_touch_config *cfg)
{
    static struct rt_touch_device *touch_device = RT_NULL;

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

    cst3240_client.bus =
        (struct rt_i2c_bus_device *)rt_device_find(cfg->dev_name);

    if (cst3240_client.bus == RT_NULL) {
        LOG_E("Can't find %s device", cfg->dev_name);
        return -RT_ERROR;
    }

    if (rt_device_open((rt_device_t)cst3240_client.bus, RT_DEVICE_FLAG_RDWR) !=
        RT_EOK) {
        LOG_E("open %s device failed", cfg->dev_name);
        return -RT_ERROR;
    }

    cst3240_client.client_addr = CST3240_ADDRESS;

    /* register touch device */
    touch_device->info.type = RT_TOUCH_TYPE_CAPACITANCE;
    touch_device->info.vendor = RT_TOUCH_VENDOR_GT;
    rt_memcpy(&touch_device->config, cfg, sizeof(struct rt_touch_config));
    touch_device->ops = &cst3240_touch_ops;

    if (RT_EOK != rt_hw_touch_register(touch_device, name, RT_DEVICE_FLAG_INT_RX, RT_NULL)) {
        LOG_E("touch device cst3240 init failed !!!");
        return -RT_ERROR;
    }

    uint8_t data[2] = {0xD1, 0x09};
    __cst3240_write_data(&cst3240_client, data, 2);
    LOG_I("touch device cst3240 init success");
    return RT_EOK;
}

static int rt_cst3240_gpio_cfg()
{
    unsigned int g, p;
    long pin;

    // RST
    pin = drv_pin_get(CST3240_RST_PIN);
    g = GPIO_GROUP(pin);
    p = GPIO_GROUP_PIN(pin);
    hal_gpio_direction_input(g, p);

    // INT
    pin = drv_pin_get(CST3240_INT_PIN);
    g = GPIO_GROUP(pin);
    p = GPIO_GROUP_PIN(pin);
    hal_gpio_direction_input(g, p);
    hal_gpio_set_irq_mode(g, p, 0);

    return 0;
}

static int rt_hw_cst3240_port(void)
{
    struct rt_touch_config cfg;
    rt_uint8_t rst_pin;

    rt_cst3240_gpio_cfg();

    rst_pin = drv_pin_get(CST3240_RST_PIN);
    cfg.dev_name = CST3240_I2C_CHAN;
    cfg.irq_pin.pin = drv_pin_get(CST3240_INT_PIN);
    cfg.irq_pin.mode = PIN_MODE_INPUT_PULLUP;
    cfg.user_data = &rst_pin;

    rt_hw_cst3240_init("cst3240", &cfg);

    return 0;
}

INIT_DEVICE_EXPORT(rt_hw_cst3240_port);
