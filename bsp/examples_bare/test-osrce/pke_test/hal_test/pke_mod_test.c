#include <stdio.h>

#include "../../../crypto_hal/pke.h"
#include "../../../crypto_include/crypto_common/utility.h"
#include "../../../crypto_include/trng/trng.h"

typedef struct
{
    uint32_t modulus_bitLen;
    uint32_t *b;
    uint32_t *small;
    uint32_t *big;
    uint32_t *std_big;
    uint32_t *std_tiny_small;
    uint32_t *std_tiny_equal;
    uint32_t *std_tiny_big;
    uint32_t *std_small_small;
    uint32_t *std_small_equal;
    uint32_t *std_small_big;
    uint32_t *std_big_small;
    uint32_t *std_big_equal;
    uint32_t *std_big_big;
} pke_mod_test_vector_t;


uint32_t pke_mod_test(pke_mod_test_vector_t *vector)
{
    uint32_t t1[OPERAND_MAX_WORD_LEN<<1];
    uint32_t t2[OPERAND_MAX_WORD_LEN];

    uint32_t wordLen = GET_WORD_LEN(vector->modulus_bitLen);
    uint32_t tmpBitLen = (vector->modulus_bitLen)&31;
    uint32_t ret = 0;

    //zero
    uint32_clear(t1, wordLen);
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    ret = pke_mod(t1, wordLen, vector->b, NULL, NULL, wordLen, t2);
#else
    ret = pke_mod(t1, wordLen, vector->b, NULL, wordLen, t2);
#endif
    if(PKE_SUCCESS != ret || 0 == uint32_BigNum_Check_Zero(t2, wordLen))
    {
        printf("\r\n %u pke_mod(input zero) failure, ret=%x", vector->modulus_bitLen, ret);
        print_BN_buf_U32(t1, wordLen, "t1");
        print_BN_buf_U32(t2, wordLen, "t2");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_mod(input zero) success", vector->modulus_bitLen);
    }

    //tiny
    pke_set_operand_uint32_value(t1, wordLen, vector->b[0]);
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    ret = pke_mod(t1, 1, vector->b, NULL, NULL, wordLen, t2);
#else
    ret = pke_mod(t1, 1, vector->b, NULL, wordLen, t2);
#endif
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t2, wordLen, t1, wordLen))
    {
        printf("\r\n %u pke_mod(input tiny) failure, ret=%x", vector->modulus_bitLen, ret);
        print_BN_buf_U32(t1, wordLen, "t1");
        print_BN_buf_U32(t2, wordLen, "t2");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_mod(input tiny) success", vector->modulus_bitLen);
    }

    //small
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    ret = pke_mod(vector->small, wordLen, vector->b, NULL, NULL, wordLen, t2);
#else
    ret = pke_mod(vector->small, wordLen, vector->b, NULL, wordLen, t2);
#endif
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t2, wordLen, vector->small, wordLen))
    {
        printf("\r\n %u pke_mod(input small) failure, ret=%x", vector->modulus_bitLen, ret);
        print_BN_buf_U32(t1, wordLen, "t1");
        print_BN_buf_U32(t2, wordLen, "t2");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_mod(input small) success", vector->modulus_bitLen);
    }

    //equal
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    ret = pke_mod(vector->b, wordLen, vector->b, NULL, NULL, wordLen, t2);
#else
    ret = pke_mod(vector->b, wordLen, vector->b, NULL, wordLen, t2);
#endif
    if(PKE_SUCCESS != ret || 0 == uint32_BigNum_Check_Zero(t2, wordLen))
    {
        printf("\r\n %u pke_mod(input equal) failure, ret=%x", vector->modulus_bitLen, ret);
        print_BN_buf_U32(t2, wordLen, "t2");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_mod(input equal) success", vector->modulus_bitLen);
    }

    //big
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    ret = pke_mod(vector->big, wordLen, vector->b, NULL, NULL, wordLen, t2);
#else
    ret = pke_mod(vector->big, wordLen, vector->b, NULL, wordLen, t2);
#endif
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t2, wordLen, vector->std_big, wordLen))
    {
        printf("\r\n %u pke_mod(input big) failure, ret=%x", vector->modulus_bitLen, ret);
        print_BN_buf_U32(vector->big, wordLen, "input");
        print_BN_buf_U32(vector->b, wordLen, "modulus");
        print_BN_buf_U32(t2, wordLen, "t2");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_mod(input big) success", vector->modulus_bitLen);
    }

    //tiny_small
    uint32_copy(t1, vector->small, wordLen);
    uint32_copy(t1+wordLen, vector->small, 1);
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    ret = pke_mod(t1, wordLen+1, vector->b, NULL, NULL, wordLen, t2);
#else
    ret = pke_mod(t1, wordLen+1, vector->b, NULL, wordLen, t2);
#endif
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t2, wordLen, vector->std_tiny_small, wordLen))
    {
        printf("\r\n %u pke_mod(input tiny_small) failure, ret=%x", vector->modulus_bitLen, ret);
        print_BN_buf_U32(t1, wordLen+1, "t1");
        print_BN_buf_U32(t2, wordLen, "t2");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_mod(input tiny_small) success", vector->modulus_bitLen);
    }

    //tiny_equal
    uint32_copy(t1, vector->b, wordLen);
    uint32_copy(t1+wordLen, vector->small, 1);
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    ret = pke_mod(t1, wordLen+1, vector->b, NULL, NULL, wordLen, t2);
#else
    ret = pke_mod(t1, wordLen+1, vector->b, NULL, wordLen, t2);
#endif
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t2, wordLen, vector->std_tiny_equal, wordLen))
    {
        printf("\r\n %u pke_mod(input tiny_equal) failure, ret=%x", vector->modulus_bitLen, ret);
        print_BN_buf_U32(t1, wordLen+1, "t1");
        print_BN_buf_U32(t2, wordLen, "t2");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_mod(input tiny_equal) success", vector->modulus_bitLen);
    }

    //tiny_big
    uint32_copy(t1, vector->big, wordLen);
    uint32_copy(t1+wordLen, vector->small, 1);
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    ret = pke_mod(t1, wordLen+1, vector->b, NULL, NULL, wordLen, t2);
#else
    ret = pke_mod(t1, wordLen+1, vector->b, NULL, wordLen, t2);
