#include <stdio.h>
#include <string.h>
#include "../test.h"

#ifdef AIC_PKE_SM2_SUPPORT

#include <sm2.h>
#include <utility.h>
#include <trng.h>

extern const eccp_curve_t sm2_curve[1];

#if (defined(AIC_PKE_SEC) && defined(SM2_SEC))
//#define SM2_SEC_TEST
#endif

//#define SM2_SPEED_TEST_BY_TIMER

#ifdef SM2_SPEED_TEST_BY_TIMER
extern u32 startP(void);
extern u32 endP(u8 mode, u32 once_bytes, u32 round);
#endif

#define SM2_SPEED_TEST_KEYGET_ROUND    (600)
#define SM2_SPEED_TEST_SIGN_ROUND      (600)
#define SM2_SPEED_TEST_VERIFY_ROUND    (500)

#define SM2_SPEED_TEST_PLAIN_BYTE_LEN  (32)   //明文字节长度
#define SM2_SPEED_TEST_ENCRYPT_ROUND   (400)
#define SM2_SPEED_TEST_DECRYPT_ROUND   (600)
#define SM2_SPEED_TEST_EXCHANGE_ROUND  (500)


#define SM2_SIGN_VERIFY_ROUND1         1
#define SM2_ENCRYPT_DECRYPT_ROUND1     1
#define SM2_KEY_EXCHANGE_ROUND1        1

#define SM2_EXCHANGE_MAX_LEN           (200)

extern const eccp_curve_t sm2_curve[1];

u32 SM2_keyget_speed_test(void)
{
    u8 prikey[32];
    u8 pubkey[65];
    u32 i = 0;
    u32 round = SM2_SPEED_TEST_KEYGET_ROUND;
    u32 ret;

    uint32_sleep(0x1FFFF, 1);

    //speed test
    printf("\r\n\r\n ======== sm2 keyget begin ======== ");

#ifdef SM2_SPEED_TEST_BY_TIMER
    round /= 10;
    startP();
#endif

    for(i=0; i<round; i++)
    {
        ret = sm2_getkey(prikey, pubkey);
        if(SM2_SUCCESS != ret)
        {
            printf("\r\n sm2 get key failure, ret=%x", ret);
            return 1;
        }
    }

#ifdef SM2_SPEED_TEST_BY_TIMER
    endP(1,0,round);
#else
    printf("\r\n keyget finished");
#endif

    return 0;
}


u32 SM2_sign_verify_speed_test(void)
{
    u8 prikey[32];
    u8 pubkey[65];
    u8 E[32]={
        0xDD,0x9E,0xB8,0x55,0xA7,0x18,0x37,0xB3,0x2F,0x93,0x5A,0x3D,0x37,0x81,0x80,0xDD,
        0xB9,0xD2,0x9C,0x7F,0x5A,0x83,0x30,0x36,0x1F,0xE8,0x08,0xD7,0xAA,0x3A,0x1E,0x00,};
    u8 signature[64];
    u32 i=0;
    u32 sign_round   = SM2_SPEED_TEST_SIGN_ROUND;
    u32 verify_round = SM2_SPEED_TEST_VERIFY_ROUND;
    u32 round;
    u32 ret;

    //a small loop for sleeping
    for(i=0;i<30;i++)
    {
        ret = sm2_getkey(prikey, pubkey);
        if(SM2_SUCCESS != ret)
        {
            printf("\r\n sm2 get key failure, ret=%x", ret);
            return 1;
        }
    }

    //sm2_sign speed test
    round = sign_round;
    printf("\r\n\r\n ======== sm2_sign begin ======== ");

#ifdef SM2_SPEED_TEST_BY_TIMER
    round /= 10;
    startP();
#endif

    for(i=0;i<round;i++)
    {
        ret = sm2_sign(E, NULL, prikey, signature);
        if(SM2_SUCCESS != ret)
        {
            printf("\r\n sm2_sign test failure, ret=%x", ret);
            return 1;
        }
    }

#ifdef SM2_SPEED_TEST_BY_TIMER
    endP(1,0,round);
#else
    printf("\r\n sm2_sign finished");
#endif


    //sm2_verify speed test
    round = verify_round;
    printf("\r\n ======== sm2_verify begin ======== ");

#ifdef SM2_SPEED_TEST_BY_TIMER
    round /= 10;
    startP();
#endif

    for(i=0;i<round;i++)
    {
        ret = sm2_verify(E, pubkey, signature);
        if(SM2_SUCCESS != ret)
        {
            printf("\r\n sm2 verify test failure, ret=%x", ret);
            return 1;
        }
    }

#ifdef SM2_SPEED_TEST_BY_TIMER
    endP(1,0,round);
#else
    printf("\r\n sm2_verify finished");
#endif


#ifdef SM2_SEC_TEST
    //sm2_sign_s speed test
    round = sign_round/3;
    printf("\r\n ======== sm2_sign_s begin ======== ");

#ifdef SM2_SPEED_TEST_BY_TIMER
    round /= 10;
    startP();
#endif

    for(i=0;i<round;i++)
    {
        ret = sm2_sign_s(E, NULL, prikey, signature);
        if(SM2_SUCCESS_S != ret)
        {
            printf("\r\n sm2_sign_s test failure, ret=%x", ret);
            return 1;
        }
    }

#ifdef SM2_SPEED_TEST_BY_TIMER
    endP(1,0,round);
#else
    printf("\r\n sm2_sign_s finished");
#endif


    //sm2_verify speed test
    round = verify_round;
    printf("\r\n ======== sm2_verify_s begin ======== ");

#ifdef SM2_SPEED_TEST_BY_TIMER
    round /= 10;
    startP();
#endif

    for(i=0;i<round;i++)
    {
        ret = sm2_verify_s(E, pubkey, signature);
        if(SM2_SUCCESS_S != ret)
        {
            printf("\r\n sm2 verify test failure, ret=%x", ret);
            return 1;
        }
    }

#ifdef SM2_SPEED_TEST_BY_TIMER
    endP(1,0,round);
#else
    printf("\r\n sm2_verify_s finished");
#endif
#endif

    return 0;
}


u32 SM2_encrypt_decrypt_speed_test(void)
{
    u8 prikey[32];
    u8 pubkey[65];
    u8 plain[SM2_SPEED_TEST_PLAIN_BYTE_LEN];
    u8 cipher[SM2_SPEED_TEST_PLAIN_BYTE_LEN+97];
    u8 replain[SM2_SPEED_TEST_PLAIN_BYTE_LEN];
    u32 bytelen, bytelen2, i=0;
    u32 enc_round = SM2_SPEED_TEST_ENCRYPT_ROUND;
    u32 dec_round = SM2_SPEED_TEST_DECRYPT_ROUND;
    u32 round;
    u32 ret;

    //a small loop for sleeping
    for(i=0;i<30;i++)
    {
        ret = sm2_getkey(prikey, pubkey);
        if(SM2_SUCCESS != ret)
        {
            printf("\r\n sm2 get key failure, ret=%x", ret);
            return 1;
        }
    }


    //sm2_encrypt speed test
    round = enc_round;
    printf("\r\n\r\n ======== sm2_encrypt begin ======== ");

#ifdef SM2_SPEED_TEST_BY_TIMER
    round /= 20;
    startP();
#endif

    for(i=0;i<round;i++)
    {
        ret = sm2_encrypt(plain, SM2_SPEED_TEST_PLAIN_BYTE_LEN, NULL, pubkey, SM2_C1C3C2, cipher, &bytelen);
        if(SM2_SUCCESS != ret)
        {
            printf("\r\n sm2_encrypt failure");
            return 1;
        }
    }

#ifdef SM2_SPEED_TEST_BY_TIMER
    endP(1,0,round);
#else
    printf("\r\n sm2_encrypt finished");
#endif


    //sm2_decrypt speed test
    round = dec_round;
    printf("\r\n ======== sm2_decrypt begin ======== ");

#ifdef SM2_SPEED_TEST_BY_TIMER
    round /= 20;
    startP();
#endif

    for(i=0;i<round;i++)
    {
        ret = sm2_decrypt(cipher, bytelen, prikey, SM2_C1C3C2, replain, &bytelen2);
        if(SM2_SUCCESS != ret)
        {
            printf("\r\n sm2_decrypt failure");
            return 1;
        }
    }

#ifdef SM2_SPEED_TEST_BY_TIMER
    endP(1,0,round);
#else
    printf("\r\n sm2_decrypt finished");
#endif


#ifdef SM2_SEC_TEST
    //sm2_encrypt_s speed test
    round = enc_round/5;
    printf("\r\n ======== sm2_encrypt_s begin ======== ");

#ifdef SM2_SPEED_TEST_BY_TIMER
    round /= 10;
    startP();
#endif

    for(i=0;i<round;i++)
    {
        ret = sm2_encrypt_s(plain, SM2_SPEED_TEST_PLAIN_BYTE_LEN, NULL, pubkey, 0, cipher, &bytelen);
        if(SM2_SUCCESS_S != ret)
        {
            printf("\r\n sm2_encrypt_s failure");
            return 1;
        }
    }

#ifdef SM2_SPEED_TEST_BY_TIMER
    endP(1,0,round);
#else
    printf("\r\n sm2_encrypt_s finished");
#endif


    //sm2_decrypt_s speed test
    round = dec_round/2;
    printf("\r\n ======== sm2_decrypt_s begin ======== ");

#ifdef SM2_SPEED_TEST_BY_TIMER
    round /= 10;
    startP();
#endif

    for(i=0;i<round;i++)
    {
        ret = sm2_decrypt_s(cipher, bytelen, prikey, 0, replain, &bytelen2);
        if(SM2_SUCCESS_S != ret)// || (SM2_SPEED_TEST_PLAIN_BYTE_LEN != bytelen2)|| memcmp_(plain, replain, bytelen))
        {
            printf("\r\n sm2_decrypt_s failure");
            return 1;
        }
    }

#ifdef SM2_SPEED_TEST_BY_TIMER
    endP(1,0,round);
#else
    printf("\r\n sm2_decrypt_s finished");
#endif
#endif

    return 0;
}


