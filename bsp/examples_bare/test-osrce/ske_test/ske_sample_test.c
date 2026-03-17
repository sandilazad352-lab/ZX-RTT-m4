#include <stdio.h>

#include <utility.h>
#include <ske.h>
#include "ske_test_config.h"

#if defined(AIC_SKE_MODE_CBC_SUPPORT)
#include <ske_cbc.h>
#endif

#if defined(SUPPORT_SKE_MODE_GCM)
#include <ske_gcm_gmac.h>
#endif

#ifdef AIC_SKE_AES_128_SUPPORT

extern void ske_call_manage(void);

#if defined(AIC_SKE_MODE_ECB_SUPPORT)
u32 ske_sec_aes128_ecb_sample(void)
{
    u8 std_in[48]       = {0x81,0x70,0x99,0x44,0xE0,0xCB,0x2E,0x1D,0xB5,0xB0,0xA4,0x77,0xD1,0xA8,0x53,0x9B,
                                0x0A,0x87,0x86,0xE3,0x4E,0xAA,0xED,0x99,0x30,0x3E,0xA6,0x97,0x55,0x95,0xB2,0x45,
                                0x4D,0x5D,0x7F,0x91,0xEB,0xBD,0x4A,0xCD,0x72,0x6C,0x0E,0x0E,0x5E,0x3E,0xB5,0x5E,};
    u8 std_key[16]      = {0xE0,0x70,0x99,0xF1,0xBF,0xAF,0xFD,0x7F,0x24,0x0C,0xD7,0x90,0xCA,0x4F,0xE1,0x34,};
    u8 std_cipher[48]   = {0x0B,0x54,0xE5,0x9F,0x47,0x14,0x4F,0xD8,0xEE,0x43,0x56,0xCA,0x0B,0x2D,0x7A,0x4B,
                                0x84,0xD8,0x17,0x26,0xE2,0x8F,0x59,0xAD,0x95,0x56,0x8C,0x52,0xDA,0x98,0x3F,0x8D,
                                0x65,0xED,0xDD,0x87,0x58,0x90,0xEC,0xE7,0xD8,0x03,0x66,0x9D,0xE2,0xF3,0x57,0x95,};
    u8 std_cipher_48_ansi_x923[64]   = {
                                0x0B,0x54,0xE5,0x9F,0x47,0x14,0x4F,0xD8,0xEE,0x43,0x56,0xCA,0x0B,0x2D,0x7A,0x4B,
                                0x84,0xD8,0x17,0x26,0xE2,0x8F,0x59,0xAD,0x95,0x56,0x8C,0x52,0xDA,0x98,0x3F,0x8D,
                                0x65,0xED,0xDD,0x87,0x58,0x90,0xEC,0xE7,0xD8,0x03,0x66,0x9D,0xE2,0xF3,0x57,0x95,
                                0x06,0xD0,0x99,0xDE,0x5C,0xC2,0xFE,0xF3,0xF0,0x5D,0x1A,0x0E,0x0A,0x8B,0xE7,0x1C,};
    u8 cipher[64], replain[64];
    u32 __attribute__((unused)) *std_in_, *std_out_, *std_out_padding_, *out, *out2;
    u32 out_bytes;
    u32 ret;
#ifdef CONFIG_SKE_SUPPORT_ARCH64_REMAP
    u32 addr_h_0 = 0;
#endif

    SKE_CTX ctx[1];

    printf("\r\n  =================== AES128 ECB sample test ==================== \r\n");

    /************** CPU style(no padding, one-off style) ***************/
    //encrypt
    memset_(cipher, 0, sizeof(cipher));
    ret = ske_sec_crypto(SKE_ALG_AES_128, SKE_MODE_ECB, SKE_CRYPTO_ENCRYPT, std_key, 0, NULL, SKE_NO_PADDING, 
            std_in, cipher, 48, &out_bytes);
    if(SKE_SUCCESS != ret || (48 != out_bytes) || memcmp_(std_cipher, cipher, out_bytes))
    {
        printf("%s line-%u AES128 ecb encryption(no padding, one-off style) failure, ret=%x\r\n", __func__, __LINE__, ret);fflush(stdout);
        print_buf_U8(std_key, 16, "std_key");
        print_buf_U8(std_in, 48, "std_in");
        print_buf_U8(std_cipher, 48, "std_cipher");
        print_buf_U8(cipher, 48, "cipher");
        return 1;
    }
    else
    {
        printf("%s line-%u AES128 ecb encryption(no padding, one-off style) success\r\n", __func__, __LINE__);fflush(stdout);
    }

    //decrypt
    memset_(replain, 0, sizeof(replain));
    ret = ske_sec_crypto(SKE_ALG_AES_128, SKE_MODE_ECB, SKE_CRYPTO_DECRYPT, std_key, 0, NULL, SKE_NO_PADDING, 
            cipher, replain, 48, &out_bytes);
    if(SKE_SUCCESS != ret || (48 != out_bytes) || memcmp_(std_in, replain, out_bytes))
    {
        printf("%s line-%u AES128 ecb decryption(no padding, one-off style) failure, ret=%x\r\n", __func__, __LINE__, ret);fflush(stdout);
        return 1;
    }
    else
    {
        printf("%s line-%u AES128 ecb decryption(no padding, one-off style) success\r\n", __func__, __LINE__);fflush(stdout);
    }

    /************** CPU style(no padding, stepwise style) ***************/
    //the following encrypt & decrypt are equivalent to the above
    //encrypt
    memset_(cipher, 0, sizeof(cipher));
    ret  = ske_sec_init(ctx, SKE_ALG_AES_128, SKE_MODE_ECB, SKE_CRYPTO_ENCRYPT, std_key, 0, NULL, SKE_NO_PADDING);
    ret |= ske_sec_update_blocks(ctx, std_in, cipher, 16);
    ret |= ske_sec_update_blocks(ctx, std_in+16, cipher+16, 32);
    if(SKE_SUCCESS != ret || memcmp_(std_cipher, cipher, 48))
    {
        printf("%s line-%u AES128 ecb encryption(no padding, stepwise style) failure, ret=%x\r\n", __func__, __LINE__, ret);fflush(stdout);
        return 1;
    }
    else
    {
        printf("%s line-%u AES128 ecb encryption(no padding, stepwise style) success\r\n", __func__, __LINE__);fflush(stdout);
    }

    //decrypt
    memset_(replain, 0, sizeof(replain));
    ret  = ske_sec_init(ctx, SKE_ALG_AES_128, SKE_MODE_ECB, SKE_CRYPTO_DECRYPT, std_key, 0, NULL, SKE_NO_PADDING);
    ret |= ske_sec_update_blocks(ctx, std_cipher, replain, 16);
    ret |= ske_sec_update_blocks(ctx, std_cipher+16, replain+16, 32);
    if(SKE_SUCCESS != ret || memcmp_(std_in, replain, 48))
    {
        printf("%s line-%u AES128 ecb decryption(no padding, stepwise style) failure, ret=%x\r\n", __func__, __LINE__, ret);fflush(stdout);
        return 1;
    }
    else
    {
        printf("%s line-%u AES128 ecb decryption(no padding, stepwise style) success\r\n", __func__, __LINE__);fflush(stdout);
    }

    /************** CPU style(ANSI_X9.23 padding, one-off style) ***************/
    //ANSI_X9.23 padding, plaintext 48 bytes, ciphertext 64 bytes.
    //encrypt
    memset_(cipher, 0, sizeof(cipher));
    ret = ske_sec_crypto(SKE_ALG_AES_128, SKE_MODE_ECB, SKE_CRYPTO_ENCRYPT, std_key, 0, NULL, SKE_ANSI_X923_PADDING, 
            std_in, cipher, 48, &out_bytes);
    if(SKE_SUCCESS != ret || (64 != out_bytes) || memcmp_(std_cipher_48_ansi_x923, cipher, out_bytes))
    {
        printf("%s line-%u AES128 ecb encryption(ANSI_X9.23 padding, one-off style) failure, ret=%x\r\n", __func__, __LINE__, ret);fflush(stdout);
        print_buf_U8(std_key, 16, "std_key");
        print_buf_U8(std_in, 48, "std_in");
        print_buf_U8(std_cipher_48_ansi_x923, 64, "std_cipher");
        print_buf_U8(cipher, out_bytes, "cipher");
        return 1;
    }
    else
    {
        printf("%s line-%u AES128 ecb encryption(ANSI_X9.23 padding, one-off style) success\r\n", __func__, __LINE__);fflush(stdout);
    }

    //decrypt
    memset_(replain, 0, sizeof(replain));
    ret = ske_sec_crypto(SKE_ALG_AES_128, SKE_MODE_ECB, SKE_CRYPTO_DECRYPT, std_key, 0, NULL, SKE_ANSI_X923_PADDING, 
            cipher, replain, 64, &out_bytes);
    if(SKE_SUCCESS != ret || (48 != out_bytes) || memcmp_(std_in, replain, out_bytes))
    {
        printf("%s line-%u AES128 ecb decryption(ANSI_X9.23 padding, one-off style) failure, ret=%x\r\n", __func__, __LINE__, ret);fflush(stdout);
        print_buf_U8(replain, out_bytes, "replain");
        return 1;
    }
    else
    {
        printf("%s line-%u AES128 ecb decryption(ANSI_X9.23 padding, one-off style) success\r\n", __func__, __LINE__);fflush(stdout);
    }

    /************** CPU style(ANSI_X9.23 padding, stepwise style) ***************/
    //ANSI_X9.23 padding, plaintext 48 bytes, ciphertext 64 bytes.
    //encrypt
    memset_(cipher, 0, sizeof(cipher));
    ret = ske_sec_init(ctx, SKE_ALG_AES_128, SKE_MODE_ECB, SKE_CRYPTO_ENCRYPT, std_key, 0, NULL, SKE_ANSI_X923_PADDING);
    ret |= ske_sec_update_blocks(ctx, std_in, cipher, 16);
    ret |= ske_sec_update_blocks(ctx, std_in+16, cipher+16, 16);
    ret |= ske_sec_update_including_last_block(ctx, std_in+32, cipher+32, 16, &out_bytes);
    if(SKE_SUCCESS != ret || (32 != out_bytes) || memcmp_(std_cipher_48_ansi_x923, cipher, 32+out_bytes))
    {
        printf("%s line-%u AES128 ecb encryption(ANSI_X9.23 padding, stepwise style) failure, ret=%x\r\n", __func__, __LINE__, ret);fflush(stdout);
        print_buf_U8(std_key, 16, "std_key");
        print_buf_U8(std_in, 48, "std_in");
        print_buf_U8(std_cipher_48_ansi_x923, 64, "std_cipher");
        print_buf_U8(cipher, out_bytes, "cipher");
        return 1;
    }
    else
    {
        printf("%s line-%u AES128 ecb encryption(ANSI_X9.23 padding, stepwise style) success\r\n", __func__, __LINE__);fflush(stdout);
    }

    //decrypt
    memset_(replain, 0, sizeof(replain));
    ret = ske_sec_init(ctx, SKE_ALG_AES_128, SKE_MODE_ECB, SKE_CRYPTO_DECRYPT, std_key, 0, NULL, SKE_ANSI_X923_PADDING);
    ret |= ske_sec_update_blocks(ctx, cipher, replain, 16);
    ret |= ske_sec_update_blocks(ctx, cipher+16, replain+16, 32);
    ret |= ske_sec_update_including_last_block(ctx, cipher+48, replain+48, 16, &out_bytes);
    if(SKE_SUCCESS != ret || (0 != out_bytes) || memcmp_(std_in, replain, 48+out_bytes))
    {
        printf("%s line-%u AES128 ecb decryption(ANSI_X9.23 padding, stepwise style) failure, ret=%x\r\n", __func__, __LINE__, ret);fflush(stdout);
        print_buf_U8(replain, out_bytes, "replain");
        return 1;
    }
    else
    {
        printf("%s line-%u AES128 ecb decryption(ANSI_X9.23 padding, stepwise style) success\r\n", __func__, __LINE__);fflush(stdout);
    }

#ifdef SKE_DMA_FUNCTION
    std_in_          = (u32 *)SKE_DMA_RAM_BASE;
    std_out_         = std_in_+64;
    std_out_padding_ = std_out_+64;

    out  = std_out_padding_+64;
    out2 = out+64;

    uint32_copy(std_in_, (u32 *)std_in, 48/4);
    uint32_copy(std_out_, (u32 *)std_cipher, 48/4);
    uint32_copy(std_out_padding_, (u32 *)std_cipher_48_ansi_x923, 64/4);

    /************** DMA style(bypass mode) ***************/
#ifdef SUPPORT_SKE_MODE_BYPASS
    ret = ske_sec_dma_init(ctx, SKE_ALG_AES_128, SKE_MODE_BYPASS, SKE_CRYPTO_ENCRYPT, std_key, 
        0, NULL, SKE_NO_PADDING);
    if(SKE_SUCCESS != ret)
    {
        printf("%s line-%u dma bypass init failure, ret=%x\r\n", __func__, __LINE__, ret);fflush(stdout);
        return 1;
    }

#ifdef CONFIG_SKE_SUPPORT_ARCH64_REMAP
    ret = ske_sec_dma_update_blocks(ctx, addr_h_0, (u32)std_in_, addr_h_0, (u32)out, 16, ske_call_manage);
    ret |= ske_sec_dma_update_blocks(ctx, addr_h_0, ((u32)std_in_)+16, addr_h_0, ((u32)out)+16, 32, ske_call_manage);
#else
    ret = ske_sec_dma_update_blocks(ctx, std_in_, out, 16, ske_call_manage);
    ret |= ske_sec_dma_update_blocks(ctx, std_in_+16/4, out+16/4, 32, ske_call_manage);
#endif
    if(SKE_SUCCESS != ret || uint32_BigNumCmp(out, 48/4, std_in_, 48/4))
    {
        printf("%s line-%u dma bypass failure, ret=%x\r\n", __func__, __LINE__, ret);fflush(stdout);
        print_buf_U32(out, 48/4, "out");
        return 1;
    }
    else
    {
        printf("%s line-%u dma bypass success\r\n", __func__, __LINE__);fflush(stdout);
    }
#endif

    /************** DMA style(no padding, one-off style) ***************/
    //ENCRYPT
#ifdef CONFIG_SKE_SUPPORT_ARCH64_REMAP
    ret = ske_sec_dma_crypto(SKE_ALG_AES_128, SKE_MODE_ECB, SKE_CRYPTO_ENCRYPT, std_key, 0, NULL, SKE_NO_PADDING,
            addr_h_0, (u32)std_in_, addr_h_0, (u32)out, 48, &out_bytes, ske_call_manage);
#else
    ret = ske_sec_dma_crypto(SKE_ALG_AES_128, SKE_MODE_ECB, SKE_CRYPTO_ENCRYPT, std_key, 0, NULL, SKE_NO_PADDING, 
            std_in_, out, 48, &out_bytes, ske_call_manage);
#endif
    if(SKE_SUCCESS != ret || (48 != out_bytes) || uint32_BigNumCmp(out, out_bytes/4, std_out_, out_bytes/4))
    {
        printf("%s line-%u AES128 ecb dma encryption(no padding, one-off style) failure, ret=%x\r\n", __func__, __LINE__, ret);fflush(stdout);
        print_buf_U32(out, 48/4, "out");
        return 1;
    }
    else
    {
        printf("%s line-%u AES128 ecb dma encryption(no padding, one-off style) success\r\n", __func__, __LINE__);fflush(stdout);
    }

    //DECRYPT
#ifdef CONFIG_SKE_SUPPORT_ARCH64_REMAP
    ret = ske_sec_dma_crypto(SKE_ALG_AES_128, SKE_MODE_ECB, SKE_CRYPTO_DECRYPT, std_key, 0, NULL, SKE_NO_PADDING,
            addr_h_0, (u32)out, addr_h_0, (u32)out2, 48, &out_bytes, ske_call_manage);
#else
    ret = ske_sec_dma_crypto(SKE_ALG_AES_128, SKE_MODE_ECB, SKE_CRYPTO_DECRYPT, std_key, 0, NULL, SKE_NO_PADDING, 
            out, out2, 48, &out_bytes, ske_call_manage);
#endif

    if(SKE_SUCCESS != ret || (48 != out_bytes) || uint32_BigNumCmp(out2, out_bytes/4, std_in_, out_bytes/4))
    {
        printf("%s line-%u AES128 ecb dma decryption(no padding, one-off style) failure, ret=%x\r\n", __func__, __LINE__, ret);fflush(stdout);
        print_buf_U32(out2, 48/4, "out2");
        return 1;
    }
    else
    {
        printf("%s line-%u AES128 ecb dma decryption(no padding, one-off style) success\r\n", __func__, __LINE__);fflush(stdout);
    }

    /************** DMA style(no padding, stepwise style) ***************/
    //the following encrypt & decrypt are equivalent to the above
    //encrypt
    uint32_clear(out, 48/4);
    ret  = ske_sec_dma_init(ctx, SKE_ALG_AES_128, SKE_MODE_ECB, SKE_CRYPTO_ENCRYPT, std_key, 0, NULL, SKE_NO_PADDING);
#ifdef CONFIG_SKE_SUPPORT_ARCH64_REMAP
    ret |= ske_sec_dma_update_blocks(ctx, addr_h_0, (u32)std_in_, addr_h_0, (u32)out, 16, ske_call_manage);
    ret |= ske_sec_dma_update_blocks(ctx, addr_h_0, (u32)std_in_+16, addr_h_0, (u32)out+16, 32, ske_call_manage);
#else
    ret |= ske_sec_dma_update_blocks(ctx, std_in_, out, 16, ske_call_manage);
    ret |= ske_sec_dma_update_blocks(ctx, std_in_+16/4, out+16/4, 32, ske_call_manage);
#endif
    if(SKE_SUCCESS != ret || uint32_BigNumCmp(out, 48/4, std_out_, 48/4))
    {
        printf("%s line-%u AES128 ecb dma encryption(no padding, stepwise style) failure, ret=%x\r\n", __func__, __LINE__, ret);fflush(stdout);
        return 1;
    }
    else
    {
        printf("%s line-%u AES128 ecb dma encryption(no padding, stepwise style) success\r\n", __func__, __LINE__);fflush(stdout);
    }

    //decrypt
    uint32_clear(out2, 48/4);
    ret  = ske_sec_dma_init(ctx, SKE_ALG_AES_128, SKE_MODE_ECB, SKE_CRYPTO_DECRYPT, std_key, 0, NULL, SKE_NO_PADDING);
#ifdef CONFIG_SKE_SUPPORT_ARCH64_REMAP
    ret |= ske_sec_dma_update_blocks(ctx, addr_h_0, (u32)out, addr_h_0, (u32)out2, 32, ske_call_manage);
    ret |= ske_sec_dma_update_blocks(ctx, addr_h_0, (u32)out+32, addr_h_0, (u32)out2+32, 16, ske_call_manage);
#else
    ret |= ske_sec_dma_update_blocks(ctx, out, out2, 32, ske_call_manage);
    ret |= ske_sec_dma_update_blocks(ctx, out+32/4, out2+32/4, 16, ske_call_manage);
#endif
    if(SKE_SUCCESS != ret || uint32_BigNumCmp(out2, 48/4, std_in_, 48/4))
    {
        printf("%s line-%u AES128 ecb dma encryption(no padding, stepwise style) failure, ret=%x\r\n", __func__, __LINE__, ret);fflush(stdout);
        return 1;
    }
    else
    {
        printf("%s line-%u AES128 ecb dma decryption(no padding, stepwise style) success\r\n", __func__, __LINE__);fflush(stdout);
    }

    /************** DMA style(ANSI_X9.23 padding, one-off style) ***************/
    //ENCRYPT
    uint32_clear(out, 64/4);
#ifdef CONFIG_SKE_SUPPORT_ARCH64_REMAP
    ret = ske_sec_dma_crypto(SKE_ALG_AES_128, SKE_MODE_ECB, SKE_CRYPTO_ENCRYPT, std_key, 0, NULL, SKE_ANSI_X923_PADDING, 
            addr_h_0, (u32)std_in_, addr_h_0, (u32)out, 48, &out_bytes, ske_call_manage);
#else
    ret = ske_sec_dma_crypto(SKE_ALG_AES_128, SKE_MODE_ECB, SKE_CRYPTO_ENCRYPT, std_key, 0, NULL, SKE_ANSI_X923_PADDING,
            std_in_, out, 48, &out_bytes, ske_call_manage);
#endif
    if(SKE_SUCCESS != ret || (64 != out_bytes) || uint32_BigNumCmp(out, out_bytes/4, std_out_padding_, out_bytes/4))
    {
        printf("%s line-%u AES128 ecb dma encryption(ANSI_X9.23 padding, one-off style) failure, ret=%x\r\n", __func__, __LINE__, ret);fflush(stdout);
        print_buf_U32(std_in_, 48/4, "std_in_");
        print_buf_U32(out, 64/4, "out");
        return 1;
    }
    else
    {
        printf("%s line-%u AES128 ecb dma encryption(ANSI_X9.23 padding, one-off style) success\r\n", __func__, __LINE__);fflush(stdout);
    }

    //DECRYPT
    uint32_clear(out2, 64/4);
#ifdef CONFIG_SKE_SUPPORT_ARCH64_REMAP
    ret = ske_sec_dma_crypto(SKE_ALG_AES_128, SKE_MODE_ECB, SKE_CRYPTO_DECRYPT, std_key, 0, NULL, SKE_ANSI_X923_PADDING, 
            addr_h_0, (u32)out, addr_h_0, (u32)out2, 64, &out_bytes, ske_call_manage);
#else
    ret = ske_sec_dma_crypto(SKE_ALG_AES_128, SKE_MODE_ECB, SKE_CRYPTO_DECRYPT, std_key, 0, NULL, SKE_ANSI_X923_PADDING,
            out, out2, 64, &out_bytes, ske_call_manage);
#endif
    if(SKE_SUCCESS != ret || (48 != out_bytes) || uint32_BigNumCmp(out2, out_bytes/4, std_in_, out_bytes/4))
    {
        printf("%s line-%u AES128 ecb dma decryption(ANSI_X9.23 padding, one-off style) failure, ret=%x\r\n", __func__, __LINE__, ret);fflush(stdout);
        print_buf_U32(out2, 48/4, "out2");
        return 1;
    }
    else
    {
        printf("%s line-%u AES128 ecb dma decryption(ANSI_X9.23 padding, one-off style) success\r\n", __func__, __LINE__);fflush(stdout);
    }

    /************** DMA style(ANSI_X9.23 padding, stepwise style) ***************/
    //ENCRYPT
    uint32_clear(out, 64/4);
    ret  = ske_sec_dma_init(ctx, SKE_ALG_AES_128, SKE_MODE_ECB, SKE_CRYPTO_ENCRYPT, std_key, 0, NULL, SKE_ANSI_X923_PADDING);
#ifdef CONFIG_SKE_SUPPORT_ARCH64_REMAP
    ret |= ske_sec_dma_update_blocks(ctx, addr_h_0, (u32)std_in_, addr_h_0, (u32)out, 16, ske_call_manage);
    ret |= ske_sec_dma_update_blocks(ctx, addr_h_0, (u32)std_in_+16, addr_h_0, (u32)out+16, 16, ske_call_manage);
    ret |= ske_sec_dma_update_including_last_block(ctx, addr_h_0, (u32)std_in_+32, addr_h_0, (u32)out+32, 16, &out_bytes, ske_call_manage);
#else
    ret |= ske_sec_dma_update_blocks(ctx, std_in_, out, 16, ske_call_manage);
    ret |= ske_sec_dma_update_blocks(ctx, std_in_+16/4, out+16/4, 16, ske_call_manage);
    ret |= ske_sec_dma_update_including_last_block(ctx, std_in_+32/4, out+32/4, 16, &out_bytes, ske_call_manage);
#endif
    if(SKE_SUCCESS != ret || (32 != out_bytes) || uint32_BigNumCmp(out, (32+out_bytes)/4, std_out_padding_, (32+out_bytes)/4))
    {
        printf("%s line-%u AES128 ecb dma encryption(ANSI_X9.23 padding, stepwise style) failure, ret=%x\r\n", __func__, __LINE__, ret);fflush(stdout);
        print_buf_U32(std_in_, 48/4, "std_in_");
        print_buf_U32(out, 64/4, "out");
        return 1;
    }
    else
    {
        printf("%s line-%u AES128 ecb dma encryption(ANSI_X9.23 padding, stepwise style) success\r\n", __func__, __LINE__);fflush(stdout);
    }

    //DECRYPT
    uint32_clear(out2, 64/4);
    ret  = ske_sec_dma_init(ctx, SKE_ALG_AES_128, SKE_MODE_ECB, SKE_CRYPTO_DECRYPT, std_key, 0, NULL, SKE_ANSI_X923_PADDING);
#ifdef CONFIG_SKE_SUPPORT_ARCH64_REMAP
    ret |= ske_sec_dma_update_blocks(ctx, addr_h_0, (u32)out, addr_h_0, (u32)out2, 16, ske_call_manage);
    ret |= ske_sec_dma_update_blocks(ctx, addr_h_0, (u32)out+16, addr_h_0, (u32)out2+16, 16, ske_call_manage);
    ret |= ske_sec_dma_update_blocks(ctx, addr_h_0, (u32)out+32, addr_h_0, (u32)out2+32, 16, ske_call_manage);
    ret |= ske_sec_dma_update_including_last_block(ctx, addr_h_0, (u32)out+48, addr_h_0, (u32)out2+48, 16, &out_bytes, ske_call_manage);
#else
    ret |= ske_sec_dma_update_blocks(ctx, out, out2, 16, ske_call_manage);
    ret |= ske_sec_dma_update_blocks(ctx, out+16/4, out2+16/4, 16, ske_call_manage);
    ret |= ske_sec_dma_update_blocks(ctx, out+32/4, out2+32/4, 16, ske_call_manage);
    ret |= ske_sec_dma_update_including_last_block(ctx, out+48/4, out2+48/4, 16, &out_bytes, ske_call_manage);
#endif

    if(SKE_SUCCESS != ret || (0 != out_bytes) || uint32_BigNumCmp(out2, 48/4, std_in_, 48/4))
    {
        printf("%s line-%u AES128 ecb dma decryption(ANSI_X9.23 padding, stepwise style) failure, ret=%x\r\n", __func__, __LINE__, ret);fflush(stdout);
        print_buf_U32(out2, 48/4, "out2");
        return 1;
    }
    else
    {
        printf("%s line-%u AES128 ecb dma decryption(ANSI_X9.23 padding, stepwise style) success\r\n", __func__, __LINE__);fflush(stdout);
    }

    fflush(stdout);

#endif

    return 0;
}
#endif

