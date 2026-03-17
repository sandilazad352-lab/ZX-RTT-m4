#include <rtconfig.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <finsh.h>
#include <string.h>
#include "aic_core.h"
#include <env.h>
#include <ota.h>
#include <absystem.h>
#include <dfs_posix.h>



#include <sys/time.h>
static float time_diff(struct timespec *start, struct timespec *end)
{
    float diff;
#define NS_PER_SEC      1000000000

    if (end->tv_nsec < start->tv_nsec) {
        diff = (float)(NS_PER_SEC + end->tv_nsec - start->tv_nsec)/NS_PER_SEC;
        diff += end->tv_sec - 1 - start->tv_sec;
    } else {
        diff = (float)(end->tv_nsec - start->tv_nsec)/NS_PER_SEC;
        diff += end->tv_sec - start->tv_sec;
    }

    return diff;
}

static struct timespec start = {0}, end = {0};


static void print_progress(size_t cur_size, size_t total_size)
{
    static unsigned char progress_sign[100 + 1];
    uint8_t i, per = cur_size * 100 / total_size;
    static unsigned char per_size = 0;

    if (per > 100) {
        per = 100;
    }

    if (per_size == per)
        return;

    for (i = 0; i < 100; i++) {
        if (i < per) {
            progress_sign[i] = '=';
        } else if (per == i) {
            progress_sign[i] = '>';
        } else {
            progress_sign[i] = ' ';
        }
    }

    progress_sign[sizeof(progress_sign) - 1] = '\0';

    // LOG_I("Download: [%s] %03d%%\033[1A", progress_sign, per);
    printf("Download: [%s] %03d%%\033[1A\r\n", progress_sign, per);

    // user_ota_per = per;

    per_size = per;
}


/* UART Register Offsets. */

static struct rt_semaphore rx_sem = {0};
static rt_device_t serial = NULL;


#define UPFILE_TYPE_CPIO 0  //文件类型

#define UUP_PACKET_SIZE		2048
#define UUP_MAX_FRAME_LEN	(UUP_PACKET_SIZE + 16)
#define OTA_RX_FRAME_NUM	8
static unsigned char uart_rx_buf[OTA_RX_FRAME_NUM][UUP_MAX_FRAME_LEN];
static unsigned char *uart_rx_ptr;
static int uart_rx_head = 0;
static int uart_rx_tail = 0;


typedef enum {
	UART_FRAME_START,
	UART_FRAME_FILEINFO,
	UART_FRAME_FILEXFER,
	UART_FRAME_FINISH,
} eUartFrameType;

typedef enum {
	OTA_STATE_IDLE,
	OTA_STATE_START,
	OTA_STATE_GET_FILEINFO,
	OTA_STATE_FILE_TFR,
	OTA_STATE_END,
} eOtaUpdateState;

#define UART_ACK_OK			1
#define UART_ACK_FAIL		0

#define OTA_MAX_FILE_SIZE	0x2000000

static int ota_status = OTA_STATE_IDLE;
static int ota_file_type = 0;
static int ota_file_size = 0;
static int ota_rev_size = 0;
static int ota_rev_packet = 0;
static int ota_rev_len = 0;

/*
 *	应答包：
 *	0x55 0x80 0x02(数据域长度) 
 *	应答帧类型(0x00开始升级 0x01文件信息 0x02文件传输 0x03传输结束) 返回结果（0x00失败 0x01成功)
 * 	校验字节
 */ 		
static void ota_send_ack(int type, int ret)
{
	unsigned char buf[7] = {0x55, 0x80, 0xc5, 0x02, 0x00, 0x00, 0x00};
	int i;

	buf[4] = type;
	buf[5] = ret;
	for (i = 1; i < 6; i++)
		buf[6] ^= buf[i];

    rt_device_write(serial, 0, buf, 7);
}

#define OTA_BUFFER_LEN 8192
static uint8_t *ota_buffer;

