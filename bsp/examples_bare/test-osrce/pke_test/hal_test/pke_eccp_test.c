#include <stdio.h>

#include "../../../crypto_hal/pke.h"
#include "../../../crypto_include/crypto_common/utility.h"
#include "../../../crypto_include/crypto_common/eccp_curve.h"
#include "../../../crypto_include/trng/trng.h"



//#define ECC_SPEED_TEST_BY_TIMER

#ifdef ECC_SPEED_TEST_BY_TIMER
extern uint32_t startP(void);
extern uint32_t endP(uint8_t mode, uint32_t once_bytes, uint32_t round);
#endif




//only one of the following two macro is available,
#define ECCP_POINTMUL_SECURE_TEST       //this is for PHE_HP/PHE_SECURE/PKE_UHP, point multiplication function is eccp_pointMul_sec
//#define ECCP_POINTMUL_SIMPLE_TEST       //point multiplication function is eccp_pointMul


uint32_t eccp_pointMul_(eccp_curve_t *curve, uint32_t *k, uint32_t *Px, uint32_t *Py,
        uint32_t *Qx, uint32_t *Qy)
{
#ifdef ECCP_POINTMUL_SECURE_TEST
    return eccp_pointMul_sec(curve, k, Px, Py, Qx, Qy);
#elif defined(ECCP_POINTMUL_SIMPLE_TEST)
    return eccp_pointMul(curve, k, Px, Py, Qx, Qy);
#endif
}



typedef struct {
    uint32_t *k;
    uint32_t *P_x;
    uint32_t *P_y;
    uint32_t *kP_x;
    uint32_t *kP_y;
    uint32_t *dbl_P_x;
    uint32_t *dbl_P_y;
    uint32_t *max_P_x;
    uint32_t *max_P_y;

} pke_eccp_pointMul_vector_t;




uint32_t eccp_pointMul_test_internal(eccp_curve_t *curve, char *curve_name, uint32_t *k, uint32_t *Px, uint32_t *Py,
        uint32_t *expected_Qx, uint32_t *expected_Qy,
        uint32_t *output_Qx, uint32_t *output_Qy,
        uint32_t expected_ret, char *info)
{
    uint32_t Qx[ECCP_MAX_WORD_LEN]={0};
    uint32_t Qy[ECCP_MAX_WORD_LEN]={0};
    uint32_t pWordLen, nWordLen;
    uint32_t ret;

    pWordLen = GET_WORD_LEN(curve->eccp_p_bitLen);
    nWordLen = GET_WORD_LEN(curve->eccp_n_bitLen);

    //test point multiple
    ret = eccp_pointMul_(curve, k, Px, Py, Qx, Qy);
    if(expected_ret != ret)
    {
        printf("\r\n %s eccp_pointMul %s error 1, ret=%x", curve_name, info, ret); 
        print_BN_buf_U32(k, nWordLen, "k");
        print_BN_buf_U32(Px, pWordLen, "Px");
        print_BN_buf_U32(Py, pWordLen, "Py");
        print_BN_buf_U32(Qx, pWordLen, "Qx");
        print_BN_buf_U32(Qy, pWordLen, "Qy");
        printf("\r\n ret=%x(0:valid, other:invalid)\r\n", eccp_pointVerify(curve, Qx, Qy));
        return 1;
    }

    if((PKE_SUCCESS == ret) && (NULL != expected_Qx) && (NULL != expected_Qy))
    {
        if(uint32_BigNumCmp(Qx, pWordLen, expected_Qx, pWordLen) || uint32_BigNumCmp(Qy, pWordLen, expected_Qy, pWordLen))
        {
            printf("\r\n %s eccp_pointMul %s failure, ret=%x", curve_name, info, ret);
            print_BN_buf_U32(Qx, pWordLen, "Qx");
            print_BN_buf_U32(Qy, pWordLen, "Qy");
            print_BN_buf_U32(expected_Qx, pWordLen, "expected_Qx");
            print_BN_buf_U32(expected_Qy, pWordLen, "expected_Qy");
            return 1;
        }
    }

    if(NULL != info)
    {
        printf("\r\n %s eccp_pointMul %s success", curve_name, info); 
    }

    if(NULL != output_Qx)
    {
        uint32_copy(output_Qx, Qx, pWordLen);
    }
    if(NULL != output_Qy)
    {
        uint32_copy(output_Qy, Qy, pWordLen);
    }

    return 0;
}


uint32_t eccp_pointMul_std_test(eccp_curve_t *curve, char *curve_name, pke_eccp_pointMul_vector_t *vector, uint32_t vector_num)
{
    char *info[] = {
        "[k]P (P is generator G)",
        "[k]P (P is a random and fixed valid point)", };
    uint32_t i;
    //uint32_t k1[ECCP_MAX_WORD_LEN]  = {0};

    printf("\r\n\r\n---- %s pointMul standard data test ----", curve_name);

    //test 1
    for(i=0; i<vector_num; i++)
    {
        if(eccp_pointMul_test_internal(curve, curve_name, vector->k, vector->P_x, vector->P_y, vector->kP_x, vector->kP_y, NULL, NULL,
            PKE_SUCCESS, i>0?info[1]:info[0]))
        {
            return 1;
        }

        vector++;
    }

/*
    //test 2  ---- in corner case test, so this could be omitted
    k1[0]=2;
    if(eccp_pointMul_test_internal(curve, curve_name, k1, vector->P_x, vector->P_y, vector->dbl_P_x, vector->dbl_P_y, NULL, NULL,
        PKE_SUCCESS, "[2]P"))
    {
        return 1;
    }
//*/
    return 0;
}


uint32_t eccp_pointMul_corner_test_internal(eccp_curve_t *curve, char *curve_name, uint32_t *Px, uint32_t *Py,
        uint32_t *dbl_Px, uint32_t *dbl_Py,
        uint32_t *max_Px, uint32_t *max_Py, uint8_t info)
{
    uint32_t k[ECCP_MAX_WORD_LEN]={0};
    uint32_t Qx[ECCP_MAX_WORD_LEN];
    uint32_t Qy[ECCP_MAX_WORD_LEN];

    uint32_t pWordLen, nWordLen;
    uint32_t ret;

    pWordLen = GET_WORD_LEN(curve->eccp_p_bitLen);
    nWordLen = GET_WORD_LEN(curve->eccp_n_bitLen);

#if 1
    //[0]P
    uint32_clear(k, nWordLen);
    ret = eccp_pointMul_test_internal(curve, curve_name, k, Px, Py, NULL, NULL, NULL, NULL, PKE_NO_MODINV, (0==info)?NULL:"[0]P");
    if(ret)
    {
        return 1;
    }
#endif

    //[1]P
    k[0]=1;
    ret = eccp_pointMul_test_internal(curve, curve_name, k, Px, Py, Px, Py, NULL, NULL, PKE_SUCCESS, (0==info)?NULL:"[1]P");
    if(ret)
    {
        return 1;
    }

    //[2]P
    k[0]=2;
    ret = eccp_pointMul_test_internal(curve, curve_name, k, Px, Py, dbl_Px, dbl_Py, NULL, NULL, PKE_SUCCESS, (0==info)?NULL:"[2]P");
    if(ret)
    {
        return 1;
    }

    //[n-2]P    caution: p(x,y) -> -p(x,-y)
    uint32_copy(k, curve->eccp_n, nWordLen);
    pke_set_operand_uint32_value(Qx, nWordLen, 2);
    pke_sub(k, Qx, k, nWordLen);
    uint32_copy(Qy, curve->eccp_p, pWordLen);
    pke_sub(Qy, dbl_Py, Qy, pWordLen);
    ret = eccp_pointMul_test_internal(curve, curve_name, k, Px, Py, dbl_Px, Qy, NULL, NULL, PKE_SUCCESS, (0==info)?NULL:"[n-2]P");
    if(ret)
    {
        return 1;
    }

    //[n-1]P    caution: p(x,y) -> -p(x,-y)
    uint32_copy(k, curve->eccp_n, nWordLen);
    pke_set_operand_uint32_value(Qx, nWordLen, 1);
    pke_sub(k, Qx, k, nWordLen);
    uint32_copy(Qy, curve->eccp_p, pWordLen);
    pke_sub(Qy, Py, Qy, pWordLen);
    ret = eccp_pointMul_test_internal(curve, curve_name, k, Px, Py, Px, Qy, NULL, NULL, PKE_SUCCESS, (0==info)?NULL:"[n-1]P");
    if(ret)
    {
        return 1;
    }

#if 1
    //[n]P
    ret = eccp_pointMul_test_internal(curve, curve_name, curve->eccp_n, Px, Py, NULL, NULL, NULL, NULL, PKE_NO_MODINV, (0==info)?NULL:"[n]P");
    if(ret)
    {
        return 1;
    }
#endif

    //[n+1]P
    uint32_copy(k, curve->eccp_n, nWordLen);
    pke_set_operand_uint32_value(Qx, nWordLen, 1);
    pke_add(k, Qx, k, nWordLen);
    ret = eccp_pointMul_test_internal(curve, curve_name, k, Px, Py, Px, Py, NULL, NULL, PKE_SUCCESS, (0==info)?NULL:"[n+1]P");
    if(ret)
    {
        return 1;
    }

#if 1
    //[n+2]P
    uint32_copy(k, curve->eccp_n, nWordLen);
    pke_set_operand_uint32_value(Qx, nWordLen, 2);
    pke_add(k, Qx, k, nWordLen);//k[0]+=1;
    ret = eccp_pointMul_test_internal(curve, curve_name, k, Px, Py, dbl_Px, dbl_Py, NULL, NULL, PKE_SUCCESS, (0==info)?NULL:"[n+2]P");
    if(ret)
    {
        return 1;
    }
#endif

    //[0xFFFF...FF]P
    if(NULL != max_Px && NULL != max_Py)
    {
        uint32_set(k, 0xFFFFFFFF, nWordLen);
        ret = eccp_pointMul_test_internal(curve, curve_name, k, Px, Py, max_Px, max_Py, NULL, NULL, PKE_SUCCESS, (0==info)?NULL:"[0xFFFF...FF]P");
        if(ret)
        {
            return 1;
        }
    }

    return 0;
}



uint32_t eccp_pointMul_corner_test(eccp_curve_t *curve, char *curve_name, pke_eccp_pointMul_vector_t *vector, uint32_t vector_num)
{
    uint32_t k[ECCP_MAX_WORD_LEN];
    uint32_t P_x[ECCP_MAX_WORD_LEN];
    uint32_t P_y[ECCP_MAX_WORD_LEN];
    uint32_t dbl_P_x[ECCP_MAX_WORD_LEN];
    uint32_t dbl_P_y[ECCP_MAX_WORD_LEN];
    uint32_t nWordLen, i;
    uint32_t ret;

    nWordLen = GET_WORD_LEN(curve->eccp_n_bitLen);

    //test 1
    printf("\r\n\r\n---- %s pointMul corner data test 1(P is generator G)----", curve_name);
    if(eccp_pointMul_corner_test_internal(curve, curve_name, vector->P_x, vector->P_y, vector->dbl_P_x, vector->dbl_P_y,
            vector->max_P_x, vector->max_P_y, 1))
    {
        return 1;
    }
    vector++;

    //test 2
    for(i=2;i<=vector_num;i++)
    {
        printf("\r\n\r\n---- %s pointMul corner data test %u(P is a random and fixed valid point)----", curve_name, i);
        if(eccp_pointMul_corner_test_internal(curve, curve_name, vector->P_x, vector->P_y, vector->dbl_P_x, vector->dbl_P_y,
                vector->max_P_x, vector->max_P_y, 1))
        {
            return 1;
        }

        vector++;
    }

    //random valid point
    printf("\r\n\r\n---- %s pointMul corner data test 3(P is real random valid point)---- \r\n", curve_name);
    for(i=0; i<100; i++)
    {
        printf(" %u", i+1);

        //random scalar
        get_rand((uint8_t *)k, nWordLen<<2);

        //get random valid point P
        ret = eccp_pointMul_(curve, k, curve->eccp_Gx, curve->eccp_Gy, P_x, P_y);
        if(ret)
        {
            printf("\r\n %s eccp_pointMul error 1, ret=%x", curve_name, ret);
            return 1;
        }

        //get [2]P
#ifdef ECCP_POINT_DOUBLE
        ret = eccp_pointDouble(curve, P_x, P_y, dbl_P_x, dbl_P_y);
#else
        pke_set_operand_uint32_value(k, nWordLen, 2);
        ret = eccp_pointMul_(curve, k, P_x, P_y, dbl_P_x, dbl_P_y);
#endif
        if(ret)
        {
            printf("\r\n %s eccp_pointMul error 2, ret=%x", curve_name, ret);
            return 1;
        }

        //input point is random valid point
        if(eccp_pointMul_corner_test_internal(curve, curve_name, P_x, P_y, dbl_P_x, dbl_P_y, NULL, NULL, 0))
        {
            return 1;
        }
    }
    printf("\r\n");

    return 0;
}



uint32_t eccp_pointMul_rand_test(eccp_curve_t *curve, char *curve_name)
{
    uint32_t k[ECCP_MAX_WORD_LEN];
    uint32_t k2[ECCP_MAX_WORD_LEN];
    uint32_t P_x[ECCP_MAX_WORD_LEN];
    uint32_t P_y[ECCP_MAX_WORD_LEN];
    uint32_t Q_x[ECCP_MAX_WORD_LEN];
    uint32_t Q_y[ECCP_MAX_WORD_LEN];
    uint32_t pWordLen, nWordLen, i;
    //uint32_t ret;

    printf("\r\n---- eccp pointMul rand test([k]P=[n+k]P,[n-k]P=-[k]P=(kPx,-kPy)) ----\r\n");

    pWordLen = GET_WORD_LEN(curve->eccp_p_bitLen);
    nWordLen = GET_WORD_LEN(curve->eccp_n_bitLen);

    for(i=0;i<100;i++)
    {
        printf(" %u", i+1);

        //get a random point P
        get_rand((uint8_t *)k, nWordLen<<2);
        if(eccp_pointMul_test_internal(curve, curve_name, k, curve->eccp_Gx, curve->eccp_Gy, NULL, NULL, P_x, P_y, PKE_SUCCESS, NULL))
        {
            return 1;
        }

        //random scalar k
        get_rand((uint8_t *)k, 4);
        uint32_clear(k+1, nWordLen-1);

        //get [k]P
        if(eccp_pointMul_test_internal(curve, curve_name, k, P_x, P_y, NULL, NULL, Q_x, Q_y, PKE_SUCCESS, NULL))
        {
            return 1;
        }

        //get [n+k]P
        uint32_copy(k2, curve->eccp_n, nWordLen);
        (void)pke_add(k2, k, k2, nWordLen);
        if(eccp_pointMul_test_internal(curve, curve_name, k2, P_x, P_y, Q_x, Q_y, NULL, NULL, PKE_SUCCESS, NULL))
        {
            return 1;
        }

        //get [n-k]P
        uint32_copy(k2, curve->eccp_n, nWordLen);
        (void)pke_sub(k2, k, k2, nWordLen);
        uint32_copy(k, curve->eccp_p, pWordLen);
        (void)pke_sub(k, Q_y, Q_y, pWordLen);
        if(eccp_pointMul_test_internal(curve, curve_name, k2, P_x, P_y, Q_x, Q_y, NULL, NULL, PKE_SUCCESS, NULL))
        {
            return 1;
        }
    }

    printf("\r\n");
    return 0;
}


uint32_t eccp_pointMul_invalid_point_test(eccp_curve_t *curve, char *curve_name)
{
    uint32_t k[ECCP_MAX_WORD_LEN];
    uint32_t b[ECCP_MAX_WORD_LEN];  //new curve parameter b
    uint32_t P_x[ECCP_MAX_WORD_LEN];
    uint32_t P_y[ECCP_MAX_WORD_LEN];
    uint32_t Q_x[ECCP_MAX_WORD_LEN];
    uint32_t Q_y[ECCP_MAX_WORD_LEN];
    uint32_t *bak_b;

    uint32_t pWordLen, nWordLen, i;
    uint32_t ret;

    printf("\r\n---- eccp pointMul invalid input point test ----\r\n");

    pWordLen = GET_WORD_LEN(curve->eccp_p_bitLen);
    nWordLen = GET_WORD_LEN(curve->eccp_n_bitLen);

    bak_b = curve->eccp_b;

    for(i=0;i<100;i++)
    {
        printf(" %u", i+1);

        /************************ k(Px, 0) no output **************************/
        //get a random invalid point (P_x, 0)
        get_rand((uint8_t *)P_x, pWordLen<<2);
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
        ret = pke_mod(P_x, pWordLen, curve->eccp_p, curve->eccp_p_h, curve->eccp_p_n0, pWordLen, P_x);  //P_x = P_x mod p
#else
        ret = pke_mod(P_x, pWordLen, curve->eccp_p, curve->eccp_p_h, pWordLen, P_x);  //P_x = P_x mod p
#endif
        if(ret)
        {
            printf("\r\n mod error 1, ret = %x", ret);
            return 1;
        }
        uint32_clear(P_y, pWordLen);

        //get a random scalar
        get_rand((uint8_t *)k, nWordLen<<2);

        //no output point
        if(eccp_pointMul_test_internal(curve, curve_name, k, P_x, P_y, NULL, NULL, Q_x, Q_y, PKE_NO_MODINV, NULL))
        {
            printf("\r\n %s eccp pointMul invalid input point test error 1, ret = %x", curve_name, ret);
            return 1;
        }

        /************************ k(Px, Py) output an invalid point **************************/
        get_rand((uint8_t *)P_y, pWordLen<<2);
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
        ret = pke_mod(P_y, pWordLen, curve->eccp_p, curve->eccp_p_h, curve->eccp_p_n0, pWordLen, P_y);  //P_x = P_x mod p
#else
        ret = pke_mod(P_y, pWordLen, curve->eccp_p, curve->eccp_p_h, pWordLen, P_y);  //P_x = P_x mod p
#endif
        if(ret)
        {
            printf("\r\n mod error 2, ret = %x", ret);
            return 1;
        }

        //get new curve's parameter b
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
        pke_set_exe_cfg(PKE_EXE_CFG_ALL_NON_MONT);
#endif
        ret = pke_modmul_internal(P_x, P_x, k, pWordLen);                   //k = x^2
        ret |= pke_modadd(curve->eccp_p, k, curve->eccp_a, k, pWordLen);    //k = x^2+a
        ret |= pke_modmul_internal(P_x, k, k, pWordLen);                    //k = x(x^2+a) = x^3+ax
        ret |= pke_modmul_internal(P_y, P_y, b, pWordLen);
        ret |= pke_modsub(curve->eccp_p, b, k, b, pWordLen);                //b = y^2 - (x^3+ax)
        if(ret)
        {
            printf("\r\n b calc error, ret = %x", ret);
            return 1;
        }

        //get a random scalar
        get_rand((uint8_t *)k, nWordLen<<2);

        //get the output point
        if(eccp_pointMul_test_internal(curve, curve_name, k, P_x, P_y, NULL, NULL, Q_x, Q_y, PKE_SUCCESS, NULL))
        {
            printf("\r\n %s eccp pointMul invalid input point test error 2, ret = %x", curve_name, ret);
            return 1;
        }

        if(PKE_SUCCESS == eccp_pointVerify(curve, Q_x, Q_y))
        {
            printf("\r\n %s eccp pointMul invalid input point test error 3, ret = %x", curve_name, ret);
            return 1;
        }

        //change b
        curve->eccp_b = b;

        //test output point using new curve
        ret = eccp_pointVerify(curve, Q_x, Q_y);
        if(ret)
        {
            printf("\r\n %s eccp pointMul invalid input point test error 4, ret = %x", curve_name, ret);

            //recover b
            curve->eccp_b = bak_b;
            return 1;
        }

        //recover b
        curve->eccp_b = bak_b;
    }

    return 0;
}


uint32_t eccp_pointMul_test(eccp_curve_t *curve, char *curve_name, pke_eccp_pointMul_vector_t *vector, uint32_t vector_num)
{
    printf("\r\n\r\n ***************************** %s pointMul test ***************************** ", curve_name);

    if(eccp_pointMul_std_test(curve, curve_name, vector, vector_num))
    {
        return 1;
    }

    if(eccp_pointMul_corner_test(curve, curve_name, vector, vector_num))
    {
        return 1;
    }

    if(eccp_pointMul_rand_test(curve, curve_name))
    {
        return 1;
    }

#ifdef ECCP_POINTMUL_SIMPLE_TEST
    if(eccp_pointMul_invalid_point_test(curve, curve_name))
    {
        return 1;
    }
#endif

    return 0;
}




uint32_t eccp_pointAdd_test_internal(eccp_curve_t *curve, char *curve_name, uint32_t *P1_x, uint32_t *P1_y, uint32_t *P2_x, uint32_t *P2_y,
        uint32_t *expected_Qx, uint32_t *expected_Qy, uint32_t *output_Qx, uint32_t *output_Qy, uint32_t expected_ret, char *info)
{
    uint32_t Qx[ECCP_MAX_WORD_LEN]={0};
    uint32_t Qy[ECCP_MAX_WORD_LEN]={0};
    uint32_t Q2x[ECCP_MAX_WORD_LEN]={0};
    uint32_t Q2y[ECCP_MAX_WORD_LEN]={0};

    uint32_t pWordLen;
    uint32_t ret;

    pWordLen = GET_WORD_LEN(curve->eccp_p_bitLen);

    /****************** P1+P2 ******************/
    ret = eccp_pointAdd(curve, P1_x, P1_y, P2_x, P2_y, Qx, Qy);
    if(expected_ret != ret)
    {
        printf("\r\n %s eccp pointAdd %s error 1, ret=%x", curve_name, info, ret);
        return 1;
    }

    if((PKE_SUCCESS == ret) && (NULL != expected_Qx) && (NULL != expected_Qy))
    {
        if(uint32_BigNumCmp(Qx, pWordLen, expected_Qx, pWordLen) ||
            uint32_BigNumCmp(Qy, pWordLen, expected_Qy, pWordLen))
        {
            printf("\r\n %s eccp pointAdd %s error 2, ret=%x", curve_name, info, ret);
            print_BN_buf_U32(P1_x, pWordLen, "P1_x");
            print_BN_buf_U32(P1_y, pWordLen, "P1_y");
            print_BN_buf_U32(P2_x, pWordLen, "P2_x");
            print_BN_buf_U32(P2_y, pWordLen, "P2_y");
            print_BN_buf_U32(Qx, pWordLen, "Qx");
            print_BN_buf_U32(Qy, pWordLen, "Qy");
            printf("\r\n ret=%x(0:valid, other:invalid)\r\n", eccp_pointVerify(curve, Qx, Qy));
            return 1;
        }
    }

    /****************** P2+P1 ******************/
    ret = eccp_pointAdd(curve, P2_x, P2_y, P1_x, P1_y, Q2x, Q2y);
    if(expected_ret != ret)
    {
        printf("\r\n %s eccp pointAdd %s error 3, ret=%x", curve_name, info, ret);
        return 1;
    }

    if(PKE_SUCCESS == ret)
    {
        if((NULL != expected_Qx) && (NULL != expected_Qy))
        {
            if(uint32_BigNumCmp(Q2x, pWordLen, expected_Qx, pWordLen) ||
                uint32_BigNumCmp(Q2y, pWordLen, expected_Qy, pWordLen))
            {
                printf("\r\n %s eccp pointAdd %s error 4, ret=%x", curve_name, info, ret);
                print_BN_buf_U32(Q2x, pWordLen, "Q2x");
                print_BN_buf_U32(Q2y, pWordLen, "Q2y");
                return 1;
            }
        }

        if(uint32_BigNumCmp(Q2x, pWordLen, Qx, pWordLen) ||
                uint32_BigNumCmp(Q2y, pWordLen, Qy, pWordLen))
        {
            printf("\r\n %s eccp pointAdd %s error 5, ret=%x", curve_name, info, ret);
            print_BN_buf_U32(Qx, pWordLen, "Qx");
            print_BN_buf_U32(Qy, pWordLen, "Qy");
            print_BN_buf_U32(Q2x, pWordLen, "Q2x");
            print_BN_buf_U32(Q2y, pWordLen, "Q2y");
            return 1;
        }
    }

    if(NULL != info)
    {
        printf("\r\n %s eccp pointAdd %s success", curve_name, info);
    }

    if(NULL != output_Qx)
    {
        uint32_copy(output_Qx, Qx, pWordLen);
    }
    if(NULL != output_Qy)
    {
        uint32_copy(output_Qy, Qy, pWordLen);
    }

    return 0;
}

typedef struct {
    uint32_t *P1_x;
    uint32_t *P1_y;
    uint32_t *P2_x;
    uint32_t *P2_y;
    uint32_t *Q_x;
    uint32_t *Q_y;

} pke_eccp_pointAdd_vector_t;

