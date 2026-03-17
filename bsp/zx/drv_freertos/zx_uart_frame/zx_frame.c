#include "zx_frame.h"
#include "stdlib.h"
#include "string.h"

typedef struct {
    uint8_t* frame;
    uint16_t length;

    uint8_t* cache_data; // will malloc length_max if cfg.flags.write_use_full_cache = 1 else length_min
    
    uint16_t length_min;
    uint16_t length_max; // will malloc this length cache full frame

    uint8_t len_offset;
    uint8_t cmd_offset;
    uint8_t crc_offset;
    uint8_t data_offset;
    uint8_t len_value_complete; // data len + command = total_len;
    
    uint64_t last_frame_time_ms;
    zx_frame_config_t cfg;
    zx_frame_driver_t driver;
} zx_uart_frame_t;

zx_frame_handle_t zx_frame_init(zx_frame_config_t* config, zx_frame_driver_t* driver) {
    if (config == NULL) {
        return NULL;
    }
    zx_uart_frame_t* frame = (zx_uart_frame_t*)calloc(1, sizeof(zx_uart_frame_t));
    if (frame == NULL) {
        return NULL;
    }
    
    frame->length_max = config->length_max;
    frame->length_min = config->header_length + config->cmd_length + config->len_length + config->crc_length;
    frame->cfg = *config;
    frame->frame = (uint8_t *)malloc(config->length_max);
    if (frame->frame == NULL) {
        goto exit;
    }
    frame->cache_data = (uint8_t *)malloc(config->flags.write_use_full_cache ? frame->length_max : frame->length_min);
    if (frame->cache_data == NULL) {
        goto exit;
    }

    frame->len_value_complete = frame->length_min;
    if (config->len_body & FRAME_CMD) {
        frame->len_value_complete -= config->cmd_length;
    }
    if (config->len_body & FRAME_LEN) {
        frame->len_value_complete -= config->len_length;
    }
    if (config->len_body & FRAME_CRC) {
        frame->len_value_complete -= config->crc_length;
    }
    if (config->len_body & FRAME_HEAD) {
        frame->len_value_complete -= config->header_length;
    }

    frame->crc_offset = config->header_length + config->cmd_length + config->len_length;
    if (config->crc_body & FRAME_HEAD) {
        frame->crc_offset = 0;
    } else {
        if (config->crc_body & FRAME_CMD) {
            frame->crc_offset -= frame->cfg.cmd_length;
        }
        if (config->crc_body & FRAME_LEN) {
            frame->crc_offset -= frame->cfg.len_length;
        }
    }

    frame->data_offset = config->header_length + config->cmd_length + config->len_length;
    if (config->flags.len_ahead_cmd) {
        frame->len_offset = config->header_length;
        frame->cmd_offset = frame->len_offset + frame->cfg.len_length;
    } else {
        frame->cmd_offset = config->header_length;
        frame->len_offset = frame->cmd_offset + frame->cfg.cmd_length;
    }

    frame->cfg.user_data = config->user_data;
    frame->driver = *driver;
    return frame;

exit:
    if (frame->cache_data) {
        free(frame->cache_data);
    }
    if (frame->frame) {
        free(frame->frame);
    }
    if (frame) {
        free(frame);
    }
    return NULL;
}

int zx_frame_write(zx_frame_handle_t handle, uint32_t cmd, uint8_t* buffer, uint16_t length) {
    zx_uart_frame_t* frame = (zx_uart_frame_t *)handle;
    uint32_t total_len = frame->length_min + length;
    if (total_len > frame->length_max && frame->cfg.flags.write_use_full_cache) {
        return -1;
    }

    uint8_t* write_ptr = frame->cache_data;
    for (uint8_t i = 0; i < frame->cfg.header_length; i++) {
        write_ptr[i] = frame->cfg.header[i];
    }
    write_ptr += frame->cfg.header_length;
    uint16_t data_length = length + frame->length_min - frame->len_value_complete;
    if (frame->cfg.flags.len_ahead_cmd) {
        for (uint8_t i = 0; i < frame->cfg.len_length; i++) {
            write_ptr[i] = (data_length >> ((frame->cfg.len_length - i - 1) * 8)) & 0xff;
        }
        write_ptr += frame->cfg.len_length;

        for (uint8_t i = 0; i < frame->cfg.cmd_length; i++) {
            write_ptr[i] = (cmd >> ((frame->cfg.cmd_length - i - 1) * 8)) & 0xff;
        }
        write_ptr += frame->cfg.cmd_length;
    } else {
        for (uint8_t i = 0; i < frame->cfg.cmd_length; i++) {
            write_ptr[i] = (cmd >> ((frame->cfg.cmd_length - i - 1) * 8)) & 0xff;
        }
        write_ptr += frame->cfg.cmd_length;

        for (uint8_t i = 0; i < frame->cfg.len_length; i++) {
            write_ptr[i] = (data_length >> ((frame->cfg.len_length - i - 1) * 8)) & 0xff;
        }
        write_ptr += frame->cfg.len_length;
    }

    uint32_t crc_value = frame->driver.frame_crc_check(0, frame->cache_data + frame->crc_offset, frame->length_min - frame->cfg.crc_length - frame->crc_offset);
    crc_value = frame->driver.frame_crc_check(crc_value, buffer, length);

    if (frame->cfg.flags.write_use_full_cache) {
        for (uint16_t i = 0; i < length; i++) {
            write_ptr[i] = buffer[i];
        }
        write_ptr += length;
    }

    for (uint8_t i = 0; i < frame->cfg.crc_length; i++) {
        write_ptr[i] = (crc_value >> ((frame->cfg.crc_length - i - 1) * 8)) & 0xff;
    }
    
    if (frame->cfg.flags.write_use_full_cache) {
        frame->driver.frame_write(frame->cache_data, total_len, frame->cfg.user_data);
    } else {
        frame->driver.frame_write(frame->cache_data, frame->length_min - frame->cfg.crc_length, frame->cfg.user_data);
        frame->driver.frame_write(buffer, length, frame->cfg.user_data);
        frame->driver.frame_write(write_ptr, frame->cfg.crc_length, frame->cfg.user_data);
    }
    return 0;
}

