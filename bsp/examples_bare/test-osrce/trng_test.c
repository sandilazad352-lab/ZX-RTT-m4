#include <stdio.h>
#include "test.h"
//#include "../crypto_hal/pke.h"
#include <trng.h>
#include <utility.h>

typedef u32 (*RAND_FUNCTION)(u8 *a, u32 bytes);

u32 trng_test_internal(u8 align, u8 drbg, RAND_FUNCTION  rand_func)
{
    u8 rand_buffer[1028];
    u8 *rand_buf = rand_buffer;
    u32 i, ret;
    u32 len[4] = {1, 5, 99, 1024};

    if(align)
        rand_buf = rand_buffer;
    else
        rand_buf = rand_buffer+1;

    if(drbg)
    {
        trng_disable();
        trng_set_mode(1);  //DRBG
        trng_enable();
    }


    for(i=0;i<4;i++)
    {
        printf("\r\n test %u, align=%u, use drbg=%u, len=%u", i+1, align, drbg, len[i]);
        memset_(rand_buf, 0, len[i]);
        ret = rand_func(rand_buf, len[i]);
        if(TRNG_SUCCESS != ret)
        {
            printf("\r\n ret = %x", ret);fflush(stdout);
            return ret;
        }
        print_buf_U8(rand_buf, len[i], "rand");
    }

    if(drbg)
    {
        trng_disable();
        trng_set_mode(0);  //TRNG
        trng_enable();
    }

    return 0;
}

u32 trng_test_1(void)
{
    trng_test_internal(1, 0, get_rand_internal);
    trng_test_internal(0, 0, get_rand_internal);

    trng_test_internal(1, 1, get_rand_internal);
    trng_test_internal(0, 1, get_rand_internal);

    trng_test_internal(1, 1, get_rand_fast);
    trng_test_internal(0, 1, get_rand_fast);

    trng_test_internal(1, 1, get_rand);
    trng_test_internal(0, 1, get_rand);

    return 0;
}

u32 trng_test_2(void)
{
    u8 rand_buf[1024];
    u32 ret;
    u16 i, j, tmp;

    for(i=0; i<500; i++)
    {
        printf("\r\n trng_test_2 %u", i+1);fflush(stdout);
        get_rand_internal(rand_buf, 100);

        /*************** ro ***************/
        trng_disable();

        trng_set_mode(rand_buf[0]&1);  printf(" %u", rand_buf[0]&1);fflush(stdout);

        tmp = rand_buf[1]&0x0F;
        if(0 == tmp)
            tmp+=1;
        trng_ro_entropy_config(tmp);   printf(" %08x", tmp);fflush(stdout);

        for(j=1; j<=4; j++)
        {
            tmp = rand_buf[2+j*2]|(((u16)(rand_buf[3+j*2]))<<8);
            trng_ro_sub_entropy_config(i, tmp);   printf(" %x", tmp);fflush(stdout);
        }

        trng_set_freq(rand_buf[10]&3);   printf(" %u", rand_buf[10]&3);fflush(stdout);

        trng_enable();
        ret = get_rand_internal(rand_buf, 100);
        if(TRNG_SUCCESS != ret)
        {
            print_buf_U8(rand_buf, 100, "rand_internal");
            printf("\r\n ret = %x", ret);fflush(stdout);
            return ret;
        }
        print_buf_U8(rand_buf, 100, "rand_internal");


        /*************** ro(no entropy reducing) ***************/
        get_rand(rand_buf, 100);
        print_buf_U8(rand_buf, 100, "rand(no entropy reducing)");

    }

    return 0;
}

void test(void)
{
/*
    u8 rand_buf[1024];
    trng_disable();
    trng_set_mode(1);  //DRBG
    trng_enable();
*/
    //get_rand_internal(rand_buf, 100);
}

u8 TRNG_test()
{
#if 1
    volatile u32 i;
    u8 rand[1024];
    u8 ret=0;

#if 1
    trng_disable();
    trng_set_mode(1);  //DRBG
    trng_set_freq(2);
    trng_enable();
#endif

    //memset_(rand,0,1024);
    printf("\r\n begin");fflush(stdout);
    for(i=0; i<1000; i++)
        ret |= get_rand_internal(rand, 1024);
        //ret |= get_rand_fast(rand, 1024);
        //ret |= get_rand(rand, 1024);

    printf("\r\n end");fflush(stdout);

    print_buf_U8(rand, 1024, "rand");

    return ret;
#else
    test();

    trng_test_1();
    trng_test_2();

    return 0;
#endif
}