uint32_t eccp_pointAdd_std_test(eccp_curve_t *curve, char *curve_name, pke_eccp_pointAdd_vector_t *vector, uint32_t vector_num)
{
    uint32_t i;

    for(i=0;i<vector_num;i++)
    {
        printf("\r\n\r\n---- %s pointAdd standard data test %u ----", curve_name, i+1);

        if(eccp_pointAdd_test_internal(curve, curve_name, vector->P1_x, vector->P1_y, vector->P2_x, vector->P2_y, vector->Q_x, vector->Q_y,
                NULL, NULL, PKE_SUCCESS, "standard data"))
        {
            return 1;
        }

        vector++;
    }

    return 0;
}


uint32_t eccp_pointAdd_corner_test_internal(eccp_curve_t *curve, char *curve_name, uint32_t *Px, uint32_t *Py, uint8_t info)
{
    uint32_t Py1[ECCP_MAX_WORD_LEN];
    uint32_t Qx[ECCP_MAX_WORD_LEN];
    uint32_t Qy[ECCP_MAX_WORD_LEN];

    uint32_t pWordLen;

    pWordLen = GET_WORD_LEN(curve->eccp_p_bitLen);

    //P+P
    if(eccp_pointAdd_test_internal(curve, curve_name, Px, Py, Px, Py, Qx, Qy, NULL, NULL, PKE_NO_MODINV, NULL))
    {
        return 1;
    }

    //get (-Py)
    pke_sub(curve->eccp_p, Py, Py1, pWordLen);

    //P+(-P)
    if(eccp_pointAdd_test_internal(curve, curve_name, Px, Py, Px, Py1, Qx, Qy, NULL, NULL, PKE_NO_MODINV, NULL))
    {
        return 1;
    }

    if(info)
    {
        printf(" %s pointAdd corner test(P+P & P+(-P)) success ", curve_name);
    }

    return 0;
}


uint32_t eccp_pointAdd_corner_test(eccp_curve_t *curve, char *curve_name)
{
    uint32_t k[ECCP_MAX_WORD_LEN];
    uint32_t P_x[ECCP_MAX_WORD_LEN];
    uint32_t P_y[ECCP_MAX_WORD_LEN];
    uint32_t nWordLen, i;

    nWordLen = GET_WORD_LEN(curve->eccp_n_bitLen);

    //test G
    printf("\r\n\r\n---- %s pointAdd corner test(P+P & P+(-P), P is G) ----\r\n", curve_name);
    if(eccp_pointAdd_corner_test_internal(curve, curve_name, curve->eccp_Gx, curve->eccp_Gy, 1))
    {
        return 1;
    }


    printf("\r\n\r\n---- %s pointAdd corner test(P+P & P+(-P), P is a random valid point) ----\r\n", curve_name);
    for(i=0;i<100;i++)
    {
        printf(" %u", i+1);

        //get a random point P
        get_rand((uint8_t *)k, nWordLen<<2);
        if(eccp_pointMul_test_internal(curve, curve_name, k, curve->eccp_Gx, curve->eccp_Gy, NULL, NULL, P_x, P_y, PKE_SUCCESS, NULL))
        {
            return 1;
        }

        //test a random valid point
        if(eccp_pointAdd_corner_test_internal(curve, curve_name, P_x, P_y, 0))
        {
            return 1;
        }
    }

    return 0;
}


uint32_t eccp_pointAdd_rand_test(eccp_curve_t *curve, char *curve_name)
{
    uint32_t k[ECCP_MAX_WORD_LEN];
    uint32_t k1[ECCP_MAX_WORD_LEN];
    uint32_t k2[ECCP_MAX_WORD_LEN];
    uint32_t P_x[ECCP_MAX_WORD_LEN];
    uint32_t P_y[ECCP_MAX_WORD_LEN];
    uint32_t k1P_x[ECCP_MAX_WORD_LEN];
    uint32_t k1P_y[ECCP_MAX_WORD_LEN];
    uint32_t k2P_x[ECCP_MAX_WORD_LEN];
    uint32_t k2P_y[ECCP_MAX_WORD_LEN];
    uint32_t Q_x[ECCP_MAX_WORD_LEN];
    uint32_t Q_y[ECCP_MAX_WORD_LEN];
    uint32_t nWordLen, i;
    //uint32_t ret;

    printf("\r\n\r\n---- eccp pointAdd rand test([k]P=[k1]P+[k-k1]P) ----\r\n");

    nWordLen = GET_WORD_LEN(curve->eccp_n_bitLen);

    for(i=0;i<100;i++)
    {
        printf(" %u", i+1);

        //get a random point P
        get_rand((uint8_t *)k, nWordLen<<2);
        if(eccp_pointMul_test_internal(curve, curve_name, k, curve->eccp_Gx, curve->eccp_Gy, NULL, NULL, P_x, P_y, PKE_SUCCESS, NULL))
        {
            return 1;
        }

        //random scalar k
        get_rand((uint8_t *)k, nWordLen<<2);
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
        pke_mod(k, nWordLen, curve->eccp_n, curve->eccp_n_h, curve->eccp_p_n0, nWordLen, k);
#else
        pke_mod(k, nWordLen, curve->eccp_n, curve->eccp_n_h, nWordLen, k);
#endif

        //get [k]P
        if(eccp_pointMul_test_internal(curve, curve_name, k, P_x, P_y, NULL, NULL, Q_x, Q_y, PKE_SUCCESS, NULL))
        {
            return 1;
        }

        //random scalar k1
        get_rand((uint8_t *)k1, nWordLen<<2);
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
        pke_mod(k1, nWordLen, curve->eccp_n, curve->eccp_n_h, curve->eccp_p_n0, nWordLen, k1);
#else
        pke_mod(k1, nWordLen, curve->eccp_n, curve->eccp_n_h, nWordLen, k1);
#endif

        //k2 = k-k1 mod n
        pke_modsub(curve->eccp_n, k,k1,k2,nWordLen);

        //get [k1]P
        if(eccp_pointMul_test_internal(curve, curve_name, k1, P_x, P_y, NULL, NULL, k1P_x, k1P_y, PKE_SUCCESS, NULL))
        {
            return 1;
        }

        //get [k2]P
        if(eccp_pointMul_test_internal(curve, curve_name, k2, P_x, P_y, NULL, NULL, k2P_x, k2P_y, PKE_SUCCESS, NULL))
        {
            return 1;
        }

        //[k]P=[k1]P+[k-k1]P
        if(eccp_pointAdd_test_internal(curve, curve_name, k1P_x, k1P_y, k2P_x, k2P_y, Q_x, Q_y, NULL, NULL, PKE_SUCCESS, NULL))
        {
            return 1;
        }
    }

    printf("\r\n");
    return 0;
}


uint32_t eccp_pointAdd_invalid_point_test(eccp_curve_t *curve, char *curve_name)
{
    uint32_t k[ECCP_MAX_WORD_LEN];
    uint32_t P1x[ECCP_MAX_WORD_LEN];
    uint32_t P1y[ECCP_MAX_WORD_LEN];
    uint32_t P2x[ECCP_MAX_WORD_LEN];
    uint32_t P2y[ECCP_MAX_WORD_LEN];
    uint32_t Qx[ECCP_MAX_WORD_LEN];
    uint32_t Qy[ECCP_MAX_WORD_LEN];

    uint32_t pWordLen, nWordLen, i;
    uint32_t ret;

    printf("\r\n\r\n---- %s pointAdd invalid point test ----\r\n", curve_name);

    pWordLen = GET_WORD_LEN(curve->eccp_p_bitLen);
    nWordLen = GET_WORD_LEN(curve->eccp_n_bitLen);

    for(i=0;i<100;i++)
    {
        printf(" %u", i+1);

        /******** test a random invalid point(P+P & P+(-P)) ********/
        get_rand((uint8_t *)P1x, pWordLen<<2);
        get_rand((uint8_t *)P1y, pWordLen<<2);
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
        ret = pke_mod(P1x, pWordLen, curve->eccp_p, curve->eccp_p_h, curve->eccp_p_n0, pWordLen, P1x);
        ret |= pke_mod(P1y, pWordLen, curve->eccp_p, curve->eccp_p_h, curve->eccp_p_n0, pWordLen, P1y);
#else
        ret = pke_mod(P1x, pWordLen, curve->eccp_p, curve->eccp_p_h, pWordLen, P1x);
        ret |= pke_mod(P1y, pWordLen, curve->eccp_p, curve->eccp_p_h, pWordLen, P1y);
#endif
        if(ret)
        {
            return 1;
        }

        if(eccp_pointAdd_corner_test_internal(curve, curve_name, P1x, P1y, 0))
        {
            return 1;
        }

        /******** test two random invalid point(output invalid point) ********/
        get_rand((uint8_t *)P2x, pWordLen<<2);
        get_rand((uint8_t *)P2y, pWordLen<<2);
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
        ret = pke_mod(P2x, pWordLen, curve->eccp_p, curve->eccp_p_h, curve->eccp_p_n0, pWordLen, P2x);
        ret |= pke_mod(P2y, pWordLen, curve->eccp_p, curve->eccp_p_h, curve->eccp_p_n0, pWordLen, P2y);
#else
        ret = pke_mod(P2x, pWordLen, curve->eccp_p, curve->eccp_p_h, pWordLen, P2x);
        ret |= pke_mod(P2y, pWordLen, curve->eccp_p, curve->eccp_p_h, pWordLen, P2y);
#endif
        if(ret)
        {
            return 1;
        }

        ret = eccp_pointAdd_test_internal(curve, curve_name, P1x, P1y, P2x, P2y, NULL, NULL, Qx, Qy, PKE_SUCCESS, NULL);
        if(ret)
        {
            return 1;
        }

        if(PKE_SUCCESS == eccp_pointVerify(curve, Qx, Qy))
        {
            return 1;
        }

        /******** test an invalid point add a valid point(output invalid point) ********/
        get_rand((uint8_t *)k, nWordLen);
        ret = eccp_pointMul_test_internal(curve, curve_name, k, curve->eccp_Gx, curve->eccp_Gy, NULL, NULL, P2x, P2y, PKE_SUCCESS, NULL);
        if(ret)
        {
            return 1;
        }

        ret = eccp_pointAdd_test_internal(curve, curve_name, P1x, P1y, P2x, P2y, NULL, NULL, Qx, Qy, PKE_SUCCESS, NULL);
        if(ret)
        {
            return 1;
        }

        if(PKE_SUCCESS == eccp_pointVerify(curve, Qx, Qy))
        {
            return 1;
        }

        /******** test an invalid point add a valid point(no output) ********/
        uint32_copy(P1x, P2x, pWordLen);
        ret = eccp_pointAdd_test_internal(curve, curve_name, P1x, P1y, P2x, P2y, NULL, NULL, NULL, NULL, PKE_NO_MODINV, NULL);
        if(ret)
        {
            return 1;
        }
    }

    return 0;
}


uint32_t eccp_pointAdd_test(eccp_curve_t *curve, char *curve_name, pke_eccp_pointAdd_vector_t *vector, uint32_t vector_num)
{
    printf("\r\n\r\n ***************************** %s pointAdd test ***************************** ", curve_name);

    if(eccp_pointAdd_std_test(curve, curve_name, vector, vector_num))
    {
        return 1;
    }

    if(eccp_pointAdd_corner_test(curve, curve_name))
    {
        return 1;
    }

    if(eccp_pointAdd_rand_test(curve, curve_name))
    {
        return 1;
    }

    if(eccp_pointAdd_invalid_point_test(curve, curve_name))
    {
        return 1;
    }

    return 0;
}




#ifdef ECCP_POINT_DOUBLE
uint32_t eccp_pointDouble_test_internal(eccp_curve_t *curve, char *curve_name, uint32_t *Px, uint32_t *Py, uint32_t *expected_Qx, uint32_t *expected_Qy,
        uint32_t *output_Qx, uint32_t *output_Qy, uint32_t expected_ret, char *info)
{
    uint32_t Qx[ECCP_MAX_WORD_LEN]={0};
    uint32_t Qy[ECCP_MAX_WORD_LEN]={0};

    uint32_t pWordLen;
    uint32_t ret;

    pWordLen = GET_WORD_LEN(curve->eccp_p_bitLen);

    ret = eccp_pointDouble(curve, Px, Py, Qx, Qy);
    if(expected_ret != ret)
    {
        printf("\r\n %s eccp eccp_pointDouble %s error 1, ret=%x", curve_name, info, ret);
        return 1;
    }

    if((PKE_SUCCESS == ret) && (NULL != expected_Qx) && (NULL != expected_Qy))
    {
        if(uint32_BigNumCmp(Qx, pWordLen, expected_Qx, pWordLen) ||
            uint32_BigNumCmp(Qy, pWordLen, expected_Qy, pWordLen))
        {
            printf("\r\n %s eccp eccp_pointDouble %s error 2, ret=%x", curve_name, info, ret);
            print_BN_buf_U32(Qx, pWordLen, "Qx");
            print_BN_buf_U32(Qy, pWordLen, "Qy");
            return 1;
        }
    }

    if(NULL != info)
    {
        printf("\r\n %s eccp eccp_pointDouble %s success", curve_name, info);
    }

    if(NULL != output_Qx)
    {
        uint32_copy(output_Qx, Qx, pWordLen);
    }
    if(NULL != output_Qy)
    {
        uint32_copy(output_Qy, Qy, pWordLen);
    }

    return 0;
}


uint32_t eccp_pointDouble_std_test(eccp_curve_t *curve, char *curve_name, pke_eccp_pointMul_vector_t *vector, uint32_t vector_num)
{
    uint32_t i;

    for(i=0;i<vector_num;i++)
    {
        printf("\r\n\r\n---- %s pointDouble standard data test %u----", curve_name, i+1);

        if(eccp_pointDouble_test_internal(curve, curve_name, vector->P_x, vector->P_y, vector->dbl_P_x, vector->dbl_P_y,
                NULL, NULL, PKE_SUCCESS, "standard data"))
        {
            return 1;
        }

        vector++;
    }

    return 0;
}


uint32_t eccp_pointDouble_property_test_internal(eccp_curve_t *curve, char *curve_name, uint32_t *Px, uint32_t *Py,
        uint32_t *expected_Qx, uint32_t *expected_Qy, uint8_t info)
{
    uint32_t k1[ECCP_MAX_WORD_LEN]={0};
    uint32_t k2[ECCP_MAX_WORD_LEN]={0};
    uint32_t P1x[ECCP_MAX_WORD_LEN];
    uint32_t P1y[ECCP_MAX_WORD_LEN];
    uint32_t P2x[ECCP_MAX_WORD_LEN];
    uint32_t P2y[ECCP_MAX_WORD_LEN];
    uint32_t Qx[ECCP_MAX_WORD_LEN]={0};
    uint32_t Qy[ECCP_MAX_WORD_LEN]={0};

    uint32_t pWordLen, nWordLen;
    //uint32_t ret;

    pWordLen = GET_WORD_LEN(curve->eccp_p_bitLen);
    nWordLen = GET_WORD_LEN(curve->eccp_n_bitLen);

    /**************** [2]P=[k]P+(-[k-2]P) ****************/
    get_rand((uint8_t *)k1, nWordLen<<2);
    k2[0]=2;
    pke_sub(k1, k2, k2, nWordLen);

    //[k]P
    if(eccp_pointMul_test_internal(curve, curve_name, k1, Px, Py, NULL, NULL, P1x, P1y, PKE_SUCCESS, NULL))
    {
        return 1;
    }

    //[k-2]P
    if(eccp_pointMul_test_internal(curve, curve_name, k2, Px, Py, NULL, NULL, P2x, P2y, PKE_SUCCESS, NULL))
    {
        return 1;
    }

    //-[k-2]P
    pke_sub(curve->eccp_p, P2y, P2y, pWordLen);

    //[k]P+(-[k-2]P)
    if(eccp_pointAdd_test_internal(curve, curve_name, P1x, P1y, P2x, P2y, NULL, NULL, P1x, P1y, PKE_SUCCESS, NULL))
    {
        return 1;
    }

    //[2]P
    if(eccp_pointDouble_test_internal(curve, curve_name, Px, Py, NULL, NULL, Qx, Qy, PKE_SUCCESS, NULL))
    {
        return 1;
    }

    if(uint32_BigNumCmp(Qx, pWordLen, P1x, pWordLen) ||
        uint32_BigNumCmp(Qy, pWordLen, P1y, pWordLen))
    {
        printf("\r\n %s eccp eccp_pointDouble error 2", curve_name);
        print_BN_buf_U32(Qx, pWordLen, "Qx");
        print_BN_buf_U32(Qy, pWordLen, "Qy");
        return 1;
    }

    if(expected_Qx && expected_Qy)
    {
        if(uint32_BigNumCmp(Qx, pWordLen, expected_Qx, pWordLen) ||
            uint32_BigNumCmp(Qy, pWordLen, expected_Qy, pWordLen))
        {
            printf("\r\n %s eccp eccp_pointDouble error 3", curve_name);
            print_BN_buf_U32(Qx, pWordLen, "Qx");
            print_BN_buf_U32(Qy, pWordLen, "Qy");
            return 1;
        }
    }


    /**************** [2]P=-[2](-P) ****************/
    pke_sub(curve->eccp_p, Py, P2y, pWordLen);

    //[2](-P)
    if(eccp_pointDouble_test_internal(curve, curve_name, Px, P2y, NULL, NULL, P2x, P2y, PKE_SUCCESS, NULL))
    {
        return 1;
    }

    pke_sub(curve->eccp_p, P2y, P2y, pWordLen);

    if(uint32_BigNumCmp(Qx, pWordLen, P2x, pWordLen) ||
        uint32_BigNumCmp(Qy, pWordLen, P2y, pWordLen))
    {
        printf("\r\n %s eccp eccp_pointDouble error 4", curve_name);
        print_BN_buf_U32(Qx, pWordLen, "Qx");
        print_BN_buf_U32(Qy, pWordLen, "Qy");
        return 1;
    }

    if(info)
    {
        printf("\r\n %s eccp eccp_pointDouble property test success", curve_name);
    }

    return 0;
}


uint32_t eccp_pointDouble_property_test(eccp_curve_t *curve, char *curve_name, pke_eccp_pointMul_vector_t *vector, uint32_t vector_num)
{
    uint32_t k1[ECCP_MAX_WORD_LEN]={0};
    uint32_t Px[ECCP_MAX_WORD_LEN];
    uint32_t Py[ECCP_MAX_WORD_LEN];

    uint32_t nWordLen, i;
    //uint32_t ret;

    nWordLen = GET_WORD_LEN(curve->eccp_n_bitLen);

    printf("\r\n\r\n==== %s pointDouble property test ([2]P=[k]P+(-[k-2]P) & [2]P=-[2](-P)) ====", curve_name);

    for(i=0; i<vector_num; i++)
    {
        printf("\r\n\r\n---- %s pointDouble property test %u (input standard point)----", curve_name, i+1);

        if(eccp_pointDouble_property_test_internal(curve, curve_name, vector->P_x, vector->P_y, vector->dbl_P_x, vector->dbl_P_y, 1))
        {
            return 1;
        }
    }

    printf("\r\n\r\n---- %s pointDouble property test (input random valid point)---- \r\n", curve_name);
    for(i=0;i<100;i++)
    {
        printf(" %u", i+1);

        get_rand((uint8_t *)k1, nWordLen<<2);

        //[k]P
        if(eccp_pointMul_test_internal(curve, curve_name, k1, curve->eccp_Gx, curve->eccp_Gy, NULL, NULL, Px, Py, PKE_SUCCESS, NULL))
        {
            return 1;
        }

        if(eccp_pointDouble_property_test_internal(curve, curve_name, Px, Py, NULL, NULL, 0))
        {
            return 1;
        }
    }

    return 0;
}


uint32_t eccp_pointDouble_invalid_point_test(eccp_curve_t *curve, char *curve_name)
{
    uint32_t k[ECCP_MAX_WORD_LEN];
    uint32_t b[ECCP_MAX_WORD_LEN];
    uint32_t P_x[ECCP_MAX_WORD_LEN];
    uint32_t P_y[ECCP_MAX_WORD_LEN];
    uint32_t Q_x[ECCP_MAX_WORD_LEN];
    uint32_t Q_y[ECCP_MAX_WORD_LEN];
    uint32_t *bak_b;

    uint32_t pWordLen, i=0;
    uint32_t ret;

    printf("\r\n\r\n---- %s pointDouble invalid point test ----\r\n", curve_name);

    pWordLen = GET_WORD_LEN(curve->eccp_p_bitLen);

    bak_b = curve->eccp_b;

    for(i=0;i<100;i++)
    {
        printf(" %u", i+1);

        /******** no output ********/
        get_rand((uint8_t *)P_x, pWordLen<<2);
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
        ret = pke_mod(P_x, pWordLen, curve->eccp_p, curve->eccp_p_h, curve->eccp_p_n0, pWordLen, P_x);
#else
        ret = pke_mod(P_x, pWordLen, curve->eccp_p, curve->eccp_p_h, pWordLen, P_x);
#endif
        if(ret)
        {
            printf("\r\n mod error 1, ret = %x", ret);
            return 1;
        }
        uint32_clear(P_y, pWordLen);

        if(eccp_pointDouble_test_internal(curve, curve_name, P_x, P_y, NULL, NULL, NULL, NULL, PKE_NO_MODINV, NULL))
        {
            printf("\r\n %s eccp pointDouble invalid input point test error 1, ret = %x", curve_name, ret);
            return 1;
        }

        /******** output invalid point ********/
        get_rand((uint8_t *)P_y, pWordLen<<2);
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
        ret = pke_mod(P_y, pWordLen, curve->eccp_p, curve->eccp_p_h, curve->eccp_p_n0, pWordLen, P_y);
#else
        ret = pke_mod(P_y, pWordLen, curve->eccp_p, curve->eccp_p_h, pWordLen, P_y);
#endif
        if(ret)
        {
            printf("\r\n mod error 2, ret = %x", ret);
            return 1;
        }

        //get new curve's parameter b
#if (defined(PKE_LP) || defined(AIC_PKE_SECURE))
        pke_set_exe_cfg(PKE_EXE_CFG_ALL_NON_MONT);
#endif
        ret = pke_modmul_internal(P_x, P_x, k, pWordLen);                   //k = x^2
        ret |= pke_modadd(curve->eccp_p, k, curve->eccp_a, k, pWordLen);    //k = x^2+a
        ret |= pke_modmul_internal(P_x, k, k, pWordLen);                    //k = x(x^2+a) = x^3+ax
        ret |= pke_modmul_internal(P_y, P_y, b, pWordLen);
        ret |= pke_modsub(curve->eccp_p, b, k, b, pWordLen);                //b = y^2 - (x^3+ax)
        if(ret)
        {
            printf("\r\n b calc error, ret = %x", ret);
            return 1;
        }

        if(eccp_pointDouble_test_internal(curve, curve_name, P_x, P_y, NULL, NULL, Q_x, Q_y, PKE_SUCCESS, NULL))
        {
            printf("\r\n %s eccp pointDouble invalid input point test error 2, ret = %x", curve_name, ret);
            return 1;
        }

        if(PKE_SUCCESS == eccp_pointVerify(curve, Q_x, Q_y))
        {
            printf("\r\n %s eccp pointDouble invalid input point test error 3, ret = %x", curve_name, ret);
            return 1;
        }

        //change b
        curve->eccp_b = b;

        //test output point using new curve
        ret = eccp_pointVerify(curve, Q_x, Q_y);
        if(ret)
        {
            printf("\r\n %s eccp pointDouble invalid input point test error 4, ret = %x", curve_name, ret);

            //recover b
            curve->eccp_b = bak_b;
            return 1;
        }

        //recover b
        curve->eccp_b = bak_b;
    }

    return 0;
}


uint32_t eccp_pointDouble_test(eccp_curve_t *curve, char *curve_name, pke_eccp_pointMul_vector_t *vector, uint32_t vector_num)
{
    printf("\r\n\r\n ***************************** %s pointDouble test ***************************** ", curve_name);

    if(eccp_pointDouble_std_test(curve, curve_name, vector, vector_num))
    {
        return 1;
    }

    if(eccp_pointDouble_property_test(curve, curve_name, vector, vector_num))
    {
        return 1;
    }

    if(eccp_pointDouble_invalid_point_test(curve, curve_name))
    {
        return 1;
    }

    return 0;
}
#endif



// P=[k]G, Q=[2]G
uint32_t eccp_std_test(eccp_curve_t *curve, char *curve_name, uint32_t *k, uint32_t *Px, uint32_t *Py, uint32_t *Qx, uint32_t *Qy)
{
    uint32_t x1[ECCP_MAX_WORD_LEN];
    uint32_t y1[ECCP_MAX_WORD_LEN];
    uint32_t x2[ECCP_MAX_WORD_LEN];
    uint32_t y2[ECCP_MAX_WORD_LEN];
    uint32_t x3[ECCP_MAX_WORD_LEN];
    uint32_t y3[ECCP_MAX_WORD_LEN];
    uint32_t k1[ECCP_MAX_WORD_LEN];
    uint32_t k2[ECCP_MAX_WORD_LEN];
    uint32_t pWordLen, nWordLen;
    uint32_t ret;

    printf("\r\n\r\n ***************************** %s standard data test ***************************** ", curve_name);

    pWordLen = GET_WORD_LEN(curve->eccp_p_bitLen);
    nWordLen = GET_WORD_LEN(curve->eccp_n_bitLen);

    //test point multiple
    ret = eccp_pointMul_(curve, k, curve->eccp_Gx, curve->eccp_Gy, x1, y1);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(x1, pWordLen, Px, pWordLen) || uint32_BigNumCmp(y1, pWordLen, Py, pWordLen))
    {
        printf("\r\n %s eccp_pointMul failure, ret=%x", curve_name,ret);
        print_BN_buf_U32(x1, pWordLen, "x1");
        print_BN_buf_U32(y1, pWordLen, "y1");
        return 1;
    }
    else
    {
        printf("\r\n %s eccp_pointMul success", curve_name);
    }

    //test point double