#if defined(AIC_SKE_MODE_CBC_SUPPORT)
u32 ske_sec_aes128_cbc_sample(void)
{
    u8 std_in[48]       = {0x81,0x70,0x99,0x44,0xE0,0xCB,0x2E,0x1D,0xB5,0xB0,0xA4,0x77,0xD1,0xA8,0x53,0x9B,
                                0x0A,0x87,0x86,0xE3,0x4E,0xAA,0xED,0x99,0x30,0x3E,0xA6,0x97,0x55,0x95,0xB2,0x45,
                                0x4D,0x5D,0x7F,0x91,0xEB,0xBD,0x4A,0xCD,0x72,0x6C,0x0E,0x0E,0x5E,0x3E,0xB5,0x5E,};
    u8 std_key[16]      = {0xE0,0x70,0x99,0xF1,0xBF,0xAF,0xFD,0x7F,0x24,0x0C,0xD7,0x90,0xCA,0x4F,0xE1,0x34,};
    u8 std_iv[16]       = {0xC7,0x2B,0x65,0x91,0xA0,0xD7,0xDE,0x8F,0x6B,0x40,0x72,0x33,0xAD,0x35,0x81,0xD6};
    u8 std_cipher[48]   = {0x2C,0x1E,0xD4,0x56,0x36,0x2E,0x00,0x85,0xA8,0x1D,0x8E,0x61,0x69,0xAD,0x38,0xB7,
                                0xB4,0x42,0x60,0xE1,0x56,0x8D,0x9E,0x85,0x0A,0x0C,0x95,0x37,0x44,0x02,0xDE,0x28,
                                0x24,0xD5,0x05,0x61,0x30,0x42,0x42,0x86,0x0A,0xE7,0x17,0x3D,0xDD,0x19,0xEC,0x5B,};
    u8 std_cipher_33_pkcs_5_7[48]   = {
                                0x2C,0x1E,0xD4,0x56,0x36,0x2E,0x00,0x85,0xA8,0x1D,0x8E,0x61,0x69,0xAD,0x38,0xB7,
                                0xB4,0x42,0x60,0xE1,0x56,0x8D,0x9E,0x85,0x0A,0x0C,0x95,0x37,0x44,0x02,0xDE,0x28,
                                0x92,0x85,0xF8,0xD5,0x22,0x3E,0x39,0xC0,0x73,0x0C,0x6B,0xA0,0x96,0x62,0x93,0x39,};
    u8 cipher[48], replain[48];
    u32 __attribute__((unused)) *std_in_, *std_out_, *std_out_padding_, *out, *out2;
    u32 out_bytes;
    u32 ret;
#ifdef CONFIG_SKE_SUPPORT_ARCH64_REMAP
    u32 addr_h_0 = 0;
#endif

    SKE_CTX ctx[1];

    printf("\r\n  =================== AES128 CBC sample test ==================== \r\n");

    /************** CPU style(no padding, one-off style) ***************/
    //encrypt
    memset_(cipher, 0, sizeof(cipher));
    ret = ske_sec_cbc_crypto(SKE_ALG_AES_128, SKE_CRYPTO_ENCRYPT, std_key, 0, std_iv, SKE_NO_PADDING, 
            std_in, cipher, 48, &out_bytes);
    if(SKE_SUCCESS != ret || (48 != out_bytes) || memcmp_(cipher, std_cipher, 48))
    {
        printf("%s line-%u AES128 cbc encryption(no padding, one-off style) failure, ret=%x\r\n", __func__, __LINE__, ret);fflush(stdout);
        return 1;
    }
    else
    {
        printf("%s line-%u AES128 cbc encryption(no padding, one-off style) success\r\n", __func__, __LINE__);fflush(stdout);
    }

    //decrypt
    memset_(replain, 0, sizeof(replain));
    ret = ske_sec_cbc_crypto(SKE_ALG_AES_128, SKE_CRYPTO_DECRYPT, std_key, 0, std_iv, SKE_NO_PADDING, 
            std_cipher, replain, 48, &out_bytes);
    if(SKE_SUCCESS != ret || (48 != out_bytes) || memcmp_(replain, std_in, 48))
    {
        printf("%s line-%u AES128 cbc decryption(no padding, one-off style) failure, ret=%x\r\n", __func__, __LINE__, ret);fflush(stdout);
        return 1;
    }
    else
    {
        printf("%s line-%u AES128 cbc decryption(no padding, one-off style) success\r\n", __func__, __LINE__);fflush(stdout);
    }

    /************** CPU style(no padding, stepwise style) ***************/
    //the following encrypt & decrypt are equivalent to the above
    //encrypt
    memset_(cipher, 0, sizeof(cipher));
    ret  = ske_sec_cbc_init(ctx, SKE_ALG_AES_128, SKE_CRYPTO_ENCRYPT, std_key, 0, std_iv, SKE_NO_PADDING);
    ret |= ske_sec_cbc_update_blocks(ctx, std_in, cipher, 16);
    ret |= ske_sec_cbc_update_blocks(ctx, std_in+16, cipher+16, 32);
    if(SKE_SUCCESS != ret ||memcmp_(cipher, std_cipher, 48))
    {
        printf("%s line-%u AES128 cbc encryption(no padding, stepwise style) failure, ret=%x\r\n", __func__, __LINE__, ret);fflush(stdout);
        return 1;
    }
    else
    {
        printf("%s line-%u AES128 cbc encryption(no padding, stepwise style) success\r\n", __func__, __LINE__);fflush(stdout);
    }

    //decrypt
    memset_(replain, 0, sizeof(replain));
    ret  = ske_sec_cbc_init(ctx, SKE_ALG_AES_128, SKE_CRYPTO_DECRYPT, std_key, 0, std_iv, SKE_NO_PADDING);
    ret |= ske_sec_cbc_update_blocks(ctx, std_cipher, replain, 16);
    ret |= ske_sec_cbc_update_blocks(ctx, std_cipher+16, replain+16, 32);
    if(SKE_SUCCESS != ret ||memcmp_(std_in, replain, 48))
    {
        printf("%s line-%u AES128 cbc decryption(no padding, stepwise style) failure, ret=%x\r\n", __func__, __LINE__, ret);fflush(stdout);
        return 1;
    }
    else
    {
        printf("%s line-%u AES128 cbc decryption(no padding, stepwise style) success\r\n", __func__, __LINE__);fflush(stdout);
    }

    /************** CPU style(PKCS_5_7 padding, one-off style) ***************/
    //plaintext 33 bytes, ciphertext 48 bytes.
    //encrypt
    memset_(cipher, 0, sizeof(cipher));
    ret = ske_sec_crypto(SKE_ALG_AES_128, SKE_MODE_CBC, SKE_CRYPTO_ENCRYPT, std_key, 0, std_iv, SKE_PKCS_5_7_PADDING, 
            std_in, cipher, 33, &out_bytes);
    if(SKE_SUCCESS != ret || (48 != out_bytes) || memcmp_(std_cipher_33_pkcs_5_7, cipher, out_bytes))
    {
        printf("%s line-%u AES128 cbc encryption(PKCS_5_7 padding, one-off style) failure, ret=%x\r\n", __func__, __LINE__, ret);fflush(stdout);
        print_buf_U8(std_key, 16, "std_key");
        print_buf_U8(std_in, 33, "std_in");
        print_buf_U8(std_cipher_33_pkcs_5_7, 48, "std_cipher");
        print_buf_U8(cipher, out_bytes, "cipher");
        return 1;
    }
    else
    {
        printf("%s line-%u AES128 cbc encryption(PKCS_5_7 padding, one-off style) success\r\n", __func__, __LINE__);fflush(stdout);
    }

    //decrypt
    memset_(replain, 0, sizeof(replain));
    ret = ske_sec_crypto(SKE_ALG_AES_128, SKE_MODE_CBC, SKE_CRYPTO_DECRYPT, std_key, 0, std_iv, SKE_PKCS_5_7_PADDING, 
            cipher, replain, 48, &out_bytes);
    if(SKE_SUCCESS != ret || (33 != out_bytes) || memcmp_(std_in, replain, out_bytes))
    {
        printf("%s line-%u AES128 cbc decryption(PKCS_5_7 padding, one-off style) failure, ret=%x\r\n", __func__, __LINE__, ret);fflush(stdout);
        print_buf_U8(replain, out_bytes, "replain");
        return 1;
    }
    else
    {
        printf("%s line-%u AES128 cbc decryption(PKCS_5_7 padding, one-off style) success\r\n", __func__, __LINE__);fflush(stdout);
    }

    /************** CPU style(PKCS_5_7 padding, stepwise  style) ***************/
    //plaintext 33 bytes, ciphertext 48 bytes.
    //encrypt
    memset_(cipher, 0, sizeof(cipher));
    ret  = ske_sec_cbc_init(ctx, SKE_ALG_AES_128, SKE_CRYPTO_ENCRYPT, std_key, 0, std_iv, SKE_PKCS_5_7_PADDING);
    ret |= ske_sec_cbc_update_blocks(ctx, std_in, cipher, 16);
    ret |= ske_sec_cbc_update_blocks(ctx, std_in+16, cipher+16, 16);
    ret |= ske_sec_cbc_update_including_last_block(ctx, std_in+32, cipher+32, 1, &out_bytes);
    if(SKE_SUCCESS != ret || (16 != out_bytes) || memcmp_(std_cipher_33_pkcs_5_7, cipher, 32+out_bytes))
    {
        printf("%s line-%u AES128 cbc encryption(PKCS_5_7 padding, stepwise style) failure, ret=%x\r\n", __func__, __LINE__, ret);fflush(stdout);
        print_buf_U8(std_key, 16, "std_key");
        print_buf_U8(std_in, 33, "std_in");
        print_buf_U8(std_cipher_33_pkcs_5_7, 48, "std_cipher");
        print_buf_U8(cipher, 32+out_bytes, "cipher");
        return 1;
    }
    else
    {
        printf("%s line-%u AES128 cbc encryption(PKCS_5_7 padding, stepwise style) success\r\n", __func__, __LINE__);fflush(stdout);
    }

    //decrypt
    memset_(replain, 0, sizeof(replain));
    ret  = ske_sec_cbc_init(ctx, SKE_ALG_AES_128, SKE_CRYPTO_DECRYPT, std_key, 0, std_iv, SKE_PKCS_5_7_PADDING);
    ret |= ske_sec_cbc_update_blocks(ctx, cipher, replain, 16);
    ret |= ske_sec_cbc_update_blocks(ctx, cipher+16, replain+16, 16);
    ret |= ske_sec_cbc_update_including_last_block(ctx, cipher+32, replain+32, 16, &out_bytes);
    if(SKE_SUCCESS != ret || (1 != out_bytes) || memcmp_(std_in, replain, 32+out_bytes))
    {
        printf("%s line-%u AES128 cbc decryption(PKCS_5_7 padding, stepwise style) failure, ret=%x\r\n", __func__, __LINE__, ret);fflush(stdout);
        print_buf_U8(replain, out_bytes, "replain");
        return 1;
    }
    else
    {
        printf("%s line-%u AES128 cbc decryption(PKCS_5_7 padding, stepwise style) success\r\n", __func__, __LINE__);fflush(stdout);
    }


#ifdef SKE_DMA_FUNCTION
    std_in_ = (u32 *)SKE_DMA_RAM_BASE;
    std_out_ = std_in_+64;
    std_out_padding_ = std_out_+64;
    out = std_out_padding_+64;
    out2 = out+64;

    uint32_copy(std_in_, (u32 *)std_in, 48/4);
    uint32_copy(std_out_, (u32 *)std_cipher, 48/4);
    uint32_copy(std_out_padding_, (u32 *)std_cipher_33_pkcs_5_7, 48/4);

    /************** DMA style(no padding, one-off style) ***************/
    //encrypt
    uint32_clear(out, 48/4);
#ifdef CONFIG_SKE_SUPPORT_ARCH64_REMAP
    ret = ske_sec_dma_cbc_crypto(SKE_ALG_AES_128, SKE_CRYPTO_ENCRYPT, std_key, 0, std_iv, SKE_NO_PADDING,
            addr_h_0, (u32)std_in_, addr_h_0, (u32)out, 48, &out_bytes, ske_call_manage);
#else
    ret = ske_sec_dma_cbc_crypto(SKE_ALG_AES_128, SKE_CRYPTO_ENCRYPT, std_key, 0, std_iv, SKE_NO_PADDING, 
            std_in_, out, 48, &out_bytes, ske_call_manage);
#endif
    if(SKE_SUCCESS != ret || (48 != out_bytes) || uint32_BigNumCmp(out, out_bytes/4, std_out_, out_bytes/4))
    {
        printf("%s line-%u AES128 cbc dma encryption(no padding, one-off style) failure, ret=%x\r\n", __func__, __LINE__, ret);fflush(stdout);
        return 1;
    }
    else
    {
        printf("%s line-%u AES128 cbc dma encryption(no padding, one-off style) success\r\n", __func__, __LINE__);fflush(stdout);
    }

    //decrypt
    uint32_clear(out2, 48/4);
#ifdef CONFIG_SKE_SUPPORT_ARCH64_REMAP
    ret = ske_sec_dma_cbc_crypto(SKE_ALG_AES_128, SKE_CRYPTO_DECRYPT, std_key, 0, std_iv, SKE_NO_PADDING, 
            addr_h_0, (u32)out, addr_h_0, (u32)out2, 48, &out_bytes, ske_call_manage);
#else
    ret = ske_sec_dma_cbc_crypto(SKE_ALG_AES_128, SKE_CRYPTO_DECRYPT, std_key, 0, std_iv, SKE_NO_PADDING,
            out, out2, 48, &out_bytes, ske_call_manage);
#endif
    if(SKE_SUCCESS != ret || (48 != out_bytes) || uint32_BigNumCmp(out2, out_bytes/4, std_in_, out_bytes/4))
    {
        printf("%s line-%u AES128 cbc dma decryption(no padding, one-off style) failure, ret=%x\r\n", __func__, __LINE__, ret);fflush(stdout);
        print_buf_U32(out, 48/4, "out");
        print_buf_U32(out2, 48/4, "out2");
        return 1;
    }
    else
    {
        printf("%s line-%u AES128 cbc dma decryption(no padding, one-off style) success\r\n", __func__, __LINE__);fflush(stdout);
    }

    /************** DMA style(no padding, stepwise style) ***************/
    //the following encrypt & decrypt are equivalent to the above
    //encrypt
    uint32_clear(out, 48/4);
    ret  = ske_sec_dma_cbc_init(ctx, SKE_ALG_AES_128, SKE_CRYPTO_ENCRYPT, std_key, 0, std_iv, SKE_NO_PADDING);
#ifdef CONFIG_SKE_SUPPORT_ARCH64_REMAP
    ret |= ske_sec_dma_cbc_update_blocks(ctx, addr_h_0, (u32)std_in_, addr_h_0, (u32)out, 16, ske_call_manage);
    ret |= ske_sec_dma_cbc_update_blocks(ctx, addr_h_0, (u32)std_in_+16, addr_h_0, (u32)out+16, 32, ske_call_manage);
#else
    ret |= ske_sec_dma_cbc_update_blocks(ctx, std_in_, out, 16, ske_call_manage);
    ret |= ske_sec_dma_cbc_update_blocks(ctx, std_in_+16/4, out+16/4, 32, ske_call_manage);
#endif
    if(SKE_SUCCESS != ret || uint32_BigNumCmp(out, 48/4, std_out_, 48/4))
    {
        printf("%s line-%u AES128 cbc dma encryption(no padding, stepwise style) failure, ret=%x\r\n", __func__, __LINE__, ret);fflush(stdout);
        print_buf_U32(std_out_, 48/4, "std_out_");
        print_buf_U32(out, 48/4, "out");
        return 1;
    }
    else
    {
        printf("%s line-%u AES128 cbc dma encryption(no padding, stepwise style) success\r\n", __func__, __LINE__);fflush(stdout);
    }

    //decrypt
    uint32_clear(out2, 48/4);
    ret  = ske_sec_dma_cbc_init(ctx, SKE_ALG_AES_128, SKE_CRYPTO_DECRYPT, std_key, 0, std_iv, SKE_NO_PADDING);
#ifdef CONFIG_SKE_SUPPORT_ARCH64_REMAP
    ret |= ske_sec_dma_cbc_update_blocks(ctx, addr_h_0, (u32)out, addr_h_0, (u32)out2, 32, ske_call_manage);
    ret |= ske_sec_dma_cbc_update_blocks(ctx, addr_h_0, (u32)out+32, addr_h_0, (u32)out2+32, 16, ske_call_manage);
#else
    ret |= ske_sec_dma_cbc_update_blocks(ctx, out, out2, 32, ske_call_manage);
    ret |= ske_sec_dma_cbc_update_blocks(ctx, out+32/4, out2+32/4, 16, ske_call_manage);
#endif
    if(SKE_SUCCESS != ret || uint32_BigNumCmp(out2, 48/4, std_in_, 48/4))
    {
        printf("%s line-%u AES128 cbc dma encryption(no padding, stepwise style) failure, ret=%x\r\n", __func__, __LINE__, ret);fflush(stdout);
        return 1;
    }
    else
    {
        printf("%s line-%u AES128 cbc dma decryption(no padding, stepwise style) success\r\n", __func__, __LINE__);fflush(stdout);
    }

    /************** DMA style(PKCS_5_7 padding, one-off style) ***************/
    //encrypt
    uint32_clear(out, 48/4);
#ifdef CONFIG_SKE_SUPPORT_ARCH64_REMAP
    ret = ske_sec_dma_cbc_crypto(SKE_ALG_AES_128, SKE_CRYPTO_ENCRYPT, std_key, 0, std_iv, SKE_PKCS_5_7_PADDING,
            addr_h_0, (u32)std_in_, addr_h_0, (u32)out, 33, &out_bytes, ske_call_manage);
#else
    ret = ske_sec_dma_cbc_crypto(SKE_ALG_AES_128, SKE_CRYPTO_ENCRYPT, std_key, 0, std_iv, SKE_PKCS_5_7_PADDING, 
            std_in_, out, 33, &out_bytes, ske_call_manage);
#endif
    if(SKE_SUCCESS != ret || (48 != out_bytes) || uint32_BigNumCmp(out, out_bytes/4, std_out_padding_, out_bytes/4))
    {
        printf("%s line-%u AES128 cbc dma encryption(PKCS_5_7 padding, one-off style) failure, ret=%x\r\n", __func__, __LINE__, ret);fflush(stdout);
        return 1;
    }
    else
    {
        printf("%s line-%u AES128 cbc dma encryption(PKCS_5_7 padding, one-off style) success\r\n", __func__, __LINE__);fflush(stdout);
    }

    //decrypt
    uint32_clear(out2, 48/4);
#ifdef CONFIG_SKE_SUPPORT_ARCH64_REMAP
    ret = ske_sec_dma_cbc_crypto(SKE_ALG_AES_128, SKE_CRYPTO_DECRYPT, std_key, 0, std_iv, SKE_PKCS_5_7_PADDING, 
            addr_h_0, (u32)out, addr_h_0, (u32)out2, 48, &out_bytes, ske_call_manage);
#else
    ret = ske_sec_dma_cbc_crypto(SKE_ALG_AES_128, SKE_CRYPTO_DECRYPT, std_key, 0, std_iv, SKE_PKCS_5_7_PADDING,
            out, out2, 48, &out_bytes, ske_call_manage);
#endif
    if(SKE_SUCCESS != ret || (33 != out_bytes) || memcmp_((u8 *)out2, (u8 *)std_in_, out_bytes))
    {
        printf("%s line-%u AES128 cbc dma decryption(PKCS_5_7 padding, one-off style) failure, ret=%x\r\n", __func__, __LINE__, ret);fflush(stdout);
        print_buf_U32(out, 48/4, "out");
        print_buf_U32(out2, 48/4, "out2");
        return 1;
    }
    else
    {
        printf("%s line-%u AES128 cbc dma decryption(PKCS_5_7 padding, one-off style) success\r\n", __func__, __LINE__);fflush(stdout);
    }

    /************** DMA style(PKCS_5_7 padding, stepwise style) ***************/
    //encrypt
    uint32_clear(out, 48/4);
    ret  = ske_sec_dma_cbc_init(ctx, SKE_ALG_AES_128, SKE_CRYPTO_ENCRYPT, std_key, 0, std_iv, SKE_PKCS_5_7_PADDING);
#ifdef CONFIG_SKE_SUPPORT_ARCH64_REMAP
    ret |= ske_sec_dma_cbc_update_blocks(ctx, addr_h_0, (u32)std_in_, addr_h_0, (u32)out, 16, ske_call_manage);
    ret |= ske_sec_dma_cbc_update_blocks(ctx, addr_h_0, (u32)std_in_+16, addr_h_0, (u32)out+16, 16, ske_call_manage);
    ret |= ske_sec_dma_cbc_update_including_last_block(ctx, addr_h_0, (u32)std_in_+32, addr_h_0, (u32)out+32, 1, &out_bytes, ske_call_manage);
#else
    ret |= ske_sec_dma_cbc_update_blocks(ctx, std_in_, out, 16, ske_call_manage);
    ret |= ske_sec_dma_cbc_update_blocks(ctx, std_in_+16/4, out+16/4, 16, ske_call_manage);
    ret |= ske_sec_dma_cbc_update_including_last_block(ctx, std_in_+32/4, out+32/4, 1, &out_bytes, ske_call_manage);
#endif
    if(SKE_SUCCESS != ret || (16 != out_bytes) || uint32_BigNumCmp(out, (32+out_bytes)/4, std_out_padding_, (32+out_bytes)/4))
    {
        printf("%s line-%u AES128 cbc dma encryption(PKCS_5_7 padding, stepwise style) failure, ret=%x\r\n", __func__, __LINE__, ret);fflush(stdout);
        return 1;
    }
    else
    {
        printf("%s line-%u AES128 cbc dma encryption(PKCS_5_7 padding, stepwise style) success\r\n", __func__, __LINE__);fflush(stdout);
    }

    //decrypt
    uint32_clear(out2, 48/4);
    ret  = ske_sec_dma_cbc_init(ctx, SKE_ALG_AES_128, SKE_CRYPTO_DECRYPT, std_key, 0, std_iv, SKE_PKCS_5_7_PADDING);
#ifdef CONFIG_SKE_SUPPORT_ARCH64_REMAP
    ret |= ske_sec_dma_cbc_update_blocks(ctx, addr_h_0, (u32)out, addr_h_0, (u32)out2, 16, ske_call_manage);
    ret |= ske_sec_dma_cbc_update_blocks(ctx, addr_h_0, (u32)out+16, addr_h_0, (u32)out2+16, 16, ske_call_manage);
    ret |= ske_sec_dma_cbc_update_including_last_block(ctx, addr_h_0, (u32)out+32, addr_h_0, (u32)out2+32, 16, &out_bytes, ske_call_manage);
#else
    ret |= ske_sec_dma_cbc_update_blocks(ctx, out, out2, 16, ske_call_manage);
    ret |= ske_sec_dma_cbc_update_blocks(ctx, out+16/4, out2+16/4, 16, ske_call_manage);
    ret |= ske_sec_dma_cbc_update_including_last_block(ctx, out+32/4, out2+32/4, 16, &out_bytes, ske_call_manage);
#endif
    if(SKE_SUCCESS != ret || (1 != out_bytes) || memcmp_((u8 *)out2, (u8 *)std_in_, 32+out_bytes))
    {
        printf("%s line-%u AES128 cbc dma decryption(PKCS_5_7 padding, stepwise style) failure, ret=%x\r\n", __func__, __LINE__, ret);fflush(stdout);
        print_buf_U32(out, 48/4, "out");
        print_buf_U32(out2, 48/4, "out2");
        return 1;
    }
    else
    {
        printf("%s line-%u AES128 cbc dma decryption(PKCS_5_7 padding, stepwise style) success\r\n", __func__, __LINE__);fflush(stdout);
    }

    fflush(stdout);

#endif

    return 0;
}
#endif




