#include <stdio.h>

#include "../../../crypto_hal/pke.h"
#include "../../../crypto_include/crypto_common/utility.h"
#include "../../../crypto_include/trng/trng.h"



#define PKE_BASIC_160_TEST
#define PKE_BASIC_256_TEST
#define PKE_BASIC_521_TEST          //n is even
#define PKE_BASIC_522_TEST
#define PKE_BASIC_768_TEST
#define PKE_BASIC_1024_TEST
#define PKE_BASIC_1502_TEST
#define PKE_BASIC_2048_TEST
#define PKE_BASIC_2766_TEST
#define PKE_BASIC_3072_TEST
#define PKE_BASIC_4096_TEST



void big_sub(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t wordLen)
{
    uint32_t i, carry, temp, temp2;

    carry = 0;
    for(i=0; i<wordLen; i++)
    {
        temp = a[i]-b[i];
        temp2 = temp-carry;
        if(temp > a[i] || temp2 > temp)
        {
            carry = 1;
        }
        else
        {
            carry = 0;
        }
        c[i] = temp2;
    }
}

void big_add(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t wordLen)
{
    uint32_t i, carry, temp, temp2;

    carry = 0;
    for(i=0; i<wordLen; i++)
    {
        temp = a[i]+b[i];
        temp2 = temp+carry;
        if(temp < a[i] || temp2 < temp)
        {
            carry = 1;
        }
        else
        {
            carry = 0;
        }
        c[i] = temp2;
    }
}





typedef struct
{
    uint32_t bitLen;
    uint32_t *a;
    uint32_t *b;
    uint32_t *p;
    uint32_t *q;
    uint32_t *fai_n;   //fai_n = (p-1)*(q-1)
    uint32_t *d;       //d = e^(-1) mod fai_n, e is 0x10001 as default here.
    uint32_t *n;       //n = p*q
    uint32_t *std_n_h;
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    uint32_t *std_n_n0;
#endif
    uint32_t *std_10_01_n_h;
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    uint32_t *std_10_01_n_n0;
#endif
    uint32_t *std_11_11_n_h;
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    uint32_t *std_11_11_n_n0;
#endif
    uint32_t *std_a_add_b;
    uint32_t *std_a_sub_b;
    uint32_t *std_b_sub_a;
    uint32_t *std_modadd;
    uint32_t *std_modsub_a_b;
    uint32_t *std_modsub_b_a;
    uint32_t *std_modmul;
    uint32_t *std_modinv_odd;
    uint32_t *std_modinv_even;
    uint32_t *std_modexp;
    uint32_t *std_modexp_d;


} pke_basic_test_vector_t;





/****************************** add ******************************/
uint32_t pke_basic_add_test(uint32_t *a, uint32_t *b, uint32_t *std_a_add_b, uint32_t bitLen)
{
    uint32_t t1[OPERAND_MAX_WORD_LEN];
    uint32_t t2[OPERAND_MAX_WORD_LEN];
    uint32_t t3[OPERAND_MAX_WORD_LEN];
    uint32_t t4[OPERAND_MAX_WORD_LEN];
    uint32_t t5[OPERAND_MAX_WORD_LEN];

    uint32_t wordLen = GET_WORD_LEN(bitLen);
    uint32_t tmpBitLen = bitLen&31;
    uint32_t ret;


    // add: a+b & b+a, no overflow
    ret = pke_add(a, b, t1, wordLen);
    ret |= pke_add(b, a, t2, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t1, wordLen, std_a_add_b, wordLen) || uint32_BigNumCmp(t1, wordLen, t2, wordLen))
    {
        printf("\r\n %u pke_add(a+b & b+a, no overflow) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t1, wordLen, "t1");
        print_BN_buf_U32(t2, wordLen, "t2");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_add(a+b & b+a, no overflow) success", bitLen);
    }

    // add: a+b & b+a, overflow
    uint32_set(t1, 0xFFFFFFFF, wordLen);
    ret = pke_add(a, t1, t2, wordLen);
    ret |= pke_add(t1, a, t1, wordLen);
    uint32_copy(t3, a, wordLen);
    t3[0]-=1;
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t1, wordLen, t3, wordLen) || uint32_BigNumCmp(t2, wordLen, t3, wordLen))
    {
        printf("\r\n %u pke_add(a+b & b+a, overflow) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t1, wordLen, "t1");
        print_BN_buf_U32(t2, wordLen, "t2");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_add(a+b & b+a, overflow) success", bitLen);
    }

    // add: 0+0
    uint32_clear(t1, wordLen);
    uint32_clear(t2, wordLen);
    ret = pke_add(t1, t2, t1, wordLen);
    if(PKE_SUCCESS != ret || 0 == uint32_BigNum_Check_Zero(t1, wordLen))
    {
        printf("\r\n %u pke_add(0+0) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t1, wordLen, "t1");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_add(0+0) success", bitLen);
    }

    // add: 1+0 & 0+1
    t1[0]=1;
    ret = pke_add(t1, t2, t3, wordLen);
    ret |= pke_add(t2, t1, t2, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t1, wordLen, t2, wordLen) || uint32_BigNumCmp(t1, wordLen, t3, wordLen))
    {
        printf("\r\n %u pke_add(1+0 & 0+1) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t1, wordLen, "t1");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_add(1+0 & 0+1) success", bitLen);
    }

    // add: 1+1
    ret = pke_add(t1, t2, t3, wordLen);
    ret |= pke_add(t2, t1, t2, wordLen);
    t1[0] = 2;
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t1, wordLen, t2, wordLen) || uint32_BigNumCmp(t1, wordLen, t3, wordLen))
    {
        printf("\r\n %u pke_add(1+1) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t1, wordLen, "t1");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_add(1+1) success", bitLen);
    }

    //add: 11...11+0 & 0+11...11
    uint32_set(t1, 0xFFFFFFFF, wordLen);
    if(tmpBitLen)
    {
        t1[wordLen-1] &= (1<<tmpBitLen)-1;
    }
    uint32_clear(t2, wordLen);
    ret = pke_add(t1, t2, t3, wordLen);
    ret |= pke_add(t2, t1, t4, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t3, wordLen, t1, wordLen) || uint32_BigNumCmp(t4, wordLen, t1, wordLen))
    {
        printf("\r\n %u pke_add(11...11+0 & 0+11...11) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        print_BN_buf_U32(t4, wordLen, "t4");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_add(11...11+0 & 0+11...11) success ", bitLen);
    }

    //add: 1...11+1 & 1+11...11
    uint32_set(t1, 0xFFFFFFFF, wordLen);
    if(tmpBitLen)
    {
        t1[wordLen-1] &= (1<<tmpBitLen)-1;
    }
    pke_set_operand_uint32_value(t2, wordLen, 1);
    uint32_clear(t5, wordLen);
    if(tmpBitLen)
    {
        t5[wordLen-1] = (1<<tmpBitLen);
    }
    ret = pke_add(t1, t2, t3, wordLen);
    ret |= pke_add(t2, t1, t4, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t3, wordLen, t5, wordLen) || uint32_BigNumCmp(t4, wordLen, t5, wordLen))
    {
        printf("\r\n %u pke_add(11...11+1 & 1+11...11) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        print_BN_buf_U32(t4, wordLen, "t4");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_add(11...11+1 & 1+11...11) success ", bitLen);
    }

    //add: 11...11+11...11
    uint32_set(t1, 0xFFFFFFFF, wordLen);
    if(tmpBitLen)
    {
        t1[wordLen-1] &= (1<<tmpBitLen)-1;
    }
    uint32_set(t5, 0xFFFFFFFF, wordLen);
    t5[0]-=1;
    if(tmpBitLen)
    {
        t5[wordLen-1] &= ((1<<(tmpBitLen+1))-1);
    }
    ret = pke_add(t1, t1, t3, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t3, wordLen, t5, wordLen))
    {
        printf("\r\n %u pke_add(11...11+11...11) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        print_BN_buf_U32(t5, wordLen, "t5");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_add(11...11+11...11) success \r\n", bitLen);
    }

    return 0;
}



/****************************** sub ******************************/
uint32_t pke_basic_sub_test(uint32_t *a, uint32_t *b, uint32_t *std_a_sub_b, uint32_t *std_b_sub_a, uint32_t bitLen)
{
    uint32_t t1[OPERAND_MAX_WORD_LEN];
    uint32_t t2[OPERAND_MAX_WORD_LEN];
    uint32_t t3[OPERAND_MAX_WORD_LEN];
    uint32_t t4[OPERAND_MAX_WORD_LEN];
    uint32_t t5[OPERAND_MAX_WORD_LEN];

    uint32_t wordLen = GET_WORD_LEN(bitLen);
    uint32_t tmpBitLen = bitLen&31;
    uint32_t ret;

    // sub: a-b & b-a, no carry & carry
    ret = pke_sub(a, b, t1, wordLen);
    ret |= pke_sub(b, a, t2, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t1, wordLen, std_a_sub_b, wordLen) || uint32_BigNumCmp(t2, wordLen, std_b_sub_a, wordLen))
    {
        printf("\r\n %u pke_sub(a-b & b-a, no carry & carry) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t1, wordLen, "t1");
        print_BN_buf_U32(t2, wordLen, "t2");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_sub(a-b & b-a, no carry & carry) success", bitLen);
    }

    // sub: 0-0
    uint32_clear(t1, wordLen);
    uint32_clear(t2, wordLen);
    uint32_set(t3, 0x5a34ba6c, wordLen);
    ret = pke_sub(t1, t2, t3, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t1, wordLen, t3, wordLen))
    {
        printf("\r\n %u pke_sub(0-0) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_sub(0-0) success", bitLen);
    }

    // sub: 1-0 & 0-1
    t1[0]=1;
    ret = pke_sub(t1, t2, t3, wordLen);
    ret |= pke_sub(t2, t1, t2, wordLen);
    uint32_set(t4, 0xffffffff, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t1, wordLen, t3, wordLen) || uint32_BigNumCmp(t2, wordLen, t4, wordLen))
    {
        printf("\r\n %u pke_sub(1-0 & 0-1) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_sub(1-0 & 0-1) success", bitLen);
    }

    // sub: 1-1
    pke_set_operand_uint32_value(t1, wordLen, 1);
    pke_set_operand_uint32_value(t2, wordLen, 1);
    ret = pke_sub(t1, t2, t3, wordLen);
    if(PKE_SUCCESS != ret || 0 == uint32_BigNum_Check_Zero(t3, wordLen))
    {
        printf("\r\n %u pke_sub(1-1) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_sub(1-1) success ", bitLen);
    }

    //sub: 11...11-0 & 0-11...11
    uint32_set(t1, 0xFFFFFFFF, wordLen);
    if(tmpBitLen)
    {
        t1[wordLen-1] &= (1<<tmpBitLen)-1;
    }
    uint32_clear(t2, wordLen);
    ret = pke_sub(t1, t2, t3, wordLen);
    ret |= pke_sub(t2, t1, t2, wordLen);
    pke_set_operand_uint32_value(t4, wordLen, 1);
    if(tmpBitLen)
    {
        t4[wordLen-1] = ~((1<<tmpBitLen)-1);
    }
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t1, wordLen, t3, wordLen) || uint32_BigNumCmp(t2, wordLen, t4, wordLen))
    {
        printf("\r\n %u pke_sub(11...11-0 & 0-11...11) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_sub(11...11-0 & 0-11...11) success", bitLen);
    }

    //sub: 11...11-1 & 1-11...11
    uint32_set(t1, 0xFFFFFFFF, wordLen);
    if(tmpBitLen)
    {
        t1[wordLen-1] &= (1<<tmpBitLen)-1;
    }
    pke_set_operand_uint32_value(t2, wordLen, 1);
    ret = pke_sub(t1, t2, t3, wordLen);
    ret |= pke_sub(t2, t1, t2, wordLen);
    uint32_copy(t4, t1, wordLen);
    t4[0]-=1;
    pke_set_operand_uint32_value(t5, wordLen, 2);
    if(tmpBitLen)
    {
        t5[wordLen-1] = ~((1<<tmpBitLen)-1);
    }
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t4, wordLen, t3, wordLen) || uint32_BigNumCmp(t2, wordLen, t5, wordLen))
    {
        printf("\r\n %u pke_sub(11...11-1 & 1-11...11) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_sub(11...11-1 & 1-11...11) success", bitLen);
    }

    //sub: 11...11-11...11
    uint32_set(t1, 0xFFFFFFFF, wordLen);
    if(tmpBitLen)
    {
        t1[wordLen-1] &= (1<<tmpBitLen)-1;
    }
    uint32_clear(t3, wordLen);
    ret = pke_sub(t1, t1, t2, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t2, wordLen, t3, wordLen))
    {
        printf("\r\n %u pke_sub(11...11-11...11) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t2, wordLen, "t2");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_sub(11...11-11...11) success\r\n", bitLen);
    }

    return 0;
}


/****************************** mul ******************************/
uint32_t pke_basic_mul_test(uint32_t *a, uint32_t *b, uint32_t *n, uint32_t *fai_n, uint32_t bitLen)
{
    uint32_t t1[OPERAND_MAX_WORD_LEN];
    uint32_t t2[OPERAND_MAX_WORD_LEN];
    uint32_t t3[OPERAND_MAX_WORD_LEN];
    uint32_t t4[OPERAND_MAX_WORD_LEN];
    //uint32_t t5[OPERAND_MAX_WORD_LEN];

    uint32_t wordLen;
    uint32_t tmpWordLen;
    uint32_t tmpLen;
    uint32_t ret;

    //caution: the following treats bitLen as even
    if(bitLen&1)
    {
        bitLen+=1;
    }
    wordLen = GET_WORD_LEN(bitLen);
    tmpWordLen = GET_WORD_LEN((bitLen)/2);
    tmpLen = (bitLen/2)&31;
//*
    // mul: p*q = n
    uint32_clear(t1, OPERAND_MAX_WORD_LEN);
    uint32_clear(t2, OPERAND_MAX_WORD_LEN);
    uint32_copy(t1, a, tmpWordLen);
    uint32_copy(t2, b, tmpWordLen);
    ret = pke_mul(t1, t2, t3, tmpWordLen);
    ret |= pke_mul(t2, t1, t4, tmpWordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t3, wordLen, n, wordLen) || uint32_BigNumCmp(t4, wordLen, n, wordLen))
    {
        printf("\r\n %u pke_mul(p*q = q*p = n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t1, wordLen, "t1");
        print_BN_buf_U32(t2, wordLen, "t2");
        print_BN_buf_U32(t3, wordLen, "t3");
        print_BN_buf_U32(t4, wordLen, "t4");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_mul(p*q = q*p = n) success", bitLen);
    }

    if((a[0]&1) && (b[0]&1))
    {
        t1[0]-=1;
        t2[0]-=1;
        ret = pke_mul(t1, t2, t3, tmpWordLen);
        ret |= pke_mul(t2, t1, t4, tmpWordLen);
        if(PKE_SUCCESS != ret || uint32_BigNumCmp(t3, wordLen, fai_n, wordLen) || uint32_BigNumCmp(t4, wordLen, fai_n, wordLen))
        {
            printf("\r\n %u pke_mul((p-1)*(q-1) = (q-1)*(p-1) = fai_n) failure, ret=%x", bitLen, ret);
            print_BN_buf_U32(t1, wordLen, "t1");
            print_BN_buf_U32(t2, wordLen, "t2");
            print_BN_buf_U32(t3, wordLen, "t3");
            print_BN_buf_U32(t4, wordLen, "t4");
            return 1;
        }
        else
        {
            printf("\r\n %u pke_mul((p-1)*(q-1) = (q-1)*(p-1) = fai_n) success", bitLen);
        }
    }
//*/
    // mul: 0*0 = 0
    uint32_clear(t1, OPERAND_MAX_WORD_LEN);
    uint32_clear(t2, OPERAND_MAX_WORD_LEN);
    uint32_clear(t3, OPERAND_MAX_WORD_LEN);
    ret = pke_mul(t1, t2, t3, tmpWordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t1, wordLen, t3, wordLen))
    {
        printf("\r\n %u pke_mul(0*0 = 0) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_mul(0*0 = 0) success", bitLen);
    }

    // mul: 1*0 = 0*1 = 0
    pke_set_operand_uint32_value(t1, OPERAND_MAX_WORD_LEN, 1);
    uint32_clear(t2, OPERAND_MAX_WORD_LEN);
    uint32_clear(t3, OPERAND_MAX_WORD_LEN);
    uint32_clear(t4, OPERAND_MAX_WORD_LEN);
    ret = pke_mul(t1, t2, t3, tmpWordLen);
    ret |= pke_mul(t2, t1, t4, tmpWordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t2, wordLen, t3, wordLen) || uint32_BigNumCmp(t2, wordLen, t4, wordLen))
    {
        printf("\r\n %u pke_mul(1*0 = 0*1 = 0) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        print_BN_buf_U32(t4, wordLen, "t4");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_mul(1*0 = 0*1 = 0) success", bitLen);
    }
//*
    // mul: 1*1 = 1
    pke_set_operand_uint32_value(t1, OPERAND_MAX_WORD_LEN, 1);
    pke_set_operand_uint32_value(t2, OPERAND_MAX_WORD_LEN, 1);
    uint32_clear(t3, OPERAND_MAX_WORD_LEN);
    ret = pke_mul(t1, t2, t3, tmpWordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t2, wordLen, t3, wordLen))
    {
        printf("\r\n %u pke_mul(1*1 = 1) failure, ret=%x \r\n", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_mul(1*1 = 1) success ", bitLen);
    }
//*/
    // mul: 11...11*0 & 0*11...11
    uint32_clear(t1, OPERAND_MAX_WORD_LEN);
    uint32_clear(t2, OPERAND_MAX_WORD_LEN);
    uint32_clear(t3, OPERAND_MAX_WORD_LEN);
    uint32_clear(t4, OPERAND_MAX_WORD_LEN);
    uint32_set(t1, 0xFFFFFFFF, tmpWordLen);
    if(tmpLen)
    {
        t1[tmpWordLen-1] &= (1<<tmpLen)-1;
    }
    uint32_clear(t2, tmpWordLen);
    ret = pke_mul(t1, t2, t3, tmpWordLen);
    ret |= pke_mul(t2, t1, t2, tmpWordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t3, wordLen, t4, wordLen) || uint32_BigNumCmp(t2, wordLen, t4, wordLen))
    {
        printf("\r\n %u pke_mul(11...11*0 & 0*11...11) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_mul(11...11*0 & 0*11...11) success", bitLen);
    }
//*
    // mul: 11...11*1 & 1*0x11...11
    uint32_clear(t1, OPERAND_MAX_WORD_LEN);
    uint32_clear(t2, OPERAND_MAX_WORD_LEN);
    uint32_clear(t3, OPERAND_MAX_WORD_LEN);
    uint32_clear(t4, OPERAND_MAX_WORD_LEN);
    uint32_set(t1, 0xFFFFFFFF, tmpWordLen);
    if(tmpLen)
    {
        t1[tmpWordLen-1] &= (1<<tmpLen)-1;
    }
    pke_set_operand_uint32_value(t2, tmpWordLen, 1);
    ret = pke_mul(t1, t2, t3, tmpWordLen);
    ret |= pke_mul(t2, t1, t4, tmpWordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t3, wordLen, t1, wordLen) || uint32_BigNumCmp(t4, wordLen, t1, wordLen))
    {
        printf("\r\n %u pke_mul(11...11*1 & 1*11...11) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_mul(11...11*1 & 1*11...11) success", bitLen);
    }
//*/
    // mul: 11...11*11...11
    uint32_clear(t1, OPERAND_MAX_WORD_LEN);
    uint32_clear(t2, OPERAND_MAX_WORD_LEN);
    uint32_clear(t3, OPERAND_MAX_WORD_LEN);
    uint32_clear(t4, OPERAND_MAX_WORD_LEN);
    uint32_set(t1, 0xFFFFFFFF, tmpWordLen);
    if(tmpLen)
    {
        t1[tmpWordLen-1] &= (1<<tmpLen)-1;
    }
    uint32_copy(t3+tmpWordLen, t1, tmpWordLen);
    t3[tmpWordLen] &= ~1;
    Big_Div2n(t3, tmpWordLen*2, tmpLen>0?(32-tmpLen):0);
    t3[0]|=1;
    ret = pke_mul(t1, t1, t2, tmpWordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t2, wordLen, t3, wordLen))
    {
        printf("\r\n %u pke_mul(11...11*11...11) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t1, tmpWordLen, "t1");
        print_BN_buf_U32(t2, wordLen, "t2");
        print_BN_buf_U32(t3, wordLen, "t3");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_mul(11...11*11...11) success\r\n", bitLen);
    }

    return 0;
}


/****************************** modadd ******************************/
uint32_t pke_basic_modadd_test(uint32_t *a, uint32_t *b, uint32_t *n, uint32_t *std_modadd, uint32_t bitLen)
{
    uint32_t t1[OPERAND_MAX_WORD_LEN];
    uint32_t t2[OPERAND_MAX_WORD_LEN];
    uint32_t t3[OPERAND_MAX_WORD_LEN];
    uint32_t t4[OPERAND_MAX_WORD_LEN];

    uint32_t wordLen = GET_WORD_LEN(bitLen);
    uint32_t ret;


    if((NULL != a) && (NULL != b) && (NULL != std_modadd))
    {
        // modadd: a+b mod n & b+a mod n, no overflow
        ret = pke_modadd(n, a, b, t1, wordLen);
        ret |= pke_modadd(n, b, a, t2, wordLen);
        if(PKE_SUCCESS != ret || uint32_BigNumCmp(t1, wordLen, std_modadd, wordLen) || uint32_BigNumCmp(t1, wordLen, t2, wordLen))
        {
            printf("\r\n %u pke_modadd(a+b mod n & b+a mod n, no overflow) failure, ret=%x", bitLen, ret);
            print_BN_buf_U32(t1, wordLen, "t1");
            print_BN_buf_U32(t2, wordLen, "t2");
            return 1;
        }
        else
        {
            printf("\r\n %u pke_modadd(a+b mod n & b+a mod n, no overflow) success", bitLen);
        }

        // modadd: a+b mod n & b+a mod n, overflow
        uint32_copy(t1, n, wordLen);
        t1[0]-=1;
        uint32_copy(t4, a, wordLen);
        t4[0]-=1;
        ret = pke_modadd(n, a, t1, t2, wordLen);
        ret |= pke_modadd(n, t1, a, t3, wordLen);
        if(PKE_SUCCESS != ret || uint32_BigNumCmp(t2, wordLen, t4, wordLen) || uint32_BigNumCmp(t3, wordLen, t4, wordLen))
        {
            printf("\r\n %u pke_modadd(a+b mod n & b+a mod n, overflow) failure, ret=%x", bitLen, ret);
            print_BN_buf_U32(t2, wordLen, "t2");
            print_BN_buf_U32(t3, wordLen, "t3");
            return 1;
        }
        else
        {
            printf("\r\n %u pke_modadd(a+b mod n & b+a mod n, overflow) success", bitLen);
        }
    }

    // modadd: 0+0 mod n
    uint32_clear(t1, wordLen);
    uint32_clear(t2, wordLen);
    ret = pke_modadd(n, t1, t2, t3, wordLen);
    if(PKE_SUCCESS != ret || 0 == uint32_BigNum_Check_Zero(t3, wordLen))
    {
        printf("\r\n %u pke_modadd(0-0 mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modadd(0-0 mod n) success", bitLen);
    }

    // modadd: 1+0 mod n & 0+1 mod n
    t1[0]=1;
    ret = pke_modadd(n, t1, t2, t3, wordLen);
    ret |= pke_modadd(n, t2, t1, t4, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t3, wordLen, t1, wordLen) || uint32_BigNumCmp(t4, wordLen, t1, wordLen))
    {
        printf("\r\n %u pke_modadd(1+0 mod n & 0+1 mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        print_BN_buf_U32(t4, wordLen, "t4");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modadd(1+0 mod n & 0+1 mod n) success", bitLen);
    }

    // modadd: 1+1 mod n
    t1[0]=t2[0]=1;
    pke_set_operand_uint32_value(t4, wordLen, 2);
    ret = pke_modadd(n, t1, t2, t3, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t3, wordLen, t4, wordLen))
    {
        printf("\r\n %u pke_modadd(1+1 mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modadd(1+1 mod n) success ", bitLen);
    }

    // modadd: (n-1)+0 mod n & 0+(n-1) mod n
    uint32_copy(t1, n, wordLen);
    pke_set_operand_uint32_value(t2, wordLen, 1);
    big_sub(t1,t2,t1, wordLen);
    t2[0]=0;
    ret = pke_modadd(n, t1, t2, t3, wordLen);
    ret |= pke_modadd(n, t2, t1, t2, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t3, wordLen, t1, wordLen) || uint32_BigNumCmp(t2, wordLen, t1, wordLen))
    {
        printf("\r\n %u pke_modadd((n-1)+0 mod n & 0+(n-1) mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modadd((n-1)+0 mod n & 0+(n-1) mod n) success ", bitLen);
    }

    // modadd: (n-1)+1 mod n & 1+(n-1) mod n
    uint32_copy(t1, n, wordLen);
    pke_set_operand_uint32_value(t2, wordLen, 1);
    big_sub(t1,t2,t1, wordLen);
    uint32_clear(t4, wordLen);
    ret = pke_modadd(n, t1, t2, t3, wordLen);
    ret |= pke_modadd(n, t2, t1, t2, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t3, wordLen, t4, wordLen) || uint32_BigNumCmp(t2, wordLen, t4, wordLen))
    {
        printf("\r\n %u pke_modadd((n-1)+1 mod n & 1+(n-1) mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modadd((n-1)+1 mod n & 1+(n-1) mod n) success ", bitLen);
    }

    // modadd: (n-1)+(n-1) mod n
    uint32_copy(t1, n, wordLen);
    pke_set_operand_uint32_value(t2, wordLen, 1);
    big_sub(t1,t2,t1, wordLen);
    uint32_copy(t3, n, wordLen);
    t2[0]=2;
    big_sub(t3,t2,t3, wordLen);
    ret = pke_modadd(n, t1, t1, t2, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t3, wordLen, t2, wordLen))
    {
        printf("\r\n %u pke_modadd((n-1)+(n-1) mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modadd((n-1)+(n-1) mod n) success \r\n", bitLen);
    }

    return 0;
}


/****************************** modsub ******************************/
uint32_t pke_basic_modsub_test(uint32_t *a, uint32_t *b, uint32_t *n, uint32_t *std_modsub_a_b, uint32_t *std_modsub_b_a, uint32_t bitLen)
{
    uint32_t t1[OPERAND_MAX_WORD_LEN];
    uint32_t t2[OPERAND_MAX_WORD_LEN];
    uint32_t t3[OPERAND_MAX_WORD_LEN];
    uint32_t t4[OPERAND_MAX_WORD_LEN];
    uint32_t t5[OPERAND_MAX_WORD_LEN];

    uint32_t wordLen = GET_WORD_LEN(bitLen);
    uint32_t ret;

    if((NULL != a) && (NULL != b) && (NULL != std_modsub_a_b) && (NULL != std_modsub_b_a))
    {
        // modsub: a-b mod n & b-a mod n, no carry & carry
        ret = pke_modsub(n, a, b, t1, wordLen);
        ret |= pke_modsub(n, b, a, t2, wordLen);
        if(PKE_SUCCESS != ret || uint32_BigNumCmp(t1, wordLen, std_modsub_a_b, wordLen) || uint32_BigNumCmp(t2, wordLen, std_modsub_b_a, wordLen))
        {
            printf("\r\n %u pke_modsub(a-b mod n & b-a mod n, no carry & carry) failure, ret=%x", bitLen, ret);
            print_BN_buf_U32(t1, wordLen, "t1");
            print_BN_buf_U32(t2, wordLen, "t2");
            return 1;
        }
        else
        {
            printf("\r\n %u pke_modsub(a-b mod n & b-a mod n, no carry & carry) success", bitLen);
        }
    }

    // modsub: 0-0 mod n
    uint32_clear(t1, wordLen);
    uint32_clear(t2, wordLen);
    ret = pke_modsub(n, t1, t2, t3, wordLen);
    if(PKE_SUCCESS != ret || 0 == uint32_BigNum_Check_Zero(t3, wordLen))
    {
        printf("\r\n %u pke_modsub(0-0 mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modsub(0-0 mod n) success", bitLen);
    }

    // modsub: 1-0 mod n & 0-1 mod n
    t1[0]=1;
    ret = pke_modsub(n, t1, t2, t3, wordLen);
    ret |= pke_modsub(n, t2, t1, t2, wordLen);
    big_sub(n, t1, t4, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t1, wordLen, t3, wordLen) || uint32_BigNumCmp(t2, wordLen, t4, wordLen))
    {
        printf("\r\n %u pke_modsub(1-0 mod n & 0-1 mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        print_BN_buf_U32(t2, wordLen, "t2");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modsub(1-0 mod n & 0-1 mod n) success", bitLen);
    }

    // modsub: 1-1 mod n
    uint32_clear(t1, wordLen);
    uint32_clear(t2, wordLen);
    t1[0]=t2[0]=1;
    ret = pke_modsub(n, t1, t2, t3, wordLen);
    if(PKE_SUCCESS != ret || 0 == uint32_BigNum_Check_Zero(t3, wordLen))
    {
        printf("\r\n %u pke_modsub(1-1 mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modsub(1-1 mod n) success ", bitLen);
    }

    // modsub: (n-1)-0 mod n & 0-(n-1) mod n
    uint32_clear(t2, wordLen);
    uint32_clear(t3, wordLen);
    t3[0]=1;
    big_sub(n, t3, t1, wordLen);
    ret = pke_modsub(n, t1, t2, t4, wordLen);
    ret |= pke_modsub(n, t2, t1, t2, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t1, wordLen, t4, wordLen) || uint32_BigNumCmp(t2, wordLen, t3, wordLen))
    {
        printf("\r\n %u pke_modsub((n-1)-0 mod n & 0-(n-1) mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        print_BN_buf_U32(t2, wordLen, "t2");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modsub((n-1)-0 mod n & 0-(n-1) mod n) success", bitLen);
    }

    // modsub: (n-1)-1 mod n & 1-(n-1) mod n
    pke_set_operand_uint32_value(t2, wordLen, 2);
    pke_set_operand_uint32_value(t3, wordLen, 1);
    big_sub(n, t3, t1, wordLen);
    big_sub(t1, t3, t5, wordLen);
    ret = pke_modsub(n, t1, t3, t4, wordLen);
    ret |= pke_modsub(n, t3, t1, t3, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t5, wordLen, t4, wordLen) || uint32_BigNumCmp(t2, wordLen, t3, wordLen))
    {
        printf("\r\n %u pke_modsub((n-1)-1 mod n & 1-(n-1) mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        print_BN_buf_U32(t4, wordLen, "t4");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modsub((n-1)-1 mod n & 1-(n-1) mod n) success", bitLen);
    }

    // modsub: (n-1)-(n-1) mod n
    uint32_clear(t2, wordLen);
    pke_set_operand_uint32_value(t3, wordLen, 1);
    big_sub(n, t3, t1, wordLen);
    ret = pke_modsub(n, t1, t1, t3, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t2, wordLen, t3, wordLen))
    {
        printf("\r\n %u pke_modsub((n-1)-(n-1) mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modsub((n-1)-(n-1) mod n) success\r\n", bitLen);
    }

    return 0;
}


/****************************** modmul *******************************/
uint32_t pke_basic_modmul_test(uint32_t *a, uint32_t *b, uint32_t *n, uint32_t *p, uint32_t *q, uint32_t *std_modmul, uint32_t bitLen)
{
    uint32_t t1[OPERAND_MAX_WORD_LEN];
    uint32_t t2[OPERAND_MAX_WORD_LEN];
    uint32_t t3[OPERAND_MAX_WORD_LEN];
    uint32_t t4[OPERAND_MAX_WORD_LEN];
    //uint32_t t5[OPERAND_MAX_WORD_LEN];

    uint32_t wordLen = GET_WORD_LEN(bitLen);
    uint32_t tmpWordLen;
    uint32_t ret;

    if(NULL != a)
    {
        // modmul: a*b mod n & b*a mod n
        ret = pke_modmul(n, a, b, t1, wordLen);
        ret |= pke_modmul(n, b, a, t2, wordLen);
        if(PKE_SUCCESS != ret || uint32_BigNumCmp(t1, wordLen, std_modmul, wordLen) || uint32_BigNumCmp(t2, wordLen, std_modmul, wordLen))
        {
            printf("\r\n %u pke_modmul(a*b mod n & b*a mod n) failure, ret=%x", bitLen, ret);
            print_BN_buf_U32(t1, wordLen, "t1");
            print_BN_buf_U32(t2, wordLen, "t2");
            return 1;
        }
        else
        {
            printf("\r\n %u pke_modmul(a*b mod n & b*a mod n) success", bitLen);
        }

        // modmul: p*q mod (p*q) & q*p mod (p*q)
        uint32_clear(t1, wordLen);
        uint32_clear(t2, wordLen);
        tmpWordLen = GET_WORD_LEN((bitLen)/2);
        uint32_copy(t1, p, tmpWordLen);
        uint32_copy(t2, q, tmpWordLen);
        ret = pke_modmul(n, t1, t2, t3, wordLen);
        ret |= pke_modmul(n, t2, t1, t4, wordLen);
        if(PKE_SUCCESS != ret || 0 == uint32_BigNum_Check_Zero(t3, wordLen) || 0 == uint32_BigNum_Check_Zero(t4, wordLen))
        {
            printf("\r\n %u pke_modmul(p*q mod (p*q) & q*p mod (p*q)) failure, ret=%x", bitLen, ret);
            print_BN_buf_U32(t3, wordLen, "t3");
            print_BN_buf_U32(t4, wordLen, "t4");
            return 1;
        }
        else
        {
            printf("\r\n %u pke_modmul(p*q mod (p*q) & q*p mod (p*q)) success", bitLen);
        }
    }

    // modmul: 0*0 mod n
    uint32_clear(t1, wordLen);
    uint32_clear(t2, wordLen);
    ret = pke_modmul(n, t1, t2, t3, wordLen);
    if(PKE_SUCCESS != ret || 0 == uint32_BigNum_Check_Zero(t3, wordLen))
    {
        printf("\r\n %u pke_modmul(0*0 mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modmul(0*0 mod n) success", bitLen);
    }

    // modmul: 1*0 mod n & 0*1 mod n
    t1[0]=1;
    ret = pke_modmul(n, t1, t2, t3, wordLen);
    ret |= pke_modmul(n, t2, t1, t4, wordLen);
    if(PKE_SUCCESS != ret || 0 == uint32_BigNum_Check_Zero(t3, wordLen) || 0 == uint32_BigNum_Check_Zero(t4, wordLen))
    {
        printf("\r\n %u pke_modmul(1*0 mod n & 0*1 mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        print_BN_buf_U32(t4, wordLen, "t4");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modmul(1*0 mod n & 0*1 mod n) success", bitLen);
    }

    // modmul: 1*1 mod n
    t2[0]=1;
    ret = pke_modmul(n, t1, t2, t3, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t3, wordLen, t1, wordLen))
    {
        printf("\r\n %u pke_modmul(1*1 mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modmul(1*1 mod n) success", bitLen);
    }

    // modmul: (n-1)*0 mod n & 0*(n-1) mod n
    pke_set_operand_uint32_value(t1, wordLen, 1);
    uint32_clear(t2, wordLen);
    big_sub(n, t1, t1, wordLen);
    ret = pke_modmul(n, t1, t2, t3, wordLen);
    ret |= pke_modmul(n, t2, t1, t4, wordLen);
    if(PKE_SUCCESS != ret || 0 == uint32_BigNum_Check_Zero(t3, wordLen) || 0 == uint32_BigNum_Check_Zero(t4, wordLen))
    {
        printf("\r\n %u pke_modmul((n-1)*0 mod n & 0*(n-1) mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        print_BN_buf_U32(t4, wordLen, "t4");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modmul((n-1)*0 mod n & 0*(n-1) mod n) success", bitLen);
    }

    // modmul: (n-1)*1 mod n & 1*(n-1) mod n
    uint32_clear(t1, wordLen);
    pke_set_operand_uint32_value(t2, wordLen, 1);
    big_sub(n, t2, t1, wordLen);
    ret = pke_modmul(n, t1, t2, t3, wordLen);
    ret |= pke_modmul(n, t2, t1, t4, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t1, wordLen, t3, wordLen) || uint32_BigNumCmp(t1, wordLen, t4, wordLen))
    {
        printf("\r\n %u pke_modmul((n-1)*1 mod n & 1*(n-1) mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        print_BN_buf_U32(t4, wordLen, "t4");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modmul((n-1)*1 mod n & 1*(n-1) mod n) success", bitLen);
    }

//if(bitLen % 256 != 0)    //PKE_HP, PKE_LP都已经解决
{
    // modmul: (n-1)*(n-1) mod n   //256,1024,4096bit,且n全为1时，计算错误
    uint32_copy(t1, n, wordLen);
    t1[0]-=1;
    pke_set_operand_uint32_value(t2, wordLen, 1);
    ret = pke_modmul(n, t1, t1, t3, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t3, wordLen, t2, wordLen))
    {
        printf("\r\n %u pke_modmul((n-1)*(n-1) mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modmul((n-1)*(n-1) mod n) success ", bitLen);
    }
}

    printf("\r\n");

    return 0;
}


/****************************** pre_mont *******************************/
//caution: the unit of R is 256 bits for PKE_HP, and is 32bits for PKE_LP
uint32_t pke_basic_pre_mont_test(uint32_t *n, uint32_t *std_n_h, uint32_t *std_n0, uint32_t bitLen)
{
    uint32_t h[OPERAND_MAX_WORD_LEN];
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    uint32_t n0[1];
#endif
    uint32_t wordLen = GET_WORD_LEN(bitLen);
    uint32_t ret;

#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    ret = pke_pre_calc_mont(n, bitLen, h, n0);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(h, wordLen, std_n_h, wordLen) || ((NULL != std_n0) &&(*std_n0 != *n0)))
#else
    ret = pke_pre_calc_mont(n, bitLen, h);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(h, wordLen, std_n_h, wordLen))
#endif
    {
        printf("\r\n %u pke_pre_calc_mont failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(n, wordLen, "n");
        print_BN_buf_U32(std_n_h, wordLen, "std_n_h");
        print_BN_buf_U32(h, wordLen, "h");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_pre_calc_mont success \r\n", bitLen);
    }

    return 0;
}


