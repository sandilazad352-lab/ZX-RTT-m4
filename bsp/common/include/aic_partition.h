/*
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <aic_common.h>

#ifndef _AIC_PARTITION_H_
#define _AIC_PARTITION_H_

#define MAX_PARTITION_NAME 144
struct aic_partition {
    char name[MAX_PARTITION_NAME];
    u64 start;
    u64 size;
    struct aic_partition *next;
};

struct aic_partition *aic_part_mtd_parse(char *parts);
struct aic_partition *aic_part_gpt_parse(char *parts);
struct aic_partition *aic_part_get_byname(struct aic_partition *head, char *name);
void aic_part_free(struct aic_partition *head);
void aic_part_dump(struct aic_partition *head);

#endif