#if defined(SUPPORT_SKE_MODE_GCM)
u32 ske_sec_aes128_gcm_sample(void)
{
    u8 std_plain[64]= {0xd9,0x31,0x32,0x25,0xf8,0x84,0x06,0xe5,0xa5,0x59,0x09,0xc5,0xaf,0xf5,0x26,0x9a,
                            0x86,0xa7,0xa9,0x53,0x15,0x34,0xf7,0xda,0x2e,0x4c,0x30,0x3d,0x8a,0x31,0x8a,0x72,
                            0x1c,0x3c,0x0c,0x95,0x95,0x68,0x09,0x53,0x2f,0xcf,0x0e,0x24,0x49,0xa6,0xb5,0x25,
                            0xb1,0x6a,0xed,0xf5,0xaa,0x0d,0xe6,0x57,0xba,0x63,0x7b,0x39,0x1a,0xaf,0xd2,0x55,};
    u8 std_key[16]  = {0xfe,0xff,0xe9,0x92,0x86,0x65,0x73,0x1c,0x6d,0x6a,0x8f,0x94,0x67,0x30,0x83,0x08};
    u8 std_iv[12]   = {0xca,0xfe,0xba,0xbe,0xfa,0xce,0xdb,0xad,0xde,0xca,0xf8,0x88};
    u8 std_aad[20]  = {0xfe,0xed,0xfa,0xce,0xde,0xad,0xbe,0xef,0xfe,0xed,0xfa,0xce,0xde,0xad,0xbe,0xef,
                            0xab,0xad,0xda,0xd2};
    u8 std_cipher[] = {0x42,0x83,0x1e,0xc2,0x21,0x77,0x74,0x24,0x4b,0x72,0x21,0xb7,0x84,0xd0,0xd4,0x9c,
                            0xe3,0xaa,0x21,0x2f,0x2c,0x02,0xa4,0xe0,0x35,0xc1,0x7e,0x23,0x29,0xac,0xa1,0x2e,
                            0x21,0xd5,0x14,0xb2,0x54,0x66,0x93,0x1c,0x7d,0x8f,0x6a,0x5a,0xac,0x84,0xaa,0x05,
                            0x1b,0xa3,0x0b,0x39,0x6a,0x0a,0xac,0x97,0x3d,0x58,0xe0,0x91,0x47,0x3f,0x59,0x85};
    u8 std_mac[]    = {0xDA,0x80,0xCE,0x83,0x0C,0xFD,0xA0,0x2D,0xA2,0xA2,0x18,0xA1,0x74,0x4F,0x4C,0x76};

    u8 cipher[64], replain[64];
    u8 mac[16];
    u32 *in, *out, *out2;
    u32 ret;
#ifdef CONFIG_SKE_SUPPORT_ARCH64_REMAP
    u32 addr_h_0 = 0;
#endif

    SKE_GCM_CTX ctx[1];

    printf("\r\n  =================== AES128 GCM sample test ==================== \r\n");

    /************** CPU style ***************/
    //encrypt
    ret = ske_sec_gcm_crypto(SKE_ALG_AES_128, SKE_CRYPTO_ENCRYPT, std_key, 0, std_iv, 12, std_aad, 20, std_plain, cipher, 64, mac, 16);
    if(SKE_SUCCESS != ret || memcmp_(cipher, std_cipher, 64) || memcmp_(mac, std_mac, 16))
    {
        printf("%s line-%u AES128 gcm encryption failure, ret=%x\r\n", __func__, __LINE__, ret);fflush(stdout);
        print_buf_U8(std_key, 16, "std_key");
        print_buf_U8(std_iv, 16, "std_iv");
        print_buf_U8(std_plain, 64, "std_plain");
        print_buf_U8(std_cipher, 64, "std_cipher");
        print_buf_U8(std_mac, 16, "std_mac");
        print_buf_U8(cipher, 64, "cipher");
        print_buf_U8(mac, 16, "mac");
        return 1;
    }
    else
    {
        printf("%s line-%u AES128 gcm encryption success\r\n", __func__, __LINE__);fflush(stdout);
    }

    //decrypt
    ret = ske_sec_gcm_crypto(SKE_ALG_AES_128, SKE_CRYPTO_DECRYPT, std_key, 0, std_iv, 12, std_aad, 20, cipher, replain, 64, mac, 16);
    if(SKE_SUCCESS != ret || memcmp_(replain, std_plain, 64))
    {
        printf("%s line-%u AES128 gcm decryption failure, ret=%x\r\n", __func__, __LINE__, ret);fflush(stdout);
        return 1;
    }
    else
    {
        printf("%s line-%u AES128 gcm decryption success\r\n", __func__, __LINE__);fflush(stdout);
    }

    //the following encrypt & decrypt are equivalent to the above
    //encrypt
    memset_(cipher, 0, sizeof(cipher));
    ret  = ske_sec_gcm_init(ctx, SKE_ALG_AES_128, SKE_CRYPTO_ENCRYPT, std_key, 0, std_iv, 12, 20, 64, 16);
#ifdef SKE_GCM_CPU_UPDATE_AAD_BY_STEP
    ret |= ske_sec_gcm_update_blocks_aad(ctx, std_aad, 20);
#else
    ret |= ske_sec_gcm_update_blocks_aad(ctx, std_aad);
#endif
    ret |= ske_sec_gcm_update_blocks(ctx, std_plain, cipher, 32);
    ret |= ske_sec_gcm_update_blocks(ctx, std_plain+32, cipher+32, 32);
    ret |= ske_sec_gcm_final(ctx, mac);
    if(SKE_SUCCESS != ret || memcmp_(cipher, std_cipher, 64) || memcmp_(mac, std_mac, 16))
    {
        printf("%s line-%u AES128 gcm encryption failure, ret=%x\r\n", __func__, __LINE__, ret);fflush(stdout);
        return 1;
    }
    else
    {
        printf("%s line-%u AES128 gcm encryption success\r\n", __func__, __LINE__);fflush(stdout);
    }


    //decrypt
    memset_(replain, 0, sizeof(replain));
    ret  = ske_sec_gcm_init(ctx, SKE_ALG_AES_128, SKE_CRYPTO_DECRYPT, std_key, 0, std_iv, 12, 20, 64, 16);
#ifdef SKE_GCM_CPU_UPDATE_AAD_BY_STEP
    ret |= ske_sec_gcm_update_blocks_aad(ctx, std_aad, 20);
#else
    ret |= ske_sec_gcm_update_blocks_aad(ctx, std_aad);
#endif
    ret |= ske_sec_gcm_update_blocks(ctx, cipher, replain, 16);
    ret |= ske_sec_gcm_update_blocks(ctx, cipher+16, replain+16, 32);
    ret |= ske_sec_gcm_update_blocks(ctx, cipher+48, replain+48, 16);
    ret |= ske_sec_gcm_final(ctx, mac);
    if(SKE_SUCCESS != ret || memcmp_(replain, std_plain, 64))
    {
        printf("%s line-%u AES128 gcm decryption failure, ret=%x\r\n", __func__, __LINE__, ret);fflush(stdout);
        return 1;
    }
    else
    {
        printf("%s line-%u AES128 gcm decryption success\r\n", __func__, __LINE__);fflush(stdout);
    }


#ifdef SKE_DMA_FUNCTION
    /************** DMA style ***************/
    in   = (u32 *)SKE_DMA_RAM_BASE;
    out  = in + 0x100;
    out2 = out + 0x200;

    //copy aad
    uint32_copy(in, (u32 *)std_aad, 20/4);
    uint32_clear(in+5, 3);

    //copy plain
    uint32_copy(in+8, (u32 *)std_plain, 64/4);

    //encrypt
    uint32_clear(out, (64+16)/4);
#ifdef CONFIG_SKE_SUPPORT_ARCH64_REMAP
    ret = ske_sec_dma_gcm_crypto(SKE_ALG_AES_128, SKE_CRYPTO_ENCRYPT, std_key, 0, std_iv, 12, 20,
            addr_h_0, (u32)in, addr_h_0, (u32)out, 64, 16, ske_call_manage);
#else
    ret = ske_sec_dma_gcm_crypto(SKE_ALG_AES_128, SKE_CRYPTO_ENCRYPT, std_key, 0, std_iv, 12, 20,
            in, out, 64, 16, ske_call_manage);
#endif

    if(SKE_SUCCESS != ret || uint32_BigNumCmp(out, 64/4, (u32 *)std_cipher, 64/4) ||
            uint32_BigNumCmp(out+64/4, 4, (u32 *)std_mac, 4))
    {
        printf("%s line-%u AES128 gcm dma encryption failure, ret=%x\r\n", __func__, __LINE__, ret);fflush(stdout);
        print_buf_U32(out, (64+32)/4, "out");
        return 1;
    }
    else
    {
        printf("%s line-%u AES128 gcm dma encryption success\r\n", __func__, __LINE__);fflush(stdout);
    }


    //copy aad
    uint32_copy(in, (u32 *)std_aad, 20/4);
    uint32_clear(in+5, 3);

    //copy cipher
    uint32_copy(in+8, (u32 *)out, 64/4);

    //decrypt
    uint32_clear(out2, (64+16)/4);
#ifdef CONFIG_SKE_SUPPORT_ARCH64_REMAP
    ret = ske_sec_dma_gcm_crypto(SKE_ALG_AES_128, SKE_CRYPTO_DECRYPT, std_key, 0, std_iv, 12, 20,
            addr_h_0, (u32)in, addr_h_0, (u32)out2, 64, 16, ske_call_manage);
#else
    ret = ske_sec_dma_gcm_crypto(SKE_ALG_AES_128, SKE_CRYPTO_DECRYPT, std_key, 0, std_iv, 12, 20,
            in, out2, 64, 16, ske_call_manage);
#endif
    if(SKE_SUCCESS != ret || uint32_BigNumCmp(out2, 64/4, (u32 *)std_plain, 64/4) ||
            uint32_BigNumCmp(out2+64/4, 4, (u32 *)std_mac, 4))
    {
        printf("%s line-%u AES128 gcm dma decryption failure, ret=%x\r\n", __func__, __LINE__, ret);fflush(stdout);
        print_buf_U32(out2, 48/4, "out2");
        return 1;
    }
    else
    {
        printf("%s line-%u AES128 gcm dma decryption success\r\n", __func__, __LINE__);fflush(stdout);
    }
#endif

    return 0;
}

#endif

#endif
