#include <stdio.h>
#include <string.h>
#include "../test.h"

#ifdef AIC_PKE_RSA_SUPPORT

#include <rsa.h>
#include <utility.h>

#if (defined(AIC_PKE_SEC) && defined(RSA_SEC))
//#define RSA_SEC_TEST
#endif

//#define RSA_SPEED_TEST_BY_TIMER

#ifdef RSA_SPEED_TEST_BY_TIMER
extern u32 startP(void);
extern u32 endP(u8 mode, u32 once_bytes, u32 round);
#endif

#define RSA_STRESS_TEST 0

#if RSA_STRESS_TEST
#define TEST_RSA1024_ROUND  5000
#define TEST_RSA1026_ROUND  5000
#define TEST_RSA2046_ROUND  1000
#define TEST_RSA2048_ROUND  1000
#define TEST_RSA3072_ROUND  500
#define TEST_RSA4096_ROUND  300
#else
#define TEST_RSA1024_ROUND  10
#define TEST_RSA1026_ROUND  10
#define TEST_RSA2046_ROUND  5
#define TEST_RSA2048_ROUND  5
#define TEST_RSA3072_ROUND  3
#define TEST_RSA4096_ROUND  2
#endif

typedef struct{
    u32 eBitLen;
    u32 nBitLen;
    u32 *n;
    u32 *e;
    u32 *d;
    u32 *p;
    u32 *q;
    u32 *dp;
    u32 *dq;
    u32 *qinv;
    u32 *pinv;
    u32 *std_in;
    u32 *std_out;
} RSA_MODEXP_SAMPLE_TEST_VECTOR;


u32 rsa_modexp_sample_test_internal(RSA_MODEXP_SAMPLE_TEST_VECTOR *vector)
{
    u32 out[128], out2[128];
    u32 *p, *q, *dp, *dq, *u;
    u32 eBitLen = vector->eBitLen;
    u32 nBitLen = vector->nBitLen;
    u32 i, wordLen = GET_WORD_LEN(nBitLen);
    u32 ret;

    printf("\r\n\r\n --------------------- RSA-%u modexp sample test ---------------------- ", nBitLen);

    for(i=0; i<2; i++)
    {
        //swap p and q
        if(i==0)
        {
            p  = vector->p;
            q  = vector->q;
            dp = vector->dp;
            dq = vector->dq;
            u  = vector->qinv;
        }
        else
        {
            printf("\r\n ------- swap p and q... -------");
            p  = vector->q;
            q  = vector->p;
            dp = vector->dq;
            dq = vector->dp;
            u  = vector->pinv;
        }

#ifndef RSA_SEC_TEST
        //sign(non-CRT mode)
        ret = RSA_ModExp(vector->std_in, vector->d, vector->n, out, get_valid_bits(vector->d, wordLen), nBitLen);
        if(RSA_SUCCESS != ret || uint32_BigNumCmp(out, wordLen, vector->std_out, wordLen))
        {
            printf("\r\n RSA-%u MOD_EXP test failure, ret=%x", nBitLen, ret);
            print_BN_buf_U32(vector->std_in, wordLen, "in");
            print_BN_buf_U32(out, wordLen, "out");
            return 1;
        }
        else
        {
            printf("\r\n RSA-%u MOD_EXP test success", nBitLen);
        }

        //sign(CRT mode)
        memset((u8 *)out, 0, wordLen<<2);
        ret = RSA_CRTModExp(vector->std_in, p, q, dp, dq, u, out, nBitLen);
        if(RSA_SUCCESS != ret || uint32_BigNumCmp(out, wordLen, vector->std_out, wordLen))
        {
            printf("\r\n RSA-%u CRT_MOD_EXP test failure, ret=%x", nBitLen, ret);
            print_BN_buf_U32(p, wordLen/2, "p");
            print_BN_buf_U32(q, wordLen/2, "q");
            print_BN_buf_U32(dp, wordLen/2, "dp");
            print_BN_buf_U32(dq, wordLen/2, "dq");
            print_BN_buf_U32(u, wordLen/2, "u");
            print_BN_buf_U32(vector->std_in, wordLen, "in");
            print_BN_buf_U32(out, wordLen, "out");
            return 1;
        }
        else
        {
            printf("\r\n RSA-%u CRT_MOD_EXP test success", nBitLen);
        }

        //verify
        ret = RSA_ModExp(out, vector->e, vector->n, out2, eBitLen, nBitLen);
        if(RSA_SUCCESS != ret || uint32_BigNumCmp(out2, wordLen, vector->std_in, wordLen))
        {
            printf("\r\n RSA-%u MOD_EXP test failure, ret=%x", nBitLen, ret);
            print_BN_buf_U32(out, wordLen, "out");
            print_BN_buf_U32(out2, wordLen, "out2");
            return 1;
        }
        else
        {
            printf("\r\n RSA-%u MOD_EXP test success", nBitLen);
        }

#else
        //sign(non-CRT mode with pub)
        ret = RSA_ModExp_with_pub(vector->std_in, vector->e, vector->d, vector->n, out, eBitLen, nBitLen);
        if(RSA_SUCCESS_S != ret || uint32_BigNumCmp(out, wordLen, vector->std_out, wordLen))
        {
            printf("\r\n RSA-%u MOD_EXP_WITH_PUB test failure, ret=%x", nBitLen, ret);
            print_BN_buf_U32(vector->std_in, wordLen, "in");
            print_BN_buf_U32(out, wordLen, "out");
            return 1;
        }
        else
        {
            printf("\r\n RSA-%u MOD_EXP_WITH_PUB test success", nBitLen);
        }

        //sign(non-CRT mode without pub)
        memset((u8 *)out, 0, wordLen<<2);
        ret = RSA_ModExp_without_pub(vector->std_in, vector->d, vector->n, out, nBitLen);
        if(RSA_SUCCESS_S != ret || uint32_BigNumCmp(out, wordLen, vector->std_out, wordLen))
        {
            printf("\r\n RSA-%u MOD_EXP_WITHOUT_PUB test failure, ret=%x", nBitLen, ret);
            print_BN_buf_U32(vector->std_in, wordLen, "in");
            print_BN_buf_U32(out, wordLen, "out");
            return 1;
        }
        else
        {
            printf("\r\n RSA-%u MOD_EXP_WITHOUT_PUB test success", nBitLen);
        }

        //sign(CRT mode with pub)
        memset((u8 *)out, 0, wordLen<<2);
        ret = RSA_CRTModExp_with_pub(vector->std_in, p, q, dp, dq, u, vector->e, out, eBitLen, nBitLen);
        if(RSA_SUCCESS_S != ret || uint32_BigNumCmp(out, wordLen, vector->std_out, wordLen))
        {
            printf("\r\n RSA-%u CRT_MOD_EXP_WITH_PUB test failure, ret=%x", nBitLen, ret);
            print_BN_buf_U32(p, wordLen/2, "p");
            print_BN_buf_U32(q, wordLen/2, "q");
            print_BN_buf_U32(dp, wordLen/2, "dp");
            print_BN_buf_U32(dq, wordLen/2, "dq");
            print_BN_buf_U32(u, wordLen/2, "u");
            print_BN_buf_U32(vector->std_in, wordLen, "in");
            print_BN_buf_U32(out, wordLen, "out");
            return 1;
        }
        else
        {
            printf("\r\n RSA-%u CRT_MOD_EXP_WITH_PUB test success", nBitLen);
        }/**/

        //sign(CRT mode without pub)
        memset((u8 *)out, 0, wordLen<<2);
        ret = RSA_CRTModExp_without_pub(vector->std_in, p, q, dp, dq, u, out, nBitLen);
        if(RSA_SUCCESS_S != ret || uint32_BigNumCmp(out, wordLen, vector->std_out, wordLen))
        {
            printf("\r\n RSA-%u CRT_MOD_EXP_WITHOUT_PUB test failure, ret=%x", nBitLen, ret);
            print_BN_buf_U32(p, wordLen/2, "p");
            print_BN_buf_U32(q, wordLen/2, "q");
            print_BN_buf_U32(dp, wordLen/2, "dp");
            print_BN_buf_U32(dq, wordLen/2, "dq");
            print_BN_buf_U32(u, wordLen/2, "u");
            print_BN_buf_U32(vector->std_in, wordLen, "in");
            print_BN_buf_U32(out, wordLen, "out");
            return 1;
        }
        else
        {
            printf("\r\n RSA-%u CRT_MOD_EXP_WITHOUT_PUB test success", nBitLen);
        }/**/

        //verify
        ret = RSA_ModExp(out, vector->e, vector->n, out2, eBitLen, nBitLen);
        if(RSA_SUCCESS != ret || uint32_BigNumCmp(out2, wordLen, vector->std_in, wordLen))
        {
            printf("\r\n RSA-%u MOD_EXP test failure, ret=%x", nBitLen, ret);
            print_BN_buf_U32(out, wordLen, "out");
            print_BN_buf_U32(out2, wordLen, "out2");
            return 1;
        }
        else
        {
            printf("\r\n RSA-%u MOD_EXP test success", nBitLen);
        }
#endif
    }

    return 0;
}