u32 SM2_exchange_speed_test(void)
{
    u8 ZA[32], ZB[32], KA[32], KB[32], S1[32], SA[32], S2[32], SB[32];
#ifdef SM2_SEC_TEST
    u8 KA_1[32], S1_1[32], SA_1[32];
#endif
    u8 PA[65];//A的公钥
    u8 RA[65];//A的临时公钥
    u8 dA[32];//A的私钥
    u8 rA[32];//A的临时私钥
    u8 PB[65];//B的公钥
    u8 RB[65];//B的临时公钥
    u8 dB[32];//B的私钥
    u8 rB[32];//B的临时私钥
    u32 i=0;
    u32 round_1 = SM2_SPEED_TEST_EXCHANGE_ROUND;
    u32 round;
    u32 ret;

    //random key exchange test
    //a small loop for sleeping
    for(i=0;i<5;i++)
    {
        ret = sm2_getkey(dA, PA);
        ret |= sm2_getkey(rA, RA);
        ret |= sm2_getkey(dB, PB);
        ret |= sm2_getkey(rB, RB);
        if(SM2_SUCCESS != ret)
        {
            printf("\r\n get key failure");
            return 1;
        }
    }

    memset(KA, 0x11, 16);
    memset(KB, 0x55, 16);
    memset(S1, 0x11, 32);
    memset(S2, 0x55, 32);
    memset(SA, 0x11, 32);
    memset(SB, 0x55, 32);

    //sm2_exchangekey speed test
    round = round_1/2;
    ret = 0;
    printf("\r\n\r\n ======== sm2_exchangekey begin ======== ");

#ifdef SM2_SPEED_TEST_BY_TIMER
    round /= 10;
    startP();
#endif

    for(i=0;i<round;i++)
    {
        ret |= sm2_exchangekey(SM2_Role_Sponsor,
                        dA, PB,
                        rA, RA,
                        RB,
                        ZA, ZB,
                        16,
                        KA, S1, SA);
    }

#ifdef SM2_SPEED_TEST_BY_TIMER
    endP(1,0,round);
#else
    printf("\r\n sm2_exchangekey finished");
#endif

    if(SM2_SUCCESS != ret)
    {
        printf("\r\n sm2_exchangekey test error. ret=%02x", ret);
    }


#ifdef SM2_SEC_TEST
    //sm2_exchangekey_s speed test
    round = round_1/4;
    ret = 0;
    printf("\r\n ======== sm2_exchangekey_s begin ======== ");

#ifdef SM2_SPEED_TEST_BY_TIMER
    round /= 10;
    startP();
#endif

    for(i=0;i<round;i++)
    {
        ret |= sm2_exchangekey_s(SM2_Role_Sponsor,
                        dA, PB,
                        rA, RA,
                        RB,
                        ZA, ZB,
                        16,
                        KA_1, S1_1, SA_1);
    }

#ifdef SM2_SPEED_TEST_BY_TIMER
    endP(1,0,round);
#else
    printf("\r\n sm2_exchangekey_s finished");
#endif

    if((SM2_SUCCESS_S != ret) || memcmp_(KA, KA_1, 16) || memcmp_(S1, S1_1, 32) ||  memcmp_(SA, SA_1, 32))
    {
        printf("\r\n sm2_exchangekey_s test error. ret=%02x", ret);
    }
#endif


#ifdef SM2_SEC_TEST
    ret = sm2_exchangekey_s(SM2_Role_Responsor,
                    dB, PA,
                    rB, RB,
                    RA,
                    ZB, ZA,
                    16,
                    KB, S2, SB);
    if((SM2_SUCCESS_S != ret) || memcmp_(KA, KB, 16) || memcmp_(S1, SB, 32) ||  memcmp_(S2, SA, 32))
#else
    ret = sm2_exchangekey(SM2_Role_Responsor,
                    dB, PA,
                    rB, RB,
                    RA,
                    ZB, ZA,
                    16,
                    KB, S2, SB);
    if((SM2_SUCCESS != ret) || memcmp_(KA, KB, 16) || memcmp_(S1, SB, 32) ||  memcmp_(S2, SA, 32))
#endif
    {
        printf("\r\n SM2 key exchange test error. ret=%02x", ret);
        print_buf_U8(KA, 16, "KA");
        print_buf_U8(S1, 32, "S1");
        print_buf_U8(SA, 32, "SA");
        print_buf_U8(KB, 16, "KB");
        print_buf_U8(S2, 32, "S2");
        print_buf_U8(SB, 32, "SB");
        return 1;
    }

    return 0;
}





u32 sm2_get_key_test_internal(u8 *prikey, u8 *pubkey, u32 ret_value, char *info)
{
    u8 buf[65]={0};
    u32 ret;

    ret = sm2_get_pubkey_from_prikey(prikey, buf);
    if((ret_value != ret))
    {
        printf("\r\n SM2 get key %s failure 1, ret=%x", info, ret);
        return 1;
    }
    if((SM2_SUCCESS == ret) && memcmp_(buf, pubkey, 65))
    {
        printf("\r\n SM2 get key %s failure 2, ret=%x", info, ret);
        print_buf_U8(prikey, 32, "std prikey");
        print_buf_U8(pubkey, 65, "std pubkey");
        print_buf_U8(buf, 65, "pubkey");
        return 1;
    }

    printf("\r\n SM2 get key %s success", info);

    return 0;
}

u32 sm2_get_key_test(void)
{
    //standard private key and public key
    u8 prikey[32]={
        0x39,0x45,0x20,0x8F,0x7B,0x21,0x44,0xB1,0x3F,0x36,0xE3,0x8A,0xC6,0xD3,0x9F,0x95,
        0x88,0x93,0x93,0x69,0x28,0x60,0xB5,0x1A,0x42,0xFB,0x81,0xEF,0x4D,0xF7,0xC5,0xB8,
    };
    u8 pubkey[65]={
        0x04,0x09,0xF9,0xDF,0x31,0x1E,0x54,0x21,0xA1,0x50,0xDD,0x7D,0x16,0x1E,0x4B,0xC5,
        0xC6,0x72,0x17,0x9F,0xAD,0x18,0x33,0xFC,0x07,0x6B,0xB0,0x8F,0xF3,0x56,0xF3,0x50,
        0x20,0xCC,0xEA,0x49,0x0C,0xE2,0x67,0x75,0xA5,0x2D,0xC6,0xEA,0x71,0x8C,0xC1,0xAA,
        0x60,0x0A,0xED,0x05,0xFB,0xF3,0x5E,0x08,0x4A,0x66,0x32,0xF6,0x07,0x2D,0xA9,0xAD,
        0x13,
    };

    //public key for the case private key is 1
    u8 pubkey_1[65]={
        0x04,0x32,0xC4,0xAE,0x2C,0x1F,0x19,0x81,0x19,0x5F,0x99,0x04,0x46,0x6A,0x39,0xC9,
        0x94,0x8F,0xE3,0x0B,0xBF,0xF2,0x66,0x0B,0xE1,0x71,0x5A,0x45,0x89,0x33,0x4C,0x74,
        0xC7,0xBC,0x37,0x36,0xA2,0xF4,0xF6,0x77,0x9C,0x59,0xBD,0xCE,0xE3,0x6B,0x69,0x21,
        0x53,0xD0,0xA9,0x87,0x7C,0xC6,0x2A,0x47,0x40,0x02,0xDF,0x32,0xE5,0x21,0x39,0xF0,
        0xA0,
    };

    //public key for the case private key is 2
    u8 pubkey_2[65]={
        0x04,0x56,0xCE,0xFD,0x60,0xD7,0xC8,0x7C,0x00,0x0D,0x58,0xEF,0x57,0xFA,0x73,0xBA,
        0x4D,0x9C,0x0D,0xFA,0x08,0xC0,0x8A,0x73,0x31,0x49,0x5C,0x2E,0x1D,0xA3,0xF2,0xBD,
        0x52,0x31,0xB7,0xE7,0xE6,0xCC,0x81,0x89,0xF6,0x68,0x53,0x5C,0xE0,0xF8,0xEA,0xF1,
        0xBD,0x6D,0xE8,0x4C,0x18,0x2F,0x6C,0x8E,0x71,0x6F,0x78,0x0D,0x3A,0x97,0x0A,0x23,
        0xC3,
    };

    //public key for the case private key is n-2
    u8 pubkey_n_minus_2[65]={
        0x04,0x56,0xCE,0xFD,0x60,0xD7,0xC8,0x7C,0x00,0x0D,0x58,0xEF,0x57,0xFA,0x73,0xBA,
        0x4D,0x9C,0x0D,0xFA,0x08,0xC0,0x8A,0x73,0x31,0x49,0x5C,0x2E,0x1D,0xA3,0xF2,0xBD,
        0x52,0xCE,0x48,0x18,0x18,0x33,0x7E,0x76,0x09,0x97,0xAC,0xA3,0x1F,0x07,0x15,0x0E,
        0x42,0x92,0x17,0xB3,0xE6,0xD0,0x93,0x71,0x8F,0x90,0x87,0xF2,0xC5,0x68,0xF5,0xDC,
        0x3C,
    };

    printf("\r\n -------------- SM2 get key test -------------- ");

    /********** sm2 get key standard data test **********/
    if(sm2_get_key_test_internal(prikey, pubkey, SM2_SUCCESS, "standard data test"))
        return 1;

    /********** prikey is 0 **********/
    memset(prikey, 0, 32);
    if(sm2_get_key_test_internal(prikey, NULL, PKE_ZERO_ALL, "test(prikey=0)"))
        return 1;

    /********** prikey is 1 **********/
    memset(prikey, 0, 32);
    prikey[31]=1;
    if(sm2_get_key_test_internal(prikey, pubkey_1, SM2_SUCCESS, "test(prikey=1)"))
        return 1;

    /********** prikey is 2 **********/
    memset(prikey, 0, 32);
    prikey[31]=2;
    if(sm2_get_key_test_internal(prikey, pubkey_2, SM2_SUCCESS, "test(prikey=2)"))
        return 1;

    /********** prikey is n-2 **********/
    reverse_byte_array((u8 *)(sm2_curve->eccp_n), prikey, 32);
    prikey[31]-=2;
    if(sm2_get_key_test_internal(prikey, pubkey_n_minus_2, SM2_SUCCESS, "test(prikey=n-2)"))
        return 1;

    /********** prikey is n-1 **********/
    reverse_byte_array((u8 *)(sm2_curve->eccp_n), prikey, 32);
    prikey[31]-=1;
    if(sm2_get_key_test_internal(prikey, NULL, PKE_INTEGER_TOO_BIG, "test(prikey=n-1)"))
        return 1;

    /********** prikey is n **********/
    reverse_byte_array((u8 *)(sm2_curve->eccp_n), prikey, 32);
    if(sm2_get_key_test_internal(prikey, NULL, PKE_INTEGER_TOO_BIG, "test(prikey=n)"))
        return 1;

    /********** prikey is n+1 **********/
    reverse_byte_array((u8 *)(sm2_curve->eccp_n), prikey, 32);
    prikey[31]+=1;
    if(sm2_get_key_test_internal(prikey, NULL, PKE_INTEGER_TOO_BIG, "test(prikey=n+1)"))
    {
        return 1;
    }
    else
    {
        printf("\r\n");
        return 0;
    }
}


