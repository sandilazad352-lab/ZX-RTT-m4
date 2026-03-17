#include <string.h>

#define DBG_ENABLE
#define DBG_SECTION_NAME "gxhtc3c"
#define DBG_LEVEL DBG_LOG
#define DBG_COLOR
#include <rtdbg.h>

#include "gxhtc3c.h"

#define CRC_POLY 0x31 

/**
 * This function write 2-byte cmd to GXHTC3C 
 *
 * @param dev the pointer of device driver structure
 * @param cmd the 16bit command to be sent to GXHTC3C
 *
 * @return the cmd transfer status, RT_EOK reprensents setting successfully.
 */
static rt_err_t write_cmd(gxhtc3c_device_t dev, rt_uint16_t cmd)
{
    struct rt_i2c_msg msgs;
    rt_uint8_t buf[2] ;
    buf[0] = cmd >> 8 ;
    buf[1] = cmd & 0xFF ;

    msgs.addr = dev->gxhtc3c_addr;
    msgs.flags = RT_I2C_WR;
    msgs.buf = buf;
    msgs.len = 2;

    if (rt_i2c_transfer(dev->i2c, &msgs, 1) == 1)
        return RT_EOK;
    else
        return -RT_ERROR;
}

/**
 *  This function read len bytes from dev and store the bytes in to buf 
 * 
 * @param dev the pointer of device driver structure
 * @param buf the pointer to buffer array 
 * @param len the number of bytes to be read 
 * 
 * @return the i2c read status, RT_EOK represents success.
 */
static rt_err_t read_bytes(gxhtc3c_device_t dev, rt_uint8_t * buf, rt_uint8_t len)
{
    if(rt_i2c_master_recv(dev->i2c, dev->gxhtc3c_addr, RT_I2C_RD, buf, len) == len){
        return RT_EOK ;
    }else
    {
        return - RT_ERROR ;
    }
    
}

/**
 * This function calculate CRC value of bytes in buffer
 * CRC_POLY is predefined as 0x31
 * 
 * @param buf the pointer to buffer array 
 * @param len the length of buffer array 
 * 
 * @return calculated CRC value.
 */
static rt_uint8_t crc8(rt_uint8_t * buf, rt_uint8_t len)
{
    rt_uint8_t crc = 0xFF ;
    rt_uint8_t i, j ;
    for(j = len; j; j --){
        crc ^= *buf ++ ;
        for( i = 8; i; i --){
            crc = (crc & 0x80) ? (crc << 1) ^ CRC_POLY : (crc << 1) ;
        }
    }
    return crc ;
}

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
rt_err_t gxhtc3c_read_singleshot(gxhtc3c_device_t dev)
{
    rt_uint8_t temp[6] ;
    rt_err_t result;
    RT_ASSERT(dev);

    result = rt_mutex_take(dev->lock, RT_WAITING_FOREVER);
    if (result == RT_EOK)
    {
        if( write_cmd(dev, dev->cmd_readout) == RT_EOK)
        {
			// wait about 5 ms
			rt_thread_mdelay(5);
            if(read_bytes(dev, temp, 6) == RT_EOK){
                if(crc8(temp, 2) == temp[2]){
                    dev->temperature = -45.0 + (temp[1] | temp[0] << 8) * 175.0 / (0xFFFF - 1) ; //sensor temperature convert to reality
                    result = RT_EOK ;
                }else{
                    result = -RT_ERROR ;
                }
                if(crc8(temp + 3, 2) == temp[5]){
                    dev->humidity = (temp[4] | temp[3] << 8) * 0.0015259022 ; //sensor humidity convert to reality
                }else{
                    result = -RT_ERROR ;
                }
            }else
            {
                result = -RT_ERROR ;
            }
        }
        else
        {
            result = -RT_ERROR ;
        }
	rt_mutex_release(dev->lock);
    }
    else
    {
        LOG_E("Taking mutex of GXHTC3C failed.");
        result = -RT_ERROR;
    }
	
    return result;
}

/**
 * This function resets all parameter with default
 *
 * @param dev the pointer of device driver structure
 *
 * @return the softreset status, RT_EOK reprensents setting successfully.
 */
rt_err_t gxhtc3c_softreset(gxhtc3c_device_t dev)
{
    RT_ASSERT(dev);
    
    if( write_cmd(dev, CMD_SOFT_RESET) == RT_EOK)
    {
        return RT_EOK;
    }else{
        return RT_ERROR ;
    }
}

/**
 * This function initializes gxhtc3c registered device driver
 *
 * @param i2c_bus_name the name of i2c device used by GXHTC3C device
 * @param gxhtc3c_addr the address of GXHTC3C device
 * 
 * @return the pointer to gxhtc3c device driver structure.
 */
