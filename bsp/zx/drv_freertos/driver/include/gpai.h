#ifndef __ZX_GPAI_DRIVER_H__
#define __ZX_GPAI_DRIVER_H__

#include "hal_gpai.h"

#define GPAI_NUM_MAX (12)

/**
 * @brief Initializes the General Purpose Analog Input (GPAI) system.
 * 
 * This function prepares the GPAI hardware for use, setting up necessary clocks,
 * interrupt handlers, and initial configurations. It should be called before
 * any other GPAI functions are used.
 * 
 * @return int Returns 0 on successful initialization, -1 on failure.
 */
int gpai_init(void);

/**
 * @brief Enables or disables a GPAI channel with specified configurations.
 * 
 * This function is used to configure and change the state of a GPAI channel. It can
 * enable or disable the channel, set its sampling mode, and define its sampling period.
 * 
 * @param ch The channel number to be configured.
 * @param enabled 1 to enable the channel, 0 to disable it.
 * @param mode The sampling mode for the channel (e.g., AIC_GPAI_MODE_SINGLE).
 * @param period_ms The sampling period in milliseconds.
 * @return int Returns 0 on success, -1 if the specified channel cannot be found or configured.
 */
int gpai_enabled(uint32_t ch, uint8_t enabled, enum aic_gpai_mode mode, uint32_t period_ms);

/**
 * @brief Reads the millivolt value from the specified GPAI channel.
 * 
 * This function reads the current analog value from the specified GPAI channel
 * and converts it into a millivolt value based on the configured reference voltage
 * and resolution.
 * 
 * @param ch The channel number from which to read the analog value.
 * @return int The read value in millivolts. Returns 0 if the channel is not available or on failure.
 */
int gpai_read_mv(uint32_t ch);

/**
 * @brief Gets the resolution of the GPAI system.
 * 
 * This function returns the resolution of the GPAI system, which is the number
 * of bits used to represent the analog values.
 * 
 * @return int The resolution of the GPAI system in bits.
 */
int gpai_get_resolution();

#endif // __ZX_GPAI_DRIVER_H__
