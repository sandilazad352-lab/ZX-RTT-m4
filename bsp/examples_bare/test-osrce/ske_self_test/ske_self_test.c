#include <stdio.h>
#include "ske_self_test.h"

u32 ske_self_test(void)
{
    u32 ret;

    ret = ske_basic_mode_self_test();
    if(ret)
    {
        printf("\r\n SKE basic mode self test error, ret = %x \r\n", ret);
    }
    else
    {
        printf("\r\n SKE basic mode self test success \r\n");
    }

#ifdef SUPPORT_SKE_MODE_XTS
    ret = ske_xts_self_test();
    if(ret)
    {
        printf("\r\n XTS self test error, ret = %x \r\n", ret);
    }
    else
    {
        printf("\r\n XTS self test success \r\n");
    }
#endif

#ifdef AIC_SKE_MODE_CBC_SUPPORT_MAC
    ret = ske_cbc_mac_self_test();
    if(ret)
    {
        printf("\r\n CBC_MAC self test error, ret = %x \r\n", ret);
    }
    else
    {
        printf("\r\n CBC_MAC self test success \r\n");
    }
#endif

#ifdef SUPPORT_SKE_MODE_CMAC
    ret = ske_cmac_self_test();
    if(ret)
    {
        printf("\r\n CMAC self test error, ret = %x \r\n", ret);
    }
    else
    {
        printf("\r\n CMAC self test success \r\n");
    }
#endif

#ifdef SUPPORT_SKE_MODE_CCM
    ret = ske_ccm_self_test();
    if(ret)
    {
        printf("\r\n CCM self test error, ret = %x \r\n", ret);
    }
    else
    {
        printf("\r\n CCM self test success \r\n");
    }
#endif

#ifdef SUPPORT_SKE_MODE_GCM
    ret = ske_gcm_gmac_self_test();
    if(ret)
    {
        printf("\r\n GCM/GMAC self test error, ret = %x \r\n", ret);
    }
    else
    {
        printf("\r\n GCM/GMAC self test success \r\n");
    }
#endif

#ifdef SUPPORT_SKE_AES_XCBC_MAC_96
    ret = ske_aes_xcbc_mac_96_self_test();
    if(ret)
    {
        printf("\r\n AES_XCBC_MAC_96 self test error, ret = %x \r\n", ret);
    }
    else
    {
        printf("\r\n AES_XCBC_MAC_96 self test success \r\n");
    }
#endif

    return 0;
}