/****************************** modinv *******************************/
uint32_t pke_basic_modinv_test(uint32_t *a, uint32_t *n, uint32_t *std_modinv_odd, uint32_t *std_modinv_even, uint32_t bitLen)
{
    uint32_t t1[OPERAND_MAX_WORD_LEN];
    uint32_t t2[OPERAND_MAX_WORD_LEN];
    uint32_t t3[OPERAND_MAX_WORD_LEN];
    uint32_t t4[OPERAND_MAX_WORD_LEN];
    uint32_t t5[OPERAND_MAX_WORD_LEN];

    uint32_t *odd_modulus, *even_modulus;
    uint32_t wordLen = GET_WORD_LEN(bitLen);

    uint32_t ret;

    uint32_copy(t1, n, wordLen);
    t1[0] -= 1;

    if(n[0] & 1)
    {
        odd_modulus = n;
        even_modulus = t1;
    }
    else
    {
        odd_modulus = t1;
        even_modulus = n;
    }

    if(NULL != a)
    {
        // modinv: odd modulus
        if(NULL != std_modinv_odd)
        {
            ret = pke_modinv(odd_modulus, a, t3, wordLen, wordLen);
            if(PKE_SUCCESS != ret || uint32_BigNumCmp(t3, wordLen, std_modinv_odd, wordLen))
            {
                printf("\r\n %u pke_modinv(odd modulus) failure, ret=%x", bitLen, ret);
                print_BN_buf_U32(t3, wordLen, "t3");
                print_BN_buf_U32(std_modinv_odd, wordLen, "std_modinv_odd");
                return 1;
            }
            else
            {
                printf("\r\n %u pke_modinv(odd modulus) odd success", bitLen);
            }
        }

        // modinv: even modulus
        if(NULL != std_modinv_even)
        {
            ret = pke_modinv(even_modulus, a, t3, wordLen, wordLen);
            if(PKE_SUCCESS != ret || uint32_BigNumCmp(t3, wordLen, std_modinv_even, wordLen))
            {
                printf("\r\n %u pke_modinv(even modulus) failure, ret=%x", bitLen, ret);
                print_BN_buf_U32(t3, wordLen, "t3");
                return 1;
            }
            else
            {
                printf("\r\n %u pke_modinv(even modulus) even success", bitLen);
            }
        }

        // modinv: inverse does not exist
        uint32_copy(t2, a, wordLen);
        t2[0] &= (~1);
        ret = pke_modinv(even_modulus, t2, t3, wordLen, wordLen);
        if(PKE_NO_MODINV != ret)
        {
            printf("\r\n %u pke_modinv(inverse does not exist) failure, ret=%x", bitLen, ret);
            print_BN_buf_U32(t3, wordLen, "t3");
            return 1;
        }
        else
        {
            printf("\r\n %u pke_modinv(inverse does not exist) success", bitLen);
        }
    }

    // modinv: a is 0, modulus is odd and even
    uint32_clear(t2, wordLen);
    ret = pke_modinv(odd_modulus, t2, t3, wordLen, wordLen);
    ret |= pke_modinv(even_modulus, t2, t4, wordLen, wordLen);
    if(PKE_NO_MODINV != ret)
    {
        printf("\r\n %u pke_modinv(a is 0, modulus is odd and even) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        print_BN_buf_U32(t4, wordLen, "t4");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modinv(a is 0, modulus is odd and even) success", bitLen);
    }

    // modinv: a is 1, modulus is odd and even
    uint32_clear(t3, wordLen);
    uint32_clear(t4, wordLen);
    t2[0] = 1;
    ret = pke_modinv(odd_modulus, t2, t3, wordLen, wordLen);
    ret |= pke_modinv(even_modulus, t2, t4, wordLen, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t3, wordLen, t2, wordLen) || uint32_BigNumCmp(t4, wordLen, t2, wordLen))
    {
        printf("\r\n %u pke_modinv(a is 1, modulus is odd and even) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        print_BN_buf_U32(t4, wordLen, "t4");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modinv(a is 1, modulus is odd and even) success", bitLen);
    }

    // modinv: a is n-1, modulus is odd and even
    uint32_copy(t2, odd_modulus, wordLen);
    uint32_copy(t4, even_modulus, wordLen);
    t2[0] -= 1;
    pke_set_operand_uint32_value(t5, wordLen, 1);
    big_sub(t4, t5, t4, wordLen);
    ret = pke_modinv(odd_modulus, t2, t3, wordLen, wordLen);
    ret |= pke_modinv(even_modulus, t4, t1, wordLen, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t2, wordLen, t3, wordLen) || uint32_BigNumCmp(t4, wordLen, t1, wordLen))
    {
        printf("\r\n %u pke_modinv(a is n-1, modulus is odd and even) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        print_BN_buf_U32(t1, wordLen, "t1");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modinv(a is n-1, modulus is odd and even) success \r\n", bitLen);
    }

    return 0;
}


/****************************** modexp *******************************/
uint32_t pke_basic_modexp_test(uint32_t *a, uint32_t *b, uint32_t *n, uint32_t *p, uint32_t *q, uint32_t *std_modexp, uint32_t bitLen)
{
    uint32_t t1[OPERAND_MAX_WORD_LEN];
    uint32_t t2[OPERAND_MAX_WORD_LEN];
    uint32_t t3[OPERAND_MAX_WORD_LEN];
    uint32_t t4[OPERAND_MAX_WORD_LEN];
    uint32_t t5[OPERAND_MAX_WORD_LEN];

    uint32_t wordLen = GET_WORD_LEN(bitLen);
    uint32_t tmpWordLen;
    uint32_t i;
    uint32_t ret;


    pke_pre_calc_mont_no_output(n, wordLen);

    if(NULL != a)
    {
        // modexp: a^b mod n
        ret = pke_modexp(n, b, a, t1, wordLen, wordLen);
        if(PKE_SUCCESS != ret || uint32_BigNumCmp(t1, wordLen, std_modexp, wordLen))
        {
            printf("\r\n %u pke_modexp(a^b mod n) failure, ret=%x", bitLen, ret);
            print_BN_buf_U32(t1, wordLen, "t1");
            return 1;
        }
        else
        {
            printf("\r\n %u pke_modexp(a^b mod n) success", bitLen);
        }

        // modexp: a^fai(n)=1 mod n
        printf("\r\n %u pke_modexp(a^fai(n)=1 mod n) test \r\n", bitLen);
        uint32_clear(t1, wordLen);
        uint32_clear(t2, wordLen);
        pke_set_operand_uint32_value(t4, wordLen, 1);
        tmpWordLen = GET_WORD_LEN((bitLen)/2);
        uint32_copy(t1, p, tmpWordLen);
        uint32_copy(t2, q, tmpWordLen);
        t1[0]-=1;
        t2[0]-=1;
        //ret = pke_modmul(n, t1, t2, t3, wordLen);
        ret = pke_mul(t1, t2, t3, tmpWordLen);
        for(i=0;i<100;i++)
        {
            printf(" %u", i+1);
            ret |= get_rand((uint8_t *)t1, wordLen<<2);
            t1[wordLen-1] = 0;
            ret |= pke_modexp(n, t3, t1, t5, wordLen, wordLen);
            if(PKE_SUCCESS != ret || uint32_BigNumCmp(t5, wordLen, t4, wordLen))
            {
                printf("\r\n %u pke_modexp(a^fai(n)=1 mod n) failure, ret=%x", bitLen, ret);
                print_BN_buf_U32(t5, wordLen, "t5");
                return 1;
            }
        }

        // modexp: a^(fai(n)+1)=a mod n
        printf("\r\n %u pke_modexp(a^(fai(n)+1)=a mod n) test \r\n", bitLen);
        big_add(t3,t4,t3,wordLen);
        for(i=0;i<100;i++)
        {
            printf(" %u", i+1);
            ret |= get_rand((uint8_t *)t1, wordLen<<2);
            t1[wordLen-1] = 0;
            ret |= pke_modexp(n, t3, t1, t5, wordLen, wordLen);
            if(PKE_SUCCESS != ret || uint32_BigNumCmp(t5, wordLen, t1, wordLen))
            {
                printf("\r\n %u pke_modexp(a^(fai(n)+1)=a mod n) failure, ret=%x", bitLen, ret);
                print_BN_buf_U32(t5, wordLen, "t5");
                return 1;
            }
        }

        // modexp: a^(fai(n)-1) = a^(-1) mod n
        printf("\r\n %u pke_modexp(a^(fai(n)-1) = a^(-1) mod n) test \r\n", bitLen);
        t4[0]=2;
        big_sub(t3,t4,t3,wordLen);
        t4[0]=1;
        for(i=0;i<100;i++)
        {
            printf(" %u", i+1);
            ret |= get_rand((uint8_t *)t1, wordLen<<2);
            t1[wordLen-1] = 0;

            //t2 = t1^(-1) mod n
            ret |= pke_modinv(n, t1, t2, wordLen, wordLen);
            if(PKE_SUCCESS != ret )
            {
                printf("\r\n %u pke_modinv failure, ret=%x", bitLen, ret);
                print_BN_buf_U32(t1, wordLen, "t1");
                print_BN_buf_U32(t2, wordLen, "t2");
                return 1;
            }

            //t5 = t1^(fai(n)-1) mod n
            ret |= pke_modexp(n, t3, t1, t5, wordLen, wordLen);
            if(PKE_SUCCESS != ret || uint32_BigNumCmp(t5, wordLen, t2, wordLen))
            {
                printf("\r\n %u pke_modexp(a^(fai(n)-1) = a^(-1) mod n) failure, ret=%x", bitLen, ret);
                print_BN_buf_U32(t5, wordLen, "t5");
                return 1;
            }

            //t2 = t1*t1^(-1) = 1 mod n
            ret = pke_modmul(n, t1, t5, t2, wordLen);
            if(PKE_SUCCESS != ret || uint32_BigNumCmp(t2, wordLen, t4, wordLen))
            {
                printf("\r\n %u pke_modmul failure, ret=%x", bitLen, ret);
                print_BN_buf_U32(t1, wordLen, "t1");
                print_BN_buf_U32(t5, wordLen, "t5");
                print_BN_buf_U32(t2, wordLen, "t2");
                return 1;
            }
        }
    }

    //a^(b+c) mod n = (a^b * a^c) mod n
    printf("\r\n %u pke_modexp(a^(b+c) = (a^b * a^c) mod n) test \r\n", bitLen);
    for(i=0;i<100;i++)
    {
        printf(" %u", i+1);
        ret = get_rand((uint8_t *)t1, wordLen<<2);
        t1[wordLen-1] = 0;

        ret |= get_rand((uint8_t *)t2, wordLen<<2);
        t2[wordLen-1] = 0;

        ret |= get_rand((uint8_t *)t3, wordLen<<2);
        t3[wordLen-1] = 0;

        ret |= pke_modexp(n, t2, t1, t4, wordLen, wordLen);

        ret |= pke_modexp(n, t3, t1, t5, wordLen, wordLen);

        ret |= pke_modmul(n, t4, t5, t5, wordLen);

        ret |= pke_add(t2, t3, t2, wordLen);

        ret |= pke_modexp(n, t2, t1, t4, wordLen, wordLen);

        if(PKE_SUCCESS != ret || uint32_BigNumCmp(t5, wordLen, t4, wordLen))
        {
            printf("\r\n %u pke_modexp(a^(b+c) = (a^b * a^c) mod n) failure, ret=%x", bitLen, ret);
            print_BN_buf_U32(t5, wordLen, "t5");
            return 1;
        }
    }

#if 1
    // modexp: 0^0 mod n     //error, 计算成功，但结果是0,耗时非常久，估计是0-1后生成非常大的数导致
    uint32_clear(t1, wordLen);
    uint32_clear(t2, wordLen);
    ret = pke_modexp(n, t1, t2, t3, wordLen, wordLen);
    print_BN_buf_U32(t3, wordLen, "\r\npke_modexp t3(0^0 mod n)");
    if(PKE_SUCCESS != ret)
    {
        printf("\r\n %u pke_modexp(0^0 mod n) failure, ret=%x", bitLen, ret);
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modexp(0^0 mod n) success", bitLen);
    }
#endif

    // modexp: 0^1 mod n
    pke_set_operand_uint32_value(t1, wordLen, 1);
    uint32_clear(t2, wordLen);
    uint32_clear(t3, wordLen);
    ret = pke_modexp(n, t1, t2, t3, wordLen, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t3, wordLen, t2, wordLen))
    {
        printf("\r\n %u pke_modexp(0^1 mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modexp(0^1 mod n) success", bitLen);
    }

    // modexp: 1^0 mod n    //虽然结果是1，但耗时非常久，估计是0-1后生成非常大的数导致
    uint32_clear(t1, wordLen);
    pke_set_operand_uint32_value(t2, wordLen, 1);
    uint32_clear(t3, wordLen);
    ret = pke_modexp(n, t1, t2, t3, wordLen, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t3, wordLen, t2, wordLen))
    {
        printf("\r\n %u pke_modexp(1^0 mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modexp(1^0 mod n) success", bitLen);
    }

    // modexp: 1^1 mod n
    pke_set_operand_uint32_value(t1, wordLen, 1);
    pke_set_operand_uint32_value(t2, wordLen, 1);
    uint32_clear(t3, wordLen);
    ret = pke_modexp(n, t1, t2, t3, wordLen, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t3, wordLen, t1, wordLen))
    {
        printf("\r\n %u pke_modexp(1^1 mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modexp(1^1 mod n) success", bitLen);
    }

    // modexp: (n-1)^0 mod n    //虽然结果是1，但耗时非常久，估计是0-1后生成非常大的数导致
    uint32_clear(t2, wordLen);
    pke_set_operand_uint32_value(t3, wordLen, 1);
    big_sub(n, t3, t1, wordLen);
    ret = pke_modexp(n, t2, t1, t4, wordLen, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t4, wordLen, t3, wordLen))
    {
        printf("\r\n %u pke_modexp((n-1)^0 mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t4, wordLen, "t4");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modexp((n-1)^0 mod n) success", bitLen);
    }

    // modexp: 0^(n-1) mod n
    uint32_clear(t2, wordLen);
    pke_set_operand_uint32_value(t3, wordLen, 1);
    big_sub(n, t3, t1, wordLen);
    ret = pke_modexp(n, t1, t2, t4, wordLen, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t4, wordLen, t2, wordLen))
    {
        printf("\r\n %u pke_modexp(0^(n-1) mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t4, wordLen, "t4");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modexp(0^(n-1) mod n) success", bitLen);
    }

    // modexp: (n-1)^1 mod n
    uint32_clear(t2, wordLen);
    pke_set_operand_uint32_value(t3, wordLen, 1);
    big_sub(n, t3, t1, wordLen);
    ret = pke_modexp(n, t3, t1, t4, wordLen, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t4, wordLen, t1, wordLen))
    {
        printf("\r\n %u pke_modexp((n-1)^1 mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t4, wordLen, "t4");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modexp((n-1)^1 mod n) success", bitLen);
    }

    // modexp: 1^(n-1) mod n
    uint32_clear(t2, wordLen);
    pke_set_operand_uint32_value(t3, wordLen, 1);
    big_sub(n, t3, t1, wordLen);
    ret = pke_modexp(n, t1, t3, t4, wordLen, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t4, wordLen, t3, wordLen))
    {
        printf("\r\n %u pke_modexp(1^(n-1) mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t4, wordLen, "t4");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modexp(1^(n-1) mod n) success", bitLen);
    }


    // modexp: (n-1)^(n-1) mod n
    uint32_clear(t2, wordLen);
    pke_set_operand_uint32_value(t3, wordLen, 1);
    big_sub(n, t3, t1, wordLen);
    ret = pke_modexp(n, t1, t1, t4, wordLen, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t4, wordLen, t3, wordLen))
    {
        printf("\r\n %u pke_modexp((n-1)^(n-1) mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t4, wordLen, "t4");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modexp((n-1)^(n-1) mod n) success", bitLen);
    }

    printf("\r\n");

    return 0;
}


#ifdef AIC_PKE_SEC
/****************************** modexp_ladder *******************************/
uint32_t pke_basic_modexp_ladder_test(uint32_t *a, uint32_t *b, uint32_t *n, uint32_t *p, uint32_t *q, uint32_t *std_modexp, uint32_t bitLen)
{
    uint32_t t1[OPERAND_MAX_WORD_LEN];
    uint32_t t2[OPERAND_MAX_WORD_LEN];
    uint32_t t3[OPERAND_MAX_WORD_LEN];
    uint32_t t4[OPERAND_MAX_WORD_LEN];
    uint32_t t5[OPERAND_MAX_WORD_LEN];

    uint32_t wordLen = GET_WORD_LEN(bitLen);
    uint32_t tmpWordLen;
    uint32_t i;
    uint32_t ret;


    pke_pre_calc_mont_no_output(n, wordLen);

    if(NULL != a)
    {
        // modexp: a^b mod n
        ret = pke_modexp_ladder(n, b, a, t1, wordLen, wordLen);
        if(PKE_SUCCESS != ret || uint32_BigNumCmp(t1, wordLen, std_modexp, wordLen))
        {
            printf("\r\n %u pke_modexp_ladder(a^b mod n) failure, ret=%x", bitLen, ret);
            print_BN_buf_U32(t1, wordLen, "t1");
            return 1;
        }
        else
        {
            printf("\r\n %u pke_modexp_ladder(a^b mod n) success", bitLen);
        }

        // modexp: a^fai(n)=1 mod n
        printf("\r\n %u pke_modexp_ladder(a^fai(n)=1 mod n) test \r\n", bitLen);
        uint32_clear(t1, wordLen);
        uint32_clear(t2, wordLen);
        pke_set_operand_uint32_value(t4, wordLen, 1);
        tmpWordLen = GET_WORD_LEN((bitLen)/2);
        uint32_copy(t1, p, tmpWordLen);
        uint32_copy(t2, q, tmpWordLen);
        t1[0]-=1;
        t2[0]-=1;
        //ret = pke_modmul(n, t1, t2, t3, wordLen);
        ret = pke_mul(t1, t2, t3, tmpWordLen);
        for(i=0;i<100;i++)
        {
            printf(" %u", i+1);
            ret |= get_rand((uint8_t *)t1, wordLen<<2);
            t1[wordLen-1] = 0;
            ret |= pke_modexp_ladder(n, t3, t1, t5, wordLen, wordLen);
            if(PKE_SUCCESS != ret || uint32_BigNumCmp(t5, wordLen, t4, wordLen))
            {
                printf("\r\n %u pke_modexp_ladder(a^fai(n)=1 mod n) failure, ret=%x", bitLen, ret);
                print_BN_buf_U32(t5, wordLen, "t5");
                return 1;
            }
        }

        // modexp: a^(fai(n)+1)=a mod n
        printf("\r\n %u pke_modexp_ladder(a^(fai(n)+1)=a mod n) test \r\n", bitLen);
        big_add(t3,t4,t3,wordLen);
        for(i=0;i<100;i++)
        {
            printf(" %u", i+1);
            ret |= get_rand((uint8_t *)t1, wordLen<<2);
            t1[wordLen-1] = 0;
            ret |= pke_modexp_ladder(n, t3, t1, t5, wordLen, wordLen);
            if(PKE_SUCCESS != ret || uint32_BigNumCmp(t5, wordLen, t1, wordLen))
            {
                printf("\r\n %u pke_modexp_ladder(a^(fai(n)+1)=a mod n) failure, ret=%x", bitLen, ret);
                print_BN_buf_U32(t5, wordLen, "t5");
                return 1;
            }
        }

        // modexp: a^(fai(n)-1) = a^(-1) mod n
        printf("\r\n %u pke_modexp_ladder(a^(fai(n)-1) = a^(-1) mod n) test \r\n", bitLen);
        t4[0]=2;
        big_sub(t3,t4,t3,wordLen);
        t4[0]=1;
        for(i=0;i<100;i++)
        {
            printf(" %u", i+1);
            ret |= get_rand((uint8_t *)t1, wordLen<<2);
            t1[wordLen-1] = 0;

            //t2 = t1^(-1) mod n
            ret |= pke_modinv(n, t1, t2, wordLen, wordLen);
            if(PKE_SUCCESS != ret )
            {
                printf("\r\n %u pke_modinv failure, ret=%x", bitLen, ret);
                print_BN_buf_U32(t1, wordLen, "t1");
                print_BN_buf_U32(t2, wordLen, "t2");
                return 1;
            }

            //t5 = t1^(fai(n)-1) mod n
            ret |= pke_modexp_ladder(n, t3, t1, t5, wordLen, wordLen);
            if(PKE_SUCCESS != ret || uint32_BigNumCmp(t5, wordLen, t2, wordLen))
            {
                printf("\r\n %u pke_modexp_ladder(a^(fai(n)-1) = a^(-1) mod n) failure, ret=%x", bitLen, ret);
                print_BN_buf_U32(t5, wordLen, "t5");
                return 1;
            }

            //t2 = t1*t1^(-1) = 1 mod n
            ret |= pke_modmul(n, t1, t5, t2, wordLen);
            if(PKE_SUCCESS != ret || uint32_BigNumCmp(t2, wordLen, t4, wordLen))
            {
                printf("\r\n %u pke_modmul failure, ret=%x", bitLen, ret);
                print_BN_buf_U32(t1, wordLen, "t1");
                print_BN_buf_U32(t5, wordLen, "t5");
                print_BN_buf_U32(t2, wordLen, "t2");
                return 1;
            }
        }
    }

    //a^(b+c) mod n = (a^b * a^c) mod n
    printf("\r\n %u pke_modexp_ladder(a^(b+c) = (a^b * a^c) mod n) test \r\n", bitLen);
    for(i=0;i<100;i++)
    {
        printf(" %u", i+1);
        ret = get_rand((uint8_t *)t1, wordLen<<2);
        t1[wordLen-1] = 0;

        ret |= get_rand((uint8_t *)t2, wordLen<<2);
        t2[wordLen-1] = 0;

        ret |= get_rand((uint8_t *)t3, wordLen<<2);
        t3[wordLen-1] = 0;

        ret |= pke_modexp_ladder(n, t2, t1, t4, wordLen, wordLen);

        ret |= pke_modexp_ladder(n, t3, t1, t5, wordLen, wordLen);

        ret |= pke_modmul(n, t4, t5, t5, wordLen);

        ret |= pke_add(t2, t3, t2, wordLen);

        ret |= pke_modexp_ladder(n, t2, t1, t4, wordLen, wordLen);

        if(PKE_SUCCESS != ret || uint32_BigNumCmp(t5, wordLen, t4, wordLen))
        {
            printf("\r\n %u pke_modexp_ladder(a^(b+c) = (a^b * a^c) mod n) failure, ret=%x", bitLen, ret);
            print_BN_buf_U32(t5, wordLen, "t5");
            return 1;
        }
    }

#if 1
    // modexp: 0^0 mod n     //error, 计算成功，但结果是0,耗时非常久，估计是0-1后生成非常大的数导致
    uint32_clear(t1, wordLen);
    uint32_clear(t2, wordLen);
    ret = pke_modexp_ladder(n, t1, t2, t3, wordLen, wordLen);
    print_BN_buf_U32(t3, wordLen, "\r\n pke_modexp_ladder t3(0^0 mod n)");
    if(PKE_SUCCESS != ret)
    {
        printf("\r\n %u pke_modexp_ladder(0^0 mod n) failure, ret=%x", bitLen, ret);
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modexp_ladder(0^0 mod n) success", bitLen);
    }
#endif

    // modexp: 0^1 mod n
    pke_set_operand_uint32_value(t1, wordLen, 1);
    uint32_clear(t2, wordLen);
    uint32_clear(t3, wordLen);
    ret = pke_modexp_ladder(n, t1, t2, t3, wordLen, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t3, wordLen, t2, wordLen))
    {
        printf("\r\n %u pke_modexp_ladder(0^1 mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modexp_ladder(0^1 mod n) success", bitLen);
    }

    // modexp: 1^0 mod n    //虽然结果是1，但耗时非常久，估计是0-1后生成非常大的数导致
    uint32_clear(t1, wordLen);
    pke_set_operand_uint32_value(t2, wordLen, 1);
    uint32_clear(t3, wordLen);
    ret = pke_modexp_ladder(n, t1, t2, t3, wordLen, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t3, wordLen, t2, wordLen))
    {
        printf("\r\n %u pke_modexp_ladder(1^0 mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modexp_ladder(1^0 mod n) success", bitLen);
    }

    // modexp: 1^1 mod n
    pke_set_operand_uint32_value(t1, wordLen, 1);
    pke_set_operand_uint32_value(t2, wordLen, 1);
    uint32_clear(t3, wordLen);
    ret = pke_modexp_ladder(n, t1, t2, t3, wordLen, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t3, wordLen, t1, wordLen))
    {
        printf("\r\n %u pke_modexp_ladder(1^1 mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modexp_ladder(1^1 mod n) success", bitLen);
    }

    // modexp: (n-1)^0 mod n    //虽然结果是1，但耗时非常久，估计是0-1后生成非常大的数导致
    uint32_clear(t2, wordLen);
    pke_set_operand_uint32_value(t3, wordLen, 1);
    big_sub(n, t3, t1, wordLen);
    ret = pke_modexp_ladder(n, t2, t1, t4, wordLen, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t4, wordLen, t3, wordLen))
    {
        printf("\r\n %u pke_modexp_ladder((n-1)^0 mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t4, wordLen, "t4");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modexp_ladder((n-1)^0 mod n) success", bitLen);
    }

    // modexp: 0^(n-1) mod n
    uint32_clear(t2, wordLen);
    pke_set_operand_uint32_value(t3, wordLen, 1);
    big_sub(n, t3, t1, wordLen);
    ret = pke_modexp_ladder(n, t1, t2, t4, wordLen, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t4, wordLen, t2, wordLen))
    {
        printf("\r\n %u pke_modexp_ladder(0^(n-1) mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t4, wordLen, "t4");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modexp_ladder(0^(n-1) mod n) success", bitLen);
    }

    // modexp: (n-1)^1 mod n
    uint32_clear(t2, wordLen);
    pke_set_operand_uint32_value(t3, wordLen, 1);
    big_sub(n, t3, t1, wordLen);
    ret = pke_modexp_ladder(n, t3, t1, t4, wordLen, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t4, wordLen, t1, wordLen))
    {
        printf("\r\n %u pke_modexp_ladder((n-1)^1 mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t4, wordLen, "t4");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modexp_ladder((n-1)^1 mod n) success", bitLen);
    }

    // modexp: 1^(n-1) mod n
    uint32_clear(t2, wordLen);
    pke_set_operand_uint32_value(t3, wordLen, 1);
    big_sub(n, t3, t1, wordLen);
    ret = pke_modexp_ladder(n, t1, t3, t4, wordLen, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t4, wordLen, t3, wordLen))
    {
        printf("\r\n %u pke_modexp_ladder(1^(n-1) mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t4, wordLen, "t4");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modexp_ladder(1^(n-1) mod n) success", bitLen);
    }


    // modexp: (n-1)^(n-1) mod n
    uint32_clear(t2, wordLen);
    pke_set_operand_uint32_value(t3, wordLen, 1);
    big_sub(n, t3, t1, wordLen);
    ret = pke_modexp_ladder(n, t1, t1, t4, wordLen, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t4, wordLen, t3, wordLen))
    {
        printf("\r\n %u pke_modexp_ladder((n-1)^(n-1) mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t4, wordLen, "t4");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modexp_ladder((n-1)^(n-1) mod n) success", bitLen);
    }

    printf("\r\n");

    return 0;
}


/****************************** modexp_with_pub *******************************/
uint32_t pke_basic_modexp_with_pub_test(uint32_t *a, uint32_t *d, uint32_t *n, uint32_t *fai_n, uint32_t *std_modexp_d, uint32_t bitLen)
{
    uint32_t t1[OPERAND_MAX_WORD_LEN];
    uint32_t t2[OPERAND_MAX_WORD_LEN];
    uint32_t t3[OPERAND_MAX_WORD_LEN];
    uint32_t t4[OPERAND_MAX_WORD_LEN];
    uint32_t t5[OPERAND_MAX_WORD_LEN];

    uint32_t wordLen = GET_WORD_LEN(bitLen);
    uint32_t i;
    uint32_t ret;

    pke_pre_calc_mont_no_output(n, wordLen);

    //standard data test
    t1[0] = 0x10001;
    ret = pke_modexp_with_pub(n, d, t1, a, t2, wordLen, wordLen, 1);
    if((PKE_SUCCESS != ret) || uint32_BigNumCmp(t2, wordLen, std_modexp_d, wordLen))
    {
        printf("\r\n %u pke_modexp_with_pub(fixed data) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(n, wordLen, "n");
        print_BN_buf_U32(d, wordLen, "d");
        print_BN_buf_U32(a, wordLen, "a");
        print_BN_buf_U32(t1, wordLen, "t1");
        print_BN_buf_U32(t2, wordLen, "t2");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modexp_with_pub(fixed data) success", bitLen);
    }

    //a=0
    uint32_clear(t2, wordLen);
    ret = pke_modexp_with_pub(n, d, t1, t2, t3, wordLen, wordLen, 1);
    if((PKE_SUCCESS != ret) || uint32_BigNumCmp(t2, wordLen, t3, wordLen))
    {
        printf("\r\n %u pke_modexp_with_pub(a=0) failure, ret=%x", bitLen, ret);
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modexp_with_pub(a=0) success", bitLen);
    }

    //a=1
    pke_set_operand_uint32_value(t2, wordLen, 1);
    ret = pke_modexp_with_pub(n, d, t1, t2, t3, wordLen, wordLen, 1);
    if((PKE_SUCCESS != ret) || uint32_BigNumCmp(t2, wordLen, t3, wordLen))
    {
        printf("\r\n %u pke_modexp_with_pub(a=1) failure, ret=%x", bitLen, ret);
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modexp_with_pub(a=1) success", bitLen);
    }

    //a=n-1
    uint32_copy(t2, n, wordLen);
    t2[0]-=1;
    ret = pke_modexp_with_pub(n, d, t1, t2, t3, wordLen, wordLen, 1);
    if((PKE_SUCCESS != ret) || uint32_BigNumCmp(t2, wordLen, t3, wordLen))
    {
        printf("\r\n %u pke_modexp_with_pub(a=n-1) failure, ret=%x", bitLen, ret);
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modexp_with_pub(a=n-1) success", bitLen);
    }

    //e=1,d=1
    pke_set_operand_uint32_value(t1, wordLen, 1);
    ret = pke_modexp_with_pub(n, t1, t1, a, t3, wordLen, wordLen, 1);
    if((PKE_SUCCESS != ret) || uint32_BigNumCmp(a, wordLen, t3, wordLen))
    {
        printf("\r\n %u pke_modexp_with_pub(e=1,d=1) failure, ret=%x", bitLen, ret);
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modexp_with_pub(e=1,d=1) success", bitLen);
    }

    //e=1,d=fai_n+1
    pke_set_operand_uint32_value(t1, wordLen, 1);
    uint32_copy(t2, fai_n, wordLen);
    t2[0]+=1;
    ret = pke_modexp_with_pub(n, t2, t1, a, t3, wordLen, wordLen, 1);
    if((PKE_SUCCESS != ret) || uint32_BigNumCmp(a, wordLen, t3, wordLen))
    {
        printf("\r\n %u pke_modexp_with_pub(e=1,d=fai_n+1) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(n, wordLen, "n");
        print_BN_buf_U32(t2, wordLen, "d");
        print_BN_buf_U32(t1, 1, "e");
        print_BN_buf_U32(a, wordLen, "a");
        print_BN_buf_U32(t3, wordLen, "t3");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modexp_with_pub(e=1,d=fai_n+1) success", bitLen);
    }

    //random e&d test.
    printf("\r\n pke_modexp_with_pub(random e,d,a) test \r\n");
GET_RAND_E:
    (void)get_rand((uint8_t *)t1, 8);
    t1[0] |= 1;
    ret = pke_modinv(fai_n, t1, t2, wordLen, 2);
    if(PKE_SUCCESS != ret)
    {
        goto GET_RAND_E;
    }
    for(i=0; i<100; i++)
    {
        (void)get_rand((uint8_t *)t3, wordLen << 2);
        t3[wordLen - 1] = 0;
        ret = pke_modexp_with_pub(n, t2, t1, t3, t4, wordLen, wordLen, 2);
        if(PKE_SUCCESS != ret)
        {
            printf("\r\n %u pke_modexp_with_pub failure, ret=%x", bitLen, ret);
            return 1;
        }
        ret |= pke_modexp(n, t2, t3, t5, wordLen, wordLen);
        if(PKE_SUCCESS != ret)
        {
            printf("\r\n %u pke_modexp failure, ret=%x", bitLen, ret);
            return 1;
        }
        if(uint32_BigNumCmp(t5, wordLen, t4, wordLen))
        {
            printf("\r\n %u pke_modexp_with_pub failure 2, ret=%x", bitLen, ret);
            print_BN_buf_U32(t4, wordLen, "t4");
            print_BN_buf_U32(t5, wordLen, "t5");
            return 1;
        }
        else
        {
            printf(" %u", i+1);
        }
    }
    printf("\r\n");

    return 0;
}



uint32_t pke_basic_modexp_without_pub_test(uint32_t *a, uint32_t *b, uint32_t *n, uint32_t *p, uint32_t *q, uint32_t *std_modexp, uint32_t bitLen)
{
    uint32_t t1[OPERAND_MAX_WORD_LEN];
    uint32_t t2[OPERAND_MAX_WORD_LEN];
    uint32_t t3[OPERAND_MAX_WORD_LEN];
    uint32_t t4[OPERAND_MAX_WORD_LEN];
    uint32_t t5[OPERAND_MAX_WORD_LEN];

    uint32_t wordLen = GET_WORD_LEN(bitLen);
    uint32_t tmpWordLen = GET_WORD_LEN((bitLen)/2);
    uint32_t tmpBitLen = (bitLen)&31;
    uint32_t tmpLen = ((bitLen)/2)&31;
    uint32_t i;
    uint32_t ret;


    pke_pre_calc_mont_no_output(n, wordLen);

    if(NULL != a)
    {
        // modexp: a^b mod n
        ret = pke_modexp_without_pub(n, b, a, t1, wordLen, wordLen);
        if(PKE_SUCCESS != ret || uint32_BigNumCmp(t1, wordLen, std_modexp, wordLen))
        {
            printf("\r\n %u pke_modexp_without_pub(a^b mod n) failure, ret=%x", bitLen, ret);
            print_BN_buf_U32(t1, wordLen, "t1");
            return 1;
        }
        else
        {
            printf("\r\n %u pke_modexp_without_pub(a^b mod n) success", bitLen);
        }

        // modexp: a^fai(n)=1 mod n
        printf("\r\n %u pke_modexp_without_pub(a^fai(n)=1 mod n) test \r\n", bitLen);
        uint32_clear(t1, wordLen);
        uint32_clear(t2, wordLen);
        pke_set_operand_uint32_value(t4, wordLen, 1);
        tmpWordLen = GET_WORD_LEN((bitLen)/2);
        uint32_copy(t1, p, tmpWordLen);
        uint32_copy(t2, q, tmpWordLen);
        t1[0]-=1;
        t2[0]-=1;
        //ret = pke_modmul(n, t1, t2, t3, wordLen);
        ret = pke_mul(t1, t2, t3, tmpWordLen);
        for(i=0;i<100;i++)
        {
            printf(" %u", i+1);
            ret |= get_rand((uint8_t *)t1, wordLen<<2);
            t1[wordLen-1] = 0;
            ret |= pke_modexp_without_pub(n, t3, t1, t5, wordLen, wordLen);
            if(PKE_SUCCESS != ret || uint32_BigNumCmp(t5, wordLen, t4, wordLen))
            {
                printf("\r\n %u pke_modexp_without_pub(a^fai(n)=1 mod n) failure, ret=%x", bitLen, ret);
                print_BN_buf_U32(t5, wordLen, "t5");
                return 1;
            }
        }

        // modexp: a^(fai(n)+1)=a mod n
        printf("\r\n %u pke_modexp_without_pub(a^(fai(n)+1)=a mod n) test \r\n", bitLen);
        big_add(t3,t4,t3,wordLen);
        for(i=0;i<100;i++)
        {
            printf(" %u", i+1);
            ret |= get_rand((uint8_t *)t1, wordLen<<2);
            t1[wordLen-1] = 0;
            ret |= pke_modexp_without_pub(n, t3, t1, t5, wordLen, wordLen);
            if(PKE_SUCCESS != ret || uint32_BigNumCmp(t5, wordLen, t1, wordLen))
            {
                printf("\r\n %u pke_modexp_without_pub(a^(fai(n)+1)=a mod n) failure, ret=%x", bitLen, ret);
                print_BN_buf_U32(t5, wordLen, "t5");
                return 1;
            }
        }

        // modexp: a^(fai(n)-1) = a^(-1) mod n
        printf("\r\n %u pke_modexp_without_pub(a^(fai(n)-1) = a^(-1) mod n) test \r\n", bitLen);
        t4[0]=2;
        big_sub(t3,t4,t3,wordLen);
        t4[0]=1;
        for(i=0;i<100;i++)
        {
            printf(" %u", i+1);
            ret |= get_rand((uint8_t *)t1, wordLen<<2);
            t1[wordLen-1] = 0;

            //t2 = t1^(-1) mod n
            ret = pke_modinv(n, t1, t2, wordLen, wordLen);
            if(PKE_SUCCESS != ret )
            {
                printf("\r\n %u pke_modinv failure, ret=%x", bitLen, ret);
                print_BN_buf_U32(t1, wordLen, "t1");
                print_BN_buf_U32(t2, wordLen, "t2");
                return 1;
            }

            //t5 = t1^(fai(n)-1) mod n
            ret |= pke_modexp_without_pub(n, t3, t1, t5, wordLen, wordLen);
            if(PKE_SUCCESS != ret || uint32_BigNumCmp(t5, wordLen, t2, wordLen))
            {
                printf("\r\n %u pke_modexp_without_pub(a^(fai(n)-1) = a^(-1) mod n) failure, ret=%x", bitLen, ret);
                print_BN_buf_U32(t5, wordLen, "t5");
                return 1;
            }

            //t2 = t1*t1^(-1) = 1 mod n
            ret = pke_modmul(n, t1, t5, t2, wordLen);
            if(PKE_SUCCESS != ret || uint32_BigNumCmp(t2, wordLen, t4, wordLen))
            {
                printf("\r\n %u pke_modmul failure, ret=%x", bitLen, ret);
                print_BN_buf_U32(t1, wordLen, "t1");
                print_BN_buf_U32(t5, wordLen, "t5");
                print_BN_buf_U32(t2, wordLen, "t2");
                return 1;
            }
        }
    }

    //a^(b+c) mod n = (a^b * a^c) mod n
    printf("\r\n %u pke_modexp_without_pub(a^(b+c) = (a^b * a^c) mod n) test \r\n", bitLen);
    for(i=0;i<100;i++)
    {
        printf(" %u", i+1);
        ret = get_rand((uint8_t *)t1, wordLen<<2);
        t1[wordLen-1] = 0;

        ret |= get_rand((uint8_t *)t2, wordLen<<2);
        t2[wordLen-1] = 0;

        ret |= get_rand((uint8_t *)t3, wordLen<<2);
        t3[wordLen-1] = 0;

        ret |= pke_modexp_without_pub(n, t2, t1, t4, wordLen, wordLen);

        ret |= pke_modexp_without_pub(n, t3, t1, t5, wordLen, wordLen);

        ret |= pke_modmul(n, t4, t5, t5, wordLen);

        ret = pke_add(t2, t3, t2, wordLen);

        ret |= pke_modexp_without_pub(n, t2, t1, t4, wordLen, wordLen);

        if(PKE_SUCCESS != ret || uint32_BigNumCmp(t5, wordLen, t4, wordLen))
        {
            printf("\r\n %u pke_modexp_without_pub(a^(b+c) = (a^b * a^c) mod n) failure, ret=%x", bitLen, ret);
            print_BN_buf_U32(t5, wordLen, "t5");
            return 1;
        }
    }

#if 1
    // modexp: 0^0 mod n     //error, 计算成功，但结果是0,耗时非常久，估计是0-1后生成非常大的数导致
    uint32_clear(t1, wordLen);
    uint32_clear(t2, wordLen);
    ret = pke_modexp_without_pub(n, t1, t2, t3, wordLen, wordLen);
    print_BN_buf_U32(t3, wordLen, "t3");
    if(PKE_SUCCESS != ret)
    {
        printf("\r\n %u pke_modexp_without_pub(0^0 mod n) failure, ret=%x", bitLen, ret);

        return 1;
    }
    else
    {
        printf("\r\n %u pke_modexp_without_pub(0^0 mod n) success", bitLen);
    }
#endif

    // modexp: 0^1 mod n
    pke_set_operand_uint32_value(t1, wordLen, 1);
    uint32_clear(t2, wordLen);
    uint32_clear(t3, wordLen);
    ret = pke_modexp_without_pub(n, t1, t2, t3, wordLen, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t3, wordLen, t2, wordLen))
    {
        printf("\r\n %u pke_modexp_without_pub(0^1 mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modexp_without_pub(0^1 mod n) success", bitLen);
    }

    // modexp: 1^0 mod n    //虽然结果是1，但耗时非常久，估计是0-1后生成非常大的数导致
    uint32_clear(t1, wordLen);
    pke_set_operand_uint32_value(t2, wordLen, 1);
    uint32_clear(t3, wordLen);
    ret = pke_modexp_without_pub(n, t1, t2, t3, wordLen, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t3, wordLen, t2, wordLen))
    {
        printf("\r\n %u pke_modexp_without_pub(1^0 mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modexp_without_pub(1^0 mod n) success", bitLen);
    }

    // modexp: 1^1 mod n
    pke_set_operand_uint32_value(t1, wordLen, 1);
    pke_set_operand_uint32_value(t2, wordLen, 1);
    uint32_clear(t3, wordLen);
    ret = pke_modexp_without_pub(n, t1, t2, t3, wordLen, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t3, wordLen, t1, wordLen))
    {
        printf("\r\n %u pke_modexp_without_pub(1^1 mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t3, wordLen, "t3");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modexp_without_pub(1^1 mod n) success", bitLen);
    }

    // modexp: (n-1)^0 mod n    //虽然结果是1，但耗时非常久，估计是0-1后生成非常大的数导致
    uint32_clear(t2, wordLen);
    pke_set_operand_uint32_value(t3, wordLen, 1);
    big_sub(n, t3, t1, wordLen);
    ret = pke_modexp_without_pub(n, t2, t1, t4, wordLen, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t4, wordLen, t3, wordLen))
    {
        printf("\r\n %u pke_modexp_without_pub((n-1)^0 mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t4, wordLen, "t4");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modexp_without_pub((n-1)^0 mod n) success", bitLen);
    }

    // modexp: 0^(n-1) mod n
    uint32_clear(t2, wordLen);
    pke_set_operand_uint32_value(t3, wordLen, 1);
    big_sub(n, t3, t1, wordLen);
    ret = pke_modexp_without_pub(n, t1, t2, t4, wordLen, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t4, wordLen, t2, wordLen))
    {
        printf("\r\n %u pke_modexp_without_pub(0^(n-1) mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t4, wordLen, "t4");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modexp_without_pub(0^(n-1) mod n) success", bitLen);
    }

    // modexp: (n-1)^1 mod n
    uint32_clear(t2, wordLen);
    pke_set_operand_uint32_value(t3, wordLen, 1);
    big_sub(n, t3, t1, wordLen);
    ret = pke_modexp_without_pub(n, t3, t1, t4, wordLen, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t4, wordLen, t1, wordLen))
    {
        printf("\r\n %u pke_modexp_without_pub((n-1)^1 mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t4, wordLen, "t4");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modexp_without_pub((n-1)^1 mod n) success", bitLen);
    }

    // modexp: 1^(n-1) mod n
    uint32_clear(t2, wordLen);
    pke_set_operand_uint32_value(t3, wordLen, 1);
    big_sub(n, t3, t1, wordLen);
    ret = pke_modexp_without_pub(n, t1, t3, t4, wordLen, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t4, wordLen, t3, wordLen))
    {
        printf("\r\n %u pke_modexp_without_pub(1^(n-1) mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t4, wordLen, "t4");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modexp_without_pub(1^(n-1) mod n) success", bitLen);
    }


    // modexp: (n-1)^(n-1) mod n
    uint32_clear(t2, wordLen);
    pke_set_operand_uint32_value(t3, wordLen, 1);
    big_sub(n, t3, t1, wordLen);
    ret = pke_modexp_without_pub(n, t1, t1, t4, wordLen, wordLen);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(t4, wordLen, t3, wordLen))
    {
        printf("\r\n %u pke_modexp_without_pub((n-1)^(n-1) mod n) failure, ret=%x", bitLen, ret);
        print_BN_buf_U32(t4, wordLen, "t4");
        return 1;
    }
    else
    {
        printf("\r\n %u pke_modexp_without_pub((n-1)^(n-1) mod n) success", bitLen);
    }

    printf("\r\n");

    return 0;
}