u32 rsa_modexp_speed_test_internal(RSA_MODEXP_SAMPLE_TEST_VECTOR *vector)
{
    u32 out[128];
    u32 eBitLen = vector->eBitLen;
    u32 nBitLen = vector->nBitLen;
    u32 i, wordLen = GET_WORD_LEN(nBitLen);
    u32 counter_1 __attribute__((unused)) = 0;
    u32 counter_2 __attribute__((unused)) = 0;
    u32 counter_3 __attribute__((unused)) = 0;
    u32 counter_4 __attribute__((unused)) = 0;
    u32 counter __attribute__((unused));
    u32 ret = 0;

    switch(nBitLen)
    {
        case 1024:
            counter_1 = 1500;
            counter_2 = 50;
            counter_3 = 200;
            counter_4 = 50;
            break;

        case 2048:
            counter_1 = 500;
            counter_2 = 20;
            counter_3 = 40;
            counter_4 = 20;
            break;

        case 3072:
            counter_1 = 200;
            counter_2 = 10;
            counter_3 = 20;
            counter_4 = 10;
            break;

        case 4096:
            counter_1 = 100;
            counter_2 = 10;
            counter_3 = 10;
            counter_4 = 10;
            break;

        default :
            break;
    }

    uint32_sleep(0x1FFFF, 1);

    //test RSA_ModExp with exponent 17 bits(0x10001)
    ret = 0;
    memset((u8 *)out, 0, wordLen<<2);
    counter = counter_1;
    printf("\r\n ======== RSA-%u RSA_ModExp test(e %u bits) begin ========", nBitLen, eBitLen);

#ifdef RSA_SPEED_TEST_BY_TIMER
    counter /= 10;
    startP();
#endif

    for(i=0; i<counter; i++)
    {
        ret |= RSA_ModExp(vector->std_out, vector->e, vector->n, out, eBitLen, nBitLen);
    }

#ifdef RSA_SPEED_TEST_BY_TIMER
    endP(1,0,counter);
#else
    printf("\r\n finished");
#endif

    if(RSA_SUCCESS != ret || uint32_BigNumCmp(out, wordLen, vector->std_in, wordLen))
    {
        printf("\r\n RSA-%u RSA_ModExp test(e %u bits) failure, ret=%x", nBitLen, eBitLen, ret);
        print_BN_buf_U32(vector->std_in, wordLen, "vector->std_in");
        print_BN_buf_U32(out, wordLen, "out");
        return 1;
    }
    else
    {;}


    //test RSA_ModExp with exponent nBitLen bits
    ret = 0;
    memset((u8 *)out, 0, wordLen<<2);
    counter = counter_2;
    printf("\r\n ======== RSA-%u RSA_ModExp test(d %u bits) begin ========", nBitLen, nBitLen);

#ifdef RSA_SPEED_TEST_BY_TIMER
    counter /= 10;
    startP();
#endif

    for(i=0; i<counter; i++)
    {
        ret |= RSA_ModExp(vector->std_in, vector->d, vector->n, out, get_valid_bits(vector->d, wordLen), nBitLen);
    }

#ifdef RSA_SPEED_TEST_BY_TIMER
    endP(1,0,counter);
#else
    printf("\r\n finished");
#endif

    if(RSA_SUCCESS != ret || uint32_BigNumCmp(out, wordLen, vector->std_out, wordLen))
    {
        printf("\r\n RSA-%u RSA_ModExp test(d %u bits) failure, ret=%x", nBitLen, nBitLen, ret);
        print_BN_buf_U32(vector->std_out, wordLen, "vector->std_out");
        print_BN_buf_U32(out, wordLen, "out");
        return 1;
    }
    else
    {;}


#ifdef RSA_SEC_TEST
    //test RSA_ModExp_with_pub
    ret = 0;
    memset((u8 *)out, 0, wordLen<<2);
    counter = counter_2;
    printf("\r\n ======== RSA-%u RSA_ModExp_with_pub test(d %u bits) begin ========", nBitLen, nBitLen);

#ifdef RSA_SPEED_TEST_BY_TIMER
    counter /= 10;
    startP();
#endif

    for(i=0; i<counter; i++)
    {
        ret |= RSA_ModExp_with_pub(vector->std_in, vector->e, vector->d, vector->n, out, eBitLen, nBitLen);
    }

#ifdef RSA_SPEED_TEST_BY_TIMER
    endP(1,0,counter);
#else
    printf("\r\n finished");
#endif

    if(RSA_SUCCESS_S != ret || uint32_BigNumCmp(out, wordLen, vector->std_out, wordLen))
    {
        printf("\r\n RSA-%u RSA_ModExp_with_pub test(d %u bits) failure, ret=%x", nBitLen, nBitLen, ret);
        print_BN_buf_U32(vector->std_out, wordLen, "vector->std_out");
        print_BN_buf_U32(out, wordLen, "out");
        return 1;
    }
    else
    {;}

    //test RSA_ModExp_without_pub
    ret = 0;
    memset((u8 *)out, 0, wordLen<<2);
    counter = counter_2;
    printf("\r\n ======== RSA-%u RSA_ModExp_without_pub test(d %u bits) begin ========", nBitLen, nBitLen);

#ifdef RSA_SPEED_TEST_BY_TIMER
    counter /= 10;
    startP();
#endif

    for(i=0; i<counter; i++)
    {
        ret |= RSA_ModExp_without_pub(vector->std_in, vector->d, vector->n, out, nBitLen);
    }

#ifdef RSA_SPEED_TEST_BY_TIMER
    endP(1,0,counter);
#else
    printf("\r\n finished");
#endif

    if(RSA_SUCCESS_S != ret || uint32_BigNumCmp(out, wordLen, vector->std_out, wordLen))
    {
        printf("\r\n RSA-%u RSA_ModExp_without_pub test(d %u bits) failure, ret=%x", nBitLen, nBitLen, ret);
        print_BN_buf_U32(vector->std_out, wordLen, "vector->std_out");
        print_BN_buf_U32(out, wordLen, "out");
        return 1;
    }
    else
    {;}
#endif


    //test RSA_CRTModExp
    ret = 0;
    memset((u8 *)out, 0, wordLen<<2);
    counter = counter_3;
    printf("\r\n ======== RSA-%u RSA_CRTModExp test begin ========", nBitLen);

#ifdef RSA_SPEED_TEST_BY_TIMER
    counter /= 10;
    startP();
#endif

    for(i=0; i<counter; i++)
    {
        ret |= RSA_CRTModExp(vector->std_in, vector->p, vector->q, vector->dp, vector->dq, vector->qinv, out, nBitLen);
    }

#ifdef RSA_SPEED_TEST_BY_TIMER
    endP(1,0,counter);
#else
    printf("\r\n finished");
#endif

    if(RSA_SUCCESS != ret || uint32_BigNumCmp(out, wordLen, vector->std_out, wordLen))
    {
        printf("\r\n RSA-%u RSA_CRTModExp test failure, ret=%x", nBitLen, ret);
        print_BN_buf_U32(vector->std_out, wordLen, "vector->std_out");
        print_BN_buf_U32(out, wordLen, "out");
        return 1;
    }
    else
    {;}


#ifdef RSA_SEC_TEST
    //test RSA_CRTModExp_with_pub
    ret = 0;
    memset((u8 *)out, 0, wordLen<<2);
    counter = counter_4;
    printf("\r\n ======== RSA-%u RSA_CRTModExp_with_pub test begin ========", nBitLen);

#ifdef RSA_SPEED_TEST_BY_TIMER
    counter /= 10;
    startP();
#endif

    for(i=0; i<counter; i++)
    {
        ret |= RSA_CRTModExp_with_pub(vector->std_in, vector->p, vector->q, vector->dp, vector->dq, vector->qinv, vector->e, 
            out, eBitLen, nBitLen);
    }

#ifdef RSA_SPEED_TEST_BY_TIMER
    endP(1,0,counter);
#else
    printf("\r\n finished");
#endif

    if(RSA_SUCCESS_S != ret || uint32_BigNumCmp(out, wordLen, vector->std_out, wordLen))
    {
        printf("\r\n RSA-%u RSA_CRTModExp_with_pub test failure, ret=%x", nBitLen, ret);
        print_BN_buf_U32(vector->std_out, wordLen, "vector->std_out");
        print_BN_buf_U32(out, wordLen, "out");
        return 1;
    }
    else
    {;}

    //test RSA_CRTModExp_without_pub
    ret = 0;
    memset((u8 *)out, 0, wordLen<<2);
    counter = counter_4;
    printf("\r\n ======== RSA-%u RSA_CRTModExp_without_pub test begin ========", nBitLen);

#ifdef RSA_SPEED_TEST_BY_TIMER
    counter /= 10;
    startP();
#endif

    for(i=0; i<counter; i++)
    {
        ret |= RSA_CRTModExp_without_pub(vector->std_in, vector->p, vector->q, vector->dp, vector->dq, vector->qinv, 
            out, nBitLen);
    }

#ifdef RSA_SPEED_TEST_BY_TIMER
    endP(1,0,counter);
#else
    printf("\r\n finished");
#endif

    if(RSA_SUCCESS_S != ret || uint32_BigNumCmp(out, wordLen, vector->std_out, wordLen))
    {
        printf("\r\n RSA-%u RSA_CRTModExp_without_pub test failure, ret=%x", nBitLen, ret);
        print_BN_buf_U32(vector->std_out, wordLen, "vector->std_out");
        print_BN_buf_U32(out, wordLen, "out");
        return 1;
    }
    else
    {;}
#endif

    printf("\r\n ");

    return 0;
}


u32 rsa_1024_modexp_sample_or_speed_test(u32 is_for_speed_test)
{
    //n: 0xEAC6532105DC60FF034A728850F3297ABB6B624F13B0A673A11C9496DCF5870E4ADD551BD9E8D3DE024A8D7E021A8D8BD4C45D10EBB5A873328CDDC58A8B35AD
    //     0D39E5A746666B3AE50ACB957F1FB804B651F00A78D589A2521851E78A1158C4EA77C2241929C590C7F8C4AEE27E76F9E397CA53E16CCCD2CFF30D5FB9BFE221
    //d: 0x7788F2DA657987894FCB9FF4F80256A43EB8B61D3D113C2EB36131E67E2A24D18A1C9730CF0A54953CE5689537CCFD34ACE82DDA67A8DE5388F160192C2924F2
    //     357B050D9B7FB00E3E97DA299BEFDA9A7A1DD9918F19334DB9504D56D99BD90523AC513DFFF374A677818237883C69382DCAF14B3A7522743241451722A31201
    u32 n[]=
    {
        0xB9BFE221,0xCFF30D5F,0xE16CCCD2,0xE397CA53,0xE27E76F9,0xC7F8C4AE,0x1929C590,0xEA77C224,
        0x8A1158C4,0x521851E7,0x78D589A2,0xB651F00A,0x7F1FB804,0xE50ACB95,0x46666B3A,0x0D39E5A7,
        0x8A8B35AD,0x328CDDC5,0xEBB5A873,0xD4C45D10,0x021A8D8B,0x024A8D7E,0xD9E8D3DE,0x4ADD551B,
        0xDCF5870E,0xA11C9496,0x13B0A673,0xBB6B624F,0x50F3297A,0x034A7288,0x05DC60FF,0xEAC65321,
    };
    u32 e[]= {0x00010001};
    u32 d[]=
    {
        0x22A31201,0x32414517,0x3A752274,0x2DCAF14B,0x883C6938,0x77818237,0xFFF374A6,0x23AC513D,
        0xD99BD905,0xB9504D56,0x8F19334D,0x7A1DD991,0x9BEFDA9A,0x3E97DA29,0x9B7FB00E,0x357B050D,
        0x2C2924F2,0x88F16019,0x67A8DE53,0xACE82DDA,0x37CCFD34,0x3CE56895,0xCF0A5495,0x8A1C9730,
        0x7E2A24D1,0xB36131E6,0x3D113C2E,0x3EB8B61D,0xF80256A4,0x4FCB9FF4,0x65798789,0x7788F2DA,
    };
    u32 p[]=
    {
        0x88E09719,0x1EA7A476,0xA345B62A,0x3361BDDD,0xD445FA6A,0x6F1A7691,0xF6267649,0xCD4E1D95,
        0x38A13B37,0x7A1B8FF0,0xFFE2E0DC,0x5B88DCEF,0x5BFF1C83,0xA4DB0C00,0xBA770091,0xED630D26,
    };
    u32 q[]=
    {
        0xA3B0AC49,0x119CFBD7,0x5B9C4781,0xD7585D6E,0xA2A8CB35,0xD858CC7A,0x0CD84FA1,0x9F256E77,
        0x0510B779,0x70F3AF31,0x554464D6,0x38F6F4AA,0x1840A831,0xEC0DEC9F,0x81E3E97A,0xFD2ED6F6,
    };
    u32 dp[]=
    {
        0x10234101,0x48D54C83,0x7B2E16D4,0xD5CF242F,0x995265D9,0xF39FC6FE,0x34E2762D,0x597D9777,
        0x8F1089BB,0xAB0765DB,0x8718DB05,0x42BB42E1,0xC113930C,0xEEC5D0AA,0x8B562865,0x2DC8CCE0,
    };
    u32 dq[]=
    {
        0x1CBBE1A9,0x355D9E7B,0xC648E880,0xE6B54427,0x435CED4C,0x89092E4A,0x8272B807,0x9A929CAF,
        0x1CE4F1C5,0x697889BD,0x6FA23772,0xAD964C3A,0x2FA9FBB4,0x391DB805,0xF13A1B7B,0x90B04CFE,
    };
    u32 qinv[]=
    {
        0xCD545DEF,0x7054F2EE,0x112844E9,0xE1FA120D,0x5AC1F87E,0x7A9F0AC0,0x6EA168A1,0x38FA13B9,
        0x26374559,0x4C9525E0,0xEAAFA157,0x07E6994E,0x4EEC4549,0x2CE40491,0xDBEE37CD,0x1A938C74,
    };
    u32 pinv[]=
    {
        0xEB8E2333,0xA1A3305E,0x262A4A5C,0xF9D2AB94,0xB916FC6B,0x28BDBDF0,0x17DF7D07,0xC3CB5638,
        0x304FCDF2,0x4508218C,0x19D83F98,0xB961D5D2,0xE75380DB,0x64385B4B,0xFC699A3E,0xE0D692DB,
    };
    u32 std_in[]={
        0x8380A8E3,0xB68BBD8D,0x9B12715F,0x8197BACE,0xA6537B67,0x6CBBEDC0,0x1E4D9F79,0x7199D7E7,
        0xD55A58ED,0x9D5CAE97,0xC2F6E3A4,0xDA3846B2,0x0ABBB632,0xB3107D1F,0x3E6A5213,0xD728448D,
        0x52B6F661,0x6C6C18F0,0x9DD5850A,0xF8297F3A,0xCD78E0BC,0x0168DCD7,0x617001B7,0x10AC0DC9,
        0xA6040D7D,0x48F25934,0xD060798D,0xDF5CC3D7,0x5AB42AC1,0xEF364428,0x241D6D81,0xD4F70209,
    };
    u32 std_out[]=
    {
        0x7C7D7E7F,0x78797A7B,0x74757677,0x70717273,0x6C6D6E6F,0x68696A6B,0x64656667,0x60616263,
        0x5C5D5E5F,0x58595A5B,0x54555657,0x50515253,0x4C4D4E4F,0x48494A4B,0x44454647,0x40414243,
        0x3C3D3E3F,0x38393A3B,0x34353637,0x30313233,0x2C2D2E2F,0x28292A2B,0x24252627,0x20212223,
        0x1C1D1E1F,0x18191A1B,0x14151617,0x10111213,0x0C0D0E0F,0x08090A0B,0x04050607,0x00010203,
    };

    RSA_MODEXP_SAMPLE_TEST_VECTOR vector[1] = {
        {
            17,
            1024,
            n,
            e,
            d,
            p,
            q,
            dp,
            dq,
            qinv,
            pinv,
            std_in,
            std_out,
        },
    };

    if(is_for_speed_test)
        return rsa_modexp_speed_test_internal(vector);
    else
        return rsa_modexp_sample_test_internal(vector);
}


