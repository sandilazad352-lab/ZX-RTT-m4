#ifndef SKE_TEST_CONFIG_H
#define SKE_TEST_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************************
 *******************************    TEST config    **********************************
 ************************************************************************************/

#define SKE_CONFIG_BASIC_TEST
/*
#define SKE_CONFIG_CBC_MAC_TEST
#define SKE_CONFIG_CMAC_TEST
#define SKE_CONFIG_GCM_TEST
#define SKE_CONFIG_GMAC_TEST
#define SKE_CONFIG_CCM_TEST
#define SKE_CONFIG_XTS_TEST
*/

#if 0

//AES/SM4/DES/TDES
#define SKE_CONFIG_SPEED_TEST
#ifdef SKE_CONFIG_SPEED_TEST
#define SKE_CONFIG_SPEED_TEST_BY_TIMER
#endif


#define SKE_CONFIG_CBC_MAC_SPEED_TEST
#ifdef SKE_CONFIG_CBC_MAC_SPEED_TEST
#define SKE_CONFIG_CBC_MAC_SPEED_TEST_BY_TIMER
#endif


#define SKE_CONFIG_CMAC_SPEED_TEST
#ifdef SKE_CONFIG_CMAC_SPEED_TEST
#define SKE_CONFIG_CMAC_SPEED_TEST_BY_TIMER
#endif


#define SKE_CONFIG_CCM_SPEED_TEST
#ifdef SKE_CONFIG_CCM_SPEED_TEST
#define SKE_CONFIG_CCM_SPEED_TEST_BY_TIMER
#endif


#define SKE_CONFIG_GCM_SPEED_TEST
#ifdef SKE_CONFIG_GCM_SPEED_TEST
#define SKE_CONFIG_GCM_SPEED_TEST_BY_TIMER
#endif


#define SKE_CONFIG_GMAC_SPEED_TEST
#ifdef SKE_CONFIG_GMAC_SPEED_TEST
#define SKE_CONFIG_GMAC_SPEED_TEST_BY_TIMER
#endif


#define SKE_CONFIG_XTS_SPEED_TEST
#ifdef SKE_CONFIG_XTS_SPEED_TEST
#define SKE_CONFIG_XTS_SPEED_TEST_BY_TIMER
#endif

#endif




#ifdef __cplusplus
}
#endif

#endif
