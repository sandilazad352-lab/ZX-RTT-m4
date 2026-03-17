#ifndef TEST_H
#define TEST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <aic_common.h>    //including int32_t definition
#include <hal_pke.h>    //including int32_t definition
#include <stdint.h>    //including int32_t definition

extern u32 startP(void);
extern u32 endP(u8 mode, u32 once_bytes, u32 round);
u32 crypto_test(void);
u32 pke_hal_test(void);
u32 DH_all_test(void);
u32 ECCp_get_curve_paras_test(void);
u32 ECCp_get_key_all_test(void);
u32 ECDH_all_test(void);
u32 ECDSA_all_test(void);
u32 ECIES_all_test(void);
u32 RSA_all_test(void);
u32 SM2_all_test(void);
u32 SKE_all_test(void);
u32 pbkdf2_hmac_test(void);
u32 ansi_x9_63_kdf_all_test(void);
u32 HASH_all_test(void);
u32 HMAC_all_test(void);
u32 HASH_HMAC_sample_test(void);
u32 X25519_all_test(void);
u32 Ed25519_all_test(void);
u32 SM9_all_test(void);
u8 TRNG_test();

#ifdef __cplusplus
}
#endif

#endif

