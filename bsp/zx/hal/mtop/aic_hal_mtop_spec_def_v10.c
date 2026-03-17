/*
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */
#include <string.h>
#include <stdint.h>
#include <aic_core.h>

uint8_t group_id[] = {0, 1};
uint8_t port_id[] = {0, 1, 2, 3};

const char * grp_name[] = {"AXI", "AHB"};
const char * prt_name[] = {"CPU", "DMA", "DE", "GVD"};