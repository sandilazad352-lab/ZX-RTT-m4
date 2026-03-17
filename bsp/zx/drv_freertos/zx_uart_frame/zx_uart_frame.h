#pragma once

#include "zx_frame.h"
#include "zx_uart.h"

zx_frame_handle_t zx_uart_frame_init(uart_num_t num, zx_frame_config_t* config, frame_callback_t cb);