#endif


uint32_t pke_basic_test(pke_basic_test_vector_t *vector)
{
    uint32_t n0[OPERAND_MAX_WORD_LEN];
    uint32_t n1[OPERAND_MAX_WORD_LEN];
    uint32_t n2[OPERAND_MAX_WORD_LEN];
    uint32_t *e = n0;

    uint32_t wordLen = GET_WORD_LEN(vector->bitLen);
    uint32_t tmpBitLen = (vector->bitLen)&31;
    //uint32_t ret;

    printf("\r\n ---------------- operand %u bits ----------------\r\n", vector->bitLen);

    //n=100...000
    uint32_clear(n0, wordLen);
    n0[wordLen-1]=((uint32_t)1)<<(0==tmpBitLen?31:(tmpBitLen-1));

    //n=100...001
    uint32_clear(n1, wordLen);
    n1[0]=1;
    n1[wordLen-1]=((uint32_t)1)<<(0==tmpBitLen?31:(tmpBitLen-1));

    //n=111...111
    uint32_set(n2, 0xFFFFFFFF, wordLen);
    if(tmpBitLen)
    {
        n2[wordLen-1] = ((1<<tmpBitLen)-1);
    }

    /************ add ************/
    if(pke_basic_add_test(vector->a, vector->b, vector->std_a_add_b, vector->bitLen))
    {
        return 1;
    }

    /************ sub ************/
    if(pke_basic_sub_test(vector->a, vector->b, vector->std_a_sub_b, vector->std_b_sub_a, vector->bitLen))
    {
        return 1;
    }

    /************ mul ************/
    if(pke_basic_mul_test(vector->p, vector->q, vector->n, vector->fai_n, vector->bitLen))
    {
        return 1;
    }

    /************ modadd ************/
    printf("\r\n ------------- pke_modadd(a,b,n are random but fixed data) -------------");
    if(pke_basic_modadd_test(vector->a, vector->b, vector->n, vector->std_modadd, vector->bitLen))
    {
        return 1;
    }

    printf("\r\n ------------- pke_modadd(corner case,n=100...000) -------------");
    if(pke_basic_modadd_test(NULL, NULL, n0, NULL, vector->bitLen))
    {
        return 1;
    }

    printf("\r\n ------------- pke_modadd(corner case,n=100...001) -------------");
    if(pke_basic_modadd_test(NULL, NULL, n1, NULL, vector->bitLen))
    {
        return 1;
    }

    printf("\r\n ------------- pke_modadd(corner case,n=111...111) -------------");
    if(pke_basic_modadd_test(NULL, NULL, n2, NULL, vector->bitLen))
    {
        return 1;
    }

    /************ modsub ************/
    printf("\r\n ------------- pke_modsub(a,b,n are random but fixed data) -------------");
    if(pke_basic_modsub_test(vector->a, vector->b, vector->n, vector->std_modsub_a_b, vector->std_modsub_b_a, vector->bitLen))
    {
        return 1;
    }

    printf("\r\n ------------- pke_modsub(corner case,n=100...000) -------------");
    if(pke_basic_modsub_test(NULL, NULL, n0, NULL, NULL, vector->bitLen))
    {
        return 1;
    }

    printf("\r\n ------------- pke_modsub(corner case,n=100...001) -------------");
    if(pke_basic_modsub_test(NULL, NULL, n1, NULL, NULL, vector->bitLen))
    {
        return 1;
    }

    printf("\r\n ------------- pke_modsub(corner case,n=111...111) -------------");
    if(pke_basic_modsub_test(NULL, NULL, n2, NULL, NULL, vector->bitLen))
    {
        return 1;
    }

    /************ modmul(only available for odd modulus) ************/
    if(vector->n[0] & 1)
    {
        printf("\r\n ------------- pke_modmul(a,b,n,p,q are random but fixed data) -------------");
        if(pke_basic_modmul_test(vector->a, vector->b, vector->n, vector->p, vector->q, vector->std_modmul, vector->bitLen))
        {
            return 1;
        }

        printf("\r\n ------------- pke_modmul(corner case,n=100...001) -------------");
        if(pke_basic_modmul_test(NULL, NULL, n1, NULL, NULL, NULL, vector->bitLen))
        {
            return 1;
        }
#if 1
        printf("\r\n ------------- pke_modmul(corner case,n=111...111) -------------");
        if(pke_basic_modmul_test(NULL, NULL, n2, NULL, NULL, NULL, vector->bitLen))
        {
            return 1;
        }
#endif
    }

    /************ pre_mont(only available for odd modulus) ************/
    if((vector->n[0] & 1) && (NULL != vector->std_n_h))
    {
        printf("\r\n ------------- pke_pre_calc_mont(n is random but fixed data) -------------");
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
        if(pke_basic_pre_mont_test(vector->n, vector->std_n_h, vector->std_n_n0, vector->bitLen))
#else
        if(pke_basic_pre_mont_test(vector->n, vector->std_n_h, NULL, vector->bitLen))
#endif
        {
            return 1;
        }

        printf("\r\n ------------- pke_pre_calc_mont(corner case,n=100...001) -------------");
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
        if(pke_basic_pre_mont_test(n1, vector->std_10_01_n_h, vector->std_10_01_n_n0, vector->bitLen))
#else
        if(pke_basic_pre_mont_test(n1, vector->std_10_01_n_h, NULL, vector->bitLen))
#endif
        {
            return 1;
        }

        printf("\r\n ------------- pke_pre_calc_mont(corner case,n=111...111) -------------");
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
        if(pke_basic_pre_mont_test(n2, vector->std_11_11_n_h, vector->std_11_11_n_n0, vector->bitLen))
#else
        if(pke_basic_pre_mont_test(n2, vector->std_11_11_n_h, NULL, vector->bitLen))
#endif
        {
            return 1;
        }
    }

    /************ modinv ************/
    printf("\r\n ------------- pke_modinv(a, n are random but fixed data) -------------");
    if(pke_basic_modinv_test(vector->a, vector->n, vector->std_modinv_odd, vector->std_modinv_even, vector->bitLen))
    {
        return 1;
    }

    if(vector->n[0] & 1)
    {
        printf("\r\n ------------- pke_modinv(a is 0x10001, n is (p-1)*(q-1), to get d) -------------");
        pke_set_operand_uint32_value(e, wordLen, 0x10001);
        if(pke_basic_modinv_test(e, vector->fai_n, NULL, vector->d, vector->bitLen))
        {
            return 1;
        }
    }

    printf("\r\n ------------- pke_modinv(corner case,n=100...001 and 100...000) -------------");
    if(pke_basic_modinv_test(NULL, n1, NULL, NULL, vector->bitLen))
    {
        return 1;
    }

    printf("\r\n ------------- pke_modinv(corner case,n=111...111 and 111...110) -------------");
    if(pke_basic_modinv_test(NULL, n2, NULL, NULL, vector->bitLen))
    {
        return 1;
    }

    /************ modexp(only available for odd modulus) ************/
    if(vector->n[0] & 1)
    {
        printf("\r\n ------------- pke_modexp(a,b,n,p,q are random but fixed data) -------------");
        if(pke_basic_modexp_test(vector->a, vector->b, vector->n, vector->p, vector->q, vector->std_modexp, vector->bitLen))
        {
            return 1;
        }

        printf("\r\n ------------- pke_modexp(corner case,n=100...001) -------------");
        if(pke_basic_modexp_test(NULL, NULL, n1, NULL, NULL, NULL, vector->bitLen))
        {
            return 1;
        }

        printf("\r\n ------------- pke_modexp(corner case,n=111...111) -------------");
        if(pke_basic_modexp_test(NULL, NULL, n2, NULL, NULL, NULL, vector->bitLen))
        {
            return 1;
        }

#ifdef AIC_PKE_SEC
        printf("\r\n ------------- pke_modexp_ladder(a,b,n,p,q are random but fixed data) -------------");
        if(pke_basic_modexp_ladder_test(vector->a, vector->b, vector->n, vector->p, vector->q, vector->std_modexp, vector->bitLen))
        {
            return 1;
        }

        printf("\r\n ------------- pke_modexp_ladder(corner case,n=100...001) -------------");
        if(pke_basic_modexp_ladder_test(NULL, NULL, n1, NULL, NULL, NULL, vector->bitLen))
        {
            return 1;
        }

        printf("\r\n ------------- pke_modexp_ladder(corner case,n=111...111) -------------");
        if(pke_basic_modexp_ladder_test(NULL, NULL, n2, NULL, NULL, NULL, vector->bitLen))
        {
            return 1;
        }

        printf("\r\n ------------- pke_modexp_with_pub(a,d,n,fai_n are random but fixed data) -------------");
        if(pke_basic_modexp_with_pub_test(vector->a, vector->d, vector->n, vector->fai_n, vector->std_modexp_d, vector->bitLen))
        {
            return 1;
        }

        printf("\r\n ------------- pke_modexp_without_pub(a,b,n,p,q are random but fixed data) -------------");
        if(pke_basic_modexp_without_pub_test(vector->a, vector->b, vector->n, vector->p, vector->q, vector->std_modexp, vector->bitLen))
        {
            return 1;
        }

        printf("\r\n ------------- pke_modexp_without_pub(corner case,n=100...001) -------------");
        if(pke_basic_modexp_without_pub_test(NULL, NULL, n1, NULL, NULL, NULL, vector->bitLen))
        {
            return 1;
        }

        printf("\r\n ------------- pke_modexp_without_pub(corner case,n=111...111) -------------");
        if(pke_basic_modexp_without_pub_test(NULL, NULL, n2, NULL, NULL, NULL, vector->bitLen))
        {
            return 1;
        }


#endif
    }

    printf("\r\n");

    return 0;
}




#ifdef PKE_BASIC_160_TEST
uint32_t pke_basic_160_test(void)
{
    pke_basic_test_vector_t   vector[1];

    uint32_t a[]                ={0xEE053693,0xFA0F948D,0x0E3030D6,0xABC4ED96,0x889D4A96};
    uint32_t b[]                ={0xAA2485E9,0xB3091313,0x0A8737EB,0x6D71D62B,0x0A464089};
    uint32_t std_a_add_b[]      ={0x9829BC7C,0xAD18A7A1,0x18B768C2,0x1936C3C1,0x92E38B20};
    uint32_t std_a_sub_b[]      ={0x43E0B0AA,0x4706817A,0x03A8F8EB,0x3E53176B,0x7E570A0D};
    uint32_t std_b_sub_a[]      ={0xBC1F4F56,0xB8F97E85,0xFC570714,0xC1ACE894,0x81A8F5F2};
    uint32_t p[]                ={0xECFAB191,0x0278B1E6,0x0000D8F6};
    uint32_t q[]                ={0x4AAA1F33,0xC38295CB,0x0000B1CC};
    uint32_t fai_n[]            ={0x2E171E20,0xAC3B28CA,0xEA168746,0x7AF11876,0x96AFA171};
    uint32_t d[]                ={0x30045A41,0x43B288EF,0x3F758CFE,0x510250EB,0x75B082D5};
    uint32_t n[]                ={0x65BBEEE3,0x7236707C,0xEA181209,0x7AF11876,0x96AFA171};
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    uint32_t n_h[]              ={0xE6EE38EC,0x4AB9BD22,0x2A8A41BB,0xAD0C05DF,0x81713831};
    uint32_t n_n0[]             ={0x9E2D3935};
#else
    uint32_t n_h[]              ={0xB5A8FDEE,0x87F5C696,0xB127BECB,0xF0F854D8,0x0B4B5918,};
#endif
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    uint32_t n_10_01_h[(160+31)/32] ={4, 0, 0, 0, 0};
    uint32_t n_10_01_n0[]           ={0xFFFFFFFF};
#else
    uint32_t n_10_01_h[(160+31)/32] ={0x00000001,0xFFFFFFF8,0xFFFFFFFF,0xFFFFFFFF,0x7FFFFFFF};
#endif
    uint32_t n_11_11_h[(160+31)/32] ={0};
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    uint32_t n_11_11_n0[]           ={1};
#endif
    uint32_t std_modsub_b_a[]   ={0x21DB3E39,0x2B2FEF02,0xE66F191E,0x3C9E010B,0x18589764};
    uint32_t std_modmul[]       ={0x6FED61D0,0x2E8B893F,0x5701232C,0xB692C4C6,0x4DF08032};
    uint32_t std_modinv_odd[]   ={0x80B5ECA7,0x7D2810CE,0x9548774B,0x395FF86F,0x6C865968};
    uint32_t std_modinv_even[]  ={0x7369DB15,0x0324C985,0x8718A5CB,0x6E5EED3A,0x6D7A05D4};
    uint32_t std_modexp[]       ={0x6CC7EA9D,0xAB062F62,0x86448D1F,0xCEAB0A53,0x3E937216};
    uint32_t std_modexp_d[]     ={0x8BE19DE0,0x91C9AA7F,0x4B5CA184,0xB0E154AF,0x3B599E43};

    vector->bitLen          = 160;
    vector->a               = a;
    vector->b               = b;
    vector->p               = p;
    vector->q               = q;
    vector->fai_n           = fai_n;
    vector->d               = d;
    vector->n               = n;
    vector->std_n_h         = n_h;
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    vector->std_n_n0        = n_n0;
    vector->std_10_01_n_h   = n_10_01_h;
    vector->std_10_01_n_n0  = n_10_01_n0;
    n_11_11_h[0]            = 1;
    vector->std_11_11_n_h   = n_11_11_h;
    vector->std_11_11_n_n0  = n_11_11_n0;
#else
    vector->std_10_01_n_h   = n_10_01_h;
    n_11_11_h[1]            = 1;
    vector->std_11_11_n_h   = n_11_11_h;
#endif
    vector->std_a_add_b     = std_a_add_b;
    vector->std_a_sub_b     = std_a_sub_b;
    vector->std_b_sub_a     = std_b_sub_a;
    vector->std_modadd      = std_a_add_b;
    vector->std_modsub_a_b  = std_a_sub_b;
    vector->std_modsub_b_a  = std_modsub_b_a;
    vector->std_modmul      = std_modmul;
    vector->std_modinv_odd  = std_modinv_odd;
    vector->std_modinv_even = std_modinv_even;
    vector->std_modexp      = std_modexp;
    vector->std_modexp_d    = std_modexp_d;

    return pke_basic_test(vector);
}
#endif


#ifdef PKE_BASIC_256_TEST
uint32_t pke_basic_256_test(void)
{
    pke_basic_test_vector_t   vector[1];

    uint32_t a[]                    ={0x8151A37F,0xFEF3BB4A,0x9B6C0972,0xB79A7808,0x6CFC70EF,0xDB58CCA7,0xA399E7E6,0x2A53B5BC,};
    uint32_t b[]                    ={0x13394777,0x6BE802BF,0x050D4814,0x8E1940E3,0x91829AA6,0x1646579D,0xB40931A9,0x44431F9D,};
    uint32_t std_a_add_b[]          ={0x948AEAF6,0x6ADBBE09,0xA0795187,0x45B3B8EB,0xFE7F0B96,0xF19F2444,0x57A3198F,0x6E96D55A,};
    uint32_t std_a_sub_b[]          ={0x6E185C08,0x930BB88B,0x965EC15E,0x29813725,0xDB79D649,0xC5127509,0xEF90B63D,0xE610961E};
    uint32_t std_b_sub_a[]          ={0x91E7A3F8,0x6CF44774,0x69A13EA1,0xD67EC8DA,0x248629B6,0x3AED8AF6,0x106F49C2,0x19EF69E1};
    uint32_t p[]                    ={0x0E3C1B83,0xE8C51A7A,0x1275C68E,0xEF946703};
    uint32_t q[]                    ={0x3B13F98D,0xA4DE2B6C,0xD296D8F8,0xDCC4C498};
    uint32_t fai_n[]                ={0x40467D18,0x57988C3D,0x0BF725DB,0xB7ADE0A3,0x9EFB0F1B,0xC151B67D,0x2647C546,0xCE9BAE22};
    uint32_t d[]                    ={0x945CF331,0xBED04ED0,0x13E11A60,0x73CE368A,0x3A638059,0xAB3BA499,0xA6EDFF74,0xAD048E24};
    uint32_t n[]                    ={0x89969227,0xE53BD223,0xF103C562,0x84070C3E,0x9EFB0F1D,0xC151B67D,0x2647C546,0xCE9BAE22,};
    uint32_t n_h[]                  ={0xAE88E8C7,0x4ED664C6,0x6AB2F6E3,0xDC21170C,0x33FE3612,0xBA1CFA6B,0x91CF880E,0x0143DCEE,};
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    uint32_t n_n0[]                 ={0x4DF04269,};
#endif
    uint32_t n_10_01_h[(256+31)/32] ={0};
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    uint32_t n_10_01_n0[]           ={0xFFFFFFFF};
#endif
    uint32_t n_11_11_h[(256+31)/32] ={0};
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    uint32_t n_11_11_n0[]           ={1};
#endif
    uint32_t std_modsub_a_b[]       ={0xF7AEEE2F,0x78478AAE,0x876286C1,0xAD884364,0x7A74E566,0x86642B87,0x15D87B84,0xB4AC4441,};
    uint32_t std_modmul[]           ={0x14D0ED51,0xE9BB0D57,0xC0A56240,0xFDA1DCF5,0x2DDD24A5,0xE289C8A7,0xF801D7F7,0xADD57EEE,};
    uint32_t std_modinv_odd[]       ={0x3BC79238,0xAC6A7993,0x21832306,0xBA1099B9,0x1599E5C8,0xC854CDDC,0x7C5F4C28,0x679165AE,};
    uint32_t std_modinv_even[]      ={0xFD323B19,0xCC7049B6,0x0AE2A3AC,0xE991365F,0xFFDAD84E,0x122B9E31,0x31D06CE9,0x5D55111E,};
    uint32_t std_modexp[]           ={0x1445261A,0xDA8BE0FD,0xC57B5706,0x9B3B8D7C,0xBAF039DA,0xB7030508,0x0D7F7476,0x6EB9AFEC,};
    uint32_t std_modexp_d[]         ={0xE4790286,0x4C2A541B,0xFD938CCF,0x8AC93563,0xCDEC67DD,0xAEB2850A,0x907999AF,0x3948D5C1,};

    vector->bitLen          = 256;
    vector->a               = a;
    vector->b               = b;
    vector->p               = p;
    vector->q               = q;
    vector->fai_n           = fai_n;
    vector->d               = d;
    vector->n               = n;
    vector->std_n_h         = n_h;
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    vector->std_n_n0        = n_n0;
#endif
    n_10_01_h[0]            = 4;
    vector->std_10_01_n_h   = n_10_01_h;
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    vector->std_10_01_n_n0  = n_10_01_n0;
#endif
    n_11_11_h[0]            = 1;
    vector->std_11_11_n_h   = n_11_11_h;
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    vector->std_11_11_n_n0  = n_11_11_n0;
#endif
    vector->std_a_add_b     = std_a_add_b;
    vector->std_a_sub_b     = std_a_sub_b;
    vector->std_b_sub_a     = std_b_sub_a;
    vector->std_modadd      = std_a_add_b;
    vector->std_modsub_a_b  = std_modsub_a_b;
    vector->std_modsub_b_a  = std_b_sub_a;
    vector->std_modmul      = std_modmul;
    vector->std_modinv_odd  = std_modinv_odd;
    vector->std_modinv_even = std_modinv_even;
    vector->std_modexp      = std_modexp;
    vector->std_modexp_d    = std_modexp_d;

    return pke_basic_test(vector);
}
#endif


#ifdef PKE_BASIC_521_TEST    //caution: n is even here
uint32_t pke_basic_521_test(void)
{
    pke_basic_test_vector_t   vector[1];

    uint32_t a[]                ={0x4DECD4E9,0xEEE0BC4E,0x16206390,0xA45F2290,0x65386C47,0x42890045,0x0ECC88F4,0x8BC22F32,0x326BA533,
                                  0x376EFC3A,0x8C9E3B76,0x5B2625E6,0xB0DE11A2,0x2A6124E8,0x97F35891,0xA4911F04,0x00000120};
    uint32_t b[]                ={0x45179671,0x6C00E4C5,0x89A6E19D,0x55AB0369,0x3B84A8A3,0x0F99A817,0x2BF80A7F,0x35F286CA,0x9A4FF64C,
                                  0x1C8B7B6B,0xAF38F9AC,0x72B60943,0x295D4D68,0xF997A14A,0x3D05AA19,0x449FCCBE,0x0000007D};
    uint32_t std_a_add_b[]      ={0x93046B5A,0x5AE1A113,0x9FC7452E,0xFA0A25F9,0xA0BD14EA,0x5222A85C,0x3AC49373,0xC1B4B5FC,0xCCBB9B7F,
                                  0x53FA77A5,0x3BD73522,0xCDDC2F2A,0xDA3B5F0A,0x23F8C632,0xD4F902AB,0xE930EBC2,0x0000019D};
    uint32_t std_a_sub_b[]      ={0x08D53E78,0x82DFD789,0x8C7981F3,0x4EB41F26,0x29B3C3A4,0x32EF582E,0xE2D47E75,0x55CFA867,0x981BAEE7,
                                  0x1AE380CE,0xDD6541CA,0xE8701CA2,0x8780C439,0x30C9839E,0x5AEDAE77,0x5FF15246,0x000000A3};
    uint32_t std_b_sub_a[]      ={0xF72AC188,0x7D202876,0x73867E0C,0xB14BE0D9,0xD64C3C5B,0xCD10A7D1,0x1D2B818A,0xAA305798,0x67E45118,
                                  0xE51C7F31,0x229ABE35,0x178FE35D,0x787F3BC6,0xCF367C61,0xA5125188,0xA00EADB9,0xFFFFFF5C};
    uint32_t p[]                ={0x46CE1C51,0x39231FC3,0x55C16097,0x681755CC,0x2B2D3297,0x6F422E99,0x6AA75ACE,0x3D471F73,0x0000001C};
    uint32_t q[]                ={0x4F8A9420,0x49D42E04,0x60A7040C,0x2A250550,0x7DB24338,0x5D333CAF,0x1E6F738F,0x2EFB0458,0x0000000C};
    uint32_t n[]                ={0x44CC5E20,0x8541A274,0xD444BA00,0xB7C93996,0x42FDE6CF,0x826B8CA6,0xE48E8E47,0x6C9DE039,0x19A88E3A,
                                  0x6287AA34,0x59059220,0x606E9EB3,0x73AB098B,0x3DC19D0B,0x1B962A49,0x0E08D079,0x00000158};
    uint32_t std_modadd[]       ={0x4E380D3A,0xD59FFE9F,0xCB828B2D,0x4240EC62,0x5DBF2E1B,0xCFB71BB6,0x5636052B,0x5516D5C2,0xB3130D45,
                                  0xF172CD71,0xE2D1A301,0x6D6D9076,0x6690557F,0xE6372927,0xB962D861,0xDB281B49,0x00000045,};
    uint32_t std_modsub_b_a[]   ={0x3BF71FA8,0x0261CAEB,0x47CB380D,0x69151A70,0x194A232B,0x4F7C3478,0x01BA0FD2,0x16CE37D2,0x818CDF53,
                                  0x47A42965,0x7BA05056,0x77FE8210,0xEC2A4551,0x0CF8196C,0xC0A87BD2,0xAE177E32,0x000000B4};
//    uint32_t std_modmul[]       ={0x0333293A,0x81C2FFA1,0x5FE9E04D,0x1D58F7D5,0x82BDA258,0x20754684,0xDE4E40C3,0x7EF96D14,0xB455AEBF,
//                                  0x3634D0EB,0x42C1D52E,0x8D06170D,0x609B3754,0x34F349DA,0xF6AD58D8,0x988C2A14,0x000002E4};
    uint32_t std_modinv_odd[]   ={0xA39EF4B0,0xFDCC06F2,0xA5B8A1C6,0x25B232CF,0xDACB012B,0x2618B31B,0x89A89E8E,0x0DEBA9C6,0xF6A690E7,
                                  0xCDC47519,0x7684867F,0x586B506E,0x439945E8,0x501E0C2D,0xBF53CB51,0x5B867D2D,0x0000011C};
    uint32_t std_modinv_even[]  ={0xC6D9A639,0xD89E062F,0xF3FFA1C6,0x80A54724,0xAA3C1ACC,0xED0B818F,0xAA217CB8,0x29C0E132,0x623F3A1C,
                                  0xCCDDF888,0xD37ECB94,0xFE280E3B,0x05C63E6B,0x88EFA90C,0xA2A1A886,0xB5F7D246,0x00000149};
//    uint32_t std_modexp[]       ={0xC4618F1C,0x673D9387,0xCEB131B8,0xF89D0EA1,0xE83E34C3,0xF771806D,0x81C5828B,0x8788D4A9,0x923F04E0,
//                                  0x4C6F7428,0xA80DA2EC,0x46D64020,0x7297A97E,0x26E860AA,0xBB0F47FD,0xBD9EE19A,0x0000018F};

    vector->bitLen          = 521;
    vector->a               = a;
    vector->b               = b;
    vector->p               = p;
    vector->q               = q;
    vector->fai_n           = NULL;
    vector->d               = NULL;
    vector->n               = n;
    vector->std_n_h         = NULL;
    vector->std_10_01_n_h   = NULL;
    vector->std_11_11_n_h   = NULL;
    vector->std_a_add_b     = std_a_add_b;
    vector->std_a_sub_b     = std_a_sub_b;
    vector->std_b_sub_a     = std_b_sub_a;
    vector->std_modadd      = std_modadd;
    vector->std_modsub_a_b  = std_a_sub_b;
    vector->std_modsub_b_a  = std_modsub_b_a;
    vector->std_modmul      = NULL;
    vector->std_modinv_odd  = std_modinv_odd;
    vector->std_modinv_even = std_modinv_even;
    vector->std_modexp      = NULL;
    vector->std_modexp_d    = NULL;

    return pke_basic_test(vector);
}
#endif


#ifdef PKE_BASIC_522_TEST
uint32_t pke_basic_522_test(void)
{
    pke_basic_test_vector_t   vector[1];

    uint32_t a[]                ={0xC144CED9,0x215EFCF0,0x471EE748,0xBD21DC8E,0x43A15F5D,0xD97999A8,0x69D7D860,0x6D6BEA85,0xC716FEA0,
                                  0xF515BF90,0x59E93FA4,0x76DE206C,0x7B96DA97,0xB33F436C,0x93A0C600,0x02C0E850,0x0000033A};
    uint32_t b[]                ={0x7AFC1FA9,0xD3535DEB,0xA679972A,0xE254B9A7,0xF2BACA8F,0xEF504532,0xDF895E96,0xA56275D6,0xB12DBE4A,
                                  0x6BF877FE,0x53416770,0x21750902,0xA224D687,0xE7D94243,0x1CA87F4B,0x33C60011,0x00000281};
    uint32_t std_a_add_b[]      ={0x3C40EE82,0xF4B25ADC,0xED987E72,0x9F769635,0x365C29ED,0xC8C9DEDB,0x496136F7,0x12CE605C,0x7844BCEB,
                                  0x610E378F,0xAD2AA715,0x9853296E,0x1DBBB11E,0x9B1885B0,0xB049454C,0x3686E861,0x000005BB};
    uint32_t std_a_sub_b[]      ={0x4648AF30,0x4E0B9F05,0xA0A5501D,0xDACD22E6,0x50E694CD,0xEA295475,0x8A4E79C9,0xC80974AE,0x15E94055,
                                  0x891D4792,0x06A7D834,0x5569176A,0xD9720410,0xCB660128,0x76F846B4,0xCEFAE83F,0x000000B8};
    uint32_t std_b_sub_a[]      ={0xB9B750D0,0xB1F460FA,0x5F5AAFE2,0x2532DD19,0xAF196B32,0x15D6AB8A,0x75B18636,0x37F68B51,0xEA16BFAA,
                                  0x76E2B86D,0xF95827CB,0xAA96E895,0x268DFBEF,0x3499FED7,0x8907B94B,0x310517C0,0xFFFFFF47};
    uint32_t p[]                ={0x010A3019,0x137448FF,0x0DC92ABE,0x7CC27C5F,0x1B9C513F,0x1E8B37D9,0x749958C1,0x37467337,0x0000001F};
    uint32_t q[]                ={0x0E538305,0x0A083E01,0x2F4B34E4,0x235A7B7E,0x74E94CB5,0x5E0D40A2,0x70FF48C1,0x4013228A,0x0000001C};
    uint32_t fai_n[]            ={0x228D0860,0x5557868B,0x61B8C870,0x45F197FD,0x5D57B333,0x5DD41EE7,0x570E0915,0x2E64EA52,0xD622F767,
                                  0xB0AB6FF3,0xEBC66671,0xD4658AE8,0x63631A4B,0x6953EBE4,0x19A0ECC7,0xDBDB8752,0x00000371};
    uint32_t d[]                ={0x36EAEE21,0x8E247E24,0x7BC2666B,0x5D707FFB,0x0930FE05,0x80F9D70E,0xBF144840,0xF26995E8,0xFCBAAE9C,
                                  0x57364FC2,0x3FBF1703,0x8BCABC22,0x92C3C19E,0x9E1D3884,0xE2F0BE21,0xBC4F7100,0x000002B1};
    uint32_t n[]                ={0x31EABB7D,0x72D40D8B,0x9ECD2812,0xE60E8FDA,0xEDDD5127,0xDA6C9762,0x3CA6AA97,0xA5BE8014,0xD622F7A2,
                                  0xB0AB6FF3,0xEBC66671,0xD4658AE8,0x63631A4B,0x6953EBE4,0x19A0ECC7,0xDBDB8752,0x00000371};
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    uint32_t n_h[]              ={0x888FD26A,0xC403B844,0xDD296B26,0x0E9091C2,0x5B18DDEB,0xF0EDE439,0xBE9C521C,0xA0CFC7AC,0x7F724189,
                                  0xA723EBED,0x51E8C711,0x052F5437,0xCD4A4E08,0xFC3CF01F,0x66E66C91,0x59F7340F,0x0000017F};
    uint32_t n_n0[]             ={0x5F102A2B,};
#else
    uint32_t n_h[]              ={0x66CFDE51,0x1BDE44E8,0xE094FD71,0x099594A8,0x7BF9A6A6,0xCC910081,0xBDBC1030,0x763D6D3B,0xA617849B,
                                  0x1386B156,0x02CEC061,0xCC7DA7C8,0xECAEE8C4,0x49D72BC8,0x7A029445,0xD6820C90,0x00000308};
#endif
    uint32_t n_10_01_h[(522+31)/32] ={0};
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    uint32_t n_10_01_n0[]           ={0xFFFFFFFF};
#endif
    uint32_t n_11_11_h[(522+31)/32] ={0};
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    uint32_t n_11_11_n0[]           ={1};
#endif
    uint32_t std_modadd[]       ={0x0A563305,0x81DE4D51,0x4ECB5660,0xB968065B,0x487ED8C5,0xEE5D4778,0x0CBA8C5F,0x6D0FE048,0xA221C548,
                                  0xB062C79B,0xC16440A3,0xC3ED9E85,0xBA5896D2,0x31C499CB,0x96A85885,0x5AAB610F,0x00000249,};
    uint32_t std_modsub_b_a[]   ={0xEBA20C4D,0x24C86E85,0xFE27D7F5,0x0B416CF3,0x9CF6BC5A,0xF04342ED,0xB25830CD,0xDDB50B65,0xC039B74C,
                                  0x278E2861,0xE51E8E3D,0x7EFC737E,0x89F1163B,0x9DEDEABB,0xA2A8A612,0x0CE09F12,0x000002B9};
    uint32_t std_modmul[]       ={0x0333293A,0x81C2FFA1,0x5FE9E04D,0x1D58F7D5,0x82BDA258,0x20754684,0xDE4E40C3,0x7EF96D14,0xB455AEBF,
                                  0x3634D0EB,0x42C1D52E,0x8D06170D,0x609B3754,0x34F349DA,0xF6AD58D8,0x988C2A14,0x000002E4};
    uint32_t std_modinv_odd[]   ={0xC8226E54,0x70BEF92E,0x69556388,0x5F89156F,0x30602D0D,0xC61C12BD,0xA4D9D915,0x31CF11EB,0x92F1CDF9,
                                  0x8B9A90FE,0xDAA3F0AB,0x4C37E426,0x227FCB96,0x50F893A3,0xB600724C,0x43700DCD,0x00000126};
    uint32_t std_modinv_even[]  ={0x9D452491,0xA1095436,0x21DF1164,0x07FB47E6,0x3BB6F5BF,0x4131651F,0xE3F7594C,0x06A0C26B,0x782C7FCA,
                                  0xEBEFEECB,0x1BD48313,0xAF839D0C,0x695FE9AE,0x6A592129,0x00A51C1D,0xCFC338A8,0x00000068};
    uint32_t std_modexp[]       ={0xC4618F1C,0x673D9387,0xCEB131B8,0xF89D0EA1,0xE83E34C3,0xF771806D,0x81C5828B,0x8788D4A9,0x923F04E0,
                                  0x4C6F7428,0xA80DA2EC,0x46D64020,0x7297A97E,0x26E860AA,0xBB0F47FD,0xBD9EE19A,0x0000018F};
    uint32_t std_modexp_d[]     ={0x4FA83280,0x9A8FD33C,0xBBF58405,0xA009EF63,0x13A507C5,0x771202A6,0x14EDE7B1,0x579BEDFF,0xEEEF9BC2,
                                  0x5178FA7C,0x9579FBCE,0xBFD26928,0x87F83BE0,0xAEE959E6,0x6AD2A344,0x43F7D558,0x0000019E};

    vector->bitLen           = 522;
    vector->a                = a;
    vector->b                = b;
    vector->p                = p;
    vector->q                = q;
    vector->fai_n            = fai_n;
    vector->d                = d;
    vector->n                = n;
    vector->std_n_h          = n_h;
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    vector->std_n_n0         = n_n0;
    n_10_01_h[1]             = 0x00004000;
    vector->std_10_01_n_h    = n_10_01_h;
    vector->std_10_01_n_n0   = n_10_01_n0;
    n_11_11_h[1]             = 0x00001000;
    vector->std_11_11_n_h    = n_11_11_h;
    vector->std_11_11_n_n0   = n_11_11_n0;
#else
    n_10_01_h[(522+31)/32-2] = 0x00004000;
    vector->std_10_01_n_h    = n_10_01_h;
    n_11_11_h[(522+31)/32-2] = 0x00001000;
    vector->std_11_11_n_h    = n_11_11_h;
#endif
    vector->std_a_add_b      = std_a_add_b;
    vector->std_a_sub_b      = std_a_sub_b;
    vector->std_b_sub_a      = std_b_sub_a;
    vector->std_modadd       = std_modadd;
    vector->std_modsub_a_b   = std_a_sub_b;
    vector->std_modsub_b_a   = std_modsub_b_a;
    vector->std_modmul       = std_modmul;
    vector->std_modinv_odd   = std_modinv_odd;
    vector->std_modinv_even  = std_modinv_even;
    vector->std_modexp       = std_modexp;
    vector->std_modexp_d     = std_modexp_d;

    return pke_basic_test(vector);
}
#endif


