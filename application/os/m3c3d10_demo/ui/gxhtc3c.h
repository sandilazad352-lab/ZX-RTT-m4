#ifndef __GXHTC3C_H__
#define __GXHTC3C_H__

#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>

/* gxhtc3c commands define */   
// soft reset
#define CMD_SOFT_RESET      0x805D      

#define CMD_MEAS_POLLING    0x7CA2  

#define GXHTC3C_ADDR_PD 0x70 // addr pin pulled down: 0x70
#define GXHTC3C_ADDR_PU 0x71 // addr pin pulled up: 0x71

struct gxhtc3c_device
{
    struct rt_i2c_bus_device *i2c;
    rt_uint8_t gxhtc3c_addr ;
    rt_mutex_t lock;
    float temperature ;
    float humidity ;
    /* the command for data readout: change it as you need */ 
    rt_uint16_t cmd_readout;
};
typedef struct gxhtc3c_device *gxhtc3c_device_t;

/** 
 * This function read temperature and humidity by single shot mode 
 * Attention: 
 *  - rt_thread_mdelay() is called to wait for GXHTC3C to be ready to read
 *  - the temperature and humidity is stored in the device driver structure
 * 
 * @param dev the pointer of device driver structure
 * 
 * @return the status of read data from GXHTC3C, RT_EOK means success.
 */
rt_err_t gxhtc3c_read_singleshot(gxhtc3c_device_t dev);

/**
 * This function resets all parameter with default
 *
 * @param dev the pointer of device driver structure
 *
 * @return the softreset status,RT_EOK reprensents setting successfully.
 */
rt_err_t gxhtc3c_softreset(gxhtc3c_device_t dev);

/**
 * This function initializes gxhtc3c registered device driver
 *
 * @param dev the name of gxhtc3c device
 *
 * @return the gxhtc3c device.
 */
gxhtc3c_device_t gxhtc3c_init(const char *i2c_bus_name, rt_uint8_t gxhtc3c_addr);

/**
 * This function releases memory and deletes mutex lock
 *
 * @param dev the pointer of device driver structure
 */
void gxhtc3c_deinit(gxhtc3c_device_t dev);

#endif /* _SHT30_H__ */