static uint32_t deal_uart_buffer(zx_frame_handle_t handle, uint8_t* buffer, uint32_t data_length) {
    zx_uart_frame_t* frame = (zx_uart_frame_t *)handle;

    uint8_t* deal_ptr = (uint8_t *)buffer;
    uint32_t packet_len = 0;

    while (data_length > frame->cfg.header_length) {
        uint8_t i;
        for (i = 0; i < frame->cfg.header_length; i++) {
            if (deal_ptr[i] != frame->cfg.header[i]) {
                break;
            }
        }
        if (i == frame->cfg.header_length) {
            break ;
        }
        deal_ptr += 1;
        data_length -= 1;
    }

    // not recv all data
    if (data_length < frame->length_min) {
        goto exit;
    }
    
    for (uint8_t i = 0; i < frame->cfg.len_length; i++) {
        packet_len |= packet_len << 8 | deal_ptr[frame->len_offset + i];
    }
    packet_len += frame->len_value_complete;

    if (packet_len > frame->length_max || packet_len < frame->length_min) {
        // skip error length, find next header
        packet_len = frame->cfg.header_length;
        goto exit;
    }

    // not recv all packet, skip now
    if (data_length < packet_len) {
        packet_len = 0;
        goto exit;
    }
    
    uint32_t crc_value = 0x0;
    for (uint8_t i = 0; i < frame->cfg.crc_length; i++) {
        crc_value = crc_value << 8;
        crc_value |= deal_ptr[packet_len - frame->cfg.crc_length + i];
    }
    uint32_t crc_result = frame->driver.frame_crc_check(0, deal_ptr + frame->crc_offset, packet_len - frame->cfg.crc_length - frame->crc_offset);
    if (crc_result != crc_value) {
        printf("uart crc failed: hope: %04lx, but: %04lx\r\n", crc_value, crc_result);
        goto exit;
    }

    if (frame->driver.frame_callback) {
        uint32_t cmd = 0;
        for (uint8_t i = 0; i < frame->cfg.cmd_length; i++) {
            cmd = cmd << 8;
            cmd |= deal_ptr[frame->cmd_offset + i];
        }
        frame->driver.frame_callback(cmd, deal_ptr + frame->data_offset, packet_len - frame->length_min);
    }

exit:
    deal_ptr += packet_len;
    return deal_ptr - buffer;
}

void zx_frame_update(zx_frame_handle_t handle) {
    zx_uart_frame_t* frame = (zx_uart_frame_t *)handle;

    uint16_t frame_recv_size = 0;
    uint32_t used_size = 0;

    // only check available data
    uint32_t read_size = frame->driver.frame_read(frame->frame + frame->length, &frame_recv_size, frame->cfg.user_data);
    frame->length += read_size;

    if (frame->driver.frame_get_time_ms) {
        uint64_t time_ms = frame->driver.frame_get_time_ms(frame->cfg.user_data);
        if (time_ms - frame->last_frame_time_ms > frame->cfg.timeout_ms) {
            // skip timeout data
            frame->length = 0;
        }
        frame->last_frame_time_ms = time_ms;
    }

    while (frame_recv_size > 0) {
        if (frame->length + frame_recv_size > frame->length_max) {
            frame_recv_size = frame->length_max - frame->length;
        }
        read_size = frame->driver.frame_read(frame->frame + frame->length, &frame_recv_size, frame->cfg.user_data);
        frame->length += read_size;

        uint32_t total_used = 0;
        do {
            used_size = deal_uart_buffer(handle, frame->frame + total_used, frame->length);
            frame->length -= used_size;
            total_used += used_size;
        } while ((frame->length > 1 && used_size != 0));
        if (frame->cfg.timeout_ms == 0) {
            frame->length = 0;
        }
        memmove(frame->frame, frame->frame + total_used, frame->length);
    }
}