#ifdef PKE_BASIC_768_TEST
uint32_t pke_basic_768_test(void)
{
    pke_basic_test_vector_t   vector[1];

    uint32_t a[]                ={0x8A1B725D,0x63C3D8CD,0x6CDC4C2E,0xA95C1E0C,0x2CCED164,0x56D50AA7,0x5DC7490D,0x0BD86EC8,
                                  0x349BB807,0x466EA28F,0x223A6468,0x81F26EE0,0xAC7A683C,0x900746E6,0x66719FBC,0xFF4D05F4,
                                  0x365C46BB,0xC077905E,0xCB11433A,0x4CBD2651,0xD8DB426C,0xB756AD94,0x27D2DD6C,0x58DC0773};
    uint32_t b[]                ={0x4DE60C3A,0xFD057B4D,0xDF9A89FD,0xA756428E,0x394BE4AE,0x10C67C92,0x063EDEB8,0x93744847,
                                  0xB431C701,0xBB819AEB,0x6A429566,0x023B076C,0x4B0539F9,0xD7B79BF7,0xB93717D0,0x666DF2E1,
                                  0xBA073756,0x4675BBC6,0xBA94B71D,0x427294DE,0xCAA26847,0x5587749A,0x23A244DC,0xABBB8D47};
    uint32_t std_a_add_b[]      ={0xD8017E97,0x60C9541A,0x4C76D62C,0x50B2609B,0x661AB613,0x679B8739,0x640627C5,0x9F4CB70F,
                                  0xE8CD7F08,0x01F03D7A,0x8C7CF9CF,0x842D764C,0xF77FA235,0x67BEE2DD,0x1FA8B78D,0x65BAF8D6,
                                  0xF0637E12,0x06ED4C24,0x85A5FA58,0x8F2FBB30,0xA37DAAB3,0x0CDE222F,0x4B752249,0x049794BA};
    uint32_t std_a_sub_b[]      ={0x3C356623,0x66BE5D80,0x8D41C230,0x0205DB7D,0xF382ECB6,0x460E8E14,0x57886A55,0x78642681,
                                  0x8069F105,0x8AED07A3,0xB7F7CF01,0x7FB76773,0x61752E43,0xB84FAAEF,0xAD3A87EB,0x98DF1312,
                                  0x7C550F65,0x7A01D497,0x107C8C1D,0x0A4A9173,0x0E38DA25,0x61CF38FA,0x04309890,0xAD207A2C};
    uint32_t std_b_sub_a[]      ={0xC3CA99DD,0x9941A27F,0x72BE3DCF,0xFDFA2482,0x0C7D1349,0xB9F171EB,0xA87795AA,0x879BD97E,
                                  0x7F960EFA,0x7512F85C,0x480830FE,0x8048988C,0x9E8AD1BC,0x47B05510,0x52C57814,0x6720ECED,
                                  0x83AAF09A,0x85FE2B68,0xEF8373E2,0xF5B56E8C,0xF1C725DA,0x9E30C705,0xFBCF676F,0x52DF85D3};
    uint32_t p[]                ={0x9CC9486B,0xBB54BBFC,0x3DB3E770,0x70A5CDDB,0x43073CD6,0x26D40674,0xEC2C3E76,0xD15B7308,
                                  0xFC8AA8D9,0xBA74E4FF,0x33EC2A60,0xDDE1E29A};
    uint32_t q[]                ={0x9E3F1857,0xA600449A,0x81DC1234,0x0EA8DC32,0x26318DB4,0x87390CFF,0xB2BF4C62,0xDB1DD2E8,
                                  0xC29EC741,0xE4C4B328,0xE0C23CFF,0xD2599D1C};
    uint32_t fai_n[]            ={0xC87E439C,0x90575D8D,0xC6239567,0x22D66487,0xA81BF315,0xAF84F6E7,0x88F44412,0x4038F0F3,
                                  0x9C5D31E3,0xA893DEDC,0x097AC44A,0xC16CD6AF,0x9982268B,0x90E46B4C,0xD532E0A5,0xAF318712,
                                  0x414AD9D7,0xD9E87ED3,0xDEF03B60,0xE9D849F0,0x9BBD2865,0xFA152F48,0xD5AF22E6,0xB650F796};
    uint32_t d[]                ={0xF8018E81,0xAA2AD215,0x880E8A15,0x6956DC35,0xD8FEDCDB,0x279E7B2A,0xAA70DAA1,0xC0EF5D5D,
                                  0x2316C02E,0x313ACC16,0x45C8495F,0x15072730,0xC6F8B995,0x4909A04A,0x624D8CD5,0xEEA0D92B,
                                  0xA74DD446,0xFF5F0487,0x1B3EF680,0xA68083FF,0x2E0092EA,0xE1460D41,0x9F997E63,0x7A0BF9B2};
    uint32_t n[]                ={0x0386A45D,0xF1AC5E25,0x85B38F0C,0xA2250E95,0x1154BD9F,0x5D920A5B,0x27DFCEEB,0xECB236E5,
                                  0x5B86A1FE,0x47CD7705,0x1E292BAB,0x71A85666,0x9982268D,0x90E46B4C,0xD532E0A5,0xAF318712,
                                  0x414AD9D7,0xD9E87ED3,0xDEF03B60,0xE9D849F0,0x9BBD2865,0xFA152F48,0xD5AF22E6,0xB650F796};

    uint32_t n_h[]              ={0x9DC4274E,0x081A56E1,0xDEB78D1A,0x330D2D86,0xD00BF328,0x22F18CE9,0x90FA29A4,0x1D503C23,
                                  0x0E5B2259,0x68E1F211,0xBA8CB825,0x7DF3854D,0x5DEA2042,0x25079C10,0x9433D519,0xA78EE4E8,
                                  0xE3E380AF,0x6E1B02FE,0x744F0194,0x61B7D22B,0x0F4D27BF,0xB0B2C381,0xAF3209D9,0x06F1372D};
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    uint32_t n_n0[]             ={0x15A3B00B,};
#endif
    uint32_t n_10_01_h[(768+31)/32] ={0};
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    uint32_t n_10_01_n0[]           ={0xFFFFFFFF};
#endif
    uint32_t n_11_11_h[(768+31)/32] ={0};
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    uint32_t n_11_11_n0[]           ={1};
#endif
    uint32_t std_modadd[]       ={0xD47ADA3A,0x6F1CF5F5,0xC6C3471F,0xAE8D5205,0x54C5F873,0x0A097CDE,0x3C2658DA,0xB29A802A,
                                  0x8D46DD09,0xBA22C675,0x6E53CE23,0x12851FE6,0x5DFD7BA8,0xD6DA7791,0x4A75D6E7,0xB68971C3,
                                  0xAF18A43A,0x2D04CD51,0xA6B5BEF7,0xA557713F,0x07C0824D,0x12C8F2E7,0x75C5FF62,0x4E469D23,};
    uint32_t std_modsub_a_b[]   ={0x3FBC0A80,0x586ABBA5,0x12F5513D,0xA42AEA13,0x04D7AA55,0xA3A09870,0x7F683940,0x65165D66,
                                  0xDBF09304,0xD2BA7EA8,0xD620FAAC,0xF15FBDD9,0xFAF754D0,0x4934163B,0x826D6891,0x48109A25,
                                  0xBD9FE93D,0x53EA536A,0xEF6CC77E,0xF422DB63,0xA9F6028A,0x5BE46842,0xD9DFBB77,0x637171C2};
    uint32_t std_modmul[]       ={0x69E8F94F,0xC2418C7B,0xCFED0902,0x505F9F7B,0x63410D7B,0xCF4DCA7A,0x72719ECF,0xC4F8A9E9,
                                  0xBB968D77,0x97D2E1ED,0x0FD845B2,0x9F95C781,0x0BDCB317,0x7BF3EE3C,0x38E175F3,0x14FA1C06,
                                  0xA23339D3,0xFFB9E236,0x0AB7D19E,0xCD14C4F1,0xB218C11A,0xCCE4127A,0x1A2D508C,0xA8252BBA};
    uint32_t std_modinv_odd[]   ={0xCFA7C20B,0x228A222A,0x146ED6F4,0x970ECBCC,0x3760F328,0xA57058DF,0x1C101A38,0xDA8A409B,
                                  0xDA3A71F5,0x355ECB4E,0xD446AF1B,0x459CAAC0,0xF8CEFC50,0x0C23D49F,0x35331260,0xD2656C57,
                                  0x7A149F38,0xE0F6C799,0xADD66694,0x8285619F,0x77F09B1A,0xF8ECE7DE,0x3431C641,0x014E7FED};
    uint32_t std_modinv_even[]  ={0xA5C82F59,0x2E296439,0xA2B44974,0xA1F4D635,0x8313169B,0xE7A2D06B,0xBF0F3939,0xE5C336FD,
                                  0x41C84712,0x5ECFEAB9,0x5B007E47,0xB38D1EF4,0xF978746E,0xD381F358,0x2EFAC389,0xAC2E2F15,
                                  0x39EAC244,0x9BD9C66C,0xE15546E9,0x5EDE9C72,0xB129861C,0x925E832C,0x141D3F45,0xA75A3B82};
    uint32_t std_modexp[]       ={0xCD9C9C28,0xC2BC0169,0x953E7451,0x9EEB70A8,0xDFA7D46F,0x60772ACA,0x35EDD559,0xC5B4C0B7,
                                  0x818B7987,0x5B6CF2EE,0x5F6C9FB1,0x6AA3EE07,0x119339E0,0xBFE68D54,0x704D3CAD,0xCB3363F1,
                                  0x3F4BCB8B,0x82A64F67,0x30D01036,0xE404FDDA,0xD4D5124A,0x3EACC406,0x991836ED,0x1E334DDD};
    uint32_t std_modexp_d[]     ={0x8186CB6A,0xEF379EA5,0x120D82C0,0x2DF3A115,0xCE31372B,0x09D18921,0x9B9617F0,0xABB71569,
                                  0xB9451942,0x7E3BEFA7,0xB5FC5263,0xDFFAE2AB,0x6929EADA,0x81EE98E6,0xADF36EC7,0x71EB4BF7,
                                  0x63BA68F6,0x9C9F19F9,0x706A03EC,0x45ADB0CE,0x1E16B78A,0xEEB8CE2E,0x1D2E57CC,0x8EB21F85};

    vector->bitLen          = 768;
    vector->a               = a;
    vector->b               = b;
    vector->p               = p;
    vector->q               = q;
    vector->fai_n           = fai_n;
    vector->d               = d;
    vector->n               = n;
    vector->std_n_h         = n_h;
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    vector->std_n_n0        = n_n0;
#endif
    n_10_01_h[0]            = 4;
    vector->std_10_01_n_h   = n_10_01_h;
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    vector->std_10_01_n_n0  = n_10_01_n0;
#endif
    n_11_11_h[0]            = 1;
    vector->std_11_11_n_h   = n_11_11_h;
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    vector->std_11_11_n_n0  = n_11_11_n0;
#endif
    vector->std_a_add_b     = std_a_add_b;
    vector->std_a_sub_b     = std_a_sub_b;
    vector->std_b_sub_a     = std_b_sub_a;
    vector->std_modadd      = std_modadd;
    vector->std_modsub_a_b  = std_modsub_a_b;
    vector->std_modsub_b_a  = std_b_sub_a;
    vector->std_modmul      = std_modmul;
    vector->std_modinv_odd  = std_modinv_odd;
    vector->std_modinv_even = std_modinv_even;
    vector->std_modexp      = std_modexp;
    vector->std_modexp_d    = std_modexp_d;

    return pke_basic_test(vector);
}
#endif


#ifdef PKE_BASIC_1024_TEST
uint32_t pke_basic_1024_test(void)
{
    pke_basic_test_vector_t   vector[1];

    uint32_t a[]                ={0xFF1E7E0F,0x3D718336,0x554A29E7,0x94BAC8BF,0x724E62BD,0x7ACA8929,0xE1FB1E17,0xF65CE081,
                                  0x1D4E2009,0x9F396817,0x4D8A761C,0x9074F282,0x75D1FB87,0x44403CF8,0xECCAA78A,0x99418465,
                                  0x0BC0DEC3,0x9D708D72,0x86A89F27,0x52C5F063,0x6C44BBE3,0x00428999,0xF4ACE77A,0x406D9C66,
                                  0x16F527D2,0x00042BBA,0x7E30D1D9,0xF55EBDB9,0x3D6739FD,0xE226EFAB,0x38BEC77D,0x300D52F7};
    uint32_t b[]                ={0x8D43E0B5,0xD187CDAA,0xE68A6B16,0x362C8F22,0x77A4EA5C,0xEBA1B3D8,0x67578056,0xFA7B99F8,
                                  0x95E3317F,0x27098D73,0x597EDDE5,0x44546814,0xDD204316,0xF0D3B38E,0x4AE68BC5,0x8622E463,
                                  0xAF6F56BC,0x57FC4F70,0x91CADE10,0xF5C68123,0x88D75545,0xF5669D7F,0x1547C8FE,0x1CE678BA,
                                  0x97A7A5C3,0x12C16788,0x73EDF1FB,0x74CF6B22,0xD98A6006,0xE2B3EEC7,0xE3B4DBD2,0x4F61091A};
    uint32_t std_a_add_b[]      ={0x8C625EC4,0x0EF950E1,0x3BD494FE,0xCAE757E2,0xE9F34D19,0x666C3D01,0x49529E6E,0xF0D87A7A,
                                  0xB3315189,0xC642F58A,0xA7095401,0xD4C95A96,0x52F23E9D,0x3513F087,0x37B13350,0x1F6468C9,
                                  0xBB303580,0xF56CDCE2,0x18737D37,0x488C7187,0xF51C1129,0xF5A92718,0x09F4B078,0x5D541521,
                                  0xAE9CCD95,0x12C59342,0xF21EC3D4,0x6A2E28DB,0x16F19A04,0xC4DADE73,0x1C73A350,0x7F6E5C12};
    uint32_t std_a_sub_b[]      ={0x71DA9D5A,0x6BE9B58C,0x6EBFBED0,0x5E8E399C,0xFAA97861,0x8F28D550,0x7AA39DC0,0xFBE14689,
                                  0x876AEE89,0x782FDAA3,0xF40B9837,0x4C208A6D,0x98B1B871,0x536C8969,0xA1E41BC4,0x131EA002,
                                  0x5C518807,0x45743E01,0xF4DDC117,0x5CFF6F3F,0xE36D669D,0x0ADBEC19,0xDF651E7B,0x238723AC,
                                  0x7F4D820F,0xED42C431,0x0A42DFDD,0x808F5297,0x63DCD9F7,0xFF7300E3,0x5509EBAA,0xE0AC49DC};
    uint32_t std_b_sub_a[]      ={0x8E2562A6,0x94164A73,0x9140412F,0xA171C663,0x0556879E,0x70D72AAF,0x855C623F,0x041EB976,
                                  0x78951176,0x87D0255C,0x0BF467C8,0xB3DF7592,0x674E478E,0xAC937696,0x5E1BE43B,0xECE15FFD,
                                  0xA3AE77F8,0xBA8BC1FE,0x0B223EE8,0xA30090C0,0x1C929962,0xF52413E6,0x209AE184,0xDC78DC53,
                                  0x80B27DF0,0x12BD3BCE,0xF5BD2022,0x7F70AD68,0x9C232608,0x008CFF1C,0xAAF61455,0x1F53B623};
    uint32_t p[]                ={0x0EC175C3,0x7FC0644E,0xC0BC71C8,0xB0C88143,0x486F7AA8,0xCCA73715,0xB1F88264,0x9C887D8B,
                                  0xB66CEEBC,0x48C61DDF,0x895D8284,0xF379C3A5,0x9EC0EFA9,0xA2B61164,0x69796EC4,0xEE3C50C4};
    uint32_t q[]                ={0x324ECD45,0xA7CA3384,0xBE36871E,0xD0EAB293,0x4ECD7116,0xBC93015D,0x4441BE92,0xC0CF454B,
                                  0xC42A8BA8,0xA9D5623C,0x4D073200,0xAC8F2201,0x12DD39B9,0x87E039C5,0x8EA29ADB,0xD45691DE};
    uint32_t fai_n[]            ={0x9BCBA188,0x6D61CC86,0xC44E8029,0x67972A95,0x33CBF41F,0x6AD943E7,0xD1429375,0x6A9A6067,
                                  0x7D101666,0xBE2B125E,0x2668C795,0xE417FAC3,0x36D0F006,0x3872C78D,0xB7C97936,0x2BEA3F7D,
                                  0xD72CD919,0xE58623C2,0xB8FA80C9,0x284E81A1,0x387C74F3,0x114117F5,0xFE17FB6E,0x03E220A4,
                                  0x5F352991,0x85336827,0xC32C2A7F,0x50D46F7D,0xEAA5832F,0xEF46B122,0x101E0FAB,0xC59A82E5};
    uint32_t d[]                ={0xBE4EFE29,0x8ACCF65C,0x7AF3A5FC,0x4E060830,0xB7A2BC03,0x814EE42F,0x32F09FC6,0x320B17D6,
                                  0xDB13745F,0xA20666CD,0x47C97D6B,0x091F02D7,0xD651EC5B,0xA292C2ED,0xDD6C8489,0x3F17B92B,
                                  0xA2618F4D,0x9D3688A2,0x63EC549A,0xAE86A789,0xF524D77D,0x2C7EC3E9,0xD13EDD3A,0x746E0594,
                                  0x1EB5972F,0x74898792,0xFE180A06,0x80779135,0x9E2AB078,0x92205C9E,0xDFC26F30,0x9728841A};
    uint32_t n[]                ={0xDCDBE48F,0x94EC6458,0x43417910,0xE94A5E6D,0xCB08DFDE,0xF4137C59,0xC77CD46C,0xC7F2233E,
                                  0xF7A790CB,0xB0C6927A,0xFCCD7C1A,0x8420E069,0xE86F196A,0x630912B6,0xAFE582D6,0xEE7D2220,
                                  0xD72CD91A,0xE58623C2,0xB8FA80C9,0x284E81A1,0x387C74F3,0x114117F5,0xFE17FB6E,0x03E220A4,
                                  0x5F352991,0x85336827,0xC32C2A7F,0x50D46F7D,0xEAA5832F,0xEF46B122,0x101E0FAB,0xC59A82E5};
    uint32_t n_h[]              ={0x2B74CB09,0x72AC4975,0xDFC39585,0xCECBEFC5,0x6F89E185,0x0C7B5E54,0x8B3E72CB,0x074DE54F,
                                  0x0410638C,0xEE3683D2,0x768AB55C,0xEFDF2972,0xDEB607A3,0xB4B6ED67,0x47585E1B,0xF4672354,
                                  0xCAFD8BDC,0x424157D9,0x5D722481,0x4AB5BA36,0x85D8FDBB,0xCAEE9C8E,0x6D65455A,0xBDA62128,
                                  0x0A1F6F11,0x2AA9A2E2,0xAE520CF5,0x471428DC,0x5DA96988,0x0E8FCDBC,0x5CE3774C,0x442A6193};
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    uint32_t n_n0[]             ={0x46174591,};
#endif
    uint32_t n_10_01_h[1024/32] ={0};
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    uint32_t n_10_01_n0[]       ={0xFFFFFFFF};
#endif
    uint32_t n_11_11_h[1024/32] ={0};
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    uint32_t n_11_11_n0[]       ={1};
#endif
    uint32_t std_modsub_a_b[]   ={0x4EB681E9,0x00D619E5,0xB20137E1,0x47D89809,0xC5B25840,0x833C51AA,0x4220722D,0xC3D369C8,
                                  0x7F127F55,0x28F66D1E,0xF0D91452,0xD0416AD7,0x8120D1DB,0xB6759C20,0x51C99E9A,0x019BC223,
                                  0x337E6122,0x2AFA61C4,0xADD841E1,0x854DF0E1,0x1BE9DB90,0x1C1D040F,0xDD7D19E9,0x27694451,
                                  0xDE82ABA0,0x72762C58,0xCD6F0A5D,0xD163C214,0x4E825D26,0xEEB9B206,0x6527FB56,0xA646CCC1,};
    uint32_t std_modmul[]       ={0xEFA7135E,0x9D0D9CF1,0x67B827BA,0xFF06A64F,0xF7D5D8A2,0x6BEA444F,0xF2E40AB8,0x52606302,
                                  0x5A27B08F,0x14D361AA,0x14962476,0x9C213176,0xA7729B32,0x51DB205C,0x0DADC205,0x380253CD,
                                  0x113535AA,0xBB4F2EBC,0x4B8DE9C7,0xEBE044F4,0xA3237416,0x04460C7C,0x5D005879,0xF7D9F169,
                                  0xE9CE1C24,0xF6E3BDBE,0x5ABFB616,0xB15D7155,0xC171FB86,0x57CF5AAE,0xBFB42119,0x28CEADD1};
    uint32_t std_modinv_odd[]   ={0xC61D2584,0x19D330EC,0x8D4B5AAD,0xA69948C1,0xC09ED63B,0x8668B13C,0xDB68439F,0x5E46FF33,
                                  0x408E99A6,0x1EEDA9D1,0xBDD7DCC6,0x81736402,0xB71C4134,0xBB8CE48A,0x70B90EEE,0x53B8A56B,
                                  0xC3044F3F,0xCA253619,0x6665E432,0xADDAEA4F,0x200C4052,0xAA3B291C,0x2CF60E1C,0xA5175445,
                                  0xEC50F088,0xEC7D032E,0x3C46D75E,0x40D995D3,0x3E80E7FC,0x2C8507A0,0xDE3D45BE,0x4F1D3894};
    uint32_t std_modinv_even[]  ={0xD6AFD1AF,0x0CC73631,0xEE94F9C2,0x14C43A29,0x0632A52E,0xA72FAD35,0x1D7105B0,0xD183CCA2,
                                  0xFE41978C,0xB982EF94,0x4633D9D6,0x0AFA7362,0x40F4A8C8,0x90E38117,0x6840A50D,0x90A04C00,
                                  0x3D290A7C,0xFE73A16E,0x2FDDBF62,0xA6B29E04,0x266F42B2,0xFE851C93,0x3E77B414,0x605CB514,
                                  0x6D104CD2,0xF4F8021C,0xEF289938,0xBB0D12CC,0x4DED2B70,0x4BA50F75,0xDD1F597A,0xAFE757F9};
    uint32_t std_modexp[]       ={0x4FA82AEC,0x4E33C97D,0xBCD72F17,0x2A381F1B,0xA3B22981,0xE694A1F5,0x8EF7139E,0xD1F0E05E,
                                  0xDCEBBB5D,0x67C58385,0x417DBF68,0xA44D540B,0x5C13B0F5,0xE1C561AD,0xA0FF20AC,0xF05796D4,
                                  0xAF22753E,0x436D1BCE,0x589023F2,0x6AF20F65,0x2ECB05C2,0xD9340CAE,0xA3ED9602,0xBF1CC6B6,
                                  0xE091CA20,0x421E5D7D,0x3D1F573A,0xB872930A,0xEC07080C,0x37318DB2,0x3BC4E8C1,0x242E00FF};
    uint32_t std_modexp_d[]     ={0x14F792FC,0x4334865A,0x62674BFA,0x6CB56C9A,0x9DBCAEDE,0x6FDFDAD0,0x31001B31,0xEF7E5C60,
                                  0x329462B3,0x8ACC404B,0x175BA817,0xFA226CD5,0xDC395E65,0xD1D18F18,0xF8992E8A,0x9473E97D,
                                  0xA8E450CF,0xEF604E56,0xDC5AEC19,0x8AA1B56F,0x3FC41A90,0x598DB391,0x62B9029C,0x9A5076BF,
                                  0x029EBEF5,0xDBE09665,0x0BDD29E7,0xAD32FCFC,0x2225D542,0xB2BE4444,0x018C49D0,0x9DC33334};

    vector->bitLen          = 1024;
    vector->a               = a;
    vector->b               = b;
    vector->p               = p;
    vector->q               = q;
    vector->fai_n           = fai_n;
    vector->d               = d;
    vector->n               = n;
    vector->std_n_h         = n_h;
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    vector->std_n_n0        = n_n0;
#endif
    n_10_01_h[0]            = 4;
    vector->std_10_01_n_h   = n_10_01_h;
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    vector->std_10_01_n_n0  = n_10_01_n0;
#endif
    n_11_11_h[0]            = 1;
    vector->std_11_11_n_h   = n_11_11_h;
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    vector->std_11_11_n_n0  = n_11_11_n0;
#endif
    vector->std_a_add_b     = std_a_add_b;
    vector->std_a_sub_b     = std_a_sub_b;
    vector->std_b_sub_a     = std_b_sub_a;
    vector->std_modadd      = std_a_add_b;
    vector->std_modsub_a_b  = std_modsub_a_b;
    vector->std_modsub_b_a  = std_b_sub_a;
    vector->std_modmul      = std_modmul;
    vector->std_modinv_odd  = std_modinv_odd;
    vector->std_modinv_even = std_modinv_even;
    vector->std_modexp      = std_modexp;
    vector->std_modexp_d    = std_modexp_d;

    return pke_basic_test(vector);
}
#endif


#ifdef PKE_BASIC_1502_TEST
uint32_t pke_basic_1502_test(void)
{
    pke_basic_test_vector_t   vector[1];

    uint32_t a[]                ={0xEEE4D24F,0x3D684638,0x92C4CE7A,0x7B999457,0x4D4A529D,0x517664D6,0x97F0C23E,0xD7F60117,
                                  0xF89C42D3,0xD495EBC8,0xDC973FF8,0xDEB665F5,0x2BC9F1F1,0xF7F5BFC1,0x61075FE3,0xF6225550,
                                  0x437FA794,0x2B72D51F,0x7049D295,0x7678BFF7,0x28D73865,0xE0C896E7,0xC8D184BF,0x3895AACA,
                                  0x4F1AF6F0,0x85904E0E,0x63626EAC,0x68A60635,0x16B5B704,0x432C6C05,0xFA010E9D,0xC9C973CC,
                                  0x78E39375,0x6A37E9B7,0x4677CEFB,0x311BF294,0x0DDABF50,0x43D8FBAA,0xDFF0A734,0x5A3EF754,
                                  0x89B49C80,0x21AD20DB,0x7D6D829E,0xDA8D691D,0x96CED300,0xCC863A07,0x2EFD4B8C};
    uint32_t b[]                ={0xEA6BD726,0xE3E1FD5D,0xFB679819,0x141C5CFC,0x07502A61,0x030212FB,0x6911ED25,0x8D9A596B,
                                  0x78FE0C36,0xDFC4A8CA,0x22937386,0xA6E47C4D,0xC75FEBC2,0xAC71FC53,0x37B3B9CE,0x344D8742,
                                  0xFA675758,0x5C18420D,0xF4BB0A94,0xC374FFC9,0xA665A503,0x2C8FAF6C,0xA595D424,0x0C3EC42A,
                                  0x0054B342,0x86C820E7,0xB3F25293,0x7BED733B,0x8C04A426,0xEE533CE0,0xC42F859D,0xD2B3F826,
                                  0xB6105D8D,0x6076413A,0xA9EEAECE,0xDC2F99AA,0x194D1487,0x1EBE4415,0x4376E68B,0x2B181C9C,
                                  0x96222A12,0xF7558337,0x56D81DFA,0x2CFB6970,0x4221BBCC,0x1DC77662,0x0379DE80};
    uint32_t std_a_add_b[]      ={0xD950A975,0x214A4396,0x8E2C6694,0x8FB5F154,0x549A7CFE,0x547877D1,0x0102AF63,0x65905A83,
                                  0x719A4F0A,0xB45A9493,0xFF2AB37F,0x859AE242,0xF329DDB4,0xA467BC14,0x98BB19B2,0x2A6FDC92,
                                  0x3DE6FEED,0x878B172D,0x6504DD29,0x39EDBFC1,0xCF3CDD69,0x0D584653,0x6E6758E4,0x44D46EF5,
                                  0x4F6FAA32,0x0C586EF5,0x1754C140,0xE4937971,0xA2BA5B2A,0x317FA8E5,0xBE30943B,0x9C7D6BF3,
                                  0x2EF3F103,0xCAAE2AF2,0xF0667DC9,0x0D4B8C3E,0x2727D3D8,0x62973FBF,0x23678DBF,0x855713F1,
                                  0x1FD6C692,0x1902A413,0xD445A099,0x0788D28D,0xD8F08ECD,0xEA4DB069,0x32772A0C};
    uint32_t std_a_sub_b[]      ={0x0478FB29,0x598648DB,0x975D3660,0x677D375A,0x45FA283C,0x4E7451DB,0x2EDED519,0x4A5BA7AC,
                                  0x7F9E369D,0xF4D142FE,0xBA03CC71,0x37D1E9A8,0x646A062F,0x4B83C36D,0x2953A615,0xC1D4CE0E,
                                  0x4918503C,0xCF5A9311,0x7B8EC800,0xB303C02D,0x82719361,0xB438E77A,0x233BB09B,0x2C56E6A0,
                                  0x4EC643AE,0xFEC82D27,0xAF701C18,0xECB892F9,0x8AB112DD,0x54D92F24,0x35D188FF,0xF7157BA6,
                                  0xC2D335E7,0x09C1A87C,0x9C89202D,0x54EC58E9,0xF48DAAC8,0x251AB794,0x9C79C0A9,0x2F26DAB8,
                                  0xF392726E,0x2A579DA3,0x269564A3,0xAD91FFAD,0x54AD1734,0xAEBEC3A5,0x2B836D0C};
    uint32_t std_b_sub_a[]      ={0xFB8704D7,0xA679B724,0x68A2C99F,0x9882C8A5,0xBA05D7C3,0xB18BAE24,0xD1212AE6,0xB5A45853,
                                  0x8061C962,0x0B2EBD01,0x45FC338E,0xC82E1657,0x9B95F9D0,0xB47C3C92,0xD6AC59EA,0x3E2B31F1,
                                  0xB6E7AFC3,0x30A56CEE,0x847137FF,0x4CFC3FD2,0x7D8E6C9E,0x4BC71885,0xDCC44F64,0xD3A9195F,
                                  0xB139BC51,0x0137D2D8,0x508FE3E7,0x13476D06,0x754EED22,0xAB26D0DB,0xCA2E7700,0x08EA8459,
                                  0x3D2CCA18,0xF63E5783,0x6376DFD2,0xAB13A716,0x0B725537,0xDAE5486B,0x63863F56,0xD0D92547,
                                  0x0C6D8D91,0xD5A8625C,0xD96A9B5C,0x526E0052,0xAB52E8CB,0x51413C5A,0xD47C92F3};
    uint32_t p[]                ={0x9B551497,0x7C335376,0xE35D2302,0x053A6F17,0xBB2E134F,0x5C36BD19,0x4D05C92D,0x0A79956F,
                                  0xD76AFD62,0x5F7EF43C,0xE5677A02,0xD33E6356,0xCF10A739,0x9E5E2D22,0x7547587A,0x9438C067,
                                  0x374ED531,0x44006259,0x39082A78,0x52771864,0xD702B163,0x1372D111,0x6524DB6C,0x00007809};
    uint32_t q[]                ={0x432FA263,0x7D0E2877,0x62196D1C,0x9F5E100B,0x986B354B,0x987B5D00,0xB5242A5E,0xF3789844,
                                  0x3E4C6E51,0x5348124C,0xBA3D197E,0x5745171E,0x3F589D3B,0xCD13706A,0x3B7E5A10,0x1A5C1E04,
                                  0x85117E6D,0xE731535D,0xA5572132,0x0E31B360,0x6172A071,0xC61CCB43,0x8B583839,0x0000702E};
    uint32_t fai_n[]            ={0x5722CD6C,0x41DCCC6F,0xEEB8B254,0xB452E826,0xE0CA9012,0xDB9D7FF6,0x1FC36DC1,0xFB62B6FC,
                                  0x59984C78,0x85E8A1CF,0x93CFB731,0x865BC029,0xA4687FB8,0x9FE3B0DF,0x215EA92B,0x1D221469,
                                  0x55C1DF9B,0x6606C2C2,0x213BFD48,0xE8118A6F,0x320D5D88,0xD2920264,0x197A8F49,0xDFE59D5A,
                                  0x1084DFDF,0xA2F7118B,0x66CABFFF,0x3846E919,0xCACD8081,0xEF19B0E9,0x8C73CB66,0x4EE4EFDF,
                                  0xE329BFAF,0x0E13B65C,0x5B8A1474,0x349B7030,0xB456C09C,0x5126E8F9,0x511CA4AF,0xFB2745F6,
                                  0x19E6C78C,0x640A38BB,0x40D1C1FD,0x688219B6,0x2F9482AC,0xC421A2D9,0x3499EF46};
    uint32_t d[]                ={0xE4712F21,0x50BE916E,0x9ED52037,0x7DC12F47,0xA4D947E6,0x4C494C17,0x0322AD33,0xC9E81824,
                                  0x4240E7A3,0x1C901D72,0xC866E6AF,0xAB7628C1,0x0DBC5CD7,0x114A708D,0x6E6F279B,0x332EB259,
                                  0xC274F85E,0x3C6217FC,0x73228294,0x611940AA,0xABDCFF78,0x88B19D4B,0x5079E082,0xC8FA5E01,
                                  0x9D307141,0x2CA85CA1,0x58ACB425,0x4356C3E3,0x47AF1160,0xA7809DEA,0xCF8024BC,0xC60ECEC0,
                                  0x5CD12B0D,0x133358FB,0x5DE6B765,0xA61B594B,0x3BFDBB28,0x249C3F93,0xED970542,0x1031E32B,
                                  0x5CD41E26,0x2CD2A0A3,0xB333A331,0xDE368218,0xD2BCA1F4,0xF623B14F,0x11A36CB0};
    uint32_t n[]                ={0x35A78465,0x3B1E485D,0x342F4273,0x58EB674A,0x3463D8AD,0xD04F9A11,0x21ED614D,0xF954E4B0,
                                  0x6F4FB82C,0x38AFA858,0x33744AB2,0xB0DF3A9F,0xB2D1C42D,0x0B554E6C,0xD2245BB7,0xCBB6F2D4,
                                  0x12223339,0x91387879,0xFF9B48F3,0x48BA5633,0x6A82AF5D,0xAC219EB9,0x09F7A2EF,0xDFE68592,
                                  0x1084DFDF,0xA2F7118B,0x66CABFFF,0x3846E919,0xCACD8081,0xEF19B0E9,0x8C73CB66,0x4EE4EFDF,
                                  0xE329BFAF,0x0E13B65C,0x5B8A1474,0x349B7030,0xB456C09C,0x5126E8F9,0x511CA4AF,0xFB2745F6,
                                  0x19E6C78C,0x640A38BB,0x40D1C1FD,0x688219B6,0x2F9482AC,0xC421A2D9,0x3499EF46};
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    uint32_t n_h[]              ={0xF07200B9,0xF7E2D9FB,0x08E34640,0xC62171BC,0x4852483A,0x97A17DEE,0xB675B907,0xE2980452,
                                  0xF39B1345,0x5DB653FA,0x002BD15A,0x529933C8,0xC6F837D1,0x8DAFD067,0x0BC2F77B,0x5DCFBA35,
                                  0x0C4F15C4,0x986DBADE,0xE3AA279F,0x242DE463,0xE0138B5C,0x1D77C61E,0x7A41A300,0x1DCB7C80,
                                  0x2DF5956B,0xEA5CFF9A,0xD90BD8AF,0xCFC6D275,0xD78D1297,0xAE6B4151,0xA4AE09A2,0x17944C79,
                                  0xBEC578D1,0x29DA2842,0x88E9F687,0xA458538C,0x3EB033A8,0x7D4C6E3C,0x77D8C8FB,0x5A3AA985,
                                  0x8D3999C8,0x00783CF5,0x7D579F33,0xC86F1088,0x05722DAE,0xAC854859,0x1CD0E84F};
    uint32_t n_n0[]             ={0xB9D27293};
#else
    uint32_t n_h[]              ={0x949AF8F9,0x3F669785,0x640485CC,0x20D47B3D,0x4E754654,0xD9C7DCD8,0xE845ACAB,0x7784A700,
                                  0x413372D5,0xC0129E78,0x4F31B060,0x6D05D42C,0xA13CFED0,0xCB37F7FA,0x1606F686,0x7F5B2F2C,
                                  0x2E6EF661,0x4CD4B960,0x40548FAB,0x184484C0,0x412D2843,0xB81073DE,0x45D8B782,0x132C82F7,
                                  0xCE587542,0x74FACB04,0xC796B101,0x4CB02E0D,0x13C49313,0x3C76D9A2,0xA72ECBB3,0x92E566CE,
                                  0x087BCA52,0xB25246DA,0xEEDB2F2C,0x0711C400,0x1FC21F8F,0xBFCA132A,0x55C6B852,0x350653E1,
                                  0xA4BE3DD0,0xB1C254C0,0x67FC6CD8,0x1C18E6F6,0x80A8C653,0x8216673E,0x212C9382};
#endif

    uint32_t n_10_01_h[(1502+31)/32] ={0};
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    uint32_t n_10_01_n0[]            ={0xFFFFFFFF};
#endif
    uint32_t n_11_11_h[(1502+31)/32] ={0};
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    uint32_t n_11_11_n0[]            ={1};
#endif
    uint32_t std_modsub_b_a[]   ={0x312E893C,0xE197FF82,0x9CD20C12,0xF16E2FEF,0xEE69B070,0x81DB4835,0xF30E8C34,0xAEF93D03,
                                  0xEFB1818F,0x43DE6559,0x79707E40,0x790D50F6,0x4E67BDFE,0xBFD18AFF,0xA8D0B5A1,0x09E224C6,
                                  0xC909E2FD,0xC1DDE567,0x840C80F2,0x95B69606,0xE8111BFB,0xF7E8B73E,0xE6BBF253,0xB38F9EF1,
                                  0xC1BE9C31,0xA42EE463,0xB75AA3E6,0x4B8E561F,0x401C6DA3,0x9A4081C5,0x56A24267,0x57CF7439,
                                  0x205689C7,0x04520DE0,0xBF00F447,0xDFAF1746,0xBFC915D3,0x2C0C3164,0xB4A2E406,0xCC006B3D,
                                  0x2654551E,0x39B29B17,0x1A3C5D5A,0xBAF01A09,0xDAE76B77,0x1562DF33,0x0916823A,};
    uint32_t std_modmul[]       ={0xFDF30DFF,0xEBA92FA5,0xD548104D,0xB801E1E8,0xDBC0A07F,0xDA604CF0,0x0BF2A093,0x37826F5F,
                                  0x0433EFBD,0x1415A55C,0x55E31A4E,0x691CC1ED,0x68C1EFED,0xE4C72C2D,0x19D11F97,0x2CDD4388,
                                  0x9C3647AB,0x50736894,0xE80C3613,0xDD08AB47,0xA73111A1,0xE50B3AED,0x1780BED9,0x4F97D347,
                                  0xF7C36EAC,0xD61689B9,0x9A207DE3,0x7F16F0EF,0x770FDAEA,0x29C0C682,0x07491A89,0x68F389D3,
                                  0x9BBFADAC,0x0DEF2E2D,0x536F5035,0xB4703211,0xE13CF952,0xB437FDD9,0xB79F785B,0xA44565DA,
                                  0x00011E7D,0x2E0A2DFF,0x6EBEDF84,0xD89710E4,0xDD38A92A,0x5B98643C,0x0B74008E};
    uint32_t std_modinv_odd[]   ={0x2B3EAAD9,0x5FB8C0E8,0xF41A9493,0x848A4817,0x2AB14DBF,0x2E06D931,0xDEA9418C,0xA79F025B,
                                  0x3385AC95,0x1AC3EB61,0xCD128B15,0x7620C1BF,0x81CA1E5A,0xC81402C9,0xBEBA444B,0x118121E0,
                                  0x5B83935D,0x4D8D3B1A,0x2C266680,0xA0E2460C,0xB024B513,0x65207353,0xAC692D76,0xC766D82F,
                                  0x1A59C5A9,0xF02AD222,0x02AAA4BA,0xB392D04E,0x75FACC42,0xC7C8F2FC,0x7D46F8F4,0xD6D7D9C8,
                                  0x55BC002D,0x66C7F616,0xBADD301C,0x50D151C4,0x4298E98E,0x2147B3F3,0x0CC3A582,0x4FDD4C86,
                                  0x60288553,0xEE0B0B27,0xF9B57213,0xB6D0D562,0x2111C500,0x6298DE47,0x0D1871EE};
    uint32_t std_modinv_even[]  ={0x90ECAC2F,0x7622BA1A,0x4D43A9A6,0xC7FA0C40,0x09BF8139,0xB601BEB4,0x4270EB2F,0x8F66E2F1,
                                  0x8316D4FC,0xC30C4576,0x9558F99D,0xCC727BCD,0xCCD3566C,0xB0602269,0xBFB6CDA2,0x6FE66F99,
                                  0xC8005FDD,0x2B9334E2,0x993D2ADE,0xA8A6D7A9,0x25E75F89,0x630D96E7,0x66CF0E1D,0xDD82C47A,
                                  0xF45A05A3,0x9E1B015E,0xBA470D37,0x69418559,0xE669DBAB,0x9139653B,0x7922F905,0x28BE18A2,
                                  0xF56581C7,0xD2BFF301,0x2650BDFA,0xBE33256A,0x79F454F0,0x49F912A2,0xC5F290C5,0xF26F291D,
                                  0xDEE7B8C0,0xC9F5C825,0x0C48F994,0xA7A46AC2,0xFE755467,0xFD2A971F,0xD419AB};
    uint32_t std_modexp[]       ={0xF40E6623,0x5E684099,0x994BAD36,0xD0031B7E,0x80A8F6EF,0xF91AAF0E,0xD25A5956,0xBE575F71,
                                  0x53A50F08,0xC2EB4C8A,0x1115556C,0xE523A0BD,0xDF8267F0,0xFAE3B967,0x3DA54ED6,0x2B0A867D,
                                  0x71EC770B,0x54F71FBC,0x9303926F,0x2732D13C,0xF1ADDC8D,0x690818CD,0x2AD9E3A0,0x08688C30,
                                  0x8642F34B,0x6F5F11BF,0x8411E063,0xF420956E,0x562FC431,0xB4FF839A,0x1BBCB116,0x80596119,
                                  0x5CD23BA1,0x087208F9,0xA8B12E18,0x90A8CAAF,0x3947FF00,0x7EBCD985,0x8AC4817A,0x67133746,
                                  0x3A897ECC,0x0A7E9B94,0xE1FCBA66,0xD2AC6326,0x0DE99F8C,0x7C118F1E,0x163D88E5};
    uint32_t std_modexp_d[]     ={0xBF5417D1,0x63017DE4,0x68D280E8,0x324BFF49,0x335A781C,0xF3B33385,0x88879FFA,0x0C82CC47,
                                  0xCF2E2CBC,0x658554B1,0xCF1AC458,0x95DE1C69,0xD4DE1254,0x4E5E5DA6,0x2173E92A,0x17CF1B66,
                                  0xC90B56C2,0xF15A821E,0x98A5DE77,0x7250F8B3,0x0DE6A8A9,0x80853F9F,0x56F41A84,0x22A5DFCF,
                                  0x5B22D41E,0xBF9F7F18,0xC007420E,0x84186DA4,0x23CD11D9,0x4632E961,0xD0AD8C64,0x4A69AB50,
                                  0x03825CAD,0x089D43F4,0x5BF77AE8,0xF42D874F,0x2D9948C4,0x3AAB428D,0xC59130C8,0xEB9602A2,
                                  0xEEDBDC70,0x91EA598C,0xABA9C68D,0x1C2A7329,0xADAEF50A,0xD5751A43,0x2166492B};

    vector->bitLen          = 1502;
    vector->a               = a;
    vector->b               = b;
    vector->p               = p;
    vector->q               = q;
    vector->fai_n           = fai_n;
    vector->d               = d;
    vector->n               = n;
    vector->std_n_h         = n_h;
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    vector->std_n_n0        = n_n0;
    n_10_01_h[0]            = 0x40;
    vector->std_10_01_n_h   = n_10_01_h;
    vector->std_10_01_n_n0  = n_10_01_n0;
    n_11_11_h[0]            = 0x10;
    vector->std_11_11_n_h   = n_11_11_h;
    vector->std_11_11_n_n0  = n_11_11_n0;
#else
    n_10_01_h[2]            = 0x40;
    vector->std_10_01_n_h   = n_10_01_h;
    n_11_11_h[2]            = 0x10;
    vector->std_11_11_n_h   = n_11_11_h;
#endif
    vector->std_a_add_b     = std_a_add_b;
    vector->std_a_sub_b     = std_a_sub_b;
    vector->std_b_sub_a     = std_b_sub_a;
    vector->std_modadd      = std_a_add_b;
    vector->std_modsub_a_b  = std_a_sub_b;
    vector->std_modsub_b_a  = std_modsub_b_a;
    vector->std_modmul      = std_modmul;
    vector->std_modinv_odd  = std_modinv_odd;
    vector->std_modinv_even = std_modinv_even;
    vector->std_modexp      = std_modexp;
    vector->std_modexp_d    = std_modexp_d;

    return pke_basic_test(vector);
}
#endif