u32 sm2_sign_verify_test_internal(u8 *E, u8 *prikey, u8 *rand_k, u8 *pubkey,
                                      u8 *signature, u32 sign_ret_value, char *info)
{
    u8 buf[64];
    u8 buf2[64];
    u8 tmp[65];
    u32 ret;
    char *inf="";

    if(info)
        inf=info;

#ifdef SM2_SEC_TEST
    ret = sm2_sign_s(E, rand_k, prikey, buf);
#else
    ret = sm2_sign(E, rand_k, prikey, buf);
#endif
    if((sign_ret_value != ret))
    {
        printf("\r\n SM2 sign %s failure, ret=%08x", inf, ret);
        return 1;
    }

#ifdef SM2_SEC_TEST
    if(SM2_SUCCESS_S == ret)
#else
    if(SM2_SUCCESS == ret)
#endif
    {
        if((NULL != rand_k) && (NULL != signature))
        {
            if(memcmp_(buf, signature, 64))
            {
                printf("\r\n SM2 sign %s failure, ret=%08x", inf, ret);
                print_buf_U8(buf, 64, "signature");
                return 1;
            }
        }
#ifdef SM2_SEC_TEST
        ret = sm2_verify_s(E, pubkey, buf);
        if(SM2_SUCCESS_S != ret)
#else
        ret = sm2_verify(E, pubkey, buf);
        if(SM2_SUCCESS != ret)
#endif
        {
            printf("\r\n SM2 verify %s failure, ret=%x", inf, ret);
            print_buf_U8(buf, 64, "signature");
            return 1;
        }


        //invalid r (conner case:r=0)
        if(rand_k)
        {
            memcpy(buf, rand_k, SM2_BYTE_LEN);
        }
        else
        {
            ret = get_rand((u8 *)buf, SM2_BYTE_LEN);
            buf[0] = 0x7F;
            if(TRNG_SUCCESS != ret)
            {
                return 1;
            }
        }

#ifdef PKE_BIG_ENDIAN
        reverse_word_array(buf, tmp, SM2_WORD_LEN);
#else
        reverse_byte_array(buf, tmp, SM2_BYTE_LEN);
#endif
        ret = eccp_pointMul((eccp_curve_t *)sm2_curve, (u32 *)tmp, sm2_curve->eccp_Gx, sm2_curve->eccp_Gy, (u32 *)tmp, NULL);
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
        ret |= pke_mod((u32 *)tmp, SM2_WORD_LEN, sm2_curve->eccp_n, sm2_curve->eccp_n_h, sm2_curve->eccp_n_n0, SM2_WORD_LEN, (u32 *)tmp);
#else
        ret |= pke_mod((u32 *)tmp, SM2_WORD_LEN, sm2_curve->eccp_n, sm2_curve->eccp_n_h, SM2_WORD_LEN, (u32 *)tmp);
#endif
        if(0 == uint32_BigNum_Check_Zero((u32 *)tmp, SM2_WORD_LEN))
        {
            ret |= pke_sub(sm2_curve->eccp_n, (u32 *)tmp, (u32 *)tmp, SM2_WORD_LEN);
        }
        if(PKE_SUCCESS != ret)
        {
            printf("\r\n invalid r (conner case:r=0) failure, ret=%x", ret);
            return 1;
        }
        else
        {;}

#ifdef PKE_BIG_ENDIAN
        reverse_word_array(tmp, tmp, SM2_WORD_LEN);
#else
        reverse_byte_array(tmp, tmp, SM2_BYTE_LEN);
#endif
        
#ifdef SM2_SEC_TEST
        ret = sm2_sign_s(tmp, buf, prikey, tmp);
        if(SM2_ERROR_S != ret)
#else
        ret = sm2_sign(tmp, buf, prikey, tmp);
        if(SM2_ZERO_ALL != ret)
#endif
        {
            printf("\r\n SM2 sign %s failure(invalid r 1), ret=%08x", inf, ret);
            print_buf_U8(tmp, 64, "signature");
            return 1;
        }

        //invalid r (conner case:r+k=n)
#ifdef PKE_BIG_ENDIAN
        reverse_word_array(buf, tmp, SM2_WORD_LEN);
        reverse_word_array(buf, buf, SM2_WORD_LEN);
#else
        reverse_byte_array(buf, tmp, SM2_BYTE_LEN);
        reverse_byte_array(buf, buf, SM2_BYTE_LEN);
#endif

        ret = eccp_pointMul((eccp_curve_t *)sm2_curve, (u32 *)tmp, sm2_curve->eccp_Gx, sm2_curve->eccp_Gy, (u32 *)tmp, NULL);
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
        ret |= pke_mod((u32 *)tmp, SM2_WORD_LEN, sm2_curve->eccp_n, sm2_curve->eccp_n_h, sm2_curve->eccp_n_n0, SM2_WORD_LEN, (u32 *)tmp);
#else
        ret |= pke_mod((u32 *)tmp, SM2_WORD_LEN, sm2_curve->eccp_n, sm2_curve->eccp_n_h, SM2_WORD_LEN, (u32 *)tmp);
#endif
        ret |= pke_sub(sm2_curve->eccp_n, (u32 *)buf, (u32 *)buf2, SM2_WORD_LEN);
        ret |= pke_modsub(sm2_curve->eccp_n, (u32 *)buf2, (u32 *)tmp, (u32 *)tmp, SM2_WORD_LEN);
        if(PKE_SUCCESS != ret)
        {
            printf("\r\n invalid r (conner case:r+k=n) failure, ret=%x", ret);
            return 1;
        }
        else
        {;}

#ifdef PKE_BIG_ENDIAN
        reverse_word_array(tmp, tmp, SM2_WORD_LEN);
        reverse_word_array(buf, buf, SM2_WORD_LEN);
#else
        reverse_byte_array(tmp, tmp, SM2_BYTE_LEN);
        reverse_byte_array(buf, buf, SM2_BYTE_LEN);
#endif

#ifdef SM2_SEC_TEST
        ret = sm2_sign_s(tmp, buf, prikey, tmp);
        if(SM2_ERROR_S != ret)
#else
        ret = sm2_sign(tmp, buf, prikey, tmp);
        if(SM2_ZERO_ALL != ret)
#endif
        {
            printf("\r\n SM2 sign %s failure(invalid r 2), ret=%08x", inf, ret);
            print_buf_U8(tmp, 64, "signature");
            return 1;
        }

        //invalid r (conner case:s=0, dA=k*r(-1) mod n)
#ifdef PKE_BIG_ENDIAN
        reverse_word_array(buf, tmp, SM2_WORD_LEN);
        reverse_word_array(buf, buf, SM2_WORD_LEN);
        reverse_word_array(E, buf2, SM2_WORD_LEN);
#else
        reverse_byte_array(buf, tmp, SM2_BYTE_LEN);
        reverse_byte_array(buf, buf, SM2_BYTE_LEN);
        reverse_byte_array(E, buf2, SM2_BYTE_LEN);
#endif

#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
        ret = pke_mod((u32 *)buf2, SM2_WORD_LEN, sm2_curve->eccp_n, sm2_curve->eccp_n_h, sm2_curve->eccp_n_n0, SM2_WORD_LEN, (u32 *)buf2);
#else
        ret = pke_mod((u32 *)buf2, SM2_WORD_LEN, sm2_curve->eccp_n, sm2_curve->eccp_n_h, SM2_WORD_LEN, (u32 *)buf2);
#endif
        ret |= eccp_pointMul((eccp_curve_t *)sm2_curve, (u32 *)tmp, sm2_curve->eccp_Gx, sm2_curve->eccp_Gy, (u32 *)tmp, NULL);
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
        ret |= pke_mod((u32 *)tmp, SM2_WORD_LEN, sm2_curve->eccp_n, sm2_curve->eccp_n_h, sm2_curve->eccp_n_n0, SM2_WORD_LEN, (u32 *)tmp);
#else
        ret |= pke_mod((u32 *)tmp, SM2_WORD_LEN, sm2_curve->eccp_n, sm2_curve->eccp_n_h, SM2_WORD_LEN, (u32 *)tmp);
#endif
        ret |= pke_modadd(sm2_curve->eccp_n, (u32 *)tmp, (u32 *)buf2, (u32 *)tmp, SM2_WORD_LEN);
        ret |= pke_modinv(sm2_curve->eccp_n, (u32 *)tmp, (u32 *)tmp, SM2_WORD_LEN, SM2_WORD_LEN);
        ret |= pke_modmul(sm2_curve->eccp_n, (u32 *)tmp, (u32 *)buf, (u32 *)tmp, SM2_WORD_LEN);
        if(PKE_SUCCESS != ret)
        {
            printf("\r\n invalid r (conner case:s=0) failure, ret=%x", ret);
            return 1;
        }
        else
        {;}

#ifdef PKE_BIG_ENDIAN
        reverse_word_array(tmp, tmp, SM2_WORD_LEN);
        reverse_word_array(buf, buf, SM2_WORD_LEN);
#else
        reverse_byte_array(tmp, tmp, SM2_BYTE_LEN);
        reverse_byte_array(buf, buf, SM2_BYTE_LEN);
#endif

#ifdef SM2_SEC_TEST
        ret = sm2_sign_s(E, buf, tmp, buf);
        if(SM2_ERROR_S != ret)
#else
        ret = sm2_sign(E, buf, tmp, buf);
        if(SM2_ZERO_ALL != ret)
#endif
        {
            printf("\r\n SM2 sign %s failure(invalid s), ret=%08x", inf, ret);
            print_buf_U8(buf, 64, "signature");
            print_buf_U8(tmp, 32, "dA");
            return 1;
        }

        //invalid E
        memcpy(tmp, E, 32);
        tmp[0]+=3;
#ifdef SM2_SEC_TEST
        ret = sm2_verify_s(tmp, pubkey, buf);
        if(SM2_ERROR_S != ret)
#else
        ret = sm2_verify(tmp, pubkey, buf);
        if(SM2_VERIFY_FAILED != ret)
#endif
        {
            printf("\r\n SM2 verify %s failure(invalid E), ret=%x", inf, ret);
            return 1;
        }

        //invalid pubkey test 1
        memcpy(tmp, pubkey, 65);
        tmp[0]+=3;
#ifdef SM2_SEC_TEST
        ret = sm2_verify_s(E, tmp, buf);
        if(SM2_ERROR_S != ret)
#else
        ret = sm2_verify(E, tmp, buf);
        if(SM2_INPUT_INVALID != ret)
#endif
        {
            printf("\r\n SM2 verify %s failure(invalid public key 1), ret=%x", inf, ret);
            return 1;
        }

        //invalid pubkey test 2
        memcpy(tmp, pubkey, 65);
        tmp[1]+=3;
#ifdef SM2_SEC_TEST
        ret = sm2_verify_s(E, tmp, buf);
        if(SM2_ERROR_S != ret)
#else
        ret = sm2_verify(E, tmp, buf);
        if(SM2_NOT_ON_CURVE != ret)
#endif
        {
            printf("\r\n SM2 verify %s failure(invalid public key 2), ret=%x", inf, ret);
            return 1;
        }

        //invalid pubkey test 3
        memcpy(tmp, pubkey, 65);
        tmp[64]+=3;
#ifdef SM2_SEC_TEST
        ret = sm2_verify_s(E, tmp, buf);
        if(SM2_ERROR_S != ret)
#else
        ret = sm2_verify(E, tmp, buf);
        if(SM2_NOT_ON_CURVE != ret)
#endif
        {
            printf("\r\n SM2 verify %s failure(invalid public key 3), ret=%x", inf, ret);
            return 1;
        }

        //invalid signature test 1(corrupted r)
        memcpy(tmp, buf, 64);
        tmp[0]+=3;
#ifdef SM2_SEC_TEST
        ret = sm2_verify_s(E, pubkey, tmp);
        if(SM2_ERROR_S != ret)
#else
        ret = sm2_verify(E, pubkey, tmp);
        if((SM2_VERIFY_FAILED != ret) && (SM2_INTEGER_TOO_BIG != ret))
#endif
        {
            printf("\r\n SM2 verify %s failure(invalid signature: corrupted r), ret=%x", inf, ret);
            return 1;
        }

        //invalid signature test 2(r=0)
        memcpy(tmp, buf, 64);
        memset(tmp, 0, 32);
#ifdef SM2_SEC_TEST
        ret = sm2_verify_s(E, pubkey, tmp);
        if(SM2_ERROR_S != ret)
#else
        ret = sm2_verify(E, pubkey, tmp);
        if(SM2_ZERO_ALL != ret)
#endif
        {
            printf("\r\n SM2 verify %s failure(invalid signature: r=0), ret=%x", inf, ret);
            return 1;
        }

        //invalid signature test 3(r=n-1)
        memcpy(tmp, buf, 64);
        reverse_byte_array((u8 *)(sm2_curve->eccp_n), tmp, 32);
        tmp[31]-=1;
#ifdef SM2_SEC_TEST
        ret = sm2_verify_s(E, pubkey, tmp);
        if(SM2_ERROR_S != ret)
#else
        ret = sm2_verify(E, pubkey, tmp);
        if(SM2_VERIFY_FAILED != ret)
#endif
        {
            printf("\r\n SM2 verify %s failure(invalid signature: r=n-1), ret=%x", inf, ret);
            return 1;
        }

        //invalid signature test 4(r=n)
        memcpy(tmp, buf, 64);
        reverse_byte_array((u8 *)(sm2_curve->eccp_n), tmp, 32);
#ifdef SM2_SEC_TEST
        ret = sm2_verify_s(E, pubkey, tmp);
        if(SM2_ERROR_S != ret)
#else
        ret = sm2_verify(E, pubkey, tmp);
        if(SM2_INTEGER_TOO_BIG != ret)
#endif
        {
            printf("\r\n SM2 verify %s failure(invalid signature: r=n), ret=%x", inf, ret);
            return 1;
        }

        //invalid signature test 5(r=n+1)
        memcpy(tmp, buf, 64);
        reverse_byte_array((u8 *)(sm2_curve->eccp_n), tmp, 32);
        tmp[31]+=1;
#ifdef SM2_SEC_TEST
        ret = sm2_verify_s(E, pubkey, tmp);
        if(SM2_ERROR_S != ret)
#else
        ret = sm2_verify(E, pubkey, tmp);
        if(SM2_INTEGER_TOO_BIG != ret)
#endif
        {
            printf("\r\n SM2 verify %s failure(invalid signature: r=n+1), ret=%x", inf, ret);
            return 1;
        }

        //invalid signature test 6(corrupted s)
        memcpy(tmp, buf, 64);
        tmp[63]+=3;
#ifdef SM2_SEC_TEST
        ret = sm2_verify_s(E, pubkey, tmp);
        if(SM2_ERROR_S != ret)
#else
        ret = sm2_verify(E, pubkey, tmp);
        if(SM2_VERIFY_FAILED != ret)
#endif
        {
            printf("\r\n SM2 verify %s failure(invalid signature: corrupted s), ret=%x", inf, ret);
            return 1;
        }

        //invalid signature test 7(s=0)
        memcpy(tmp, buf, 64);
        memset(tmp+32, 0, 32);
#ifdef SM2_SEC_TEST
        ret = sm2_verify_s(E, pubkey, tmp);
        if(SM2_ERROR_S != ret)
#else
        ret = sm2_verify(E, pubkey, tmp);
        if(SM2_ZERO_ALL != ret)
#endif
        {
            printf("\r\n SM2 verify %s failure(invalid signature: s=0), ret=%x", inf, ret);
            return 1;
        }

        //invalid signature test 8(s=n-1)
        memcpy(tmp, buf, 64);
        reverse_byte_array((u8 *)(sm2_curve->eccp_n), tmp+32, 32);
        tmp[63]-=1;
#ifdef SM2_SEC_TEST
        ret = sm2_verify_s(E, pubkey, tmp);
        if(SM2_ERROR_S != ret)
#else
        ret = sm2_verify(E, pubkey, tmp);
        if(SM2_VERIFY_FAILED != ret)
#endif
        {
            printf("\r\n SM2 verify %s failure(invalid signature: s=n-1), ret=%x", inf, ret);
            return 1;
        }

        //invalid signature test 9(s=n)
        memcpy(tmp, buf, 64);
        reverse_byte_array((u8 *)(sm2_curve->eccp_n), tmp+32, 32);
#ifdef SM2_SEC_TEST
        ret = sm2_verify_s(E, pubkey, tmp);
        if(SM2_ERROR_S != ret)
#else
        ret = sm2_verify(E, pubkey, tmp);
        if(SM2_INTEGER_TOO_BIG != ret)
#endif
        {
            printf("\r\n SM2 verify %s failure(invalid signature: s=n), ret=%x", inf, ret);
            return 1;
        }

        //invalid signature test 10(s=n+1)
        memcpy(tmp, buf, 64);
        reverse_byte_array((u8 *)(sm2_curve->eccp_n), tmp+32, 32);
        tmp[63]+=1;
#ifdef SM2_SEC_TEST
        ret = sm2_verify_s(E, pubkey, tmp);
        if(SM2_ERROR_S != ret)
#else
        ret = sm2_verify(E, pubkey, tmp);
        if(SM2_INTEGER_TOO_BIG != ret)
#endif
        {
            printf("\r\n SM2 verify %s failure(invalid signature: r=n+1), ret=%x", inf, ret);
            return 1;
        }
    }

    if(NULL != info)
        printf("\r\n SM2 sign & verify %s success", inf);

    return 0;
}


