#include <stdio.h>
#include <stdint.h>

#include <hash.h>

u32 sample_test_sm3(void);
u32 sample_test_hmac_sm3(void);
u32 sample_test_md5(void);
u32 sample_test_hmac_md5(void);
u32 sample_test_sha256(void);
u32 sample_test_hmac_sha256(void);
u32 sample_test_sha384(void);
u32 sample_test_hmac_sha384(void);
u32 sample_test_sha512(void);
u32 sample_test_hmac_sha512(void);
u32 sample_test_sha1(void);
u32 sample_test_hmac_sha1(void);
u32 sample_test_sha224(void);
u32 sample_test_hmac_sha224(void);
u32 sample_test_sha512_224(void);
u32 sample_test_hmac_sha512_224(void);
u32 sample_test_sha512_256(void);
u32 sample_test_hmac_sha512_256(void);
//u32 sample_test_sha3_224(void);
//u32 sample_test_sha3_256(void);
//u32 sample_test_sha3_384(void);
//u32 sample_test_sha3_512(void);

u32 HASH_HMAC_sample_test(void)
{
    printf("\r\n\r\n\r\n =================== HASH HMAC sample test =================== ");


#ifdef AIC_HASH_SM3_SUPPORT
    if(sample_test_sm3())
        return 1;

    if(sample_test_hmac_sm3())
        return 1;
#endif

#ifdef SUPPORT_HASH_MD5
    if(sample_test_md5())
        return 1;

    if(sample_test_hmac_md5())
        return 1;
#endif

#ifdef AIC_HASH_SHA256_SUPPORT
    if(sample_test_sha256())
        return 1;

    if(sample_test_hmac_sha256())
        return 1;
#endif

#ifdef SUPPORT_HASH_SHA384
    if(sample_test_sha384())
        return 1;

    if(sample_test_hmac_sha384())
        return 1;
#endif

#ifdef SUPPORT_HASH_SHA512
    if(sample_test_sha512())
        return 1;

    if(sample_test_hmac_sha512())
        return 1;
#endif

#ifdef AIC_HASH_SHA1_SUPPORT
    if(sample_test_sha1())
        return 1;

    if(sample_test_hmac_sha1())
        return 1;
#endif

#ifdef AIC_HASH_SHA224_SUPPORT
    if(sample_test_sha224())
        return 1;

    if(sample_test_hmac_sha224())
        return 1;
#endif

#ifdef SUPPORT_HASH_SHA512_224
    if(sample_test_sha512_224())
        return 1;

    if(sample_test_hmac_sha512_224())
        return 1;
#endif

#ifdef SUPPORT_HASH_SHA512_256
    if(sample_test_sha512_256())
        return 1;

    if(sample_test_hmac_sha512_256())
        return 1;
#endif

#ifdef SUPPORT_HASH_SHA3_224
    if(sample_test_sha3_224())
        return 1;
#endif

#ifdef SUPPORT_HASH_SHA3_256
    if(sample_test_sha3_256())
        return 1;
#endif

#ifdef SUPPORT_HASH_SHA3_384
    if(sample_test_sha3_384())
        return 1;
#endif

#ifdef SUPPORT_HASH_SHA3_512
    if(sample_test_sha3_512())
        return 1;
#endif

    printf("\r\n\r\n HASH HMAC sample test success \r\n\r\n");

    return 0;
}