#ifdef PKE_BASIC_2048_TEST
uint32_t pke_basic_2048_test(void)
{
    pke_basic_test_vector_t   vector[1];

    uint32_t a[]                ={0x9EB3779B,0x057A6629,0x0D6644A2,0xDBF51104,0xEC0CF11F,0x5F692160,0x432195AE,0xD9C8253D,
                                  0xDB355DDF,0xF51EA63E,0xAFF5DEF6,0xD8BD3749,0x116A0714,0x3707F119,0xCD143490,0xC28024BF,
                                  0x8E74B4C5,0x15BEEEE0,0x1ADFE30E,0x63DAE2CA,0x5C801361,0xAF871F5E,0xE926385B,0x010C0D17,
                                  0x15D4E390,0x1D00E2E2,0x0E6177CB,0x2A211B24,0xD0E3C175,0x415D323E,0x916E696B,0x29984444,
                                  0xBFB1DB91,0x3F24C5A4,0xC2A77D9F,0x32377622,0x36532872,0x32859A5C,0x39CF13A0,0x03F7C175,
                                  0x60E46C9C,0x0EBC2749,0x7269845E,0x90CFB243,0x8735EE45,0xC51311AD,0x57BA8D7D,0x551C4DCA,
                                  0xA97157AE,0x4085BB83,0x215190A0,0x41AB2AC3,0xB7A112C7,0x81AF95EA,0xE7288C32,0x115AB4B8,
                                  0x205B4D85,0x3482EFB1,0x0520D07F,0x2C3656A8,0x10D7A81C,0x1C75D741,0x701FF4BA,0xC60FA51B};
    uint32_t b[]                ={0x4061F561,0x4AFF9042,0xB764D6D0,0x91B66BC9,0xF51800ED,0x69699715,0x299C3E11,0x708C537B,
                                  0xCD53C006,0xCE3AB6C9,0x1285E152,0x1D08958C,0x9211AB57,0x85CC702D,0x34BE61EA,0x5899D060,
                                  0x1DB9F0ED,0x4D67EA79,0x3222BCE1,0xA815FE5F,0x298B1013,0x432A606C,0x31C1BE7D,0x57A6EE9E,
                                  0x8DE37E80,0x519861E5,0x393D9939,0xE5BB9479,0x4F4F982B,0x19260DF4,0x2472B44C,0x816A4A99,
                                  0xF8DD305B,0xED77C35B,0xDA014972,0x8C7458D6,0xF2DF11C3,0x15FF25F1,0xC051985C,0x7FAAD9C1,
                                  0x553F4C24,0xB73B78B9,0x1C709BC5,0x27270BA1,0xD4657181,0xB9F5C161,0xE887952B,0x298E49F9,
                                  0x42DEECF5,0xF1FD9209,0xBCBA82D8,0x32631536,0x3EB70F7F,0xB438C0A3,0x7DBAECA3,0x6B18FE1E,
                                  0x3FEFCFD8,0x328097E0,0x91D5BD39,0x176D9BB5,0x9B8E69C3,0x8239493B,0xC6EB3500,0x77FC0653};
    uint32_t std_a_add_b[]      ={0xDF156CFC,0x5079F66B,0xC4CB1B72,0x6DAB7CCD,0xE124F20D,0xC8D2B876,0x6CBDD3BF,0x4A5478B8,
                                  0xA8891DE6,0xC3595D08,0xC27BC049,0xF5C5CCD5,0xA37BB26B,0xBCD46146,0x01D2967A,0x1B19F520,
                                  0xAC2EA5B3,0x6326D959,0x4D029FEF,0x0BF0E129,0x860B2375,0xF2B17FCA,0x1AE7F6D8,0x58B2FBB6,
                                  0xA3B86210,0x6E9944C7,0x479F1104,0x0FDCAF9D,0x203359A1,0x5A834033,0xB5E11DB7,0xAB028EDD,
                                  0xB88F0BEC,0x2C9C8900,0x9CA8C712,0xBEABCEF9,0x29323A35,0x4884C04E,0xFA20ABFC,0x83A29B36,
                                  0xB623B8C0,0xC5F7A002,0x8EDA2023,0xB7F6BDE4,0x5B9B5FC6,0x7F08D30F,0x404222A9,0x7EAA97C4,
                                  0xEC5044A3,0x32834D8C,0xDE0C1379,0x740E3FF9,0xF6582246,0x35E8568D,0x64E378D6,0x7C73B2D7,
                                  0x604B1D5D,0x67038791,0x96F68DB8,0x43A3F25D,0xAC6611DF,0x9EAF207C,0x370B29BA,0x3E0BAB6F};
    uint32_t std_a_sub_b[]      ={0x5E51823A,0xBA7AD5E7,0x56016DD1,0x4A3EA53A,0xF6F4F032,0xF5FF8A4A,0x1985579C,0x693BD1C2,
                                  0x0DE19DD9,0x26E3EF75,0x9D6FFDA4,0xBBB4A1BD,0x7F585BBD,0xB13B80EB,0x9855D2A5,0x69E6545F,
                                  0x70BAC3D8,0xC8570467,0xE8BD262C,0xBBC4E46A,0x32F5034D,0x6C5CBEF2,0xB76479DE,0xA9651E79,
                                  0x87F1650F,0xCB6880FC,0xD523DE91,0x446586AA,0x81942949,0x2837244A,0x6CFBB51F,0xA82DF9AB,
                                  0xC6D4AB35,0x51AD0248,0xE8A6342C,0xA5C31D4B,0x437416AE,0x1C86746A,0x797D7B44,0x844CE7B3,
                                  0x0BA52077,0x5780AE90,0x55F8E898,0x69A8A6A2,0xB2D07CC4,0x0B1D504B,0x6F32F852,0x2B8E03D0,
                                  0x66926AB9,0x4E88297A,0x64970DC7,0x0F48158C,0x78EA0348,0xCD76D547,0x696D9F8E,0xA641B69A,
                                  0xE06B7DAC,0x020257D0,0x734B1346,0x14C8BAF2,0x75493E59,0x9A3C8E05,0xA934BFB9,0x4E139EC7};
    uint32_t std_b_sub_a[]      ={0xA1AE7DC6,0x45852A18,0xA9FE922E,0xB5C15AC5,0x090B0FCD,0x0A0075B5,0xE67AA863,0x96C42E3D,
                                  0xF21E6226,0xD91C108A,0x6290025B,0x444B5E42,0x80A7A442,0x4EC47F14,0x67AA2D5A,0x9619ABA0,
                                  0x8F453C27,0x37A8FB98,0x1742D9D3,0x443B1B95,0xCD0AFCB2,0x93A3410D,0x489B8621,0x569AE186,
                                  0x780E9AF0,0x34977F03,0x2ADC216E,0xBB9A7955,0x7E6BD6B6,0xD7C8DBB5,0x93044AE0,0x57D20654,
                                  0x392B54CA,0xAE52FDB7,0x1759CBD3,0x5A3CE2B4,0xBC8BE951,0xE3798B95,0x868284BB,0x7BB3184C,
                                  0xF45ADF88,0xA87F516F,0xAA071767,0x9657595D,0x4D2F833B,0xF4E2AFB4,0x90CD07AD,0xD471FC2F,
                                  0x996D9546,0xB177D685,0x9B68F238,0xF0B7EA73,0x8715FCB7,0x32892AB8,0x96926071,0x59BE4965,
                                  0x1F948253,0xFDFDA82F,0x8CB4ECB9,0xEB37450D,0x8AB6C1A6,0x65C371FA,0x56CB4046,0xB1EC6138};
    uint32_t p[]                ={0xEE67BA57,0xCC77C55A,0xE816C722,0xE6166940,0xAE6CB80A,0xC6746D17,0xB5515437,0x993AFC45,
                                  0x753C5C25,0xE04A3B2F,0x1D15731D,0xC27A6B37,0x7D3F5E46,0x934F433B,0x2E177B60,0x9E2C8A36,
                                  0x066A8106,0x391DF215,0xBA3C790F,0x0A273B70,0xD365FB6D,0x95508622,0xD54AAB28,0x0948F80B,
                                  0xAD4BF547,0x4A569264,0x5A280655,0x400A4F6B,0xDD6C0D46,0x61487E67,0x652FB273,0xFB2A5F39};
    uint32_t q[]                ={0xC65B0779,0xD70E8B75,0x285D5E4A,0x066A547A,0xC95DB021,0xD73BA008,0xD913154B,0xA8788333,
                                  0x9F41691A,0x4D73B613,0xC4526040,0xF82AD00D,0xE03DE555,0x674BC401,0x4516B156,0x536B9C3F,
                                  0x097A2D07,0x1755765D,0x58488C01,0x9E65BE4A,0xFA4B2266,0x914DAC3C,0x21460A6B,0xC040FF4F,
                                  0x7705F646,0xCF5FFA7B,0x67755833,0x24689D32,0xA951B001,0x0F76274A,0x126A3600,0xF965CB0B};
    uint32_t fai_n[]            ={0x5749B250,0x4C89D370,0xE35FC726,0x71AD2DB6,0x42C65F64,0xFA8FCEF5,0x7EC527BE,0xE544AF42,
                                  0x5EBFF569,0x1A4AA575,0x4D4EB468,0x76831838,0x404D4E0E,0x4D47CF00,0xE17A2EA3,0x78638715,
                                  0x339A4F89,0xE31CA637,0x6E790834,0x44F87424,0x54D34FC7,0x43153C30,0x739256A0,0x9D12F31A,
                                  0xF5436027,0x4F6CAD33,0xD05A1359,0x8E8D347D,0x30E25778,0xABD8193D,0x790D8D00,0x7E16FEB8,
                                  0x4FE252B0,0xEAAF8664,0x1C3E5629,0xF7ADC580,0xE6FD8686,0xE5C1D974,0xDD1A9320,0xBBACD6AC,
                                  0xBC6BFB28,0x369EC362,0x972C9A1D,0x38151F1E,0x434953E2,0xCC073279,0x212E90EF,0xC119ECF5,
                                  0x70525B75,0x93FEA9BD,0xBA977727,0x128B60A9,0x58023F8C,0x74B99509,0xF6DBAF29,0xF0ACC92B,
                                  0x80D5B50B,0xE7BD12F5,0x6F3E4D6C,0x7F94F102,0x9636C968,0x42D5D0F3,0xDB761A50,0xF4B0158B};
    uint32_t d[]                ={0x3E2E6951,0x56B06BB9,0x77DF6CEA,0xA6A6014D,0x2C1256A8,0xFE72D0CE,0xF2C7B105,0xEC4F72D6,
                                  0xA17B5E2D,0x0F61B20E,0x6F5EC432,0x3FBC95E7,0xE77DE685,0xE6374A44,0xA9ED8D07,0xF27C0F60,
                                  0x8051C85C,0xEE1760C3,0x55EE081F,0x02297F31,0xFA9CC08D,0xE9C19B52,0xE6C74E70,0x4074D3EA,
                                  0x5F0AAE56,0xED6D66C7,0xD3974447,0x8B2DD59F,0x7165C861,0x95AAB20A,0x83A81978,0xE6A5ECE1,
                                  0x5D8E49D3,0x73508CE4,0x4E803D81,0x32FF473D,0x2A42D3EA,0xE98CFA56,0x3B4B529B,0x8E6B2A23,
                                  0x9E43DEFE,0xA4C80377,0x0F2E5B34,0x94EE4CAB,0xE941CEB0,0x1B880A08,0xFBD96B3E,0x4C31B539,
                                  0x7D56FA0A,0x4F08A11B,0x470E841C,0x7E139E3B,0x90CAEF21,0x2B492E1B,0x4267EC1E,0xCF448A59,
                                  0x0888D18E,0xD6C20A73,0xBFEE2C86,0xACBC96E8,0xE8B4C964,0x0B19F921,0x868D59B6,0x4763CC95};
    uint32_t n[]                ={0x0C0C741F,0xF0102441,0xF3D3EC93,0x5E2DEB71,0xBA90C790,0x983FDC15,0x0D299142,0x26F82EBC,
                                  0x733DBAAA,0x480896B8,0x2EB687C6,0x3128537D,0x9DCA91AB,0x47E2D63D,0x54A85B5A,0x69FBAD8B,
                                  0x437EFD97,0x33900EA9,0x80FE0D45,0xED856DDF,0x22846D9A,0x69B36E90,0x6A230C34,0x669CEA75,
                                  0x19954BB5,0x69233A14,0x91F771E2,0xF300211B,0xB7A014BF,0x1C96BEEF,0xF0A77574,0x72A728FC,
                                  0x4FE252B2,0xEAAF8664,0x1C3E5629,0xF7ADC580,0xE6FD8686,0xE5C1D974,0xDD1A9320,0xBBACD6AC,
                                  0xBC6BFB28,0x369EC362,0x972C9A1D,0x38151F1E,0x434953E2,0xCC073279,0x212E90EF,0xC119ECF5,
                                  0x70525B75,0x93FEA9BD,0xBA977727,0x128B60A9,0x58023F8C,0x74B99509,0xF6DBAF29,0xF0ACC92B,
                                  0x80D5B50B,0xE7BD12F5,0x6F3E4D6C,0x7F94F102,0x9636C968,0x42D5D0F3,0xDB761A50,0xF4B0158B};
    uint32_t n_h[]              ={0x5B292633,0x367AA1B9,0xCF3DC2B5,0x6FC7C918,0x4268331E,0xE2B69BA9,0x39CE91D3,0x860B7963,
                                  0x778FA78C,0xD1B096E7,0xB0DDCC97,0xD9DEB792,0xACBBCC56,0x5A6F7650,0x52BFDA10,0xA491790F,
                                  0x9C3E9943,0x41A4E0FD,0x7B3EE7A5,0xCAF09CFF,0xCA9519EF,0xF8F150E6,0xF5048828,0xAF051CB9,
                                  0x5D20DFEF,0x5ECC6254,0x6DF2F74C,0xD70EF202,0xC5BD9480,0xCAF8FDF1,0x567E8659,0x447ACFED,
                                  0x66FF7321,0x76BE5233,0xEC06F28D,0x3242F3A9,0xD4DB116D,0x07631F57,0xAA52964A,0x96C4BD8B,
                                  0xCE625687,0xFCE47F65,0x96ADF972,0x46DE6B9B,0x55A94BEE,0xF118EFDD,0xC3054E75,0x717CEF58,
                                  0xE3220FCD,0xF61E3202,0x19E329A6,0xDAA51371,0x35D3AAD3,0x060C69B0,0xC3EA99B5,0xF04F7DD1,
                                  0x090DD262,0x65E227A3,0x3105075A,0xDA284CAC,0x0A5F33CA,0x72B555E6,0x6930DDA9,0xB5E37263};
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    uint32_t n_n0[]             ={0x9939F821,};
#endif
    uint32_t n_10_01_h[2048/32] ={0};
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    uint32_t n_10_01_n0[]       ={0xFFFFFFFF};
#endif
    uint32_t n_11_11_h[2048/32] ={0};
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    uint32_t n_11_11_n0[]       ={1};
#endif
    uint32_t std_modadd[]       ={0xD308F8DD,0x6069D22A,0xD0F72EDE,0x0F7D915B,0x26942A7D,0x3092DC61,0x5F94427D,0x235C49FC,
                                  0x354B633C,0x7B50C650,0x93C53883,0xC49D7958,0x05B120C0,0x74F18B09,0xAD2A3B20,0xB11E4794,
                                  0x68AFA81B,0x2F96CAB0,0xCC0492AA,0x1E6B7349,0x6386B5DA,0x88FE113A,0xB0C4EAA4,0xF2161140,
                                  0x8A23165A,0x05760AB3,0xB5A79F22,0x1CDC8E81,0x689344E1,0x3DEC8143,0xC539A843,0x385B65E0,
                                  0x68ACB93A,0x41ED029C,0x806A70E8,0xC6FE0979,0x4234B3AE,0x62C2E6D9,0x1D0618DB,0xC7F5C48A,
                                  0xF9B7BD97,0x8F58DC9F,0xF7AD8606,0x7FE19EC5,0x18520BE4,0xB301A096,0x1F1391B9,0xBD90AACF,
                                  0x7BFDE92D,0x9E84A3CF,0x23749C51,0x6182DF50,0x9E55E2BA,0xC12EC184,0x6E07C9AC,0x8BC6E9AB,
                                  0xDF756851,0x7F46749B,0x27B8404B,0xC40F015B,0x162F4876,0x5BD94F89,0x5B950F6A,0x495B95E3,};
    uint32_t std_modsub_b_a[]   ={0xADBAF1E5,0x35954E59,0x9DD27EC2,0x13EF4637,0xC39BD75E,0xA24051CA,0xF3A439A5,0xBDBC5CF9,
                                  0x655C1CD0,0x2124A743,0x91468A22,0x7573B1BF,0x1E7235ED,0x96A75552,0xBC5288B4,0x0015592B,
                                  0xD2C439BF,0x6B390A41,0x9840E718,0x31C08974,0xEF8F6A4D,0xFD56AF9D,0xB2BE9255,0xBD37CBFB,
                                  0x91A3E6A5,0x9DBAB917,0xBCD39350,0xAE9A9A70,0x360BEB76,0xF45F9AA5,0x83ABC054,0xCA792F51,
                                  0x890DA77C,0x9902841B,0x339821FD,0x51EAA834,0xA3896FD8,0xC93B650A,0x639D17DC,0x375FEEF9,
                                  0xB0C6DAB1,0xDF1E14D2,0x4133B184,0xCE6C787C,0x9078D71D,0xC0E9E22D,0xB1FB989D,0x958BE924,
                                  0x09BFF0BC,0x45768043,0x56006960,0x03434B1D,0xDF183C44,0xA742BFC1,0x8D6E0F9A,0x4A6B1291,
                                  0xA06A375F,0xE5BABB24,0xFBF33A26,0x6ACC360F,0x20ED8B0F,0xA89942EE,0x32415A96,0xA69C76C4,};
    uint32_t std_modmul[]       ={0x63B061BB,0x6CC11777,0xFEF5C9BF,0xFFB78594,0x7A32F761,0xC2E74C67,0x065C99B1,0x283DE464,
                                  0x81791C0F,0x72BAD5B2,0x9876E56C,0x839BC6C8,0x4C3830EC,0xC57B96BE,0x216EDD97,0x8B18B81D,
                                  0x1D3A0FD0,0x147724CF,0x43053CC4,0xBAB4B8C1,0xBE7B58B7,0x72F2D818,0xDC3ADBF9,0x9C563A47,
                                  0xE55D332F,0x885CD073,0xF306FED1,0x6F152A75,0x065755BE,0x3F8E1ADC,0xFD538855,0xE8BF35AF,
                                  0x5DC48E82,0xC84D63B7,0xDBA4AC6D,0xFF27D1BA,0xE35B61BF,0x17F991D1,0x6AA03870,0xE82ED6ED,
                                  0x4D620C96,0x6CF44DA8,0xA0D4A996,0x3E8B2141,0x01053052,0x8217DD47,0x5ED45D78,0xE1C2B159,
                                  0x122043D6,0x813AE9CE,0x18947428,0x7805410C,0xC2C1AF77,0xD1272A48,0xED242A8E,0x5222DB4F,
                                  0xF0548BC2,0xC8474ED4,0xA8800E84,0x997D61CF,0x986845D3,0xFB1A78E4,0x1F12AABB,0x41934500};
    uint32_t std_modinv_odd[]   ={0xEAEF6F19,0x10F066AB,0xEA11B5A1,0x80AF71F1,0x3A40E4D5,0xECD635D8,0xAF4177F3,0x50C3C40C,
                                  0xEBEE78FA,0xBC3913FC,0xDA8A2134,0x32A8F6A1,0xF3F46BBD,0x15914FFF,0x0FA6FF49,0x40651A69,
                                  0x54F5910B,0xCB622F98,0x3B6972B9,0xEF1D29D6,0x9B15EAF8,0xF45C797B,0x5FE4985F,0xA5B39934,
                                  0x5D43FB4C,0xB25D7B7E,0x0DBB7EA3,0xB8919C3A,0x8DA9B1D9,0x95367FB1,0xACC5FD31,0x50D64FF7,
                                  0x784CA130,0x278EBD1C,0x4F9A47C5,0x132516AF,0x68EA6114,0x10EACEC8,0x49C1A935,0xF96FC1B3,
                                  0xF36A8993,0x6C891B16,0x3AC3140D,0xEDC9526F,0xCF62278E,0x23239897,0x11B1615F,0x7CD05DBC,
                                  0x5CE031D7,0x7AD9800F,0x51BC9CD3,0x0ED7309A,0x98170138,0x01C0309A,0xFB7E5A26,0x3D9ECE3E,
                                  0x9465911E,0x6B1CBDCA,0x69227505,0xD5C9E668,0x80039189,0x3341F3AA,0xA0DB400A,0xC3D05279};
    uint32_t std_modinv_even[]  ={0x00542319,0xDCF8EE57,0xC319C34B,0x4255DD90,0xBE707BAA,0xBE70C439,0x6AC775A0,0x96EF81A5,
                                  0x880C71A8,0xAC60050F,0x4C868BD0,0x88697B23,0x23243943,0x40B25D8B,0x9A9F6848,0xA20D3311,
                                  0x492DDC78,0x7F3507D1,0xA5E4CD4E,0x3DE6117A,0xB7AE544F,0x98210569,0x3DCDB124,0xF1044809,
                                  0x4D380F2A,0x1CCB21C6,0xEB83FA23,0x3629DFEE,0xC597815A,0x968BACA1,0xA86AA159,0xD811566F,
                                  0x23BAEF6B,0x4253C1B0,0x105D75F3,0xA42D1537,0x92B88B59,0x5664FC05,0x1A08A968,0xA702CF22,
                                  0x06AE18B5,0x00DE7D5A,0x75AB3E41,0xEEC7A8CF,0xA400AC51,0xDDF56F80,0xD2C947A5,0xE5EC568E,
                                  0x726FE67C,0x74F2C63B,0x0FCD1BED,0xB5D358D1,0x98093C6A,0xF65F55CF,0x464AD647,0xA1D9C7A7,
                                  0x8B8FFE91,0x5051B69D,0x69FEB8E8,0x8056AB97,0x42126213,0xED1D7E72,0xE597F628,0xC270A78C};
    uint32_t std_modexp[]       ={0x3233BB69,0x1327B0BA,0xF3EE51AF,0x55AECA80,0xA8E64C82,0x711EB4B5,0xB808B9B8,0x49C3F431,
                                  0xF1675330,0x20DE9715,0x6328A8E8,0x5FE52BA0,0x16722BAC,0xCB94E2FF,0xD769C8E5,0xCBF9D238,
                                  0x33813A2A,0xCC8D122E,0xECF4BDA0,0xDC766DC0,0xCA17B920,0x54EC6308,0xD96DA780,0x0B0DAC9B,
                                  0x8A574759,0x79D92E0F,0xBC0EE65B,0xB7CD0AD8,0x9D6AE460,0x9AADDA2A,0x9DFD913B,0x4B5392FD,
                                  0x89288106,0xB4B5E09C,0x664E0833,0x8DD9E73A,0xE576EE00,0x9DD98C96,0xDE7E1909,0xA75941C4,
                                  0xE8642179,0x682449C8,0x66FFC65C,0xD564715A,0x742AE0C1,0x991E5EC6,0x0131F735,0xF37CC4F5,
                                  0xACC89B81,0x553C0642,0x3F76CF2D,0xA9D7F569,0x0B08AC77,0xF3A5EA11,0xDD4EE0A5,0xDB92396F,
                                  0x7BA59509,0x176AA8A8,0xE0C369F0,0x26333FF1,0x8EFBE852,0xD75F9D32,0xA78CCF22,0xCF27B66F};
    uint32_t std_modexp_d[]     ={0x5E1F660E,0x284FD973,0x19C24E4C,0x56112B97,0x1D68CDED,0x5E040805,0xFB42BB3E,0x7C0C578E,
                                  0xEE1AD904,0xC4EC899C,0x6322827A,0x4C332CF0,0x0407D868,0x4F90A912,0x66151909,0x83E9C481,
                                  0x76F18400,0x372F7F2C,0x7EA0C5D6,0x1FD4A977,0x892565D7,0xFA07D3B7,0x1B1BEB2D,0x5065FFF0,
                                  0x1184E6A9,0x25E9B3D5,0xE43D6A64,0x47492FA5,0xBEBAD5CF,0xEDB116AA,0x913E914D,0x9B1A9D59,
                                  0x9D2D3BCD,0x33195EBD,0xB4C32C5A,0x6323DC82,0x64A463C1,0xF9FB8715,0x8A486013,0x7614E252,
                                  0xA6E66313,0x1E087B20,0xF59EBE05,0xCFE057DB,0xC0350F26,0x6DF866F5,0x8431A7D0,0x1DA40293,
                                  0xEEBF2C10,0x8279C8AC,0x06269028,0x41FB469C,0xDA0F07DA,0xB25F7AB6,0x3E7BEAED,0x22325EC0,
                                  0x06067339,0x3FF5A2A4,0xA79486B0,0xD90971B6,0x573AD6DB,0x6ADC8FF7,0xC575FE74,0x7BB163D2};

    vector->bitLen          = 2048;
    vector->a               = a;
    vector->b               = b;
    vector->p               = p;
    vector->q               = q;
    vector->fai_n           = fai_n;
    vector->d               = d;
    vector->n               = n;
    vector->std_n_h         = n_h;
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    vector->std_n_n0        = n_n0;
#endif
    n_10_01_h[0]            = 4;
    vector->std_10_01_n_h   = n_10_01_h;
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    vector->std_10_01_n_n0  = n_10_01_n0;
#endif
    n_11_11_h[0]            = 1;
    vector->std_11_11_n_h   = n_11_11_h;
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    vector->std_11_11_n_n0  = n_11_11_n0;
#endif
    vector->std_a_add_b     = std_a_add_b;
    vector->std_a_sub_b     = std_a_sub_b;
    vector->std_b_sub_a     = std_b_sub_a;
    vector->std_modadd      = std_modadd;
    vector->std_modsub_a_b  = std_a_sub_b;
    vector->std_modsub_b_a  = std_modsub_b_a;
    vector->std_modmul      = std_modmul;
    vector->std_modinv_odd  = std_modinv_odd;
    vector->std_modinv_even = std_modinv_even;
    vector->std_modexp      = std_modexp;
    vector->std_modexp_d    = std_modexp_d;

    return pke_basic_test(vector);
}
#endif