gxhtc3c_device_t gxhtc3c_init(const char *i2c_bus_name, rt_uint8_t gxhtc3c_addr)
{
    gxhtc3c_device_t dev;
    RT_ASSERT(i2c_bus_name);

    dev = rt_calloc(1, sizeof(struct gxhtc3c_device));
    if (dev == RT_NULL)
    {
        LOG_E("Can't allocate memory for gxhtc3c device on '%s' ", i2c_bus_name);
        return RT_NULL;
    }

    if(gxhtc3c_addr == GXHTC3C_ADDR_PD || gxhtc3c_addr == GXHTC3C_ADDR_PU){
        dev->gxhtc3c_addr = gxhtc3c_addr ;
    }else{
        LOG_E("Illegal gxhtc3c address:'%x'", gxhtc3c_addr);
        rt_free(dev);
        return RT_NULL;
    }
    
    dev->i2c = rt_i2c_bus_device_find(i2c_bus_name);

    if (dev->i2c == RT_NULL)
    {
        LOG_E("Can't find gxhtc3c device on '%s' ", i2c_bus_name);
        rt_free(dev);
        return RT_NULL;
    }

    dev->lock = rt_mutex_create("mutex_gxhtc3c", RT_IPC_FLAG_FIFO);
    if (dev->lock == RT_NULL)
    {
        LOG_E("Can't create mutex for gxhtc3c device on '%s' ", i2c_bus_name);
        rt_free(dev);
        return RT_NULL;
    }

    dev->cmd_readout = CMD_MEAS_POLLING ;
	
    return dev;
}

/**
 * This function releases memory and deletes mutex lock
 *
 * @param dev the pointer of device driver structure
 */
void gxhtc3c_deinit(gxhtc3c_device_t dev)
{
    RT_ASSERT(dev);

    rt_mutex_delete(dev->lock);

    rt_free(dev);
}

/**
 * This function is exported to MSH commands list 
 * Usage example:
 *  - gxhtc3c probe i2c1 pu : initialize gxhtc3c device on i2c1 bus with address pin pulled up(i2c address 0x45)
 *  - gxhtc3c probe i2c1: initialize gxhtc3c device one i2c1 bus with address pin pulled down by default(i2c address 0x44)
 *  - gxhtc3c read: read and print temperature and humidity from previously initialized gxhtc3c
 */

int humidity = 0, temp = 0;

void gxhtc3c(int argc, char *argv[])
{
    static gxhtc3c_device_t dev = RT_NULL;
    rt_uint8_t sht_addr = GXHTC3C_ADDR_PD ;
    
    if (argc > 1)
    {
        if (!strcmp(argv[1], "probe"))
        {
            if (argc >= 3)
            {
                /* initialize the sensor when first probe */
                if (!dev || strcmp(dev->i2c->parent.parent.name, argv[2]))
                {
                    /* deinit the old device */
                    if(dev)
                    {
						rt_kprintf("Deinit gxhtc3c");
                        gxhtc3c_deinit(dev);
                    }
                    // no else needed here
                    if( argc > 3)
                    {
                        if( !strcmp("pd", argv[3]))
                        {
                            sht_addr = GXHTC3C_ADDR_PD ;
                        }
                        else if( !strcmp("pu", argv[3]))
                        {
                            sht_addr = GXHTC3C_ADDR_PU ;
                        }
                        else
                        {
                            rt_kprintf("Illegal gxhtc3c address, using 0x44 by default\n");
                            sht_addr = GXHTC3C_ADDR_PD ; // pulled down by default: 0x44 
                        }
                    }
                    // no else needed here

                    dev = gxhtc3c_init(argv[2], sht_addr);
                    if(!dev)
                    {
                        rt_kprintf("gxhtc3c probe failed, check input args\n");
                    }else
					{
						rt_kprintf("gxhtc3c probed, addr:0x%x\n", sht_addr) ;
					}
                }
            }
            else
            {
                rt_kprintf("gxhtc3c probe <i2c dev name>   - probe sensor by given name\n");
            }
        }
        else if (!strcmp(argv[1], "read"))
        {
            if (dev)
            {
                /* read the sensor data */
                gxhtc3c_read_singleshot(dev);

                rt_kprintf("gxhtc3c humidity   : %d.%d \n", (int)dev->humidity, (int)(dev->humidity * 10) % 10);
                rt_kprintf("gxhtc3c temperature: %d.%d \n", (int)dev->temperature, (int)(dev->temperature * 10) % 10);
                humidity = (int)dev->humidity * 10 + (int)(dev->humidity * 10) % 10;
                temp = (int)dev->temperature * 10 + (int)(dev->temperature * 10) % 10;
            }
            else
            {
                rt_kprintf("Please using 'gxhtc3c probe <i2c dev name> <pu/pd>' first\n");
            }
        }
		else if (!strcmp(argv[1], "reset"))
		{
			if(dev)
			{
				if(gxhtc3c_softreset(dev) == RT_EOK)  //复位
				{
					rt_kprintf("gxhtc3c reset cmd sent\n");
				}else
				{
					rt_kprintf("gxhtc3c reset cmd not sent\n");
				}
			}else
			{
				rt_kprintf("Please using 'gxhtc3c probe <i2c dev name> <pu/pd>' first\n");
			}
		}
        else
        {
            rt_kprintf("Unknown command. Enter 'gxhtc3c' for help\n");
        }
    }
    else
    {
        rt_kprintf("Usage:\n");
        rt_kprintf("\tgxhtc3c probe <i2c dev name> <pu/pd> -- probe sensor by i2c dev name and pull config\n");
        rt_kprintf("\tgxhtc3c read -- read sensor gxhtc3c data\n");
		rt_kprintf("\tgxhtc3c reset -- send soft reset command to gxhtc3c\n");
    }
}

MSH_CMD_EXPORT(gxhtc3c, gxhtc3c sensor);

#if 0
    gxhtc3c probe i2c1

    gxhtc3c read
#endif