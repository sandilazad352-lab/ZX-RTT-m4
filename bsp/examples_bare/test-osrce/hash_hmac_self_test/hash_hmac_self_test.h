#ifndef HASH_HMAC_SELF_TEST_H
#define HASH_HMAC_SELF_TEST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>    //including int32_t definition

uint32_t hash_self_test(void);
uint32_t hmac_self_test(void);

#ifdef __cplusplus
}
#endif

#endif