u32 sm2_sign_verify_test(void)
{
    u8 prikey[32]={
        0x39,0x45,0x20,0x8F,0x7B,0x21,0x44,0xB1,0x3F,0x36,0xE3,0x8A,0xC6,0xD3,0x9F,0x95,
        0x88,0x93,0x93,0x69,0x28,0x60,0xB5,0x1A,0x42,0xFB,0x81,0xEF,0x4D,0xF7,0xC5,0xB8,
    };
    u8 pubkey[65]={
        0x04,0x09,0xF9,0xDF,0x31,0x1E,0x54,0x21,0xA1,0x50,0xDD,0x7D,0x16,0x1E,0x4B,0xC5,
        0xC6,0x72,0x17,0x9F,0xAD,0x18,0x33,0xFC,0x07,0x6B,0xB0,0x8F,0xF3,0x56,0xF3,0x50,
        0x20,0xCC,0xEA,0x49,0x0C,0xE2,0x67,0x75,0xA5,0x2D,0xC6,0xEA,0x71,0x8C,0xC1,0xAA,
        0x60,0x0A,0xED,0x05,0xFB,0xF3,0x5E,0x08,0x4A,0x66,0x32,0xF6,0x07,0x2D,0xA9,0xAD,
        0x13,
    };
    u8 rand_k[32]={
        0x59,0x27,0x6E,0x27,0xD5,0x06,0x86,0x1A,0x16,0x68,0x0F,0x3A,0xD9,0xC0,0x2D,0xCC,
        0xEF,0x3C,0xC1,0xFA,0x3C,0xDB,0xE4,0xCE,0x6D,0x54,0xB8,0x0D,0xEA,0xC1,0xBC,0x21,
    };
    //"message digest"
    u8 msg[100] = {
        0x6D,0x65,0x73,0x73,0x61,0x67,0x65,0x20,0x64,0x69,0x67,0x65,0x73,0x74};
    //"1234567812345678"
    u8 id[16] = {
        0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38};
    u8 std_Z[]={
        0xB2,0xE1,0x4C,0x5C,0x79,0xC6,0xDF,0x5B,0x85,0xF4,0xFE,0x7E,0xD8,0xDB,0x7A,0x26,
        0x2B,0x9D,0xA7,0xE0,0x7C,0xCB,0x0E,0xA9,0xF4,0x74,0x7B,0x8C,0xCD,0xA8,0xA4,0xF3,};
    u8 std_E[]={
        0xF0,0xB4,0x3E,0x94,0xBA,0x45,0xAC,0xCA,0xAC,0xE6,0x92,0xED,0x53,0x43,0x82,0xEB,
        0x17,0xE6,0xAB,0x5A,0x19,0xCE,0x7B,0x31,0xF4,0x48,0x6F,0xDF,0xC0,0xD2,0x86,0x40,};
    u8 std_signature[64]={
        0xF5,0xA0,0x3B,0x06,0x48,0xD2,0xC4,0x63,0x0E,0xEA,0xC5,0x13,0xE1,0xBB,0x81,0xA1,
        0x59,0x44,0xDA,0x38,0x27,0xD5,0xB7,0x41,0x43,0xAC,0x7E,0xAC,0xEE,0xE7,0x20,0xB3,
        0xB1,0xB6,0xAA,0x29,0xDF,0x21,0x2F,0xD8,0x76,0x31,0x82,0xBC,0x0D,0x42,0x1C,0xA1,
        0xBB,0x90,0x38,0xFD,0x1F,0x7F,0x42,0xD4,0x84,0x0B,0x69,0xC4,0x85,0xBB,0xC1,0xAA,};
    u8 *k;
    u8 Z[32],E[32];

    //e=0
    u8 signature_e_0[64] = {
        0x04,0xEB,0xFC,0x71,0x8E,0x8D,0x17,0x98,0x62,0x04,0x32,0x26,0x8E,0x77,0xFE,0xB6,
        0x41,0x5E,0x2E,0xDE,0x0E,0x07,0x3C,0x0F,0x4F,0x64,0x0E,0xCD,0x2E,0x14,0x9A,0x73,
        0x74,0x0F,0x49,0x4C,0x6B,0x9D,0xCC,0xA0,0x75,0x8F,0xD4,0xCF,0x0E,0xAA,0x8B,0xA8,
        0xCD,0x76,0xE9,0xB7,0xE4,0xFD,0x91,0x6F,0x7C,0x48,0x41,0x68,0xF6,0xA4,0xB3,0x8F};

    //e=1
    u8 signature_e_1[64] = {
        0x04,0xEB,0xFC,0x71,0x8E,0x8D,0x17,0x98,0x62,0x04,0x32,0x26,0x8E,0x77,0xFE,0xB6,
        0x41,0x5E,0x2E,0xDE,0x0E,0x07,0x3C,0x0F,0x4F,0x64,0x0E,0xCD,0x2E,0x14,0x9A,0x74,
        0xC2,0x0D,0xE6,0xE8,0x8A,0xF6,0xCE,0x75,0x5C,0x85,0x63,0x1D,0xD2,0x7A,0xD1,0x10,
        0x4F,0xA4,0x0F,0x08,0xDE,0xB6,0x1F,0xF1,0xE9,0x63,0x9C,0xA3,0xB0,0x71,0xC3,0x6E};

    //e=n
    u8 signature_e_n[64] = {
        0x04,0xEB,0xFC,0x71,0x8E,0x8D,0x17,0x98,0x62,0x04,0x32,0x26,0x8E,0x77,0xFE,0xB6,
        0x41,0x5E,0x2E,0xDE,0x0E,0x07,0x3C,0x0F,0x4F,0x64,0x0E,0xCD,0x2E,0x14,0x9A,0x73,
        0x74,0x0F,0x49,0x4C,0x6B,0x9D,0xCC,0xA0,0x75,0x8F,0xD4,0xCF,0x0E,0xAA,0x8B,0xA8,
        0xCD,0x76,0xE9,0xB7,0xE4,0xFD,0x91,0x6F,0x7C,0x48,0x41,0x68,0xF6,0xA4,0xB3,0x8F};

    //e=0xFFF...FFF
    u8 signature_e_FF[64] = {
        0x04,0xEB,0xFC,0x72,0x8E,0x8D,0x17,0x98,0x62,0x04,0x32,0x26,0x8E,0x77,0xFE,0xB6,
        0xCF,0x5A,0x4F,0x72,0xEC,0x41,0x36,0xE3,0xFB,0xA8,0x1A,0xC3,0xF4,0x3F,0x59,0x4F,
        0xC8,0x26,0x83,0x8A,0x13,0x67,0x62,0xB3,0x6B,0xD9,0x88,0xEF,0xC2,0xC6,0x5E,0x76,
        0xEB,0xE3,0x77,0x4C,0xD3,0xE8,0x08,0x1F,0xE1,0xAA,0x0A,0x76,0x14,0x30,0xAB,0x2C};

    u32 i,j;
    u32 ret;
#ifdef SM2_GETE_BY_STEPS
    HASH_CTX ctx[1];
#endif

    printf("\r\n -------------- SM2 get Z, E, key, sign and verify test -------------- ");


    /********** sm2 get Z standard data test **********/
    ret = sm2_getZ(id, 16, pubkey, Z);
    if(SM2_SUCCESS != ret || memcmp_(Z, std_Z, 32))
    {
        printf("\r\n SM2 get Z standard data test failure, ret=%x", ret);
        return 1;
    }
    else
    {
        printf("\r\n SM2 get Z standard data test success");
    }

    /********** sm2 get E standard data test **********/
    ret = sm2_getE(msg, 14, Z, E);
    if(SM2_SUCCESS != ret || memcmp_(E, std_E, 32))
    {
        printf("\r\n SM2 get E standard data test failure, ret=%x", ret);
    }
    else
    {
        printf("\r\n SM2 get E standard data test success");
    }
#ifdef SM2_GETE_BY_STEPS
    memset(E, 0, 32);
    ret = sm2_getE_init(ctx, Z);
    ret |= sm2_getE_update(ctx, msg, 1);
    ret |= sm2_getE_update(ctx, msg+1, 3);
    ret |= sm2_getE_update(ctx, msg+4, 5);
    ret |= sm2_getE_update(ctx, msg+9, 3);
    ret |= sm2_getE_update(ctx, msg+12, 2);
    ret |= sm2_getE_final(ctx, E);
    if(SM2_SUCCESS != ret || memcmp_(E, std_E, 32))
    {
        printf("\r\n SM2 get E standard data stepwise test failure, ret=%x", ret);
    }
    else
    {
        printf("\r\n SM2 get E standard data stepwise test success");
    }
#endif

    /********** sm2 sign & verify standard data test **********/
#ifdef SM2_SEC_TEST
    if(sm2_sign_verify_test_internal(std_E, prikey, rand_k, pubkey, std_signature, SM2_SUCCESS_S, "standard data test"))
#else
    if(sm2_sign_verify_test_internal(std_E, prikey, rand_k, pubkey, std_signature, SM2_SUCCESS, "standard data test"))
#endif
        return 1;


    /********** e = 0 **********/
    memset(E, 0, 32);
#ifdef SM2_SEC_TEST
    if(sm2_sign_verify_test_internal(E, prikey, rand_k, pubkey, signature_e_0, SM2_SUCCESS_S, "test(e=0)"))
#else
    if(sm2_sign_verify_test_internal(E, prikey, rand_k, pubkey, signature_e_0, SM2_SUCCESS, "test(e=0)"))
#endif
        return 1;

    /********** e = 1 **********/
    memset(E, 0, 32);
    E[31]=1;
#ifdef SM2_SEC_TEST
    if(sm2_sign_verify_test_internal(E, prikey, rand_k, pubkey, signature_e_1, SM2_SUCCESS_S, "test(e=1)"))
#else
    if(sm2_sign_verify_test_internal(E, prikey, rand_k, pubkey, signature_e_1, SM2_SUCCESS, "test(e=1)"))
#endif
        return 1;

    /********** e = n **********/
    reverse_byte_array((u8 *)(sm2_curve->eccp_n), E, 32);
#ifdef SM2_SEC_TEST
    if(sm2_sign_verify_test_internal(E, prikey, rand_k, pubkey, signature_e_n, SM2_SUCCESS_S, "test(e=n)"))
#else
    if(sm2_sign_verify_test_internal(E, prikey, rand_k, pubkey, signature_e_n, SM2_SUCCESS, "test(e=n)"))
#endif
        return 1;

    /********** e = 0xFF...FF **********/
    memset(E, 0xFF, 32);
#ifdef SM2_SEC_TEST
    if(sm2_sign_verify_test_internal(E, prikey, rand_k, pubkey, signature_e_FF, SM2_SUCCESS_S, "test(e=0xFF...FF)"))
#else
    if(sm2_sign_verify_test_internal(E, prikey, rand_k, pubkey, signature_e_FF, SM2_SUCCESS, "test(e=0xFF...FF)"))
#endif
        return 1;


    /********* sm2 get key, sign, verify random test **********/
    printf("\r\n\r\n SM2 sign & verify random data test: \r\n");

    for(i=0;i<SM2_SIGN_VERIFY_ROUND1;i++)
    {
        //random E
        ret = get_rand(E, 32);
        if(TRNG_SUCCESS != ret)
        {
            printf("\r\n get rand error 1, ret = %02x", ret);
            return 1;
        }

        //random key pair
        memset(prikey,0,32);
        memset(pubkey,0,65);
        ret = sm2_getkey(prikey, pubkey);
        if(SM2_SUCCESS != ret)
        {
            printf("\r\n sm2 get key failure, ret=%x", ret);
            return 1;
        }
        //random k
        if(prikey[0]>0x80)
        {
            k=NULL;
        }
        else
        {
            k=rand_k;
            ret = get_rand(k, 32);
            if(TRNG_SUCCESS != ret)
            {
                printf("\r\n get rand error 2, ret = %02x", ret);
                return 1;
            }
            k[0] = 0x7F;  //make sure k < n
        }

        for(j=0;j<50;j++)
        {
            printf("\r\n SM2 sign & verify round1=%u, round2=%u", i+1, j+1);

#ifdef SM2_SEC_TEST
            if(sm2_sign_verify_test_internal(E, prikey, k, pubkey, NULL, SM2_SUCCESS_S, NULL))
#else
            if(sm2_sign_verify_test_internal(E, prikey, k, pubkey, NULL, SM2_SUCCESS, NULL))
#endif
                return 1;
        }
    }
    

    return 0;
}




