#include <stdio.h>
#include <aic_common.h>

#include "hash_hmac_self_test.h"

u32 hash_hmac_self_test(void)
{
    u32 ret;

    ret = hash_self_test();
    if(ret)
    {
        printf("\r\n HASH self test error, ret = %x \r\n", ret);
    }
    else
    {
        printf("\r\n HASH self test success \r\n");
    }

    ret = hmac_self_test();
    if(ret)
    {
        printf("\r\n HMAC self test error, ret = %x \r\n", ret);
    }
    else
    {
        printf("\r\n HMAC self test success \r\n");
    }

    return 0;
}

