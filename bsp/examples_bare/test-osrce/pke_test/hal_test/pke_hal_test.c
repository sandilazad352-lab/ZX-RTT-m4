#include <stdio.h>

#include "../../../crypto_hal/pke.h"


extern uint32_t hal_pke_basic_test(void);
extern uint32_t pke_big_mod_little_test(void);
extern uint32_t hal_pke_eccp_test(void);
extern uint32_t hal_pke_mod_test(void);
extern uint32_t hal_pke_x25519_test(void);
extern uint32_t hal_pke_ed25519_test(void);
extern uint32_t hal_pke_sm9_test(void);


uint32_t pke_hal_test(void)
{
    printf("\n\r PKE Version: %08x \n\r", rPKE_VERSION);

#ifdef AIC_PKE_SEC
    pke_sec_init();
#endif

#if 1
    if(hal_pke_basic_test())
        return 1;
#endif

#if 1
    if(pke_big_mod_little_test())
        return 1;
#endif

#if 1
    if(hal_pke_eccp_test())
        return 1;
#endif

#if 1
    if(hal_pke_mod_test())
        return 1;
#endif

#if 0
    if(hal_pke_x25519_test())
        return 1;
#endif

#if 0
    if(hal_pke_ed25519_test())
        return 1;
#endif

#if 0
    if(hal_pke_sm9_test())
        return 1;
#endif




    return 0;
}

