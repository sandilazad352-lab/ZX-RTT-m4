#ifndef __ZX_UTILS_H__
#define __ZX_UTILS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "aic_osal.h"

typedef enum {
    ZX_OK = 0x00,
    ZX_FAIL,
    ZX_INVALID_ARG,
    ZX_MALLOC_FAIL,
    ZX_TIMEOUT,
} ZX_t;

#define ZX_PARAM_CHECK(con) do { \
        if (!(con)) { \
            return ZX_INVALID_ARG; \
        } \
    } while(0)

#ifndef pdTICKS_TO_MS
    #define pdTICKS_TO_MS( xTicks )       ( ( TickType_t ) ( ( uint64_t ) ( xTicks ) * 1000 / configTICK_RATE_HZ ) )
#endif

#define ZX_MALLOC malloc

void freertos_heap_info_print();

void psram_heap_info_print();

#ifdef __cplusplus
}
#endif

#endif