u32 rsa_2048_modexp_sample_or_speed_test(u32 is_for_speed_test)
{
    //n: 0xA7C5D82068EC9949...3958A753BB5248B9
    //d: 0x7FBA29CE92D2FDE8...E52AE46DBFBF5039
    u32 n[]=
    {
        0xBB5248B9,0x3958A753,0x8374E33D,0x361B77F2,0xCB85F949,0xCA8E668C,0x66EDE655,0x3DDEE69E,
        0x8AACE682,0x99F46E90,0x4492985F,0x29F0F9C0,0xEBF0722B,0xE0179832,0xCF18AE20,0xE665C7C1,
        0x0CBE0F3A,0x40B1FD85,0x55F30FEC,0x667DD0CF,0x0445664A,0x324C8E9A,0x86ACD519,0xBB6B6E72,
        0xF2B5D9FE,0x147B9465,0xD1A21022,0xCBA4B564,0xFD868F9A,0x5FBDFE51,0x66635C00,0xD477C1B2,
        0xD154302A,0x60BD6712,0x40451294,0xBD3D93D6,0xB99E7133,0x42BF8DF8,0x481A2235,0x4F4290A2,
        0xFE90B45E,0x892B8244,0x0A33F5FA,0x381E10F9,0xA2B41B34,0xFCD8D1E4,0x865B96DA,0xF46D1760,
        0xCC5085D7,0x88EA540B,0xED35377E,0x07160284,0xFA1C1603,0xFB6CC61F,0x0C76B0BE,0x56353B18,
        0x3ED0F658,0x4C9FA8B4,0x59CC2863,0xC2128C81,0x371A8B13,0x6C807C1E,0x68EC9949,0xA7C5D820,
    };
    u32 e[]= {0x00010001};
    u32 d[]=
    {
        0xBFBF5039,0xE52AE46D,0x52610295,0x72AAF83F,0x28D40FA1,0x270A858B,0x8DEAB989,0x78ED0FB3,
        0x29946A0E,0xC56355E4,0x3F186B53,0xD486B4B3,0x63F56868,0xE450D0AE,0x1722616A,0x77199EFC,
        0x02FAEA03,0xBE0DA0CA,0xB94C869D,0xA9A94DD5,0x17082AB9,0xE71BEE19,0xDDF45A1D,0x7D1BE624,
        0x65397435,0x40B8B56E,0x4B591AA6,0x13FC9A6D,0x0FD48ABF,0xD7B78EDB,0x71C18BCF,0x073EA51C,
        0x6583789F,0xFA407806,0xEDC5285D,0x6DEE416C,0x5A2C7BF6,0x71BBAFF4,0xEEB3A6B3,0xA9ABFDBC,
        0x8391DB1F,0x667ACF54,0x1687748F,0x50ECE2F3,0xF7E9B284,0x9B0EDECC,0x3D4EE14C,0x5C7EE93B,
        0x0B048AC1,0x2DCE686A,0xE03B91A2,0xECBB1EF3,0xD88E255B,0xEE2863B5,0x8C9D97F5,0x062BC46F,
        0x8BE63E86,0x37A59DA4,0x126551A2,0x3B462FDE,0xEDB1B18C,0x5E487B35,0x92D2FDE8,0x7FBA29CE,
    };
    u32 p[]=
    {
        0xD2C5399F,0xF3EF5110,0x04970D25,0xE64CAEC5,0x5D5A068D,0xA7CE6463,0x80E71F9D,0x69331C79,
        0x45871E7C,0xA0B6B1D0,0xB5B27E5B,0x538F84A9,0x8DB003F7,0x68356ACE,0x42252C1D,0xDD889308,
        0x9D55A1E2,0xB51B0532,0x6D605FBA,0xEDAE1122,0xB5F34AF5,0xB552403E,0x7CC23ED4,0xA10075A3,
        0xB88DD8E3,0xDA3A1A7C,0x46942D7E,0x718EBCF2,0x339F1881,0xFF79A8CA,0x394262E7,0xD38DFA8E,
    };
    u32 q[]=
    {
        0xC39D0EA7,0x7C85F825,0x479D951E,0x65D3CC95,0xB544E463,0x5D704D69,0x7D3B641D,0xE5D7637B,
        0x3BB96DE8,0x3CF3EAFE,0x6E36C52E,0xB8148229,0xBB82F49E,0xE6A13EFE,0x5BA6BCA1,0xD0C9CCF0,
        0xE3B07AE5,0x67EEF090,0x6B9D44C0,0x6AF163AC,0x53749666,0xB90FAA34,0x770B3AB0,0x727A2482,
        0x0633A781,0x767780EA,0x1A0CBC6A,0xEB01038D,0x1E1FD9C1,0xA2D9284B,0xDED4E7A4,0xCB052788,
    };
    u32 dp[]=
    {
        0xDB507F89,0x23B7FCFD,0x5C1C714E,0xC6D07B7A,0x9357ABA1,0x6739F404,0x929A6045,0xCD381F53,
        0x14E8EB0D,0x0EA5CD18,0xBFCF414E,0x0067D48B,0xF19AC916,0x5A536CA3,0xD45FA412,0x47C0DF86,
        0x0B4A9B2E,0xB9644313,0xB1C99B7B,0xFA188B68,0x59063167,0x9DF56E18,0xF133AC96,0x0A299C90,
        0x61CE43C5,0x2F6C834A,0x8A572FDF,0x8EE58583,0xE8151555,0x4AE7E982,0xC79D7147,0x4CD6F8F6,
    };
    u32 dq[]=
    {
        0x10101715,0x0F98249C,0xBB96FB4E,0xE5BFD64A,0x6BA46F1C,0x9940C563,0x8F40084E,0xA37A7032,
        0xEDA54D31,0xAE6AC537,0x7DC043DF,0xCA9EA769,0x8E982024,0xBD410FA8,0x971F310C,0xA5EA69CF,
        0x95F7FDDA,0x7783167C,0xABA3CECF,0x0F3B41FD,0xA45AFA22,0x89F0CCF2,0xE173F472,0xB7283956,
        0x257EE399,0xDE489159,0xD8C9E2D1,0x1E7364EB,0x22564A75,0x7EBD126C,0x8041927F,0x847AF297,
    };
    u32 qinv[]=
    {
        0x273134B8,0x811F3B02,0x61385B16,0x4940D62E,0xAC11E62E,0x90657139,0xE0BA820D,0x3517F5E3,
        0xA0152677,0xFC65FE57,0xCF28A8AF,0x65E9080E,0xD88F479E,0xF9775F4D,0x60BF9490,0xB2BAEE6E,
        0x5A68CD17,0xB549B32F,0xF86E8A0F,0x0376A3B5,0xB34A8B3D,0x93B8C51A,0xE4A04408,0x2250EA7A,
        0xD905105E,0x2367E8BE,0x8C6F6643,0x0123C7C4,0xE9C2A57D,0xE3939FE8,0xFF8A801C,0xC99AC5E1,
    };
    u32 pinv[]=
    {
        0x6887920E,0xA7E082B0,0x184406E2,0x7935D005,0x0A285FA5,0x8343EA67,0x641C1E74,0xFF7F188F,
        0xD595F84F,0xBF3A8E2D,0xED1675E6,0x183B3618,0x48D28D47,0x690389C0,0x1633FF2E,0x227D31F1,
        0x70E2B96C,0x47C5AD9B,0x70AF87F7,0xB6A0D5E6,0x4CC52870,0xFC820095,0xDB4DDCE3,0xC45A2CA8,
        0xDBB176B6,0x09D274E0,0x09F08707,0x627C1B7F,0x3BDBD479,0x9F6E4A16,0xE1BCF438,0x098C7279,
    };
    u32 std_in[]=
    {
        0x65DE5107,0xEC26699D,0x0D76CE00,0x397AD40D,0x0DBD4F97,0xF30D4311,0x48CA76D0,0x5F2723C7,
        0xE3A21752,0xCB6F36A3,0xDA1204DB,0x66DF3D61,0x0334BACC,0xEC127801,0x90AD6D35,0x9307AE25,
        0x4B6F3716,0x74B04B2C,0x61E072D6,0x9DA1A085,0x4E685D85,0x640EC696,0x6AAF6343,0x87351877,
        0x70C97CE5,0x7B035CED,0xCB2C2C9F,0x17620FA2,0x724EBFBE,0x9F61B6F8,0xD58F829B,0x2F299CC5,
        0xCA5D6B61,0x2095905B,0xB3C8958A,0x36DA96E2,0x0DD9993C,0x8C8A76EC,0x91B75607,0xB25A39A3,
        0x8EDB1559,0xE5D2A164,0x3053F1B2,0x4AB1E95E,0x1EEB0CAB,0xD6EF57D0,0x5913D85F,0x64EEC60E,
        0xCFF42207,0x1F8065E2,0x6ED6A7AB,0x705F2F9E,0xD67FFDE5,0x03DAAFED,0x3372EFF6,0x4F58C1A8,
        0x3A1B21D6,0x1AD49793,0x3589FF12,0xA222521F,0x8A78096C,0x749AC189,0x411577B9,0x263BA261,
    };
    u32 std_out[]=
    {
        0xD546389D,0xA281B3D4,0x90F8486B,0x13F7DE45,0xDB95774A,0xAAADA06B,0x62A86A06,0x1D7C267C,
        0xA5997E8C,0x635F528D,0xEB953983,0x8F85C67F,0xB8A03C02,0x00606815,0xA8B67AB7,0xF396046B,
        0xEEE8652B,0xFE8907FA,0x0C8A668A,0x2702D390,0xC1F92941,0x0AFBC3D9,0xC8DA0942,0xB972600C,
        0x6ACAB100,0x67FE190D,0x4EB9C471,0x7004B44C,0xB1525834,0x5464DCDC,0xAA850FE1,0xBF3160CA,
        0xC8821059,0x7D06525F,0xA575568F,0x09AC6B6E,0x232BDDA9,0xA3A4E719,0x7D0D8740,0xAFE7C3D2,
        0x08F11F68,0x619C52B5,0x096DF470,0x06B80A44,0x686D7466,0x639B22BB,0x9328F92E,0xF78260F4,
        0xD184E5E8,0xF8A2DC30,0xB10F7EDC,0x3B292D6A,0x1EF097E5,0x4DE693CD,0x72CA0250,0x431F605F,
        0x1048297D,0xE9B7CAE9,0x5F99D459,0xF3E6A625,0x2B127D27,0x2EE60884,0x234C380A,0x6D47A3EE,
    };

    RSA_MODEXP_SAMPLE_TEST_VECTOR vector[1] = {
        {
            17,
            2048,
            n,
            e,
            d,
            p,
            q,
            dp,
            dq,
            qinv,
            pinv,
            std_in,
            std_out,
        },
    };

    if(is_for_speed_test)
        return rsa_modexp_speed_test_internal(vector);
    else
        return rsa_modexp_sample_test_internal(vector);
}