#ifdef ECCP_POINT_DOUBLE
    ret = eccp_pointDouble(curve, curve->eccp_Gx, curve->eccp_Gy, x1, y1);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(x1, pWordLen, Qx, pWordLen) || uint32_BigNumCmp(y1, pWordLen, Qy, pWordLen))
    {
        printf("\r\n %s eccp_pointDouble failure, ret=%x", curve_name,ret);
        print_BN_buf_U32(x1, pWordLen, "x1");
        print_BN_buf_U32(y1, pWordLen, "y1");
        return 1;
    }
    else
    {
        printf("\r\n %s eccp_pointDouble success", curve_name);
    }
#endif

    //test valid point
    ret = eccp_pointVerify(curve, x1, y1);
    if(PKE_SUCCESS != ret)
    {
        printf("\r\n %s eccp_pointVerify test 1 failure, ret=%x", curve_name,ret);
        print_BN_buf_U32(x1, pWordLen, "x1");
        print_BN_buf_U32(y1, pWordLen, "y1");
        return 1;
    }
    else
    {
        printf("\r\n %s eccp_pointVerify test 1 success", curve_name);
    }

    //test invalid point
    x1[0]-=1;
    ret = eccp_pointVerify(curve, x1, y1);
    if(PKE_NOT_ON_CURVE != ret)
    {
        printf("\r\n %s eccp_pointVerify test 2 failure, ret=%x", curve_name,ret);
        print_BN_buf_U32(x1, pWordLen, "x1");
        print_BN_buf_U32(y1, pWordLen, "y1");
        return 1;
    }
    else
    {
        printf("\r\n %s eccp_pointVerify test 2 success", curve_name);
    }


    //make k = k1+k2 mod n, test [k]G = [k1]G+[k2]G
    get_rand((uint8_t *)k1, nWordLen<<2);
    k1[nWordLen-1] = 0;
    ret = pke_modsub(curve->eccp_n, k, k1, k2, nWordLen);
    ret |= eccp_pointMul_(curve, k1, curve->eccp_Gx, curve->eccp_Gy, x1, y1);
    ret |= eccp_pointMul_(curve, k2, curve->eccp_Gx, curve->eccp_Gy, x2, y2);
    ret |= eccp_pointAdd(curve, x1, y1, x2, y2, x1, y1);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(x1, pWordLen, Px, pWordLen) || uint32_BigNumCmp(y1, pWordLen, Py, pWordLen))
    {
        printf("\r\n %s eccp_pointAdd failure, ret=%x", curve_name,ret);
        print_BN_buf_U32(x1, pWordLen, "x1");
        print_BN_buf_U32(y1, pWordLen, "y1");
        return 1;
    }
    else
    {
        printf("\r\n %s eccp_pointAdd success", curve_name);
    }

#if !(defined(PKE_LP) || defined(AIC_PKE_SECURE))

#if 0
    //test [k]G = [k1]G+[k2]G, here k = k1+k2 mod n
    //caution: it is not suggested to do this when the two input point are the same, it may return PKE_NO_MODINV,
    //         because it does point addtion, but G + G is point double!

    uint32_clear(x1, pWordLen);
    uint32_clear(y1, pWordLen);
    ret = eccp_pointMul_Shamir(curve, k1, curve->eccp_Gx, curve->eccp_Gy,
                                     k2, curve->eccp_Gx, curve->eccp_Gy,
                                     x1, y1);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(x1, wordLen, Px, wordLen) || uint32_BigNumCmp(y1, wordLen, Py, wordLen))
    {
        printf("\r\n %s eccp_pointMul_Shamir test 1 failure, ret=%x", curve_name,ret);
        print_BN_buf_U32(x1, pWordLen, "x1");
        print_BN_buf_U32(y1, pWordLen, "y1");
        return 1;
    }
    else
    {
        printf("\r\n %s eccp_pointMul_Shamir test 1 success", curve_name);
    }
#endif

    //test [k]G = [k1]P1+[k2]G, here k = k1||k2, P1 = [2^bitLen]G, bitLen = 1 + bit length of k2
    uint32_clear(x1, pWordLen);
    uint32_clear(y1, pWordLen);
    ret = eccp_pointMul_base(curve, k, x1, y1);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(x1, pWordLen, Px, pWordLen) || uint32_BigNumCmp(y1, pWordLen, Py, pWordLen))
    {
        printf("\r\n %s eccp_pointMul_Shamir test 2 failure, ret=%x", curve_name,ret);
        print_BN_buf_U32(x1, pWordLen, "x1");
        print_BN_buf_U32(y1, pWordLen, "y1");
        return 1;
    }
    else
    {
        printf("\r\n %s eccp_pointMul_Shamir test 2 success", curve_name);
    }


    //test [k]G = [k1*k2]G + [k3]G = [k1]([k2]G) + [k3]G,   k3 = k-(k1*k2) mod n
    pke_modmul(curve->eccp_n, k1, k2, k2, nWordLen);
    pke_modsub(curve->eccp_n, k, k2, k2, nWordLen);
    ret = eccp_pointMul_Shamir(curve, k1, x2, y2,
                                 k2, curve->eccp_Gx, curve->eccp_Gy,
                                 x1, y1);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(x1, pWordLen, Px, pWordLen) || uint32_BigNumCmp(y1, pWordLen, Py, pWordLen))
    {
        printf("\r\n %s eccp_pointMul_Shamir test 3 failure, ret=%x", curve_name,ret);
        print_BN_buf_U32(x1, pWordLen, "x1");
        print_BN_buf_U32(y1, pWordLen, "y1");
        return 1;
    }
    else
    {
        printf("\r\n %s eccp_pointMul_Shamir test 3 success ", curve_name);
    }
#endif

#if 1
    //test point multiple [0]G
    uint32_clear(k1, nWordLen);
    ret = eccp_pointMul_(curve, k1, curve->eccp_Gx, curve->eccp_Gy, x1, y1);
    if(PKE_NO_MODINV != ret)
    {
        printf("\r\n %s eccp_pointMul [0]G failure, ret=%x", curve_name,ret);
        return 1;
    }
    else
    {
        printf("\r\n %s eccp_pointMul [0]G success", curve_name);
    }
#endif

    //test point multiple [1]G
    pke_set_operand_uint32_value(k1, nWordLen, 1);
    uint32_clear(x1, pWordLen);
    uint32_clear(y1, pWordLen);
    ret = eccp_pointMul_(curve, k1, curve->eccp_Gx, curve->eccp_Gy, x1, y1);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(x1, pWordLen, curve->eccp_Gx, pWordLen) || uint32_BigNumCmp(y1, pWordLen, curve->eccp_Gy, pWordLen))
    {
        printf("\r\n %s eccp_pointMul [1]G failure, ret=%x", curve_name,ret);
        print_BN_buf_U32(x1, pWordLen, "x1");
        print_BN_buf_U32(y1, pWordLen, "y1");
        return 1;
    }
    else
    {
        printf("\r\n %s eccp_pointMul [1]G success", curve_name);
    }

#if 1
    //test point multiple [n]G
    ret = eccp_pointMul_(curve, curve->eccp_n, curve->eccp_Gx, curve->eccp_Gy, x1, y1);
    if(PKE_NO_MODINV != ret)
    {
        printf("\r\n %s eccp_pointMul [n]G failure, ret=%x", curve_name,ret);
        return 1;
    }
    else
    {
        printf("\r\n %s eccp_pointMul [n]G success(return PKE_NO_MODINV)", curve_name);
    }
#endif

    //test point multiple [n+1]G
    uint32_copy(k1, curve->eccp_n, nWordLen);
    k1[0] += 1;
    uint32_clear(x1, pWordLen);
    uint32_clear(y1, pWordLen);
    ret = eccp_pointMul_(curve, k1, curve->eccp_Gx, curve->eccp_Gy, x1, y1);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(x1, pWordLen, curve->eccp_Gx, pWordLen) || uint32_BigNumCmp(y1, pWordLen, curve->eccp_Gy, pWordLen))
    {
        printf("\r\n %s eccp_pointMul [n+1]G failure, ret=%x", curve_name,ret);
        print_BN_buf_U32(x1, pWordLen, "x1");
        print_BN_buf_U32(y1, pWordLen, "y1");
        return 1;
    }
    else
    {
        printf("\r\n %s eccp_pointMul [n+1]G success", curve_name);
    }

    //test point multiple [n-1]G
    uint32_copy(k1, curve->eccp_n, nWordLen);
    k1[0] -= 1;
    uint32_clear(x1, pWordLen);
    uint32_clear(y1, pWordLen);
    ret = pke_sub(curve->eccp_p, curve->eccp_Gy, y2, pWordLen);
    ret |= eccp_pointMul_(curve, k1, curve->eccp_Gx, curve->eccp_Gy, x1, y1);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(x1, pWordLen, curve->eccp_Gx, pWordLen) || uint32_BigNumCmp(y1, pWordLen, y2, pWordLen))
    {
        printf("\r\n %s eccp_pointMul [n-1]G failure, ret=%x", curve_name,ret);
        print_BN_buf_U32(x1, pWordLen, "x1");
        print_BN_buf_U32(y1, pWordLen, "y1");
        return 1;
    }
    else
    {
        printf("\r\n %s eccp_pointMul [n-1]G success", curve_name);
    }

    //test point addition G + (-G)
    ret = eccp_pointAdd(curve, curve->eccp_Gx, curve->eccp_Gy, curve->eccp_Gx, y2, x1, y1);
    if(PKE_NO_MODINV != ret)
    {
        printf("\r\n %s eccp_pointAdd(G+(-G)) failure, ret=%x", curve_name,ret);
        return 1;
    }
    else
    {
        printf("\r\n %s eccp_pointAdd(G+(-G)) success(return PKE_NO_MODINV)", curve_name);
    }

#if !(defined(PKE_LP) || defined(AIC_PKE_SECURE))

    //test [2]P + [3]P, here k1 and k2 have the same bit length.
    pke_set_operand_uint32_value(k1, nWordLen, 2);
    pke_set_operand_uint32_value(k2, nWordLen, 3);
    ret = eccp_pointMul_Shamir(curve, k1, Px, Py,
                                     k2, Px, Py,
                                     x1, y1);
    if(PKE_SUCCESS == ret)
    {
        k1[0] = 5;
        (void)eccp_pointMul_(curve, k1, Px, Py, x2, y2);
        if(uint32_BigNumCmp(x1, pWordLen, x2, pWordLen) || uint32_BigNumCmp(y1, pWordLen, y2, pWordLen))
        {
            printf("\r\n %s eccp_pointMul_Shamir test ([2]P + [3]P) failure, ret=%x", curve_name,ret);
            print_BN_buf_U32(Px, pWordLen, "Px");
            print_BN_buf_U32(Py, pWordLen, "Py");
            print_BN_buf_U32(x2, pWordLen, "standard x");
            print_BN_buf_U32(y2, pWordLen, "standard y");
            print_BN_buf_U32(x1, pWordLen, "x1");
            print_BN_buf_U32(y1, pWordLen, "y1");
            return 1;
        }
        else
        {
            printf("\r\n %s eccp_pointMul_Shamir test ([2]P + [3]P) success", curve_name);
        }
    }
    else if(PKE_NO_MODINV == ret)
    {
        printf("\r\n %s eccp_pointMul_Shamir test ([2]P + [3]P) failure(return PKE_NO_MODINV)", curve_name);
    }
    else
    {
        printf("\r\n %s eccp_pointMul_Shamir test ([2]P + [3]P) failure(return %u)", curve_name, ret);
    }

    //test [4]P + [6](-P), here k1 and k2 have the same bit length.
    pke_set_operand_uint32_value(k1, nWordLen, 4);
    pke_set_operand_uint32_value(k2, nWordLen, 6);
    ret = pke_sub(curve->eccp_p, Py, y2, pWordLen);
    ret |= eccp_pointMul_Shamir(curve, k1, Px, Py,
                                     k2, Px, y2,
                                     x1, y1);
    if(PKE_SUCCESS == ret)
    {
        k1[0] = 2;
        (void)eccp_pointMul_(curve, k1, Px, y2, x2, y2);
        if(uint32_BigNumCmp(x1, pWordLen, x2, pWordLen) || uint32_BigNumCmp(y1, pWordLen, y2, pWordLen))
        {
            printf("\r\n %s eccp_pointMul_Shamir test ([4]P + [6](-P)) failure, ret=%x", curve_name,ret);
            print_BN_buf_U32(Px, pWordLen, "Px");
            print_BN_buf_U32(Py, pWordLen, "Py");
            print_BN_buf_U32(x2, pWordLen, "standard x");
            print_BN_buf_U32(y2, pWordLen, "standard y");
            print_BN_buf_U32(x1, pWordLen, "x1");
            print_BN_buf_U32(y1, pWordLen, "y1");
            return 1;
        }
        else
        {
            printf("\r\n %s eccp_pointMul_Shamir test ([4]P + [6](-P)) success", curve_name);
        }
    }
    else if(PKE_NO_MODINV == ret)
    {
        printf("\r\n %s eccp_pointMul_Shamir test ([4]P + [6](-P)) failure(return PKE_NO_MODINV)", curve_name);
    }
    else
    {
        printf("\r\n %s eccp_pointMul_Shamir test ([4]P + [6](-P)) failure(return %u)", curve_name, ret);
    }

    //test [5]G + [2]([2]G)
    pke_set_operand_uint32_value(k1, nWordLen, 5);
    pke_set_operand_uint32_value(k2, nWordLen, 2);
    ret = eccp_pointMul_Shamir(curve, k1, curve->eccp_Gx, curve->eccp_Gy,
                                     k2, Qx, Qy,
                                     x1, y1);
    if(PKE_SUCCESS == ret)
    {
        k1[0] = 9;
        (void)eccp_pointMul_(curve, k1, curve->eccp_Gx, curve->eccp_Gy, x2, y2);
        if(uint32_BigNumCmp(x1, pWordLen, x2, pWordLen) || uint32_BigNumCmp(y1, pWordLen, y2, pWordLen))
        {
            printf("\r\n %s eccp_pointMul_Shamir test ([5]G + [2]([2]G)) failure, ret=%x", curve_name,ret);
            print_BN_buf_U32(curve->eccp_Gx, pWordLen, "Gx");
            print_BN_buf_U32(curve->eccp_Gy, pWordLen, "Gy");
            print_BN_buf_U32(x2, pWordLen, "standard x");
            print_BN_buf_U32(y2, pWordLen, "standard y");
            print_BN_buf_U32(x1, pWordLen, "x1");
            print_BN_buf_U32(y1, pWordLen, "y1");
            return 1;
        }
        else
        {
            printf("\r\n %s eccp_pointMul_Shamir test ([5]G + [2]([2]G)) success", curve_name);
        }
    }
    else if(PKE_NO_MODINV == ret)
    {
        printf("\r\n %s eccp_pointMul_Shamir test ([5]G + [2]([2]G)) failure(return PKE_NO_MODINV)", curve_name);
    }
    else
    {
        printf("\r\n %s eccp_pointMul_Shamir test ([5]G + [2]([2]G)) failure(return %u)", curve_name, ret);
    }

    //test [11]G + [4](-[2]G)
    pke_set_operand_uint32_value(k1, nWordLen, 11);
    pke_set_operand_uint32_value(k2, nWordLen, 4);
    ret = pke_sub(curve->eccp_p, Qy, y1, pWordLen);
    ret = eccp_pointMul_Shamir(curve, k1, curve->eccp_Gx, curve->eccp_Gy,
                                     k2, Qx, y1,
                                     x1, y1);
    if(PKE_SUCCESS == ret)
    {
        k1[0] = 3;
        (void)eccp_pointMul_(curve, k1, curve->eccp_Gx, curve->eccp_Gy, x2, y2);
        if(uint32_BigNumCmp(x1, pWordLen, x2, pWordLen) || uint32_BigNumCmp(y1, pWordLen, y2, pWordLen))
        {
            printf("\r\n %s eccp_pointMul_Shamir test ([11]G + [4](-[2]G)) failure, ret=%x", curve_name,ret);
            print_BN_buf_U32(curve->eccp_Gx, pWordLen, "Gx");
            print_BN_buf_U32(curve->eccp_Gy, pWordLen, "Gy");
            print_BN_buf_U32(x2, pWordLen, "standard x");
            print_BN_buf_U32(y2, pWordLen, "standard y");
            print_BN_buf_U32(x1, pWordLen, "x1");
            print_BN_buf_U32(y1, pWordLen, "y1");
            return 1;
        }
        else
        {
            printf("\r\n %s eccp_pointMul_Shamir test ([11]G + [4](-[2]G)) success", curve_name);
        }
    }
    else if(PKE_NO_MODINV == ret)
    {
        printf("\r\n %s eccp_pointMul_Shamir test ([11]G + [4](-[2]G)) failure(return PKE_NO_MODINV)", curve_name);
    }
    else
    {
        printf("\r\n %s eccp_pointMul_Shamir test ([11]G + [4](-[2]G)) failure(return %u)", curve_name, ret);
    }

#if 0
    //test [0]G = [0]P+[0]G,  ---error，能计算完毕，结果点竟然是有效点
    uint32_clear(k1, nWordLen);
    uint32_clear(k2, nWordLen);
    ret = eccp_pointMul_Shamir(curve, k1, Px, Py,
                                     k2, curve->eccp_Gx, curve->eccp_Gy,
                                     x1, y1);
    if(PKE_SUCCESS != ret)
    {
        printf("\r\n %s eccp_pointMul_Shamir test ([0]P+[0]G) failure, ret=%x", curve_name,ret);
        print_BN_buf_U32(x1, pWordLen, "x1");
        print_BN_buf_U32(y1, pWordLen, "y1");
        return 1;
    }
    else
    {
        print_BN_buf_U32(x1, pWordLen, "x1");
        print_BN_buf_U32(y1, pWordLen, "y1");
        printf("\r\n %s eccp_pointMul_Shamir test ([0]P+[0]G) success", curve_name);
    }
#endif

    //[0]P+[1]Q = Q
    k1[0] = 0x45cb32a9;
    ret = eccp_pointMul_(curve, k1, curve->eccp_Gx, curve->eccp_Gy, x1, y1);
    uint32_clear(k1, nWordLen);
    pke_set_operand_uint32_value(k2, nWordLen, 1);
    ret |= eccp_pointMul_Shamir(curve, k1, x1, y1,
                                     k2, curve->eccp_Gx, curve->eccp_Gy,
                                     x3, y3);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(x3, pWordLen, curve->eccp_Gx, pWordLen) || uint32_BigNumCmp(y3, pWordLen, curve->eccp_Gy, pWordLen))
    {
        printf("\r\n %s eccp_pointMul_Shamir test ([0]P+[1]Q = Q) failure, ret=%x", curve_name,ret);
        print_BN_buf_U32(x1, pWordLen, "x1");
        print_BN_buf_U32(y1, pWordLen, "y1");
        return 1;
    }
    else
    {
        printf("\r\n %s eccp_pointMul_Shamir test ([0]P+[1]Q = Q) success", curve_name);
    }

    //[1]P+[0]Q = P
    pke_set_operand_uint32_value(k1, nWordLen, 1);
    uint32_clear(k2, nWordLen);
    ret = eccp_pointMul_Shamir(curve, k1, x1, y1,
                                     k2, curve->eccp_Gx, curve->eccp_Gy,
                                     x3, y3);
    if(PKE_SUCCESS != ret || uint32_BigNumCmp(x3, pWordLen, x1, pWordLen) || uint32_BigNumCmp(y3, pWordLen, y1, pWordLen))
    {
        printf("\r\n %s eccp_pointMul_Shamir test ([1]P+[0]Q = P) failure, ret=%x", curve_name,ret);
        print_BN_buf_U32(x1, pWordLen, "x1");
        print_BN_buf_U32(y1, pWordLen, "y1");
        return 1;
    }
    else
    {
        printf("\r\n %s eccp_pointMul_Shamir test ([1]P+[0]Q = P) success", curve_name);
    }

    //[1]P+[1]Q = P+Q
    ret = eccp_pointAdd(curve, x1, y1, curve->eccp_Gx, curve->eccp_Gy, x2, y2);
    pke_set_operand_uint32_value(k1, nWordLen, 1);
    pke_set_operand_uint32_value(k2, nWordLen, 1);
    ret |= eccp_pointMul_Shamir(curve, k1, x1, y1,
                                     k2, curve->eccp_Gx, curve->eccp_Gy,
                                     x3, y3);

    if(PKE_SUCCESS != ret || uint32_BigNumCmp(x3, pWordLen, x2, pWordLen) || uint32_BigNumCmp(y3, pWordLen, y2, pWordLen))
    {
        printf("\r\n %s eccp_pointMul_Shamir test ([1]P+[1]Q = P+Q) failure, ret=%x", curve_name,ret);
        print_BN_buf_U32(x1, pWordLen, "x1");
        print_BN_buf_U32(y1, pWordLen, "y1");
        return 1;
    }
    else
    {
        printf("\r\n %s eccp_pointMul_Shamir test ([1]P+[1]Q = P+Q) success", curve_name);
    }

    //[1]P+[n]Q = P
    pke_set_operand_uint32_value(k1, nWordLen, 1);
    ret = eccp_pointMul_Shamir(curve, k1, x1, y1,
                                     curve->eccp_n, curve->eccp_Gx, curve->eccp_Gy,
                                     x3, y3);

    if(PKE_SUCCESS != ret || uint32_BigNumCmp(x3, pWordLen, x1, pWordLen) || uint32_BigNumCmp(y3, pWordLen, y1, pWordLen))
    {
        printf("\r\n %s eccp_pointMul_Shamir test ([1]P+[n]Q = P) failure, ret=%x", curve_name,ret);
        print_BN_buf_U32(x3, pWordLen, "x3");
        print_BN_buf_U32(y3, pWordLen, "y3");
        return 1;
    }
    else
    {
        printf("\r\n %s eccp_pointMul_Shamir test ([1]P+[n]Q = P) success", curve_name);
    }

#if 0
    //[n]P+[1]Q = Q       ----error, 返回码总是逆不存在
    uint32_clear(k1, nWordLen);
    pke_set_operand_uint32_value(k2, nWordLen, 1);
    ret = eccp_pointMul_Shamir(curve, curve->eccp_n, x1, y1,  //curve->eccp_Gx, curve->eccp_Gy,//
                                     k2, curve->eccp_Gx, curve->eccp_Gy,
                                     x3, y3);

    if(PKE_SUCCESS != ret || uint32_BigNumCmp(x3, pWordLen, curve->eccp_Gx, pWordLen) || uint32_BigNumCmp(y3, pWordLen, curve->eccp_Gy, pWordLen))
    {
        printf("\r\n %s eccp_pointMul_Shamir test ([n]P+[1]Q = Q) failure, ret=%x", curve_name,ret);
        print_BN_buf_U32(x3, pWordLen, "x3");
        print_BN_buf_U32(y3, pWordLen, "y3");
        return 1;
    }
    else
    {
        printf("\r\n %s eccp_pointMul_Shamir test ([n]P+[1]Q = Q) success", curve_name);
    }
#endif

    //[n]P+[n]Q
    ret = eccp_pointMul_Shamir(curve, curve->eccp_n, x1, y1,
                                     curve->eccp_n, curve->eccp_Gx, curve->eccp_Gy,
                                     x3, y3);

    if(PKE_NO_MODINV != ret)
    {
        printf("\r\n %s eccp_pointMul_Shamir test ([n]P+[n]Q) failure, ret=%x", curve_name,ret);
        print_BN_buf_U32(x3, pWordLen, "x3");
        print_BN_buf_U32(y3, pWordLen, "y3");
        return 1;
    }
    else
    {
        printf("\r\n %s eccp_pointMul_Shamir test ([n]P+[n]Q) success(return PKE_NO_MODINV)", curve_name);
    }

#endif

    printf("\r\n");

    return 0;
}