#endif
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t2, wordLen, vector->std_tiny_big, wordLen))
    {
        printf("\r\n %u pke_mod(input tiny_big) failure, ret=%x", vector->modulus_bitLen, ret);
        print_BN_buf_U32(t1, wordLen+1, "t1");
        print_BN_buf_U32(t2, wordLen, "t2");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_mod(input tiny_big) success", vector->modulus_bitLen);
    }

    //small_small
    uint32_copy(t1, vector->small, wordLen);
    uint32_copy(t1+wordLen, vector->small, wordLen);
    if(tmpBitLen)
    {
        t1[wordLen-1] |= (vector->small[0])<<tmpBitLen;
        Big_Div2n(t1+wordLen, wordLen,32-tmpBitLen);
    }
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    ret = pke_mod(t1, get_valid_words(t1, wordLen<<1), vector->b, NULL, NULL, wordLen, t2);
#else
    ret = pke_mod(t1, get_valid_words(t1, wordLen<<1), vector->b, NULL, wordLen, t2);
#endif
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t2, wordLen, vector->std_small_small, wordLen))
    {
        printf("\r\n %u pke_mod(input small_small) failure, ret=%x", vector->modulus_bitLen, ret);
        print_BN_buf_U32(t1, wordLen<<1, "t1");
        print_BN_buf_U32(t2, wordLen, "t2");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_mod(input small_small) success", vector->modulus_bitLen);
    }

    //small_equal
    uint32_copy(t1, vector->b, wordLen);
    uint32_copy(t1+wordLen, vector->small, wordLen);
    if(tmpBitLen)
    {
        t1[wordLen-1] |= (vector->small[0])<<tmpBitLen;
        Big_Div2n(t1+wordLen, wordLen,32-tmpBitLen);
    }
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    ret = pke_mod(t1, get_valid_words(t1, wordLen<<1), vector->b, NULL, NULL, wordLen, t2);
#else
    ret = pke_mod(t1, get_valid_words(t1, wordLen<<1), vector->b, NULL, wordLen, t2);
#endif
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t2, wordLen, vector->std_small_equal, wordLen))
    {
        printf("\r\n %u pke_mod(input small_equal) failure, ret=%x", vector->modulus_bitLen, ret);
        print_BN_buf_U32(t1, wordLen<<1, "t1");
        print_BN_buf_U32(t2, wordLen, "t2");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_mod(input small_equal) success", vector->modulus_bitLen);
    }

    //small_big
    uint32_copy(t1, vector->big, wordLen);
    uint32_copy(t1+wordLen, vector->small, wordLen);
    if(tmpBitLen)
    {
        t1[wordLen-1] |= (vector->small[0])<<tmpBitLen;
        Big_Div2n(t1+wordLen, wordLen,32-tmpBitLen);
    }
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    ret = pke_mod(t1, get_valid_words(t1, wordLen<<1), vector->b, NULL, NULL, wordLen, t2);
#else
    ret = pke_mod(t1, get_valid_words(t1, wordLen<<1), vector->b, NULL, wordLen, t2);
#endif
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t2, wordLen, vector->std_small_big, wordLen))
    {
        printf("\r\n %u pke_mod(input small_big) failure, ret=%x", vector->modulus_bitLen, ret);
        print_BN_buf_U32(t1, wordLen<<1, "t1");
        print_BN_buf_U32(t2, wordLen, "t2");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_mod(input small_big) success", vector->modulus_bitLen);
    }

    //equal_small
    uint32_copy(t1, vector->small, wordLen);
    uint32_copy(t1+wordLen, vector->b, wordLen);
    if(tmpBitLen)
    {
        t1[wordLen-1] |= (vector->b[0])<<tmpBitLen;
        Big_Div2n(t1+wordLen, wordLen,32-tmpBitLen);
    }
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    ret = pke_mod(t1, get_valid_words(t1, wordLen<<1), vector->b, NULL, NULL, wordLen, t2);
#else
    ret = pke_mod(t1, get_valid_words(t1, wordLen<<1), vector->b, NULL, wordLen, t2);
#endif
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t2, wordLen, vector->small, wordLen))
    {
        printf("\r\n %u pke_mod(input equal_small) failure, ret=%x", vector->modulus_bitLen, ret);
        print_BN_buf_U32(t1, wordLen<<1, "t1");
        print_BN_buf_U32(t2, wordLen, "t2");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_mod(input equal_small) success", vector->modulus_bitLen);
    }


    //equal_equal
    uint32_copy(t1, vector->b, wordLen);
    uint32_copy(t1+wordLen, vector->b, wordLen);
    if(tmpBitLen)
    {
        t1[wordLen-1] |= (vector->b[0])<<tmpBitLen;
        Big_Div2n(t1+wordLen, wordLen,32-tmpBitLen);
    }
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    ret = pke_mod(t1, get_valid_words(t1, wordLen<<1), vector->b, NULL, NULL, wordLen, t2);
#else
    ret = pke_mod(t1, get_valid_words(t1, wordLen<<1), vector->b, NULL, wordLen, t2);
#endif
    if(PKE_SUCCESS != ret || 0 == uint32_BigNum_Check_Zero(t2, wordLen))
    {
        printf("\r\n %u pke_mod(input equal_equal) failure, ret=%x", vector->modulus_bitLen, ret);
        print_BN_buf_U32(t1, wordLen<<1, "t1");
        print_BN_buf_U32(t2, wordLen, "t2");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_mod(input equal_equal) success", vector->modulus_bitLen);
    }


    //equal_big
    uint32_copy(t1, vector->big, wordLen);
    uint32_copy(t1+wordLen, vector->b, wordLen);
    if(tmpBitLen)
    {
        t1[wordLen-1] |= (vector->b[0])<<tmpBitLen;
        Big_Div2n(t1+wordLen, wordLen,32-tmpBitLen);
    }
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    ret = pke_mod(t1, get_valid_words(t1, wordLen<<1), vector->b, NULL, NULL, wordLen, t2);
#else
    ret = pke_mod(t1, get_valid_words(t1, wordLen<<1), vector->b, NULL, wordLen, t2);