#ifdef PKE_BASIC_2766_TEST
uint32_t pke_basic_2766_test(void)
{
    pke_basic_test_vector_t   vector[1];

    uint32_t a[]                ={0x361AA9BB,0xC566DA28,0x84DC71F3,0x613C269A,0x4C3ADDD5,0x0AA13BF4,0x61637D35,0x2E989CB2,
                                  0x5864FAB4,0xE9740F45,0xE60E9F6E,0x263FD92E,0xCABD57CC,0xC45C3A38,0x2194845D,0xBB040A3F,
                                  0x524436EB,0x354C1681,0xA04E37E2,0x3953866D,0x8C3FDCC7,0x33F1BFD6,0x9D58C543,0xFB4D75F7,
                                  0x7C9336AC,0x43BB9450,0x7C295A54,0xD2F0A262,0x32013899,0x578ABCEB,0x619CBE34,0xFD03C7EC,
                                  0x7B17924B,0x2A665E76,0x11377E32,0x4E086A7F,0xE1476245,0x366AF9FC,0x10F9B403,0x61B41056,
                                  0x825A3D2B,0xF9B90B9B,0x2F3137A8,0x3EF82CA9,0xCCEE8232,0x20E2C8B3,0x8C83FC91,0xBBB64915,
                                  0x4F411FFD,0x85D53C62,0xEF5A3FBE,0x2CAD0663,0xD26B6C42,0x783B4510,0x9DED6FCD,0x439911DF,
                                  0x53EFEF74,0xE803DE14,0xCCB1277A,0x8DCFF026,0x6B9DAD35,0x822C6F37,0x143E5E38,0x63A82D54,
                                  0xECF53E05,0x5E07AFAC,0xB1BA393C,0x1A454336,0x4FB052E7,0x36740D01,0xD87CBBE8,0xE967F077,
                                  0xDAE43751,0x5DA10078,0xD05B26F1,0xF9ADADC0,0x1E7200C4,0x66D957F5,0xF472B27E,0x65D3F1F7,
                                  0xEBF70B0F,0xF4E1F01D,0xF4426CB6,0xF0C68D2C,0x4DE511B8,0x0084B97A,0x00001DDD};
    uint32_t b[]                ={0x21ADA7FA,0x7AF65D9F,0xD9E4BC00,0xF8DBC4CA,0x30E2379B,0x15D1CEF1,0x5A75CFB2,0x8186F521,
                                  0x8E50BE65,0x54E8D509,0x2773BC7E,0x4F2E912E,0x602CD3A7,0xFF8761FD,0x5E28EF6F,0x433CCE39,
                                  0xC50B7ADC,0xDEAC55EE,0xE002A7D4,0xAF92B17C,0x5A5DF25A,0xEA90ECA8,0xC7A182CF,0xD0FBA88A,
                                  0xB9848C58,0xA001BD31,0xA7AE028E,0xC9F0984D,0x13840B2D,0x8DA069A1,0x682CB801,0xAFB5E634,
                                  0xF0D96A02,0xBA208B10,0xB839F2E5,0x2BB183BE,0x1A7B5432,0x344066E6,0x9AFA8DAA,0x46AF9ADA,
                                  0x3FA937DD,0x4A68BD81,0xC42B1DF6,0xB7E1BD85,0xBBAF8C5E,0x266A3CE6,0x3CB90951,0xCB2B2444,
                                  0x652BEA8B,0x7980B118,0xCC613799,0xE982DA45,0x0ABDCCA8,0xC34CD149,0xB0E9F60E,0xEED9206A,
                                  0x8601D216,0xD2508F5E,0xB7C020BA,0x60F1394A,0xFCD534B8,0x787000C9,0xA29F8AF5,0xD2C410E1,
                                  0x7A444B47,0xD4AED29C,0x8DF87D26,0x611E090F,0x56F906D7,0x92C078CD,0x4000C598,0x3256AF8F,
                                  0x29ADC0C6,0xBB69CF87,0xFF794031,0x31862237,0xFB822FFC,0x0F42E163,0x15AC7B50,0x3DEE0067,
                                  0xE65F34CC,0x71DF4925,0xCED9DB65,0xEA0B80F6,0x5230E4C9,0x71F70FAA,0x000019AC};
    uint32_t std_a_add_b[]      ={0x57C851B5,0x405D37C7,0x5EC12DF4,0x5A17EB65,0x7D1D1571,0x20730AE5,0xBBD94CE7,0xB01F91D3,
                                  0xE6B5B919,0x3E5CE44E,0x0D825BED,0x756E6A5D,0x2AEA2B73,0xC3E39C36,0x7FBD73CD,0xFE40D878,
                                  0x174FB1C7,0x13F86C70,0x8050DFB7,0xE8E637EA,0xE69DCF21,0x1E82AC7E,0x64FA4813,0xCC491E82,
                                  0x3617C305,0xE3BD5182,0x23D75CE2,0x9CE13AB0,0x458543C7,0xE52B268C,0xC9C97635,0xACB9AE20,
                                  0x6BF0FC4E,0xE486E987,0xC9717117,0x79B9EE3D,0xFBC2B677,0x6AAB60E2,0xABF441AD,0xA863AB30,
                                  0xC2037508,0x4421C91C,0xF35C559F,0xF6D9EA2E,0x889E0E90,0x474D059A,0xC93D05E2,0x86E16D59,
                                  0xB46D0A89,0xFF55ED7A,0xBBBB7757,0x162FE0A9,0xDD2938EB,0x3B881659,0x4ED765DC,0x3272324A,
                                  0xD9F1C18B,0xBA546D72,0x84714835,0xEEC12971,0x6872E1ED,0xFA9C7001,0xB6DDE92D,0x366C3E35,
                                  0x6739894D,0x32B68249,0x3FB2B663,0x7B634C46,0xA6A959BE,0xC93485CE,0x187D8180,0x1BBEA007,
                                  0x0491F818,0x190AD000,0xCFD46723,0x2B33CFF8,0x19F430C1,0x761C3959,0x0A1F2DCE,0xA3C1F25F,
                                  0xD2563FDB,0x66C13943,0xC31C481C,0xDAD20E23,0xA015F682,0x727BC924,0x00003789};
    uint32_t std_a_sub_b[]      ={0x146D01C1,0x4A707C89,0xAAF7B5F3,0x686061CF,0x1B58A639,0xF4CF6D03,0x06EDAD82,0xAD11A791,
                                  0xCA143C4E,0x948B3A3B,0xBE9AE2F0,0xD7114800,0x6A908424,0xC4D4D83B,0xC36B94ED,0x77C73C05,
                                  0x8D38BC0F,0x569FC092,0xC04B900D,0x89C0D4F0,0x31E1EA6C,0x4960D32E,0xD5B74273,0x2A51CD6C,
                                  0xC30EAA54,0xA3B9D71E,0xD47B57C5,0x09000A14,0x1E7D2D6C,0xC9EA534A,0xF9700632,0x4D4DE1B7,
                                  0x8A3E2849,0x7045D365,0x58FD8B4C,0x2256E6C0,0xC6CC0E13,0x022A9316,0x75FF2659,0x1B04757B,
                                  0x42B1054E,0xAF504E1A,0x6B0619B2,0x87166F23,0x113EF5D3,0xFA788BCD,0x4FCAF33F,0xF08B24D1,
                                  0xEA153571,0x0C548B49,0x22F90825,0x432A2C1E,0xC7AD9F99,0xB4EE73C7,0xED0379BE,0x54BFF174,
                                  0xCDEE1D5D,0x15B34EB5,0x14F106C0,0x2CDEB6DC,0x6EC8787D,0x09BC6E6D,0x719ED343,0x90E41C72,
                                  0x72B0F2BD,0x8958DD10,0x23C1BC15,0xB9273A27,0xF8B74C0F,0xA3B39433,0x987BF64F,0xB71140E8,
                                  0xB136768B,0xA23730F1,0xD0E1E6BF,0xC8278B88,0x22EFD0C8,0x57967691,0xDEC6372E,0x27E5F190,
                                  0x0597D643,0x8302A6F8,0x25689151,0x06BB0C36,0xFBB42CEF,0x8E8DA9CF,0x00000430};
    uint32_t std_b_sub_a[]      ={0xEB92FE3F,0xB58F8376,0x55084A0C,0x979F9E30,0xE4A759C6,0x0B3092FC,0xF912527D,0x52EE586E,
                                  0x35EBC3B1,0x6B74C5C4,0x41651D0F,0x28EEB7FF,0x956F7BDB,0x3B2B27C4,0x3C946B12,0x8838C3FA,
                                  0x72C743F0,0xA9603F6D,0x3FB46FF2,0x763F2B0F,0xCE1E1593,0xB69F2CD1,0x2A48BD8C,0xD5AE3293,
                                  0x3CF155AB,0x5C4628E1,0x2B84A83A,0xF6FFF5EB,0xE182D293,0x3615ACB5,0x068FF9CD,0xB2B21E48,
                                  0x75C1D7B6,0x8FBA2C9A,0xA70274B3,0xDDA9193F,0x3933F1EC,0xFDD56CE9,0x8A00D9A6,0xE4FB8A84,
                                  0xBD4EFAB1,0x50AFB1E5,0x94F9E64D,0x78E990DC,0xEEC10A2C,0x05877432,0xB0350CC0,0x0F74DB2E,
                                  0x15EACA8E,0xF3AB74B6,0xDD06F7DA,0xBCD5D3E1,0x38526066,0x4B118C38,0x12FC8641,0xAB400E8B,
                                  0x3211E2A2,0xEA4CB14A,0xEB0EF93F,0xD3214923,0x91378782,0xF6439192,0x8E612CBC,0x6F1BE38D,
                                  0x8D4F0D42,0x76A722EF,0xDC3E43EA,0x46D8C5D8,0x0748B3F0,0x5C4C6BCC,0x678409B0,0x48EEBF17,
                                  0x4EC98974,0x5DC8CF0E,0x2F1E1940,0x37D87477,0xDD102F37,0xA869896E,0x2139C8D1,0xD81A0E6F,
                                  0xFA6829BC,0x7CFD5907,0xDA976EAE,0xF944F3C9,0x044BD310,0x71725630,0xFFFFFBCF};
    uint32_t p[]                ={0x4AF43D8D,0xCCE11025,0x89BA663C,0x403782E3,0xA4EE4F82,0x256D2F48,0xF649EA05,0xCDB95EE5,
                                  0x0F2D4A0E,0x45A7C256,0xF61155B1,0xDC194590,0xCFCA0302,0x205A6939,0x2F6A3EFB,0x66E2444A,
                                  0xABD941B7,0x4A8EEA91,0x8AEDF219,0x82DAA544,0x6E124F8C,0x6615742A,0x1563E5F5,0x28CDEE82,
                                  0xA2185480,0x02A81465,0x62512D56,0x72E431A6,0x3C2628EB,0x8D83DCB4,0x66DD1504,0x248D9CFD,
                                  0x654F4C1F,0x67711CF8,0x454E1F14,0xD4C23D61,0x242DB029,0xB97327FE,0x01078AFA,0x92E6E136,
                                  0x1A250A7C,0x1FCB10F0,0x92F89AD2,0x0000007C};
    uint32_t q[]                ={0xAAA6FEA1,0x5CC482E9,0xD322387B,0x7903A632,0xE9C27957,0xDCC2F91A,0x86F1B83C,0xEDC4E887,
                                  0xAEC51E4D,0x446F93A2,0x7BD0710E,0xF6627277,0x22DA3A11,0x76B28AB9,0x0683DF19,0x005EF6C0,
                                  0x4C428200,0xD45F8745,0x7AB4F922,0x34E38613,0xBB9CC122,0x13E05473,0x0A8FB2F5,0x03CA0D16,
                                  0x83BEC923,0x3D78B67C,0xCCCD98D8,0x9F0E2FB9,0xD414943B,0x83F71656,0x815FF3FA,0x5EBEB15A,
                                  0x0D768827,0x31E8B244,0x2C78E7E1,0x857A99EA,0xA978CA15,0x636103C2,0x202FF8E0,0x5D70482C,
                                  0xCC52939B,0xC42309E8,0x8C876B7C,0x00000069};
    uint32_t fai_n[]            ={0x0E7F5F80,0x4560FF58,0x395A8553,0xC7F74DF1,0x96F96B2B,0x5F83D8F7,0x383007D3,0x300B40AA,
                                  0x694A33CE,0x7160C9DC,0x2CA685AD,0xF2BDC89E,0x3EA8D3DD,0x76979150,0x68990CF4,0x1683FDEF,
                                  0x3A31706E,0xDE612756,0xF679E4B2,0x1563822E,0x6EBCD76C,0xD246C345,0xE48B5280,0xA7F3D945,
                                  0x1ED5E22D,0x34515783,0x2FCBC3F8,0x36C3FC95,0x0DCCBCE2,0x7040B900,0xA571EEF6,0xE7571111,
                                  0x4F4E9BB1,0xF3F586F9,0xA91C8F26,0xE9D1930D,0xD95ED491,0xEFC1656F,0xDE9B5632,0xE4AA72A0,
                                  0x02DE2880,0xB1AD16D8,0xD74959AC,0xEB0666CB,0xE410F73A,0x64CF2FA6,0x0AB73F75,0x25C2F706,
                                  0x297968F4,0x21B2452D,0x4152753A,0x6D36705F,0xA0D95CDD,0x0A8327FC,0xC600437A,0x10B98502,
                                  0x63F4918B,0xD49BDFED,0x596EFB5D,0xE26282EE,0x0FCF0FFF,0x43F85DCF,0xFA28B3F6,0x79F6A54F,
                                  0xA5954B56,0xEF63BF41,0x369C3F33,0x2D7B5AC5,0x16E4E98E,0x9F90C8E1,0xDB377A84,0x94389116,
                                  0xBC2B1DA1,0xD0BD49B4,0x18AB9BD0,0x54427F6F,0x3D1A44AD,0x265976B7,0xE09202CC,0xD1CD73BA,
                                  0xEDAA5B9D,0xB46CA2AE,0x649D1B3F,0xA3A86DDD,0x232FD5DE,0xAA3D4418,0x0000335C};
    uint32_t d[]                ={0x41F60301,0x9FD9093B,0x088A7448,0x8A05D230,0x335C485D,0x8CE5533F,0x3A873E1A,0xE9A87D1A,
                                  0x0398AF4C,0x522F42E3,0x5A6800B7,0x773278EA,0x8801EB9F,0xAD66962F,0xFF50ED81,0xCB605310,
                                  0x717E4C19,0x1B80808E,0xE4C1B541,0xBEE79A80,0xD697FB5D,0x490CAE26,0x389AB729,0x72FEE297,
                                  0xD0218128,0xF5071638,0x72D5B2E7,0x98BC23E7,0x605CC943,0x1B4CCCBC,0x915DA523,0x23697EE1,
                                  0xA03FABE1,0xB8D73538,0xC7712C0A,0xBFE557B8,0xA9D20AB3,0xBCDE94E1,0xFD6EAEC9,0x77F474CB,
                                  0x4C818862,0x877FAF13,0x458FA85C,0x536C1A84,0xDA259CFC,0x282E042A,0xAF3F3936,0x05E56204,
                                  0x715D5ADD,0x2BF79464,0xDA28A33D,0x422EE8C7,0xC09B4CA8,0xA8CE8D99,0xBF74ABB1,0x051341F3,
                                  0xC9094DEC,0x3DCAC959,0xE3987143,0x3E3193E7,0x48155B28,0x2A22871D,0xD91B6E5C,0x4CA4BDB4,
                                  0x4B32DBB9,0xD32A77F4,0xF4A5DBE2,0xED80367E,0xD8B60861,0x471200A7,0x2F0EA3A9,0x2E9E0CA5,
                                  0x5BC1EA63,0x9D590245,0xD4D522A4,0x5375A29A,0xF8E7FEE7,0xCB0B6705,0x896BFEC8,0x810A39A3,
                                  0xE5B34BE2,0x97699494,0x59D1941E,0x31A34D17,0x1E834CE7,0x1088B4D3,0x00000B88};
    uint32_t n[]                ={0x041A9BAD,0x6F069267,0x9637240B,0x81327707,0x25AA3405,0x61B4015B,0xB56BAA15,0xEB898817,
                                  0x273C9C2A,0xFB781FD5,0x9E884C6C,0xC53980A6,0x314D10F2,0x0DA48543,0x9E872B09,0x7DC538F9,
                                  0x324D3425,0xFD4F992D,0xFC1CCFEE,0xCD21AD86,0x986BE81A,0x4C3C8BE3,0x047EEB6B,0xD48BD4DE,
                                  0x44ACFFD0,0x74722265,0x5EEA8A26,0x48B65DF5,0x1E077A09,0x81BBAC0B,0x8DAEF7F5,0x6AA35F69,
                                  0xC2146FF8,0x8D4F5635,0x1AE3961C,0x440E6A59,0xA7054ED1,0x0C959130,0xFFD2DA0E,0xD5019C02,
                                  0xE955C698,0x959B31B0,0xF6C95FFB,0xEB0667B1,0xE410F73A,0x64CF2FA6,0x0AB73F75,0x25C2F706,
                                  0x297968F4,0x21B2452D,0x4152753A,0x6D36705F,0xA0D95CDD,0x0A8327FC,0xC600437A,0x10B98502,
                                  0x63F4918B,0xD49BDFED,0x596EFB5D,0xE26282EE,0x0FCF0FFF,0x43F85DCF,0xFA28B3F6,0x79F6A54F,
                                  0xA5954B56,0xEF63BF41,0x369C3F33,0x2D7B5AC5,0x16E4E98E,0x9F90C8E1,0xDB377A84,0x94389116,
                                  0xBC2B1DA1,0xD0BD49B4,0x18AB9BD0,0x54427F6F,0x3D1A44AD,0x265976B7,0xE09202CC,0xD1CD73BA,
                                  0xEDAA5B9D,0xB46CA2AE,0x649D1B3F,0xA3A86DDD,0x232FD5DE,0xAA3D4418,0x0000335C};
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    uint32_t n_h[]              ={0x809CD860,0x0705DD35,0x02BFF920,0x5BBD28B5,0x92DA7B8C,0x1DF1A5E6,0xEC34DD66,0x73811673,
                                  0x0DD5BFF3,0xBF733E2A,0x06B9DB7B,0x186840D2,0x91A58D47,0xFB30763D,0xBC13195C,0x5F3D6AE3,
                                  0xD2A81B1B,0x58F36ECC,0xBEE39A20,0xCE8C9658,0xBECDCC23,0xF94DFABA,0xCDDCB7AA,0x9BC27DBB,
                                  0x56103DBA,0x79E3991A,0xF85775D5,0x0B31224E,0x55D170CD,0xCBD0B5D5,0x5BBD46D4,0xDA4C26E8,
                                  0x676602BA,0xC096DE37,0xF87B5B3A,0x25A6B8A1,0xD68A88C4,0xB9A52059,0xA5C9E46A,0xF58BEE57,
                                  0x8401806F,0x0A454C3E,0x2E4BADB7,0x598A0E2F,0xB8D0A0B6,0x13CB1233,0x39381539,0x48F7FD87,
                                  0x9C4C2607,0x3DD24620,0xAF6D80C3,0xA3014EC0,0x9E659380,0x396F53BE,0x015EF47E,0xC9837B53,
                                  0x96419730,0x89C77FE3,0x45B6ED9D,0x600FFFA5,0xF375B074,0x8AA5910F,0x862FEB98,0x615DAA6D,
                                  0x12CBF663,0x61839514,0xE32FC52F,0xA1906759,0x199D66F6,0x0562E838,0x8A7F0267,0x843680C6,
                                  0xA7106A28,0xD7E6A96A,0x5769B503,0xD9E589D9,0xF0FE04FA,0xF9EC4ECB,0x97CEE94D,0xC2310E34,
                                  0xBCCE764F,0xF389C790,0xF4E97408,0xF5108525,0x865300DD,0x2CD655B7,0x2590};
    uint32_t n_n0[]             ={0xB24A7FDB,};
#else
    uint32_t n_h[]              ={0xC625C54E,0x7F0AF0C3,0xB621DD75,0xF2748CAB,0xED601F5D,0x5227085D,0x0E8AD2E5,0x6397C87D,
                                  0xFDA62930,0x55FE9B23,0x953BF199,0x73455C39,0x75AC526E,0xAE09E847,0x84185B79,0xA0086E15,
                                  0x2F055011,0x35D3E178,0x19EB61A2,0x0BDB95C6,0xCF0958C6,0x346CFE84,0xAD6CB6EC,0xDBD65155,
                                  0x90D3D8E5,0xFA94F2FA,0x63A36CAC,0x295DF4EB,0x8E352011,0xAD8F59FB,0x34B284A9,0x70388F9B,
                                  0x8D9D0536,0x4F1281D6,0x2984AE05,0xD4B2E26A,0x3BB06E32,0x7C932311,0xA29E7E00,0xF4ACC6D7,
                                  0xC2990AAE,0x3D9B8C01,0xBFBFD9F7,0x85632288,0xF24C8D34,0xF5CADC9E,0xED8C8A6C,0x4E46E541,
                                  0x68F08B3A,0xF5C480B5,0xB2E67D67,0xC9CBB04B,0x2EF34BED,0xE2321A09,0xA46A6A33,0xCCA2381C,
                                  0x7451E768,0xCE3AF39D,0x7D9E654F,0xA5529759,0x0C131B57,0x7587A362,0x5714670D,0x9693F442,
                                  0x01D533AB,0xB3A00C2F,0x64010260,0x812D7EA8,0xD82236A8,0x7BF5897C,0x0706C567,0x0935168D,
                                  0x75A69F0F,0xC7DF05D9,0xD88ADFEC,0x74BE6974,0xD383B2E3,0x41C3CDBC,0xA4E9A38C,0xFB25DD18,
                                  0x30ECB079,0x710CA122,0xD0E38B6A,0x1F492D51,0x9F216AA8,0x592DD5D7,0x00001FDD};
#endif

    uint32_t n_10_01_h[(2766+31)/32] ={0};
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    uint32_t n_10_01_n0[]            ={0xFFFFFFFF};
#endif
    uint32_t n_11_11_h[(2766+31)/32] ={0};
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    uint32_t n_11_11_n0[]            ={1};
#endif
    uint32_t std_modadd[]       ={0x53ADB608,0xD156A560,0xC88A09E8,0xD8E5745D,0x5772E16B,0xBEBF098A,0x066DA2D1,0xC49609BC,
                                  0xBF791CEE,0x42E4C479,0x6EFA0F80,0xB034E9B6,0xF99D1A80,0xB63F16F2,0xE13648C4,0x807B9F7E,
                                  0xE5027DA2,0x16A8D342,0x84340FC8,0x1BC48A63,0x4E31E707,0xD246209B,0x607B5CA7,0xF7BD49A4,
                                  0xF16AC334,0x6F4B2F1C,0xC4ECD2BC,0x542ADCBA,0x277DC9BE,0x636F7A81,0x3C1A7E40,0x42164EB7,
                                  0xA9DC8C56,0x57379351,0xAE8DDAFB,0x35AB83E4,0x54BD67A6,0x5E15CFB2,0xAC21679F,0xD3620F2D,
                                  0xD8ADAE6F,0xAE86976B,0xFC92F5A3,0x0BD3827C,0xA48D1756,0xE27DD5F3,0xBE85C66C,0x611E7653,
                                  0x8AF3A195,0xDDA3A84D,0x7A69021D,0xA8F9704A,0x3C4FDC0D,0x3104EE5D,0x88D72262,0x21B8AD47,
                                  0x75FD3000,0xE5B88D85,0x2B024CD7,0x0C5EA683,0x58A3D1EE,0xB6A41232,0xBCB53537,0xBC7598E5,
                                  0xC1A43DF6,0x4352C307,0x0916772F,0x4DE7F181,0x8FC47030,0x29A3BCED,0x3D4606FC,0x87860EF0,
                                  0x4866DA76,0x484D864B,0xB728CB52,0xD6F15089,0xDCD9EC13,0x4FC2C2A1,0x298D2B02,0xD1F47EA4,
                                  0xE4ABE43D,0xB2549694,0x5E7F2CDC,0x3729A046,0x7CE620A4,0xC83E850C,0x0000042C,};
    uint32_t std_modsub_b_a[]   ={0xEFAD99EC,0x249615DD,0xEB3F6E18,0x18D21537,0x0A518DCC,0x6CE49458,0xAE7DFC92,0x3E77E086,
                                  0x5D285FDC,0x66ECE599,0xDFED697C,0xEE2838A5,0xC6BC8CCD,0x48CFAD07,0xDB1B961B,0x05FDFCF3,
                                  0xA5147816,0xA6AFD89A,0x3BD13FE1,0x4360D896,0x6689FDAE,0x02DBB8B5,0x2EC7A8F8,0xAA3A0771,
                                  0x819E557C,0xD0B84B46,0x8A6F3260,0x3FB653E0,0xFF8A4C9D,0xB7D158C0,0x943EF1C2,0x1D557DB1,
                                  0x37D647AF,0x1D0982D0,0xC1E60AD0,0x21B78398,0xE03940BE,0x0A6AFE19,0x89D3B3B5,0xB9FD2687,
                                  0xA6A4C14A,0xE64AE396,0x8BC34648,0x63EFF88E,0xD2D20167,0x6A56A3D9,0xBAEC4C35,0x3537D234,
                                  0x3F643382,0x155DB9E3,0x1E596D15,0x2A0C4441,0xD92BBD44,0x5594B434,0xD8FCC9BB,0xBBF9938D,
                                  0x9606742D,0xBEE89137,0x447DF49D,0xB583CC12,0xA1069782,0x3A3BEF61,0x8889E0B3,0xE91288DD,
                                  0x32E45898,0x660AE231,0x12DA831E,0x7454209E,0x1E2D9D7E,0xFBDD34AD,0x42BB8434,0xDD27502E,
                                  0x0AF4A715,0x2E8618C3,0x47C9B511,0x8C1AF3E6,0x1A2A73E4,0xCEC30026,0x01CBCB9D,0xA9E7822A,
                                  0xE812855A,0x3169FBB6,0x3F3489EE,0x9CED61A7,0x277BA8EF,0x1BAF9A48,0x00002F2C,};
    uint32_t std_modmul[]       ={0xA115FE20,0xCE676C3C,0x9744A222,0x5F97A31D,0xF9E51463,0x45891743,0x25430793,0x4A514E0E,
                                  0xBD205A6C,0xC8B4A0D2,0x59A50E44,0xC8A51297,0x10BFD655,0xDFEDC0C3,0x62B6D5F5,0xBD6FBEDF,
                                  0xDEEF3691,0xB32B01B3,0x1E2D6930,0xF627153D,0x99D685DC,0x01B99E33,0x57643096,0x78C0BF7F,
                                  0xB5E972B4,0xB7C616AE,0xB7AFE388,0x8C10391B,0x79FB4EFA,0x89AE4BBC,0xCCE49D3A,0x171FDAD8,
                                  0x1AB49ED6,0xEC5B3C1B,0x5C80C34A,0x4ACB3715,0x91C6490D,0x08200FB2,0x8000C064,0x683A008D,
                                  0x46579DBA,0xFC2F15CC,0x14B3CF39,0x66925C4D,0x9561D194,0xF2B18030,0xE71E07DE,0xE3643217,
                                  0xABF6AC33,0x63788BC0,0xAF953D2B,0x0C25A8C2,0xE2365F55,0x29E9F9BE,0x8229F4F1,0xDA18C667,
                                  0x88A70415,0xABAF4998,0x27FB5CD1,0xF712F73B,0xCBFD9DD5,0x6143DBFA,0x34E573F7,0x3022E674,
                                  0x56FE8D2D,0xDA9C3857,0x3B6FD828,0xCA569946,0xDCAFB134,0x790C5AC2,0x7A9FA0E5,0xBEF517F3,
                                  0xCE5E6554,0xEFB6A0DE,0x853AB436,0x3C452B2D,0xA14AED21,0x0C1A6C21,0x04C33DB3,0x74E4571E,
                                  0xD5336B6A,0xE9942B4A,0xCB53F919,0x7C80D45C,0x0E5AF2DF,0x64E81066,0x0000201C};
    uint32_t std_modinv_odd[]   ={0x559365F6,0x5B122216,0x5A41FC59,0x9B390117,0x354AEF54,0xD9D7C8E2,0x03A458C3,0x2E5D9326,
                                  0x46B58DEC,0x715228E2,0xA510EBA0,0xFD01966A,0xE38A52A2,0x1C5964CC,0x083A53E4,0x63ADCF44,
                                  0x5D5E7EE0,0x9A9B1AB6,0x9981E4A2,0xC0C4D34C,0xE35D7365,0x2536C1DE,0x04998698,0xE706A712,
                                  0xC7C53688,0xBC7D7B56,0x22FD66AD,0x42409E91,0x9F31DEAF,0xF93736BD,0x7CEE47F6,0xCCD88A2C,
                                  0x70CD161A,0x7F62BD17,0x2839044E,0x281C9595,0x5FA48E2C,0xB7401B1C,0xD82CF5D8,0xAB3338D3,
                                  0xE7E028B6,0x09A2C167,0x9D9EDF85,0xD9FF7904,0x5ECF1F48,0xB8A23D08,0x827E3CF9,0x524FF36F,
                                  0x488330D3,0x40BCCB3F,0x3D9AEBAB,0xC97A3BCF,0x6502113E,0xC5743158,0xAFD08A66,0xC8C51F62,
                                  0x759D8E1F,0x68C20A68,0x2A8EEE19,0x472F852B,0xBCE7D43A,0x5A00423A,0xB0CE9A77,0xAF0EE4C7,
                                  0x5E503B6D,0x7F0ECC80,0xEEECCDD1,0x9D3A8109,0xF06B95E2,0x7E8EE5E3,0xE2617842,0x23CFD701,
                                  0xF2D1CBEA,0xF200CC90,0x42A0CA0D,0x3964C047,0x18B54A4D,0x5C236D33,0x47BD6515,0x6448D5FC,
                                  0xFD5A10CD,0x187140A1,0x6A0241CC,0x77193409,0x537593B8,0x06B7CC15,0x00001198};
    uint32_t std_modinv_even[]  ={0xC412AEFF,0x64D9BFAE,0xBC2FC3AF,0x43AA9554,0xC3A449EC,0x32D37A62,0xE19BFA10,0x78B762D3,
                                  0xEAAA0EC0,0x8BC4A88F,0x2DE63973,0x418339F2,0xC4E905F4,0x4F8751BF,0x84B8A818,0xA202F151,
                                  0xD18E6308,0xC9E1E25D,0x995533BF,0x0D9027CB,0xAD5A9610,0x846A8A73,0xC750CEE8,0x63344C4D,
                                  0x844CE336,0xF7A13C40,0xFC23E01D,0xAE406F0C,0x58CB796C,0x98876963,0xECE330DF,0x5303F05F,
                                  0x19C203DB,0xD5BF20F5,0xBFCDE7F8,0x7266B5E8,0x5822A13E,0x20DE6AA7,0x3A7D5C6C,0xADB134FE,
                                  0xAF882066,0x4BD9792E,0x8B8492D2,0xF79520F7,0x2811DA16,0x1D35DB9D,0x4E662679,0x76818270,
                                  0x420B6656,0x0FAF2919,0x954DBCD2,0x674325A8,0xE4B66041,0xB13A835E,0x336386D3,0x19718FEB,
                                  0xBB1BD311,0x3F009648,0x0302B950,0x5C98E657,0xA64CBAAB,0xB8407CCE,0x98B4DB6C,0x8851BA88,
                                  0xAA9BA0F1,0x044458A0,0x7974991D,0x765EFE0A,0xD77FA8ED,0xB3C05A67,0x918B3787,0x68BAD427,
                                  0xA1B98CC9,0x4A03477B,0x0901A55E,0xED78EDE2,0xB21D1386,0xA40EDD42,0x5FF26685,0x5876B45F,
                                  0x75F055BE,0xD7347ED5,0x31BFD6AB,0x9C1C7AB1,0x96985AA1,0x427B6263,0x000007E9};
    uint32_t std_modexp[]       ={0x9EBE64BA,0x50C2D453,0x8F76003C,0xDC2EC155,0xCCB94CF7,0x7CA50159,0x9D994FE8,0x76370BF5,
                                  0x112F54C7,0xA920F054,0x10CD2AFF,0xE21C2749,0x787976D5,0xBCCF030F,0x2B68FE89,0xE2A70E26,
                                  0x4460D04B,0x4ACAC2EF,0x448F9C71,0x2950C426,0x420D3F6D,0xFA21535B,0x867A2530,0x0EDEFCA6,
                                  0xB69BF5D1,0x0D481D82,0xD928A65E,0x065E1AC1,0x884C1AF6,0x37838D35,0x6B2E780A,0x9D15E0CD,
                                  0x99BEA9B2,0xC6510942,0xFBFF2605,0x3AA3DECE,0x100A69B5,0xC7F66CB8,0x7EB42E6E,0x065AF85E,
                                  0xA9102D4C,0x5E56B292,0x350286A4,0x43D09C27,0xF3C96A39,0x572B36AE,0xBB3DD396,0x2966DA3B,
                                  0xEC2464A0,0x18DF9C1A,0x134CA61C,0x8ED1E2A5,0x751103CB,0xE901DC84,0xAD84568E,0x604F0D71,
                                  0xC8510DD3,0x27893DAC,0xE8F985C2,0x9CEC51DF,0xDB62D804,0x9C6C0341,0x9115BE6C,0x864B35D6,
                                  0x0E662151,0x03034189,0xF725D121,0xBA374809,0xDBDAA8FB,0xAB2B0806,0xA975F487,0x6D602CC1,
                                  0x01A80562,0xBA8F8763,0x00538A8D,0x20DEFAB8,0x8A765FE1,0x53964393,0xD788588E,0x9D665DA8,
                                  0xFF636956,0xD3DC56AC,0x4A4111D2,0xA14C70A9,0x340A2BA4,0x06BCE541,0x0000239B};
    uint32_t std_modexp_d[]     ={0xC93EBE71,0xE4FA0FD9,0xE059F77D,0x0E81C660,0xEE836DDE,0xD5B5CE24,0xC0A5505D,0x35EBED71,
                                  0x2594E682,0xC59400C0,0x090FB384,0xBF6E15DF,0xDA25AA22,0x0D2C200A,0xD4064044,0x4E7960B1,
                                  0x0012C163,0xD91FD64A,0x1E232677,0x6A77B997,0x27E7298E,0xB4468DFD,0x403E18E6,0x03708FA9,
                                  0xBD25CCE9,0x02C3A2EC,0x6CEDD482,0xEE118BEA,0x62B299F8,0x123007BD,0x55F93910,0x83C5663C,
                                  0xB3569DD5,0xC4FDF000,0x174F8CBB,0x165105DE,0xA3C6A24A,0xF91AA2DE,0x09469947,0xACE1B2FC,
                                  0x07E8B201,0x53769ECD,0x2ADBC287,0x45419097,0x7EDD1A2C,0x8822653A,0x0D1220A4,0x195F9537,
                                  0xB2FF76D0,0x52FD3FA2,0x37518646,0xEE3A1DC5,0x086D9E6C,0x8BDD2AC1,0x634D874D,0x9331E6AB,
                                  0x9C2F3B95,0xBCCD9B78,0x573F4473,0xFA76C26E,0x5C0C0279,0xB2DD86E8,0xDB53C800,0xF0EFD7DE,
                                  0xF7FDC7A6,0xB5E56F4C,0x1B7A730C,0xB5AF8F57,0x594F5356,0x59AE8FD6,0xDD605F62,0xA4A3C09B,
                                  0xA09C2FD5,0xE3213D27,0x973B7C0F,0xC9372132,0x6EE91B5D,0x9972CFA0,0xEF4BA93E,0x96AA7983,
                                  0x1AD4D0DA,0x609797FF,0xB5B2EAA7,0x702BD334,0x76AB2AFA,0x3299EEE6,0x0000160D};

    vector->bitLen          = 2766;
    vector->a               = a;
    vector->b               = b;
    vector->p               = p;
    vector->q               = q;
    vector->fai_n           = fai_n;
    vector->d               = d;
    vector->n               = n;
    vector->std_n_h         = n_h;
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    vector->std_n_n0        = n_n0;
    n_10_01_h[1]            = 0x40;
    vector->std_10_01_n_h   = n_10_01_h;
    vector->std_10_01_n_n0  = n_10_01_n0;
    n_11_11_h[1]            = 0x10;
    vector->std_11_11_n_h   = n_11_11_h;
    vector->std_11_11_n_n0  = n_11_11_n0;
#else
    n_10_01_h[3] = 0x40;
    vector->std_10_01_n_h   = n_10_01_h;
    n_11_11_h[3] = 0x10;
    vector->std_11_11_n_h   = n_11_11_h;
#endif
    vector->std_a_add_b     = std_a_add_b;
    vector->std_a_sub_b     = std_a_sub_b;
    vector->std_b_sub_a     = std_b_sub_a;
    vector->std_modadd      = std_modadd;
    vector->std_modsub_a_b  = std_a_sub_b;
    vector->std_modsub_b_a  = std_modsub_b_a;
    vector->std_modmul      = std_modmul;
    vector->std_modinv_odd  = std_modinv_odd;
    vector->std_modinv_even = std_modinv_even;
    vector->std_modexp      = std_modexp;
    vector->std_modexp_d    = std_modexp_d;

    return pke_basic_test(vector);
}
#endif