#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 160) && defined(SUPPORT_SECP160K1))
uint32_t eccp_secp160k1_test(void)
{
    //pointMul vector 1
    uint32_t k1[]      = {0xC50193BE,0xDEE72DC2,0xBFAD855C,0xE28E9B1F,0x9BEA94C1,0x00000000};
    uint32_t k1G_x[]   = {0x6D862581,0x2162275C,0xF0AF0753,0x302EF0F7,0xCA6CEFFE};
    uint32_t k1G_y[]   = {0x04862A5A,0x832C946F,0x033D5D49,0x3ED8E7DC,0x799878CE};
    uint32_t dbl_G_x[] = {0x6533A0E3,0x227AFF94,0x54EE5FA0,0x95E965FC,0x74FC71CB};
    uint32_t dbl_G_y[] = {0xF358CDF9,0x9F0DB1DC,0xED3EB7D4,0x57B56930,0xCEC51789};
    uint32_t max_G_x[] = {0x00612088,0xBF82811E,0xA3D34931,0x5B6CC615,0x12582217};
    uint32_t max_G_y[] = {0x1F818F0B,0x686F77F1,0xD16FFC18,0x788B77B9,0x4DE16E76};

    //pointMul vector 2
    uint32_t k2[]      = {0x6E690250,0x79187AF9,0x5F17EA6A,0xAE6B3EC1,0x89CAF466,0x00000000};
    uint32_t P_x[]     = {0x3DAE0D5D,0xE49D9AEA,0x05C57012,0x70AF0E33,0x76BFB346};
    uint32_t P_y[]     = {0x1F6BC52F,0x1849DB71,0xD14C6D9A,0x68DCB611,0xF1536070};
    uint32_t k2P_x[]   = {0xCB996126,0x89250058,0xD719819D,0xA1700FAC,0xD7498D2D};
    uint32_t k2P_y[]   = {0xA113CB14,0x2E184EAD,0x94A2BC75,0xF57DBE10,0x4B10EFDA};
    uint32_t dbl_P_x[] = {0x38F679BC,0x2492FFB8,0x7B4D697B,0xC63D0654,0x438479EB};
    uint32_t dbl_P_y[] = {0x483380B2,0xE6D5762A,0xE7F80A40,0x42BC75F7,0xA6014785};
    uint32_t max_P_x[] = {0x221B9E12,0x6845930F,0x31D82AAD,0x7C293EB4,0xE9A25575};
    uint32_t max_P_y[] = {0x09EFAF9D,0x656A33BE,0xBF36D5B4,0x02DDFFF6,0xC764B090};

    //pointAdd vector 1
    uint32_t k1G_k2P_x[]   = {0xE1740CD7,0x9FD8A62E,0x2B5FEB43,0xFF24DB3D,0xA39686E8};
    uint32_t k1G_k2P_y[]   = {0x2284BEB0,0xE5EA241B,0x82626B93,0x3447A2B9,0x61874C1F};

    //pointAdd vector 2
    uint32_t maxG_maxP_x[]   = {0x5F9C4BB4,0xE855D30F,0xE15C948F,0x744E98AB,0x91A927BD};
    uint32_t maxG_maxP_y[]   = {0x3BCEA713,0x7792C47D,0xCE2E0ECF,0x7EF3699F,0xF93C3192};

    eccp_curve_t *curve = (eccp_curve_t *)secp160k1;
    char *curve_name = "secp160k1";

    pke_eccp_pointMul_vector_t pointMul_vector[2];
    pke_eccp_pointAdd_vector_t pointAdd_vector[2];

    pointMul_vector[0].k       = k1;
    pointMul_vector[0].P_x     = curve->eccp_Gx;
    pointMul_vector[0].P_y     = curve->eccp_Gy;
    pointMul_vector[0].kP_x    = k1G_x;
    pointMul_vector[0].kP_y    = k1G_y;
    pointMul_vector[0].dbl_P_x = dbl_G_x;
    pointMul_vector[0].dbl_P_y = dbl_G_y;
    pointMul_vector[0].max_P_x = max_G_x;
    pointMul_vector[0].max_P_y = max_G_y;

    pointMul_vector[1].k       = k2;
    pointMul_vector[1].P_x     = P_x;
    pointMul_vector[1].P_y     = P_y;
    pointMul_vector[1].kP_x    = k2P_x;
    pointMul_vector[1].kP_y    = k2P_y;
    pointMul_vector[1].dbl_P_x = dbl_P_x;
    pointMul_vector[1].dbl_P_y = dbl_P_y;
    pointMul_vector[1].max_P_x = max_P_x;
    pointMul_vector[1].max_P_y = max_P_y;

    if(eccp_pointMul_test(curve, curve_name, pointMul_vector, 2))
    {
        return 1;
    }

    pointAdd_vector[0].P1_x = k1G_x;
    pointAdd_vector[0].P1_y = k1G_y;
    pointAdd_vector[0].P2_x = k2P_x;
    pointAdd_vector[0].P2_y = k2P_y;
    pointAdd_vector[0].Q_x  = k1G_k2P_x;
    pointAdd_vector[0].Q_y  = k1G_k2P_y;

    pointAdd_vector[1].P1_x = max_G_x;
    pointAdd_vector[1].P1_y = max_G_y;
    pointAdd_vector[1].P2_x = max_P_x;
    pointAdd_vector[1].P2_y = max_P_y;
    pointAdd_vector[1].Q_x  = maxG_maxP_x;
    pointAdd_vector[1].Q_y  = maxG_maxP_y;

    if(eccp_pointAdd_test(curve, curve_name, pointAdd_vector, 2))
    {
        return 1;
    }

#ifdef ECCP_POINT_DOUBLE
    if(eccp_pointDouble_test(curve, curve_name, pointMul_vector, 2))
    {
        return 1;
    }
#endif

    return 0;
}
#endif


#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 192) && defined(SUPPORT_SECP192K1))
uint32_t eccp_secp192k1_test(void)
{
    //vector 1
    uint32_t k1[]      = {0xFC45ED85,0xF917DDC3,0x6933453B,0x1C99A254,0xCAE8A0CB,0x64076DD3};
    uint32_t k1G_x[]   = {0x7C09D3E3,0xD3A25408,0x34D85FBB,0x6EEA6725,0x23CF8BE6,0x9CF81A43};
    uint32_t k1G_y[]   = {0xE6A6C12F,0x320CB538,0xB2D9CD32,0xDD506CB6,0x4B8F2877,0xAD2A3004};
    uint32_t dbl_G_x[] = {0x94F93CB6,0xA8BED93B,0x9CD1D4B3,0x84F5D254,0x31B17476,0xF091CF63};
    uint32_t dbl_G_y[] = {0xA18F2ED5,0x03DA2D98,0x1C5E42E6,0x2E626866,0x1E7565A0,0xFD7AF42E};
    uint32_t max_G_x[] = {0xCB21D265,0xDBC2D534,0x6A7C2285,0xE25DF323,0xD479AC6E,0x06E517BB};
    uint32_t max_G_y[] = {0xD54A35FA,0x60C44C55,0x85090712,0x638A22A6,0x6DC45A97,0xD551D4EE};

    //vector 2
    uint32_t k2[]      = {0x29650EC9,0x8DCE76E8,0x833877B7,0x95831533,0x31629144,0x8DA30E67};
    uint32_t P_x[]     = {0xA67B4583,0x065487C9,0x615CA3B9,0xBF2A1BA7,0xAE2FD5E0,0x6E43B7DC};
    uint32_t P_y[]     = {0xA19A6A58,0x06A290F7,0x09E4C196,0xD5FEDC3D,0x7AE08E84,0xC48DCEA4};
    uint32_t k2P_x[]   = {0xAFD3E672,0xBEED5664,0xEF8A1FE0,0xE7276640,0x5F4CB3CA,0x4360E4ED};
    uint32_t k2P_y[]   = {0x9B0D0B50,0xBE6EF318,0xE24451DB,0x6E0C8291,0x497C4982,0xE32E3AD7};
    uint32_t dbl_P_x[] = {0xA6C44EE9,0xCDD39EC2,0x81A39A40,0x5EABC46B,0x993F384A,0xD5CFBCCA};
    uint32_t dbl_P_y[] = {0xEFD0922F,0xD42115D5,0xC7DA4467,0xAC745267,0x35309B7D,0xD2B873DA};
    uint32_t max_P_x[] = {0xD9D3999C,0x787AB8C4,0xFBF6471D,0xF9E29888,0xC532D5AB,0xE97E00AD};
    uint32_t max_P_y[] = {0x9B3FF423,0xD00A1966,0x4ECECCD7,0xCD49B67A,0x8213456F,0xEE161399};

    //pointAdd vector 1
    uint32_t k1G_k2P_x[]   = {0x139063F4,0x738E18CC,0xC5EACDC5,0x904AED93,0x7AF69982,0x665948CD};
    uint32_t k1G_k2P_y[]   = {0xDF2099CC,0x525044BD,0x9CAEE98E,0x5779D9A0,0xB69E3471,0x65F2D67F};

    //pointAdd vector 2
    uint32_t maxG_maxP_x[]   = {0x7DF681FE,0xEA8D5D8B,0x652F5CD7,0xBBA61E25,0x637B26B0,0x77C51FA3};
    uint32_t maxG_maxP_y[]   = {0xE442B0CC,0x519C4DD3,0xEEA53A9C,0x3DE593CB,0x4385AD08,0xE5F8D419};

    eccp_curve_t *curve = (eccp_curve_t *)secp192k1;
    char *curve_name = "secp192k1";

    pke_eccp_pointMul_vector_t pointMul_vector[2];
    pke_eccp_pointAdd_vector_t pointAdd_vector[2];

    pointMul_vector[0].k       = k1;
    pointMul_vector[0].P_x     = curve->eccp_Gx;
    pointMul_vector[0].P_y     = curve->eccp_Gy;
    pointMul_vector[0].kP_x    = k1G_x;
    pointMul_vector[0].kP_y    = k1G_y;
    pointMul_vector[0].dbl_P_x = dbl_G_x;
    pointMul_vector[0].dbl_P_y = dbl_G_y;
    pointMul_vector[0].max_P_x = max_G_x;
    pointMul_vector[0].max_P_y = max_G_y;

    pointMul_vector[1].k       = k2;
    pointMul_vector[1].P_x     = P_x;
    pointMul_vector[1].P_y     = P_y;
    pointMul_vector[1].kP_x    = k2P_x;
    pointMul_vector[1].kP_y    = k2P_y;
    pointMul_vector[1].dbl_P_x = dbl_P_x;
    pointMul_vector[1].dbl_P_y = dbl_P_y;
    pointMul_vector[1].max_P_x = max_P_x;
    pointMul_vector[1].max_P_y = max_P_y;

    if(eccp_pointMul_test(curve, curve_name, pointMul_vector, 2))
    {
        return 1;
    }

    pointAdd_vector[0].P1_x = k1G_x;
    pointAdd_vector[0].P1_y = k1G_y;
    pointAdd_vector[0].P2_x = k2P_x;
    pointAdd_vector[0].P2_y = k2P_y;
    pointAdd_vector[0].Q_x  = k1G_k2P_x;
    pointAdd_vector[0].Q_y  = k1G_k2P_y;

    pointAdd_vector[1].P1_x = max_G_x;
    pointAdd_vector[1].P1_y = max_G_y;
    pointAdd_vector[1].P2_x = max_P_x;
    pointAdd_vector[1].P2_y = max_P_y;
    pointAdd_vector[1].Q_x  = maxG_maxP_x;
    pointAdd_vector[1].Q_y  = maxG_maxP_y;

    if(eccp_pointAdd_test(curve, curve_name, pointAdd_vector, 2))
    {
        return 1;
    }

#ifdef ECCP_POINT_DOUBLE
    if(eccp_pointDouble_test(curve, curve_name, pointMul_vector, 2))
    {
        return 1;
    }
#endif

    return 0;
}
#endif


#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 224) && defined(SUPPORT_SECP224K1))
uint32_t eccp_secp224k1_test(void)
{
    //pointMul vector 1
    uint32_t k1[]      = {0xD9914E4A,0xF64EE0E0,0xEAB1FF0A,0xA2F0A959,0xB3EECD27,0x7FE8F0C5,0x0D04EB99,0x00000000};
    uint32_t k1G_x[]   = {0xF0F89ED5,0x3A1234D7,0x80E57E90,0xCF4BD817,0x07D6C2F9,0x6C8D5BB5,0x85D8A75A};
    uint32_t k1G_y[]   = {0xEE01B615,0x7EAA073E,0x3E89F22E,0x44DF66C6,0xD77EFA91,0x6FF6B2C3,0x887E5A5C};
    uint32_t dbl_G_x[] = {0x26749E60,0xA1CE8CF4,0x96B9639F,0x232B9BF5,0x390999A0,0x6AEB9712,0x86C0DEB5};
    uint32_t dbl_G_y[] = {0x9F06B69A,0xDBF4A9FA,0x539ED633,0x906B855C,0x5B474A79,0x4E108555,0x8F598C95};
    uint32_t max_G_x[] = {0x065F4147,0xE6B91AE8,0x324F1F3B,0xB1D5C664,0x759DC782,0xC51EDD34,0x926D8C64};
    uint32_t max_G_y[] = {0x1118A072,0x0C947B4C,0x5D6BA091,0xD27F29E3,0x73350CBC,0xABFF40E7,0xDF1411C7};

    //pointMul vector 2
    uint32_t k2[]      = {0x8B7D7EE4,0x25311734,0xBA577E84,0x2B505F46,0xF875A981,0x01A0B525,0x20C98127,0x00000000};
    uint32_t P_x[]     = {0xF7A0DD28,0xB3D03EC4,0xD9701679,0xF6F1B667,0x9FC1976A,0x68CC40AD,0xFA182DC2};
    uint32_t P_y[]     = {0xC121661F,0x3BA39DD0,0x59A80A61,0x23406389,0x3C6F383D,0xAE938E86,0xDDE2C5F5};
    uint32_t k2P_x[]   = {0xB2AC7372,0x7F3C2536,0x90C7287B,0xFB87FD3C,0xD24CB906,0x285E85F5,0xDD96AB2A};
    uint32_t k2P_y[]   = {0xC9E5B1DC,0x1702582C,0x3D336850,0x0DB67654,0xABE31D0F,0x11DB081F,0xCDD24F70};
    uint32_t dbl_P_x[] = {0x58A20029,0xAF85B731,0x0E41A811,0x03BF04D3,0x65B09D65,0xBB9C9827,0xECA9C439};
    uint32_t dbl_P_y[] = {0xDA6B374A,0x65AAA048,0xCECE7CE0,0x80D4CE28,0x0C59FDDA,0x796A4E3E,0x283F2763};
    uint32_t max_P_x[] = {0x8F893EDA,0x91B2098C,0x3ACF2954,0x245A15E8,0x8593EBB6,0x3856F0C4,0x0E80F8B4};
    uint32_t max_P_y[] = {0x9713D35B,0xED23573D,0xAE292694,0x1AD25D89,0x697A7052,0x0FF817B0,0xE67795E7};

    //pointAdd vector 1
    uint32_t k1G_k2P_x[]   = {0xA9FCACB4,0x3FF4C311,0x51903B5E,0x4DE3CEA2,0x68FE31EC,0x3346D1AC,0x765AA6DD};
    uint32_t k1G_k2P_y[]   = {0xE423FA31,0xB2EDD9F9,0x484966F2,0xB36EC758,0x00AD56EE,0x1136D86F,0x7683D4A4};

    //pointAdd vector 2
    uint32_t maxG_maxP_x[]   = {0x2C97CAD5,0x91F9967D,0x2E067B70,0xE1B4552B,0x479F5BE9,0x2E9288D2,0xE734329D};
    uint32_t maxG_maxP_y[]   = {0x01330646,0xABE43D23,0xE7F51E63,0x31CC5BA7,0x4E6449D4,0xB58D5D7F,0x8AEDDB4F};

    eccp_curve_t *curve = (eccp_curve_t *)secp224k1;
    char *curve_name = "secp224k1";

    pke_eccp_pointMul_vector_t pointMul_vector[2];
    pke_eccp_pointAdd_vector_t pointAdd_vector[2];

    pointMul_vector[0].k       = k1;
    pointMul_vector[0].P_x     = curve->eccp_Gx;
    pointMul_vector[0].P_y     = curve->eccp_Gy;
    pointMul_vector[0].kP_x    = k1G_x;
    pointMul_vector[0].kP_y    = k1G_y;
    pointMul_vector[0].dbl_P_x = dbl_G_x;
    pointMul_vector[0].dbl_P_y = dbl_G_y;
    pointMul_vector[0].max_P_x = max_G_x;
    pointMul_vector[0].max_P_y = max_G_y;

    pointMul_vector[1].k       = k2;
    pointMul_vector[1].P_x     = P_x;
    pointMul_vector[1].P_y     = P_y;
    pointMul_vector[1].kP_x    = k2P_x;
    pointMul_vector[1].kP_y    = k2P_y;
    pointMul_vector[1].dbl_P_x = dbl_P_x;
    pointMul_vector[1].dbl_P_y = dbl_P_y;
    pointMul_vector[1].max_P_x = max_P_x;
    pointMul_vector[1].max_P_y = max_P_y;

    if(eccp_pointMul_test(curve, curve_name, pointMul_vector, 2))
    {
        return 1;
    }

    pointAdd_vector[0].P1_x = k1G_x;
    pointAdd_vector[0].P1_y = k1G_y;
    pointAdd_vector[0].P2_x = k2P_x;
    pointAdd_vector[0].P2_y = k2P_y;
    pointAdd_vector[0].Q_x  = k1G_k2P_x;
    pointAdd_vector[0].Q_y  = k1G_k2P_y;

    pointAdd_vector[1].P1_x = max_G_x;
    pointAdd_vector[1].P1_y = max_G_y;
    pointAdd_vector[1].P2_x = max_P_x;
    pointAdd_vector[1].P2_y = max_P_y;
    pointAdd_vector[1].Q_x  = maxG_maxP_x;
    pointAdd_vector[1].Q_y  = maxG_maxP_y;

    if(eccp_pointAdd_test(curve, curve_name, pointAdd_vector, 2))
    {
        return 1;
    }

#ifdef ECCP_POINT_DOUBLE
    if(eccp_pointDouble_test(curve, curve_name, pointMul_vector, 2))
    {
        return 1;
    }
#endif

    return 0;
}
#endif


#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 256) && defined(SUPPORT_SECP256K1))
uint32_t eccp_secp256k1_test(void)
{
    //pointMul vector 1
    uint32_t k1[]      = {0x8555834E,0x6A14E579,0x97E636E9,0xD0C1B85F,0x078A705A,0xA63E7AA2,0x6D1DC21A,0xB2097273};
    uint32_t k1G_x[]   = {0xA0D6B307,0x1AAEAE82,0x1FA14EDD,0xFAAFA73A,0xB6261432,0x3830B70A,0x12A2810D,0xC8A5B3DD};
    uint32_t k1G_y[]   = {0x29AFE4EA,0x7A3DC418,0x6157C1F2,0x3D65A19B,0x311E7AC0,0xFD10BFFB,0xA44225A7,0xCD90137F};
    uint32_t dbl_G_x[] = {0x5C709EE5,0xABAC09B9,0x8CEF3CA7,0x5C778E4B,0x95C07CD8,0x3045406E,0x41ED7D6D,0xC6047F94};
    uint32_t dbl_G_y[] = {0x50CFE52A,0x236431A9,0x3266D0E1,0xF7F63265,0x466CEAEE,0xA3C58419,0xA63DC339,0x1AE168FE};
    uint32_t max_G_x[] = {0xB56606AF,0x15714AA4,0x5F894F47,0x6B4A2209,0xF69D7F35,0x5F9E3DF9,0xB9F905E5,0x9166C289};
    uint32_t max_G_y[] = {0x499255E7,0x1099AF98,0x6C93FD09,0x94E214F0,0x66D809BE,0xCFF9E16B,0x6BE4ACB5,0xF181EB96};

    //pointMul vector 2
    uint32_t k2[]      = {0xDCAA7E80,0x9986EE2D,0xA6AB6142,0x32D9669F,0x7935F090,0xA8F353C0,0x1353792E,0x63C6E9BD};
    uint32_t P_x[]     = {0xBCE036F9,0x8601F113,0x836F99B0,0xB531C845,0xF89D5229,0x49344F85,0x9258C310,0xF9308A01};
    uint32_t P_y[]     = {0x84B8E672,0x6CB9FD75,0x34C2231B,0x6500A999,0x2A37F356,0x0FE337E6,0x632DE814,0x388F7B0F};
    uint32_t k2P_x[]   = {0x6AE12709,0xF30DBE5C,0x86495B40,0x0F372870,0x05F51615,0x67A22A3C,0xD73195C3,0x2E95508D};
    uint32_t k2P_y[]   = {0x1CFEB3C0,0x9E97D62D,0xDE596159,0x3BD0919C,0xA25D18EB,0xD9C89974,0xFCE0EB6F,0x009900F7};
    uint32_t dbl_P_x[] = {0x60297556,0x2F057A14,0x8568A18B,0x82F6472F,0x355235D3,0x20453A14,0x755EEEA4,0xFFF97BD5};
    uint32_t dbl_P_y[] = {0xB075F297,0x3C870C36,0x518FE4A0,0xDE80F0F6,0x7F45C560,0xF3BE9601,0xACFBB620,0xAE12777A};
    uint32_t max_P_x[] = {0x2395ADD3,0x92999432,0x01D82593,0x24CBB6F9,0x44A59BF0,0x1B475F63,0x35058F34,0x771ED3A2};
    uint32_t max_P_y[] = {0x6022D93C,0xE826B199,0x59E201C3,0x4E7EE1F8,0x9B1A1540,0xDEC47D45,0x2A41FF10,0x90A157FD};

    //pointAdd vector 1
    uint32_t k1G_k2P_x[]   = {0x08DA145D,0xF7D9E9BB,0x3511D5DE,0xF92A21B5,0x21C3D352,0x8373160D,0x7A6DD6C8,0x51BBA02C};
    uint32_t k1G_k2P_y[]   = {0x7FBB0694,0x64E37E2B,0x39C41754,0x6C6A59FF,0x9AA0B595,0x98E3624A,0x64CEC824,0x660C96DD};

    //pointAdd vector 2
    uint32_t maxG_maxP_x[]   = {0xF0188757,0x20DE5838,0xE29711AA,0x65703334,0x42ECE4D1,0x9B0BE854,0x7CDAF3A6,0x4A95FB97};
    uint32_t maxG_maxP_y[]   = {0x25A605BE,0x0A83609A,0xA0D917DB,0x71443E6D,0x37FD2C21,0x673DBAC7,0xF5BBC590,0xD7A054D2};

    eccp_curve_t *curve = (eccp_curve_t *)secp256k1;
    char *curve_name = "secp256k1";

    pke_eccp_pointMul_vector_t pointMul_vector[2];
    pke_eccp_pointAdd_vector_t pointAdd_vector[2];

    pointMul_vector[0].k       = k1;
    pointMul_vector[0].P_x     = curve->eccp_Gx;
    pointMul_vector[0].P_y     = curve->eccp_Gy;
    pointMul_vector[0].kP_x    = k1G_x;
    pointMul_vector[0].kP_y    = k1G_y;
    pointMul_vector[0].dbl_P_x = dbl_G_x;
    pointMul_vector[0].dbl_P_y = dbl_G_y;
    pointMul_vector[0].max_P_x = max_G_x;
    pointMul_vector[0].max_P_y = max_G_y;

    pointMul_vector[1].k       = k2;
    pointMul_vector[1].P_x     = P_x;
    pointMul_vector[1].P_y     = P_y;
    pointMul_vector[1].kP_x    = k2P_x;
    pointMul_vector[1].kP_y    = k2P_y;
    pointMul_vector[1].dbl_P_x = dbl_P_x;
    pointMul_vector[1].dbl_P_y = dbl_P_y;
    pointMul_vector[1].max_P_x = max_P_x;
    pointMul_vector[1].max_P_y = max_P_y;

    if(eccp_pointMul_test(curve, curve_name, pointMul_vector, 2))
    {
        return 1;
    }

    pointAdd_vector[0].P1_x = k1G_x;
    pointAdd_vector[0].P1_y = k1G_y;
    pointAdd_vector[0].P2_x = k2P_x;
    pointAdd_vector[0].P2_y = k2P_y;
    pointAdd_vector[0].Q_x  = k1G_k2P_x;
    pointAdd_vector[0].Q_y  = k1G_k2P_y;

    pointAdd_vector[1].P1_x = max_G_x;
    pointAdd_vector[1].P1_y = max_G_y;
    pointAdd_vector[1].P2_x = max_P_x;
    pointAdd_vector[1].P2_y = max_P_y;
    pointAdd_vector[1].Q_x  = maxG_maxP_x;
    pointAdd_vector[1].Q_y  = maxG_maxP_y;

    if(eccp_pointAdd_test(curve, curve_name, pointAdd_vector, 2))
    {
        return 1;
    }

#ifdef ECCP_POINT_DOUBLE
    if(eccp_pointDouble_test(curve, curve_name, pointMul_vector, 2))
    {
        return 1;
    }
#endif

    return 0;
}
#endif