u32 rsa_3072_modexp_sample_or_speed_test(u32 is_for_speed_test)
{
    //n: 0xB4AB019CB2390D1F...DCC4356E5944DD97
    //d: 0x5773AB4D192BEA81...0C4EE25028EE4009
    u32 n[]=
    {
        0x5944DD97,0xDCC4356E,0x87C33F36,0x9BDCA3A0,0x67063D8A,0x348DFA4C,0x2BD26484,0x0B8CDF3B,
        0x76B895AB,0x1C77ED4E,0x6E1C2A77,0x3861676A,0x78D9CAA2,0x4119F149,0x57108434,0x6793E507,
        0xD5D4F373,0x535D3450,0x1BD46E75,0x6A2D7770,0xDE9711CF,0x1776E704,0x1928B920,0x23A36D7A,
        0x7DE8A29C,0xAA9127E2,0x0EB6D12C,0xD47C0A46,0xA02B9385,0xF69ED371,0x448C8E33,0x412AF937,
        0x44B55A29,0x2E773D00,0x88995157,0x9CFA8A8E,0x5999F539,0xC277161C,0xBC3095AD,0x27A48FE6,
        0xA23F5C69,0xB34FC951,0xEBF30493,0x340FE525,0xDC6D0268,0xCCC2D459,0x36DD381A,0x5F97906B,
        0x0E8460CB,0xCDF2ABA8,0xCD5B4E3F,0x671963E7,0xD3D51828,0x23729B3D,0xBD4A9735,0xC44784B6,
        0x1E726B2D,0x259468AC,0x968D8DDD,0x249182EC,0x9C78E528,0x1E6A1452,0x11976E8C,0x20EA9E20,
        0xD5B2C9A5,0xCA84C973,0xB825EA90,0x52D4D90C,0x5C288DF0,0xEBB3DE52,0xEB00170F,0x521F8D4E,
        0xE690E9FF,0x3D112B89,0xF1C21379,0xA2E0F0F1,0x0C903E59,0xEF5408F3,0xCB6901C4,0x1DB3B5FD,
        0x82B9F5A2,0x78DF23A9,0x4832D1E6,0xB1B42891,0x1C943CF8,0x21B60B12,0x85212AC0,0x222EA6E1,
        0x52CBB22A,0x6F9E6265,0x346A6A38,0xD5AEF54D,0x1D89BFF1,0x8BA9A3EB,0xB2390D1F,0xB4AB019C,
    };
    u32 e[]= {0x00010001};
    u32 d[]=
    {
        0x28EE4009,0x0C4EE250,0x9BACFC05,0x82115E47,0xA43651C5,0x9996E23E,0xDC1B9099,0x5EECB000,
        0x94659830,0xD460EB5F,0x201A8B15,0x6F5B648E,0xADBDE638,0xF71C8348,0x8B45ABE5,0x5878CD65,
        0x2F002A57,0x849024A7,0xE63A4D65,0xAE3D1FB6,0xBA8B22E4,0x6C1DC5A5,0xD2804FEF,0xFFF8E1C1,
        0x1D643A53,0x04CB9B5A,0x02E882FA,0x15319B99,0x2227BD14,0x51602CD8,0x2A12E816,0x4B35B773,
        0x73D09B2E,0x68272518,0x8E054F4F,0x092CC7DF,0x5E6A5112,0x79067F41,0x7AB6FA8C,0x86498D15,
        0x8C093BB3,0x2654B524,0x4C802550,0x31E3EBB1,0x67DE77DE,0xA9E7487C,0xBEF36FBB,0x0092FC47,
        0x2E01C004,0x8200243D,0x66E3FA85,0x28BEAE90,0x69576DE1,0xCD99CD31,0x11E98835,0xFBA18EC9,
        0x87D764AF,0x8CBD40C9,0x48C7EE52,0xE8BE92BC,0xFC67BCAC,0x86885C9B,0xA16B46B4,0x02DFEE78,
        0xA37B6E86,0xC328F186,0x2173210E,0x13F839B4,0x65FCAF70,0x880941E5,0x7A5649D3,0x63D72804,
        0x317615FE,0x46C607E8,0x52A6D9DC,0x7DA71A9A,0x067ACFE3,0x1AC5F7AB,0x093F23FF,0xA4F2F776,
        0x9E95BB23,0x9B238FD4,0xE1FFCF80,0x86D1290D,0x8683EEDB,0x129656D7,0xA9017BFB,0xE806E2FA,
        0xFC83E312,0x8A640546,0x29AD150B,0x1B161C61,0x03184994,0x498E6CED,0x192BEA81,0x5773AB4D,
    };
    u32 p[]=
    {
        0xB349862D,0xF8E70B37,0x440A7B73,0xB662596D,0xC06B0F3B,0xAAF2C3B0,0xEB683822,0xFFEA9D70,
        0x73BF881F,0x932CDC27,0x285D7A32,0xD79F0256,0x388A177B,0x11E3B18B,0xB1F838ED,0xE62BA4F0,
        0xA75C65B6,0x36BA98B0,0x4EC6F2EF,0x38E639D2,0x1780E6C1,0x090EEC4B,0x537775B8,0x84813DB0,
        0xB901FE2E,0x6AC17BB9,0xD6256A81,0x5E0B5EEB,0xAF132070,0xED9A4B18,0x4E0257C8,0x02978FD2,
        0x2316210B,0x731B9427,0xD799C81A,0xC5493241,0xC80FDDB5,0xEED5F829,0xD58145B1,0xBBF6CBF5,
        0xC692769D,0x2344D2F0,0x2592DB2E,0xC240B405,0x894A4388,0xB12E9EBE,0xA2A5C949,0xEB1C2954,
    };
    u32 q[]=
    {
        0x6656F153,0x291250E6,0xCAEB5747,0x963B9381,0x11292D9D,0xDA2E0DC1,0x6B17C00E,0x1E50A7BB,
        0x612DCF13,0x22DD55CA,0xF29A951E,0xA13F3855,0x01F9A085,0x36725BB0,0x6298F97E,0x344A5E65,
        0xC325A848,0x33106583,0xC6D23453,0x1367BF16,0xFEB2720A,0xF2F9A465,0x416D2A95,0x04BDF615,
        0x380E14FA,0xE866CDB5,0xFB187DC8,0xA2D19919,0x6B821712,0x5BEC64D6,0xAFDAFFA3,0x25154437,
        0x06375D37,0xE542A62A,0x10372ABF,0xBF001F59,0xFEA15862,0x76A40304,0xAEF1504B,0x2DA94CBD,
        0x057A9FE3,0xD2A91EA3,0xB3412AB9,0x8BDDB050,0x47462882,0xC49A3C04,0x07888575,0xC4B88030,
    };
    u32 dp[]=
    {
        0x95B89091,0xEF166276,0xFEDFFCF0,0x417FD57E,0xD53E0BA4,0xD76D5FDE,0x5CE6FDFA,0x956AA6D4,
        0x9BD1E80C,0x87385F92,0x849FAB31,0xBC55A333,0xFF6A396D,0xC2223BE5,0xE04607AE,0xF552F5EA,
        0xB5B51CCA,0x70E9D122,0xC040B1D8,0xFE19FDA0,0xCD6F3B23,0xE0698956,0x110668EC,0x38C56F6F,
        0x5D27FFF1,0xDF6A2537,0xBA7FE342,0xBB2295E8,0x5826D2A8,0xCF04E9BA,0x1F15BEBC,0x0FAAE67B,
        0x7649DF89,0x4D91040B,0x517AE994,0xD916C2CE,0x7E1162A0,0xD85841B9,0xA6C5B0B6,0x4BC83E97,
        0x6D3821FD,0x9FB9D96A,0x1F4A2D22,0x700FECB8,0x5AF860ED,0xE3036474,0xADB49CE6,0x605BCE64,
    };
    u32 dq[]=
    {
        0xCD866169,0xA8E8779F,0x66A18FB9,0x017FE1D3,0xCA56B6C7,0x817C4EB0,0xDBB10056,0xBFE58399,
        0xA1C7059D,0x1C2BEED6,0xAC126888,0x3C7F145D,0xFEF2CB2E,0xB0A6D476,0xEDEE8C0A,0xE5C8CA7C,
        0x6A3A07DF,0x5BE4EED7,0xB760C417,0x76B1E2C4,0x2F82ED6D,0x9D08845C,0x9128F0B7,0xBA6A8D13,
        0xDBA6601B,0x6F004331,0xB7780454,0x8B529D6E,0xF6647231,0x437E69DB,0xDE27B926,0xF5DB56BA,
        0xD9203E2A,0x01B18263,0xD601DECA,0x704C1619,0x1F4E9733,0x83ECD16E,0x859A4857,0x6F5825FB,
        0x5BD4CB57,0x8E7C4404,0x27B134DD,0x7DBB8D82,0x8F609134,0x769DBAF4,0x696D9995,0xA5A808AA,
    };
    u32 qinv[]=
    {
        0x392CD6E0,0xCFDA1773,0x9E211A18,0x0CD08431,0x8E93137A,0xE339FEF6,0x0FF6FCC7,0xE769979B,
        0x88C1ADF5,0x02A0A0E7,0xBEA6A2AA,0x41D69EC6,0xD1216B5F,0x0AF1A5AF,0xB7045818,0x55DB1F3B,
        0xF46D2447,0x89C3627C,0x0DFC2C7B,0xE6595224,0x7DCC1165,0x398CBC48,0xE35A6B65,0x90275E2C,
        0x52792F2B,0xAC0EE007,0x66329C10,0x5D63A3EF,0x684DFFB5,0xE7728143,0x3CFA1100,0xE93A23EB,
        0x60261E1C,0x7EC2260C,0x661680B5,0xD08245D7,0x168DA788,0x0ACD47B3,0x243976A2,0x751171D1,
        0xE0FAD7ED,0x8228D8C1,0xD361BD1D,0xB3E53796,0x41806B89,0x79079C08,0x7E06221D,0x3DC88ECB,
    };
    u32 pinv[]=
    {
        0x8A74B1D8,0xC8543698,0x1BEBF20D,0x1144389C,0x10B594A8,0xE73A5CFF,0xB85EAC30,0xA19425F5,
        0x87E1637D,0x3A30DEA3,0xC3D7C71B,0x05312A80,0xE9CEFD49,0xFFB153CC,0x4DB372E0,0x2AA001CB,
        0x54227990,0x092D3357,0x728374C1,0x2CDB7FE8,0xDD8FA0E4,0xE4A59EF0,0xE1D1F1B8,0x61CECF7A,
        0xE044A774,0xDA263582,0xD61BD6E9,0x485B4FCA,0xC16E27BC,0x18595018,0xA8905BB2,0x950AF2C6,
        0xC3E43927,0x6DBC5EDF,0x1F6A38D3,0xEF43AAE4,0xB0A23ACC,0xE61CF6D1,0xA33D4C6A,0xE38E3D4C,
        0x2A5F70C3,0x26E66732,0x22514258,0x727AD45A,0x1FC2E7EA,0x9399C428,0x290C9839,0x910681B6,
    };
    u32 std_in[]=
    {
        0x92A57F82,0x614B78CF,0xA4AE0D1A,0x1B4BED0E,0x03C544AD,0x3509BA08,0xB3383C70,0x70FA3368,
        0x8F2DBA02,0x3E20EA75,0x780E6F9D,0x4B470BAE,0xD26310A8,0xC1304A6A,0xDB1E90BF,0xC1207135,
        0xC24628A7,0xA2B3A64E,0x1C0FDE6B,0x5DAC870D,0x1E20FA2C,0x71E8B4DF,0x27FA6AAB,0x82383CBA,
        0x36226889,0xF2DDDBDB,0x7A4E05DD,0x7B5E1C55,0xE378A5C0,0x7DB83495,0xCF83D6EC,0xD508877D,
        0x79204F7A,0x32446BA2,0x9106C3FA,0x45C964FF,0x0CC53D5A,0xC8341DF4,0xD7F1CEA6,0x2D49E24F,
        0xB0EE7338,0x38DBCE80,0xD5A04C07,0xAB9C5AD1,0x3B58ED94,0xDD25740D,0x282E74D0,0xE2B929D3,
        0xEFC37F77,0x5166ACB8,0x1F28E0C6,0xE585B53C,0x36DDE396,0x14BAC083,0xDAFDE881,0x35ED0FE5,
        0xA6BEEF74,0x4195E065,0xB779F445,0x8B0882DD,0x47B1CE48,0x3A1EF3D0,0xE534D84E,0x361D5BB1,
        0xB56D8046,0x8F5A70BA,0xDB958FDC,0x7B98DC15,0xBC661801,0x1E8D7DDF,0x2016459F,0x73799407,
        0x45F5C76F,0x91BC24C8,0x2806B8E5,0x1DF453E2,0x33E633F8,0xDA0DF8CC,0x45AD6E2C,0xB8DA24ED,
        0x15B10BFA,0x5148D009,0x32F1F9CF,0xCB3E3A62,0x13D37D64,0x092659B5,0x91D0CE79,0xFBD16430,
        0x16C1C579,0x3DC085AB,0x72DDF546,0x8322F6A4,0xA102EB24,0x035BCD7A,0xA2E87814,0x4A483A09,
    };
    u32 std_out[]=
    {
        0xA450CE3B,0x24FA94A2,0xC4E88F95,0x9C9DB128,0xC282A921,0x6592E0AD,0x2696D8A4,0x608D1EA7,
        0xA3668101,0x3FA4ABAA,0x9A3D69E5,0xA2AF8919,0xA240547D,0xF12B4545,0x3EAEE9B6,0x382A2DD7,
        0x81C73C86,0x4DD32674,0x15FCB6C1,0xA8D60D46,0x3014EA8B,0xA46EACEC,0x46FE18D7,0x3979FD4B,
        0xE0F0E7C0,0x741BD34F,0x537A4923,0x435761B3,0xB6E0D349,0xFF0AE6A7,0xC268DA86,0x3EAE5A65,
        0x1A97BA0D,0x473B0703,0x17EB4F86,0xBE7096DB,0x058C8F6F,0x28F724F1,0xF16C7BDF,0xFF614992,
        0xFB69E1EC,0xF9746D2B,0xF6F2F03A,0xF5390993,0xA5E6AD2F,0xE2C9A163,0x183285A4,0x36D40A8E,
        0x8F06AD93,0xE314AD50,0xCF5A6468,0xDC433B6C,0x525232FD,0xE33B1432,0xEF18B7C0,0x7E53A8C0,
        0x22882C76,0x3F8D9C55,0x8E1A2A64,0x176D97CD,0xEC8C211B,0x3215809A,0x48671606,0x96FD4260,
        0x4C490826,0xD8FD5E9A,0x4047B8EF,0x2941B191,0x0D627B35,0xB470D948,0xAB5333E6,0x035101CE,
        0xCC76AE88,0x39A9003A,0xDBE1CB3D,0xE35F3102,0x970674B2,0x4DF82EFA,0xC9F2EF8B,0xD6F819C6,
        0x2BC8B601,0xEA2EFDE6,0x04F5B789,0x69515915,0x4FD2B561,0xE6661088,0x40F6FB50,0x25C3F9B4,
        0x5F169048,0xB7722B27,0xB2A5D5E9,0x6ACB644B,0xA3DD5D04,0xE4EC1C58,0xA7F6A3AE,0x11BAEC23,
    };

    RSA_MODEXP_SAMPLE_TEST_VECTOR vector[1] = {
        {
            17,
            3072,
            n,
            e,
            d,
            p,
            q,
            dp,
            dq,
            qinv,
            pinv,
            std_in,
            std_out,
        },
    };

    if(is_for_speed_test)
        return rsa_modexp_speed_test_internal(vector);
    else
        return rsa_modexp_sample_test_internal(vector);
}


