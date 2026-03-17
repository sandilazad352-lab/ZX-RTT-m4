#pragma once

#include "stdio.h"
#define HEADER_MAX_LENGTH 2

typedef void (*frame_callback_t)(uint32_t cmd, uint8_t* data, uint16_t len);
typedef uint32_t (*frame_crc_check_t)(uint32_t default_value, uint8_t* data, uint16_t len);
// if len == 0, query how many bytes can be read, len rewrite available bytes returun read data length
typedef uint32_t (*frame_read_t)(uint8_t* data, uint16_t* len, void* user_data); 
typedef uint32_t (*frame_write_t)(uint8_t* data, uint16_t len, void* user_data);
typedef uint64_t (*frame_get_time_ms_t)(void* user_data);
typedef void* zx_frame_handle_t;

#ifndef BIT
#define BIT(x) (1 << (x))
#endif

typedef enum {
    FRAME_HEAD = BIT(1),
    FRAME_CMD = BIT(2),
    FRAME_LEN = BIT(3),
    FRAME_DATA = BIT(4),
    FRAME_CRC = BIT(5),
} frame_body_t;

typedef struct  {
    uint8_t header[HEADER_MAX_LENGTH];
    uint8_t header_length;
    uint8_t cmd_length;
    uint8_t len_length;     // len high byte first
    uint8_t crc_length;
    
    uint32_t length_max;

    frame_body_t len_body;
    frame_body_t crc_body;
    
    void* user_data; // most used for uart handle
    uint32_t timeout_ms; // skip last recv data if timeout == 0

    struct {
        uint8_t len_ahead_cmd: 1; 
        uint8_t write_use_full_cache: 1;  // send full packet once time
    } flags;
} zx_frame_config_t;

typedef struct {
    frame_callback_t frame_callback;
    frame_crc_check_t frame_crc_check;
    frame_read_t frame_read;
    frame_write_t frame_write;
    frame_get_time_ms_t frame_get_time_ms;
} zx_frame_driver_t;

// HEAD(1B) + CMD(2B) + LEN(1B) + DATA + CRC(1B)
// LEN = DATA LEN, CRC = HEAD + CMD + LEN + DATA
#define ZX_FRAME_CONFIG_1 { \
    .header = {0x55}, \
    .header_length = 1, \
    .cmd_length = 2, \
    .len_length = 1, \
    .crc_length = 1, \
    .length_max = 256, \
    .len_body = FRAME_DATA, \
    .crc_body = FRAME_HEAD | FRAME_CMD | FRAME_LEN | FRAME_DATA, \
    .timeout_ms = 20, \
    .flags = { \
        .len_ahead_cmd = 0, \
        .write_use_full_cache = 1, \
    }, \
}

// HEAD(2B) + LEN(2B) + CMD(2B) + DATA + CRC(2B)
// LEN = HEAD + LEN + CMD + DATA + CRC, CRC = HEAD + LEN + CMD + DATA
#define ZX_FRAME_CONFIG_2 { \
    .header = {0xaa, 0x55}, \
    .header_length = 2, \
    .cmd_length = 2, \
    .len_length = 2, \
    .crc_length = 2, \
    .length_max = 256, \
    .len_body = FRAME_HEAD | FRAME_CMD | FRAME_LEN | FRAME_DATA | FRAME_CRC, \
    .crc_body = FRAME_HEAD | FRAME_CMD | FRAME_LEN | FRAME_DATA, \
    .timeout_ms = 20, \
    .flags = { \
        .len_ahead_cmd = 1, \
        .write_use_full_cache = 1, \
    }, \
}

zx_frame_handle_t zx_frame_init(zx_frame_config_t* config, zx_frame_driver_t* driver);

int zx_frame_write(zx_frame_handle_t handle, uint32_t cmd, uint8_t* buffer, uint16_t length);

void zx_frame_update(zx_frame_handle_t handle);