#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 160) && defined(SUPPORT_SECP160R1))
uint32_t eccp_secp160r1_test(void)
{
    //pointMul vector 1
    uint32_t k1[]      = {0xEB844DD4,0xFE675456,0xEE99655B,0x005A2596,0xAFE9B4BA,0x00000000};
    uint32_t k1G_x[]   = {0x58729957,0x1018C29E,0x9112F0D8,0x6A508750,0x7CE3F81F};
    uint32_t k1G_y[]   = {0x76C1CC13,0xE061A2D0,0x7B5D71F6,0xC8AFECD1,0x3F6277B2};
    uint32_t dbl_G_x[] = {0xE8F46686,0x675D3E92,0x55D3EDF8,0x3C5ED04C,0x02F997F3};
    uint32_t dbl_G_y[] = {0x7DF8797B,0x21CFB773,0x440E817E,0x482993E9,0xF083A323};
    uint32_t max_G_x[] = {0xF69CAFEF,0xF38A4A8D,0xE4058741,0xEDD6CE54,0xBEDE8D6F};
    uint32_t max_G_y[] = {0xF8E6EC5C,0xE1FBC8A8,0x2A980623,0x70B32F77,0xAADD73EE};

    //pointMul vector 2
    uint32_t k2[]      = {0xC349BE62,0xEADC86C5,0x2439273E,0xE2766214,0x3A0B14EA,0x00000000};
    uint32_t P_x[]     = {0xA958BC59,0x50BD48DA,0xDF13DE16,0x1EF363F2,0x7B76FF54};
    uint32_t P_y[]     = {0xFE9F6F5A,0x9D12854F,0xB55BE007,0x0D8C8877,0xC915CA79};
    uint32_t k2P_x[]   = {0x96BF4010,0x58D7F57C,0x911E77E0,0x3DB08968,0x93514297};
    uint32_t k2P_y[]   = {0x0408184B,0xF9E576EA,0x734A6861,0x7B76BA01,0xA897BA3F};
    uint32_t dbl_P_x[] = {0xAC3A397E,0xA006B15D,0xD524362B,0x209F5A76,0xEB0570B9};
    uint32_t dbl_P_y[] = {0x035338A6,0x2049A5FA,0x4FF1CAB1,0x83D22F11,0x136DF966};
    uint32_t max_P_x[] = {0x40F501E9,0x53F38B76,0xA607C669,0x2A3E14E3,0xCC5A8C79};
    uint32_t max_P_y[] = {0x6F25B3A3,0xD93C3922,0x659DD0B7,0xB46D8105,0x83A390D9};

    //pointAdd vector 1
    uint32_t k1G_k2P_x[]   = {0x0D9F696B,0xD0B2CA8A,0xC1604A4D,0x0633D7E3,0x7ABF6EB2};
    uint32_t k1G_k2P_y[]   = {0xC86D39D5,0xAB0C2684,0xEC0E10F5,0xBB81B131,0xFDBA0398};

    //pointAdd vector 2
    uint32_t maxG_maxP_x[]   = {0x0544AA66,0x3E73A4E4,0x41418516,0xE6E2F5C4,0x774A038A};
    uint32_t maxG_maxP_y[]   = {0xBFDA5C8D,0xB99B12CA,0xDEB89BCA,0x8F3E5708,0xBB008539};

    eccp_curve_t *curve = (eccp_curve_t *)secp160r1;
    char *curve_name = "secp160r1";

    pke_eccp_pointMul_vector_t pointMul_vector[2];
    pke_eccp_pointAdd_vector_t pointAdd_vector[2];

    pointMul_vector[0].k       = k1;
    pointMul_vector[0].P_x     = curve->eccp_Gx;
    pointMul_vector[0].P_y     = curve->eccp_Gy;
    pointMul_vector[0].kP_x    = k1G_x;
    pointMul_vector[0].kP_y    = k1G_y;
    pointMul_vector[0].dbl_P_x = dbl_G_x;
    pointMul_vector[0].dbl_P_y = dbl_G_y;
    pointMul_vector[0].max_P_x = max_G_x;
    pointMul_vector[0].max_P_y = max_G_y;

    pointMul_vector[1].k       = k2;
    pointMul_vector[1].P_x     = P_x;
    pointMul_vector[1].P_y     = P_y;
    pointMul_vector[1].kP_x    = k2P_x;
    pointMul_vector[1].kP_y    = k2P_y;
    pointMul_vector[1].dbl_P_x = dbl_P_x;
    pointMul_vector[1].dbl_P_y = dbl_P_y;
    pointMul_vector[1].max_P_x = max_P_x;
    pointMul_vector[1].max_P_y = max_P_y;

    if(eccp_pointMul_test(curve, curve_name, pointMul_vector, 2))
    {
        return 1;
    }

    pointAdd_vector[0].P1_x = k1G_x;
    pointAdd_vector[0].P1_y = k1G_y;
    pointAdd_vector[0].P2_x = k2P_x;
    pointAdd_vector[0].P2_y = k2P_y;
    pointAdd_vector[0].Q_x  = k1G_k2P_x;
    pointAdd_vector[0].Q_y  = k1G_k2P_y;

    pointAdd_vector[1].P1_x = max_G_x;
    pointAdd_vector[1].P1_y = max_G_y;
    pointAdd_vector[1].P2_x = max_P_x;
    pointAdd_vector[1].P2_y = max_P_y;
    pointAdd_vector[1].Q_x  = maxG_maxP_x;
    pointAdd_vector[1].Q_y  = maxG_maxP_y;

    if(eccp_pointAdd_test(curve, curve_name, pointAdd_vector, 2))
    {
        return 1;
    }

#ifdef ECCP_POINT_DOUBLE
    if(eccp_pointDouble_test(curve, curve_name, pointMul_vector, 2))
    {
        return 1;
    }
#endif

    return 0;
}
#endif


#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 160) && defined(SUPPORT_SECP160R2))
uint32_t eccp_secp160r2_test(void)
{
    //pointMul vector 1
    uint32_t k1[]      = {0xF13CB799,0xAA79E927,0x4F68E678,0x29BDE98E,0x3E9E8AF6,0x00000000};
    uint32_t k1G_x[]   = {0x5EB9B8F2,0xC8C8AEA9,0xFBA1118D,0x83341488,0x51F539A7};
    uint32_t k1G_y[]   = {0x6441301A,0x031210CD,0x58DEDD40,0x7B7F4FF1,0x49EB191B};
    uint32_t dbl_G_x[] = {0xB900C8CD,0x883E136A,0x54DD638D,0xA41A0359,0x26788D2B};
    uint32_t dbl_G_y[] = {0x4B5FFB4A,0xD8DAFACE,0x8EB33CA3,0xD1774497,0xFCE35CEB};
    uint32_t max_G_x[] = {0x6D5A376E,0xE0F3C03A,0x4478E475,0x56960EF5,0x977453FB};
    uint32_t max_G_y[] = {0x68915B13,0xE13F10D6,0x407E38F2,0x8539D559,0x1EB13CE8};

    //pointMul vector 2
    uint32_t k2[]      = {0x36DB9007,0x8A92653C,0x6BCFCB0E,0xF2837D1C,0xCEAFF68E,0x00000000};
    uint32_t P_x[]     = {0x2C5888A5,0x75FC3752,0x50FD9BF3,0x605DABE3,0xE84FF970};
    uint32_t P_y[]     = {0x894EC182,0x29982ADC,0x3EF4BC3A,0xC4A72CEA,0x78DE7599};
    uint32_t k2P_x[]   = {0x526300DA,0x1F537EA4,0xC1608904,0xA8D3AC75,0x92F35A4D};
    uint32_t k2P_y[]   = {0x4DF301B1,0x5A115AE2,0x4408038D,0x8267DDDD,0xE6EF2379};
    uint32_t dbl_P_x[] = {0xA0475C17,0x8B173AEE,0x80A8DB8C,0x75CDA7B5,0x34EF22E9};
    uint32_t dbl_P_y[] = {0xC4372247,0x30C16D33,0xE6CFF5C8,0x1C656C5E,0x79FB05E7};
    uint32_t max_P_x[] = {0x7CF037F1,0xB3323812,0xC92CC5B2,0xF115D450,0x2F4C0F41};
    uint32_t max_P_y[] = {0xA689CAFD,0x385DFF2E,0x22DA093E,0xE994E014,0xD75A22E7};

    //pointAdd vector 1
    uint32_t k1G_k2P_x[]   = {0xC9460B2A,0xF31AEBAC,0x53293BE5,0x69E5BF5B,0xA74F6402};
    uint32_t k1G_k2P_y[]   = {0xF9510F12,0x233F190D,0x491D78FA,0x924C288B,0x53C72054};

    //pointAdd vector 2
    uint32_t maxG_maxP_x[]   = {0x46ED9E70,0x76ADA86F,0xB26143AA,0x43154818,0x23CDAACC};
    uint32_t maxG_maxP_y[]   = {0x20B6B436,0x9B131162,0xCD63209C,0x5E3DBDC9,0x919AD094};

    eccp_curve_t *curve = (eccp_curve_t *)secp160r2;
    char *curve_name = "secp160r2";

    pke_eccp_pointMul_vector_t pointMul_vector[2];
    pke_eccp_pointAdd_vector_t pointAdd_vector[2];

    pointMul_vector[0].k       = k1;
    pointMul_vector[0].P_x     = curve->eccp_Gx;
    pointMul_vector[0].P_y     = curve->eccp_Gy;
    pointMul_vector[0].kP_x    = k1G_x;
    pointMul_vector[0].kP_y    = k1G_y;
    pointMul_vector[0].dbl_P_x = dbl_G_x;
    pointMul_vector[0].dbl_P_y = dbl_G_y;
    pointMul_vector[0].max_P_x = max_G_x;
    pointMul_vector[0].max_P_y = max_G_y;

    pointMul_vector[1].k       = k2;
    pointMul_vector[1].P_x     = P_x;
    pointMul_vector[1].P_y     = P_y;
    pointMul_vector[1].kP_x    = k2P_x;
    pointMul_vector[1].kP_y    = k2P_y;
    pointMul_vector[1].dbl_P_x = dbl_P_x;
    pointMul_vector[1].dbl_P_y = dbl_P_y;
    pointMul_vector[1].max_P_x = max_P_x;
    pointMul_vector[1].max_P_y = max_P_y;

    if(eccp_pointMul_test(curve, curve_name, pointMul_vector, 2))
    {
        return 1;
    }

    pointAdd_vector[0].P1_x = k1G_x;
    pointAdd_vector[0].P1_y = k1G_y;
    pointAdd_vector[0].P2_x = k2P_x;
    pointAdd_vector[0].P2_y = k2P_y;
    pointAdd_vector[0].Q_x  = k1G_k2P_x;
    pointAdd_vector[0].Q_y  = k1G_k2P_y;

    pointAdd_vector[1].P1_x = max_G_x;
    pointAdd_vector[1].P1_y = max_G_y;
    pointAdd_vector[1].P2_x = max_P_x;
    pointAdd_vector[1].P2_y = max_P_y;
    pointAdd_vector[1].Q_x  = maxG_maxP_x;
    pointAdd_vector[1].Q_y  = maxG_maxP_y;

    if(eccp_pointAdd_test(curve, curve_name, pointAdd_vector, 2))
    {
        return 1;
    }

#ifdef ECCP_POINT_DOUBLE
    if(eccp_pointDouble_test(curve, curve_name, pointMul_vector, 2))
    {
        return 1;
    }
#endif

    return 0;
}
#endif


#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 192) && defined(SUPPORT_SECP192R1))
uint32_t eccp_secp192r1_test(void)
{
    //vector 1
    uint32_t k1[]      = {0x43E617F4,0xADFDAB34,0x3A094AFC,0xBD197A67,0x2F2D10FF,0x14E04195};
    uint32_t k1G_x[]   = {0x264AD6F1,0xBEE826E3,0xE1634F70,0xC5673168,0x7AFCC925,0x1554DE43};
    uint32_t k1G_y[]   = {0x769D06A8,0x4BEACA81,0x2C03ED2A,0xDBE752DE,0x9D8E4A9B,0xAA00D5E8};
    uint32_t dbl_G_x[] = {0x6982A888,0x29A70FB1,0x1588A3F6,0xD3553463,0x28783F2A,0xDAFEBF58};
    uint32_t dbl_G_y[] = {0x5C7E93AB,0x59331AFA,0x141B868F,0x46B27BBC,0x993DA0FA,0xDD6BDA0D};
    uint32_t max_G_x[] = {0x3F213ECF,0x85791135,0x1B3DC3CF,0x7284E6D4,0xE777B4A4,0xCC4AF403};
    uint32_t max_G_y[] = {0xF15D1810,0x125E875E,0xC30497A9,0x468DB706,0x64E1D23E,0x968E7021};

    //vector 2
    uint32_t k2[]      = {0xD1A3BB74,0x81FFE829,0xCA918A7F,0x1B6FE982,0x0F7DB0B3,0x2B26D917};
    uint32_t P_x[]     = {0x7F55BFAF,0x91941370,0x433E826F,0xFDC54747,0xC9912C1A,0x4F4779B5};
    uint32_t P_y[]     = {0x5C107FF0,0xDC06518A,0x3782DA08,0x6B5A1612,0xE01B980E,0xA1958378};
    uint32_t k2P_x[]   = {0x7573F426,0x0720E50A,0x8A1A1C22,0x85018E30,0xDDEFB84C,0x50FA027E};
    uint32_t k2P_y[]   = {0x25F86FA8,0xFBB1C7F8,0x626EA699,0xF18B1C75,0x0657AAA2,0xE26D042A};
    uint32_t dbl_P_x[] = {0xB5BFF013,0x5EBCE869,0xAAC1E6B7,0xBA6EF5C9,0xF656C636,0x3A674A7D};
    uint32_t dbl_P_y[] = {0xAE1CCDD4,0xDBB03947,0xCFB52071,0xCA3DC627,0x1B49D609,0x70475E20};
    uint32_t max_P_x[] = {0x604264D3,0xB8BAC16A,0xC79D524E,0xE397CBF2,0x7A0CA47B,0xE42541C0};
    uint32_t max_P_y[] = {0xE30EF958,0xE5CB8EF9,0xFCBD4448,0xCF7CCB2C,0xDB0D2AEB,0x37E94F05};

    //pointAdd vector 1
    uint32_t k1G_k2P_x[]   = {0x79D4AC94,0x2C1876FE,0x09869F5E,0x115DADBD,0x5D2717AE,0x5A80473B};
    uint32_t k1G_k2P_y[]   = {0xD3D642B1,0x6D44D11F,0xBF949732,0x271DD36B,0xB21B6F00,0x9D01BE83};

    //pointAdd vector 2
    uint32_t maxG_maxP_x[]   = {0x6A485DF3,0xACC32149,0xCC97FF17,0x21DB4A8D,0x2A637CA8,0x79606605};
    uint32_t maxG_maxP_y[]   = {0xCAFA8072,0x9B2448F1,0x7E74312A,0x90C785A0,0xAC6EBAAE,0xEFACE738};

    eccp_curve_t *curve = (eccp_curve_t *)secp192r1;
    char *curve_name = "secp192r1";

    pke_eccp_pointMul_vector_t pointMul_vector[2];
    pke_eccp_pointAdd_vector_t pointAdd_vector[2];

    pointMul_vector[0].k       = k1;
    pointMul_vector[0].P_x     = curve->eccp_Gx;
    pointMul_vector[0].P_y     = curve->eccp_Gy;
    pointMul_vector[0].kP_x    = k1G_x;
    pointMul_vector[0].kP_y    = k1G_y;
    pointMul_vector[0].dbl_P_x = dbl_G_x;
    pointMul_vector[0].dbl_P_y = dbl_G_y;
    pointMul_vector[0].max_P_x = max_G_x;
    pointMul_vector[0].max_P_y = max_G_y;

    pointMul_vector[1].k       = k2;
    pointMul_vector[1].P_x     = P_x;
    pointMul_vector[1].P_y     = P_y;
    pointMul_vector[1].kP_x    = k2P_x;
    pointMul_vector[1].kP_y    = k2P_y;
    pointMul_vector[1].dbl_P_x = dbl_P_x;
    pointMul_vector[1].dbl_P_y = dbl_P_y;
    pointMul_vector[1].max_P_x = max_P_x;
    pointMul_vector[1].max_P_y = max_P_y;

    if(eccp_pointMul_test(curve, curve_name, pointMul_vector, 2))
    {
        return 1;
    }

    pointAdd_vector[0].P1_x = k1G_x;
    pointAdd_vector[0].P1_y = k1G_y;
    pointAdd_vector[0].P2_x = k2P_x;
    pointAdd_vector[0].P2_y = k2P_y;
    pointAdd_vector[0].Q_x  = k1G_k2P_x;
    pointAdd_vector[0].Q_y  = k1G_k2P_y;

    pointAdd_vector[1].P1_x = max_G_x;
    pointAdd_vector[1].P1_y = max_G_y;
    pointAdd_vector[1].P2_x = max_P_x;
    pointAdd_vector[1].P2_y = max_P_y;
    pointAdd_vector[1].Q_x  = maxG_maxP_x;
    pointAdd_vector[1].Q_y  = maxG_maxP_y;

    if(eccp_pointAdd_test(curve, curve_name, pointAdd_vector, 2))
    {
        return 1;
    }

#ifdef ECCP_POINT_DOUBLE
    if(eccp_pointDouble_test(curve, curve_name, pointMul_vector, 2))
    {
        return 1;
    }
#endif

    return 0;
}
#endif


#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 224) && defined(SUPPORT_SECP224R1))
uint32_t eccp_secp224r1_test(void)
{
    //vector 1
    uint32_t k1[]      = {0x89F2493E,0xF910DDE2,0xE2899826,0x54BB4787,0x8DFBD993,0x59E15395,0x47E2EF7A};
    uint32_t k1G_x[]   = {0xD4C2F79D,0xDF4C8CB4,0x8B845709,0x9C127A03,0xB867A559,0x48E4B25A,0xA4B87130};
    uint32_t k1G_y[]   = {0xC5C83695,0x0426DB7A,0xE0950078,0xD6515395,0x4F0F19A4,0x0FF18D5B,0x8A51BB94};
    uint32_t dbl_G_x[] = {0x1A704FA6,0x32D268FD,0xD16DC180,0x89474788,0x98E60E6D,0x76DCB767,0x706A46DC};
    uint32_t dbl_G_y[] = {0xD2E4E8BB,0x7ACF3709,0xFCA62948,0x86892849,0x2A704FA9,0xBC25E770,0x1C2B76A7};
    uint32_t max_G_x[] = {0xC186C49F,0x9BFAD5C1,0x010E7A38,0xBD04D516,0x6501EBAA,0x65DC68E4,0x1AED85AD};
    uint32_t max_G_y[] = {0x9B3014F8,0x47900BED,0x33262BFF,0xBB59F744,0xFACBDE8F,0xD59CC791,0xD2692F36};

    //vector 2
    uint32_t k2[]      = {0xC9132F73,0x8432D98E,0x1EA7E952,0xB855C5E3,0x2A7E3265,0x39AC9EA0,0xBD06E4F9};
    uint32_t P_x[]     = {0xE7BF47FA,0x9591E077,0xAF761C13,0x4AEAFF49,0x2E979B24,0xCB17CDAF,0x3EE157A2};
    uint32_t P_y[]     = {0x6A34AB23,0x0DF69301,0x8C45B181,0xF98D310C,0xAA27A9F2,0xA74C3103,0x45217F8A};
    uint32_t k2P_x[]   = {0xE4CA46C8,0x1930E39A,0x0DA7D5D7,0x18DB2069,0x4827D478,0xFC55AA51,0xCCCA9BFE};
    uint32_t k2P_y[]   = {0xDA4FF025,0x579BF08F,0x52A9E1AF,0x42DA861E,0x2DA1F33E,0x1F206311,0x9EDDBE26};
    uint32_t dbl_P_x[] = {0xFE8A4770,0x396A4CA6,0x62A6B426,0x8B94BC2D,0xE67F96E7,0xA0967070,0x807DA889};
    uint32_t dbl_P_y[] = {0xCF73F5E0,0xAAA14A3B,0x7E8DB03F,0x5DD7DB00,0x68D16898,0x5C75A060,0xC4363EA2};
    uint32_t max_P_x[] = {0x2CFE5CFB,0x0D6673B6,0xE754E9C8,0x8DB175FE,0x04306C63,0xD47A3D9A,0x1828CEB3};
    uint32_t max_P_y[] = {0x3A6D640F,0x422DC6E4,0x186EDD4D,0xC24B6AE8,0x4C99BE14,0x504C62EF,0x9C55ADF1};

    //pointAdd vector 1
    uint32_t k1G_k2P_x[]   = {0x8D388AC3,0x6E9C861D,0x6196EF87,0x59D8702A,0xD1023888,0x8588C1B5,0xAFE3090C};
    uint32_t k1G_k2P_y[]   = {0xE48B21D3,0x218F37AC,0x6B6EA8C9,0x82166D69,0x962A9F84,0xC6C02AB2,0x3F383154};

    //pointAdd vector 2
    uint32_t maxG_maxP_x[]   = {0x8286FA09,0x2ABC4D3F,0x643FCFE6,0x6119E265,0x684A8F0E,0x331519CA,0xA88AE267};
    uint32_t maxG_maxP_y[]   = {0x892A0D21,0x66B8939D,0x603DB8C2,0xAF3B8E81,0xB71DD097,0x9A429954,0x94A1EFE2};

    eccp_curve_t *curve = (eccp_curve_t *)secp224r1;
    char *curve_name = "secp224r1";

    pke_eccp_pointMul_vector_t pointMul_vector[2];
    pke_eccp_pointAdd_vector_t pointAdd_vector[2];

    pointMul_vector[0].k       = k1;
    pointMul_vector[0].P_x     = curve->eccp_Gx;
    pointMul_vector[0].P_y     = curve->eccp_Gy;
    pointMul_vector[0].kP_x    = k1G_x;
    pointMul_vector[0].kP_y    = k1G_y;
    pointMul_vector[0].dbl_P_x = dbl_G_x;
    pointMul_vector[0].dbl_P_y = dbl_G_y;
    pointMul_vector[0].max_P_x = max_G_x;
    pointMul_vector[0].max_P_y = max_G_y;

    pointMul_vector[1].k       = k2;
    pointMul_vector[1].P_x     = P_x;
    pointMul_vector[1].P_y     = P_y;
    pointMul_vector[1].kP_x    = k2P_x;
    pointMul_vector[1].kP_y    = k2P_y;
    pointMul_vector[1].dbl_P_x = dbl_P_x;
    pointMul_vector[1].dbl_P_y = dbl_P_y;
    pointMul_vector[1].max_P_x = max_P_x;
    pointMul_vector[1].max_P_y = max_P_y;

    if(eccp_pointMul_test(curve, curve_name, pointMul_vector, 2))
    {
        return 1;
    }

    pointAdd_vector[0].P1_x = k1G_x;
    pointAdd_vector[0].P1_y = k1G_y;
    pointAdd_vector[0].P2_x = k2P_x;
    pointAdd_vector[0].P2_y = k2P_y;
    pointAdd_vector[0].Q_x  = k1G_k2P_x;
    pointAdd_vector[0].Q_y  = k1G_k2P_y;

    pointAdd_vector[1].P1_x = max_G_x;
    pointAdd_vector[1].P1_y = max_G_y;
    pointAdd_vector[1].P2_x = max_P_x;
    pointAdd_vector[1].P2_y = max_P_y;
    pointAdd_vector[1].Q_x  = maxG_maxP_x;
    pointAdd_vector[1].Q_y  = maxG_maxP_y;

    if(eccp_pointAdd_test(curve, curve_name, pointAdd_vector, 2))
    {
        return 1;
    }

#ifdef ECCP_POINT_DOUBLE
    if(eccp_pointDouble_test(curve, curve_name, pointMul_vector, 2))
    {
        return 1;
    }
#endif

    return 0;
}
#endif