#endif
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t2, wordLen, vector->std_big, wordLen))
    {
        printf("\r\n %u pke_mod(input equal_big) failure, ret=%x", vector->modulus_bitLen, ret);
        print_BN_buf_U32(t1, wordLen<<1, "t1");
        print_BN_buf_U32(t2, wordLen, "t2");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_mod(input equal_big) success", vector->modulus_bitLen);
    }


    //big_small
    uint32_copy(t1, vector->small, wordLen);
    uint32_copy(t1+wordLen, vector->big, wordLen);
    if(tmpBitLen)
    {
        t1[wordLen-1] |= (vector->big[0])<<tmpBitLen;
        Big_Div2n(t1+wordLen, wordLen,32-tmpBitLen);
    }
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    ret = pke_mod(t1, get_valid_words(t1, wordLen<<1), vector->b, NULL, NULL, wordLen, t2);
#else
    ret = pke_mod(t1, get_valid_words(t1, wordLen<<1), vector->b, NULL, wordLen, t2);
#endif
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t2, wordLen, vector->std_big_small, wordLen))
    {
        printf("\r\n %u pke_mod(input big_small) failure, ret=%x", vector->modulus_bitLen, ret);
        print_BN_buf_U32(t1, wordLen<<1, "t1");
        print_BN_buf_U32(t2, wordLen, "t2");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_mod(input big_small) success", vector->modulus_bitLen);
    }


    //big_equal
    uint32_copy(t1, vector->b, wordLen);
    uint32_copy(t1+wordLen, vector->big, wordLen);
    if(tmpBitLen)
    {
        t1[wordLen-1] |= (vector->big[0])<<tmpBitLen;
        Big_Div2n(t1+wordLen, wordLen,32-tmpBitLen);
    }
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    ret = pke_mod(t1, get_valid_words(t1, wordLen<<1), vector->b, NULL, NULL, wordLen, t2);
#else
    ret = pke_mod(t1, get_valid_words(t1, wordLen<<1), vector->b, NULL, wordLen, t2);
#endif
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t2, wordLen, vector->std_big_equal, wordLen))
    {
        printf("\r\n %u pke_mod(input big_equal) failure, ret=%x", vector->modulus_bitLen, ret);
        print_BN_buf_U32(t1, wordLen<<1, "t1");
        print_BN_buf_U32(t2, wordLen, "t2");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_mod(input big_equal) success", vector->modulus_bitLen);
    }


    //big_big
    uint32_copy(t1, vector->big, wordLen);
    uint32_copy(t1+wordLen, vector->big, wordLen);
    if(tmpBitLen)
    {
        t1[wordLen-1] |= (vector->big[0])<<tmpBitLen;
        Big_Div2n(t1+wordLen, wordLen,32-tmpBitLen);
    }
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    ret = pke_mod(t1, get_valid_words(t1, wordLen<<1), vector->b, NULL, NULL, wordLen, t2);
#else
    ret = pke_mod(t1, get_valid_words(t1, wordLen<<1), vector->b, NULL, wordLen, t2);
#endif
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t2, wordLen, vector->std_big_big, wordLen))
    {
        printf("\r\n %u pke_mod(input big_big) failure, ret=%x", vector->modulus_bitLen, ret);
        print_BN_buf_U32(t1, wordLen<<1, "t1");
        print_BN_buf_U32(t2, wordLen, "t2");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_mod(input big_big) success", vector->modulus_bitLen);
    }

    printf("\r\n");

    return 0;
}


uint32_t pke_mod_160_test(void)
{
    pke_mod_test_vector_t vector[1];

    uint32_t b[]                ={0xEE053693,0xFA0F948D,0x0E3030D6,0xABC4ED96,0x889D4A96};
    uint32_t small[]            ={0xAA2485E9,0xB3091313,0x0A8737EB,0x6D71D62B,0x0A464089};
    uint32_t big[]              ={0xE5279A49,0xDF81EB58,0xB4BDF0DB,0x70749A17,0xB3204BDB};
    uint32_t std_big[]          ={0xF72263B6,0xE57256CA,0xA68DC004,0xC4AFAC81,0x2A830144};
    uint32_t std_tiny_small[]   ={0x82A768DC,0x7CAE8767,0x01B5663E,0xE215ADD3,0x7DD2D268};
    uint32_t std_tiny_equal[]   ={0xD882E2F3,0xC9A57453,0xF72E2E52,0x74A3D7A7,0x738C91DF};
    uint32_t std_tiny_big[]     ={0xE1A01016,0xB5083690,0x8F8BBD80,0x8D8E9693,0x1572488D};
    uint32_t std_small_small[]  ={0xBDF853B9,0x61A1D1E8,0xB010BDD4,0xF0F6AC13,0x02824919};
    uint32_t std_small_equal[]  ={0x01D90463,0xA8A85363,0xB3B9B6BF,0x2F49C37E,0x80D95327};
    uint32_t std_small_big[]    ={0x0AF63186,0x940B15A0,0x4C1745ED,0x4834826A,0x22BF09D5};
    uint32_t std_big_small[]    ={0x48B9D554,0x565FBC92,0x6791018B,0x30982543,0x234B50B0};
    uint32_t std_big_equal[]    ={0x9E954F6B,0xA356A97E,0x5D09C99F,0xC3264F18,0x19051026};
    uint32_t std_big_big[]      ={0x95B7B321,0x88C90049,0x039789A4,0x87D5FB9A,0x4388116B};


    vector->modulus_bitLen  = 160;
    vector->b               = b;
    vector->small           = small;
    vector->big             = big;
    vector->std_big         = std_big;
    vector->std_tiny_small  = std_tiny_small;
    vector->std_tiny_equal  = std_tiny_equal;
    vector->std_tiny_big    = std_tiny_big;
    vector->std_small_small = std_small_small;
    vector->std_small_equal = std_small_equal;
    vector->std_small_big   = std_small_big;
    vector->std_big_small   = std_big_small;
    vector->std_big_equal   = std_big_equal;
    vector->std_big_big     = std_big_big;

    return pke_mod_test(vector);
}