u32 rsa_4096_modexp_sample_or_speed_test(u32 is_for_speed_test)
{
    //n: 0x9D818C140584075B...27ACB0B7BB3E4243
    //d: 0x827CEC273E3696BD...EF0F0517EFC58C59
    u32 n[]=
    {
        0xBB3E4243,0x27ACB0B7,0x41408F95,0x1CEBE95F,0x0AA58A89,0x9B6CF8BE,0x483629C0,0xE8921BCB,
        0x5697DB7C,0x663E2EA9,0xDC36E607,0xA738A884,0x222A3517,0x2394B61E,0x2619FAA2,0x8438C2E9,
        0x6778D284,0xA3BFAC4A,0x36B33248,0xFFFA3623,0xE9609AED,0x0B564C2F,0x1A46A2F7,0xAA09F2BF,
        0xEBB7AE47,0x0E46F157,0x9EE119FF,0xA814B8DB,0x557D4917,0x8F732EE6,0x41593C91,0x07B12ABD,
        0x6A61BD10,0x6826B484,0x661EF3F1,0xDF8E183F,0xE4EC805D,0x53A6664A,0xB5699FC4,0xF34486C6,
        0xFF728227,0x80231905,0x09FCE35E,0xAF60A847,0x3901D332,0xEB9D1233,0x922A0426,0x8863DB9E,
        0x0C2B22E8,0x4476D10C,0xE673F506,0x8EB0A4A2,0xF8A4FB18,0x2B01ACF0,0xA2D122AC,0x97E2FBC3,
        0xA40A1CE5,0x00E47563,0x25493C75,0x3C6F5989,0xAC7EE05D,0x762338B3,0xC3E03977,0x98471761,
        0xAF92D844,0xAA761798,0x49DE310E,0x795A4076,0xB6E04817,0xD7D646F9,0xED3E62B2,0xE5519DC2,
        0xA15E0C96,0x70291DD7,0xC60F6581,0xCD8F68A2,0xEB9C54A0,0x510CB72A,0xE987B728,0x13A9C60C,
        0x15AFEE45,0x16E321A5,0xEDA6CDEF,0x41D97761,0x867CB4D0,0x268B851F,0x98C1F841,0x50D86CDC,
        0xB32D5A65,0x23DF0CF1,0xF173431D,0xC03BFE22,0x96A1EF85,0x912244F7,0x784C47A0,0x244EB554,
        0xA2306EF5,0xF70DF39F,0x9A2B4CD1,0x61AC28CF,0x49C57F03,0x3DBB842E,0x62C9F6BC,0x019350C5,
        0x12EC3188,0x088DEAEA,0xB2A67CD7,0x296A1121,0x3DA0620E,0xF0F46C56,0x7237CF1C,0xAE606B95,
        0x4EEEA4B2,0xCC068682,0x318F467A,0xC029306A,0x4184C1AC,0x47DD2F6D,0xB3CDD4AE,0x6297D772,
        0x327F0C78,0x6CCF3487,0xB0E7EA88,0x41397D62,0xF7E79557,0x653F652A,0x0584075B,0x9D818C14,
    };
    u32 e[]= {0x00010001};
    u32 d[]=
    {
        0xEFC58C59,0xEF0F0517,0x2B3A0BAF,0xB50800B6,0xF4B15DBB,0x8148F0FE,0xF0140EFA,0x074E2749,
        0x41FDDFFE,0x0D89D041,0xDDEE0182,0x40535700,0xD9F22675,0xFA090359,0x61FF7C8D,0xCC133368,
        0xD559AA85,0xE6990C5A,0x628C4653,0x820DBF26,0xAE4C264B,0xF5496C09,0x20CD7BC4,0xC0F6BADA,
        0xD878EF55,0x982ECC2A,0xB7AD4CA5,0xCB015CB4,0x9DFA63B7,0xA79B6229,0xDE7ED010,0x37F5C848,
        0x50F06DC1,0xCE2AE04E,0xC0877F02,0x13983C93,0x7D29EF2F,0xE6F3BE3E,0x772BBA8C,0x78917DFA,
        0x9882F310,0x977F92EF,0xCB07458F,0x26B00182,0x5C9AF156,0x60B82EE8,0x4E2466E6,0xB635CF10,
        0xC954B4F2,0xE643F335,0x0D607F4A,0x21EADEBC,0xD0BEB2F4,0xDEB2E2D8,0xD562BAA6,0xF69C6864,
        0xB02C8342,0xDBD5B492,0x1626BB77,0xB62126A2,0x6FC8F209,0x8B6D0E1E,0xECC0CCA6,0xB4F7542A,
        0x26C43A84,0xB2CD6959,0xC8E8DAAD,0xAD99F6E3,0xECD93D00,0x97FF3F07,0x998760CF,0x3186FF73,
        0x7BF0E5EF,0x5C57C410,0x9845F32B,0xA02A9A5E,0x7C9E2482,0x4C79835A,0xD39A8B44,0xB350A8F2,
        0xBAF2E92C,0x256D00D7,0x22845A02,0x5150B016,0xF70363ED,0xE6961A30,0xFFD35B2F,0x0719767E,
        0xA8FAFEF4,0x46A4A91D,0x4240E2AE,0x710282D6,0xCBB3D733,0xFBE86F48,0x8E766BB4,0xC17CAFBF,
        0x75A2389A,0x1FFBA005,0x0E08A779,0xCFF9884A,0xC7B66636,0x67C56D89,0x7A35A643,0xE5CA3D55,
        0x1D812EBB,0x7617D532,0x1807D44F,0x34C659F1,0x05D154CB,0xEEB620F7,0xD619A46D,0xECD796E9,
        0xE036D79D,0x9D1E1EDB,0xB15D68DF,0xC6939937,0x243CAF13,0x074F98D6,0x33C267DC,0x759D2472,
        0x64E45AD9,0x150B4912,0xFBBCF752,0x755F02A0,0xD5FF377B,0xC250D628,0x3E3696BD,0x827CEC27,
    };
    u32 p[]=
    {
        0x6E398BED,0x32AB03AF,0xB38E38F2,0x3F2E7A3F,0x565D8581,0x8F57E4C1,0xA0E13806,0xFE3780EF,
        0x1ACE9C56,0xFC1DEC74,0x54E53C69,0xA6D87112,0x0D7A9AB5,0xD6E4E405,0x3BD3148A,0x6FA6C209,
        0xFC458EFC,0xCFFDDA6F,0x4FF3BE98,0x0A4A1C8E,0x474AFE4A,0x322433EE,0x9F0107C7,0x2FE3D0B3,
        0x3395B427,0xC92B0265,0x183A7871,0x87D42CC5,0xB24AD74F,0x7F0816CF,0x1ACDBD59,0x412DA54F,
        0x547BF34C,0x476F3BAD,0xBCC8BDC0,0x366EBDE9,0xA33EA735,0x71EB0880,0x42A68EAC,0xF3F2A351,
        0x9481A948,0xFA77BE2C,0xBF2F7316,0x78BB6637,0x542FCFB7,0xF0919C7A,0x54031904,0x09C9C73F,
        0x19F4CE39,0x8CD72063,0x0A8543D7,0xF00183F4,0x437258DB,0x0D209BB2,0xBD5E1365,0x786E14AF,
        0x6AF35B8D,0xEF139552,0x0A6C1839,0x0F30CB7E,0xB2A5F6F9,0xA13362C3,0xC54164B6,0xCCC44439,
    };
    u32 q[]=
    {
        0xAF4320EF,0x8ECCD1FA,0xBE1F8AE4,0x9D0C0C56,0x49025CFA,0x1F6224A5,0x72B40453,0xC62E01FF,
        0x6F146D59,0x8AB8A843,0xD7F6F422,0xCA147CD9,0x78625EA6,0xD91BC4B6,0x2954BE7C,0xFA4C673D,
        0x75427994,0xD29DC754,0x2E82F1F2,0xFAFC8024,0x96CE9658,0x38CA488F,0xC513CDC9,0x5FCB6D50,
        0x40B6717F,0x04E87686,0xF08CA191,0x1962EA84,0x1DA3F1E9,0x62A6F4E4,0x090B757D,0xAB6845AD,
        0x0E004FB5,0x9F58016B,0x204B6BD7,0xA2EF5817,0x49640C37,0xC5B18DB6,0x35484B6D,0x45722C94,
        0x8A5AAAE2,0x1DF3FDEA,0x4E56A319,0xF04E82C7,0x9CC01B3A,0x4AB13C56,0x11B6F605,0x20B95F23,
        0x26A2C3C1,0x3A00D23A,0xF4B1614A,0x9C37D84E,0x038A1DF0,0xA334B566,0x992C2A3E,0xB1975139,
        0x0A62D321,0x49177AEF,0x71C6A5EC,0x0A645045,0x95C04A77,0xB8979ED9,0x446AE998,0xC4EA238F,
    };
    u32 dp[]=
    {
        0xE5799D39,0x9A1CC297,0xA31A2792,0x5FC2387C,0xF063C4CC,0x11928FF9,0x572A0E1F,0xFB6ADCA2,
        0xFA228739,0x4D28D038,0xD4A42B6D,0xAD65B32D,0x3DDC43FC,0xA4B162C2,0xFA4DF45E,0xB817EB15,
        0xE9EADBAF,0x3BA295EC,0x6034FFFD,0xF03707C0,0x6D0A8D24,0x0B63136E,0xB29B290B,0xC9931094,
        0x05BFD648,0xFD76800F,0x584AF029,0x8D4F815C,0xEFF5B2A9,0x0C7E9112,0xC5EB9B9D,0x175C767D,
        0x301EC8E9,0xCB285B32,0x765B8889,0x2236C830,0x53297C20,0x1CC15923,0x4DA00C39,0x7636298D,
        0x3C480D6B,0x4EA88CA2,0xC4CFF6F3,0x43452C1F,0x985F829C,0x3316F568,0xBDC8FB67,0x149F922A,
        0x4F771B80,0x16EA159B,0x9E4ACB16,0xC2A1D4BD,0x5E7511AD,0x0859A11F,0xAD78A1F8,0x365B2FE4,
        0xD73808D7,0x634E3E3F,0x0021D18B,0xA1A46B47,0x1D7186C0,0x7A6AA52C,0x08B67EB3,0x1D39DE2B,
    };
    u32 dq[]=
    {
        0xB31A719D,0x1797E4C4,0xA6890501,0x7786645D,0x88BC8431,0xF498F8F0,0x21836AB3,0x906E95AB,
        0xB7BD61E5,0x8F44CCDC,0x667B457F,0x12832C92,0x566AA2EE,0xF2E68BBB,0x310EB5F0,0xE7BF713C,
        0x2693A35B,0xAF107371,0x496A52FA,0x1374C5B8,0x48D4865E,0xA7AF6175,0xCA011C9B,0x2BE02F07,
        0x8BD2C776,0x4877EC77,0x66EEA0AE,0xBC682A55,0xA8E694D6,0x513016EB,0x6ED22EBB,0x73152E2B,
        0x12A818C8,0x1297C082,0x62AC07C4,0xB2F75991,0x0FBC57B8,0xC8295594,0x82659456,0xE9350A5C,
        0x843D2C04,0x2F9AD4B9,0x35667E91,0x6E97B01A,0x4808E49A,0x7509019B,0x1C4A7D11,0x895F7503,
        0x6CBBD5DF,0x1A0A6B79,0x6DDFC198,0x6D87F259,0xD4223808,0x4CBFE0F0,0xA1CEB423,0xC896876A,
        0xFFA5D1C8,0xA9206F71,0x3E98BD0F,0xB65296F5,0xACDFC0D6,0x632D2830,0xD1E7C2FA,0xC43E9C45,
    };
    u32 qinv[]=
    {
        0xEF4906CD,0x535332A5,0x964B8FC6,0x3BF17B0A,0x6F2A359E,0x413C68CA,0x892EA1E3,0x437E2C90,
        0xDC837BEB,0xF9E80AA2,0x6DEB95F9,0x55C85E78,0x71EB8333,0x5F815BF2,0xA6A46DB6,0x3BEA94D0,
        0x4D3F774D,0xBAE32518,0xB5505E0A,0xE30BEAC9,0x9E14C3E5,0xB037D667,0x141DF9F8,0xA28924D9,
        0xFC2918EA,0x291CE43B,0xA92B80DE,0xF8F0CA55,0x8CF11EB7,0xFC990287,0x6F65740E,0x2A2EEB7D,
        0xCB61AF03,0x8F970D5B,0x8D1B9A26,0x224F7708,0x679D6076,0xBA69497F,0x4D1E1030,0xAB541089,
        0x1F94E5C6,0x569D9272,0xC72B348D,0x251E38D5,0xE8C73EF0,0x2B352D4A,0x3E8F8C1F,0x6468A993,
        0x832F1372,0x7B752EF0,0x4A3D700B,0x1A21EBDD,0x25EACFFC,0xECEFC3FA,0xD0EC4C79,0xE0FC2FBC,
        0xA7E65DE2,0x9A2B594C,0xC43C2C06,0x2D0C822F,0x3DC22B69,0x3DBFDF17,0x28F46E9A,0x8A88EF32,
    };
    u32 pinv[]=
    {
        0x40459A45,0x80443E02,0x3CC12549,0xBFCBB655,0x1493B7BC,0x8569EA5E,0x90ED5246,0x77F6C340,
        0x23AF1F59,0xDECA4F85,0x147C4918,0xDD829375,0x61FF15D8,0x002DC374,0x58CA345A,0x07F1D907,
        0xEEB2DAA2,0xDD2CF083,0x268FEF0D,0x4244ABFB,0xDA439AC7,0xB4399B93,0xD5F3700B,0xA3E1FDF4,
        0x3B044563,0x1376F3B8,0x73955B6D,0x89619AB0,0xFC3B77DD,0x9C0A5BC6,0xF1DDFA64,0x92DEAE7F,
        0x4C76A147,0x1B93F652,0x45C76B8B,0x9ED38384,0x9CC7AB87,0xF21D2BF4,0xFCEB468C,0xE85F0907,
        0xBCA6F085,0xCE1CF25B,0x2331BE69,0x17308984,0x51034797,0x628D1574,0xB9A2253E,0x4B2BC905,
        0x2DF079DB,0x57A38A28,0x55174351,0x3648C458,0x361A7E41,0x7EF76794,0x21117021,0x937AAECF,
        0xE0CEBB17,0x297C2F99,0x0665B6BC,0x0DA838AB,0xFD1CE917,0x6B56D2F4,0xD797CB6D,0x3FB1280F,
    };
    u32 std_in[]=
    {
        0x5DFBF726,0x5EDF920F,0x2C6FBC35,0x216BB3DF,0x676C3526,0xE7891D10,0x8F1E3961,0x1DC36F63,
        0xC276A87A,0xB5681B77,0x360156B1,0x62456253,0xAAA85E9C,0x3ECADB05,0x9912DA41,0xD623F7DC,
        0xF5F18B71,0x8816DA8A,0xEA5C26DF,0xE25396F4,0xA58FDD02,0x0B16A18F,0xBBDC579B,0xF74B6D37,
        0xD554F13D,0x49923779,0xB76E6EF4,0xF2F7E936,0xBCB81F39,0x255C8F9D,0x6C1C4AE5,0xC4D2EEC7,
        0xDFFA2FE4,0xE82769D8,0x991E9AF1,0xA5C2ABAB,0xAD7DCF43,0x90F733BF,0x3AA1C1FC,0x8DBB1417,
        0xE72A29E1,0x5AD59D45,0x30EE4D52,0x6CFEE5FD,0x744E6583,0x2957B491,0xB3FA767E,0x25A3897D,
        0x94D059E6,0x45ED9A1C,0x5DBB5DF5,0xFF692B25,0x5A10F7AA,0xF28F4F55,0xFDE5CBA2,0x479843B0,
        0x49E3175D,0x79B2CBCE,0x1FB0C6BC,0xED739AF8,0xDC10E5C8,0x5139E7FA,0x63872534,0x073E5F90,
        0xFE2D17DF,0xB17D111B,0xFA57F75D,0x704982D2,0xEC79126B,0x6BAC2356,0xD5220343,0xC7C0F9AA,
        0x6C7CF5A2,0xCFBF0582,0x788C305C,0x3990C845,0xEBA12CA4,0x82D420E7,0x55A712C5,0xDE767648,
        0x6AAEA5A6,0xC1F13A35,0xEBAABDE7,0x5DD445F7,0xBEA107B4,0x4802EDDE,0x7ACF3C03,0x9B6F674D,
        0x90770900,0xAFB6EDCC,0xA85B52B2,0x5A7C9970,0x9B7D7653,0xA5BD01C8,0x357B0BC3,0xAADD26AA,
        0xDB7FE6FA,0x14623989,0xAA00B6F6,0x388A5C56,0x73230AB8,0xC3BF5E53,0x04D34F4D,0xFB9F84DF,
        0xFF1C4407,0xC77EE6FB,0xC8C5419B,0x43499208,0x9C4E55E8,0xE8483B5E,0x7EAB2875,0x22D75118,
        0x662FE1E6,0x512B5999,0x26D252C3,0x7BEA1D49,0x6467B0FF,0x4879FEB5,0x02B2DCBF,0x518CE56B,
        0x67DE0CBB,0xF1CF8E0D,0x6C1C2703,0x628D8B87,0x0BA9EE79,0x5CB74C74,0x7CD3B62F,0x79212C23,
    };
    u32 std_out[]=
    {
        0xBC2F985D,0x70A3093A,0xA584EB16,0xD3E96601,0xEFA218E5,0x65428D64,0x467AB95A,0x8A7C785F,
        0x6D302DB4,0x65637848,0xB6FD4488,0xD320239F,0x95E2D776,0xDA9EB2ED,0x4146AC32,0xF106E5A3,
        0x07378E82,0x1E30F640,0x25610BFD,0x683D2DF3,0xC7605A0B,0xA67D0EDB,0x94D42737,0x06324A9A,
        0xC448DE5E,0xCDAE7878,0x465E5039,0x83CD41F9,0x90C73CC2,0x620E78B5,0x9B959703,0x1BC7AEFA,
        0x4BA84C1C,0x41D7A3E4,0x09A9BB40,0xC1D566DF,0x921ADBA2,0xBC5118DD,0x6E196E6A,0xA09087AB,
        0x5699BCF7,0x168C18C6,0xD1191215,0x96BE6216,0x2A75B8ED,0xA5427CB0,0x7435C7B4,0xB38E30E4,
        0xD68166D6,0xC2DDD732,0x881EC7DC,0x854272F6,0xD707D9A1,0x5EEA503A,0x7209022F,0x99BD4E79,
        0x4A685B2E,0xE6A3C751,0x6D5D9678,0x148FFA6F,0xEE42219E,0x0C3985A2,0x35AC4F5D,0x95BF8551,
        0x9B1F8A93,0xE8813995,0x84951495,0x147E5813,0x2ED38E14,0xA2B5567B,0xDEFE0830,0xF3EF2DD1,
        0x651B22BB,0xB4420C5C,0xFDE31F76,0xE413F30D,0xC8D0EF53,0xCA7A93D9,0xB735A679,0xBE333E51,
        0x25FF6585,0x9790016A,0x021CF285,0xAC8EC818,0x7EDF238A,0xE4670E2C,0x4D64D339,0x405E90C1,
        0x49D0D475,0x416F7817,0x42587844,0x459AFCFE,0x8E85516E,0xA98A5387,0x9F3C4BFF,0x3F0A5AC0,
        0xD3DA7F82,0x1FD55C70,0xAAD8ED7A,0x5FAEF4E1,0xC28D1773,0xE7C950A7,0x4B680CB7,0x3DC46AAA,
        0xC885F933,0x2CBAF65B,0x32E8E930,0x24D89012,0xFDE10A51,0x78BFEFF4,0xB0D82E3E,0x45B9415A,
        0x249FBC16,0x43BC74FF,0x8DC8B715,0xB5DB048A,0x4C1E50E4,0xE6488DEE,0x8D742CFF,0x26584D66,
        0x3E9152C2,0xA36C795A,0xE70774DB,0xB56CB7FD,0x1A5B7671,0x916BE755,0x92C51131,0x6C0AC90B,
    };

    RSA_MODEXP_SAMPLE_TEST_VECTOR vector[1] = {
        {
            17,
            4096,
            n,
            e,
            d,
            p,
            q,
            dp,
            dq,
            qinv,
            pinv,
            std_in,
            std_out,
        },
    };

    if(is_for_speed_test)
        return rsa_modexp_speed_test_internal(vector);
    else
        return rsa_modexp_sample_test_internal(vector);
}