#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 256) && defined(SUPPORT_SECP256R1))
uint32_t eccp_secp256r1_test(void)
{
    //vector 1
    uint32_t k1[]      = {0x9FFF5AA8,0x25DE64CB,0x1A69C999,0x9C8BB3D0,0x78E52060,0x8E4C76C4,0x68C7FDB0,0xF9BFE893};
    uint32_t k1G_x[]   = {0xEDC04207,0xD3C1B21E,0x965E431D,0x34B0FA7E,0x699ABF07,0x0B4BEEF7,0xE7376B8D,0xA20FBCED};
    uint32_t k1G_y[]   = {0xC25E2AB2,0x8A478DEF,0x7315C253,0x1DFB7060,0x860C3CC0,0x2B01AFAD,0x52431001,0xFB1BEC0F};
    uint32_t dbl_G_x[] = {0x47669978,0xA60B48FC,0x77F21B35,0xC08969E2,0x04B51AC3,0x8A523803,0x8D034F7E,0x7CF27B18};
    uint32_t dbl_G_y[] = {0x227873D1,0x9E04B79D,0x3CE98229,0xBA7DADE6,0x9F7430DB,0x293D9AC6,0xDB8ED040,0x07775510};
    uint32_t max_G_x[] = {0x9DB9D31A,0x1A3D132B,0x9C3677CC,0x2C6102C4,0x9586EB53,0x1B102317,0x0E26C0D2,0xF72CBD24};
    uint32_t max_G_y[] = {0xA83408A7,0xE453D93F,0xCDCA831E,0x23250EF0,0xBFE7A5D2,0xDC0DBD91,0xE2A36621,0x43E4CA77};

    //vector 2
    uint32_t k2[]      = {0xAFC72DF4,0x051C7DEB,0xBF32A1B6,0x159BE441,0x819B8AF8,0x07EAC2C3,0x227C4630,0xB815CCEF};
    uint32_t P_x[]     = {0xF8D95287,0x692BC2ED,0xC2033181,0x997757C8,0xE77A79DC,0x06FEE0AF,0x1776F54A,0xACE2167D};
    uint32_t P_y[]     = {0x8E18BB8A,0xA84F31CD,0xCC014B2B,0x876430D5,0xD2A4781F,0x3A166074,0x66FAE4C3,0x837FBAC1};
    uint32_t k2P_x[]   = {0x6A9F0780,0xFD670A24,0xC1E14A9F,0x94CFD02A,0x34C9BBDF,0x31EF379C,0x7EA4C462,0x97517994};
    uint32_t k2P_y[]   = {0x5CB77586,0x919FB323,0xC959DD4F,0xA7C41942,0x34720941,0xCA15D113,0x3640E761,0x1AF4C78A};
    uint32_t dbl_P_x[] = {0xA6DFBC0E,0xC2158EEB,0x501E8596,0x5117F5EF,0xFE2EC6EF,0xB110D8DA,0x07125488,0xDA7E5B6A};
    uint32_t dbl_P_y[] = {0x72CFA30F,0x986C1983,0x0CEB689D,0x76FC2B44,0xFC2D57B5,0xE8BF172C,0xC9830A99,0xD9BBEA04};
    uint32_t max_P_x[] = {0xCE263F84,0xCE7C2DB2,0x74AEE88E,0xAAF82009,0x57A342AC,0xB4217DB0,0x19948F99,0x8FBF81F6};
    uint32_t max_P_y[] = {0xE73D101D,0x800D5F34,0xC3041803,0x492EB7E9,0xE330319B,0x4CEFFCEF,0x7C92D803,0xF71790AD};

    //pointAdd vector 1
    uint32_t k1G_k2P_x[]   = {0x76412DC4,0xBC24F7F9,0x7D05214B,0x14B18998,0xD58E31B2,0x887B7667,0xBF424270,0xBAFA8959};
    uint32_t k1G_k2P_y[]   = {0xB0132C17,0xEAA8AEA4,0xA9DE2F6C,0x363DCC18,0xA7FE3EC8,0x3215CCB2,0x0BCE668A,0xF9B056B7};

    //pointAdd vector 2
    uint32_t maxG_maxP_x[]   = {0xD3651552,0xECFA9321,0xBC8ECF84,0x13C5E908,0x4995FE91,0xF35A03D9,0x050D221F,0x31885880};
    uint32_t maxG_maxP_y[]   = {0x0221E069,0xAF50AC48,0x19FCDE43,0x170354C3,0x40B86161,0x090DF98E,0x745807A1,0x0F42BEAB};

    eccp_curve_t *curve = (eccp_curve_t *)secp256r1;
    char *curve_name = "secp256r1";

    pke_eccp_pointMul_vector_t pointMul_vector[2];
    pke_eccp_pointAdd_vector_t pointAdd_vector[2];

    pointMul_vector[0].k       = k1;
    pointMul_vector[0].P_x     = curve->eccp_Gx;
    pointMul_vector[0].P_y     = curve->eccp_Gy;
    pointMul_vector[0].kP_x    = k1G_x;
    pointMul_vector[0].kP_y    = k1G_y;
    pointMul_vector[0].dbl_P_x = dbl_G_x;
    pointMul_vector[0].dbl_P_y = dbl_G_y;
    pointMul_vector[0].max_P_x = max_G_x;
    pointMul_vector[0].max_P_y = max_G_y;

    pointMul_vector[1].k       = k2;
    pointMul_vector[1].P_x     = P_x;
    pointMul_vector[1].P_y     = P_y;
    pointMul_vector[1].kP_x    = k2P_x;
    pointMul_vector[1].kP_y    = k2P_y;
    pointMul_vector[1].dbl_P_x = dbl_P_x;
    pointMul_vector[1].dbl_P_y = dbl_P_y;
    pointMul_vector[1].max_P_x = max_P_x;
    pointMul_vector[1].max_P_y = max_P_y;

    if(eccp_pointMul_test(curve, curve_name, pointMul_vector, 2))
    {
        return 1;
    }

    pointAdd_vector[0].P1_x = k1G_x;
    pointAdd_vector[0].P1_y = k1G_y;
    pointAdd_vector[0].P2_x = k2P_x;
    pointAdd_vector[0].P2_y = k2P_y;
    pointAdd_vector[0].Q_x  = k1G_k2P_x;
    pointAdd_vector[0].Q_y  = k1G_k2P_y;

    pointAdd_vector[1].P1_x = max_G_x;
    pointAdd_vector[1].P1_y = max_G_y;
    pointAdd_vector[1].P2_x = max_P_x;
    pointAdd_vector[1].P2_y = max_P_y;
    pointAdd_vector[1].Q_x  = maxG_maxP_x;
    pointAdd_vector[1].Q_y  = maxG_maxP_y;

    if(eccp_pointAdd_test(curve, curve_name, pointAdd_vector, 2))
    {
        return 1;
    }

#ifdef ECCP_POINT_DOUBLE
    if(eccp_pointDouble_test(curve, curve_name, pointMul_vector, 2))
    {
        return 1;
    }
#endif

    return 0;
}
#endif


#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 384) && defined(SUPPORT_SECP384R1))
uint32_t eccp_secp384r1_test(void)
{
    //vector 1
    uint32_t k1[]      = {0x06CCE671,0xE6D3E47E,0xBD293319,0x19351950,0x100C5672,0xD8F005FE,0xB44DE59B,0x36E3C04A,
                          0xAD74F660,0xAA3B8E25,0xA027FF82,0xBF4A12EA};
    uint32_t k1G_x[]   = {0x8E65A6C9,0x75091588,0x0D6D4B94,0x5163EFE6,0xE8514CEB,0x05210E5F,0x5CFD60C0,0x608C7D4A,
                          0x00A2B307,0x4C8B5A4F,0xC9CDCC63,0xBBCDE926};
    uint32_t k1G_y[]   = {0x929251B7,0xD1F2C2FF,0xBD554015,0xF66A93D5,0xE6CA536B,0x2FCDCE25,0xE9806810,0x71E8C0F8,
                          0x5B88689A,0x3E989BA4,0x8FD722DA,0x5B2AF8AA};
    uint32_t dbl_G_x[] = {0x5295DF61,0x5B96A9C7,0xBE0E64F8,0x4FE0E86E,0x9FB96E9E,0x51D207D1,0xA6F434D6,0x89025959,
                          0xC55B97F0,0x69260045,0x7BA3D2D9,0x08D99905};
    uint32_t dbl_G_y[] = {0x0A940E80,0x61501E70,0x4D39E22D,0x5FFD43E9,0x256AB425,0x904E505F,0xBC6CC43E,0xB275D875,
                          0xFD6DBA74,0xB7BFE8DF,0x5B1B3CED,0x8E80F1FA};
    uint32_t max_G_x[] = {0x65B6BF3B,0x8FCE11B1,0x938F2250,0x4A3AC516,0xD6A8AE36,0x4ED61004,0xC2D8F841,0x2CF810C0,
                          0x71E833A2,0x3D52786A,0x50A570E5,0x8E323818};
    uint32_t max_G_y[] = {0x63971EFD,0xD58B6A6D,0xBE40E2FB,0xEB832BD2,0x0D30D55B,0xB5AA8BEB,0xE3A5FA88,0x92DBE041,
                          0xF232D7EE,0xBC850E06,0x4C3CF596,0xD262873A};

    //vector 2
    uint32_t k2[]      = {0xE0C2C229,0xFAD13B24,0x1DF79774,0xD8968F12,0xB4BE90C0,0xEF477E8D,0x67471249,0xCE2759BE,
                          0x379E935B,0xFE3E2958,0x13D47B6B,0x7BEC9377};
    uint32_t P_x[]     = {0x8F2D8207,0xC1F09AFD,0xE7937F67,0x25D239F1,0xD1A1FA09,0x0BE377C3,0x63B13D45,0xBA805AD4,
                          0xFC3D0371,0x2260E241,0x4CACEE8C,0xF0E7A320};
    uint32_t P_y[]     = {0x41F19741,0x743C9AFA,0xA7FA8A85,0xFCC9B11D,0xAA6970E0,0xF67C63A1,0x1A5D50FF,0xC6701701,
                          0xF57EA46C,0x42243DBA,0x7174F420,0xFC6505E8};
    uint32_t k2P_x[]   = {0x78B7BA9A,0xBC4FADDB,0x18A9EB5B,0xCCCDBFAA,0x61A82EE7,0x83E51CF3,0x880DBC8C,0xE3ECC368,
                          0xEE9E03EF,0x855F7DAD,0x0ECD9045,0xB29BA31D};
    uint32_t k2P_y[]   = {0x3E720976,0x037CB7AC,0x3AF59573,0x01D13DA5,0xFB9DBBC1,0x30D0157D,0x51D68F1D,0x53C12590,
                          0x80BE360E,0x91C4E010,0x58F7D639,0x219E951A};
    uint32_t dbl_P_x[] = {0x5102FB06,0xA8F44713,0x57202E17,0xE035A06C,0x369DD87B,0x551560D4,0x1C4F2757,0xEDFD4975,
                          0x1BB3E5D3,0xC00BD77A,0x90D518E3,0x160116B2};
    uint32_t dbl_P_y[] = {0xCD5E4864,0x80E044F6,0x5561BB23,0x48F309B0,0xAB150AE8,0xDE3A0E86,0x2977EB08,0x3C06215E,
                          0x4BE145FF,0xF6DBABC5,0x1D6264A3,0x8C9F5E12};
    uint32_t max_P_x[] = {0x62B81202,0x6405361F,0x6C2C2E13,0x3AEEB6BB,0xAD42AFFD,0x0493C3CA,0x23EBE909,0x03076969,
                          0xE5BBEF07,0x79A82468,0x5FA7FF0C,0xE5E73227};
    uint32_t max_P_y[] = {0x302E7EA2,0xB93B6A41,0xF3BEFA4C,0x68DCDF9A,0x5046F05C,0x846C85EE,0x394952B0,0xE9D2786A,
                          0xD58E6397,0xD239DDBB,0x10E81842,0x9126BCA2};

    //pointAdd vector 1
    uint32_t k1G_k2P_x[]   = {0xD9441C31,0xF59D21E6,0x880A1395,0xF5988E99,0x39DE784A,0xE2A9CE7C,0x9136682D,0xB16484C7,
                              0xB6CE1666,0x3EB33E38,0x000A10EC,0x00D5BA90};
    uint32_t k1G_k2P_y[]   = {0xBBAA0222,0xA189D9D1,0x7A8AF3BC,0x527B6A01,0xEDBE399F,0x720BB126,0x2E8C1829,0x59B1A6C8,
                              0xBEBC500A,0x07F0AD42,0xA27E43B7,0x82BC33A5};

    //pointAdd vector 2
    uint32_t maxG_maxP_x[]   = {0x83F3A99F,0xA9D5CF90,0x531E240F,0xB93C87B5,0xCF21EA07,0x6AFB9A4C,0x8C00B240,0xD63DCEA5,
                                0xC95B71F4,0x7BF4A398,0x4B686E6A,0x73D729B5};
    uint32_t maxG_maxP_y[]   = {0xEDC2D703,0x9014F5A5,0x20A1A368,0x961FA9A9,0x7BF7732F,0xEF906D8E,0xC6939206,0xFF50E7B4,
                                0xD2FCDC9E,0x4C4DBD93,0x2E6081F2,0x59634E54};

    eccp_curve_t *curve = (eccp_curve_t *)secp384r1;
    char *curve_name = "secp384r1";

    pke_eccp_pointMul_vector_t pointMul_vector[2];
    pke_eccp_pointAdd_vector_t pointAdd_vector[2];

    pointMul_vector[0].k       = k1;
    pointMul_vector[0].P_x     = curve->eccp_Gx;
    pointMul_vector[0].P_y     = curve->eccp_Gy;
    pointMul_vector[0].kP_x    = k1G_x;
    pointMul_vector[0].kP_y    = k1G_y;
    pointMul_vector[0].dbl_P_x = dbl_G_x;
    pointMul_vector[0].dbl_P_y = dbl_G_y;
    pointMul_vector[0].max_P_x = max_G_x;
    pointMul_vector[0].max_P_y = max_G_y;

    pointMul_vector[1].k       = k2;
    pointMul_vector[1].P_x     = P_x;
    pointMul_vector[1].P_y     = P_y;
    pointMul_vector[1].kP_x    = k2P_x;
    pointMul_vector[1].kP_y    = k2P_y;
    pointMul_vector[1].dbl_P_x = dbl_P_x;
    pointMul_vector[1].dbl_P_y = dbl_P_y;
    pointMul_vector[1].max_P_x = max_P_x;
    pointMul_vector[1].max_P_y = max_P_y;

    if(eccp_pointMul_test(curve, curve_name, pointMul_vector, 2))
    {
        return 1;
    }

    pointAdd_vector[0].P1_x = k1G_x;
    pointAdd_vector[0].P1_y = k1G_y;
    pointAdd_vector[0].P2_x = k2P_x;
    pointAdd_vector[0].P2_y = k2P_y;
    pointAdd_vector[0].Q_x  = k1G_k2P_x;
    pointAdd_vector[0].Q_y  = k1G_k2P_y;

    pointAdd_vector[1].P1_x = max_G_x;
    pointAdd_vector[1].P1_y = max_G_y;
    pointAdd_vector[1].P2_x = max_P_x;
    pointAdd_vector[1].P2_y = max_P_y;
    pointAdd_vector[1].Q_x  = maxG_maxP_x;
    pointAdd_vector[1].Q_y  = maxG_maxP_y;

    if(eccp_pointAdd_test(curve, curve_name, pointAdd_vector, 2))
    {
        return 1;
    }

#ifdef ECCP_POINT_DOUBLE
    if(eccp_pointDouble_test(curve, curve_name, pointMul_vector, 2))
    {
        return 1;
    }
#endif

    return 0;
}
#endif


#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 521) && defined(SUPPORT_SECP521R1))
uint32_t eccp_secp521r1_test(void)
{
    //vector 1
    uint32_t k1[]      = {0xF785F012,0x0608CA87,0xD7DC3892,0x7C3C81A0,0x5D09672D,0xE8198E55,0x3D1BD560,0x38EECEE3,
                          0xBC18A718,0x7D2C3643,0xC4AB3CC9,0x99EDDC70,0xFF3BBE84,0x82F15CF4,0x195115F6,0xF538F500,
                          0x000001EA};
    uint32_t k1G_x[]   = {0x5056A9DC,0xB2773BD7,0x3FE98D88,0x7CC0E9E3,0x16FB2AF2,0x3926C515,0xFBA2EA57,0xB2C1B545,
                          0x7EAA6F78,0x6DABD847,0x8A09C74F,0x7DFE8AB5,0x4E469158,0xFF0AA192,0x55220293,0x2684919C,
                          0x000001ED};
    uint32_t k1G_y[]   = {0xBBC97309,0x47D1605F,0x8CEA0A1B,0x69C27176,0x455A6DBD,0x3080056E,0x0186FFEA,0x6C1239E6,
                          0x38CD6BD6,0x4C4DA360,0x0DD55FA7,0x0918B14F,0x76F293E1,0x11C7BA44,0xC474E9D6,0x694025AD,
                          0x000001C0};
    uint32_t dbl_G_x[] = {0xBA6D783D,0xF43E3933,0xD60FD967,0xCF2FA364,0x35C5AF41,0xAA104A3A,0x6EF55507,0xB3B204DA,
                          0xD769BE97,0x2C6E5505,0x1CCC0635,0x7403279B,0x48C28274,0x2FCB2881,0x277E7E68,0x3C219024,
                          0x00000043};
    uint32_t dbl_G_y[] = {0x61F41B02,0x1BE356D6,0xEDC0F4F7,0xEAFCBE95,0x9A3248F4,0x93937FA9,0x9F251F6B,0xB3E377DE,
                          0x06C42DBB,0xAB21A299,0x4DA97740,0xC6B5107C,0xEED3F0B5,0xA7F3ECEE,0x6DB26700,0xBB8CC7F8,
                          0x000000F4};
    uint32_t max_G_x[] = {0xCE867566,0x3BC84EF2,0x0BD72E2D,0xCC2B1088,0x4E9E8922,0x4F7ADC49,0x8CF04124,0xB0FD2803,
                          0x35D21217,0xF2C2CF9B,0x2B2B3779,0xE2092DC4,0xBEC1A742,0xA0E48C53,0x3FAE5E1D,0x1435E6CF,
                          0x000001DF};
    uint32_t max_G_y[] = {0xB59DE451,0x97BC49F5,0xFC91D152,0x2EA1C185,0xE14FC726,0xBD352337,0x909F8C7F,0xA1A8FE74,
                          0x00A47A56,0xB283FF37,0x0C3F018F,0x925BAE6D,0x436D2931,0xDC285EF2,0x03A5E4A6,0x5B6415E3,
                          0x00000173};

    //vector 2
    uint32_t k2[]      = {0x0E1BE9CA,0x6B1705E3,0xD48ED7AB,0x0EFD1945,0x124DC227,0x04F61A68,0x17956844,0xDCB65E71,
                          0x81B54FD3,0x63E40908,0xE374412B,0xB0CF3228,0x0568030B,0x1DBE7838,0x9D3A9CE9,0xF1991236,
                          0x0000014E};
    uint32_t P_x[]     = {0xF556A1BF,0xD059DF7A,0x2CFFAE59,0x371D6BDD,0x7E49619F,0x005B9DEC,0x10626E83,0xD47589DA,
                          0x3373A691,0x5A2CC337,0x106ECA6B,0x23512404,0xD4B7C1B4,0x6DEB3CE2,0x41538E85,0x3D4AE25C,
                          0x00000011};
    uint32_t P_y[]     = {0x175B7B4D,0x08225AE9,0x002E59D5,0x89B4355A,0x7F9FC7F7,0x85D8ED2D,0x5D4728D0,0xC3FEE2B7,
                          0x99AA6C62,0x022275E4,0x2E21659F,0xB17AAEE2,0xD2AE229F,0x7C677308,0x9CF66636,0x8B5B95AF,
                          0x0000012D};
    uint32_t k2P_x[]   = {0x48D0CE9D,0xF766D4E1,0x8621E89C,0x0D7ECAD2,0x2722015A,0x19DD9118,0xE8AAF3B0,0xDF11B732,
                          0x8A84AB55,0xDCA985B9,0x511ACDE6,0x9459DB32,0x66D3C2F5,0x5834A3B0,0x435032B2,0x692E5CA8,
                          0x0000016E};
    uint32_t k2P_y[]   = {0xBB23B2A8,0xA586BA43,0xE5DC3497,0x7293EC07,0x2D46CD60,0x830E06B5,0xB9CA6789,0x2181381C,
                          0x317A52FA,0x350BB2FC,0xDA6C6506,0x13B6E3B7,0xBF451B34,0xF384E5BC,0xCD460C53,0x35AE1556,
                          0x000001B0};
    uint32_t dbl_P_x[] = {0x21ABB92A,0x82C5F1B2,0xA8DA7891,0x5F1C779D,0x37A11B4B,0xDC99C5E9,0xA42997A5,0x9FCF9083,
                          0xB9DA83FA,0xD8CFCE76,0xF261729F,0x17143178,0x71B74E34,0x4E38231F,0xF5C92522,0x33E931BF,
                          0x000000A0};
    uint32_t dbl_P_y[] = {0xB022272D,0x2E2ED8AD,0xAE672E92,0xC0376F1C,0xE46D22DA,0x9B907758,0x6051D0D2,0xC0EC0DA8,
                          0xBAE2BB66,0x2159A94E,0x8C262D3E,0x555696E1,0x5C0235ED,0xBA7EC1E5,0xC3CD868E,0xED0690BA,
                          0x00000128};
    uint32_t max_P_x[] = {0xB49D70C1,0x512D1068,0xA8F41D46,0xADCED529,0xBFCF9BAA,0x38B74697,0xE7FD83DA,0x6E91E206,
                          0x6368BBBE,0x13629D91,0x3B2BF5FA,0xE99AA3C5,0xD45B0E2D,0x4E79DFFA,0x8D9A7092,0xD9679139,
                          0x000001FD};
    uint32_t max_P_y[] = {0x705ED117,0x720EE710,0xC5CD5D1B,0x76E7B132,0x45EEBADF,0x6ABB36E2,0x0E55B7BB,0xFBACA499,
                          0xAC5018FF,0xBBE1649A,0xD07ECEA7,0x0DE5AAD5,0x1685F60F,0xD36C8239,0x8FFBD542,0x664CD016,
                          0x0000008E};

    //pointAdd vector 1
    uint32_t k1G_k2P_x[]   = {0x32D02076,0xDFBB2BC5,0xD591CC02,0x9A951AD7,0xC49EC129,0x08D69931,0x814A1559,0xCB68B51F,
                              0x58C3137D,0xBE75E87B,0xE849BEBC,0x9CDF57D1,0x73DE6EED,0x51CE92FF,0x7F9DBCD4,0x0FE2B1B5,
                              0x000001B6};
    uint32_t k1G_k2P_y[]   = {0x58ABB879,0xD3F87729,0x0A3225B8,0xC1047311,0xE06766A1,0xA097AD8F,0xFBAC8E84,0xC4BF5FC8,
                              0xE7213C07,0xBEC9F176,0xAC067BB0,0x1C3BD8EF,0x9AC18586,0xA7221493,0x1BB31148,0x128D81EE,
                              0x00000112};

    //pointAdd vector 2
    uint32_t maxG_maxP_x[]   = {0x904EB438,0x658EB1A8,0x47976A38,0x43614CE4,0x3116A416,0x66770FA8,0xBBDF34A5,0xC9BDBF30,
                                0xEB69910E,0x437F29C9,0x6F0EEABE,0xCA439673,0x323192EE,0x2C6DD6AD,0xB7AF4D17,0x4EB5F4D6,
                                0x000000B5};
    uint32_t maxG_maxP_y[]   = {0x191D3045,0xE06DE2A7,0xDF9C264D,0x2AC19531,0x727A59EE,0x19E216D7,0xE25E57A9,0xEE252AFD,
                                0x962EE86A,0x9FD3A9AB,0x1321067E,0x25235AFC,0xD1E1FD57,0x951F20A8,0xEF64218E,0xE9B86675,
                                0x000000AB};

    eccp_curve_t *curve = (eccp_curve_t *)secp521r1;
    char *curve_name = "secp521r1";

    pke_eccp_pointMul_vector_t pointMul_vector[2];
    pke_eccp_pointAdd_vector_t pointAdd_vector[2];

    pointMul_vector[0].k       = k1;
    pointMul_vector[0].P_x     = curve->eccp_Gx;
    pointMul_vector[0].P_y     = curve->eccp_Gy;
    pointMul_vector[0].kP_x    = k1G_x;
    pointMul_vector[0].kP_y    = k1G_y;
    pointMul_vector[0].dbl_P_x = dbl_G_x;
    pointMul_vector[0].dbl_P_y = dbl_G_y;
    pointMul_vector[0].max_P_x = max_G_x;
    pointMul_vector[0].max_P_y = max_G_y;

    pointMul_vector[1].k       = k2;
    pointMul_vector[1].P_x     = P_x;
    pointMul_vector[1].P_y     = P_y;
    pointMul_vector[1].kP_x    = k2P_x;
    pointMul_vector[1].kP_y    = k2P_y;
    pointMul_vector[1].dbl_P_x = dbl_P_x;
    pointMul_vector[1].dbl_P_y = dbl_P_y;
    pointMul_vector[1].max_P_x = max_P_x;
    pointMul_vector[1].max_P_y = max_P_y;

    if(eccp_pointMul_test(curve, curve_name, pointMul_vector, 2))
    {
        return 1;
    }

    pointAdd_vector[0].P1_x = k1G_x;
    pointAdd_vector[0].P1_y = k1G_y;
    pointAdd_vector[0].P2_x = k2P_x;
    pointAdd_vector[0].P2_y = k2P_y;
    pointAdd_vector[0].Q_x  = k1G_k2P_x;
    pointAdd_vector[0].Q_y  = k1G_k2P_y;

    pointAdd_vector[1].P1_x = max_G_x;
    pointAdd_vector[1].P1_y = max_G_y;
    pointAdd_vector[1].P2_x = max_P_x;
    pointAdd_vector[1].P2_y = max_P_y;
    pointAdd_vector[1].Q_x  = maxG_maxP_x;
    pointAdd_vector[1].Q_y  = maxG_maxP_y;

    if(eccp_pointAdd_test(curve, curve_name, pointAdd_vector, 2))
    {
        return 1;
    }

#ifdef ECCP_POINT_DOUBLE
    if(eccp_pointDouble_test(curve, curve_name, pointMul_vector, 2))
    {
        return 1;
    }
#endif

    return 0;
}
#endif


