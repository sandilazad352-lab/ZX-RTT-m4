/* 
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef __AIC_DRV_HRTIMER_H__
#define __AIC_DRV_HRTIMER_H__

#include "aic_common.h"

struct hrtimer_info {
    char name[12];
    u32 id;
    rt_hwtimer_t hrtimer;
};

#endif
