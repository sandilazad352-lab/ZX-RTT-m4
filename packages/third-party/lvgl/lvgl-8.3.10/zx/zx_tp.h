/*
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef ZX_TP
#define ZX_TP

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __RTTHREAD__
#include <rtthread.h>
#include <rtdevice.h>
#endif

#ifdef KERNEL_FREERTOS
#include "touch_freertos.h"
#endif

int tpc_run(const char *name);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ZX_TP */
