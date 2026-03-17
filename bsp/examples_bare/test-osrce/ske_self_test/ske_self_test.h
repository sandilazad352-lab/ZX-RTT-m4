#ifndef SKE_SELF_TEST_H
#define SKE_SELF_TEST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <aic_core.h>
#include <stdint.h>    //including int32_t definition

u32 ske_basic_mode_self_test(void);
u32 ske_xts_self_test(void);
u32 ske_cmac_self_test(void);
u32 ske_cbc_mac_self_test(void);
u32 ske_ccm_self_test(void);
u32 ske_gcm_gmac_self_test(void);
u32 ske_aes_xcbc_mac_96_self_test(void);

#ifdef __cplusplus
}
#endif

#endif
