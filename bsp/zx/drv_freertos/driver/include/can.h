#ifndef __ZX_CAN_DRIVER_H__
#define __ZX_CAN_DRIVER_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <aic_common.h>
#include <aic_errno.h>
#include <aic_hal_can.h>
#include <aic_osal.h>

/**
 * @brief Structure representing a CAN handle.
 *
 * This structure wraps a CAN handle and a queue for received messages,
 * allowing for interaction with the CAN peripheral.
 */
typedef struct {
    can_handle can_handle;      ///< The CAN handle.
    aicos_queue_t recv_queue;   ///< Queue for received CAN messages.
} can_handle_t;

/**
 * @brief Configuration structure for initializing a CAN device.
 *
 * Holds the configuration parameters for setting up a CAN device,
 * including baud rate, mode of operation, and receive queue length.
 */
typedef struct {
    uint32_t baudrate;     ///< CAN communication baud rate.
    can_mode_t mode;       ///< CAN mode of operation.
    uint8_t rx_queue_len;  ///< Length of the receive message queue.
} can_config_t;

/**
 * @brief Enumeration of CAN device indices.
 *
 * Defines identifiers for different CAN peripherals available on the device.
 */
typedef enum {
    CAN_0 = 0x0,  ///< Identifier for the first CAN device.
    CAN_1,        ///< Identifier for the second CAN device.
    CAN_MAX,      ///< Total number of CAN devices.
} can_index_t;

/**
 * @brief Initialize a CAN device.
 *
 * Allocates and initializes a CAN device handle according to the specified
 * configuration. It sets up the baud rate, mode, and receive queue for the device.
 *
 * @param index The index of the CAN device to initialize.
 * @param config Pointer to the configuration structure for the CAN device.
 * @return Pointer to the initialized can_handle_t structure, or NULL on failure.
 */
can_handle_t* can_init(can_index_t index, can_config_t* config);

/**
 * @brief Write a CAN frame.
 *
 * Sends a CAN message frame using the specified CAN handle.
 *
 * @param handle Pointer to the CAN handle.
 * @param msg Pointer to the CAN message to send.
 * @param req Operation request type for the CAN frame transmission.
 * @return 0 on success, -1 on failure.
 */
int can_write_frame(can_handle_t* handle, can_msg_t* msg, can_op_req_t req);

/**
 * @brief Set a CAN filter.
 *
 * Configures a message filter for a CAN device to specify which messages
 * should be received.
 *
 * @param handle Pointer to the CAN handle.
 * @param filter Pointer to the filter configuration structure.
 * @return 0 on success, -1 on failure.
 */
int can_set_filter(can_handle_t* handle, can_filter_config_t* filter);

/**
 * @brief Get the status of the CAN device.
 *
 * Retrieves the current status of the CAN device, such as error states and
 * operational mode.
 *
 * @param handle Pointer to the CAN handle.
 * @param status Pointer to a structure where the status will be stored.
 * @return 0 on success, -1 on failure.
 */
int can_get_status(can_handle_t* handle, can_status_t* status);

/**
 * @brief Receive a CAN message.
 *
 * Attempts to receive a CAN message within the specified timeout period.
 *
 * @param handle Pointer to the CAN handle.
 * @param msg Pointer to the structure where the received message will be stored.
 * @param timeout_ms Timeout for the receive operation in milliseconds.
 * @return 0 on success, -1 on failure.
 */
int can_recv_msg(can_handle_t* handle, can_msg_t* msg, uint32_t timeout_ms);

#endif // __ZX_CAN_DRIVER_H__