u32 rsa_1024_modexp_sample_test(void)
{
    return rsa_1024_modexp_sample_or_speed_test(0);
}

u32 rsa_2048_modexp_sample_test(void)
{
    return rsa_2048_modexp_sample_or_speed_test(0);
}

u32 rsa_3072_modexp_sample_test(void)
{
    return rsa_3072_modexp_sample_or_speed_test(0);
}

u32 rsa_4096_modexp_sample_test(void)
{
    return rsa_4096_modexp_sample_or_speed_test(0);
}


u32 rsa_1024_modexp_speed_test(void)
{
    return rsa_1024_modexp_sample_or_speed_test(1);
}

u32 rsa_2048_modexp_speed_test(void)
{
    return rsa_2048_modexp_sample_or_speed_test(1);
}

u32 rsa_3072_modexp_speed_test(void)
{
    return rsa_3072_modexp_sample_or_speed_test(1);
}

u32 rsa_4096_modexp_speed_test(void)
{
    return rsa_4096_modexp_sample_or_speed_test(1);
}


//please record the time through uart print for key generation speed test.
u32 rsa_get_key_speed_test(u32 is_for_crt, u32 round, u32 eBitLen, u32 nBitLen)
{
    u32 e[128]={0};
    u32 d[128];
    u32 n[128];
    u32 p[64];
    u32 q[64];
    u32 dp[64];
    u32 dq[64];
    u32 u[64];
    u32 i, count = 0, ret;

    if(nBitLen <= 1024)
    {
        count = 50;
    }
    else if(nBitLen <= 2048)
    {
        count = 10;
    }
    else if(nBitLen <= 4096)
    {
        count = 1;
    }

    if(is_for_crt)
    {
        for(i=0; i<round; i++)
        {
            ret = RSA_GetCRTKey(e, p, q, dp, dq, u, n, eBitLen, nBitLen);
            if(RSA_SUCCESS != ret)
            {
                printf("\r\n RSA_GetCRTKey failure, ret = %x", ret);
                return 1;
            }

            if((i+1)%count == 0)
            {
                printf("\r\n RSA-CRT%u %u", nBitLen, i+1);
            }
        }
    }
    else
    {
        for(i=0; i<round; i++)
        {
            ret = RSA_GetKey(e, d, n, eBitLen, nBitLen);
            if(RSA_SUCCESS != ret)
            {
                printf("\r\n RSA_GetKey failure, ret = %x", ret);
                return 1;
            }

            if((i+1)%count == 0)
            {
                printf("\r\n RSA%u %u", nBitLen, i+1);
            }
        }
    }

    return 0;
}