#define SM2_PLAIN_MAX_LEN  (200)
u32 sm2_encrypt_decrypt_test(void)
{
    u8 prikey[32]={
        0x39,0x45,0x20,0x8F,0x7B,0x21,0x44,0xB1,0x3F,0x36,0xE3,0x8A,0xC6,0xD3,0x9F,0x95,
        0x88,0x93,0x93,0x69,0x28,0x60,0xB5,0x1A,0x42,0xFB,0x81,0xEF,0x4D,0xF7,0xC5,0xB8,
    };
    u8 pubkey[65]={
        0x04,0x09,0xF9,0xDF,0x31,0x1E,0x54,0x21,0xA1,0x50,0xDD,0x7D,0x16,0x1E,0x4B,0xC5,
        0xC6,0x72,0x17,0x9F,0xAD,0x18,0x33,0xFC,0x07,0x6B,0xB0,0x8F,0xF3,0x56,0xF3,0x50,
        0x20,0xCC,0xEA,0x49,0x0C,0xE2,0x67,0x75,0xA5,0x2D,0xC6,0xEA,0x71,0x8C,0xC1,0xAA,
        0x60,0x0A,0xED,0x05,0xFB,0xF3,0x5E,0x08,0x4A,0x66,0x32,0xF6,0x07,0x2D,0xA9,0xAD,
        0x13,
    };
    u8 rand_k[32]={
        0x59,0x27,0x6E,0x27,0xD5,0x06,0x86,0x1A,0x16,0x68,0x0F,0x3A,0xD9,0xC0,0x2D,0xCC,
        0xEF,0x3C,0xC1,0xFA,0x3C,0xDB,0xE4,0xCE,0x6D,0x54,0xB8,0x0D,0xEA,0xC1,0xBC,0x21,
    };

    u8 plain[SM2_PLAIN_MAX_LEN]={
        0x65,0x6E,0x63,0x72,0x79,0x70,0x74,0x69,0x6F,0x6E,0x20,0x73,0x74,0x61,0x6E,0x64,
        0x61,0x72,0x64,
    };
    //C1C3C2
    u8 std_cipher[19+97]={
        0x04,0x04,0xEB,0xFC,0x71,0x8E,0x8D,0x17,0x98,0x62,0x04,0x32,0x26,0x8E,0x77,0xFE,
        0xB6,0x41,0x5E,0x2E,0xDE,0x0E,0x07,0x3C,0x0F,0x4F,0x64,0x0E,0xCD,0x2E,0x14,0x9A,
        0x73,0xE8,0x58,0xF9,0xD8,0x1E,0x54,0x30,0xA5,0x7B,0x36,0xDA,0xAB,0x8F,0x95,0x0A,
        0x3C,0x64,0xE6,0xEE,0x6A,0x63,0x09,0x4D,0x99,0x28,0x3A,0xFF,0x76,0x7E,0x12,0x4D,
        0xF0,0x59,0x98,0x3C,0x18,0xF8,0x09,0xE2,0x62,0x92,0x3C,0x53,0xAE,0xC2,0x95,0xD3,
        0x03,0x83,0xB5,0x4E,0x39,0xD6,0x09,0xD1,0x60,0xAF,0xCB,0x19,0x08,0xD0,0xBD,0x87,
        0x66,0x21,0x88,0x6C,0xA9,0x89,0xCA,0x9C,0x7D,0x58,0x08,0x73,0x07,0xCA,0x93,0x09,
        0x2D,0x65,0x1E,0xFA
    };
    u8 cipher[SM2_PLAIN_MAX_LEN+97];
    u8 replain[SM2_PLAIN_MAX_LEN+97];
    u8 *k;
    u32 i,j, bytelen, bytelen2;
    sm2_cipher_order_e order = SM2_C1C3C2;
    u32 ret;

    printf("\r\n\r\n -------------- SM2 encrypt & decrypt test -------------- ");


    /********** encrypt & decrypt standard data test **********/
    //standard data test 1
#ifdef SM2_SEC_TEST
    ret = sm2_encrypt_s(plain, 19, rand_k, pubkey, SM2_C1C3C2, cipher, &bytelen);
    if((SM2_SUCCESS_S != ret) || (bytelen!=19+97) || memcmp_(cipher, std_cipher, bytelen))
#else
    ret = sm2_encrypt(plain, 19, rand_k, pubkey, SM2_C1C3C2, cipher, &bytelen);
    if((SM2_SUCCESS != ret) || (bytelen!=19+97) || memcmp_(cipher, std_cipher, bytelen))
#endif
    {
        printf("\r\n SM2 encrypt & decrypt standard data test 1 encrypt failure, ret=%x", ret);
        return 1;
    }
    memset(replain, 0, 200);
#ifdef SM2_SEC_TEST
    ret = sm2_decrypt_s(cipher, bytelen, prikey, SM2_C1C3C2, replain, &bytelen2);
    if((SM2_SUCCESS_S != ret) || (19 != bytelen2)|| memcmp_(plain, replain, bytelen2))
#else
    ret = sm2_decrypt(cipher, bytelen, prikey, SM2_C1C3C2, replain, &bytelen2);
    if((SM2_SUCCESS != ret) || (19 != bytelen2)|| memcmp_(plain, replain, bytelen2))
#endif
    {
        printf("\r\n SM2 encrypt & decrypt standard data test 1 failure, ret=%x", ret);
        return 1;
    }

    //transform to C1C2C3
    memcpy(replain, std_cipher+65, 32);  //store C3
    memcpy(std_cipher+65, std_cipher+65+32, 19);
    memcpy(std_cipher+65+19, replain, 32);

    //standard data test 2
    memset(cipher, 0x11, 19+97);
#ifdef SM2_SEC_TEST
    ret = sm2_encrypt_s(plain, 19, rand_k, pubkey, SM2_C1C2C3, cipher, &bytelen);
    if((SM2_SUCCESS_S != ret) || (bytelen!=19+97) || memcmp_(cipher, std_cipher, bytelen))
#else
    ret = sm2_encrypt(plain, 19, rand_k, pubkey, SM2_C1C2C3, cipher, &bytelen);
    if((SM2_SUCCESS != ret) || (bytelen!=19+97) || memcmp_(cipher, std_cipher, bytelen))
#endif
    {
        printf("\r\n SM2 encrypt & decrypt standard data test 2 encrypt failure, ret=%x", ret);
        return 1;
    }
    memset(replain, 0, 200);
#ifdef SM2_SEC_TEST
    ret = sm2_decrypt_s(cipher, bytelen, prikey, SM2_C1C2C3, replain, &bytelen2);
    if((SM2_SUCCESS_S != ret) || (19 != bytelen2)|| memcmp_(plain, replain, bytelen2))
#else
    ret = sm2_decrypt(cipher, bytelen, prikey, SM2_C1C2C3, replain, &bytelen2);
    if((SM2_SUCCESS != ret) || (19 != bytelen2)|| memcmp_(plain, replain, bytelen2))
#endif
    {
        printf("\r\n SM2 encrypt & decrypt standard data test 2 decrypt failure, ret=%x", ret);
        return 1;
    }
    printf("\r\n SM2 encrypt & decrypt standard data test success");

    /********** encrypt & decrypt random data test **********/
    printf("\r\n\r\n SM2 encrypt & decrypt random data test: \r\n");
    for(i=0;i<SM2_ENCRYPT_DECRYPT_ROUND1;i++)
    {
        //random key pair
        memset(prikey,0,32);
        memset(pubkey,0,65);
        ret = sm2_getkey(prikey, pubkey);
        if(SM2_SUCCESS != ret)
        {
            printf("\r\n SM2 get key pair error, ret = %02x", ret);
            return 1;
        }

        //random plaintext
        ret = get_rand(plain, SM2_PLAIN_MAX_LEN);
        if(TRNG_SUCCESS != ret)
        {
            printf("\r\n get rand error 1, ret = %02x", ret);
            return 1;
        }

        //random order
        order ^= (plain[0]+plain[1]+plain[2])&1;

        //random k
        if(plain[0]>0x80)
        {
            k=NULL;
        }
        else
        {
            k=rand_k;
            ret = get_rand(k, 32);
            if(TRNG_SUCCESS != ret)
            {
                printf("\r\n get rand error 2, ret = %02x", ret);
                return 1;
            }
            k[0] = 0x7F;  //make sure k < n
        }

        //明文长度可以从1字节开始递增，但这里测试明文长度从10开始，是因为如果太短，如明文只有1字节，则约有1/256的概率加密失败，即kdf的结果是全0，从而返回失败。
        //实际应用中，若明文很短，且加密返回失败（SM2_ZERO_ALL），则重新尝试下加密就好了，这是正常的。
        for(j=10; j<=SM2_PLAIN_MAX_LEN; j++)
        {
            printf("\r\n SM2 encrypt & decrypt round1=%u, plaintext byte length=%u, order=%u", i+1, j, (u32)order);

            if(j&1)
            {
#ifdef SM2_SEC_TEST
                ret = sm2_encrypt_s(plain, j, k, pubkey, order, cipher, &bytelen);
#else
                ret = sm2_encrypt(plain, j, k, pubkey, order, cipher, &bytelen);
#endif
            }
            else
            {
                memcpy(cipher, plain, j);
#ifdef SM2_SEC_TEST
                ret = sm2_encrypt_s(cipher, j, k, pubkey, order, cipher, &bytelen);
#else
                ret = sm2_encrypt(cipher, j, k, pubkey, order, cipher, &bytelen);
#endif
            }
#ifdef SM2_SEC_TEST
            if((SM2_SUCCESS_S != ret) || (j+97 != bytelen))
#else
            if((SM2_SUCCESS != ret) || (j+97 != bytelen))
#endif
            {
                print_buf_U8(plain, j, "plain");
                print_buf_U8(prikey, 32, "prikey");
                print_buf_U8(pubkey, 65, "pubkey");
                if(k)
                    print_buf_U8(k, 32, "k");
                print_buf_U8(cipher, bytelen, "cipher");
                printf("\r\n encrypt failure, ret = %02x", ret);
                return 1;
            }

            if(j&1)
            {
                memset(replain, 0, SM2_PLAIN_MAX_LEN);
#ifdef SM2_SEC_TEST
                ret = sm2_decrypt_s(cipher, bytelen, prikey, order, replain, &bytelen2);
#else
                ret = sm2_decrypt(cipher, bytelen, prikey, order, replain, &bytelen2);
#endif
            }
            else
            {
                memcpy(replain, cipher, bytelen);
#ifdef SM2_SEC_TEST
                ret = sm2_decrypt_s(replain, bytelen, prikey, order, replain, &bytelen2);
#else
                ret = sm2_decrypt(replain, bytelen, prikey, order, replain, &bytelen2);
#endif
            }
#ifdef SM2_SEC_TEST
            if((SM2_SUCCESS_S != ret) || (j != bytelen2)|| memcmp_(plain, replain, bytelen2))
#else
            if((SM2_SUCCESS != ret) || (j != bytelen2)|| memcmp_(plain, replain, bytelen2))
#endif
            {
                print_buf_U8(plain, j, "plain");
                print_buf_U8(cipher, bytelen, "cipher");
                print_buf_U8(prikey, 32, "prikey");
                print_buf_U8(pubkey, 65, "pubkey");
                printf("\r\n decrypt failure, ret = %02x", ret);
                return 1;
            }
            else
            {
                //printf("\r\n decrypt pass");
            }
        }
    }

    

    return 0;
}