#ifdef PKE_BASIC_3072_TEST
uint32_t pke_basic_3072_test(void)
{
    pke_basic_test_vector_t   vector[1];

    uint32_t a[]                ={0x7FA45DBB,0xB61F8A2A,0x827F55BA,0x1398C21E,0x4EF9C477,0xB6060D84,0x3847D613,0x426BFF87,
                                  0xC7875087,0x4A2AE93A,0x21AA6229,0x7DD5724A,0x906094BD,0x7BD94E30,0xA12402A1,0xE18D5721,
                                  0x0E6BD27B,0x5BC820BD,0x49B3C0BC,0x39B5C673,0x4A9F076D,0x7731E3AF,0xD2CC46B6,0x86D99344,
                                  0xA182D132,0x01520CC3,0xEF08910A,0xF5F594C6,0x49495A6A,0xE8C33BC0,0x4ED8E351,0x21C187F0,
                                  0xF8FF9905,0x446340DD,0x57273C52,0xDBF08EEC,0x2A0CDB2E,0xF1A97CAA,0xC6488899,0xC6B8854D,
                                  0x4DF9B79C,0xCEFFFAF5,0xAF4FD7A5,0xEFB25A83,0xB5055DE1,0xD5A766A1,0x8A284D8C,0xF4991156,
                                  0x7C1DF896,0xF7ECBDC6,0xD203CEF5,0xB67D2651,0x051442DC,0x76B58A96,0x01752AAA,0x046133F1,
                                  0xF78C86B3,0xB83D8C7A,0xBDF61E0E,0x56458ED1,0x75ADC6B2,0x63EAA864,0xC3E28A56,0xB377DCD0,
                                  0xDB43F332,0xB5AF3632,0x1FD29455,0x22CCC197,0xCAF4BA71,0xB56D6E46,0xE6F479C4,0xDA5B1AE8,
                                  0xE35ADAE4,0xF622E611,0x138E78C0,0xFE38816F,0x2B3FF36B,0x438ABD4F,0xF6FA6764,0x731F465F,
                                  0x4A0E9008,0xB1E09BCA,0x6AA390F7,0x18A69A6C,0x6D5CAFC0,0x71DB9566,0x086EE0DD,0xE4AA74AD,
                                  0x6C8361E3,0x5ED0D8AC,0x2C41DAA5,0x53D2D29C,0xC8E3A724,0xEB15A15D,0x3BBED056,0x364BC486};
    uint32_t b[]                ={0x2A71989F,0x6221E274,0x253D2B77,0xD74FBAA8,0x310E27A5,0xEADEC1CB,0x3F37856C,0x5D07B143,
                                  0x980F6022,0x8E96D7FF,0xCBE41810,0xB52F02B1,0x2CE6A01F,0x991932B6,0x502F8883,0x57C39F98,
                                  0xB97D33A0,0xCF48066D,0x2A38FA8C,0x1B9FE54C,0x73D5C0D1,0xE12C612A,0xBE64FA88,0x3F9F711B,
                                  0x4CC7609E,0xA4E0BA99,0xB563E8D9,0x30104E7C,0x61C80297,0xBEC673DF,0x10C0B439,0x5E4012FF,
                                  0x34A53759,0x49CA16E4,0xCB074ADD,0xC5D642C4,0x3900E269,0x4AB29C2A,0x2EE7107D,0x26D15B80,
                                  0x8AA4DA55,0x8E2405DE,0x0D483901,0x8B4E4E9B,0xC15F481D,0x15C22A2C,0xB316AB16,0x087D6359,
                                  0x6B3E0C5D,0xF1A7A4E1,0x589490DE,0x3F8BB667,0xA2CF0F6D,0x910A5E29,0x6E6BAE47,0x50ED6FEF,
                                  0xA3333A24,0xC1B35E6E,0xDA49D21B,0x1D41C731,0x5B2B9CD5,0x1BA97D95,0x8FEBB81C,0x7CAF4495,
                                  0x57885AF9,0x5F45705C,0xC4F46038,0x135060A8,0xE9890A5F,0xD1D093D8,0x527F1893,0x383D03EE,
                                  0x9A697314,0x1F3BB0FC,0x7D709BE5,0x64A025DA,0xF7402E49,0xA7F1CA36,0x479D5BEB,0x0FB0852A,
                                  0xC931FD99,0x011EB0E6,0xC3B5FCF1,0xD4D04A2E,0x19267B48,0xE49A89AD,0xA42B25A1,0x10154566,
                                  0xC17B396A,0xFD44CCE4,0x392AE5CA,0xAE7B26C6,0xBB48F53E,0x682B2E4F,0x958FE023,0xB57E36C1};
    uint32_t std_a_add_b[]      ={0xAA15F65A,0x18416C9E,0xA7BC8132,0xEAE87CC6,0x8007EC1C,0xA0E4CF4F,0x777F5B80,0x9F73B0CA,
                                  0x5F96B0A9,0xD8C1C13A,0xED8E7A39,0x330474FB,0xBD4734DD,0x14F280E6,0xF1538B25,0x3950F6B9,
                                  0xC7E9061C,0x2B10272A,0x73ECBB49,0x5555ABBF,0xBE74C83E,0x585E44D9,0x9131413F,0xC6790460,
                                  0xEE4A31D0,0xA632C75C,0xA46C79E3,0x2605E343,0xAB115D02,0xA789AF9F,0x5F99978B,0x80019AEF,
                                  0x2DA4D05E,0x8E2D57C2,0x222E872F,0xA1C6D1B1,0x630DBD98,0x3C5C18D4,0xF52F9917,0xED89E0CD,
                                  0xD89E91F1,0x5D2400D3,0xBC9810A7,0x7B00A91E,0x7664A5FF,0xEB6990CE,0x3D3EF8A2,0xFD1674B0,
                                  0xE75C04F3,0xE99462A7,0x2A985FD4,0xF608DCB9,0xA7E35249,0x07BFE8BF,0x6FE0D8F2,0x554EA3E0,
                                  0x9ABFC0D7,0x79F0EAE9,0x983FF02A,0x73875603,0xD0D96387,0x7F9425F9,0x53CE4272,0x30272166,
                                  0x32CC4E2C,0x14F4A68F,0xE4C6F48E,0x361D223F,0xB47DC4D0,0x873E021F,0x39739258,0x12981ED7,
                                  0x7DC44DF9,0x155E970E,0x90FF14A6,0x62D8A749,0x228021B5,0xEB7C8786,0x3E97C34F,0x82CFCB8A,
                                  0x13408DA1,0xB2FF4CB1,0x2E598DE8,0xED76E49B,0x86832B08,0x56761F13,0xAC9A067F,0xF4BFBA13,
                                  0x2DFE9B4D,0x5C15A591,0x656CC070,0x024DF962,0x842C9C63,0x5340CFAD,0xD14EB07A,0xEBC9FB47};
    uint32_t std_a_sub_b[]      ={0x5532C51C,0x53FDA7B6,0x5D422A43,0x3C490776,0x1DEB9CD1,0xCB274BB9,0xF91050A6,0xE5644E43,
                                  0x2F77F064,0xBB94113B,0x55C64A18,0xC8A66F98,0x6379F49D,0xE2C01B7A,0x50F47A1D,0x89C9B789,
                                  0x54EE9EDB,0x8C801A4F,0x1F7AC62F,0x1E15E127,0xD6C9469C,0x96058284,0x14674C2D,0x473A2229,
                                  0x54BB7094,0x5C71522A,0x39A4A830,0xC5E5464A,0xE78157D3,0x29FCC7E0,0x3E182F18,0xC38174F1,
                                  0xC45A61AB,0xFA9929F9,0x8C1FF174,0x161A4C27,0xF10BF8C5,0xA6F6E07F,0x9761781C,0x9FE729CD,
                                  0xC354DD47,0x40DBF516,0xA2079EA4,0x64640BE8,0xF3A615C4,0xBFE53C74,0xD711A276,0xEC1BADFC,
                                  0x10DFEC39,0x064518E5,0x796F3E17,0x76F16FEA,0x6245336F,0xE5AB2C6C,0x93097C62,0xB373C401,
                                  0x54594C8E,0xF68A2E0C,0xE3AC4BF2,0x3903C79F,0x1A8229DD,0x48412ACF,0x33F6D23A,0x36C8983B,
                                  0x83BB9839,0x5669C5D6,0x5ADE341D,0x0F7C60EE,0xE16BB012,0xE39CDA6D,0x94756130,0xA21E16FA,
                                  0x48F167D0,0xD6E73515,0x961DDCDB,0x99985B94,0x33FFC522,0x9B98F318,0xAF5D0B78,0x636EC135,
                                  0x80DC926F,0xB0C1EAE3,0xA6ED9406,0x43D6503D,0x54363477,0x8D410BB9,0x6443BB3B,0xD4952F46,
                                  0xAB082879,0x618C0BC7,0xF316F4DA,0xA557ABD5,0x0D9AB1E5,0x82EA730E,0xA62EF033,0x80CD8DC4};
    uint32_t std_b_sub_a[]      ={0xAACD3AE4,0xAC025849,0xA2BDD5BC,0xC3B6F889,0xE214632E,0x34D8B446,0x06EFAF59,0x1A9BB1BC,
                                  0xD0880F9B,0x446BEEC4,0xAA39B5E7,0x37599067,0x9C860B62,0x1D3FE485,0xAF0B85E2,0x76364876,
                                  0xAB116124,0x737FE5B0,0xE08539D0,0xE1EA1ED8,0x2936B963,0x69FA7D7B,0xEB98B3D2,0xB8C5DDD6,
                                  0xAB448F6B,0xA38EADD5,0xC65B57CF,0x3A1AB9B5,0x187EA82C,0xD603381F,0xC1E7D0E7,0x3C7E8B0E,
                                  0x3BA59E54,0x0566D606,0x73E00E8B,0xE9E5B3D8,0x0EF4073A,0x59091F80,0x689E87E3,0x6018D632,
                                  0x3CAB22B8,0xBF240AE9,0x5DF8615B,0x9B9BF417,0x0C59EA3B,0x401AC38B,0x28EE5D89,0x13E45203,
                                  0xEF2013C6,0xF9BAE71A,0x8690C1E8,0x890E9015,0x9DBACC90,0x1A54D393,0x6CF6839D,0x4C8C3BFE,
                                  0xABA6B371,0x0975D1F3,0x1C53B40D,0xC6FC3860,0xE57DD622,0xB7BED530,0xCC092DC5,0xC93767C4,
                                  0x7C4467C6,0xA9963A29,0xA521CBE2,0xF0839F11,0x1E944FED,0x1C632592,0x6B8A9ECF,0x5DE1E905,
                                  0xB70E982F,0x2918CAEA,0x69E22324,0x6667A46B,0xCC003ADD,0x64670CE7,0x50A2F487,0x9C913ECA,
                                  0x7F236D90,0x4F3E151C,0x59126BF9,0xBC29AFC2,0xABC9CB88,0x72BEF446,0x9BBC44C4,0x2B6AD0B9,
                                  0x54F7D786,0x9E73F438,0x0CE90B25,0x5AA8542A,0xF2654E1A,0x7D158CF1,0x59D10FCC,0x7F32723B};
    uint32_t p[]                ={0x5E87642D,0x0CC5EDE3,0x641F5CAE,0x57D69653,0xD938E8E3,0xFA84A42A,0xD04B327D,0x2A664635,
                                  0xDEEEFEA2,0xEB0B91BF,0x344AD45B,0xDDC9497D,0xEE387BBB,0x6C059638,0xBE16EF19,0xC27768FF,
                                  0x98A5D99E,0x18BB23BA,0xB3029788,0x90CE4248,0x11129DC9,0xB6BB5297,0x085276DF,0x7102D7E6,
                                  0xF48B64C8,0x271FFA08,0x924C7028,0x239C5E72,0xA59A7E0F,0xE4367DE2,0x61D62EC0,0x426676D6,
                                  0x122A08F3,0x43CA0134,0x68900DB9,0x11F00580,0x3BD1829C,0x35CEAE10,0xFB62C848,0xDB82D687,
                                  0x5AB1B4F7,0xB79AC21D,0x4BA16346,0x47913ED9,0xF2D4F698,0xB3A31063,0x6E9DBF8E,0xF0BFEA7A};
    uint32_t q[]                ={0x9EA92665,0x79E4040B,0x7FDBCA7A,0x9B4C2350,0xB47310D5,0x2C3F76F2,0x18C3BA1A,0xC3D58C06,
                                  0x8D58A2A6,0xAE136572,0x51F383D8,0x73D00A5F,0x34A2CF56,0xDB129224,0x45CBFD1C,0xFD397D46,
                                  0xB613EDF9,0x46F7E377,0xC4057F62,0x6889D088,0x4BAA644A,0x0B125222,0xC29D0276,0x2A9779E9,
                                  0x4FE6B936,0xDA773092,0x5F53D39A,0x9CEF66EA,0xAC66BCAA,0xF47F3B69,0x699C19CE,0x10A54BD2,
                                  0xFD327433,0x0DEBE2D4,0xED2156E0,0xD3AA4399,0x20707CB6,0xC87D3D15,0xB4B115B6,0x00B6B0E6,
                                  0x6A9A9119,0xD10A11DD,0x3DBCF56B,0x71894FD5,0x3735E5FD,0x3C1A6ED0,0x144C4180,0xEBCE6923};
    uint32_t fai_n[]            ={0x4ECDA930,0xEEA3A1F1,0xC0660B83,0x78554FF3,0x435A15F6,0xCC860C03,0xDCBE128E,0x5F4E07DA,
                                  0xBB4DCB69,0x84FC0E8A,0x9FAC0F65,0xFC2908A0,0x25E99D70,0xF92C8C6E,0x7E8E9EE9,0x1B95480D,
                                  0x834AF58B,0xC9C72227,0x1DDF22C9,0x46310DDF,0xD5280CF3,0x298FFED9,0x4C3F245F,0xE9109C60,
                                  0x3C5E7002,0xC0CF6F11,0xC9664DDB,0x7E80B5F2,0x6EF76197,0x1C408E1D,0xCA47267E,0x7A570910,
                                  0x93661D7F,0x7043235D,0xE84788C7,0xC70F7ECC,0x451EDE73,0xA23470F2,0x3EEFB3F4,0x994A0048,
                                  0x45E5592E,0x994B7ED5,0x7B942272,0x3E05B4FC,0xCB50D480,0x1406D866,0x50F2BC52,0x3A99818A,
                                  0xBD6EE749,0x24D30A04,0x3497CFCD,0xEDB14AAA,0xAA67CC9A,0x0F101093,0x74DF7400,0x2A5059BF,
                                  0x5B74140D,0x5D2FA7DE,0x8C6E2C99,0x468614AE,0xA3022255,0x3E85748E,0x7D513C5F,0x83736948,
                                  0x66A75FDF,0x29CF81A1,0x41C94483,0x1C4BBBCD,0x52CE3D5B,0x4D9D78D5,0xD70E71D4,0x0B7F0262,
                                  0x8839A22E,0x8B8A2D87,0x80111BAF,0x56BA66D2,0x938F6BF6,0x08840699,0x6D6A7806,0x4344AB32,
                                  0x8EC771D8,0xA6AB93CB,0x7723D085,0x088043FA,0xF7C0AA83,0x303A8ADB,0xA4ECF05B,0x1D394F86,
                                  0x488518EE,0x17A3752A,0x243E71A2,0x1734B371,0x8B3B75F9,0x76DF4935,0xC68FCCC8,0xDDC2498C};
    uint32_t d[]                ={0xD2B2F641,0x90CBF95E,0x8D936173,0xC2E25CCB,0x5A969E43,0x5CCBD421,0xC9BB2313,0xD63CC4B9,
                                  0xF1B9C8B4,0x775B1A21,0xDAC53FA7,0x456A7CEE,0x80894E4C,0x70C1800F,0x9784D3BB,0x02219E34,
                                  0x2896AB08,0xA6182928,0x0E0BF6D6,0x780B32E4,0x38116C34,0x731396C0,0x9D68A53F,0xBDFF044F,
                                  0xCB1CD83A,0xCDB9BFA7,0x5071BFD9,0xA47B9B9B,0xBDC50AAA,0x98EB8C2F,0x281070AB,0x5AC23F19,
                                  0x858ADBEC,0x675EC0AA,0x350E1A98,0x0C013902,0x334D51B7,0x2095928A,0x2F1A2F17,0x43E3D2CE,
                                  0x0F7ED258,0x33BE161C,0xCCE967AF,0x20ED5C1F,0x237FC6DF,0xE9818587,0xD821CD6A,0x3659C254,
                                  0x7E3CE99F,0x5A62C80C,0xC422904A,0x98AF5D0F,0xF33A5C61,0xBD135CFB,0x1B12BF04,0x31974A38,
                                  0xA70B507E,0xE9B235CA,0xBC2B49CB,0x962559BD,0x60B279F6,0x797A95D7,0xC618CFAC,0x7486D9E5,
                                  0x3555F5F8,0x1A26EE22,0xBFDF660F,0xCCBF520E,0xB92EB6BC,0x5D172EC9,0x47E2E52A,0xD1FD2C61,
                                  0xE7FDBE65,0x6FCF8636,0x759846EE,0x5C335613,0x99EBF1F7,0x6441FD3A,0x047A13DC,0xC9ECFCB8,
                                  0x2506A635,0xDB8DAFA1,0xA1903400,0x3FC41CD0,0xD834EDB3,0x03BA3D7B,0x7A4973C3,0x0A9BD56A,
                                  0xF8BD4503,0xDBC1F674,0xBD5415D0,0xE5F1FE91,0xAC2115DB,0x92163DA5,0xCAE4F2BB,0xB20D2918};
    uint32_t n[]                ={0x4BFE33C1,0x754D93E0,0xA46132AC,0x6B780997,0xD1060FAF,0xF34A2720,0xC5CCFF26,0x4D89DA16,
                                  0x27956CB2,0x1E1B05BD,0x25EA679A,0x4DC25C7D,0x48C4E883,0x4044B4CB,0x82718B20,0xDB462E53,
                                  0xD204BD23,0x297A2959,0x94E739B4,0x3F8920B0,0x31E50F07,0xEB5DA393,0x172E9DB4,0x84AAEE30,
                                  0x80D08E01,0xC26699AC,0xBB06919E,0x3F0C7B4F,0xC0F89C51,0xF4F64769,0x95B96F0D,0xCD62CBB9,
                                  0xA2C29AA5,0xC1F90766,0x3DF8ED60,0xACA9C7E7,0xA160DDC6,0xA0805C17,0xEF0391F3,0x758387B6,
                                  0x0B319F3F,0x21F052D0,0x04F27B25,0xF72043AB,0xF55BB115,0x03C4579A,0xD3DCBD61,0x1727D527,
                                  0xBD6EE74B,0x24D30A04,0x3497CFCD,0xEDB14AAA,0xAA67CC9A,0x0F101093,0x74DF7400,0x2A5059BF,
                                  0x5B74140D,0x5D2FA7DE,0x8C6E2C99,0x468614AE,0xA3022255,0x3E85748E,0x7D513C5F,0x83736948,
                                  0x66A75FDF,0x29CF81A1,0x41C94483,0x1C4BBBCD,0x52CE3D5B,0x4D9D78D5,0xD70E71D4,0x0B7F0262,
                                  0x8839A22E,0x8B8A2D87,0x80111BAF,0x56BA66D2,0x938F6BF6,0x08840699,0x6D6A7806,0x4344AB32,
                                  0x8EC771D8,0xA6AB93CB,0x7723D085,0x088043FA,0xF7C0AA83,0x303A8ADB,0xA4ECF05B,0x1D394F86,
                                  0x488518EE,0x17A3752A,0x243E71A2,0x1734B371,0x8B3B75F9,0x76DF4935,0xC68FCCC8,0xDDC2498C};
    uint32_t n_h[]              ={0x95770C4D,0xA6BDDC34,0x3D94422A,0xB6FA1744,0x0ABFE701,0x77AC9DD6,0x6E4C95DF,0x065FEA54,
                                  0xF0D14C1D,0x897CF1D8,0x9B21CE2D,0xFCD0CE75,0xEB7CBC45,0xE29EED92,0x17114425,0x3EAAEC9F,
                                  0xBF995A6E,0x1EA3EFCE,0x1E125079,0x08916C84,0x7E38CC3F,0x0199D915,0xC8D9EDBA,0xEAD5DFEE,
                                  0x34D2F709,0x68647684,0x0760F518,0x5DE30EF9,0x5685AE11,0x8099D33B,0xE17FBB2B,0xB29B87A1,
                                  0x17F76272,0x1D232F1C,0xE1B02463,0x193F35EA,0x6F572825,0x68720561,0x1318795B,0x3DFC671C,
                                  0x16B503FC,0x9489E406,0x7EC5CBB1,0x762B1DB2,0x9832B326,0xDFA8EB00,0xC8B14F3D,0xB59DC0D3,
                                  0x9BB8032C,0xF4DD0821,0xFF20CFAB,0x16490139,0xC6303E7B,0xA50FE002,0x2C4F3F88,0xCC61C5CE,
                                  0x28126FF8,0x25FB8627,0xDC9647A8,0xF8802E0D,0x070474A3,0xF2155EC3,0x8E6BA76D,0x36F67CC9,
                                  0xE4862CDF,0x693D53F6,0xA517C988,0x94FDB0E8,0x88B431FE,0x96D51D9C,0x3D934B56,0xBDCE2C2E,
                                  0x78A0EB95,0xF16A6F9D,0xF70D5DD4,0x4B998821,0x9272F194,0x3EDE5B1F,0xB8B306BA,0xA2BB8067,
                                  0x3BCAA376,0x04A90C3F,0x79D153C5,0x5295BC1C,0x3CBD0563,0xD8228A37,0x2C10D15B,0x8C31A5C9,
                                  0xF3FA8233,0x0D6FBA14,0x4B35ED84,0xC4A011F8,0xB5128EBA,0x72467BD3,0xD31EFE8B,0xC86E9A2B};
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    uint32_t n_n0[]             ={0x0D4423BF,};
#endif
    uint32_t n_10_01_h[(3072+31)/32] ={0};
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    uint32_t n_10_01_n0[]            ={0xFFFFFFFF};
#endif
    uint32_t n_11_11_h[(3072+31)/32] ={0};
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    uint32_t n_11_11_n0[]            ={1};
#endif
    uint32_t std_modadd[]       ={0x5E17C299,0xA2F3D8BE,0x035B4E85,0x7F70732F,0xAF01DC6D,0xAD9AA82E,0xB1B25C59,0x51E9D6B3,
                                  0x380143F7,0xBAA6BB7D,0xC7A4129F,0xE542187E,0x74824C59,0xD4ADCC1B,0x6EE20004,0x5E0AC866,
                                  0xF5E448F8,0x0195FDD0,0xDF058195,0x15CC8B0E,0x8C8FB937,0x6D00A146,0x7A02A38A,0x41CE1630,
                                  0x6D79A3CF,0xE3CC2DB0,0xE965E844,0xE6F967F3,0xEA18C0B0,0xB2936835,0xC9E0287D,0xB29ECF35,
                                  0x8AE235B8,0xCC34505B,0xE43599CE,0xF51D09C9,0xC1ACDFD1,0x9BDBBCBC,0x062C0723,0x78065917,
                                  0xCD6CF2B2,0x3B33AE03,0xB7A59582,0x83E06573,0x8108F4E9,0xE7A53933,0x69623B41,0xE5EE9F88,
                                  0x29ED1DA8,0xC4C158A3,0xF6009007,0x0857920E,0xFD7B85AF,0xF8AFD82B,0xFB0164F1,0x2AFE4A20,
                                  0x3F4BACCA,0x1CC1430B,0x0BD1C391,0x2D014155,0x2DD74132,0x410EB16B,0xD67D0613,0xACB3B81D,
                                  0xCC24EE4C,0xEB2524ED,0xA2FDB00A,0x19D16672,0x61AF8775,0x39A0894A,0x62652084,0x07191C74,
                                  0xF58AABCB,0x89D46986,0x10EDF8F6,0x0C1E4077,0x8EF0B5BF,0xE2F880EC,0xD12D4B49,0x3F8B2057,
                                  0x84791BC9,0x0C53B8E5,0xB735BD63,0xE4F6A0A0,0x8EC28085,0x263B9437,0x07AD1624,0xD7866A8D,
                                  0xE579825F,0x44723066,0x412E4ECE,0xEB1945F1,0xF8F12669,0xDC618677,0x0ABEE3B1,0x0E07B1BB,};
    uint32_t std_modsub_a_b[]   ={0xA130F8DD,0xC94B3B96,0x01A35CEF,0xA7C1110E,0xEEF1AC80,0xBE7172D9,0xBEDD4FCD,0x32EE285A,
                                  0x570D5D17,0xD9AF16F8,0x7BB0B1B2,0x1668CC15,0xAC3EDD21,0x2304D045,0xD366053E,0x650FE5DC,
                                  0x26F35BFF,0xB5FA43A9,0xB461FFE3,0x5D9F01D7,0x08AE55A3,0x81632618,0x2B95E9E2,0xCBE51059,
                                  0xD58BFE95,0x1ED7EBD6,0xF4AB39CF,0x04F1C199,0xA879F425,0x1EF30F4A,0xD3D19E26,0x90E440AA,
                                  0x671CFC51,0xBC923160,0xCA18DED5,0xC2C4140E,0x926CD68B,0x47773C97,0x86650A10,0x156AB184,
                                  0xCE867C87,0x62CC47E6,0xA6FA19C9,0x5B844F93,0xE901C6DA,0xC3A9940F,0xAAEE5FD7,0x03438324,
                                  0xCE4ED385,0x2B1822E9,0xAE070DE4,0x64A2BA94,0x0CAD000A,0xF4BB3D00,0x07E8F062,0xDDC41DC1,
                                  0xAFCD609B,0x53B9D5EA,0x701A788C,0x7F89DC4E,0xBD844C32,0x86C69F5D,0xB1480E99,0xBA3C0183,
                                  0xEA62F818,0x80394777,0x9CA778A0,0x2BC81CBB,0x3439ED6D,0x313A5343,0x6B83D305,0xAD9D195D,
                                  0xD12B09FE,0x6271629C,0x162EF88B,0xF052C267,0xC78F3118,0xA41CF9B1,0x1CC7837E,0xA6B36C68,
                                  0x0FA40447,0x576D7EAF,0x1E11648C,0x4C569438,0x4BF6DEFA,0xBD7B9695,0x0930AB96,0xF1CE7ECD,
                                  0xF38D4167,0x792F80F1,0x1755667C,0xBC8C5F47,0x98D627DE,0xF9C9BC43,0x6CBEBCFB,0x5E8FD751,};
    uint32_t std_modmul[]       ={0x033146C2,0xD36FC019,0x7F860C63,0x843C6F01,0xD0D04955,0xACFA9FA1,0xB56457D2,0x46D4AD39,
                                  0xE900AAFC,0xD490CCF1,0xA23101EF,0xB2A20A2D,0x3762A6B2,0x9D19B2D1,0x8FC0C47C,0x7D3C1602,
                                  0x9A436812,0x18C0AEFC,0x6B2CB585,0xE4348EE0,0x13F91B98,0x3EAF27EE,0x5B94B7D8,0xC7C44C98,
                                  0x9AEF7603,0xC9FF1183,0x3DEA8CDB,0x97EF84D0,0x5FCED976,0x11B4A6C8,0xE834A7D2,0x473C8C37,
                                  0x7852FBDD,0x44EF68B4,0x556D3149,0xED8B120A,0x7EBB2970,0xB7492D96,0xE1EDEDFA,0xFB8C7FF4,
                                  0x0DE0E1F6,0x16FCCE8B,0x3EA6BBC7,0x041E8606,0xEB9F56ED,0x30C644A9,0x154B4E03,0x9A4494B0,
                                  0x36A76A31,0x6E2FA642,0x02920A14,0x14CB9509,0xEA1CA71C,0xB5842FF9,0xD130BC6C,0x0944E9BE,
                                  0x8C22AF48,0x17C7F547,0xF098E620,0xF7B44AAC,0x54BACEEC,0x635485A1,0x2882A931,0x1E488682,
                                  0x0DDF5CA7,0xFCA7F76D,0xCF51CD7A,0xD0D6723D,0x3FCD2AAA,0x5816B80F,0x9CF9BFCA,0xE9341243,
                                  0x9DD33F07,0x298040E2,0x19F887E3,0x426FE5EC,0x78BA71E3,0x2A8E634B,0xCEE56D3D,0x1B46F38A,
                                  0x4E1EEFD1,0x94D2BA37,0xDAD691E1,0xE386982D,0x6E954C60,0xF0FE9E6D,0x7C8745A8,0x358F1519,
                                  0xC645E5FF,0x480ACB54,0x06A6CE8E,0xF6754368,0x79C196C8,0x9A40A253,0x72D243BA,0xB8C7567F};
    uint32_t std_modinv_odd[]   ={0xC3F36B7C,0xDC907AA9,0x3A9C0732,0x921E2C65,0xC6662D20,0x946EAAAD,0x3013E908,0x99990995,
                                  0xC608BA00,0xF8096E44,0xFD53F682,0x4ACE5CFC,0x16B882F7,0x37B298F9,0xEFDF4F3A,0x13BAFE56,
                                  0x44844240,0x07BC1BD1,0x8EBC590B,0xA3AC6678,0xDA50C276,0xF5476268,0x23C72795,0x211E0BFB,
                                  0x8AC632B5,0x5665CF10,0x5A87E9C0,0xD1292702,0x9B4B135B,0x5BED2C9A,0xFAB1BD9A,0x609D7153,
                                  0x7E2252F4,0x8DA1906C,0xE2FACDCE,0xF60A36FF,0xEBF5633D,0x0DAE69A7,0x12E79196,0xF2095541,
                                  0xEE458895,0x59969688,0xE961122C,0x4BA0A0CE,0x12913E59,0xED358BA1,0x9EBB820C,0x0B6E04E7,
                                  0xF89C1093,0x9F315B47,0x886523ED,0xF3A859B6,0xEE69C24D,0xAD6B44AA,0xB868F18D,0x0B4534A3,
                                  0x698C7D55,0xD1E2A358,0xF0F97644,0x13FC66A4,0x3443601C,0x19924A06,0x03A089DA,0x65F136F6,
                                  0x7A0E4539,0x041AD6AC,0xB6AB72B4,0x183D1C51,0x0D412B7A,0x858FA95D,0xCB6608B0,0xE2786DE7,
                                  0x87CAC330,0x8B2612F5,0xD1C064EF,0xA182C5A1,0x4CA4CE7C,0x0513181F,0x947A7736,0x6F4A6181,
                                  0x68FD37B9,0x6A587BF2,0xFA31A8D1,0xED888032,0xA37D00A8,0xB7B8611E,0x962D4BFE,0xDEF3CA3B,
                                  0x55271EC9,0x9CBD2CB9,0x1E2D6DDF,0x7A1070AC,0x1EC57BCA,0x76A94396,0x224CB287,0x91690AD9};
    uint32_t std_modinv_even[]  ={0x3D34F433,0xB9B4F9A4,0xA89F3AA5,0x40C8227A,0xE9993824,0x40303711,0x112475D6,0x34E48A55,
                                  0x86F9AA36,0x2E15E305,0xC7429581,0xCDE79CE9,0x48D9EAE8,0x93F76F24,0x252B34DD,0xC61B15B1,
                                  0x470F05C0,0x2E0C93BC,0xF7CF8D0D,0xA71F17D1,0x65C17169,0x10EE69F2,0x5D65F22C,0xCF7D831A,
                                  0x4CCABFC4,0xDAEC86BB,0x1C837801,0x47820729,0x3E67A75E,0x3AB98B0F,0x494D2208,0x82B8FA92,
                                  0xCE9CEB20,0x1578029B,0x3CDA6636,0x32169346,0x5F17E60B,0xF441A54C,0x7A1AC638,0x90506FBD,
                                  0xB284C08E,0x97449B08,0x73FFAF86,0x0DF22C27,0x9C36494F,0x97F364AA,0x820E0DFE,0x6CA56A57,
                                  0x5DC6B209,0xE77085FD,0x203E5CF3,0x0EA2EE09,0x67029EA9,0x1DDCA770,0x98C79464,0x297D4C59,
                                  0xB90DF5C7,0xFB8082B7,0x8EE210E4,0xE583344F,0xD2480A54,0x1F480CD5,0xEDDCC909,0x28E7B869,
                                  0x09B8C766,0x22C472DF,0x3E5BAE30,0x492EE8FC,0x84DE9CAE,0x561E1B84,0xE8DDB8B7,0xC4C0DFBE,
                                  0xB927C33D,0x692C4331,0x865B919F,0x1D28853A,0x0BF33124,0x13B5FA4F,0x304EBCDB,0xB1D41490,
                                  0x5CCC8B9E,0x01B9DE45,0x86019573,0x3D01466D,0xCC9E2EC1,0xDDED13E4,0x15FB3F7A,0xEBD7D23B,
                                  0xE051B54F,0x2A1F5D98,0x76C028DA,0x9110326E,0x087F25CC,0xAFFB3B69,0x29C578E9,0x85E0572D};
    uint32_t std_modexp[]       ={0xE1A09143,0x7CD45685,0x3F904A64,0x9DBF5529,0xC2B537CF,0x3B92BB9B,0x7F3C5698,0x5B05C5D3,
                                  0xBCC371C8,0x5635836E,0xA1698E8E,0xE3C48AB7,0xA9BCCD33,0x6A078FD3,0x958694CE,0xB8A370E7,
                                  0x3FFCB445,0xBABD68D7,0xD14E1D83,0x94C2975A,0xFEBAE742,0x3A13061E,0xE61F2510,0xE188EE1D,
                                  0x0A877B34,0x1F5706DB,0x6F3F8392,0x22B6C50E,0xA38653E6,0xE9CDF70A,0x07FF4CD9,0x84FB8FF0,
                                  0xCBC45DDD,0xDA1B8924,0x9C501B24,0x7FBFDBBD,0xAA2BBE4C,0x3CE6285B,0x5C671A72,0xFD87E719,
                                  0xDC8ACA6D,0x95D2D30F,0x8F02C882,0x97E5E2DD,0x3615A754,0xBC964222,0x4D9E7CD7,0x9DC85D67,
                                  0xB7056ACC,0x8D51EE81,0x270BDE42,0xB67384E7,0x118BB79A,0xB54A48FF,0x2E32E6CD,0x320C3836,
                                  0x4707F670,0xEFA7C33D,0x3670BC6E,0x33F7E364,0x05ECF9F9,0x416E4230,0x1EBAB493,0xEBAFA47D,
                                  0x25B01AF8,0x347DAB80,0x21015008,0x783439EC,0xC6FBC0BF,0x491D3791,0x70066CA9,0x950804F0,
                                  0x0178547B,0xFE48612D,0xCA59B796,0xDBC8ADAD,0x5896BBAB,0xF89D8932,0x47E725AA,0x14043BED,
                                  0x36BFE23E,0xD905F7CC,0xA436F8A0,0x6DC488E9,0x4E960CE8,0xF2CC34EB,0xD975AF03,0xD4ACCB96,
                                  0x42892BBB,0x85D34DAB,0x27B80323,0xA9101747,0x96919953,0x0CABE890,0xDE4EA219,0x9E331D8D};
    uint32_t std_modexp_d[]     ={0x12C4FCE4,0x76A241AC,0x14F8C65C,0x6B5A7CA3,0x14C42E32,0x2A905212,0xB105DFF1,0x7B0A6668,
                                  0xC67A64D5,0x1951EFC3,0x8B34FDAB,0xDD5BC182,0xEC74D5F5,0xB8EA58B5,0xF4634834,0x5E6CB20F,
                                  0x8354258F,0xB07CEB01,0xE4C31296,0x4F496EAA,0x719B1AAD,0xC214224F,0x845A4791,0xDB737EA6,
                                  0x9026BEA8,0x089788C0,0xEA7785ED,0xB1D0AB59,0xA300C506,0x8347FBE7,0x49F4967E,0x159C44A8,
                                  0x257BC219,0x94340F2B,0x6D219EBA,0xA87340F3,0x6C19B96E,0x61B2DEDB,0x9EC9E726,0x064E845C,
                                  0xF3F52B21,0x8C191590,0xB45A51BD,0x5DF9C501,0xD7AE49FB,0xBABBD99D,0xDEB9B1F5,0xCC98EE8C,
                                  0xC99464CA,0x917FFD35,0x1F97EABC,0xBE0733A5,0xE4932F29,0x17DEA1D3,0xAB06B7E5,0x1BD143AB,
                                  0xDBF007BB,0xE2768C40,0x6DDACFA7,0x54AD5EF4,0x626BE956,0xD8083EF5,0xE5C21D3A,0x01063EED,
                                  0xB131C7FB,0x3AD3641F,0x7625CA01,0x03BF2E92,0x2D6424AF,0xBF44D2DC,0x7E58B713,0x328AA22E,
                                  0x1E0C9392,0x0A1DEDE6,0x7BD3547E,0xC221A634,0xD87F7435,0xAD41E1DB,0x24542B91,0xE413DA90,
                                  0x77A5D6FA,0xDC3725B6,0x90C23A53,0xE405A965,0x3007352F,0x89197397,0xF9BB60DD,0x4DE7E240,
                                  0x3697E2A1,0xA99357D1,0xE8BCA3B8,0x2F24E5D9,0x2371F319,0x35C4F03C,0x699D9BAC,0xBE8CD925};

    vector->bitLen          = 3072;
    vector->a               = a;
    vector->b               = b;
    vector->p               = p;
    vector->q               = q;
    vector->fai_n           = fai_n;
    vector->d               = d;
    vector->n               = n;
    vector->std_n_h         = n_h;
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    vector->std_n_n0        = n_n0;
#endif
    n_10_01_h[0]            = 4;
    vector->std_10_01_n_h   = n_10_01_h;
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    vector->std_10_01_n_n0  = n_10_01_n0;
#endif
    n_11_11_h[0]            = 1;
    vector->std_11_11_n_h   = n_11_11_h;
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    vector->std_11_11_n_n0  = n_11_11_n0;
#endif
    vector->std_a_add_b     = std_a_add_b;
    vector->std_a_sub_b     = std_a_sub_b;
    vector->std_b_sub_a     = std_b_sub_a;
    vector->std_modadd      = std_modadd;
    vector->std_modsub_a_b  = std_modsub_a_b;
    vector->std_modsub_b_a  = std_b_sub_a;
    vector->std_modmul      = std_modmul;
    vector->std_modinv_odd  = std_modinv_odd;
    vector->std_modinv_even = std_modinv_even;
    vector->std_modexp      = std_modexp;
    vector->std_modexp_d    = std_modexp_d;

    return pke_basic_test(vector);
}
#endif