#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 160) && defined(SUPPORT_BRAINPOOLP160R1))
uint32_t eccp_brainpoolp160r1_test(void)
{
    //pointMul vector 1
    uint32_t k1[]      = {0xFCFD7289,0x23F05E7E,0x331A146A,0x31A21DDB,0xE9449273};
    uint32_t k1G_x[]   = {0x499BA682,0xB503FB13,0x3D8A016D,0x1D8C05F1,0x366A4501};
    uint32_t k1G_y[]   = {0xEAE06B01,0x06B27830,0x71A11181,0x5E896072,0xCDB6A4C3};
    uint32_t dbl_G_x[] = {0xC2A2459D,0xA3225161,0x3147A858,0xF5A6B954,0xC750DB2A};
    uint32_t dbl_G_y[] = {0x91641EF7,0x5B44BCE7,0xA09D3B19,0xD46BB965,0x115E0A63};
    uint32_t max_G_x[] = {0x0F706B77,0xF6FA7AAE,0x0730E12E,0xFD4FF63E,0x59EA55C9};
    uint32_t max_G_y[] = {0x0F6C5912,0xA6A5EADA,0x98CA5C4A,0xD072BC81,0xA10F9DC4};

    //pointMul vector 2
    uint32_t k2[]      = {0xA91362CB,0xCB7787D3,0x5D8BAA08,0x0FA73D6C,0xC78E2B88};
    uint32_t P_x[]     = {0xA4BBC008,0x7EB192DE,0xF8777D4F,0x01AA0EC1,0xB4AA0AAA};
    uint32_t P_y[]     = {0x9E7A1549,0xA414082D,0x1006B650,0x977EBCC9,0x9DC6DD8E};
    uint32_t k2P_x[]   = {0xA26C3A5F,0x5BA3293E,0xEAF30C5D,0x75118087,0x0E49D8B0};
    uint32_t k2P_y[]   = {0xCA996615,0x7BD2760B,0xCB3DBF35,0x41DB8FA4,0xD153E546};
    uint32_t dbl_P_x[] = {0x87345F3C,0x5F1928F1,0x78D08A80,0x57E1229C,0x48BD284E};
    uint32_t dbl_P_y[] = {0xC95BA489,0x2B084316,0x1467165E,0xB6259B7F,0x1394FB8D};
    uint32_t max_P_x[] = {0xAA7762AC,0x9942A594,0x8AB0FA00,0x1E1EE763,0xE4E42C98};
    uint32_t max_P_y[] = {0xA1F488DF,0x4191B388,0x56A19CD0,0x89DAB041,0x355576E7};

    //pointAdd vector 1
    uint32_t k1G_k2P_x[]   = {0xF7BAAA3E,0x7351E0C7,0x4E51304E,0x9C7288FD,0x13629D01};
    uint32_t k1G_k2P_y[]   = {0x5726F4F9,0x88B5570E,0xC91DB03D,0x6A7C58E6,0x47A823D3};

    //pointAdd vector 2
    uint32_t maxG_maxP_x[]   = {0x5346B256,0xD6DF7026,0x2FB2C877,0xD442781D,0xBBB2F5E0};
    uint32_t maxG_maxP_y[]   = {0x6240A485,0x9BD35410,0xC48F45A9,0xFDA1E305,0x7A8E8552};

    eccp_curve_t *curve = (eccp_curve_t *)brainpoolp160r1;
    char *curve_name = "brainpoolp160r1";

    pke_eccp_pointMul_vector_t pointMul_vector[2];
    pke_eccp_pointAdd_vector_t pointAdd_vector[2];

    pointMul_vector[0].k       = k1;
    pointMul_vector[0].P_x     = curve->eccp_Gx;
    pointMul_vector[0].P_y     = curve->eccp_Gy;
    pointMul_vector[0].kP_x    = k1G_x;
    pointMul_vector[0].kP_y    = k1G_y;
    pointMul_vector[0].dbl_P_x = dbl_G_x;
    pointMul_vector[0].dbl_P_y = dbl_G_y;
    pointMul_vector[0].max_P_x = max_G_x;
    pointMul_vector[0].max_P_y = max_G_y;

    pointMul_vector[1].k       = k2;
    pointMul_vector[1].P_x     = P_x;
    pointMul_vector[1].P_y     = P_y;
    pointMul_vector[1].kP_x    = k2P_x;
    pointMul_vector[1].kP_y    = k2P_y;
    pointMul_vector[1].dbl_P_x = dbl_P_x;
    pointMul_vector[1].dbl_P_y = dbl_P_y;
    pointMul_vector[1].max_P_x = max_P_x;
    pointMul_vector[1].max_P_y = max_P_y;

    if(eccp_pointMul_test(curve, curve_name, pointMul_vector, 2))
    {
        return 1;
    }

    pointAdd_vector[0].P1_x = k1G_x;
    pointAdd_vector[0].P1_y = k1G_y;
    pointAdd_vector[0].P2_x = k2P_x;
    pointAdd_vector[0].P2_y = k2P_y;
    pointAdd_vector[0].Q_x  = k1G_k2P_x;
    pointAdd_vector[0].Q_y  = k1G_k2P_y;

    pointAdd_vector[1].P1_x = max_G_x;
    pointAdd_vector[1].P1_y = max_G_y;
    pointAdd_vector[1].P2_x = max_P_x;
    pointAdd_vector[1].P2_y = max_P_y;
    pointAdd_vector[1].Q_x  = maxG_maxP_x;
    pointAdd_vector[1].Q_y  = maxG_maxP_y;

    if(eccp_pointAdd_test(curve, curve_name, pointAdd_vector, 2))
    {
        return 1;
    }

#ifdef ECCP_POINT_DOUBLE
    if(eccp_pointDouble_test(curve, curve_name, pointMul_vector, 2))
    {
        return 1;
    }
#endif

    return 0;
}
#endif


#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 512) && defined(SUPPORT_BRAINPOOLP512R1))
uint32_t eccp_brainpoolp512r1_test(void)
{
    //vector 1
    uint32_t k1[]      = {0xE4C3D209,0x3AA74802,0xC6B3CF7E,0x4EF0869C,0x3FF9E38C,0x0F38F5A5,0xEC1261D2,0x0ADF56FB,
                          0xFDBBF051,0x03D1BA61,0x4BEEE803,0x48C66EE2,0x24C374F1,0xB92CBBDB,0x561523DC,0x476D72EC};
    uint32_t k1G_x[]   = {0xC7371A6A,0x735ACB07,0x25009DD9,0x8BEE70BB,0xBD942FB0,0x049E0A30,0x73AEBB0D,0x8FD95B2C,
                          0xD57BF32A,0x1CCE9879,0x9CF37D91,0xDA1ACD1F,0x6710CC0C,0x801A0F4A,0xE0E9BC47,0x2D30418C};
    uint32_t k1G_y[]   = {0x83A487F7,0x2A2736DD,0x7A3ED628,0xDCB900EC,0xCC91AF42,0x26B97AD3,0xB38CC212,0xA6DAB59A,
                          0xC65E755C,0x88F290EA,0x90EE5BC2,0x0040943E,0xDB5841F1,0x966FFCDA,0x1E2CE4B9,0x012AF993};
    uint32_t dbl_G_x[] = {0xF02F4850,0x080447DE,0x089A4CC5,0xF698F404,0x2915A4F4,0x97116E70,0xA8F0F725,0x957877F3,
                          0xF4CB6581,0xA30D3035,0xB8A18064,0xD18D8141,0x58F399B3,0x0A632857,0x80EDF980,0x9F4945F6};
    uint32_t dbl_G_y[] = {0xBE592E71,0xE269AD21,0x81FD929C,0x63A42E41,0xB0960CBF,0xA5C3B602,0x709C700F,0xFDF99BB1,
                          0x03E5D1B2,0x5A2C366B,0x8D3F1E0F,0x149CE123,0x6292F29D,0x49826B71,0x8B699C56,0x6D6B4B18};
    uint32_t max_G_x[] = {0x2AC6FCED,0xAE5CDEC7,0x2D3E7FEF,0xCBD3E720,0x41999C7C,0x15DFD44C,0xAA1CE052,0x5C2C8733,
                          0xB23A0E67,0xD80B317F,0x15F73765,0x8B6E2685,0xBF64E741,0xD4FBC892,0xEED6BDCB,0x397CBE99};
    uint32_t max_G_y[] = {0x818B92E8,0xF6A22445,0x173944E0,0x20BCBD0F,0xD4394DFE,0x8351CEAD,0x552E6E40,0xC44F3387,
                          0xDD4788BC,0x39566EDC,0x78756303,0x3218ED53,0xEFDDFC66,0xC9B122E9,0x47A28DAB,0xAAC34BFC};

    //vector 2
    uint32_t k2[]      = {0x209ECD9E,0x1166FFB1,0xDCF86392,0x390BF68F,0xCB8A5D3A,0x50388180,0x3E6CF573,0x530D2E3F,
                          0xAC36095B,0x7F1ACBDF,0x4E5F9047,0x0CF537CF,0x10BEFC31,0x5099D12F,0x7F62F561,0x436D0303};
    uint32_t P_x[]     = {0xFFDD9431,0xFFE82C2D,0xDED914A0,0x3E59AD64,0xE5774FF4,0x448FF85A,0xE34201C2,0x5DBC4093,
                          0x7A5733A1,0xFAB6B1E3,0xF4DE45EA,0xF8277D91,0x9D4F0F77,0x3C25023C,0x27BB5DCF,0x26D8C202};
    uint32_t P_y[]     = {0x5A43EC85,0x910EEC7A,0xED6468DD,0x8C70534C,0x605F18C3,0xFFD41629,0x5488DEFF,0xBA82D5ED,
                          0x9A2F8805,0x0094855F,0x6460622C,0xC1633A42,0x2DA140E2,0x1E9861FC,0x855D7F95,0x0B90E33C};
    uint32_t k2P_x[]   = {0xD2F0C5C3,0x3EE8F5F7,0x47AEA985,0xFD068A93,0xDDAD265D,0x72E69EF5,0x8739A562,0xFBFF7225,
                          0x13328ED2,0x24C1D6AC,0xC680CBF4,0xACE0B8F7,0x40C2C645,0x775F6E4F,0xD94AC5E3,0x44399169};
    uint32_t k2P_y[]   = {0x23613044,0xBA233BA0,0xBB029F12,0x87C5FD64,0x7E8F67FA,0xE0F695B6,0x41C07D67,0xA0BA42A4,
                          0x291A05B6,0x72A546CD,0x8F321DB5,0x72A82469,0xD0A56ACD,0x84099364,0x4C1555C3,0x8D46B443};
    uint32_t dbl_P_x[] = {0x346D84AE,0xD747418A,0x4A23E532,0x657B0431,0xC9DBA2F4,0xF5B98FE5,0xED39B367,0xB0F5BE10,
                          0xA8B343DE,0x40BB54DB,0x1FB4F5E5,0xB700D1AC,0xED4A6B53,0x1A950BAC,0x3967A730,0x8B659ED4};
    uint32_t dbl_P_y[] = {0xF2202024,0xB6F11BEC,0xF6BBCB78,0x488CEFB3,0xB8CDFB24,0xAC17B29C,0xC189DDEA,0x70193045,
                          0x9F6E1AFF,0x9033ACB2,0x8E1216F5,0x6D8E73F0,0xA285326E,0x6BA11DF3,0x584A154E,0x4036D1C2};
    uint32_t max_P_x[] = {0x2795377F,0xC783A98D,0xC86EB60D,0x95590A9B,0x77C1EDBA,0x6F2C0A75,0x524F5CCF,0xF6ABC079,
                          0x99A2B41B,0xDA811B00,0x76BA42E1,0x7F936964,0x6CC87EE5,0x88B670FA,0x031A7944,0x017C2004};
    uint32_t max_P_y[] = {0xCF06A411,0x89CA8465,0x935CCB8F,0x4D276267,0x23E830DF,0x4A9CA8D5,0xCA332168,0x20F898D3,
                          0x1290E6BB,0x1B816FA6,0x2BAF40B5,0xBEC1906B,0x045990E5,0x19644530,0xC648BB2C,0x7BEB78EE};

    //pointAdd vector 1
    uint32_t k1G_k2P_x[]   = {0x8A4B98FD,0x15310E37,0x747AC3DE,0xC9836AF0,0x97A9E5D4,0xF058E329,0xEA742069,0x679A29EB,
                              0x0CA7C2D1,0xCFA2269F,0x4F20E2F4,0xB78E94EC,0xACE582C2,0x338B57E2,0x7D070CD0,0x9279B2DA};
    uint32_t k1G_k2P_y[]   = {0xCD640BD1,0xC45150C3,0xB9885002,0xCAAC3D66,0xF8833A14,0xFC31FFFC,0x64788258,0x363EC522,
                              0x1C2B6DDD,0xD21069A0,0x65ADC690,0x5E1EC6CB,0x5EE51C19,0x86B84699,0x31AE2B3C,0x02A4508D};

    //pointAdd vector 2
    uint32_t maxG_maxP_x[]   = {0xC2E5743B,0x7257CC32,0x2ADA37A4,0x1BDE4E93,0xAEDF3036,0x4319AB44,0x918670C2,0x1D0F3E2B,
                                0xAF556C4B,0x25874B6D,0xACCB60D3,0xD4BACC8D,0x9406CF5D,0x5DE7C6F0,0xB0849D92,0x70B9E734};
    uint32_t maxG_maxP_y[]   = {0xEEF68A5E,0xD1B5D48E,0x39835081,0x6652E10C,0xE8824FF8,0xD329336A,0xDEFD6B31,0x7D56BA1B,
                                0x218AD2B4,0xB9E06C72,0xF0B72ACC,0x562BBB30,0x0B6B740F,0x8B8CA407,0xFBA673F9,0x63CCD43B};

    eccp_curve_t *curve = (eccp_curve_t *)brainpoolp512r1;
    char *curve_name = "brainpoolp512r1";

    pke_eccp_pointMul_vector_t pointMul_vector[2];
    pke_eccp_pointAdd_vector_t pointAdd_vector[2];

    pointMul_vector[0].k       = k1;
    pointMul_vector[0].P_x     = curve->eccp_Gx;
    pointMul_vector[0].P_y     = curve->eccp_Gy;
    pointMul_vector[0].kP_x    = k1G_x;
    pointMul_vector[0].kP_y    = k1G_y;
    pointMul_vector[0].dbl_P_x = dbl_G_x;
    pointMul_vector[0].dbl_P_y = dbl_G_y;
    pointMul_vector[0].max_P_x = max_G_x;
    pointMul_vector[0].max_P_y = max_G_y;

    pointMul_vector[1].k       = k2;
    pointMul_vector[1].P_x     = P_x;
    pointMul_vector[1].P_y     = P_y;
    pointMul_vector[1].kP_x    = k2P_x;
    pointMul_vector[1].kP_y    = k2P_y;
    pointMul_vector[1].dbl_P_x = dbl_P_x;
    pointMul_vector[1].dbl_P_y = dbl_P_y;
    pointMul_vector[1].max_P_x = max_P_x;
    pointMul_vector[1].max_P_y = max_P_y;

    if(eccp_pointMul_test(curve, curve_name, pointMul_vector, 2))
    {
        return 1;
    }

    pointAdd_vector[0].P1_x = k1G_x;
    pointAdd_vector[0].P1_y = k1G_y;
    pointAdd_vector[0].P2_x = k2P_x;
    pointAdd_vector[0].P2_y = k2P_y;
    pointAdd_vector[0].Q_x  = k1G_k2P_x;
    pointAdd_vector[0].Q_y  = k1G_k2P_y;

    pointAdd_vector[1].P1_x = max_G_x;
    pointAdd_vector[1].P1_y = max_G_y;
    pointAdd_vector[1].P2_x = max_P_x;
    pointAdd_vector[1].P2_y = max_P_y;
    pointAdd_vector[1].Q_x  = maxG_maxP_x;
    pointAdd_vector[1].Q_y  = maxG_maxP_y;

    if(eccp_pointAdd_test(curve, curve_name, pointAdd_vector, 2))
    {
        return 1;
    }

#ifdef ECCP_POINT_DOUBLE
    if(eccp_pointDouble_test(curve, curve_name, pointMul_vector, 2))
    {
        return 1;
    }
#endif

    return 0;
}
#endif


#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 256) && defined(SUPPORT_SM9))
uint32_t eccp_sm9p256v1_test(void)
{
    //vector 1
    uint32_t k1[]      = {0x9FFF5AA8,0x25DE64CB,0x1A69C999,0x9C8BB3D0,0x78E52060,0x8E4C76C4,0x68C7FDB0,0xF9BFE893};
    uint32_t k1G_x[]   = {0x0df9d114,0x3cac679d,0xa0c00d76,0xb1eac5de,0x2060e27c,0x91c34d4b,0x220fe5cd,0x52ab00ea};
    uint32_t k1G_y[]   = {0x7f0b3f17,0xf6a6df36,0x51cbf5b4,0x08403785,0x488b78bc,0xfb167c8e,0x528a70c3,0x72c4b977};
    uint32_t dbl_G_x[] = {0x5563f4b2,0x33347c29,0xee2eee2f,0x61d539ac,0xb2f8b3a6,0x3d43546f,0xc761cd35,0x98308a2c};
    uint32_t dbl_G_y[] = {0xd412a1fd,0xc25b85d0,0x8b2a814b,0xd1ccbac8,0xb2fee68b,0xca48a0cb,0x776ea1dd,0x5c8edf80};
    uint32_t max_G_x[] = {0x26186eb0,0x07ed657d,0xbb20fa28,0xa6867990,0xce3ba942,0xe943cd3b,0x21761f12,0x79ee17f6};
    uint32_t max_G_y[] = {0x5ebaa975,0x3cec9c69,0x5e9d4bda,0x5a366b16,0x59379eb1,0x9af851fd,0x2e9d8bda,0xae7b2b64};

    //vector 2
    uint32_t k2[]      = {0x0496d34f,0x5dcf1630,0x6077c442,0x50f69361,0x37c99247,0xa5fe085b,0xb931b3bc,0x3c6ba03f};
    uint32_t P_x[]     = {0x4b3a06b0,0x4d950b88,0xf84aa2b2,0x08259857,0xbe78affd,0x641226bc,0x8a53ec0c,0x4265ed94};
    uint32_t P_y[]     = {0x2313b7af,0x1ea03ed7,0xcbb71aad,0xfb7dda97,0x1e0b9dfa,0xb1f531c6,0x93a69a13,0x0d3e55ed};
    uint32_t k2P_x[]   = {0x8d0a8d52,0x8600249d,0x3158b108,0xd122fda9,0xf2b4d7bc,0x06fefc54,0x5c4005eb,0x61538517};
    uint32_t k2P_y[]   = {0xd6d39e6c,0xda800960,0x0cfabb58,0xde170b65,0xbe89da20,0xa8a37e34,0x607d9859,0x4d48232f};
    uint32_t dbl_P_x[] = {0x81195663,0xd6e4d40c,0xf1b0bab8,0xe7979cdf,0xe6219568,0x1b4911d3,0xe5d5a529,0x12890b97};
    uint32_t dbl_P_y[] = {0x9199834d,0xe80f1336,0x022bf12c,0x2ff312e9,0xdb65a8f3,0xeec8cbbc,0x38e6e982,0xb3d932ef};
    uint32_t max_P_x[] = {0x11d3511c,0xe4a28417,0x06011a4a,0xe4a9d751,0x2e7186bc,0x7f8880da,0xdce48ef8,0x15186db3};
    uint32_t max_P_y[] = {0xd49c8f1f,0x87f7420c,0xc24a466e,0xb8eeff8f,0x029b3d7d,0x59e57102,0x92281544,0x6383e448};

    //pointAdd vector 1
    uint32_t k1G_k2P_x[]   = {0x4214112e,0xd436a8a5,0xb06c5956,0xf730ddbf,0xa5d5572a,0x4a11f4c8,0xa6d04157,0x3639e036};
    uint32_t k1G_k2P_y[]   = {0x6488e372,0x41108195,0x83a1d44d,0x99a75d27,0x8abd0949,0xd6b41a1d,0x8db28509,0x5563b1d0};

    //pointAdd vector 2
    uint32_t maxG_maxP_x[]   = {0xd5cb97bc,0xf2798447,0xf3ab20fc,0x943915a5,0xd3161127,0xf06350a2,0xa9e5577b,0x71b0ac82};
    uint32_t maxG_maxP_y[]   = {0x681b3a70,0x8a797f51,0x175df92f,0xa3ae406a,0xae8a0f5e,0xb6ec1852,0xd1361ac8,0x55c80382};

    eccp_curve_t *curve = (eccp_curve_t *)sm9_curve;
    char *curve_name = "sm9p256v1";

    pke_eccp_pointMul_vector_t pointMul_vector[2];
    pke_eccp_pointAdd_vector_t pointAdd_vector[2];

    pointMul_vector[0].k       = k1;
    pointMul_vector[0].P_x     = curve->eccp_Gx;
    pointMul_vector[0].P_y     = curve->eccp_Gy;
    pointMul_vector[0].kP_x    = k1G_x;
    pointMul_vector[0].kP_y    = k1G_y;
    pointMul_vector[0].dbl_P_x = dbl_G_x;
    pointMul_vector[0].dbl_P_y = dbl_G_y;
    pointMul_vector[0].max_P_x = max_G_x;
    pointMul_vector[0].max_P_y = max_G_y;

    pointMul_vector[1].k       = k2;
    pointMul_vector[1].P_x     = P_x;
    pointMul_vector[1].P_y     = P_y;
    pointMul_vector[1].kP_x    = k2P_x;
    pointMul_vector[1].kP_y    = k2P_y;
    pointMul_vector[1].dbl_P_x = dbl_P_x;
    pointMul_vector[1].dbl_P_y = dbl_P_y;
    pointMul_vector[1].max_P_x = max_P_x;
    pointMul_vector[1].max_P_y = max_P_y;

    if(eccp_pointMul_test(curve, curve_name, pointMul_vector, 2))
    {
        return 1;
    }

    pointAdd_vector[0].P1_x = k1G_x;
    pointAdd_vector[0].P1_y = k1G_y;
    pointAdd_vector[0].P2_x = k2P_x;
    pointAdd_vector[0].P2_y = k2P_y;
    pointAdd_vector[0].Q_x  = k1G_k2P_x;
    pointAdd_vector[0].Q_y  = k1G_k2P_y;

    pointAdd_vector[1].P1_x = max_G_x;
    pointAdd_vector[1].P1_y = max_G_y;
    pointAdd_vector[1].P2_x = max_P_x;
    pointAdd_vector[1].P2_y = max_P_y;
    pointAdd_vector[1].Q_x  = maxG_maxP_x;
    pointAdd_vector[1].Q_y  = maxG_maxP_y;

    if(eccp_pointAdd_test(curve, curve_name, pointAdd_vector, 2))
    {
        return 1;
    }

#ifdef ECCP_POINT_DOUBLE
    if(eccp_pointDouble_test(curve, curve_name, pointMul_vector, 2))
    {
        return 1;
    }
#endif

    return 0;
}
#endif


