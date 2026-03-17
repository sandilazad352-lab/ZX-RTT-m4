#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <aic_common.h>
#include <aic_errno.h>
#include <aic_hal_can.h>
#include <aic_osal.h>
#include "can.h"

static void can_rx_callback(can_handle * phandle, void *arg) {
    if ((u32)arg == CAN_EVENT_RX_IND) {
        can_handle_t* handle = container_of(phandle, can_handle_t, can_handle);
        aicos_queue_send(handle->recv_queue, &phandle->msg);
    }
}

can_handle_t* can_init(can_index_t index, can_config_t* config) {
    if (index >= CAN_MAX) {
        return NULL;
    }
    can_handle_t* handle = (can_handle_t *)malloc(sizeof(can_handle_t));
    handle->recv_queue = aicos_queue_create(sizeof(can_msg_t), config->rx_queue_len);
    hal_can_init(&handle->can_handle, 0);
    hal_can_ioctl(&handle->can_handle, CAN_IOCTL_SET_BAUDRATE, (void *)config->baudrate);
    hal_can_ioctl(&handle->can_handle, CAN_IOCTL_SET_MODE, (void *)config->mode);
    if (index == CAN_0) {
        aicos_request_irq(CAN0_IRQn, hal_can_isr_handler, 0, NULL, (void *)&handle->can_handle);
    } else {
        aicos_request_irq(CAN1_IRQn, hal_can_isr_handler, 0, NULL, (void *)&handle->can_handle);
    }
    hal_can_enable_interrupt(&handle->can_handle);
    hal_can_attach_callback(&handle->can_handle, can_rx_callback, &handle->recv_queue);
    return handle;
}

int can_write_frame(can_handle_t* handle, can_msg_t* msg, can_op_req_t req) {
    if (handle == NULL) {
        return -1;
    }
    hal_can_send_frame(&handle->can_handle, msg, req);
    return 0;
}

int can_set_filter(can_handle_t* handle, can_filter_config_t* filter) {
    if (handle == NULL) {
        return -1;
    }
    hal_can_ioctl(&handle->can_handle, CAN_IOCTL_SET_FILTER, (void *)filter);
    return 0;
}

int can_get_status(can_handle_t* handle, can_status_t* status) {
    if (handle == NULL) {
        return -1;
    }
    *status = handle->can_handle.status;
    return 0;
}

int can_recv_msg(can_handle_t* handle, can_msg_t* msg, uint32_t timeout_ms) {
    if (handle == NULL) {
        return -1;
    }
    return aicos_queue_receive(handle->recv_queue, msg, timeout_ms);
}