u32 sm2_key_exchange_test(void)
{
    u8 KA[SM2_EXCHANGE_MAX_LEN], KB[SM2_EXCHANGE_MAX_LEN], S1[32]={0}, SA[32]={0}, S2[32]={0}, SB[32]={0};

    u8 ZA[32]={
        0x3B,0x85,0xA5,0x71,0x79,0xE1,0x1E,0x7E,0x51,0x3A,0xA6,0x22,0x99,0x1F,0x2C,0xA7,
        0x4D,0x18,0x07,0xA0,0xBD,0x4D,0x4B,0x38,0xF9,0x09,0x87,0xA1,0x7A,0xC2,0x45,0xB1,
    };
    u8 ZB[32]={
        0x79,0xC9,0x88,0xD6,0x32,0x29,0xD9,0x7E,0xF1,0x9F,0xE0,0x2C,0xA1,0x05,0x6E,0x01,
        0xE6,0xA7,0x41,0x1E,0xD2,0x46,0x94,0xAA,0x8F,0x83,0x4F,0x4A,0x4A,0xB0,0x22,0xF7,
    };

    //A的身份
    u8 IDa[16]= {
        0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,
    };
    //A的公钥
    u8 PA[65] = {0x04,
        0x16,0x0E,0x12,0x89,0x7D,0xF4,0xED,0xB6,0x1D,0xD8,0x12,0xFE,0xB9,0x67,0x48,0xFB,
        0xD3,0xCC,0xF4,0xFF,0xE2,0x6A,0xA6,0xF6,0xDB,0x95,0x40,0xAF,0x49,0xC9,0x42,0x32,
        0x4A,0x7D,0xAD,0x08,0xBB,0x9A,0x45,0x95,0x31,0x69,0x4B,0xEB,0x20,0xAA,0x48,0x9D,
        0x66,0x49,0x97,0x5E,0x1B,0xFC,0xF8,0xC4,0x74,0x1B,0x78,0xB4,0xB2,0x23,0x00,0x7F,
    };
    //A的临时公钥
    u8 RA[65] = {0x04,
        0x64,0xCE,0xD1,0xBD,0xBC,0x99,0xD5,0x90,0x04,0x9B,0x43,0x4D,0x0F,0xD7,0x34,0x28,
        0xCF,0x60,0x8A,0x5D,0xB8,0xFE,0x5C,0xE0,0x7F,0x15,0x02,0x69,0x40,0xBA,0xE4,0x0E,
        0x37,0x66,0x29,0xC7,0xAB,0x21,0xE7,0xDB,0x26,0x09,0x22,0x49,0x9D,0xDB,0x11,0x8F,
        0x07,0xCE,0x8E,0xAA,0xE3,0xE7,0x72,0x0A,0xFE,0xF6,0xA5,0xCC,0x06,0x20,0x70,0xC0,
    };
    //A的私钥
    u8 dA[32] = {
        0x81,0xEB,0x26,0xE9,0x41,0xBB,0x5A,0xF1,0x6D,0xF1,0x16,0x49,0x5F,0x90,0x69,0x52,
        0x72,0xAE,0x2C,0xD6,0x3D,0x6C,0x4A,0xE1,0x67,0x84,0x18,0xBE,0x48,0x23,0x00,0x29,
    };
    //A的临时私钥
    u8 rA[32] = {
        0xD4,0xDE,0x15,0x47,0x4D,0xB7,0x4D,0x06,0x49,0x1C,0x44,0x0D,0x30,0x5E,0x01,0x24,
        0x00,0x99,0x0F,0x3E,0x39,0x0C,0x7E,0x87,0x15,0x3C,0x12,0xDB,0x2E,0xA6,0x0B,0xB3,
    };
    //B的身份
    u8 IDb[16]= {
        0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,
    };
    //B的公钥
    u8 PB[65] = {0x04,
        0x6A,0xE8,0x48,0xC5,0x7C,0x53,0xC7,0xB1,0xB5,0xFA,0x99,0xEB,0x22,0x86,0xAF,0x07,
        0x8B,0xA6,0x4C,0x64,0x59,0x1B,0x8B,0x56,0x6F,0x73,0x57,0xD5,0x76,0xF1,0x6D,0xFB,
        0xEE,0x48,0x9D,0x77,0x16,0x21,0xA2,0x7B,0x36,0xC5,0xC7,0x99,0x20,0x62,0xE9,0xCD,
        0x09,0xA9,0x26,0x43,0x86,0xF3,0xFB,0xEA,0x54,0xDF,0xF6,0x93,0x05,0x62,0x1C,0x4D,
    };
    //B的临时公钥
    u8 RB[65] = {0x04,
        0xAC,0xC2,0x76,0x88,0xA6,0xF7,0xB7,0x06,0x09,0x8B,0xC9,0x1F,0xF3,0xAD,0x1B,0xFF,
        0x7D,0xC2,0x80,0x2C,0xDB,0x14,0xCC,0xCC,0xDB,0x0A,0x90,0x47,0x1F,0x9B,0xD7,0x07,
        0x2F,0xED,0xAC,0x04,0x94,0xB2,0xFF,0xC4,0xD6,0x85,0x38,0x76,0xC7,0x9B,0x8F,0x30,
        0x1C,0x65,0x73,0xAD,0x0A,0xA5,0x0F,0x39,0xFC,0x87,0x18,0x1E,0x1A,0x1B,0x46,0xFE,
    };
    //B的私钥
    u8 dB[32] = {
        0x78,0x51,0x29,0x91,0x7D,0x45,0xA9,0xEA,0x54,0x37,0xA5,0x93,0x56,0xB8,0x23,0x38,
        0xEA,0xAD,0xDA,0x6C,0xEB,0x19,0x90,0x88,0xF1,0x4A,0xE1,0x0D,0xEF,0xA2,0x29,0xB5,
    };
    //B的临时私钥
    u8 rB[32] = {
        0x7E,0x07,0x12,0x48,0x14,0xB3,0x09,0x48,0x91,0x25,0xEA,0xED,0x10,0x11,0x13,0x16,
        0x4E,0xBF,0x0F,0x34,0x58,0xC5,0xBD,0x88,0x33,0x5C,0x1F,0x9D,0x59,0x62,0x43,0xD6,
    };
    u8 const std_key[16] = {
        0x6C,0x89,0x34,0x73,0x54,0xDE,0x24,0x84,0xC6,0x0B,0x4A,0xB1,0xFD,0xE4,0xC6,0xE5,
    };
    u8 const std_S1_SB[32] = {
        0xD3,0xA0,0xFE,0x15,0xDE,0xE1,0x85,0xCE,0xAE,0x90,0x7A,0x6B,0x59,0x5C,0xC3,0x2A,
        0x26,0x6E,0xD7,0xB3,0x36,0x7E,0x99,0x83,0xA8,0x96,0xDC,0x32,0xFA,0x20,0xF8,0xEB
    };
    u8 const std_S2_SA[32] = {
        0x18,0xC7,0x89,0x4B,0x38,0x16,0xDF,0x16,0xCF,0x07,0xB0,0x5C,0x5E,0xC0,0xBE,0xF5,
        0xD6,0x55,0xD5,0x8F,0x77,0x9C,0xC1,0xB4,0x00,0xA4,0xF3,0x88,0x46,0x44,0xDB,0x88
    };
    u32 i;
    u32 keyByteLen;
    u32 ret;


    printf("\r\n\r\n -------------- SM2 key exchange --------------");

    /********** get key standard data test **********/
    //get key standard data test
    ret = sm2_get_pubkey_from_prikey(dA, KA);
    if((SM2_SUCCESS != ret) || memcmp_(PA, KA, 65))
    {
        printf("\r\n SM2 get key standard data test 1 failure, ret=%x", ret);
        return 1;
    }
    ret = sm2_get_pubkey_from_prikey(rA, KA);
    if((SM2_SUCCESS != ret) || memcmp_(RA, KA, 65))
    {
        printf("\r\n SM2 get key standard data test 2 failure, ret=%x", ret);
        return 1;
    }
    ret = sm2_get_pubkey_from_prikey(dB, KA);
    if((SM2_SUCCESS != ret) || memcmp_(PB, KA, 65))
    {
        printf("\r\n SM2 get key standard data test 3 failure, ret=%x", ret);
        return 1;
    }
    ret = sm2_get_pubkey_from_prikey(rB, KA);
    if((SM2_SUCCESS != ret) || memcmp_(RB, KA, 65))
    {
        printf("\r\n SM2 get key standard data test 4 failure, ret=%x", ret);
        return 1;
    }
    printf("\r\n SM2 get key standard data test success");



    /********** key exchange standard data test **********/
    ret = sm2_getZ(IDa, 16, PA, KA);
    if((SM2_SUCCESS != ret) || memcmp_(ZA, KA, 32))
    {
        print_buf_U8(KA, 32, "ZA");
        printf("\r\n SM2 get Z test 1 failure, ret=%x", ret);
        return 1;
    }
    ret = sm2_getZ(IDb, 16, PB, KA);
    if((SM2_SUCCESS != ret) || memcmp_(ZB, KA, 32))
    {
        print_buf_U8(KA, 32, "ZB");
        printf("\r\n SM2 get Z test 2 failure, ret=%x", ret);
        return 1;
    }
    printf("\r\n SM2 get Z standard data test success");

    //standard data test 1(with SA/S1/SB/S2)
#ifdef SM2_SEC_TEST
    ret = sm2_exchangekey_s(SM2_Role_Sponsor,
                    dA, PB,
                    rA, RA,
                    RB,
                    ZA, ZB,
                    16,
                    KA, S1, SA);
    ret |= sm2_exchangekey_s(SM2_Role_Responsor,
                    dB, PA,
                    rB, RB,
                    RA,
                    ZB, ZA,
                    16,
                    KB, S2, SB);
    if((SM2_SUCCESS_S != ret) || memcmp_((void *)std_key, KA, 16) || memcmp_((void *)std_key, KB, 16) || memcmp_((void *)std_S1_SB, S1, 32)
            || memcmp_((void *)std_S1_SB, SB, 32) || memcmp_((void *)std_S2_SA, S2, 32) || memcmp_((void *)std_S2_SA, SA, 32))
#else
    ret = sm2_exchangekey(SM2_Role_Sponsor,
                    dA, PB,
                    rA, RA,
                    RB,
                    ZA, ZB,
                    16,
                    KA, S1, SA);
    ret |= sm2_exchangekey(SM2_Role_Responsor,
                    dB, PA,
                    rB, RB,
                    RA,
                    ZB, ZA,
                    16,
                    KB, S2, SB);
    if((SM2_SUCCESS != ret) || memcmp_((void *)std_key, KA, 16) || memcmp_((void *)std_key, KB, 16) || memcmp_((void *)std_S1_SB, S1, 32)
            || memcmp_((void *)std_S1_SB, SB, 32) || memcmp_((void *)std_S2_SA, S2, 32) || memcmp_((void *)std_S2_SA, SA, 32))
#endif
    {
        printf("\r\n SM2 key exchange standard data test 1 error. ret=%08x", ret);
        print_buf_U8(KA, 16, "KA");
        print_buf_U8(S1, 32, "S1");
        print_buf_U8(SA, 32, "SA");
        print_buf_U8(KB, 16, "KB");
        print_buf_U8(S2, 32, "S2");
        print_buf_U8(SB, 32, "SB");
        return 1;
    }
    else
    {
        printf("\r\n SM2 key exchange standard data test 1 success. ");
    }

    //standard data test 2(without SA/S1/SB/S2)
#ifdef SM2_SEC_TEST
    ret = sm2_exchangekey_s(SM2_Role_Sponsor,
                    dA, PB,
                    rA, RA,
                    RB,
                    ZA, ZB,
                    16,
                    KA, NULL, NULL);
    ret |= sm2_exchangekey_s(SM2_Role_Responsor,
                    dB, PA,
                    rB, RB,
                    RA,
                    ZB, ZA,
                    16,
                    KB, NULL, NULL);
    if((SM2_SUCCESS_S != ret) || memcmp_((void *)std_key, KA, 16) || memcmp_((void *)std_key, KB, 16))
#else
    ret = sm2_exchangekey(SM2_Role_Sponsor,
                    dA, PB,
                    rA, RA,
                    RB,
                    ZA, ZB,
                    16,
                    KA, NULL, NULL);
    ret |= sm2_exchangekey(SM2_Role_Responsor,
                    dB, PA,
                    rB, RB,
                    RA,
                    ZB, ZA,
                    16,
                    KB, NULL, NULL);
    if((SM2_SUCCESS != ret) || memcmp_((void *)std_key, KA, 16) || memcmp_((void *)std_key, KB, 16))
#endif
    {
        printf("\r\n SM2 key exchange standard data test 2 error(no SA/S1/SB/S2). ret=%08x", ret);
        print_buf_U8(KA, 16, "KA");
        print_buf_U8(S1, 32, "S1");
        print_buf_U8(SA, 32, "SA");
        print_buf_U8(KB, 16, "KB");
        print_buf_U8(S2, 32, "S2");
        print_buf_U8(SB, 32, "SB");
        return 1;
    }
    else
    {
        printf("\r\n SM2 key exchange standard data test 2 success(no SA/S1/SB/S2). ");
    }

    /********** key exchange random data test **********/
    printf("\r\n\r\n SM2 key exchange random data test: \r\n");
    for(i=0; i<SM2_KEY_EXCHANGE_ROUND1; i++)
    {
        //random key pairs
        memset(dA,0,32);
        memset(PA,0,65);
        memset(rA,0,32);
        memset(RA,0,65);
        memset(dB,0,32);
        memset(PB,0,65);
        memset(rB,0,32);
        memset(RB,0,65);
        ret = sm2_getkey(dA, PA);
        ret |= sm2_getkey(rA, RA);
        ret |= sm2_getkey(dB, PB);
        ret |= sm2_getkey(rB, RB);
        if(SM2_SUCCESS != ret)
        {
            printf("\r\n SM2 key exchange random data test get key failure, ret = %02x", ret);
            return 1;
        }

        //random ZA & ZB
        ret = get_rand(ZA, 32);
        ret |= get_rand(ZB, 32);
        if(TRNG_SUCCESS != ret)
        {
            printf("\r\n get rand error, ret = %02x", ret);
            return 1;
        }

        //类似加解密，协商出来的密钥若是全0，则协商失败，1个字节时可能会协商失败
        for(keyByteLen=8; keyByteLen<=SM2_EXCHANGE_MAX_LEN; keyByteLen++)
        {
            printf("\r\n SM2 key exchange round1=%u, keyByteLen=%u", i+1, keyByteLen);
            //get random key pairs
            memset(KA, 0x11, keyByteLen);
            memset(KB, 0x55, keyByteLen);
            memset(S1, 0x11, 32);
            memset(S2, 0x55, 32);
            memset(SA, 0x11, 32);
            memset(SB, 0x55, 32);

           //test with SA/S1/SB/S2
#ifdef SM2_SEC_TEST
            ret = sm2_exchangekey_s(SM2_Role_Sponsor,
                            dA, PB,
                            rA, RA,
                            RB,
                            ZA, ZB,
                            keyByteLen,
                            KA, S1, SA);
            ret |= sm2_exchangekey_s(SM2_Role_Responsor,
                            dB, PA,
                            rB, RB,
                            RA,
                            ZB, ZA,
                            keyByteLen,
                            KB, S2, SB);
            if((SM2_SUCCESS_S != ret) || memcmp_(KA, KB, keyByteLen) || memcmp_(S1, SB, 32) ||  memcmp_(S2, SA, 32))
#else
            ret = sm2_exchangekey(SM2_Role_Sponsor,
                            dA, PB,
                            rA, RA,
                            RB,
                            ZA, ZB,
                            keyByteLen,
                            KA, S1, SA);
            ret |= sm2_exchangekey(SM2_Role_Responsor,
                            dB, PA,
                            rB, RB,
                            RA,
                            ZB, ZA,
                            keyByteLen,
                            KB, S2, SB);
            if((SM2_SUCCESS != ret) || memcmp_(KA, KB, keyByteLen) || memcmp_(S1, SB, 32) ||  memcmp_(S2, SA, 32))
#endif
            {
                printf("\r\n SM2 key exchange random data test 1 error. ret=%02x", ret);
                print_buf_U8(KA, keyByteLen, "KA");
                print_buf_U8(S1, 32, "S1");
                print_buf_U8(SA, 32, "SA");
                print_buf_U8(KB, keyByteLen, "KB");
                print_buf_U8(S2, 32, "S2");
                print_buf_U8(SB, 32, "SB");
                return 1;
            }

           //test without SA/S1/SB/S2
            memset(S1, 0x11, 32);
            memset(S2, 0x55, 32);
            memset(SA, 0x11, 32);
            memset(SB, 0x55, 32);
#ifdef SM2_SEC_TEST
            ret = sm2_exchangekey_s(SM2_Role_Sponsor,
                            dA, PB,
                            rA, RA,
                            RB,
                            ZA, ZB,
                            keyByteLen,
                            KA, NULL, NULL);
            ret |= sm2_exchangekey_s(SM2_Role_Responsor,
                            dB, PA,
                            rB, RB,
                            RA,
                            ZB, ZA,
                            keyByteLen,
                            KB, NULL, NULL);
            if((SM2_SUCCESS_S != ret) || memcmp_(KA, KB, keyByteLen))
#else
            ret = sm2_exchangekey(SM2_Role_Sponsor,
                            dA, PB,
                            rA, RA,
                            RB,
                            ZA, ZB,
                            keyByteLen,
                            KA, NULL, NULL);
            ret |= sm2_exchangekey(SM2_Role_Responsor,
                            dB, PA,
                            rB, RB,
                            RA,
                            ZB, ZA,
                            keyByteLen,
                            KB, NULL, NULL);
            if((SM2_SUCCESS != ret) || memcmp_(KA, KB, keyByteLen))
#endif
            {
                printf("\r\n SM2 key exchange random data test 2 error(no SA/S1/SB/S2). ret=%02x", ret);
                print_buf_U8(KA, keyByteLen, "KA");
                print_buf_U8(S1, 32, "S1");
                print_buf_U8(SA, 32, "SA");
                print_buf_U8(KB, keyByteLen, "KB");
                print_buf_U8(S2, 32, "S2");
                print_buf_U8(SB, 32, "SB");
                return 1;
            }
        }
    }

    printf("\r\n ");

    return 0;
}