uint32_t pke_mod_256_test(void)
{
    pke_mod_test_vector_t vector[1];

    uint32_t b[]                ={0xE3095D4F,0xC1F584E1,0x94DE7179,0x4127A63D,0x8873220B,0x9F6A4D4B,0x30B452D8,0xED170456};
    uint32_t small[]            ={0xAE07EA27,0x86B34C54,0xBFDAB696,0xE93D67A6,0x5E0019AA,0xBC175F5A,0xA5599AF2,0xE5E0CCBF};
    uint32_t big[]              ={0xCA92C3B5,0xFE76B8E3,0x78FCBB6A,0x808C0378,0xD4CF5207,0x5E01ADF8,0x70E22921,0xFF4DA381};
    uint32_t std_big[]          ={0xE7896666,0x3C813401,0xE41E49F1,0x3F645D3A,0x4C5C2FFC,0xBE9760AD,0x402DD648,0x12369F2B};
    uint32_t std_tiny_small[]   ={0x1ED502D3,0x460BE015,0xFF640F89,0xD0DF9A4F,0xEB27B182,0x3EDC18FD,0xE1D623CB,0x3CAFF762};
    uint32_t std_tiny_equal[]   ={0x53D675FB,0x814E18A2,0xD467CA6C,0x28C9D8E6,0x159AB9E3,0x222F06EF,0x6D30DBB1,0x43E62EF9};
    uint32_t std_tiny_big[]     ={0x3B5FDC61,0xBDCF4CA4,0xB886145D,0x682E3621,0x61F6E9DF,0xE0C6679C,0xAD5EB1F9,0x561CCE24};
    uint32_t std_small_small[]  ={0x54F107E0,0x6B28C492,0x2C1BDAB7,0x81FFB1F9,0xEBDC4438,0x60EFB721,0x73C1D11B,0xE7701976};
    uint32_t std_small_equal[]  ={0xA6E91DB9,0xE475783D,0x6C412420,0x98C24A52,0x8DDC2A8D,0xA4D857C7,0xCE683628,0x018F4CB6};
    uint32_t std_small_big[]    ={0x8E72841F,0x20F6AC3F,0x505F6E12,0xD826A78D,0xDA385A89,0x636FB874,0x0E960C71,0x13C5EBE2};
    uint32_t std_big_small[]    ={0x3A206147,0x6178302E,0xD3E4FB49,0xDF0B36F0,0x923936A7,0xC3DDF700,0x60535339,0x0A9CBAD8};
    uint32_t std_big_equal[]    ={0x6F21D46F,0x9CBA68BB,0xA8E8B62C,0x36F57587,0xBCAC3F08,0xA730E4F1,0xEBAE0B1F,0x11D2F26E};
    uint32_t std_big_big[]      ={0x56AB3AD5,0xD93B9CBD,0x8D07001D,0x7659D2C2,0x09086F04,0x65C8459F,0x2BDBE168,0x2409919A};


    vector->modulus_bitLen  = 256;
    vector->b               = b;
    vector->small           = small;
    vector->big             = big;
    vector->std_big         = std_big;
    vector->std_tiny_small  = std_tiny_small;
    vector->std_tiny_equal  = std_tiny_equal;
    vector->std_tiny_big    = std_tiny_big;
    vector->std_small_small = std_small_small;
    vector->std_small_equal = std_small_equal;
    vector->std_small_big   = std_small_big;
    vector->std_big_small   = std_big_small;
    vector->std_big_equal   = std_big_equal;
    vector->std_big_big     = std_big_big;

    return pke_mod_test(vector);
}