#ifdef PKE_BASIC_4096_TEST
#if (OPERAND_MAX_WORD_LEN >= 128)
uint32_t pke_basic_4096_test(void)
{
    pke_basic_test_vector_t   vector[1];

    uint32_t a[]                ={0x97B64BDF,0x699C57F0,0x38F29159,0xFA5B47E5,0x3D0E6B76,0xE1205B81,0x0FFD14B8,0xBD1A9656,
                                  0x26E7523B,0x0616C064,0xE1650E17,0xDAA1B473,0xFA1F8705,0xC38C4490,0x88B787D3,0x40BDCB40,
                                  0x92E0B827,0xE942728B,0x4C2C25D8,0x1A37EB21,0x897EA9EB,0x4C866854,0x679AA03E,0x1948CE2E,
                                  0x65A0526B,0x5DE13FCC,0x40CCF216,0xA6D4280B,0x697EE049,0xE61DE83D,0xD6F52672,0xE78EC78E,
                                  0xAF01D311,0x5BDF3B69,0xA8ECC6D9,0xC5B284DB,0x68B671F1,0xF121A1FF,0xA109A889,0x3CD41B8C,
                                  0xFA1A8A56,0xC299994E,0x851C30A1,0xCF031A61,0x8BC25E97,0xFAA9BDC7,0x0E55CF32,0xCE03D63E,
                                  0x8DCC57B8,0x48B65244,0x48E9A0C8,0x378B6ED2,0x840E50B7,0x9F8C63C3,0x8391D61E,0xF43D9D9E,
                                  0xB19BE04F,0x4B6C3DB6,0xBE272F05,0x29BDCA0D,0xE1AC4F9A,0x8BDD2621,0x17C42342,0xF1D5D5FA,
                                  0x8D54127A,0x39F9AAEB,0x39FF2111,0xCBC991A5,0xA09D19A2,0x34E711E2,0x3083F7D6,0x7082C129,
                                  0xDFD2C51C,0xB8324E2D,0xFF2182B8,0xFFACBDAD,0x185E1547,0x55BBD60D,0x8105133F,0x29227EBC,
                                  0x5CB9CC4C,0x875BE9D2,0xCA5FCD84,0xFDA28030,0x22926DA5,0xBDC54DE1,0xA047C8BE,0x56FE3EA1,
                                  0x70D6A735,0x0986E7DB,0x32A226B1,0x698470A0,0xAE871497,0x38759EED,0x8395381A,0xBE33BDD3,
                                  0xED100609,0x68BF8C4A,0xA61005C5,0x6ACC6B6A,0x92DA7CF5,0x1894619A,0x32B16A93,0x84A5457E,
                                  0xBC6D8B5B,0xFD46ECE3,0x6C141025,0x85A65AC3,0x43B33A6A,0x3C6E7471,0x981E1457,0x08FFE090,
                                  0x3C037B92,0x1CF8B2DD,0x72C25259,0x58B19355,0x082B5CF3,0xCC361458,0x63C13B1C,0x803AA17A,
                                  0x89BCEC46,0x200E94C9,0x247C4B3E,0xEA5B6E11,0x77DC2B1B,0x83FAC21B,0xA73BEF37,0x8193B3CC};
    uint32_t b[]                ={0x050339D1,0xAA17E05B,0xA4D8F84E,0xD79F773F,0x4488DE8A,0x15936C8E,0x2771D5F3,0x7849F453,
                                  0x38868A2D,0xF5C88ADB,0x2203C1B0,0xCE25DECB,0x2134E5BF,0xA27DE635,0x9FBA3151,0x808BE9DF,
                                  0x397AB279,0x8DA0FA59,0xEAD9213E,0xD26B4B93,0xE68106BE,0x2C21428A,0x6BAD3FB2,0xD11CB1E8,
                                  0xBF52F411,0xC253C5BA,0x222B13E9,0x26EC49C6,0x759C906F,0xE42ED028,0x3F93B8D5,0x5E96D567,
                                  0x968D98C6,0x461B9AB8,0xB1A774E9,0xCF7373C3,0x50476DCC,0xBA8472CE,0xB6B25A73,0x9ABE04EC,
                                  0x154E91A4,0xD7F00D74,0xE7F81BCA,0x1F3690F5,0x4AB2B6AD,0x68DE1791,0x3AE22035,0xFBB07520,
                                  0x920E868B,0x8A71229F,0x3293721B,0xF035C3F3,0x1A87AA4B,0xBFED6FF3,0xA04FF3CF,0x9B589783,
                                  0x2CFED56A,0x8AF31FAB,0xA700E3B2,0x333FFA39,0x35E90AA8,0xDD308E1F,0x1D2D80F2,0x4AA6516C,
                                  0x9ECD94C8,0xBBE698FD,0x8C14AB5A,0xDA585377,0x3E7E3614,0x76CFF8BC,0x40BB7F44,0xE7C398B7,
                                  0x4C62BA76,0xF21A9144,0xFEE929E8,0x16FF3101,0x76E77103,0xD939C515,0xF27EDEB9,0xB319B696,
                                  0xFA240B35,0x313669F1,0x55C8AD90,0x9CDC6D64,0x324EF398,0x7A30FEDE,0x8658BF6E,0xDBF25C03,
                                  0xC0467324,0xEAE2090E,0x6143D227,0x432842DD,0x103BDD09,0xC5D656C1,0x53ADD199,0x6968C938,
                                  0xCBB8E8D4,0x726A5351,0xAD912711,0x78051C9A,0xB01817C0,0x3408031B,0x675055EB,0xD8AE5DA7,
                                  0xBDF36F1B,0x2AA4C76A,0xA0594889,0x3CE17015,0xB29D29AA,0x4650594D,0x7E0AF024,0x09E7BAAE,
                                  0x908CC546,0x0E24267F,0xAA53EA6F,0xD8A0C420,0x57F48F8A,0x41E5C11D,0x36B75DA9,0xE2C1C692,
                                  0xFE603270,0xB1CB34E0,0x22AE18DC,0x2EC7A857,0x576E77FD,0xFE2955AD,0x3FACFE15,0x16BDB3C6};
    uint32_t std_a_add_b[]      ={0x9CB985B0,0x13B4384B,0xDDCB89A8,0xD1FABF24,0x81974A01,0xF6B3C80F,0x376EEAAB,0x35648AA9,
                                  0x5F6DDC69,0xFBDF4B3F,0x0368CFC7,0xA8C7933F,0x1B546CC5,0x660A2AC6,0x2871B925,0xC149B520,
                                  0xCC5B6AA0,0x76E36CE4,0x37054717,0xECA336B5,0x6FFFB0A9,0x78A7AADF,0xD347DFF0,0xEA658016,
                                  0x24F3467C,0x20350587,0x62F80600,0xCDC071D1,0xDF1B70B8,0xCA4CB865,0x1688DF48,0x46259CF6,
                                  0x458F6BD8,0xA1FAD622,0x5A943BC2,0x9525F89F,0xB8FDDFBE,0xABA614CD,0x57BC02FD,0xD7922079,
                                  0x0F691BFA,0x9A89A6C3,0x6D144C6C,0xEE39AB57,0xD6751544,0x6387D558,0x4937EF68,0xC9B44B5E,
                                  0x1FDADE44,0xD32774E4,0x7B7D12E3,0x27C132C5,0x9E95FB03,0x5F79D3B6,0x23E1C9EE,0x8F963522,
                                  0xDE9AB5BA,0xD65F5D61,0x652812B7,0x5CFDC447,0x17955A42,0x690DB441,0x34F1A435,0x3C7C2766,
                                  0x2C21A743,0xF5E043E9,0xC613CC6B,0xA621E51C,0xDF1B4FB7,0xABB70A9E,0x713F771A,0x584659E0,
                                  0x2C357F93,0xAA4CDF72,0xFE0AACA1,0x16ABEEAF,0x8F45864B,0x2EF59B22,0x7383F1F9,0xDC3C3553,
                                  0x56DDD781,0xB89253C4,0x20287B14,0x9A7EED95,0x54E1613E,0x37F64CBF,0x26A0882D,0x32F09AA5,
                                  0x311D1A5A,0xF468F0EA,0x93E5F8D8,0xACACB37D,0xBEC2F1A0,0xFE4BF5AE,0xD74309B3,0x279C870B,
                                  0xB8C8EEDE,0xDB29DF9C,0x53A12CD6,0xE2D18805,0x42F294B5,0x4C9C64B6,0x9A01C07E,0x5D53A325,
                                  0x7A60FA77,0x27EBB44E,0x0C6D58AF,0xC287CAD9,0xF6506414,0x82BECDBE,0x1629047B,0x12E79B3F,
                                  0xCC9040D8,0x2B1CD95C,0x1D163CC8,0x31525776,0x601FEC7E,0x0E1BD575,0x9A7898C6,0x62FC680C,
                                  0x881D1EB7,0xD1D9C9AA,0x472A641A,0x19231668,0xCF4AA319,0x822417C8,0xE6E8ED4D,0x98516792};
    uint32_t std_a_sub_b[]      ={0x92B3120E,0xBF847795,0x9419990A,0x22BBD0A5,0xF8858CEC,0xCB8CEEF2,0xE88B3EC5,0x44D0A202,
                                  0xEE60C80E,0x104E3588,0xBF614C66,0x0C7BD5A8,0xD8EAA146,0x210E5E5B,0xE8FD5682,0xC031E160,
                                  0x596605AD,0x5BA17832,0x6153049A,0x47CC9F8D,0xA2FDA32C,0x206525C9,0xFBED608C,0x482C1C45,
                                  0xA64D5E59,0x9B8D7A11,0x1EA1DE2C,0x7FE7DE45,0xF3E24FDA,0x01EF1814,0x97616D9D,0x88F7F227,
                                  0x18743A4B,0x15C3A0B1,0xF74551F0,0xF63F1117,0x186F0424,0x369D2F31,0xEA574E16,0xA216169F,
                                  0xE4CBF8B1,0xEAA98BDA,0x9D2414D6,0xAFCC896B,0x410FA7EA,0x91CBA636,0xD373AEFD,0xD253611D,
                                  0xFBBDD12C,0xBE452FA4,0x16562EAC,0x4755AADF,0x6986A66B,0xDF9EF3D0,0xE341E24E,0x58E5061A,
                                  0x849D0AE5,0xC0791E0B,0x17264B52,0xF67DCFD4,0xABC344F1,0xAEAC9802,0xFA96A24F,0xA72F848D,
                                  0xEE867DB2,0x7E1311ED,0xADEA75B6,0xF1713E2D,0x621EE38D,0xBE171926,0xEFC87891,0x88BF2871,
                                  0x93700AA5,0xC617BCE9,0x003858CF,0xE8AD8CAC,0xA176A444,0x7C8210F7,0x8E863485,0x7608C825,
                                  0x6295C116,0x56257FE0,0x74971FF4,0x60C612CC,0xF0437A0D,0x43944F02,0x19EF0950,0x7B0BE29E,
                                  0xB0903410,0x1EA4DECC,0xD15E5489,0x265C2DC2,0x9E4B378E,0x729F482C,0x2FE76680,0x54CAF49B,
                                  0x21571D35,0xF65538F9,0xF87EDEB3,0xF2C74ECF,0xE2C26534,0xE48C5E7E,0xCB6114A7,0xABF6E7D6,
                                  0xFE7A1C3F,0xD2A22578,0xCBBAC79C,0x48C4EAAD,0x911610C0,0xF61E1B23,0x1A132432,0xFF1825E2,
                                  0xAB76B64B,0x0ED48C5D,0xC86E67EA,0x8010CF34,0xB036CD68,0x8A50533A,0x2D09DD73,0x9D78DAE8,
                                  0x8B5CB9D5,0x6E435FE8,0x01CE3261,0xBB93C5BA,0x206DB31E,0x85D16C6E,0x678EF121,0x6AD60006};
    uint32_t std_b_sub_a[]      ={0x6D4CEDF2,0x407B886A,0x6BE666F5,0xDD442F5A,0x077A7313,0x3473110D,0x1774C13A,0xBB2F5DFD,
                                  0x119F37F1,0xEFB1CA77,0x409EB399,0xF3842A57,0x27155EB9,0xDEF1A1A4,0x1702A97D,0x3FCE1E9F,
                                  0xA699FA52,0xA45E87CD,0x9EACFB65,0xB8336072,0x5D025CD3,0xDF9ADA36,0x04129F73,0xB7D3E3BA,
                                  0x59B2A1A6,0x647285EE,0xE15E21D3,0x801821BA,0x0C1DB025,0xFE10E7EB,0x689E9262,0x77080DD8,
                                  0xE78BC5B4,0xEA3C5F4E,0x08BAAE0F,0x09C0EEE8,0xE790FBDB,0xC962D0CE,0x15A8B1E9,0x5DE9E960,
                                  0x1B34074E,0x15567425,0x62DBEB29,0x50337694,0xBEF05815,0x6E3459C9,0x2C8C5102,0x2DAC9EE2,
                                  0x04422ED3,0x41BAD05B,0xE9A9D153,0xB8AA5520,0x96795994,0x20610C2F,0x1CBE1DB1,0xA71AF9E5,
                                  0x7B62F51A,0x3F86E1F4,0xE8D9B4AD,0x0982302B,0x543CBB0E,0x515367FD,0x05695DB0,0x58D07B72,
                                  0x1179824D,0x81ECEE12,0x52158A49,0x0E8EC1D2,0x9DE11C72,0x41E8E6D9,0x1037876E,0x7740D78E,
                                  0x6C8FF55A,0x39E84316,0xFFC7A730,0x17527353,0x5E895BBB,0x837DEF08,0x7179CB7A,0x89F737DA,
                                  0x9D6A3EE9,0xA9DA801F,0x8B68E00B,0x9F39ED33,0x0FBC85F2,0xBC6BB0FD,0xE610F6AF,0x84F41D61,
                                  0x4F6FCBEF,0xE15B2133,0x2EA1AB76,0xD9A3D23D,0x61B4C871,0x8D60B7D3,0xD018997F,0xAB350B64,
                                  0xDEA8E2CA,0x09AAC706,0x0781214C,0x0D38B130,0x1D3D9ACB,0x1B73A181,0x349EEB58,0x54091829,
                                  0x0185E3C0,0x2D5DDA87,0x34453863,0xB73B1552,0x6EE9EF3F,0x09E1E4DC,0xE5ECDBCD,0x00E7DA1D,
                                  0x548949B4,0xF12B73A2,0x37919815,0x7FEF30CB,0x4FC93297,0x75AFACC5,0xD2F6228C,0x62872517,
                                  0x74A3462A,0x91BCA017,0xFE31CD9E,0x446C3A45,0xDF924CE1,0x7A2E9391,0x98710EDE,0x9529FFF9};
    uint32_t p[]                ={0xB71BAA73,0xBCFB32B8,0xB8DB85A7,0xD9F50754,0x77EF235F,0xF2FFAAD8,0xAFFA1691,0x7ADECB73,
                                  0xADDA45E2,0x759D76BA,0x9FDEC4D2,0x7FFA36B4,0xB7FB87E9,0x3FDF3BD2,0xDFED89AD,0xEBDFEB98,
                                  0x77B64AEC,0xDEFB1CA4,0xAFFC64B4,0xBCFD26E3,0x75A85887,0xA0ED0D74,0xA63C3D6A,0x7ABEB68B,
                                  0xFDFC5A5A,0x77772DF4,0xC29E1200,0xDF7EBFB5,0xE5F8CE9A,0xFFDC4712,0xF81F3264,0xEFB732BD,
                                  0xE9BF7ADB,0x6FFB9055,0x6FAA7A8F,0xFFFD5CDC,0xFFFFE219,0xCDFE20DA,0xFC3CE488,0xDD7E7309,
                                  0x9A6A431A,0x72F31981,0x7A7F2CFF,0xEBCE0575,0xEF5EDEB4,0x5B5F4273,0xF7DF1C3E,0xA792B708,
                                  0xFF3F2F7A,0xABA6EA40,0x6F730E46,0xF62FF40B,0xC2BAA906,0xDB7F6A9F,0x5F94EF59,0xBFFB9195,
                                  0xF9647F44,0x9FFDCA66,0xF6FC32B0,0xF7FC86C0,0xF5CF0DA2,0xFDDEF9BC,0xDFED7DCD,0xD67614A8};
    uint32_t q[]                ={0x7EFF478B,0xFFAFA12D,0xBF8EB6D5,0xAEC81B90,0xDF7ACB05,0xF77BF68A,0xDBEF8A03,0x6EC36D1C,
                                  0xABB6EBB2,0xCCF20A1C,0xF6E6B796,0xE9F77CE5,0x77FCA5F2,0x7B9FFE82,0xF77D0554,0xEBDE543A,
                                  0xFBE5874A,0x4DBFCF4D,0xEFE8E0B2,0xD61678F5,0xEF6D1E77,0x3FEAB238,0xA93D5824,0x77FA0F40,
                                  0xFDFFB3B2,0x3F0E7B58,0xBDF76B9A,0xFAF7F4B5,0xB5FF1246,0xC76E9653,0xFF77E2A8,0x6FEA397E,
                                  0xFBFD3BFB,0xF77BA0EC,0xB7BF83E8,0x9B97AFFA,0xFBFE1B9C,0xBFFC50DE,0xDF6DBCD7,0xF4EF4B33,
                                  0xDBEB0616,0xF6BFA515,0xF3D6A51B,0xFFFEAC0E,0xAECFB871,0xDFDECFA3,0xDAF645D2,0x6FFDA24A,
                                  0xFFFCB45B,0xFFB85CB3,0xEFFF1445,0xBC6FF98A,0xF9FFBEB8,0x5DEFBBF7,0xFFF60080,0xBFEE400C,
                                  0xEE46F5F4,0xEDF78DD2,0x5E7BFC51,0xDDFB53C1,0xFF762A73,0xEDBA6305,0xCAFE3703,0xC7DAE25C};
    uint32_t fai_n[]            ={0x44BD7F74,0xA2953CD8,0xB5A6202F,0xC9C9A678,0x615B4225,0x47F9059E,0x3442543D,0x75E08176,
                                  0x3BFEBD85,0xD82FDCA1,0x6A64FAFE,0x619F0CC6,0x7FA44321,0xF85BE2C1,0x4D37B8D5,0xB4C53FFD,
                                  0x9B632DD5,0x80036D71,0x60255314,0xA912C633,0xE385EFE8,0x43BCC645,0xA2219813,0x8C01EF0D,
                                  0x4E8FF702,0xF0DB9E71,0x5B62096B,0x1F16B3E0,0x9D13C2E8,0x939A7688,0xC8E16566,0x709B7218,
                                  0x118C1BEB,0xCB564A66,0xDFE502C7,0x64E2B4D6,0x7A9D283C,0xE3EA5064,0x2C16665F,0xCF8AC386,
                                  0xDE784406,0x7C443045,0xC70D5E0A,0xF64414B2,0xB655553B,0x359DDA67,0x9EF9E6EC,0xDBA57CE1,
                                  0x3AACB83E,0x1F25D62A,0x1EF40F95,0xBD1D4647,0xDAF2C3EE,0x96CBAACE,0xDA0AFB75,0x2C500FE3,
                                  0x854B9B3B,0xCF77BFBA,0x3612B323,0x02D2C2BE,0x71EF95BC,0x827A2F57,0x1E7B73DE,0x47C88C28,
                                  0x98BD42C3,0x2157B92A,0x08333932,0xE727F0AF,0x9C251E3B,0x21A26521,0x85D4C715,0x7C7295B8,
                                  0x8EB5C056,0x753AD2A3,0x7B79E1E7,0x60C35A7F,0x4FBC84AA,0x9DD29D3F,0x0AF86698,0xEE2FDC9C,
                                  0x43A63013,0xDDFF6881,0x370F6582,0xF4AE80B3,0x99BE2951,0xE8ED52C0,0x240F3CF9,0x43D9B299,
                                  0xA7D92A70,0xAA0649C6,0xFFCF76F7,0x6CBD10A3,0x16414414,0x0221270B,0x2274F5E4,0x1A5D915A,
                                  0x850EDA33,0xF117C236,0xB92BA20A,0x726B93E6,0x28A2213F,0x6B451929,0x4A7CEBA7,0x00A7C193,
                                  0x1CAA8EDB,0xF4850506,0x7F64304C,0x352559E6,0x70F5F3AB,0x805F4633,0x34ECB6C4,0xCB9B4194,
                                  0x77A5F318,0x6D63C372,0x028F4CAA,0x15706A91,0xCF432EFE,0x4B124097,0x3665604A,0x7753EC24,
                                  0x27E1677A,0x682F0D1F,0xEA766C3C,0xB0FB4911,0xAF6B4890,0xCD652306,0xD89AF2F4,0xA76D283E};
    uint32_t d[]                ={0x75B55129,0x3A94B5A2,0xF5EF0EFE,0xA1230B56,0xF5651806,0x7D7D8A98,0xFC67782E,0xD8C34C1C,
                                  0x1F9AE8C0,0x17D22A54,0x3CA2A27A,0x25CFEB81,0xB3C4BEAE,0xBCBD4668,0x95BECFAD,0x47DA8433,
                                  0x98673494,0xE7619A3F,0x0DB035EF,0x72B1F55A,0x04F2FA98,0x1F3EBA44,0x71306959,0x80B5A06D,
                                  0xC32E7D9D,0xBBEF961E,0x895C986B,0x56BB71E3,0x9D35182C,0x0A475D8F,0x6559F733,0x0EFDC803,
                                  0xC58CDF6B,0x42D302B6,0xDDCDE575,0xFD4988D1,0x8CE5BED5,0xE894EB3F,0x7926EE04,0xFECE4F48,
                                  0x15B5CAC6,0x2E279CC2,0xBD267A68,0xC6911445,0x181ACF36,0xBDBB0E53,0xB396D3C8,0xBF555977,
                                  0x9E5679F9,0xDA352AC8,0x2ED67B9E,0x9E31BEFA,0x147B9CFA,0x33E8EED9,0x62021158,0x568E956A,
                                  0xB8118D63,0xFABD8433,0xE99C259E,0x72CC543C,0x89568DD6,0xC135EFDC,0x64D7742C,0x7E4B6079,
                                  0x26A17F45,0x2155E663,0x18AE089B,0xC176A316,0xBFBBB415,0xFE6D1167,0x2A8C5EC4,0x53795229,
                                  0x626EF375,0x9F560B5C,0xC3282479,0xB610E643,0xEA22BDAE,0xAD2C3A52,0x5BC38AD4,0x245229D3,
                                  0x55D22D74,0xF2A012FE,0x277D3CCF,0x9D44527B,0x79A32053,0x21103E93,0xC5B5029B,0x4143DE20,
                                  0x4AA4C8A2,0x713EF093,0xE2E3F17D,0x0EE2779C,0xF2CABD96,0xDB51AE84,0x5D74CE06,0x414CE576,
                                  0x2C4A1F6E,0x2EE505AA,0xE14952ED,0x9DE1CABD,0xF019D3B3,0x54C8A688,0xFB7A2D0E,0xAF7A191C,
                                  0x034A2744,0xD0F9FDDE,0x0DACED7D,0xFCD101D1,0x91AAA44E,0x9F7173DB,0xEC4AB44D,0x2AE74AEE,
                                  0x4D66D4DD,0x954B91CA,0xDF4FF051,0x549B8102,0xC9FBF0EB,0x14CE2380,0xA116A4FD,0xB37E507A,
                                  0x68A4E236,0x67D0DBAD,0x55FE1CCB,0x4E1A3197,0x4553222B,0xAD836D83,0x133C00FC,0x18BC05A0};
    uint32_t n[]                ={0x7AD87171,0x5F4010BE,0x2E105CAD,0x5286C95E,0xB8C5308B,0x3274A701,0xC02BF4D3,0x5F82BA06,
                                  0x958FEF1A,0x1ABF5D78,0x012A7768,0xCB90C061,0xAF9C70FD,0xB3DB1D16,0x24A247D7,0x8C837FD1,
                                  0x0EFF000D,0xACBE5964,0x000A987B,0x3C26660D,0x489B66E8,0x249485F3,0xF19B2DA2,0x7EBAB4D9,
                                  0x4A8C050F,0xA76147BF,0xDBF78706,0xF98D684B,0x390BA3C9,0x5AE553EF,0xC0787A74,0xD03CDE55,
                                  0xF748D2C2,0x32CD7BA8,0x074F0140,0x0077C1AE,0x769B25F3,0x71E4C21E,0x07C107C0,0xA1F881C4,
                                  0x54CD8D38,0xE5F6EEDD,0x35633025,0xE210C637,0x5483EC62,0x70DBEC7F,0x71CF48FD,0xF335D635,
                                  0x39E89C14,0xCA851D1F,0x7E663221,0x6FBD33DD,0x97AD2BAE,0xD03AD166,0x3995EB4F,0xAC39E186,
                                  0x6CF71074,0x5D6D17F4,0x8B8AE226,0xD8CA9D40,0x6734CDD2,0x6E138C1A,0xC96728B0,0xE619832D,
                                  0x98BD42C4,0x2157B92A,0x08333932,0xE727F0AF,0x9C251E3B,0x21A26521,0x85D4C715,0x7C7295B8,
                                  0x8EB5C056,0x753AD2A3,0x7B79E1E7,0x60C35A7F,0x4FBC84AA,0x9DD29D3F,0x0AF86698,0xEE2FDC9C,
                                  0x43A63013,0xDDFF6881,0x370F6582,0xF4AE80B3,0x99BE2951,0xE8ED52C0,0x240F3CF9,0x43D9B299,
                                  0xA7D92A70,0xAA0649C6,0xFFCF76F7,0x6CBD10A3,0x16414414,0x0221270B,0x2274F5E4,0x1A5D915A,
                                  0x850EDA33,0xF117C236,0xB92BA20A,0x726B93E6,0x28A2213F,0x6B451929,0x4A7CEBA7,0x00A7C193,
                                  0x1CAA8EDB,0xF4850506,0x7F64304C,0x352559E6,0x70F5F3AB,0x805F4633,0x34ECB6C4,0xCB9B4194,
                                  0x77A5F318,0x6D63C372,0x028F4CAA,0x15706A91,0xCF432EFE,0x4B124097,0x3665604A,0x7753EC24,
                                  0x27E1677A,0x682F0D1F,0xEA766C3C,0xB0FB4911,0xAF6B4890,0xCD652306,0xD89AF2F4,0xA76D283E};
    uint32_t n_h[]              ={0xD3FD2CDE,0xCD7E4FB1,0x8CD53E2D,0x2294C500,0x71B2A621,0x98587E2B,0x2D38AD02,0x4F2D053E,
                                  0xD4B5B447,0x2B2DF82E,0x400239BC,0xEA709E5E,0xD71FCCA6,0xA24BD79A,0x1BB8CB02,0x8C5E5BA0,
                                  0xB9B387DF,0xE8761C3B,0x5DE63828,0x1432C19A,0xB7C703B8,0x2D048F8B,0x35DA7F44,0x1E8E5683,
                                  0x0BC68A9B,0x31AC2B2D,0x87B3AA0A,0x46E99F0C,0x9539E5DD,0xBC3A62DB,0x68DFF6AF,0x959C1662,
                                  0xC3D42ECA,0x1F4B3F44,0xF980C7F4,0xC27E69A2,0xC8D65805,0xA11CD5DE,0x32727A07,0x3AC4FE7C,
                                  0x451051F3,0x436D8A7A,0x3A881ED0,0x2690ADE9,0xD6DE0390,0x1AC25A06,0x3E1F385A,0xF5B2D1BE,
                                  0x4349DBE1,0x98A873C9,0xFFC1CF7E,0xF33D1621,0x07283ED4,0x73BD772F,0x762E5FE6,0x82582031,
                                  0x3FCA0940,0x080ECD9D,0xBFD02F7C,0x7348E709,0xA5D60612,0x00A4B1C1,0x2AF1405E,0x6D70CEF7,
                                  0x8CBEDF5E,0x7D72460F,0x6E50E567,0x0F7DE947,0x254196F0,0x7E2B00A3,0xABE5EFAD,0xDBF82E75,
                                  0x763386CD,0x55F9EEB7,0xCAFA2CBC,0x1133E59E,0x2C809F28,0x4A776EE2,0xECB014E7,0x171B29E7,
                                  0xBD0F12D8,0xF38555E9,0x8FA76C0D,0x4EF627D7,0xCB18061A,0xB55634DA,0x6B1F7A65,0x7A95E8FC,
                                  0x43F45328,0x61FCA731,0x4AA4C646,0x972D974D,0x6240F45F,0x59B2B04C,0x2309BF53,0x09D2FC6E,
                                  0xBF36D854,0x06D1C650,0x47A9E90F,0x3B29B12C,0x12ACA50A,0x257581C9,0xC0209B23,0xC6C4ECCB,
                                  0x7A5A93CD,0x5D1BA6B4,0xF8F509E3,0x202BDD78,0xEA82D002,0xCB0F284A,0x6D646418,0x82073AD8,
                                  0x684E8769,0x597B181D,0x8CE04DEF,0xA773DEF6,0xF7298C68,0xC3E9CA7C,0x29C8456D,0x74306DFF,
                                  0x151F1DF2,0xA35F0447,0x4690062C,0x5617287E,0x4EF602AE,0x6C1E4AEF,0x165CFBA8,0x18B7F94E,};
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    uint32_t n_n0[]             ={0xF22BD06F,};
#endif
    uint32_t n_10_01_h[4096/32] ={0};
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    uint32_t n_10_01_n0[]       ={0xFFFFFFFF};
#endif
    uint32_t n_11_11_h[4096/32] ={0};
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    uint32_t n_11_11_n0[]       ={1};
#endif
    uint32_t std_modsub_b_a[]   ={0xE8255F63,0x9FBB9928,0x99F6C3A2,0x2FCAF8B8,0xC03FA39F,0x66E7B80E,0xD7A0B60D,0x1AB21803,
                                  0xA72F270C,0x0A7127EF,0x41C92B02,0xBF14EAB8,0xD6B1CFB7,0x92CCBEBA,0x3BA4F155,0xCC519E70,
                                  0xB598FA5F,0x511CE131,0x9EB793E1,0xF459C67F,0xA59DC3BB,0x042F6029,0xF5ADCD16,0x368E9893,
                                  0xA43EA6B6,0x0BD3CDAD,0xBD55A8DA,0x79A58A06,0x452953EF,0x58F63BDA,0x29170CD7,0x4744EC2E,
                                  0xDED49877,0x1D09DAF7,0x1009AF50,0x0A38B096,0x5E2C21CE,0x3B4792ED,0x1D69B9AA,0xFFE26B24,
                                  0x70019486,0xFB4D6302,0x983F1B4E,0x32443CCB,0x13744478,0xDF104649,0x9E5B99FF,0x20E27517,
                                  0x3E2ACAE8,0x0C3FED7A,0x68100375,0x286788FE,0x2E268543,0xF09BDD96,0x56540900,0x5354DB6B,
                                  0xE85A058F,0x9CF3F9E8,0x746496D3,0xE24CCD6C,0xBB7188E0,0xBF66F417,0xCED08660,0x3EE9FE9F,
                                  0xAA36C512,0xA344A73C,0x5A48C37B,0xF5B6B281,0x3A063AAD,0x638B4BFB,0x960C4E83,0xF3B36D46,
                                  0xFB45B5B0,0xAF2315B9,0x7B418917,0x7815CDD3,0xAE45E065,0x21508C47,0x7C723213,0x78271476,
                                  0xE1106EFD,0x87D9E8A0,0xC278458E,0x93E86DE6,0xA97AAF44,0xA55903BD,0x0A2033A9,0xC8CDCFFB,
                                  0xF748F65F,0x8B616AF9,0x2E71226E,0x4660E2E1,0x77F60C86,0x8F81DEDE,0xF28D8F63,0xC5929CBE,
                                  0x63B7BCFD,0xFAC2893D,0xC0ACC356,0x7FA44516,0x45DFBC0A,0x86B8BAAA,0x7F1BD6FF,0x54B0D9BC,
                                  0x1E30729B,0x21E2DF8D,0xB3A968B0,0xEC606F38,0xDFDFE2EA,0x8A412B0F,0x1AD99291,0xCC831BB2,
                                  0xCC2F3CCC,0x5E8F3714,0x3A20E4C0,0x955F9B5C,0x1F0C6195,0xC0C1ED5D,0x095B82D6,0xD9DB113C,
                                  0x9C84ADA4,0xF9EBAD36,0xE8A839DA,0xF5678357,0x8EFD9571,0x4793B698,0x710C01D3,0x3C972838};
    uint32_t std_modmul[]       ={0x6B415C83,0xBDF4FB00,0x412772EC,0x1D5F61D3,0x250824A0,0xBB66169E,0xAD2B66D0,0x2E16F4A7,
                                  0x47555595,0x758C610F,0xE84E65DD,0x2A4D41AF,0x4FE1834D,0x993B3445,0x181D264F,0xE063C2EE,
                                  0xADF51662,0x5FEA1EC5,0xB6BA168D,0x8FFFC522,0x215BFD4D,0x13D3133E,0xFB687910,0x0D552A67,
                                  0x925FE946,0x88B479D8,0x3A9D4626,0x092A692E,0x7CD19EA4,0x269E97F7,0x371818C1,0x159541BE,
                                  0x3FD2097F,0xC67BA32F,0x24B40106,0x8BB054F4,0x045E778A,0xA3802710,0x72F3BDE0,0xA2D59251,
                                  0xAA2D9632,0x357E7FF7,0x63DB2AEB,0xE70EE530,0x9979956F,0x8431F7DF,0xB33F9616,0xC739140B,
                                  0xD7ACB1F4,0xB91D2653,0x636A27CF,0x01229FB6,0xB159D742,0x6C7B54E9,0x242579D3,0x62BDF6E3,
                                  0x896F84DB,0x10A93266,0x50DEFAF3,0x3A043C08,0x1967B785,0x03B88B60,0x87DCC456,0x58F211BA,
                                  0xBF41B7F8,0x6434E043,0x1A9C7686,0x734055E9,0xB8B82114,0x65D01316,0xDD17A48F,0x398C26E7,
                                  0x72A5FD5F,0x57950949,0xCEC694DD,0xB8F754DE,0xCE3702FC,0xEBF7E0A3,0x014936BF,0x079998A3,
                                  0x3F099388,0xB6F67759,0x3BFC4D43,0x2D578E85,0xE227F6FB,0x0E2F3284,0x25164362,0xBAAAFC14,
                                  0xED856728,0x829573C0,0xEDB5951F,0x9B3A4AD4,0x36A44212,0xC52FBF6F,0x5238E955,0x335BD6CB,
                                  0x78E36112,0x2529FF9D,0x7026554D,0x7B582DF6,0x01FA26A3,0xA2EB24AE,0xDBB51A85,0x936143F2,
                                  0x3F68FB70,0x3BFF165E,0xF9225656,0xA0367BF6,0xD715D79D,0xB20D863D,0xC114118B,0x3548379D,
                                  0xFCEC522A,0x1B89F149,0xE1D0244C,0xA0DBCFD3,0x2FBEB015,0x46724DA4,0x57454A6A,0x176F3919,
                                  0x7B63859A,0x23423281,0xDF9193A8,0xB5BBC55D,0x4FE7C04B,0x89F5EB2A,0x1036CFE6,0xA645F99B};
    uint32_t std_modinv_odd[]   ={0x71141A14,0x995876B9,0xE5E37612,0xDA9CFA58,0x38EF8907,0x4706829E,0x71E48447,0xDFC6127C,
                                  0x3B322FB9,0x9C53D34B,0xCEC3A4C6,0x67A304A2,0x54F8D6F2,0xDD6AB4B9,0x745A9899,0xB8DABF52,
                                  0x4A441C32,0x8786A567,0x06E08A85,0x9A299B38,0x7F2BAF74,0x3586E374,0x8FBC977E,0xA56F46E7,
                                  0x5FC3CBA4,0x87F82CFE,0xF2FA4AEA,0x7FAC414C,0x548DD23F,0x13DEE07D,0x4D984ADB,0x95CDF224,
                                  0x560286DD,0xDE142120,0xC4C71214,0xDFE5BAA6,0x30A1DAD8,0x4AF35184,0xF32C499E,0x2A3F3056,
                                  0x6DFFCEA4,0x78FDE493,0x3A5D3253,0x31604900,0xA09D5BDC,0xE4400827,0x3148C1D4,0x43F3B159,
                                  0x437FADFA,0x5560EC8D,0x7BEE82BD,0xB5468E99,0xE8CD8C48,0xB113AC59,0x81C36686,0x9AC56743,
                                  0x3F256AD8,0x3EC684C4,0x2427D68B,0x8D56CD6E,0x5A6F8C0D,0xF224F52F,0xD43B0EDD,0xD42C1BFF,
                                  0x230D044C,0xAEDCFC9C,0x8D044AB6,0xB718AE2B,0x3FE8F1E3,0x08233618,0x476BC263,0x6F22FB49,
                                  0xDB878136,0x29791E80,0x7ACD3A2B,0xD68F2328,0xE1F420E8,0xE2B3E89C,0xB17708ED,0x2749D515,
                                  0x2040EC6C,0xDF5C0247,0x0965D4CD,0xD6391B12,0xBC3D15A2,0xDEB2FF6E,0x02F1E36A,0x98962B08,
                                  0x28D20E35,0xC77F6684,0xE23A8A70,0xD879F9C0,0xFBA899F2,0x680F4D41,0x05ED4EA3,0x104FCC47,
                                  0x027F32EC,0xBDFF4A1F,0x3C5F24DF,0x239C4222,0x8A6F1663,0xA5D10984,0xFB62EC61,0xDC3AB892,
                                  0x12DC9B68,0xCD7B3094,0xCFB0BEC2,0x0E9155C3,0xF82E413C,0x4DA710C0,0x247BD21E,0xF3A95D85,
                                  0xF023BCB4,0x3B199C26,0xA02566CC,0x59EBB1D4,0x53D861A1,0x4074A2AD,0x6E5A8319,0xC053B507,
                                  0x8AD174B4,0x4549CFAD,0x9ADF597A,0x2913688F,0xD48D6383,0x21641ACA,0x935D5137,0x10635129};
    uint32_t std_modinv_even[]  ={0x9F70343F,0x43F71659,0x827D5FCD,0xF9EDFC07,0x8A871BB7,0x8CDDF262,0xABD8EE68,0xD52998D1,
                                  0x788AF99C,0x111811F6,0x9E86B5C7,0xCB4ADE0A,0x425F47B7,0xE5EC4370,0x2C430C7A,0xFAC04897,
                                  0xF830C3ED,0xD663A53C,0x0A4FC34C,0xDDBE389B,0x49CABA06,0x818F6F2B,0xD4A4156F,0xF24CCAAC,
                                  0x32F7F4E2,0x7994ECD3,0x1D4098B8,0xF9837912,0x8F00894A,0x00E40DE4,0x4CF0D7E6,0xC09FCA4B,
                                  0x15E88E9A,0x85CA8101,0xD521CD08,0x00E158C8,0x7C4FE74C,0x3C7CEDE9,0x71655ADC,0xBC00084E,
                                  0x0FA3E2A1,0x2D0CEDD1,0xCE893745,0x3A67999E,0x7CE2FE7F,0x11D0A823,0x23236FFF,0xF27A98BC,
                                  0x5D7097B4,0x0FBE652E,0x08DBA14A,0x9FAE2DB8,0x5C05724C,0x85E05FA7,0xF18F2853,0xA4B0B048,
                                  0xF00142BA,0x04874587,0x2BA36850,0x60046CD0,0xE4066C14,0xF1CF5EEB,0x14DAC0F4,0x414776FD,
                                  0xD3FEB461,0xF20562E9,0x211A09B7,0x8932B638,0xAB20D117,0xFAA863A7,0xA4F20C9A,0x98E2D8F7,
                                  0xE33993A3,0x5AA7A5E6,0xF7B4ABAD,0x37C30116,0x4DF6E5DB,0x3A7E37F9,0xB59DD6C1,0x1AB9171E,
                                  0x7922B0CD,0x6599C266,0x1F34B9CF,0xCFDBD2CA,0xD1830550,0x35A9DD0B,0x5F219A49,0xF4C54369,
                                  0x6AE254F9,0x586FFDBA,0xD088F221,0x90F4FC0E,0x5A162724,0x73620A20,0x1E160359,0xA01AC2BD,
                                  0x37521219,0x0FF6E917,0x7ADE8164,0x2B167361,0xF7C64B90,0x4CD05754,0xB1C91ADE,0x7142BAC3,
                                  0x32708EEA,0x86A0EFB3,0x2DCEB4F8,0x6EA2E9CB,0x74D3F6D8,0x75B2A3B9,0xAC7F8207,0xDBE99191,
                                  0x105B4212,0x26A9E341,0x7AE3B329,0x9F1B389C,0x212F69B8,0xD5A865A3,0x3B67F661,0x671C4843,
                                  0xFB91FD5F,0x4249A8D8,0xC909118A,0x297E9F75,0x640C2FAD,0x47D348CD,0x934D5C73,0x5409F8C8};
    uint32_t std_modexp[]       ={0xFC0FB3F1,0x664CB85F,0xC7CD18A2,0x2F5A3902,0x31CE10CB,0xE8DA6AEB,0x49CFC37F,0xBA3543FB,
                                  0xBCC9A736,0x540E0BC9,0xFB7D312C,0x66CE1B39,0xE3879457,0x393AA73D,0x5D4BE9C4,0x1B9B5572,
                                  0x6F0686D5,0xD2B70119,0xEFC6F6F2,0xA4D39266,0x32171AF7,0x57EEAE6A,0x47E00F5F,0xF9B3C8C9,
                                  0xE9884A90,0x57A3CF77,0x3D16FB28,0xED80307E,0xB78F7596,0x0C25D8F5,0xCCB017CF,0x05EC172D,
                                  0x2A7579BD,0xB107AB5E,0xB071109C,0xA338305A,0x7BFC7E3D,0xCFD922DB,0x22841D06,0xCEA79BCF,
                                  0xF57D13FF,0x636555C0,0x06134250,0x0A8A33E7,0xEC336D9F,0xB0527899,0x3FBF1AE3,0xD8E82226,
                                  0x0F0E035E,0x6DE1C691,0x2A8B40D8,0x818B04D3,0x63F04FA9,0x56ECD44B,0x93D1AFAD,0xCF41483A,
                                  0x93BE4A6A,0xF5F2A1CC,0xDF8670E8,0x299B8346,0x2D576920,0x77E2EF04,0x84E90C14,0x29124683,
                                  0x479B5EA0,0xF7208D01,0xECC7818A,0x626B46B5,0x5759C1D0,0xA2A9FAAF,0xB1EBFA87,0x2174FB77,
                                  0xF8AF92C1,0x7785F1C8,0xEBE62471,0x3FCC79CB,0xD8F8DF5A,0xC3857284,0x9B5EC4F4,0xD4973B8C,
                                  0xB1A7032E,0x7B381AB6,0x5D9904A9,0xD32DF724,0x156A7360,0x9F4622EC,0xDFDF0801,0x05791A6A,
                                  0x00A74955,0xB45E13EF,0x2FFEAEB7,0x468184D3,0x59DC4544,0xB98F7A2B,0x0E15C04A,0xD6BF71B2,
                                  0x60855234,0x364A695E,0x9E53F9C4,0x7A98FB31,0xB609ADD0,0xF4199A91,0x56F9BC73,0x8025C4D5,
                                  0x8A340A6B,0x95389090,0x9C0641A1,0x9F5DFBCC,0xDC6B7404,0xED60A9B2,0x0CCE0DE9,0xE0931DCC,
                                  0xA6B22C37,0xBA9D82BA,0x298C3D97,0x1CFE0388,0xA5AA2D34,0xFA8D4410,0xA7E8233A,0xF9ADE960,
                                  0x3FA1A726,0xB9DB08C2,0x9B6598BE,0x9EBE4826,0x9BB8227A,0x408AC95C,0x8834C8A4,0x1BD509ED};
    uint32_t std_modexp_d[]     ={0x2362F23D,0x08CD9B0A,0xAAF7E1CA,0x2B49E9EF,0xF15C7CD2,0xB0B9C1B5,0x953FF814,0xE068F545,
                                  0x1976DD36,0xACFD325B,0x41FE4170,0xD0E3C5F5,0xCBD86937,0x07BD1CAD,0x48573279,0x93EF8B9A,
                                  0x3395245D,0x6417F6E1,0x2BA25179,0xE73888C1,0xC5F0DA40,0xD7C00017,0xA918FA4B,0x6A8000CA,
                                  0xE772DCB7,0xCC413A27,0x5A4F3C4A,0xC2188F41,0x2E6E59C8,0xC8A03AE8,0x86A604C2,0x92071D52,
                                  0x037F24C7,0x4C8E9081,0xCE69FCBA,0x2D66FE9D,0x4316A66D,0x30D8211D,0xAF13D7DE,0xC03D78E8,
                                  0x4428BF77,0xFEE484F4,0x53084FEC,0x80FCBD10,0xF646A66F,0x4D7B95D8,0xB1A9667B,0x2C2484AA,
                                  0x47FEE62F,0x87C18455,0xBC2C3B24,0x1FB29D78,0xFD65B016,0x346D2DA8,0xB5C3C38B,0x90AE63E5,
                                  0x202C09B3,0xFDEDCFD6,0xF50C5866,0x49C3BB06,0x35EF13D8,0x203CD775,0xEC29F372,0x4267FC5A,
                                  0x0B2C7B9B,0x6F79688B,0x8555C9E8,0x31F00AC7,0x1E434207,0x66A550CA,0xD6FFBD9F,0x036286E9,
                                  0x2843C57A,0x4B352A43,0xD739DC5A,0x24746AD5,0x4952C5A6,0xEF7362E5,0x1504E033,0xB7944F67,
                                  0xA6076831,0x46E128D4,0x5318B15D,0x16495935,0xD90B2CAA,0xCE2719B0,0x0E72A58E,0x189853FE,
                                  0x33D93891,0x777273E3,0x49A17CE4,0x783EE7BE,0x67650D0E,0x65797241,0x80A38822,0x05BC9399,
                                  0xD003D16A,0xECCCBA93,0xD5BCB35B,0x5D2B4B03,0x44858FB4,0x6D0C0D5E,0x4ECECB14,0xAC583183,
                                  0x04D37972,0xE34DFCC5,0x89A0105C,0xD462FA3A,0x768574D1,0x6B88B1FC,0xFA8EBCEF,0x825482D7,
                                  0xC7234DE9,0x3B471607,0x8006A5F4,0x6DB57AF3,0xBB9191B4,0xE14A8CD8,0x924F86E7,0xD2C576BB,
                                  0xBA676B65,0x9C888AFB,0xF03ACDCE,0xDA60529E,0x195EF9FA,0xE3C4D08C,0x8D313D87,0x795DA11D};

    vector->bitLen          = 4096;
    vector->a               = a;
    vector->b               = b;
    vector->p               = p;
    vector->q               = q;
    vector->fai_n           = fai_n;
    vector->d               = d;
    vector->n               = n;
    vector->std_n_h         = n_h;
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    vector->std_n_n0        = n_n0;
#endif
    n_10_01_h[0]            = 4;
    vector->std_10_01_n_h   = n_10_01_h;
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    vector->std_10_01_n_n0  = n_10_01_n0;
#endif
    n_11_11_h[0]            = 1;
    vector->std_11_11_n_h   = n_11_11_h;
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
    vector->std_11_11_n_n0  = n_11_11_n0;
#endif
    vector->std_a_add_b     = std_a_add_b;
    vector->std_a_sub_b     = std_a_sub_b;
    vector->std_b_sub_a     = std_b_sub_a;
    vector->std_modadd      = std_a_add_b;
    vector->std_modsub_a_b  = std_a_sub_b;
    vector->std_modsub_b_a  = std_modsub_b_a;
    vector->std_modmul      = std_modmul;
    vector->std_modinv_odd  = std_modinv_odd;
    vector->std_modinv_even = std_modinv_even;
    vector->std_modexp      = std_modexp;
    vector->std_modexp_d    = std_modexp_d;

    return pke_basic_test(vector);
}
#endif
#endif




uint32_t hal_pke_basic_test(void)
{
    printf("\r\n\r\n =========================== HAL basic test ============================= \r\n");

/**/
#ifdef PKE_BASIC_160_TEST
    if(pke_basic_160_test())
        return 1;
#endif

#ifdef PKE_BASIC_256_TEST
    if(pke_basic_256_test())
        return 1;
#endif

#ifdef PKE_BASIC_521_TEST
    if(pke_basic_521_test())   //n is even
        return 1;
#endif

#ifdef PKE_BASIC_522_TEST
    if(pke_basic_522_test())
        return 1;
#endif

#ifdef PKE_BASIC_768_TEST
    if(pke_basic_768_test())
        return 1;
#endif

#ifdef PKE_BASIC_1024_TEST
    if(pke_basic_1024_test())
        return 1;
#endif

#ifdef PKE_BASIC_1502_TEST
    if(pke_basic_1502_test())
        return 1;
#endif

#ifdef PKE_BASIC_2048_TEST
    if(pke_basic_2048_test())
        return 1;
#endif

#ifdef PKE_BASIC_2766_TEST
    if(pke_basic_2766_test())
        return 1;
#endif

#ifdef PKE_BASIC_3072_TEST
    if(pke_basic_3072_test())
        return 1;
#endif

#ifdef PKE_BASIC_4096_TEST
#if (OPERAND_MAX_WORD_LEN >= 128)
    if(pke_basic_4096_test())
        return 1;
#endif
#endif



    return 0;
}