u32 rsa_get_key_traversal_test(void)
{
    u32 e[128]={0};
    u32 d[128];
    u32 n[128];
    u32 in[128] __attribute__((unused));
    u32 out[128] __attribute__((unused));
    u32 out2[128] __attribute__((unused));
    u32 eBitLen=17;
    u32 nBitLen;
    u32 eWordLen __attribute__((unused));
    u32 nWordLen __attribute__((unused));
    u32 ret;

    for(nBitLen=1024; nBitLen<=4096; nBitLen+=2)//for(nBitLen=1024; nBitLen<=2048; nBitLen+=2)
    {
        ret = RSA_GetKey(e, d, n, eBitLen, nBitLen);
        if(RSA_SUCCESS != ret)
        {
            printf("\r\n RSA_GetKey failure, ret = %x", ret);
            return 1;
        }
        //printf("\r\n RSA_GetKey nBitLen = %u", nBitLen);

#ifdef RSA_SEC_TEST
        eWordLen=GET_WORD_LEN(eBitLen);
        nWordLen=GET_WORD_LEN(nBitLen);
        uint32_set(in, 0x1, nWordLen);
        ret = RSA_ModExp_with_pub(in, e, d, n, out, eBitLen, nBitLen);
        if(RSA_SUCCESS_S != ret)
        {
            print_BN_buf_U32(in, nWordLen, "in");
            print_BN_buf_U32(out, nWordLen, "out");
            print_BN_buf_U32(e, eWordLen, "e");
            print_BN_buf_U32(d, nWordLen, "d");
            print_BN_buf_U32(n, nWordLen, "n");
            printf("\r\n nBitLen = %u, RSA_ModExp_with_pub error, ret = %x", nBitLen, ret);
            return 1;
        }

        uint32_clear(out2, nWordLen);
        ret = RSA_ModExp_without_pub(in, d, n, out2, nBitLen);
        if(RSA_SUCCESS_S != ret)
        {
            print_BN_buf_U32(in, nWordLen, "in");
            print_BN_buf_U32(out2, nWordLen, "out2");
            print_BN_buf_U32(d, nWordLen, "d");
            print_BN_buf_U32(n, nWordLen, "n");
            printf("\r\n nBitLen = %d, RSA_ModExp_without_pub error, ret = %x", nBitLen, ret);
            return 1;
        }

        if(uint32_BigNumCmp(out, nWordLen, out2, nWordLen))
        {
            printf("\r\n RSA_ModExp decrypt error");
            print_BN_buf_U32(in, nWordLen, "in");
            print_BN_buf_U32(out, nWordLen, "out");
            print_BN_buf_U32(out2, nWordLen, "out2");
            print_BN_buf_U32(e, eWordLen, "e");
            print_BN_buf_U32(d, nWordLen, "d");
            print_BN_buf_U32(n, nWordLen, "n");
            return 1;
        }
        else
        {
            //printf("\r\n RSA_ModExp_with_pub decrypt success");
        }

        ret = RSA_ModExp(out, e, n, out2, eBitLen, nBitLen);
        if(RSA_SUCCESS != ret)
        {
            printf("\r\n RSA_ModExp error, ret = %x", ret);
            return 1;
        }

        if(uint32_BigNumCmp(out2, nWordLen, in, nWordLen))
        {
            printf("\r\n RSA_ModExp encrypt error");
            print_BN_buf_U32(in, nWordLen, "in");
            print_BN_buf_U32(out, nWordLen, "out");
            print_BN_buf_U32(out2, nWordLen, "out2");
            print_BN_buf_U32(e, eWordLen, "e");
            print_BN_buf_U32(d, nWordLen, "d");
            print_BN_buf_U32(n, nWordLen, "n");
            return 1;
        }
        else
        {
            //printf("\r\n RSA_ModExp encrypt success");
        }
#endif
        printf("\r\n nBitLen = %u", nBitLen);
    }

    return 0;
}


u32 rsa_get_crt_key_traversal_test(void)
{
    u32 e[128]={0};
    //u32 d[128];
    u32 n[128];
    u32 p[64];
    u32 q[64];
    u32 dp[64];
    u32 dq[64];
    u32 u[64];
    u32 in[128] __attribute__((unused));
    u32 out[128] __attribute__((unused));
    u32 out2[128] __attribute__((unused));
    u32 eBitLen=17;
    u32 nBitLen;
    u32 eWordLen __attribute__((unused));
    u32 nWordLen __attribute__((unused));
    u32 pWordLen __attribute__((unused));
    u32 ret;

    for(nBitLen=1024; nBitLen<=4096; nBitLen+=2)
    {
        ret = RSA_GetCRTKey(e, p, q, dp, dq, u, n, eBitLen, nBitLen);
        if(RSA_SUCCESS != ret)
        {
            printf("\r\n RSA_GetCRTKey failure, ret = %x", ret);
            return 1;
        }
        //printf("\r\n RSA_GetCRTKey nBitLen = %u", nBitLen);

#ifdef RSA_SEC_TEST
        eWordLen=GET_WORD_LEN(eBitLen);
        nWordLen=GET_WORD_LEN(nBitLen);
        pWordLen=GET_WORD_LEN(nBitLen/2);
        uint32_set(in, 0x1, nWordLen);
        ret = RSA_ModExp(in, e, n, out, eBitLen, nBitLen);
        //}printf("\r\n end     ");
        if(RSA_SUCCESS != ret)
        {
            printf("\r\n  CRT nBitLen = %u, RSA_ModExp error, ret = %x", nBitLen, ret);
            return 1;
        }

        uint32_clear(out2, nWordLen);
        ret = RSA_CRTModExp_with_pub(out, p, q, dp, dq, u, e, out2, eBitLen, nBitLen);
        if(RSA_SUCCESS_S != ret)
        {
            printf("\r\n RSA_CRTModExp_with_pub error, ret = %x", ret);
            pWordLen=GET_WORD_LEN(nBitLen/2);
            print_BN_buf_U32(in, nWordLen, "in");
            print_BN_buf_U32(out, nWordLen, "out");
            print_BN_buf_U32(out2, nWordLen, "out2");
            print_BN_buf_U32(e, eWordLen, "e");
            print_BN_buf_U32(p, pWordLen, "p");
            print_BN_buf_U32(q, pWordLen, "q");
            print_BN_buf_U32(dp, pWordLen, "dp");
            print_BN_buf_U32(dq, pWordLen, "dq");
            print_BN_buf_U32(u, pWordLen, "u");
            print_BN_buf_U32(n, nWordLen, "n");
            return 1;
        }

        if(uint32_BigNumCmp(out2, nWordLen, in, nWordLen))
        {
            printf("\r\n RSA_CRTModExp_with_pub decrypt error");
            pWordLen=GET_WORD_LEN(nBitLen/2);
            print_BN_buf_U32(in, nWordLen, "in");
            print_BN_buf_U32(out, nWordLen, "out");
            print_BN_buf_U32(out2, nWordLen, "out2");
            print_BN_buf_U32(e, eWordLen, "e");
            print_BN_buf_U32(p, pWordLen, "p");
            print_BN_buf_U32(q, pWordLen, "q");
            print_BN_buf_U32(dp, pWordLen, "dp");
            print_BN_buf_U32(dq, pWordLen, "dq");
            print_BN_buf_U32(u, pWordLen, "u");
            print_BN_buf_U32(n, nWordLen, "n");
            return 1;
        }
        else
        {
            //printf("\r\n RSA_CRTModExp_with_pub decrypt success");
        }

        uint32_clear(out2, nWordLen);
        ret = RSA_CRTModExp_without_pub(out, p, q, dp, dq, u, out2, nBitLen);
        if(RSA_SUCCESS_S != ret)
        {
            printf("\r\n RSA_CRTModExp_without_pub error, ret = %x", ret);
            pWordLen=GET_WORD_LEN(nBitLen/2);
            print_BN_buf_U32(in, nWordLen, "in");
            print_BN_buf_U32(out, nWordLen, "out");
            print_BN_buf_U32(out2, nWordLen, "out2");
            print_BN_buf_U32(p, pWordLen, "p");
            print_BN_buf_U32(q, pWordLen, "q");
            print_BN_buf_U32(dp, pWordLen, "dp");
            print_BN_buf_U32(dq, pWordLen, "dq");
            print_BN_buf_U32(u, pWordLen, "u");
            print_BN_buf_U32(n, nWordLen, "n");
            return 1;
        }

        if(uint32_BigNumCmp(out2, nWordLen, in, nWordLen))
        {
            printf("\r\n RSA_CRTModExp_without_pub decrypt error");
            pWordLen=GET_WORD_LEN(nBitLen/2);
            print_BN_buf_U32(in, nWordLen, "in");
            print_BN_buf_U32(out, nWordLen, "out");
            print_BN_buf_U32(out2, nWordLen, "out2");
            print_BN_buf_U32(p, pWordLen, "p");
            print_BN_buf_U32(q, pWordLen, "q");
            print_BN_buf_U32(dp, pWordLen, "dp");
            print_BN_buf_U32(dq, pWordLen, "dq");
            print_BN_buf_U32(u, pWordLen, "u");
            print_BN_buf_U32(n, nWordLen, "n");
            return 1;
        }
        else
        {
            //printf("\r\n RSA_CRTModExp_without_pub decrypt success");
        }
#endif
        printf("\r\n CRT nBitLen = %u", nBitLen);
    }

    return 0;
}



u32 rsa_modexp_corner_test(u32 eBitLen, u32 nBitLen, u32 round)
{
    u32 e[128]={0};
    u32 d[128];
    u32 n[128];
    u32 in[128];
    u32 out[128];
    u32 out2[128];
    u32 eWordLen;
    u32 nWordLen;
    u32 ret;
    u32 i;

    //RSA NON-CRT
    eWordLen=GET_WORD_LEN(eBitLen);
    nWordLen=GET_WORD_LEN(nBitLen);
    for(i=0;i<round;i++)
    {
        ret = RSA_GetKey(e, d, n, eBitLen, nBitLen);
        if(RSA_SUCCESS != ret)
        {
            printf("\r\n RSA_GetKey failure, ret = %x", ret);
            return 1;
        }

#if 1
        //test encrypt & decrypt
        uint32_set(in, 0x1, nWordLen);
//printf("\r\n begin     "); 
//for(int j=0;j<10;j++){
#ifdef RSA_SEC_TEST
        ret = 0;
        ret |= RSA_ModExp_with_pub(in, e, d, n, out, eBitLen, nBitLen);
        ret |= RSA_ModExp_without_pub(in, d, n, out2, nBitLen);
//}printf("\r\n end     ");
        if((RSA_SUCCESS_S != ret) || uint32_BigNumCmp(out2, nWordLen, out, nWordLen))
#else
        ret = RSA_ModExp(in, d, n, out, get_valid_bits(d, nWordLen), nBitLen);
//}printf("\r\n end     ");
        if(RSA_SUCCESS != ret)
#endif
        {
            printf("\r\n RSA_ModExp error 1, ret = %x", ret);
            return 1;
        }

//printf("\r\n begin     "); 
//for(int j=0;j<8000;j++){
        ret = RSA_ModExp(out, e, n, out2, eBitLen, nBitLen);
//}printf("\r\n end     ");
        if(RSA_SUCCESS != ret)
        {
            printf("\r\n RSA_ModExp error 2, ret = %x", ret);
            return 1;
        }

        if(uint32_BigNumCmp(out2, nWordLen, in, nWordLen))
        {
            printf("\r\n RSA encrypt & decrypt error");
            print_BN_buf_U32(in, nWordLen, "in");
            print_BN_buf_U32(out, nWordLen, "out");
            print_BN_buf_U32(out2, nWordLen, "out2");
            print_BN_buf_U32(e, eWordLen, "e");
            print_BN_buf_U32(d, nWordLen, "d");
            print_BN_buf_U32(n, nWordLen, "n");
            return 1;
        }
        else
        {
            //printf("\r\n RSA encrypt & decrypt success");
        }

        //corner case: 0^0 mod n
        uint32_clear(in, nWordLen);
#ifdef RSA_SEC_TEST
        ret = 0;
        ret |= RSA_ModExp_with_pub(in, e, in, n, out, eBitLen, nBitLen);
        ret |= RSA_ModExp_without_pub(in, in, n, out, nBitLen);
        if(RSA_SUCCESS_S == ret)
#else
        ret = RSA_ModExp(in, in, n, out, 0, nBitLen);
        if(RSA_SUCCESS == ret)
#endif
        {
            printf("\r\n RSA_ModExp corner test 1 error, ret = %x", ret);
            return 1;
        }

        //corner case: base is n
#ifdef RSA_SEC_TEST
        ret = 0;
        ret |= RSA_ModExp_with_pub(n, e, d, n, out, eBitLen, nBitLen);
        ret |= RSA_ModExp_without_pub(n, d, n, out2, nBitLen);
        if((RSA_SUCCESS_S != ret) || (0 == uint32_BigNum_Check_Zero(out, nWordLen)) || (0 == uint32_BigNum_Check_Zero(out2, nWordLen)))
#else
        ret = RSA_ModExp(n, e, n, out, eBitLen, nBitLen);
        if((RSA_SUCCESS != ret) || (0 == uint32_BigNum_Check_Zero(out, nWordLen)))
#endif
        {
            printf("\r\n RSA_ModExp corner test 2 error, ret = %x", ret);
            return 1;
        }

#endif
        //if((i+1)%50 == 0)
        {
            printf("\r\n RSA%u %u", nBitLen, i+1);
        }
    }

    return 0;
}


