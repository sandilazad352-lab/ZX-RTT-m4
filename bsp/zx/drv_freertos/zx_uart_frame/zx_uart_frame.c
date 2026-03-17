#include "zx_uart_frame.h"
#include "aic_osal.h"

const uint16_t crctalbeabs[] = { 
	0x0000, 0xCC01, 0xD801, 0x1400, 0xF001, 0x3C00, 0x2800, 0xE401, 
	0xA001, 0x6C00, 0x7800, 0xB401, 0x5000, 0x9C01, 0x8801, 0x4400 
};
 
uint16_t crc16tablefast(uint8_t *data, uint32_t len)
{
	uint16_t crc = 0xffff; 
	uint8_t *ptr = data;
	uint32_t i;
	uint8_t ch;
 
	for (i = 0; i < len; i++) {
		ch = *ptr++;
		crc = crctalbeabs[(ch ^ crc) & 15] ^ (crc >> 4);
		crc = crctalbeabs[((ch >> 4) ^ crc) & 15] ^ (crc >> 4);
	} 
	
	return crc;
}

uint16_t crc16tablefast_muti(uint8_t *data, uint32_t len, 
                             uint8_t *data1, uint32_t len1) 
{
	uint16_t crc = 0xffff; 
	uint8_t *ptr = data;
	uint8_t *pt1 = data1;
	uint32_t i;
	uint8_t ch;

	for (i = 0; i < len; i++) {
		ch = *ptr++;
		crc = crctalbeabs[(ch ^ crc) & 15] ^ (crc >> 4);
		crc = crctalbeabs[((ch >> 4) ^ crc) & 15] ^ (crc >> 4);
	} 

	for(i = 0; i<len1; i++) {
		ch = *pt1++;
		crc = crctalbeabs[(ch ^ crc) & 15] ^ (crc >> 4);
		crc = crctalbeabs[((ch >> 4) ^ crc) & 15] ^ (crc >> 4);
	}

	return crc;
}

uint32_t frame_crc_check_modbus_8005(uint32_t default_value, uint8_t* data, uint16_t len) {
	uint16_t crc = default_value; 
	uint8_t *ptr = data;
	uint32_t i;
	uint8_t ch;

	for (i = 0; i < len; i++) {
		ch = *ptr++;
		crc = crctalbeabs[(ch ^ crc) & 15] ^ (crc >> 4);
		crc = crctalbeabs[((ch >> 4) ^ crc) & 15] ^ (crc >> 4);
	} 

	return crc;
}

uint32_t frame_crc_check(uint32_t default_value, uint8_t* data, uint16_t len) {
    for (uint16_t i = 0; i < len; i++) {
        default_value ^= data[i];
    }
    return default_value;
}

uint32_t frame_read(uint8_t* data, uint16_t* len, void* user_data) {
    uart_num_t num = (uart_num_t)user_data;
    // len == 0, query how many bytes can be read
    if (*len == 0) {
        *len = zx_uart_wait_rx_data(num, AICOS_WAIT_FOREVER);
        return 0;
    }

    uint16_t read_len = zx_uart_read(num, data, *len);
    *len = zx_uart_rx_available(num);
    return read_len;
}

uint32_t frame_write(uint8_t* data, uint16_t len, void* user_data) {
    uart_num_t num = (uart_num_t)user_data;
    zx_uart_write(num, data, len);

    // printf("write data: ");
    // for (uint16_t i = 0; i < len; i++) {
    //     printf("%02x ", data[i]);
    // }
    // printf("\r\n");
    return len;
}

uint64_t frame_get_time_ms(void* user_data) {
    return aic_get_time_ms();
}

zx_frame_handle_t zx_uart_frame_init(uart_num_t num, zx_frame_config_t* config, frame_callback_t cb) {
    zx_frame_driver_t driver = {
        .frame_callback = cb,
        .frame_crc_check = frame_crc_check_modbus_8005,
        .frame_read = frame_read,
        .frame_write = frame_write,
        .frame_get_time_ms = frame_get_time_ms,
    };

    config->user_data = (void*)num;
    zx_frame_handle_t handle = zx_frame_init(config, &driver);
    return handle;
}