static void _ota_update(uint8_t *framebuf, size_t len)
{
	// framebuf 指向数据域
	int frametype = framebuf[0];	//帧类型(0x00开始升级 0x01文件信息 0x02文件传输 0x03传输结束)
	uint8_t *buf = framebuf + 1;	//数据域
	unsigned int framelen;
	unsigned int packetnum;			//数据包序数

	switch (frametype) 
	{
		//接收到开始信号
		case UART_FRAME_START:
			ota_send_ack(frametype, UART_ACK_OK);
			ota_status = OTA_STATE_START;
			break;

		//接收文件信息
		case UART_FRAME_FILEINFO:
			if (ota_status != OTA_STATE_START && ota_status != OTA_STATE_GET_FILEINFO) {
				ota_send_ack(frametype, UART_ACK_FAIL);
				break;
			}
			ota_rev_len = 0;		//接收字节长度
			ota_rev_packet = 0;		//接收包数量
            ota_rev_size = 0;

			ota_file_type = buf[0];		//文件类型
            printf("ota_file_type %d.\n", ota_file_type);
			if (ota_file_type != UPFILE_TYPE_CPIO) {
				printf("Rev wrong file type %d.\n", ota_file_type);
				ota_send_ack(frametype, UART_ACK_FAIL);
				break;
			}

			ota_file_size = (buf[1] << 24) | (buf[2] << 16) | (buf[3] << 8) | buf[4];	//文件数据包个数  ota_packet_num

			// ota_file_size = UUP_PACKET_SIZE * ota_packet_num;			//文件总大小
            // printf("ota_packet_num %d.\n", ota_packet_num);
            printf("ota_file_size %d.\n", ota_file_size);
			if (ota_file_size > OTA_MAX_FILE_SIZE) {
				printf("Rev wrong file size.\n");
				ota_send_ack(frametype, UART_ACK_FAIL);
				break;
			}

			ota_send_ack(frametype, UART_ACK_OK);
			ota_status = OTA_STATE_GET_FILEINFO;
			break;

		//接收文件数据
		case UART_FRAME_FILEXFER:
			if (ota_status != OTA_STATE_GET_FILEINFO && ota_status != OTA_STATE_FILE_TFR) {
				ota_send_ack(frametype, UART_ACK_FAIL);
				break;
			}

			packetnum = buf[0];		//数据包序数
			// printf("ota_rev_packet %d.\n", ota_rev_packet);
			if ((ota_rev_packet & 0xff) != packetnum) {
				printf("Wrong packet number.\n");
				ota_send_ack(frametype, UART_ACK_FAIL);
				break;
			}


			framelen = len - 2;		//帧长度	数据帧：帧类型+包序数+数据包
			/* only last frame size is less than UUP_PACKET_SIZE */
			// if (framelen > UUP_PACKET_SIZE ||
			// 	(framelen < UUP_PACKET_SIZE && ota_rev_packet != ota_packet_num - 1)) {
			// 	printf("Wrong packet len.\n");
			// 	ota_send_ack(frametype, UART_ACK_FAIL);
			// 	break;
			// }

			if (ota_rev_len + framelen > OTA_BUFFER_LEN) {
				printf("ota_buffer is overflow.\n");
				ota_send_ack(frametype, UART_ACK_FAIL);
				break;
			}

			// rt_kprintf("ota_rev_len %d\n", ota_rev_len);
			memcpy(ota_buffer + ota_rev_len, buf + 1, framelen);	//将buf中有效文件数据拷贝到 ota_buffer缓存区
			ota_rev_len += framelen;
            ota_rev_size += framelen;
            ota_rev_packet++;

            // rt_kprintf("framelen %d\n", framelen);
            // rt_kprintf("ota_rev_size %d\n", ota_rev_size);

            // print_progress(ota_rev_size, ota_file_size);
            if(ota_rev_len >= 2048)	
			{
				//写入flash
				ota_shard_download_fun((char *)ota_buffer, 2048);

				ota_rev_len -= 2048;
				memcpy(ota_buffer, ota_buffer + 2048, ota_rev_len);
			}

			ota_send_ack(frametype, UART_ACK_OK);
            // rt_kprintf("UART_ACK_OK\n");
			ota_status = OTA_STATE_FILE_TFR;
			break;

		case UART_FRAME_FINISH:
            rt_kprintf("ota_rev_size %d\n", ota_rev_size);
            rt_kprintf("ota_file_size %d\n", ota_file_size);
			if (ota_status != OTA_STATE_FILE_TFR && ota_status != UART_FRAME_FINISH) {
				ota_send_ack(frametype, UART_ACK_FAIL);
				break;
			}
			//0异常  1正常
			if (!buf[0]) {
				printf("update end with error!\n");
				ota_send_ack(frametype, UART_ACK_FAIL);
				ota_status = OTA_STATE_END;
				break;
			}
            
            if(ota_rev_size != ota_file_size)
            {
                rt_kprintf("ota_rev_size %d\n", ota_rev_size);
                rt_kprintf("ota_file_size %d\n", ota_file_size);
                rt_kprintf("Wrong packet num\n");
                ota_send_ack(frametype, UART_ACK_FAIL);
				ota_status = OTA_STATE_END;
                break;
            }
			if(ota_rev_len != 0)
			{
				//写入flash 剩余长度ota_rev_len
				ota_shard_download_fun((char *)ota_buffer, ota_rev_len);

				ota_rev_len = 0;
			}
	
			printf("update from ota ok.\n");
			ota_send_ack(frametype, UART_ACK_OK);

			ota_status = OTA_STATE_END;
			LOG_I("\033[0B");
			LOG_I("Download firmware to flash success.");
			LOG_I("System now will restart...");
			rt_thread_delay(rt_tick_from_millisecond(5));
	        int ret = RT_EOK;
			ret = aic_upgrade_end();
			if (ret) {
				LOG_E("Aic upgrade end");
			}

            clock_gettime(CLOCK_REALTIME, &end);
            printf("ota time:%0.1f\n",time_diff(&start, &end));

            rt_thread_mdelay(2000);
            extern void cmd_reboot(int argc, char **argv);
            cmd_reboot(0, NULL);        //重启
			break;
	}
}