u32 rsa_crt_modexp_corner_test(u32 eBitLen, u32 nBitLen, u32 round)
{
    u32 e[128]={0};
    //u32 d[128];
    u32 n[128];
    u32 p[64];
    u32 q[64];
    u32 dp[64];
    u32 dq[64];
    u32 u[64];
    u32 in[128];
    u32 out[128];
    u32 out2[128];
    u32 out3[128];
    u32 eWordLen;
    u32 nWordLen;
    u32 pWordLen;
    u32 ret;
    u32 i;


    //RSA CRT
    eWordLen=GET_WORD_LEN(eBitLen);
    nWordLen=GET_WORD_LEN(nBitLen);
    pWordLen=GET_WORD_LEN(nBitLen/2);
    for(i=0;i<round;i++)
    {
        ret = RSA_GetCRTKey(e, p, q, dp, dq, u, n, eBitLen, nBitLen);
        if(RSA_SUCCESS != ret)
        {
            printf("\r\n RSA_GetCRTKey failure, ret = %x", ret);
            return 1;
        }

#if 1
        //test encrypt & decrypt
        uint32_set(in, 0x1, nWordLen);
//printf("\r\n begin     "); 
//for(int j=0;j<25000;j++){
        ret = RSA_ModExp(in, e, n, out, eBitLen, nBitLen);
//}printf("\r\n end     ");
        if(RSA_SUCCESS != ret)
        {
            printf("\r\n RSA_ModExp error 1, ret = %x", ret);
            return 1;
        }

//printf("\r\n begin     "); 
//for(int j=0;j<100;j++){
#ifdef RSA_SEC_TEST
        ret = 0;
        ret |= RSA_CRTModExp_with_pub(out, p, q, dp, dq, u, e, out2, eBitLen, nBitLen);
        ret |= RSA_CRTModExp_without_pub(out, p, q, dp, dq, u, out3, nBitLen);
//}printf("\r\n end     ");
        if((RSA_SUCCESS_S != ret) || uint32_BigNumCmp(out2, nWordLen, out3, nWordLen))
#else
        ret = RSA_CRTModExp(out, p, q, dp, dq, u, out2, nBitLen);
//}printf("\r\n end     ");
        if(RSA_SUCCESS != ret)
#endif
        {
            printf("\r\n RSA_CRTModExp error 2, ret = %x", ret);
            return 1;
        }

        if(uint32_BigNumCmp(out2, nWordLen, in, nWordLen))
        {
            printf("\r\n RSA CRT encrypt & decrypt error");
            print_BN_buf_U32(in, nWordLen, "in");
            print_BN_buf_U32(out, nWordLen, "out");
            print_BN_buf_U32(out2, nWordLen, "out2");
            print_BN_buf_U32(out3, nWordLen, "out2");
            print_BN_buf_U32(e, eWordLen, "e");
            print_BN_buf_U32(p, pWordLen, "p");
            print_BN_buf_U32(q, pWordLen, "q");
            print_BN_buf_U32(dp, pWordLen, "dp");
            print_BN_buf_U32(dq, pWordLen, "dq");
            print_BN_buf_U32(u, pWordLen, "u");
            print_BN_buf_U32(n, nWordLen, "n");
            return 1;
        }
        else
        {
            //printf("\r\n RSA CRT encrypt & decrypt success");
        }

        //corner case: base is 0
        uint32_clear(in, nWordLen);
#ifdef RSA_SEC_TEST
        ret = 0;
        ret |= RSA_CRTModExp_with_pub(in, p, q, dp, dq, u, e, out, eBitLen, nBitLen);
        ret |= RSA_CRTModExp_without_pub(in, p, q, dp, dq, u, out2, nBitLen);
        if(RSA_SUCCESS_S != ret || (1 != uint32_BigNum_Check_Zero(out, nWordLen)) || (1 != uint32_BigNum_Check_Zero(out2, nWordLen)))
#else
        ret = RSA_CRTModExp(in, p, q, dp, dq, u, out, nBitLen);
        if(RSA_SUCCESS != ret || (1 != uint32_BigNum_Check_Zero(out, nWordLen)))
#endif
        {
            printf("\r\n RSA_CRTModExp corner test 1 error, ret = %x", ret);
            return 1;
        }

        //corner case: base is 1
        pke_set_operand_uint32_value(in, nWordLen, 1);
#ifdef RSA_SEC_TEST
        ret = 0;
        ret |= RSA_CRTModExp_with_pub(in, p, q, dp, dq, u, e, out, eBitLen, nBitLen);
        ret |= RSA_CRTModExp_without_pub(in, p, q, dp, dq, u, out2, nBitLen);
        if(RSA_SUCCESS_S != ret || (0 != uint32_BigNumCmp(out, nWordLen, in, nWordLen)) || (0 != uint32_BigNumCmp(out2, nWordLen, in, nWordLen)))
#else
        ret = RSA_CRTModExp(in, p, q, dp, dq, u, out, nBitLen);
        if(RSA_SUCCESS != ret || (0 != uint32_BigNumCmp(out, nWordLen, in, nWordLen)))
#endif
        {
            printf("\r\n RSA_CRTModExp corner test 2 error, ret = %x", ret);
            print_BN_buf_U32(out, nWordLen, "n");
            return 1;
        }

        //corner case: base is 2
        pke_set_operand_uint32_value(in, nWordLen, 2);
#ifdef RSA_SEC_TEST
        ret = 0;
        ret |= RSA_CRTModExp_with_pub(in, p, q, dp, dq, u, e, out, eBitLen, nBitLen);
        ret |= RSA_CRTModExp_without_pub(in, p, q, dp, dq, u, out3, nBitLen);
        (void)RSA_ModExp(out, e, n, out2, eBitLen, nBitLen);
        if(RSA_SUCCESS_S != ret || (0 != uint32_BigNumCmp(out2, nWordLen, in, nWordLen)) || (0 != uint32_BigNumCmp(out3, nWordLen, out, nWordLen)))
#else
        ret = RSA_CRTModExp(in, p, q, dp, dq, u, out, nBitLen);
        ret |= RSA_ModExp(out, e, n, out2, eBitLen, nBitLen);
        if(RSA_SUCCESS != ret || (0 != uint32_BigNumCmp(out2, nWordLen, in, nWordLen)))
#endif
        {
            printf("\r\n RSA_CRTModExp corner test 3 error, ret = %x", ret);
            return 1;
        }

        //corner case: base is n-2
        uint32_copy(in, n, nWordLen);
        in[0]-=2;
#ifdef RSA_SEC_TEST
        ret = 0;
        ret |= RSA_CRTModExp_with_pub(in, p, q, dp, dq, u, e, out, eBitLen, nBitLen);
        ret |= RSA_CRTModExp_without_pub(in, p, q, dp, dq, u, out3, nBitLen);
        (void)RSA_ModExp(out, e, n, out2, eBitLen, nBitLen);
        if(RSA_SUCCESS_S != ret || (0 != uint32_BigNumCmp(out2, nWordLen, in, nWordLen)) || (0 != uint32_BigNumCmp(out3, nWordLen, out, nWordLen)))
#else
        ret = RSA_CRTModExp(in, p, q, dp, dq, u, out, nBitLen);
        ret |= RSA_ModExp(out, e, n, out2, eBitLen, nBitLen);
        if(RSA_SUCCESS != ret || (0 != uint32_BigNumCmp(out2, nWordLen, in, nWordLen)))
#endif
        {
            printf("\r\n RSA_CRTModExp corner test 4 error, ret = %x", ret);
            return 1;
        }

        //corner case: base is n-1
        uint32_copy(in, n, nWordLen);
        in[0]-=1;
#ifdef RSA_SEC_TEST
        ret = 0;
        ret |= RSA_CRTModExp_with_pub(in, p, q, dp, dq, u, e, out, eBitLen, nBitLen);
        ret |= RSA_CRTModExp_without_pub(in, p, q, dp, dq, u, out2, nBitLen);
        if(RSA_SUCCESS_S != ret || (0 != uint32_BigNumCmp(out, nWordLen, in, nWordLen)) || (0 != uint32_BigNumCmp(out2, nWordLen, in, nWordLen)))
#else
        ret = RSA_CRTModExp(in, p, q, dp, dq, u, out, nBitLen);
        if(RSA_SUCCESS != ret || (0 != uint32_BigNumCmp(out, nWordLen, in, nWordLen)))
#endif
        {
            printf("\r\n RSA_CRTModExp corner test 5 error, ret = %x", ret);
            return 1;
        }

        //corner case: base is n
#ifdef RSA_SEC_TEST
        ret = 0;
        ret |= RSA_CRTModExp_with_pub(n, p, q, dp, dq, u, e, out, eBitLen, nBitLen);
        ret |= RSA_CRTModExp_without_pub(n, p, q, dp, dq, u, out2, nBitLen);
        if((RSA_SUCCESS_S != ret) || (1 != uint32_BigNum_Check_Zero(out, nWordLen)) || (1 != uint32_BigNum_Check_Zero(out2, nWordLen)))
#else
        ret = RSA_CRTModExp(n, p, q, dp, dq, u, out, nBitLen);
        if((RSA_SUCCESS != ret) || (1 != uint32_BigNum_Check_Zero(out, nWordLen)))
#endif
        {
            printf("\r\n RSA_CRTModExp corner test 6 error, ret = %x", ret);
            return 1;
        }
#endif
        //if((i+1)%50 == 0)
        {
            printf("\r\n RSA-CRT%u %u", nBitLen, i+1);
        }
    }

    return 0;
}

u32 RSA_all_test(void)
{
    u32 ret=0;

    printf("\r\n\r\n\r\n =================== RSA test =================== ");

#ifdef RSA_SEC_TEST
    pke_sec_init();
#endif

#if 0

#if 1
    rsa_1024_modexp_speed_test();
    rsa_2048_modexp_speed_test();
    rsa_3072_modexp_speed_test();
    rsa_4096_modexp_speed_test();
#endif

#if 0
    rsa_get_key_speed_test(0, 1000, 17, 1024);
    rsa_get_key_speed_test(1, 1000, 17, 1024);
    rsa_get_key_speed_test(0, 300, 17, 2048);
    rsa_get_key_speed_test(1, 300, 17, 2048);
    rsa_get_key_speed_test(0, 150, 17, 3072);
    rsa_get_key_speed_test(1, 150, 17, 3072);
    rsa_get_key_speed_test(0, 100, 17, 4096);
    rsa_get_key_speed_test(1, 100, 17, 4096);
#endif

#else

#if 1
//for(int i=0; i<100; i++)
    if(rsa_1024_modexp_sample_test())
        return 1;

    if(rsa_2048_modexp_sample_test())
        return 1;

    //if(rsa_3072_modexp_sample_test())
    //    return 1;

    //if(rsa_4096_modexp_sample_test())
    //    return 1;
#endif

#if 0
    if(rsa_get_key_traversal_test())
        return 1;
#endif

#if 0
    if(rsa_get_crt_key_traversal_test())
        return 1;
#endif

#if 1
    ret  = rsa_modexp_corner_test(17, 1024, TEST_RSA1024_ROUND);
    ret |= rsa_modexp_corner_test(17, 1026, TEST_RSA1026_ROUND);
    ret |= rsa_modexp_corner_test(17, 2046, TEST_RSA2046_ROUND);
    ret |= rsa_modexp_corner_test(17, 2048, TEST_RSA2048_ROUND);
    //ret |= rsa_modexp_corner_test(17, 3072, TEST_RSA3072_ROUND);
    //ret |= rsa_modexp_corner_test(17, 4096, TEST_RSA4096_ROUND);
    if(ret)
        return 1;
#endif

#if 1
    ret  = rsa_crt_modexp_corner_test(17, 1024, TEST_RSA1024_ROUND);
    ret |= rsa_crt_modexp_corner_test(17, 1026, TEST_RSA1026_ROUND);
    ret |= rsa_crt_modexp_corner_test(17, 2046, TEST_RSA2046_ROUND);
    ret |= rsa_crt_modexp_corner_test(17, 2048, TEST_RSA2048_ROUND);
    //ret |= rsa_crt_modexp_corner_test(17, 3072, TEST_RSA3072_ROUND);
    //ret |= rsa_crt_modexp_corner_test(17, 4096, TEST_RSA4096_ROUND);
    if(ret)
        return 1;
#endif

#endif

    printf("\r\n RSA test over");

    return 0;
}

#endif