#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 160) && defined(SUPPORT_SECP160K1))
uint32_t eccp_secp160k1_std_test(void)
{
    uint32_t k[] ={0x6A7F441B,0xC523FEB4,0x425A1DE0,0xD96CEA76,0x1911ABE9,0x00000000};
    uint32_t Px[]={0xD30A3542,0xE021D513,0x3237BC31,0xB25DA41C,0x16571863};
    uint32_t Py[]={0xBFAF9778,0xB8607BAE,0x5B46E825,0x793DB2CE,0xFD1C256F};
    uint32_t Qx[]={0x6533A0E3,0x227AFF94,0x54EE5FA0,0x95E965FC,0x74FC71CB};
    uint32_t Qy[]={0xF358CDF9,0x9F0DB1DC,0xED3EB7D4,0x57B56930,0xCEC51789};

    return eccp_std_test((eccp_curve_t *)secp160k1, "secp160k1", k, Px, Py, Qx, Qy);
}
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 192) && defined(SUPPORT_SECP192K1))
uint32_t eccp_secp192k1_std_test(void)
{
    uint32_t k[] ={0xB8E0C430,0xBA697104,0xEC3A884B,0x35BF8484,0x1BC07711,0x80B671B1};
    uint32_t Px[]={0x29FD9A62,0x2E7F7E67,0x74AFD08D,0x71E92B65,0x2D02E10B,0xFE81F619};
    uint32_t Py[]={0x232F3182,0xCB3B0ACC,0x91C898FE,0xB06ADD2E,0x13E20E3A,0xBE2C20C5};
    uint32_t Qx[]={0x94F93CB6,0xA8BED93B,0x9CD1D4B3,0x84F5D254,0x31B17476,0xF091CF63};
    uint32_t Qy[]={0xA18F2ED5,0x03DA2D98,0x1C5E42E6,0x2E626866,0x1E7565A0,0xFD7AF42E};

    return eccp_std_test((eccp_curve_t *)secp192k1, "secp192k1", k, Px, Py, Qx, Qy);
}
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 224) && defined(SUPPORT_SECP224K1))
uint32_t eccp_secp224k1_std_test(void)
{
    uint32_t k[] ={0x86AA4F4B,0x072B67BB,0x1147682E,0xD56AB3B8,0xB2D6613B,0x5B5932E0,0x4C1AC5CB,0x00000000};
    uint32_t Px[]={0x64C50EC4,0x3CC6BC65,0x05551911,0x9F60A619,0xC74EE5BA,0xD8A9B7FA,0xC6ACFEE1};
    uint32_t Py[]={0xE5D92892,0x027EC7E9,0x4C3E80A8,0x97C3A0DF,0xD761ACAF,0x4B7EA486,0x3AA28A65};
    uint32_t Qx[]={0x26749E60,0xA1CE8CF4,0x96B9639F,0x232B9BF5,0x390999A0,0x6AEB9712,0x86C0DEB5};
    uint32_t Qy[]={0x9F06B69A,0xDBF4A9FA,0x539ED633,0x906B855C,0x5B474A79,0x4E108555,0x8F598C95};

    return eccp_std_test((eccp_curve_t *)secp224k1, "secp224k1", k, Px, Py, Qx, Qy);
}
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 256) && defined(SUPPORT_SECP256K1))
uint32_t eccp_secp256k1_std_test(void)
{
    uint32_t k[] ={0x7E2D3D65,0xFAC0D7D6,0x5D0FA4F5,0xEEDE4A34,0x7EDEAAF3,0x01701309,0x1A2580CA,0xDF7908E5};
    uint32_t Px[]={0x804B6121,0x0F099990,0x8806AF17,0xE1FB945F,0x964BDB08,0xD99E9B0B,0x128E18D9,0x223284F6};
    uint32_t Py[]={0x92A014FF,0xDEFB4225,0x62BE16C0,0x5EEC0BE8,0x8ED51E33,0x956D18F3,0xDA31B79A,0x248AD941};
    uint32_t Qx[]={0x5C709EE5,0xABAC09B9,0x8CEF3CA7,0x5C778E4B,0x95C07CD8,0x3045406E,0x41ED7D6D,0xC6047F94};
    uint32_t Qy[]={0x50CFE52A,0x236431A9,0x3266D0E1,0xF7F63265,0x466CEAEE,0xA3C58419,0xA63DC339,0x1AE168FE};

    return eccp_std_test((eccp_curve_t *)secp256k1, "secp256k1", k, Px, Py, Qx, Qy);
}
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 160) && defined(SUPPORT_BRAINPOOLP160R1))
uint32_t eccp_brainpoolp160r1_std_test(void)
{
    uint32_t k[] ={0xFCFD7289,0x23F05E7E,0x331A146A,0x31A21DDB,0xE9449273};
    uint32_t Px[]={0x499BA682,0xB503FB13,0x3D8A016D,0x1D8C05F1,0x366A4501};
    uint32_t Py[]={0xEAE06B01,0x06B27830,0x71A11181,0x5E896072,0xCDB6A4C3};
    uint32_t Qx[]={0xC2A2459D,0xA3225161,0x3147A858,0xF5A6B954,0xC750DB2A};
    uint32_t Qy[]={0x91641EF7,0x5B44BCE7,0xA09D3B19,0xD46BB965,0x115E0A63};

    return eccp_std_test((eccp_curve_t *)brainpoolp160r1, "brainpoolp160r1", k, Px, Py, Qx, Qy);
}
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 160) && defined(SUPPORT_SECP160R1))
uint32_t eccp_secp160r1_std_test(void)
{
    uint32_t k[] ={0x6D41374A,0x06DE4725,0x6FEFE81C,0xF5CE123C,0xDCA8DA89,0x00000000};
    uint32_t Px[]={0x9434ED36,0x8BE60EEA,0xE1F5040D,0x1EA15589,0xE1845263};
    uint32_t Py[]={0x732BB2E3,0x1310404C,0xFF27EC04,0x2BD0D7B8,0x51B28FE8};
    uint32_t Qx[]={0xE8F46686,0x675D3E92,0x55D3EDF8,0x3C5ED04C,0x02F997F3};
    uint32_t Qy[]={0x7DF8797B,0x21CFB773,0x440E817E,0x482993E9,0xF083A323};

    return eccp_std_test((eccp_curve_t *)secp160r1, "secp160r1", k, Px, Py, Qx, Qy);
}
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 160) && defined(SUPPORT_SECP160R2))
uint32_t eccp_secp160r2_std_test(void)
{
    uint32_t k[] ={0x51F89AD6,0x3248B110,0x2614F1B4,0x7A8A5887,0x46BCB4A6,0x00000000};
    uint32_t Px[]={0x3306FDBE,0x373CAF1E,0x5D981C50,0x353303CA,0xFC18E2C5};
    uint32_t Py[]={0xA0347EF1,0x807FB46D,0x43563A80,0x01C459B2,0x2B1C48EE};
    uint32_t Qx[]={0xB900C8CD,0x883E136A,0x54DD638D,0xA41A0359,0x26788D2B};
    uint32_t Qy[]={0x4B5FFB4A,0xD8DAFACE,0x8EB33CA3,0xD1774497,0xFCE35CEB};

    return eccp_std_test((eccp_curve_t *)secp160r2, "secp160r2", k, Px, Py, Qx, Qy);
}
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 192) && defined(SUPPORT_SECP192R1))
uint32_t eccp_secp192r1_std_test(void)
{
    uint32_t k[] ={0x59B6D9E4,0x46413D0E,0x4C8D8BE4,0xFD07E474,0xE97B29D4,0xAB8E54F2};
    uint32_t Px[]={0xD013ACD8,0x9ED30503,0xD356006F,0x43CDF9C9,0x555767B0,0x4668A941};
    uint32_t Py[]={0x5DAE0776,0xD8817103,0xA3BBB271,0xEDF94723,0x735ABC8D,0x72C8E7B7};
    uint32_t Qx[]={0x6982A888,0x29A70FB1,0x1588A3F6,0xD3553463,0x28783F2A,0xDAFEBF58};
    uint32_t Qy[]={0x5C7E93AB,0x59331AFA,0x141B868F,0x46B27BBC,0x993DA0FA,0xDD6BDA0D};

    return eccp_std_test((eccp_curve_t *)secp192r1, "secp192r1", k, Px, Py, Qx, Qy);
}
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 224) && defined(SUPPORT_SECP224R1))
uint32_t eccp_secp224r1_std_test(void)
{
    uint32_t k[] ={0xB92773A1,0xCE5FAE60,0x13D63F46,0x0F1CFE1F,0x13C40A8F,0x0BFFF9D7,0x92AF36C0};
    uint32_t Px[]={0x10127AFC,0xE4EAB0BB,0x2E2FA920,0x59D54058,0x9CADB7B0,0xD6DFC2F2,0x50B4FECE};
    uint32_t Py[]={0x58A3128C,0xAD143A7B,0xC5E6605C,0xADD7B3DA,0x8279DB38,0x6A5A9B13,0x54EFC79C};
    uint32_t Qx[]={0x1A704FA6,0x32D268FD,0xD16DC180,0x89474788,0x98E60E6D,0x76DCB767,0x706A46DC};
    uint32_t Qy[]={0xD2E4E8BB,0x7ACF3709,0xFCA62948,0x86892849,0x2A704FA9,0xBC25E770,0x1C2B76A7};

    return eccp_std_test((eccp_curve_t *)secp224r1, "secp224r1", k, Px, Py, Qx, Qy);
}
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 256) && defined(SUPPORT_SECP256R1))
uint32_t eccp_secp256r1_std_test(void)
{
    uint32_t k[] ={0x9FFF5AA8,0x25DE64CB,0x1A69C999,0x9C8BB3D0,0x78E52060,0x8E4C76C4,0x68C7FDB0,0xF9BFE893};
    uint32_t Px[]={0xEDC04207,0xD3C1B21E,0x965E431D,0x34B0FA7E,0x699ABF07,0x0B4BEEF7,0xE7376B8D,0xA20FBCED};
    uint32_t Py[]={0xC25E2AB2,0x8A478DEF,0x7315C253,0x1DFB7060,0x860C3CC0,0x2B01AFAD,0x52431001,0xFB1BEC0F};
    uint32_t Qx[]={0x47669978,0xA60B48FC,0x77F21B35,0xC08969E2,0x04B51AC3,0x8A523803,0x8D034F7E,0x7CF27B18};
    uint32_t Qy[]={0x227873D1,0x9E04B79D,0x3CE98229,0xBA7DADE6,0x9F7430DB,0x293D9AC6,0xDB8ED040,0x07775510};

    return eccp_std_test((eccp_curve_t *)secp256r1, "secp256r1", k, Px, Py, Qx, Qy);
}
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 384) && defined(SUPPORT_SECP384R1))
uint32_t eccp_secp384r1_std_test(void)
{
    uint32_t k[]  ={0x848FB704,0xBF0D559A,0xB42C4E7F,0x96895127,0x10F762AA,0xE6C487C2,0x98D14E45,0x56B8F302,
                    0xBE470462,0xB16ED15E,0xB0BFF538,0xB14110B0};
    uint32_t Px[] ={0xAA4EF4D0,0x7CC5A90C,0x565FA4DD,0xDD4E71C0,0x358FDC54,0x5680D101,0x17B32A1A,0xBD5A9287,
                    0x2B4A051F,0x39982236,0x9D41B012,0x9BA29F29};
    uint32_t Py[] ={0x8A01AAEE,0x2C3BDD01,0x15267777,0x3DBA3813,0xAF76B7F7,0xDCC05745,0x39177B66,0xD959E2C8,
                    0xFFC57090,0x0267618A,0xDDAA0514,0x43F82351};
    uint32_t Qx[] ={0x5295DF61,0x5B96A9C7,0xBE0E64F8,0x4FE0E86E,0x9FB96E9E,0x51D207D1,0xA6F434D6,0x89025959,
                    0xC55B97F0,0x69260045,0x7BA3D2D9,0x08D99905};
    uint32_t Qy[] ={0x0A940E80,0x61501E70,0x4D39E22D,0x5FFD43E9,0x256AB425,0x904E505F,0xBC6CC43E,0xB275D875,
                    0xFD6DBA74,0xB7BFE8DF,0x5B1B3CED,0x8E80F1FA};

    return eccp_std_test((eccp_curve_t *)secp384r1, "secp384r1", k, Px, Py, Qx, Qy);
}
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 512) && defined(SUPPORT_BRAINPOOLP512R1))
uint32_t eccp_brainpoolp512r1_std_test(void)
{
    uint32_t k[] = {0x70D155E6,0xD8FF23D6,0x1C995608,0x6E02E2CE,0xE9A44DDB,0x35BB8F2B,0xC374D596,0x3306261B,
                    0x2FA9E4C0,0x73D4C7CC,0xE07AB98D,0xA871197D,0xCC2032A7,0x4D4C08A2,0xC9491A03,0xA23D0DC0};
    uint32_t Px[]= {0x3D7CC5C3,0x36D513D5,0x70C6819C,0xFC246385,0x75B696D8,0x6A2B68DE,0x7D78DAE3,0x75D9784C,
                    0xF8D39474,0x047708C5,0x94A708AB,0x7EB0AF78,0x6135B9D0,0x3E933A65,0x0E5F6A50,0x5ABB8CC4};
    uint32_t Py[]= {0x4209C93F,0x078231B4,0xDEF8EF61,0x3D58E479,0xD7D4F5DE,0x10CCCA2F,0x04F2147F,0xF32C422A,
                    0xB3BBEF51,0x76CE5D30,0x7B7DC485,0xF3C44DB4,0x50603B1B,0xB5D354DA,0x7CE6E6F2,0x1DA0BFAF};
    uint32_t Qx[]= {0xF02F4850,0x080447DE,0x089A4CC5,0xF698F404,0x2915A4F4,0x97116E70,0xA8F0F725,0x957877F3,
                    0xF4CB6581,0xA30D3035,0xB8A18064,0xD18D8141,0x58F399B3,0x0A632857,0x80EDF980,0x9F4945F6};
    uint32_t Qy[]= {0xBE592E71,0xE269AD21,0x81FD929C,0x63A42E41,0xB0960CBF,0xA5C3B602,0x709C700F,0xFDF99BB1,
                    0x03E5D1B2,0x5A2C366B,0x8D3F1E0F,0x149CE123,0x6292F29D,0x49826B71,0x8B699C56,0x6D6B4B18};

    return eccp_std_test((eccp_curve_t *)brainpoolp512r1, "brainpoolp512r1", k, Px, Py, Qx, Qy);
}
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 521) && defined(SUPPORT_SECP521R1))
uint32_t eccp_secp521r1_std_test(void)
{
    uint32_t k[]  ={0x4834D06A,0x81EF5DB6,0x23C21CA1,0xB2D215D4,0x6EF25DE4,0xB23B7597,0x0EA6B362,0x7C991193,
                    0x69C96558,0xA69C4F54,0x07879DA5,0xF93C22E5,0xFA992240,0x6770C291,0xB05A0874,0xB22CD4EC,0x000001D1};
    uint32_t Px[] ={0x306C5B65,0xB8468589,0xEF7A7498,0xE1051508,0xD42907AF,0x22E2660F,0xAE075B2A,0xFEFA1391,
                    0x4B9DDDA6,0xA8CA9FC6,0x6DCBEDEA,0xE14036F1,0xE2E59A13,0x1730AD26,0x8A841745,0x644CDE9F,0x0000017B};
    uint32_t Py[] ={0xADB617D7,0x8865A8FF,0xC94DFD0D,0x38621B7B,0x90D18650,0xDE5F4A52,0x9671DFA1,0xC12F271B,
                    0xC14ED898,0xBF0F9D0B,0x565A60CE,0x96FC1412,0xEDFCB9B7,0xEF75D644,0x9D6250D5,0xFEC6D00D,0x0000011D};
    uint32_t Qx[] ={0xBA6D783D,0xF43E3933,0xD60FD967,0xCF2FA364,0x35C5AF41,0xAA104A3A,0x6EF55507,0xB3B204DA,
                    0xD769BE97,0x2C6E5505,0x1CCC0635,0x7403279B,0x48C28274,0x2FCB2881,0x277E7E68,0x3C219024,0x00000043};
    uint32_t Qy[] ={0x61F41B02,0x1BE356D6,0xEDC0F4F7,0xEAFCBE95,0x9A3248F4,0x93937FA9,0x9F251F6B,0xB3E377DE,
                    0x06C42DBB,0xAB21A299,0x4DA97740,0xC6B5107C,0xEED3F0B5,0xA7F3ECEE,0x6DB26700,0xBB8CC7F8,0x000000F4};

    return eccp_std_test((eccp_curve_t *)secp521r1, "secp521r1", k, Px, Py, Qx, Qy);
}
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 256) && defined(SUPPORT_SM9))
uint32_t eccp_sm9p256v1_std_test(void)
{
    uint32_t k[] ={0x0A3EA616,0x0C464CD7,0xFA602435,0x1C1C00CB,0x5C395BBC,0x63106512,0x4F21E607,0x21FE8DDA};
    uint32_t Px[]={0x448e4957,0x015e000b,0x678d7f97,0xe1dfa909,0xff34fd1c,0xbc51f622,0xa8a1ef1e,0x92d51af0};
    uint32_t Py[]={0xb095ec57,0x430f3a74,0xddfdcdae,0x724edd97,0xe74ae07f,0x3560a54e,0xb28aa534,0x6dd54c0f};
    uint32_t Qx[]={0x5563F4B2,0x33347C29,0xEE2EEE2F,0x61D539AC,0xB2F8B3A6,0x3D43546F,0xC761CD35,0x98308A2C};
    uint32_t Qy[]={0xD412A1FD,0xC25B85D0,0x8B2A814B,0xD1CCBAC8,0xB2FEE68B,0xCA48A0CB,0x776EA1DD,0x5C8EDF80};

    return eccp_std_test((eccp_curve_t *)sm9_curve, "sm9p256v1", k, Px, Py, Qx, Qy);
}
#endif



uint32_t eccp_speed_test(eccp_curve_t *curve, char *curve_name)
{
    uint32_t k1[ECCP_MAX_WORD_LEN]={0};
    uint32_t Qx[ECCP_MAX_WORD_LEN]={0};
    uint32_t Qy[ECCP_MAX_WORD_LEN]={0};
    uint32_t i, tmpLen, byteLen, wordLen;
    uint32_t round, round_1;
    uint32_t ret;

    printf("\r\n\r\n *************************** %s *************************** \r\n", curve_name);

    uint32_sleep(0x1FFFF, 1);

    if(curve->eccp_p_bitLen <= 160)
    {
        round = 300;
    }
    else if(curve->eccp_p_bitLen <= 192)
    {
        round = 250;
    }
    else if(curve->eccp_p_bitLen <= 224)
    {
        round = 200;
    }
    else if(curve->eccp_p_bitLen <= 256)
    {
        round = 150;
    }
    else if(curve->eccp_p_bitLen <= 384)
    {
        round = 40;
    }
    else if(curve->eccp_p_bitLen <= 521)
    {
        round = 20;
    }
    else if(curve->eccp_p_bitLen <= 638)
    {
        round = 10;
    }

    byteLen = GET_BYTE_LEN(curve->eccp_n_bitLen);
    wordLen = GET_WORD_LEN(curve->eccp_n_bitLen);

    //get random K1
    ret = get_rand((uint8_t *)k1, byteLen);
    if(TRNG_SUCCESS != ret)
    {
        printf("\r\n get rand error, ret = %02x", ret);
        return 1;
    }
    tmpLen = (curve->eccp_n_bitLen)&0x1F;
    if(tmpLen)
    {
        k1[wordLen-1] &= (1<<(tmpLen))-1;
    }


    //test eccp_pointMul
    ret = 0;
    printf("\r\n ======== eccp_pointMul begin ======== ");

#ifdef ECC_SPEED_TEST_BY_TIMER
    round /= 10;
    startP();
#endif

    for(i=0;i<round;i++)
    {
        ret |= eccp_pointMul(curve, k1, curve->eccp_Gx, curve->eccp_Gy, Qx, Qy);
    }

#ifdef ECC_SPEED_TEST_BY_TIMER
    endP(1,0,round);
#else
    printf("\r\n finished");
#endif

    if(PKE_SUCCESS != ret)
    {
        printf("\r\n eccp_pointMul error, ret = %08x\r\n", ret);
        return 1;
    }

    return 0;
}


uint32_t hal_pke_eccp_test(void)
{
    uint32_t i;

    printf("\r\n\r\n =========================== HAL ECCP test ============================= \r\n");

#if 0
//*
#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 160) && defined(SUPPORT_SECP160K1))
    if(eccp_speed_test((eccp_curve_t *)secp160k1, "secp160k1"))
        return 1;
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 192) && defined(SUPPORT_SECP192K1))
    if(eccp_speed_test((eccp_curve_t *)secp192k1, "secp192k1"))
        return 1;
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 224) && defined(SUPPORT_SECP224K1))
    if(eccp_speed_test((eccp_curve_t *)secp224k1, "secp224k1"))
        return 1;
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 256) && defined(SUPPORT_SECP256K1))
    if(eccp_speed_test((eccp_curve_t *)secp256k1, "secp256k1"))
        return 1;
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 160) && defined(SUPPORT_SECP160R1))
    if(eccp_speed_test((eccp_curve_t *)secp160r1, "secp160r1"))
        return 1;
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 160) && defined(SUPPORT_SECP160R2))
    if(eccp_speed_test((eccp_curve_t *)secp160r2, "secp160r2"))
        return 1;
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 192) && defined(SUPPORT_SECP192R1))
    if(eccp_speed_test((eccp_curve_t *)secp192r1, "secp192r1"))
        return 1;
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 224) && defined(SUPPORT_SECP224R1))
    if(eccp_speed_test((eccp_curve_t *)secp224r1, "secp224r1"))
        return 1;
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 256) && defined(SUPPORT_SECP256R1))
    if(eccp_speed_test((eccp_curve_t *)secp256r1, "secp256r1"))
        return 1;
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 384) && defined(SUPPORT_SECP384R1))
    if(eccp_speed_test((eccp_curve_t *)secp384r1, "secp384r1"))
        return 1;
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 521) && defined(SUPPORT_SECP521R1))
    if(eccp_speed_test((eccp_curve_t *)secp521r1, "secp521r1"))
        return 1;
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 160) && defined(SUPPORT_BRAINPOOLP160R1))
    if(eccp_speed_test((eccp_curve_t *)brainpoolp160r1, "brainpoolp160r1"))
        return 1;
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 192) && defined(SUPPORT_BRAINPOOLP192R1))
    if(eccp_speed_test((eccp_curve_t *)brainpoolp192r1, "brainpoolp192r1"))
        return 1;
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 224) && defined(SUPPORT_BRAINPOOLP224R1))
    if(eccp_speed_test((eccp_curve_t *)brainpoolp224r1, "brainpoolp224r1"))
        return 1;
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 256) && defined(SUPPORT_BRAINPOOLP256R1))
    if(eccp_speed_test((eccp_curve_t *)brainpoolp256r1, "brainpoolp256r1"))
        return 1;
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 320) && defined(SUPPORT_BRAINPOOLP320R1))
    if(eccp_speed_test((eccp_curve_t *)brainpoolp320r1, "brainpoolp320r1"))
        return 1;
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 384) && defined(SUPPORT_BRAINPOOLP384R1))
    if(eccp_speed_test((eccp_curve_t *)brainpoolp384r1, "brainpoolp384r1"))
        return 1;
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 512) && defined(SUPPORT_BRAINPOOLP512R1))
    if(eccp_speed_test((eccp_curve_t *)brainpoolp512r1, "brainpoolp512r1"))
        return 1;
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 256) && defined(SUPPORT_BN256))
    if(eccp_speed_test((eccp_curve_t *)bn256, "bn256"))
        return 1;
#endif
		
#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 638) && defined(SUPPORT_BN638))
    if(eccp_speed_test((eccp_curve_t *)bn638, "bn638"))
        return 1;
#endif
//*/
#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 256) && defined(SUPPORT_SM9))
    if(eccp_speed_test((eccp_curve_t *)sm9_curve, "sm9p256v1"))
        return 1;
#endif

#endif


    for(i=0; i<1; i++)
    {
#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 160) && defined(SUPPORT_SECP160K1))
        if(eccp_secp160k1_std_test())
            return 1;
#endif
//*
#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 192) && defined(SUPPORT_SECP192K1))
        if(eccp_secp192k1_std_test())
            return 1;
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 224) && defined(SUPPORT_SECP224K1))
        if(eccp_secp224k1_std_test())
            return 1;
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 256) && defined(SUPPORT_SECP256K1))
        if(eccp_secp256k1_std_test())
            return 1;
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 160) && defined(SUPPORT_SECP160R1))
        if(eccp_secp160r1_std_test())
            return 1;
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 160) && defined(SUPPORT_SECP160R2))
        if(eccp_secp160r2_std_test())
            return 1;
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 192) && defined(SUPPORT_SECP192R1))
        if(eccp_secp192r1_std_test())
            return 1;
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 224) && defined(SUPPORT_SECP224R1))
        if(eccp_secp224r1_std_test())
            return 1;
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 256) && defined(SUPPORT_SECP256R1))
        if(eccp_secp256r1_std_test())
            return 1;
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 384) && defined(SUPPORT_SECP384R1))
        if(eccp_secp384r1_std_test())
            return 1;
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 521) && defined(SUPPORT_SECP521R1))
        if(eccp_secp521r1_std_test())
            return 1;
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 160) && defined(SUPPORT_BRAINPOOLP160R1))
        if(eccp_brainpoolp160r1_std_test())
            return 1;
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 512) && defined(SUPPORT_BRAINPOOLP512R1))
        if(eccp_brainpoolp512r1_std_test())
            return 1;
#endif

//*/
#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 256) && defined(SUPPORT_SM9))
        if(eccp_sm9p256v1_std_test())
            return 1;
#endif
    }




#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 160) && defined(SUPPORT_SECP160K1))
    if(eccp_secp160k1_test())
        return 1;
#endif
//*
#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 192) && defined(SUPPORT_SECP192K1))
    if(eccp_secp192k1_test())
        return 1;
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 224) && defined(SUPPORT_SECP224K1))
    if(eccp_secp224k1_test())
        return 1;
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 256) && defined(SUPPORT_SECP256K1))
    if(eccp_secp256k1_test())
        return 1;
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 160) && defined(SUPPORT_SECP160R1))
    if(eccp_secp160r1_test())
        return 1;
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 160) && defined(SUPPORT_SECP160R2))
    if(eccp_secp160r2_test())
        return 1;
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 192) && defined(SUPPORT_SECP192R1))
    if(eccp_secp192r1_test())
        return 1;
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 224) && defined(SUPPORT_SECP224R1))
    if(eccp_secp224r1_test())
        return 1;
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 256) && defined(SUPPORT_SECP256R1))
    if(eccp_secp256r1_test())
        return 1;
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 384) && defined(SUPPORT_SECP384R1))
    if(eccp_secp384r1_test())
        return 1;
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 521) && defined(SUPPORT_SECP521R1))
    if(eccp_secp521r1_test())
        return 1;
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 160) && defined(SUPPORT_BRAINPOOLP160R1))
    if(eccp_brainpoolp160r1_test())
        return 1;
#endif

#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 512) && defined(SUPPORT_BRAINPOOLP512R1))
    if(eccp_brainpoolp512r1_test())
        return 1;
#endif

//*/
#if ((AIC_PKE_ECCP_MAX_BIT_LEN >= 256) && defined(SUPPORT_SM9))
    if(eccp_sm9p256v1_test())
        return 1;
#endif


    return 0;
}