uint32_t pke_mod_521_test(void)
{
    pke_mod_test_vector_t vector[1];

    uint32_t b[]                ={0x91386409,0xBB6FB71E,0x899C47AE,0x3BB5C9B8,0xF709A5D0,0x7FCC0148,0xBF2F966B,0x51868783,
                                  0xFFFFFFFA,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,
                                  0x000001FF};
    uint32_t small[]            ={0x2336EE35,0x01BA0D3C,0x95F74BBF,0x4FD9B144,0x77EEE841,0x2AEF1F58,0xB5BB79CB,0xA5920D60,
                                  0x37A4F8EC,0x1DAC3D5C,0xE5417712,0x025715DC,0xC67B420F,0x22FEF8B5,0xF22B9E13,0xEB545ED4,
                                  0x00000000};
    uint32_t big[]              ={0x257ED2D1,0xB6DFD653,0x82D572B7,0x1B177963,0x0C18DAAF,0x058E6D17,0xF9963695,0x246E9C6E,
                                  0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,
                                  0x000001FF};
    uint32_t std_big[]          ={0x94466EC8,0xFB701F34,0xF9392B08,0xDF61AFAA,0x150F34DE,0x85C26BCE,0x3A66A029,0xD2E814EB,
                                  0x00000004,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
                                  0x00000000};
    uint32_t std_tiny_small[]   ={0x34B6EE35,0x0D50C3B2,0x11BAF569,0x5BF473B8,0x31DBF365,0x6DA780A9,0x928CAD87,0xD5F22169,
                                  0x74DF16BC,0x1E1046B5,0xE5417712,0x025715DC,0xC67B420F,0x22FEF8B5,0xF22B9E13,0xEB545ED4,
                                  0x00000000};
    uint32_t std_tiny_equal[]   ={0x11800000,0x0B96B676,0x7BC3A9AA,0x0C1AC273,0xB9ED0B24,0x42B86150,0xDCD133BC,0x30601408,
                                  0x3D3A1DD0,0x00640959,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
                                  0x00000000};
    uint32_t std_tiny_big[]     ={0xA5C66EC8,0x0706D5AA,0x74FCD4B3,0xEB7C721E,0xCEFC4002,0xC87ACD1E,0x1737D3E5,0x034828F4,
                                  0x3D3A1DD5,0x00640959,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
                                  0x00000000};
    uint32_t std_small_small[]  ={0x9F929A20,0xBE4E3909,0xDB9581E7,0x2829744F,0x40E444C3,0x18CFC5C9,0x4DF0601E,0xE4A4F6A9,
                                  0x525CFAAC,0x9F1BF4D0,0x8183DD41,0x222F1D88,0x0EC508FB,0x9D0CEAF8,0xD460B7F6,0xAECDA9F1,
                                  0x000000CC};
    uint32_t std_small_equal[]  ={0x7C5BABEB,0xBC942BCD,0x459E3628,0xD84FC30B,0xC8F55C81,0xEDE0A670,0x9834E652,0x3F12E948,
                                  0x1AB801C0,0x816FB774,0x9C42662F,0x1FD807AB,0x4849C6EC,0x7A0DF242,0xE23519E3,0xC3794B1C,
                                  0x000000CB};
    uint32_t std_small_big[]    ={0x10A21AB3,0xB8044B02,0x3ED76131,0xB7B172B6,0xDE049160,0x73A3123E,0xD29B867C,0x11FAFE33,
                                  0x1AB801C5,0x816FB774,0x9C42662F,0x1FD807AB,0x4849C6EC,0x7A0DF242,0xE23519E3,0xC3794B1C,
                                  0x000000CB};
    uint32_t std_big_small[]    ={0xE5B6E92D,0xE8F222DF,0x95BD0474,0x4BF1F301,0xF85781F9,0x4DFFA6AF,0xC507ACBE,0x724B76CA,
                                  0x1B812EA3,0x011394E2,0xF1FBEBCB,0x10046D18,0x7D5B385A,0x6807067C,0x05ECDEAA,0x53807A8B,
                                  0x0000001C};
    uint32_t std_big_equal[]    ={0xC27FFAF8,0xE73815A3,0xFFC5B8B5,0xFC1841BC,0x806899B7,0x23108757,0x0F4C32F3,0xCCB9696A,
                                  0xE3DC35B6,0xE3675785,0x0CBA74B8,0x0DAD573C,0xB6DFF64B,0x45080DC6,0x13C14097,0x682C1BB6,
                                  0x0000001B};
    uint32_t std_big_big[]      ={0x56C669C0,0xE2A834D8,0xF8FEE3BE,0xDB79F167,0x9577CE96,0xA8D2F325,0x49B2D31C,0x9FA17E55,
                                  0xE3DC35BB,0xE3675785,0x0CBA74B8,0x0DAD573C,0xB6DFF64B,0x45080DC6,0x13C14097,0x682C1BB6,
                                  0x0000001B};

    vector->modulus_bitLen  = 521;
    vector->b               = b;
    vector->small           = small;
    vector->big             = big;
    vector->std_big         = std_big;
    vector->std_tiny_small  = std_tiny_small;
    vector->std_tiny_equal  = std_tiny_equal;
    vector->std_tiny_big    = std_tiny_big;
    vector->std_small_small = std_small_small;
    vector->std_small_equal = std_small_equal;
    vector->std_small_big   = std_small_big;
    vector->std_big_small   = std_big_small;
    vector->std_big_equal   = std_big_equal;
    vector->std_big_big     = std_big_big;

    return pke_mod_test(vector);
}

