#include <stdio.h>
#include "test.h"

u32 crypto_test(void)
{
#if 0
    if(pke_hal_test())
        return 1;
#endif

#if 1
    if(SKE_all_test())
        return 1;
#endif

#if 1
    if(pbkdf2_hmac_test())
        return 1;
#endif

#if 1
    if(ansi_x9_63_kdf_all_test())
        return 1;
#endif

#if 1
    if(HASH_all_test())
        return 1;
#endif

#if 1
    if(HMAC_all_test())
        return 1;
#endif

#if 1
    if(HASH_HMAC_sample_test())
        return 1;
#endif

#if 0
    if(DH_all_test())
        return 1;
#endif

#if 1
    if(SM2_all_test())
        return 1;
#endif

#if 0
    if(ECCp_get_curve_paras_test())
        return 1;
#endif

#if 0
    if(ECCp_get_key_all_test())
        return 1;
#endif

#if 0
    if(ECDH_all_test())
        return 1;
#endif

#if 0
    if(ECDSA_all_test())
        return 1;
#endif

#if 0
    if(ECIES_all_test())
        return 1;
#endif

#if 1
    if(RSA_all_test())
        return 1;
#endif

#if 0
    if(X25519_all_test())
        return 1;
#endif

#if 0
    if(Ed25519_all_test())
        return 1;
#endif

#if 0
    if(SM9_all_test())
        return 1;
#endif

    printf("\r\n TEST OVER \r\n");//fflush(stdout);
    return 0;
}

