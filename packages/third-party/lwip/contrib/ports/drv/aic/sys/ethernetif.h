/*
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __ZX_ETHERNETIF_H__
#define __ZX_ETHERNETIF_H__

#include "lwip/err.h"
#include "lwip/netif.h"

err_t ethernetif_init(struct netif *netif);
err_t ethernetif_input(struct netif *netif);
void ethernetif_input_poll(void);

#endif
