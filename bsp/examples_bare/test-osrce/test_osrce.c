/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <console.h>
#include "test.h"

static void cmd_test_osrce_help(void)
{
    printf("test_lpce command usage:\n");
    printf("  test_osrce help : Get this help.\n");
#ifdef AIC_OSR_CE_SKE_BARE_TEST
    printf("  test_osrce ske : Test ske algorithms.\n");
#endif
#ifdef AIC_OSR_CE_PKE_BARE_TEST
    printf("  test_osrce pke : Test pke algorithms.\n");
#endif
#ifdef AIC_OSR_CE_HASH_BARE_TEST
    printf("  test_osrce hash : Test hash algorithms.\n");
#endif
#ifdef AIC_OSR_CE_TRNG_BARE_TEST
    printf("  test_osrce trng : Test trng algorithms.\n");
#endif
#if defined(AIC_OSR_CE_SKE_BARE_TEST) && defined(AIC_OSR_CE_PKE_BARE_TEST) && \
    defined(AIC_OSR_CE_HASH_BARE_TEST) && defined(AIC_OSR_CE_TRNG_BARE_TEST)
    printf("  test_osrce all  : Test all algorithms.\n");
    printf("  test_osrce stability  : Test all algorithms stability.\n");
#endif
}

static int cmd_test_osrce_do(int argc, char **argv)
{
    if (argc < 2) {
        return -1;
    }
#ifdef AIC_OSR_CE_HASH_BARE_TEST
    if (!strcmp(argv[1], "hash")) {
        HASH_all_test();
        HMAC_all_test();
        HASH_HMAC_sample_test();
#ifdef AIC_HASH_PBKDF2
        pbkdf2_hmac_test();
#endif
#ifdef AIC_HASH_ANSI_X9_63_KDF
        ansi_x9_63_kdf_all_test();
#endif
        return 0;
    }
#endif
#ifdef AIC_OSR_CE_SKE_BARE_TEST
    if (!strcmp(argv[1], "ske")) {
        SKE_all_test();
        return 0;
    }
#endif
#ifdef AIC_OSR_CE_PKE_BARE_TEST
    if (!strcmp(argv[1], "pke")) {
        pke_init();
#ifdef AIC_PKE_ECDH_SUPPORT
        ECCp_get_curve_paras_test();
        ECCp_get_key_all_test();
#endif
#ifdef AIC_PKE_RSA_SUPPORT
        RSA_all_test();
#endif
#ifdef AIC_PKE_SM2_SUPPORT
        SM2_all_test();
#endif
        return 0;
    }
#endif
#ifdef AIC_OSR_CE_TRNG_BARE_TEST
    if (!strcmp(argv[1], "trng")) {
        TRNG_test();
        return 0;
    }
#endif
#if defined(AIC_OSR_CE_SKE_BARE_TEST) && defined(AIC_OSR_CE_PKE_BARE_TEST) && \
    defined(AIC_OSR_CE_HASH_BARE_TEST) && defined(AIC_OSR_CE_TRNG_BARE_TEST)
    if (!strcmp(argv[1], "all")) {
        //crypto_test();
        return 0;
    }
    if (!strcmp(argv[1], "stability")) {
        while(1) {
            if (console_get_ctrlc())
                break;
            //crypto_test();
        }
        return 0;
    }
#endif

    cmd_test_osrce_help();
    return 0;
}
CONSOLE_CMD(test_osrce, cmd_test_osrce_do, "test osr ce example");