//服务器：等待MCU请求升级
static void uart_rx_demo_thread(void *param)
{
	int ret = RT_EOK;
	ret = aic_upgrade_start();
    if (ret) {
        LOG_E("Aic get os to upgrade");
        return;
    }
	ota_init();
	
    ota_buffer = rt_malloc(OTA_BUFFER_LEN);
    memset(ota_buffer, 0, OTA_BUFFER_LEN);

	//打开并配置串口
	uint8_t uartrx[UUP_MAX_FRAME_LEN] = {0};
	volatile int ret_r = 0;
	int len = 0;
	int str_index = 0;

	int i = 0;
    unsigned int pack_len = 0;
    rt_int32_t wait_tick = rt_tick_from_millisecond(1000);
    uint8_t time_out = 0;
	//接收串口数据
	for (;;) {
        memset(uartrx, 0, sizeof(uartrx));
		ret_r = rt_device_read(serial, -1, uartrx, 1);

        if(ret_r > 0)
        {
            if(uartrx[0] == 0x55)
            {
                static bool flag = false;
                if(flag == false){
                    flag = true;
                    clock_gettime(CLOCK_REALTIME, &start);
                }

                uart_rx_ptr = &uart_rx_buf[uart_rx_head][0];
                len = 0;
                str_index = 1;
                while(len < 2)
                {
                    ret_r = rt_device_read(serial, -1, uartrx+str_index, 2-len);
                    if(ret_r > 0)
                    {
                        len += ret_r;
                        str_index+=ret_r;
                    }
                }
                str_index = 3;
                if(uartrx[1] == 0x81 && uartrx[2] == 0xC6)
                {
                    len = 0;
                    while(len < 2)
                    {
                        ret_r = rt_device_read(serial, -1, uartrx+str_index, 2-len);
                        if(ret_r > 0)
                        {
                            len += ret_r;
                            str_index+=ret_r;
                        }
                    }
                    pack_len = (uartrx[3] << 8) | uartrx[4];
                    str_index = 5;
                    len = 0;
                    while(len < pack_len+1)
                    {
                        ret_r = rt_device_read(serial, -1, uartrx+str_index, pack_len+1-len);
                        if(ret_r > 0)
                        {
                            len += ret_r;
                            str_index+=ret_r;
                        }
                    }

                    memcpy(uart_rx_ptr, uartrx+1, pack_len+5);
                    uart_rx_head = (uart_rx_head + 1) % OTA_RX_FRAME_NUM;
                }
            }else{
                printf("uartrx[0] error\n");
            }
        }

		//处理并解析串口数据
		if (uart_rx_tail != uart_rx_head) {
			unsigned char *buf;
			unsigned int checksum = 0;

			buf = &uart_rx_buf[uart_rx_tail][0];

			len = (buf[2] << 8) | buf[3];

			//BCC校验
			for (i = 0; i < len + 4; i++)
				checksum ^= buf[i];

			if (checksum == buf[len + 4]) {
				//解析串口数据
				_ota_update(buf + 4, len);	//buf + 3 指针指向数据域
                
			} else {
				printf("rev frame checksum err.\n");
			}
			uart_rx_tail = (uart_rx_tail + 1) % OTA_RX_FRAME_NUM;
            len = 0;
		}
        
		if(RT_EOK != rt_sem_take(&rx_sem, wait_tick))// 串口接收超时处理
        {
            if(++time_out >= 10)
            {
                rt_device_close(serial);
                rt_sem_detach(&rx_sem);
                goto __exit;
            }
        }
	}
__exit:


    ota_deinit();

    return;
}

static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
{
	rt_sem_release(&rx_sem);

	return RT_EOK;
}


int uart_rx_demo(char *zx_uart)
{
	rt_err_t ret = RT_EOK;

    rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);

	serial = rt_device_find(zx_uart);
    if (!serial)
    {
        rt_kprintf("find %s failed!\n", zx_uart);
        ret = RT_ERROR;
        goto exit;
    }

    //串口接收线程
	rt_thread_t thread = rt_thread_create("serial", uart_rx_demo_thread, RT_NULL, 1024*8, 10, 10);
    if (thread != RT_NULL)
    {
        rt_thread_startup(thread);
    }
    else
    {
        ret = RT_ERROR;
        goto exit;
    }
    
    ret = rt_device_open(serial, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
	if (ret != RT_EOK)
    {
        rt_kprintf("open %s failed : %d !\n", zx_uart, ret);
        ret = RT_ERROR;
        goto exit;
    }
    rt_kprintf("open %s finish\n", zx_uart);
    rt_device_set_rx_indicate(serial, uart_input);

exit:
    rt_sem_detach(&rx_sem);
	return 0;
}

void zx_uart_ota(uint8_t argc, char **argv)
{
    if (argc < 2) {
        rt_kprintf("using: zx_uart_ota uart7\n");
    } else {
        uart_rx_demo(argv[1]);
    }
}
MSH_CMD_EXPORT(zx_uart_ota, Use Uart to download the firmware);