uint32_t pke_mod_2048_test(void)
{
    pke_mod_test_vector_t vector[1];

    uint32_t b[]                ={0xEA406073,0x56E99882,0x51C84C37,0xDF955E7D,0xEE87AA18,0x85080428,0x2CA3365F,0xC54F6899,
                                  0xD9E7A9FF,0x832D1854,0xE17CC9FE,0x3688C884,0x0FFC87EA,0x04855BBF,0xA92DD257,0x82982C52,
                                  0x1045BEA5,0xA4C6B6DE,0x68F9DA3D,0x5E77B39A,0x33B0D819,0xA19B5268,0x6BD9DB8A,0xFD6304C8,
                                  0xC1ECBF63,0x42A9B258,0xC51CB734,0xFE125D83,0x16AB6CC5,0x5EDDFEF9,0x5E5C75D5,0x178D43F1,
                                  0x3B481C12,0x425B8BC1,0x922AC87B,0xFF34811D,0xA4E65F30,0xEF395D16,0x4847D19E,0x46BCFE4F,
                                  0xFB34CE8A,0x2CA71DC1,0x15C8BB19,0xCF46299B,0x2C2CC75B,0xA31A33BB,0x0671491F,0x2775B96A,
                                  0xBA66B62B,0xB1E202BB,0x7AFA5FB1,0xF2892456,0xD6F6DA0E,0xC3885DCB,0x1C8B5015,0x336E0DD6,
                                  0x2568F4F7,0xE5D24379,0xDD3F4074,0x43F21205,0xA0F55505,0x33F6DE9C,0xB783EC85,0xD72E5C02,};

    uint32_t small[]            ={0xC521D1BD,0xB49A0776,0x55FBC12C,0x58314BBF,0x2F9EDBA7,0x8E3078AD,0xB248B2E0,0x8B137C34,
                                  0x36B4B4EF,0x29D8518F,0x97607B00,0x0B2FDA4F,0xBACE0A19,0xD3B68F79,0x103DB5F6,0x296707A3,
                                  0x38CF904C,0x1A822FC8,0xB5E99ABB,0xFAAA6F1E,0x117D2CAB,0x093EF678,0x00D40587,0x660BBAA9,
                                  0xFC1125CF,0xA1C93055,0xFD22876F,0x45721779,0x6DD5B5EB,0x5B1117EE,0x204DCF06,0xCB1D4304,
                                  0xD1464755,0xE2E56E71,0x49D6545C,0x1DDB2826,0x85290189,0x7E024E5C,0xC4A98137,0xB040768F,
                                  0x70AAF6C6,0xE7AEA63E,0xF3FD8CBF,0xBE4ED322,0xA092509E,0x9AD19511,0x6339E464,0x75D6122B,
                                  0x2EED7257,0xBB53C23E,0x48689C90,0x736A2CE0,0xF6CDC838,0x53A45668,0x437E254B,0xE17BFF08,
                                  0xEA193F3D,0xFB54C4E9,0x6CFC086E,0xB9E43F7C,0x7D0BA86E,0x2DBFD605,0x0E64F902,0x1B98AD51};

    uint32_t big[]              ={0xA42327BF,0xE07843F2,0x484589F0,0x1764FF6E,0x2CEF2028,0xFA68BBAB,0xD13ECF1E,0x54050A85,
                                  0x4D15739E,0xAFC278CD,0xE0C73077,0x849F62CF,0x88FA7154,0x5B0F77EF,0x9281F65A,0xF0900BDF,
                                  0x1C6D5EC8,0x7FD34FA7,0x319115DA,0x8CDE2D50,0x3223AEF8,0x96F6813C,0x654E1F15,0x082CCA51,
                                  0x148FC1B5,0x3A2F32C2,0xE35A8FE7,0x383FD658,0xB606386D,0x46F074C1,0xFD2F55D8,0x7C81C612,
                                  0x48F713C8,0xDF4D4781,0xCFE66858,0x0653253D,0x0DF4DDA7,0xE84F2624,0xE7886C2D,0x1C7C897C,
                                  0xAF0239CC,0x11FAE383,0x4F1872EC,0xB80811DB,0xF29031D7,0xC7B27653,0xFB9FFC78,0xD5BE531D,
                                  0x609D87DA,0x15ECCAA4,0xB33EB976,0xEA75282E,0x62405CCF,0x9222A958,0xD554CE28,0x4ECC5676,
                                  0x8AF63981,0x4EBC9F40,0x09A9AA9E,0xB73D587A,0x1CA171A5,0x4A7AEC62,0x38B353E2,0xE5DED1AB};

    uint32_t std_big[]          ={0xB9E2C74C,0x898EAB6F,0xF67D3DB9,0x37CFA0F0,0x3E67760F,0x7560B782,0xA49B98BF,0x8EB5A1EC,
                                  0x732DC99E,0x2C956078,0xFF4A6679,0x4E169A4A,0x78FDE96A,0x568A1C30,0xE9542403,0x6DF7DF8C,
                                  0x0C27A023,0xDB0C98C9,0xC8973B9C,0x2E6679B5,0xFE72D6DF,0xF55B2ED3,0xF974438A,0x0AC9C588,
                                  0x52A30251,0xF7858069,0x1E3DD8B2,0x3A2D78D5,0x9F5ACBA7,0xE81275C8,0x9ED2E002,0x64F48221,
                                  0x0DAEF7B6,0x9CF1BBC0,0x3DBB9FDD,0x071EA420,0x690E7E76,0xF915C90D,0x9F409A8E,0xD5BF8B2D,
                                  0xB3CD6B41,0xE553C5C1,0x394FB7D2,0xE8C1E840,0xC6636A7B,0x24984298,0xF52EB359,0xAE4899B3,
                                  0xA636D1AF,0x640AC7E8,0x384459C4,0xF7EC03D8,0x8B4982C0,0xCE9A4B8C,0xB8C97E12,0x1B5E48A0,
                                  0x658D448A,0x68EA5BC7,0x2C6A6A29,0x734B4674,0x7BAC1CA0,0x16840DC5,0x812F675D,0x0EB075A8,};

    uint32_t std_tiny_small[]   ={0x98FD7EE1,0xA1278874,0xE17FF911,0x98048A88,0x5563CAC2,0xB9195A31,0xB3ADB7E1,0xBDC0327A,
                                  0x9C398851,0x4D29DBDA,0xC2680026,0xD6A9143E,0x9831D620,0xD529415A,0x7037EEC4,0x59E7FEB2,
                                  0xAE910443,0x97F3F33C,0x39A307E1,0x61227168,0x7BDBFD3B,0xC50E2F6F,0x3065E021,0x62562F64,
                                  0x8DBD59D0,0x355E96B6,0x229B7CC9,0x95864AEC,0xD8E73177,0x2EDBE5D9,0x37A95893,0x4BE7A486,
                                  0x9C99B3EF,0x7AE68A35,0xCBEE8244,0x9BC85740,0x96C96A58,0xBDA43462,0xEEDFBC78,0xC16BFC28,
                                  0x90071631,0x086D9006,0xCD11AD77,0x0BE7A911,0x43FF4A22,0x94554B59,0xCDA8C06A,0x9F80E54A,
                                  0x9EFFA0FA,0x7323FCD9,0xB4CCDFE3,0x10644B5E,0xCE2DC9A6,0x627059FC,0x8275ECFB,0x02A43CEE,
                                  0x927FF1C1,0x490B64EE,0x7D4FE3A9,0x8B1050BB,0x3FFBFC41,0xB9244D17,0xB121D0B6,0x9D6AB9CF};

    uint32_t std_tiny_equal[]   ={0xD3DBAD24,0xEC8D80FD,0x8B8437E4,0x3FD33EC9,0x25C4EF1B,0x2AE8E184,0x01650501,0x32ACB646,
                                  0x6584D362,0x23518A4B,0x2B078526,0xCB7939EF,0xDD63CC07,0x0172B1E0,0x5FFA38CE,0x3080F70F,
                                  0x75C173F7,0x7D71C374,0x83B96D26,0x66780249,0x6A5ED08F,0xBBCF38F7,0x2F91DA9A,0xFC4A74BB,
                                  0x91AC3400,0x93956660,0x2578F559,0x50143372,0x6B117B8C,0xD3CACDEB,0x175B898C,0x80CA6182,
                                  0xCB536C99,0x98011BC3,0x82182DE7,0x7DED2F1A,0x11A068CF,0x3FA1E606,0x2A363B41,0x112B8599,
                                  0x1F5C1F6B,0x20BEE9C8,0xD91420B7,0x4D98D5EE,0xA36CF983,0xF983B647,0x6A6EDC05,0x29AAD31F,
                                  0x70122EA3,0xB7D03A9B,0x6C644352,0x9CFA1E7E,0xD760016D,0x0ECC0393,0x3EF7C7B0,0x21283DE6,
                                  0xA866B283,0x4DB6A004,0x1053DB3A,0xD12C113F,0xC2F053D2,0x8B647711,0xA2BCD7B4,0x81D20C7E};

    uint32_t std_tiny_big[]     ={0x8DBE7470,0x761C2C6D,0x8201759E,0x77A2DFBA,0x642C652A,0xA0499906,0xA6009DC0,0xC1625832,
                                  0xD8B29D00,0x4FE6EAC3,0x2A51EB9F,0x198FD43A,0x5661B572,0x57FCCE11,0x494E5CD1,0x9E78D69C,
                                  0x81E9141A,0x587E5C3D,0x4C50A8C3,0x94DE7BFF,0x68D1A76E,0xB12A67CB,0x29061E25,0x07143A44,
                                  0xE44F3652,0x8B1AE6C9,0x43B6CE0C,0x8A41AC47,0x0A6C4733,0xBBDD43B4,0xB62E698F,0xE5BEE3A3,
                                  0xD902644F,0x34F2D783,0xBFD3CDC5,0x850BD33A,0x7AAEE745,0x38B7AF13,0xC976D5D0,0xE6EB10C6,
                                  0xD3298AAC,0x0612AF89,0x1263D88A,0x365ABE2F,0x69D063FF,0x1E1BF8E0,0x5F9D8F5F,0xD7F36CD3,
                                  0x16490052,0x1BDB0284,0xA4A89D17,0x94E62256,0x62A9842E,0xDD664F20,0xF7C145C2,0x3C868686,
                                  0x0DF3F70D,0xB6A0FBCC,0x3CBE4563,0x447757B3,0x3E9C7073,0xA1E884D7,0x23EC3F11,0x90828227};

    uint32_t std_small_small[]  ={0x9ED4894B,0xAF65F7F8,0xCC64C7B8,0xB0A04ECD,0x4414C063,0xD2332F1C,0xF09DBA36,0x304CD324,
                                  0x67AFD33D,0x88A5092B,0x6A7431C4,0x6D88E345,0xCC7E08CE,0x044AB398,0xD4E64764,0xED07BFD0,
                                  0xE1F2095B,0x7D115AA4,0x86BB9ADE,0x3139A741,0x50763FDA,0x1F00AFA8,0xF5892CF1,0xD7F4DD01,
                                  0x07F6367B,0x22B9D473,0x252B27BF,0x0F6C3BFB,0x66B1AA9E,0x7775D432,0x258F59E6,0x01F018DE,
                                  0x0A156BE7,0x869D208B,0x913AA049,0xD446B18C,0xB1A264A5,0xEDD1E532,0xBAF1D81B,0x48CB7C48,
                                  0x13DEDA58,0x359A8FE4,0xAD607B2E,0xF205E80E,0x282C5206,0x43481155,0x0C42970A,0x44438E4C,
                                  0x6F66F0D7,0x86E961EB,0x679D3201,0xB2B65892,0x3CD12F14,0xE23BFEDF,0xB73DC7D9,0x19DE0D61,
                                  0xA0D086BA,0xD0B075DE,0xE47DAA76,0xB00969A2,0x9D9651CC,0x88FD383A,0xFEAF2CF8,0x18B90276};

    uint32_t std_small_equal[]  ={0xC3F31801,0x51B58904,0xC83152C3,0x3804618B,0x02FD8ED5,0xC90ABA98,0x6AF83DB5,0x6A88BF89,
                                  0x0AE2C84D,0xE1F9CFF1,0xB49080C2,0x98E1D17A,0x21AC869F,0x35197FDE,0x6DD663C4,0x4638E480,
                                  0xB96837B5,0x0755E1BA,0x39CBDA61,0x9506EBBD,0x72A9EB47,0xB75D0B98,0x608F02F4,0x6F4C2721,
                                  0xCDD1D010,0xC39A5675,0xED255783,0xC80C8204,0x0F876178,0x7B42BB3D,0x639E00B5,0x4E6019CB,
                                  0x741740A3,0xE6133DDA,0xD98F1467,0xB5A00A83,0xD15FC24D,0x5F08F3EC,0x3E902883,0xDF480408,
                                  0x9E68B21B,0x7A930767,0xCF2BA987,0x02FD3E86,0xB3C6C8C4,0x4B90AFFE,0xAF79FBC5,0xF5E3358A,
                                  0xFAE034AA,0x7D77A268,0x9A2EF522,0x31D55008,0x1CFA40EB,0x52200642,0x904AF2A4,0x6BD01C2F,
                                  0xDC203C73,0xBB2DF46D,0x54C0E27C,0x3A173C2C,0xC17FFE63,0x8F3440D1,0xA7CE207B,0xD44EB128};

    uint32_t std_small_big[]    ={0x93957EDA,0x845A9BF1,0x6CE64445,0x903EA3FF,0x52DD5ACB,0xB9636DF1,0xE2F0A015,0x33EEF8DC,
                                  0xA428E7EC,0x8B621814,0xD25E1D3D,0xB06FA340,0x8AADE81F,0x871E404F,0xADFCB570,0x319897BA,
                                  0xB54A1933,0x3D9BC3A5,0x99693BC0,0x64F5B1D8,0x3D6BEA0D,0x0B1CE804,0xEE296AF5,0x7CB2E7E1,
                                  0x5E8812FD,0x78762486,0x46467902,0x04279D56,0x9836C05A,0x0477320C,0xA4146AE3,0x9BC757FB,
                                  0x467E1C47,0x40A96DD9,0x851FEBCA,0xBD8A2D86,0x9587E192,0x68E55FE3,0x9588F173,0x6E4A90E6,
                                  0x57014ED3,0x333FAF67,0xF2B2A641,0x1C78FD2B,0x4DFD6BE4,0xCD0EBEDC,0x9E3765FE,0x7CB615D4,
                                  0xE6B0502F,0x2FA06795,0x5778EF35,0x37382F8A,0xD14CE99D,0x5D31F402,0x2C8920A1,0x53C056FA,
                                  0x1C448C06,0x3E460CBC,0xA3EC0C31,0x6970709A,0x9C36C5FE,0x71C16FFA,0x71799B53,0x0BD0CACE};

    uint32_t std_big_small[]    ={0x8694B358,0x2FC7C9BD,0x132DC707,0x065446B0,0x6B7A9E99,0x017C8B4E,0xFC2D28E6,0xFE560897,
                                  0x8F2FF3EF,0xBFE1D6D7,0xBD261205,0x326B4E84,0xDE434360,0xCCF32201,0x7C2A4D44,0xA68FA8CE,
                                  0xDD0F3616,0x48B60543,0x3A90774F,0xA49C005A,0xA2803B67,0x21925D78,0xD56C9229,0xF6FA7B1E,
                                  0x5CF35E15,0x6996E85B,0x5C697535,0xBF733735,0x7008BC18,0x65B2C4CE,0xE9EAFAA8,0x37018F6A,
                                  0x7E624C35,0xE00D5A4F,0x9726B566,0x404F1472,0x1AA3D8F8,0xB684AAD8,0xCF4795B3,0x4650236E,
                                  0x8E5F46DD,0xCCC0EAA4,0xC32EB30D,0x7ADC760E,0x05E90EAF,0x08B46DE8,0x3CD51211,0x86549A21,
                                  0x43D5BFF5,0xCF32158F,0xFBDE33E3,0xA45292F5,0x61C51438,0x50577BDD,0x6A346888,0x374E0FC5,
                                  0xCC007271,0x559B40A3,0x271B4A9F,0xC51D3FD8,0x27AB3C1F,0xAB1CAD2E,0xD9FF52EB,0x9712AF8F};

    uint32_t std_big_equal[]    ={0xC172E19B,0x7B2DC246,0xBD3205DA,0xAE22FAF0,0x3BDBC2F1,0x734C12A1,0x49E47605,0x73428C63,
                                  0x587B3F00,0x96098548,0x25C59705,0x273B7435,0x23753947,0xF93C9288,0x6BEC974D,0x7D28A12B,
                                  0xA43FA5CA,0x2E33D57B,0x84A6DC94,0xA9F1913B,0x91030EBB,0x18536700,0xD4988CA2,0x90EEC075,
                                  0x60E23846,0xC7CDB805,0x5F46EDC5,0x7A011FBB,0x0233062D,0x0AA1ACE0,0xC99D2BA2,0x6BE44C66,
                                  0xAD1C04DF,0xFD27EBDD,0x4D506109,0x2273EC4C,0x957AD76F,0x38825C7B,0x0A9E147C,0x960FACDF,
                                  0x1DB45016,0xE5124466,0xCF31264D,0xBC8DA2EB,0x6556BE10,0x6DE2D8D6,0xD99B2DAC,0x107E87F5,
                                  0x14E84D9E,0x13DE5351,0xB3759753,0x30E86615,0x6AF74C00,0xFCB32574,0x26B6433C,0x55D210BD,
                                  0xE1E73333,0x5A467BB9,0xBA1F4230,0x0B39005B,0xAA9F93B1,0x7D5CD728,0xCB9A59E9,0x7B7A023E};

    uint32_t std_big_big[]      ={0x7B55A8E7,0x04BC6DB6,0xB3AF4394,0xE5F29BE1,0x7A433900,0xE8ACCA23,0xEE800EC4,0x01F82E4F,
                                  0xCBA9089F,0xC29EE5C0,0x250FFD7E,0x75520E80,0x9C7322B1,0x4FC6AEB8,0x5540BB51,0xEB2080B8,
                                  0xB06745ED,0x09406E44,0x4D3E1831,0xD8580AF1,0x8F75E59A,0x0DAE95D4,0xCE0CD02D,0x9BB885FE,
                                  0xB3853A97,0xBF53386E,0x7D84C678,0xB42E9890,0xA18DD1D4,0xF2B422A8,0x68700BA4,0xD0D8CE88,
                                  0xBACAFC95,0x9A19A79D,0x8B0C00E7,0x2992906C,0xFE8955E5,0x31982588,0xA9DEAF0B,0x6BCF380C,
                                  0xD181BB58,0xCA660A27,0x0880DE20,0xA54F8B2C,0x2BBA288C,0x927B1B6F,0xCEC9E105,0xBEC721A9,
                                  0xBB1F1F4D,0x77E91B39,0xEBB9F117,0x28D469ED,0xF640CEC1,0xCB4D7100,0xDF7FC14F,0x7130595D,
                                  0x477477BD,0xC330D781,0xE689AC59,0x7E8446CF,0x264BB051,0x93E0E4EE,0x4CC9C146,0x8A2A77E7};


    vector->modulus_bitLen  = 2048;
    vector->b               = b;
    vector->small           = small;
    vector->big             = big;
    vector->std_big         = std_big;
    vector->std_tiny_small  = std_tiny_small;
    vector->std_tiny_equal  = std_tiny_equal;
    vector->std_tiny_big    = std_tiny_big;
    vector->std_small_small = std_small_small;
    vector->std_small_equal = std_small_equal;
    vector->std_small_big   = std_small_big;
    vector->std_big_small   = std_big_small;
    vector->std_big_equal   = std_big_equal;
    vector->std_big_big     = std_big_big;

    return pke_mod_test(vector);
}



uint32_t hal_pke_mod_test(void)
{
    printf("\r\n\r\n =========================== HAL mod test ============================= \r\n");

#if 1
    if(pke_mod_160_test())
        return 1;
#endif

#if 1
    if(pke_mod_256_test())
        return 1;
#endif

#if 1
    if(pke_mod_521_test())
        return 1;
#endif

#if 1
    if(pke_mod_2048_test())
        return 1;
#endif

printf("\r\n\r\n =========================== HAL mod test end============================= \r\n");

    return 0;
}