#ifdef TRNG_POKER_TEST
u32 sm2_rng_test(void)
{
    u8 prikey[32];
    u8 pubkey[65];
    u8 buf1[32];
    u8 buf2[164];
    u8 buf3[164];
    u32 i, bytelen;
    u32 ret=0;

    for(i=0;i<3000;i++)
    {
        //get key
        ret |= sm2_getkey(prikey, pubkey);
        if(SM2_SUCCESS != ret)
        {
            printf("\r\n SM2 get key failure, ret = %02x", ret);
            return 1;
        }

        //sign & verify
#ifdef SM2_SEC_TEST
        ret = sm2_sign_s(buf1, NULL, prikey, buf2);
        if(SM2_SUCCESS_S != ret)
#else
        ret = sm2_sign(buf1, NULL, prikey, buf2);
        if(SM2_SUCCESS != ret)
#endif
        {
            printf("\r\n SM2 sign failure, ret=%x", ret);
            return 1;
        }
#ifdef SM2_SEC_TEST
        ret = sm2_verify_s(buf1, pubkey, buf2);
        if(SM2_SUCCESS_S != ret)
#else
        ret = sm2_verify(buf1, pubkey, buf2);
        if(SM2_SUCCESS != ret)
#endif
        {
            printf("\r\n SM2 verify failure, ret=%x", ret);
            return 1;
        }

        //encrypt & decrypt
#ifdef SM2_SEC_TEST
        ret = sm2_encrypt_s(buf1, 32, NULL, pubkey, pubkey[2]&1, buf2, &bytelen);
        if((SM2_SUCCESS_S != ret) || (32+97 != bytelen))
#else
        ret = sm2_encrypt(buf1, 32, NULL, pubkey, pubkey[2]&1, buf2, &bytelen);
        if((SM2_SUCCESS != ret) || (32+97 != bytelen))
#endif
        {
            printf("\r\n encrypt failure, ret = %02x", ret);
            return 1;
        }
#ifdef SM2_SEC_TEST
        ret = sm2_decrypt_s(buf2, bytelen, prikey, pubkey[2]&1, buf3, &bytelen);
        if((SM2_SUCCESS_S != ret) || (32 != bytelen)|| memcmp_(buf1, buf3, bytelen))
#else
        ret = sm2_decrypt(buf2, bytelen, prikey, pubkey[2]&1, buf3, &bytelen);
        if((SM2_SUCCESS != ret) || (32 != bytelen)|| memcmp_(buf1, buf3, bytelen))
#endif
        {
            printf("\r\n decrypt failure, ret = %02x", ret);
            return 1;
        }
    }

    return 0;
}
#endif



u32 SM2_all_test(void)
{
    u32 ret;

    printf("\r\n\r\n =================== SM2 test =================== ");

#ifdef SM2_SEC_TEST
    pke_sec_init();
#endif

    //test internal random numbers in sm2 get_key,sign and encrypt
#ifdef TRNG_POKER_TEST
    if(sm2_rng_test())
        return 1;
#endif

#if 0
    SM2_keyget_speed_test();
    SM2_sign_verify_speed_test();
    SM2_encrypt_decrypt_speed_test();
    SM2_exchange_speed_test();
#else
/**/
    ret = sm2_get_key_test();
    if(ret)
        return 1;

    ret = sm2_sign_verify_test();
    if(ret)
        return 1;

    ret = sm2_encrypt_decrypt_test();
    if(ret)
        return 1;

    ret = sm2_key_exchange_test();
    if(ret)
        return 1;
#endif


    return 0;
}


#endif

