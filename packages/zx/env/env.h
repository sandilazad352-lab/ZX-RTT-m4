/*
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef __ENV_H__
#define __ENV_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <boot_param.h>

int fw_env_open(void);
int fw_env_close(void);
int fw_env_flush(void);
int fw_env_write(char *name, char *value);
char *fw_getenv(char *name);

#ifdef __cplusplus
}
#endif

#endif
