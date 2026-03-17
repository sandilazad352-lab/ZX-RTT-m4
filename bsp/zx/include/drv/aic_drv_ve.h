/*
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#include "aic_hal_ve.h"

struct aic_ve_client *drv_ve_open(void);
int drv_ve_close(struct aic_ve_client *client);
int drv_ve_control(struct aic_ve_client *client, int cmd, void *arg);
