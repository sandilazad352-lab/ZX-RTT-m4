#include <stdio.h>
#include <string.h>
#include <hash.h>
#include <utility.h>
#include <trng.h>

//#define HASH_SPEED_TEST_BY_TIMER

#ifdef HASH_SPEED_TEST_BY_TIMER
extern u32 startP(void);
extern u32 endP(u8 mode, u32 once_bytes, u32 round);
#endif

//get random value in [1,max_number-1]
u32 hash_get_rand_number(u32 max_number)
{
    static u32 i=34;

#if 1
    get_rand((u8 *)&i, 4);
#else
    //simple random number generator
    i *= i<<1;
    i -= i*3;
    i ^= 0x78345bc;
    i += 89 | i>>1;
#endif

    i = i%max_number;
    if(0 == i)
    {
        i=1;
    }

    return i;
}

//callback function
void hash_call_manage(void)
{
    //printf("call manage???\n");
    //return;
}

u32 hash_test(HASH_ALG hash_alg, u8 *std_digest1,u8 *std_digest2,u8 *std_digest3)
{
    u8 message[500];
    u8 digest[64];
    u32 iterator[HASH_ITERATOR_MAX_WORD_LEN];
    u32 block_bytes = hash_get_block_word_len(hash_alg)<<2;
    HASH_CTX hash_ctx[1];

    char *name[] = {"SM3","MD5","SHA256","SHA384","SHA512","SHA1","SHA224","SHA512_224","SHA512_256"};
    u8 outLen[] = {32,16,32,48,64,20,28,28,32};
    u32 msg_bytes;
    u32 i, j, total;

    u32 len[5][15]=     //sum of each line is 500 
    {
        {4,   91,  173, 23,  56, 79, 2,  11, 61, 0,  0,  0,  0,  0,  0},   //padding with 0 to avoid some bad compiler's error(memset not found)
        {84,  37,  68,  293, 13, 1,  4,  0,  0,  0,  0,  0,  0,  0,  0},
        {33,  20,  208, 3,   7,  67, 33, 88, 19, 22, 0,  0,  0,  0,  0},
        {59,  111, 29,  34,  27, 89, 9,  6,  79, 22, 35, 0,  0,  0,  0},
        {333, 2,   5,   39,  77, 16, 24, 4,  0,  0,  0,  0,  0,  0,  0},
    };

    u32 ret;

#ifdef AIC_HASH_NODE
    HASH_NODE hash_node[4];
#endif

    memcpy(message, (u8 *)"abc", 3);   //to avoid some bad compiler's error(memset not found)

    printf("\r\n -------------- %s test -------------- ", name[hash_alg]);


    /*************************** test1 ***************************/
    ret = hash_init(hash_ctx, hash_alg);
    ret |= hash_update(hash_ctx, message, 3);
    ret |= hash_final(hash_ctx, digest);

#if 0
    print_buf_U8(message, 3, "message");    
    print_buf_U8(digest, outLen[hash_alg], "digest");
#endif

    if(HASH_SUCCESS != ret || memcmp_(digest, std_digest1, outLen[hash_alg]))
    {
        printf("\r\n %s test 1 failure, ret=%x\r\n", name[hash_alg], ret);
        print_buf_U8(message, 3, "message");
        print_buf_U8(digest, outLen[hash_alg], "digest");
        return 1;
    }
    else
    {
        printf("\r\n %s test 1 success ", name[hash_alg]);
    }


    /*************************** test2 ***************************/
    for(i = 0; i < 500; i++)
    {
        message[i] = i;
    }
    msg_bytes = 500;

    //test2-1
#if 1
    ret = hash(hash_alg, message, msg_bytes, digest);
#else
    ret = hash_init(hash_ctx, hash_alg);
    ret |= hash_update(hash_ctx, message, msg_bytes);
    ret |= hash_final(hash_ctx, digest);
#endif
    if(HASH_SUCCESS != ret || memcmp_(digest, std_digest2, outLen[hash_alg]))
    {
        printf("\r\n %s test 2-%u failure, ret=%x \r\n", name[hash_alg], 1, ret);
        print_buf_U8(message, msg_bytes, "message");
        print_buf_U8(digest, outLen[hash_alg], "digest");    
        return 1;
    }
    else
    {
        printf("\r\n %s test 2-%u success ", name[hash_alg], 1);
    }

    //test2-2...2-6
    for(i = 0; i < 5; i++)
    {
        j = 0;
        total = 0;
        memset(digest, 0, sizeof(digest));

        ret = hash_init(hash_ctx, hash_alg);

        while(len[i][j])
        {
            ret |= hash_update(hash_ctx, message+total, len[i][j]);
            total += len[i][j];
            j++;
        }

        ret |= hash_final(hash_ctx, digest);
        if(HASH_SUCCESS != ret || memcmp_(digest, std_digest2, outLen[hash_alg]))
        {
            printf("\r\n %s test 2-%u failure, ret=%x \r\n", name[hash_alg], i+2, ret);
            print_buf_U8(digest, outLen[hash_alg], "digest");
            return 1;
        }
        else 
        {
            printf("\r\n %s test 2-%u success", name[hash_alg], i+2);
        }
    }

    //test2-7
    memset(digest, 0, sizeof(digest));
    //step 1. init, input one block and stop, get iterator and backup it.
    ret = hash_init(hash_ctx, hash_alg);
    ret |= hash_update(hash_ctx, message, block_bytes);
    if(HASH_SUCCESS != ret)
    {
        printf("\r\n %s test 2-7 init and update failure, ret=%x \r\n", name[hash_alg], ret);
        return 1;
    }
#ifdef AIC_HASH_MUL_THREAD
    memcpy((u8 *)iterator, (u8 *)hash_ctx->iterator, (hash_ctx->iterator_word_len)<<2);
#else
    hash_get_iterator((u8 *)iterator, hash_ctx->iterator_word_len);
#endif
    //step 2. release resource(clear ctx) and do some other things
    memset((u8 *)hash_ctx, 0, sizeof(HASH_CTX));
    //...
    //step 3. continue till end
    ret = hash_init_with_iv_and_updated_length(hash_ctx, hash_alg, iterator, 0, block_bytes);  //not hash_ctx->block_byte_len
    ret |= hash_update(hash_ctx, message + block_bytes, msg_bytes - block_bytes);
    ret |= hash_final(hash_ctx, digest);
    if(HASH_SUCCESS != ret || memcmp_(digest, std_digest2, outLen[hash_alg]))
    {
        printf("\r\n %s test 2-7 failure, ret=%x \r\n", name[hash_alg], ret);
        print_buf_U8(message, msg_bytes, "message");
        print_buf_U8(digest, outLen[hash_alg], "digest");
        return 1;
    }
    else
    {
        printf("\r\n %s test 2-7 success ", name[hash_alg]);
    }

    //test2-8...
    printf("\r\n");
    for(i=0; i<100; i++)
    {
        total = 0;
        memset(digest, 0, sizeof(digest));

        ret = hash_init(hash_ctx, hash_alg);

        while(total < msg_bytes)
        {
            j = hash_get_rand_number(msg_bytes);
            if(j%10 < 3)
            {
                j = j%10;
            }
            else if(j%10 < 5)
            {
                j = j%70;
            }
            else if(j%10 < 8)
            {
                j = j%128;
            }

            if(j > msg_bytes-total)
            {
                j = msg_bytes-total;
            }

            ret |= hash_update(hash_ctx, message+total, j);
            total += j;
        }

        ret |= hash_final(hash_ctx, digest);
        if(HASH_SUCCESS != ret || memcmp_(digest, std_digest2, outLen[hash_alg]))
        {
            printf("\r\n %s test 2-%u failure, ret=%x \r\n", name[hash_alg], i+8, ret);//
            print_buf_U8(digest, outLen[hash_alg], "digest");
            return 1;
        }
        else
        {
            //printf("\r\n %s test 2-%u success ", name[hash_alg], i+8);
            printf("2-%u ", i+8);//
        }
    }

#ifdef AIC_HASH_NODE
    //test 2-final
    memset(digest, 0, sizeof(digest));
    hash_node[0].msg_addr  = message;
    hash_node[0].msg_bytes = 10;
    hash_node[1].msg_addr  = hash_node[0].msg_addr + hash_node[0].msg_bytes;
    hash_node[1].msg_bytes = 190;
    hash_node[2].msg_addr  = hash_node[1].msg_addr + hash_node[1].msg_bytes;
    hash_node[2].msg_bytes = 1;
    hash_node[3].msg_addr  = hash_node[2].msg_addr + hash_node[2].msg_bytes;
    hash_node[3].msg_bytes = msg_bytes - 10 - 190 - 1;
    ret = hash_node_steps(hash_alg, hash_node, 4, digest);
    if(HASH_SUCCESS != ret || memcmp_(digest, std_digest2, outLen[hash_alg]))
    {
        printf("\r\n %s test 2-final(hash node style) failure, ret=%x \r\n", name[hash_alg], ret);
        print_buf_U8(message, msg_bytes, "message");
        print_buf_U8(digest, outLen[hash_alg], "digest");
        return 1;
    }
    else
    {
        printf("\r\n %s test 2-final(hash node style) success ", name[hash_alg]);
    }
#endif

    /*************************** test3 ***************************/
    for(i = 0; i < 500; i++)
    {
        message[i] = i;
    }

    ret = 0;
    for(i = 0; i <= 500; i++)
    {
        j = hash_get_rand_number(2);

        if(j)
        {
            ret |= hash_init(hash_ctx, hash_alg);
            ret |= hash_update(hash_ctx, message, i);
            ret |= hash_final(hash_ctx, message);
        }
        else
        {
            ret |= hash(hash_alg, message, i, message);
        }
    }

    if(HASH_SUCCESS != ret || memcmp_(message, std_digest3, outLen[hash_alg]))
    {
        printf("\r\n %s test 3 failure, ret=%x \r\n", name[hash_alg], ret);
        print_buf_U8(message, outLen[hash_alg], "digest");    
        return 1;
    }
    else
    {
        printf("\r\n %s test 3 success", name[hash_alg]);
    }

    printf("\r\n");

    return 0;
}


#if (defined(AIC_HASH_MUL_THREAD) && defined(AIC_HASH_SHA256_SUPPORT) && defined(SUPPORT_HASH_SHA512))
u32 SHA256_SHA512_cross_test(void)
{
    //u32 msg_byte_len = 500;
    u8 message[500];
    u8 digest1[32];
    u8 digest2[64];
    u8 std_sha256_digest[32] =  {
        0x6A,0x25,0x9D,0xA4,0xDA,0xCD,0xFB,0x0F,0x51,0x36,0x96,0x49,0xCB,0xF8,0x86,0x4D,
        0x8E,0x2D,0x67,0x54,0x62,0xC8,0x62,0x5A,0x70,0x33,0x4B,0xFC,0x2C,0x50,0xD1,0xAF,};
    u8 std_sha512_digest[64] = {
        0xF8,0xD1,0xC9,0x17,0xDA,0x2E,0x66,0xBE,0x92,0x02,0x2D,0x6A,0x49,0xEC,0xEE,0x0F,
        0x0D,0x89,0x7C,0x6D,0x97,0x92,0x7A,0xDC,0x65,0x4C,0x73,0xD7,0x35,0xDC,0xA4,0x20,
        0xDD,0x80,0x8B,0xE0,0x67,0x94,0xF5,0x14,0xD5,0xA8,0x8F,0xA2,0xE2,0x4C,0x0C,0x0F,
        0x08,0x17,0xC9,0x5A,0xD4,0x48,0x37,0x1B,0x4C,0xD6,0xA8,0xCB,0x6A,0xA6,0x56,0xB3,};

    HASH_CTX sha256_ctx[1];
    HASH_CTX sha512_ctx[1];

#ifdef AIC_HASH_DMA
    u32 *msg_buf = (u32 *)(AIC_HASH_DMA_RAM_BASE);
    u32 *digest1_buf = msg_buf+500;
    u32 *digest2_buf = msg_buf+600;

    HASH_DMA_CTX sha256_dma_ctx[1];
    HASH_DMA_CTX sha512_dma_ctx[1];

#ifdef AIC_HASH_ADDRESS_HIGH_LOW
    u32 addr_h_0 = 0;
#endif
#endif

    u32 i;
    u32 ret;

    printf("\r\n -------------- SHA256 & SHA512 cross test -------------- ");

    for(i = 0; i < 500; i++)
    {
        message[i] = i;
    }

    ret = hash_init(sha256_ctx, HASH_SHA256);
    ret |= hash_init(sha512_ctx, HASH_SHA512);

    ret |= hash_update(sha256_ctx, message, 25);

    ret |= hash_update(sha512_ctx, message, 220);

    ret |= hash_update(sha256_ctx, message+25, 251);

    ret |= hash_update(sha512_ctx, message+220, 41);

    ret |= hash_update(sha256_ctx, message+276, 115);

    ret |= hash_update(sha512_ctx, message+261, 137);

    ret |= hash_update(sha256_ctx, message+391, 109);

    ret |= hash_update(sha512_ctx, message+398, 38+64);

    ret |= hash_final(sha256_ctx, digest1);
    if(HASH_SUCCESS != ret || memcmp_(digest1, std_sha256_digest, 32))
    {
        printf("\r\n SHA256 test failure \r\n");
        print_buf_U8(message, 500, "msg");
        print_buf_U8(digest1, 32, "SHA256 digest");
        return 1;
    }
    else 
    {
        printf("\r\n SHA256 test success ");
    }

    ret |= hash_final(sha512_ctx, digest2);
    if(HASH_SUCCESS != ret || memcmp_(digest2, std_sha512_digest, 64))
    {
        printf("\r\n SHA512 test failure \r\n");
        print_buf_U8(message, 500, "msg");
        print_buf_U8(digest2, 64, "SHA512 digest");
        return 1;
    }
    else 
    {
        printf("\r\n SHA512 test success ");
    }

#ifdef AIC_HASH_DMA
    uint32_clear(digest1_buf, 32/4);
    uint32_clear(digest2_buf, 64/4);
    memcpy((u8 *)msg_buf, message, 500);

    //print_buf_U8(msg_buf, 500, "msg_buf");

    ret = hash_dma_init(sha256_dma_ctx, HASH_SHA256, hash_call_manage);
    ret |= hash_dma_init(sha512_dma_ctx, HASH_SHA512, hash_call_manage);

#ifdef AIC_HASH_ADDRESS_HIGH_LOW
    ret |= hash_dma_update_blocks(sha256_dma_ctx, addr_h_0, (u32)msg_buf, 64*1);

    ret |= hash_dma_update_blocks(sha512_dma_ctx, addr_h_0, (u32)msg_buf, 128*1);

    ret |= hash_dma_update_blocks(sha256_dma_ctx, addr_h_0, ((u32)msg_buf)+64*1, 64*2);

    ret |= hash_dma_update_blocks(sha512_dma_ctx, addr_h_0, ((u32)msg_buf)+128*1, 128*1);

    ret |= hash_dma_update_blocks(sha256_dma_ctx, addr_h_0, ((u32)msg_buf)+64*3, 64*4);

    ret |= hash_dma_update_blocks(sha512_dma_ctx, addr_h_0, ((u32)msg_buf)+128*2, 128*1);

    ret |= hash_dma_final(sha256_dma_ctx, addr_h_0, ((u32)msg_buf)+(64*7), 52, addr_h_0, (u32)digest1_buf);
#else
    ret |= hash_dma_update_blocks(sha256_dma_ctx, msg_buf, 64*1);

    ret |= hash_dma_update_blocks(sha512_dma_ctx, msg_buf, 128*1);

    ret |= hash_dma_update_blocks(sha256_dma_ctx, msg_buf+64/4*1, 64*2);

    ret |= hash_dma_update_blocks(sha512_dma_ctx, msg_buf+128/4*1, 128*1);

    ret |= hash_dma_update_blocks(sha256_dma_ctx, msg_buf+64/4*3, 64*4);

    ret |= hash_dma_update_blocks(sha512_dma_ctx, msg_buf+128/4*2, 128*1);

    ret |= hash_dma_final(sha256_dma_ctx, msg_buf+(64/4*7), 52, digest1_buf);
#endif
    if(HASH_SUCCESS != ret || memcmp_((u8 *)digest1_buf, std_sha256_digest, 32))
    {
        printf("\r\n SHA256 dma test failure \r\n");
        print_buf_U8(message, 500, "msg");
        print_buf_U8((u8 *)digest1_buf, 32, "SHA256 digest");
        print_buf_U8(std_sha256_digest, 32, "std digest");
        return 1;
    }
    else
    {
        printf("\r\n SHA256 dma test success ");
    }

#ifdef AIC_HASH_ADDRESS_HIGH_LOW
    ret = hash_dma_final(sha512_dma_ctx, addr_h_0, ((u32)msg_buf)+(128*3), 116, addr_h_0, (u32)digest2_buf);
#else
    ret = hash_dma_final(sha512_dma_ctx, msg_buf+(128/4*3), 116, digest2_buf);
#endif
    if(HASH_SUCCESS != ret || memcmp_((u8 *)digest2_buf, std_sha512_digest, 64))
    {
        printf("\r\n SHA512 dma test failure \r\n");
        print_buf_U8(message, 500, "msg");
        print_buf_U8((u8 *)digest2_buf, 64, "SHA512 digest");
        print_buf_U8(std_sha512_digest, 64, "std digest");
        return 1;
    }
    else
    {
        printf("\r\n SHA512 dma test success ");
    }
#endif

    return 0;
}
#endif


#define HASH_SPEED_TEST_ROUND             (40960)
#define HASH_SPEED_TEST_BYTE_LEN          (1024)
#define HASH_DMA_SPEED_TEST_ROUND         (5120)
#define HASH_DMA_SPEED_TEST_BYTE_LEN      (1024*16)    //16K

u32 hash_speed_test_internal(HASH_ALG hash_alg, u32 round, u32 once_bytes)
{
    char *name_alg[]={"SM3","MD5","SHA256","SHA384","SHA512","SHA1","SHA224","SHA512_224","SHA512_256",
            "SHA3_224","SHA3_256","SHA3_384","SHA3_512",};
    u8 message[8192];
    u8 digest[64];
    u32 i;
    u32 ret;
    HASH_CTX ctx[1];

    uint32_sleep(0xFFFFF, 1);

    printf("\r\n===================== HASH %s CPU begin =====================", name_alg[hash_alg]);

#ifdef HASH_SPEED_TEST_BY_TIMER
    startP();
#endif

    ret = hash_init(ctx, hash_alg);
    if(HASH_SUCCESS != ret)
    {
        printf("\r\n init error ret=%x", ret); 
        return 1;
    }

    for(i = 0; i < round; i++)
    {
        ret = hash_update(ctx, message, once_bytes);
        if(HASH_SUCCESS != ret)
        {
            printf("\r\n update error ret=%x", ret); 
            return 1;
        }
    }

    ret = hash_final(ctx, digest);
    if(HASH_SUCCESS != ret)
    {
        printf("\r\n final error ret=%x", ret); 
        return 1;
    }

#ifdef HASH_SPEED_TEST_BY_TIMER
    endP(0, once_bytes, round);
#else
    printf("\r\n finished");
#endif

    return 0;
}


u32 hash_speed_test()
{
    u32 bytes = HASH_SPEED_TEST_BYTE_LEN;
    u32 round = HASH_SPEED_TEST_ROUND;

#ifdef HASH_SPEED_TEST_BY_TIMER
    round = 1024;
#endif

#if 1
#ifdef AIC_HASH_SM3_SUPPORT
    hash_speed_test_internal(HASH_SM3, round, bytes);
#endif
#endif

#if 1
#ifdef SUPPORT_HASH_MD5
    hash_speed_test_internal(HASH_MD5, round, bytes);
#endif
#endif

#if 1
#ifdef AIC_HASH_SHA256_SUPPORT
    hash_speed_test_internal(HASH_SHA256, round, bytes);
#endif
#endif

#if 1
#ifdef SUPPORT_HASH_SHA384
    hash_speed_test_internal(HASH_SHA384, round, bytes);
#endif
#endif

#if 1
#ifdef SUPPORT_HASH_SHA512
    hash_speed_test_internal(HASH_SHA512, round, bytes);
#endif
#endif

#if 1
#ifdef AIC_HASH_SHA1_SUPPORT
    hash_speed_test_internal(HASH_SHA1, round, bytes);
#endif
#endif

#if 1
#ifdef AIC_HASH_SHA224_SUPPORT
    hash_speed_test_internal(HASH_SHA224, round, bytes);
#endif
#endif

#if 1
#ifdef SUPPORT_HASH_SHA512_224
    hash_speed_test_internal(HASH_SHA512_224, round, bytes);
#endif
#endif

#if 1
#ifdef SUPPORT_HASH_SHA512_256
    hash_speed_test_internal(HASH_SHA512_256, round, bytes);
#endif
#endif

#if 1
#ifdef SUPPORT_HASH_SHA3_224
    hash_speed_test_internal(HASH_SHA3_224, round, bytes);
#endif
#endif

#if 1
#ifdef SUPPORT_HASH_SHA3_256
    hash_speed_test_internal(HASH_SHA3_256, round, bytes);
#endif
#endif

#if 1
#ifdef SUPPORT_HASH_SHA3_384
    hash_speed_test_internal(HASH_SHA3_384, round, bytes);
#endif
#endif

#if 1
#ifdef SUPPORT_HASH_SHA3_512
    hash_speed_test_internal(HASH_SHA3_512, round, bytes);
#endif
#endif

#if 1
#ifdef AIC_HASH_SHA256_SUPPORT
    round = HASH_SPEED_TEST_ROUND;
#ifdef HASH_SPEED_TEST_BY_TIMER
    round = 1024;
#endif
    hash_speed_test_internal(HASH_SHA256, round*32, 32);
    hash_speed_test_internal(HASH_SHA256, round*16, 64);
    hash_speed_test_internal(HASH_SHA256, round*8, 128);
    hash_speed_test_internal(HASH_SHA256, round*4, 256);
    hash_speed_test_internal(HASH_SHA256, round*2, 512);
    hash_speed_test_internal(HASH_SHA256, round,   1024);
    hash_speed_test_internal(HASH_SHA256, round/4, 4096);
    hash_speed_test_internal(HASH_SHA256, round/8, 8192);
#endif
#endif

    return 0;
}


#ifdef AIC_HASH_DMA
u32 hash_dma_speed_test_internal(HASH_ALG hash_alg, u32 round, u32 once_bytes)
{
    char *name_alg[]={"SM3","MD5","SHA256","SHA384","SHA512","SHA1","SHA224","SHA512_224","SHA512_256",
            "SHA3_224","SHA3_256","SHA3_384","SHA3_512",};
    u32 i, block_byte_len;
    u32 *msg=(u32 *)(AIC_HASH_DMA_RAM_BASE);
    u32 *digest = msg+512;
    u32 ret;
    HASH_DMA_CTX ctx[1];
#ifdef AIC_HASH_ADDRESS_HIGH_LOW
    u32 addr_h_0 = 0;
#endif

    uint32_sleep(0xFFFFF, 1);

    //this is for SHA3, since its block byte length is 72/104/136/144
    block_byte_len = hash_get_block_word_len(hash_alg)*4;
    once_bytes = (once_bytes/block_byte_len)*block_byte_len;

    printf("\r\n===================== HASH %s DMA begin =====================", name_alg[hash_alg]);

#ifdef HASH_SPEED_TEST_BY_TIMER
    startP();
#endif

    ret = hash_dma_init(ctx, hash_alg, hash_call_manage);
    if(HASH_SUCCESS != ret)
    {
        printf("\r\n dma init error ret=%x", ret); 
        return 1;
    }

    for(i = 0; i < round; i++)
    {
#ifdef AIC_HASH_ADDRESS_HIGH_LOW
        ret = hash_dma_update_blocks(ctx, addr_h_0, (u32)msg, once_bytes);
#else
        ret = hash_dma_update_blocks(ctx, msg, once_bytes);
#endif
        if(HASH_SUCCESS != ret)
        {
            printf("\r\n dma update error ret=%x", ret); 
            return 1;
        }
    }

//        for(i=0;i<100000;i++)
//        {
//            ret = hash_dma_update_blocks(ctx, msg, 3072/4);
//        }

#ifdef AIC_HASH_ADDRESS_HIGH_LOW
    ret = hash_dma_final(ctx, addr_h_0, (u32)msg, 0, addr_h_0, (u32)digest);
#else
    ret = hash_dma_final(ctx, msg, 0, digest);
#endif
    if(HASH_SUCCESS != ret)
    {
        printf("\r\n dma final error ret=%x", ret); 
        return 1;
    }

#ifdef HASH_SPEED_TEST_BY_TIMER
    endP(0, once_bytes, round);
#else
    printf("\r\n finished");
#endif

    return 0;
}


u32 hash_dma_speed_test()
{
    u32 round = HASH_DMA_SPEED_TEST_ROUND;
    u32 bytes = HASH_DMA_SPEED_TEST_BYTE_LEN;

#ifdef HASH_SPEED_TEST_BY_TIMER
    round = 512;
#endif

#if 1
#ifdef AIC_HASH_SM3_SUPPORT
    hash_dma_speed_test_internal(HASH_SM3, round, bytes);
#endif
#endif

#if 1
#ifdef SUPPORT_HASH_MD5
    hash_dma_speed_test_internal(HASH_MD5, round, bytes);
#endif
#endif

#if 1
#ifdef AIC_HASH_SHA256_SUPPORT
    hash_dma_speed_test_internal(HASH_SHA256, round, bytes);
#endif
#endif

#if 1
#ifdef SUPPORT_HASH_SHA384
    hash_dma_speed_test_internal(HASH_SHA384, round, bytes);
#endif
#endif

#if 1
#ifdef SUPPORT_HASH_SHA512
    hash_dma_speed_test_internal(HASH_SHA512, round, bytes);
#endif
#endif

#if 1
#ifdef AIC_HASH_SHA1_SUPPORT
    hash_dma_speed_test_internal(HASH_SHA1, round, bytes);
#endif
#endif

#if 1
#ifdef AIC_HASH_SHA224_SUPPORT
    hash_dma_speed_test_internal(HASH_SHA224, round, bytes);
#endif
#endif

#if 1
#ifdef SUPPORT_HASH_SHA512_224
    hash_dma_speed_test_internal(HASH_SHA512_224, round, bytes);
#endif
#endif

#if 1
#ifdef SUPPORT_HASH_SHA512_256
    hash_dma_speed_test_internal(HASH_SHA512_256, round, bytes);
#endif
#endif

#if 1
#ifdef SUPPORT_HASH_SHA3_224
    hash_dma_speed_test_internal(HASH_SHA3_224, round, bytes);
#endif
#endif

#if 1
#ifdef SUPPORT_HASH_SHA3_256
//    hash_dma_speed_test_internal(HASH_SHA3_256);
#endif
#endif

#if 1
#ifdef SUPPORT_HASH_SHA3_384
//    hash_dma_speed_test_internal(HASH_SHA3_384);
#endif
#endif

#if 1
#ifdef SUPPORT_HASH_SHA3_512
//    hash_dma_speed_test_internal(HASH_SHA3_512);
#endif
#endif

#if 1
#ifdef AIC_HASH_SHA256_SUPPORT
    round = HASH_DMA_SPEED_TEST_ROUND/2;
#ifdef HASH_SPEED_TEST_BY_TIMER
    round = 512;
#endif
    //hash_dma_speed_test_internal(HASH_SHA256, round*256, 32); //do not use this line, since block is 512bit.
    hash_dma_speed_test_internal(HASH_SHA256, round*128, 64);
    hash_dma_speed_test_internal(HASH_SHA256, round*64, 128);
    hash_dma_speed_test_internal(HASH_SHA256, round*32, 256);
    hash_dma_speed_test_internal(HASH_SHA256, round*16, 512);
    hash_dma_speed_test_internal(HASH_SHA256, round*8, 1024);
    hash_dma_speed_test_internal(HASH_SHA256, round*2, 4096);
    hash_dma_speed_test_internal(HASH_SHA256, round,   8192);
    hash_dma_speed_test_internal(HASH_SHA256, round/2,16384);
#endif
#endif

    return 0;
}


u32 hash_dma_test(HASH_ALG hash_alg, u8 *std_digest1, u8 *std_digest2, u8 *std_digest3)
{
    HASH_DMA_CTX ctx[1];
    u8 tmp_buf[500];
    u8 message[500];
    u32 iterator[HASH_ITERATOR_MAX_WORD_LEN];

    u32 block_word_len, block_byte_len, output_word_len, digest_byte_len, iterator_word_len;
    char *name[] = {"SM3","MD5","SHA_256","SHA_384","SHA_512","SHA_1","SHA_224","SHA_512_224","SHA_512_256"};
    u32 *msg = (u32 *)(AIC_HASH_DMA_RAM_BASE);
    u32 *digest = msg+512;

    u32 msg_bytes;
    u32 i,j,total;
    u32 ret;
#ifdef AIC_HASH_ADDRESS_HIGH_LOW
    u32 addr_h_0 = 0;
#endif

#ifdef AIC_HASH_DMA_NODE
    HASH_DMA_NODE hash_dma_node[4];
#endif

    memset(message, 0, sizeof(message));
    memcpy(message, (u8 *)"abc", 3);   //to avoid some bad compiler's error(memset not found)

    printf("\r\n ------------ %s DMA test ------------ ", name[hash_alg]);

    block_word_len    = hash_get_block_word_len(hash_alg);
    block_byte_len    = block_word_len<<2;
    digest_byte_len   = hash_get_digest_word_len(hash_alg)<<2;
    output_word_len   = ((digest_byte_len+15)/16)*4;  //must be a multiple of 4(namely 128bits)
    iterator_word_len = hash_get_iterator_word_len(hash_alg);


    //test1  message: 0x616263, 3 bytes.
    uint32_clear(msg, 512/4);
    uint32_copy(msg, (u32 *)message, 1);

    //the two styles are equivalent
#ifdef AIC_HASH_ADDRESS_HIGH_LOW
#if 1
    ret = hash_dma(hash_alg, addr_h_0, (u32)msg, 3, addr_h_0, (u32)digest, hash_call_manage);
#else
    ret = hash_dma_init(ctx, hash_alg, hash_call_manage);
    ret |= hash_dma_final(ctx, addr_h_0, (u32)msg, 3, addr_h_0, (u32)digest);
#endif
#else
#if 1
    ret = hash_dma(hash_alg, msg, 3, digest, hash_call_manage);
#else
    ret = hash_dma_init(ctx, hash_alg, hash_call_manage);
    ret |= hash_dma_final(ctx, msg, 3, digest);
#endif
#endif
    if(HASH_SUCCESS != ret || memcmp_(std_digest1, (u8 *)digest, digest_byte_len))
    {
        printf("\r\n %s DMA test 1 failure, ret=%x \r\n", name[hash_alg], ret);
        print_buf_U8(message, 3, "message");
        print_buf_U8((u8 *)digest, output_word_len<<2, "digest");
        print_buf_U8(std_digest1, digest_byte_len, "std digest");
        return 1;
    }
    else
    {
        printf("\r\n %s DMA test 1 success ", name[hash_alg]); //
    }


    //test2  message: 0x000102..., 500 bytes.
    msg_bytes = 500;
    for(i = 0; i < msg_bytes; i++)
    {
        message[i] = i;
    }

    uint32_clear(msg, 512/4);
    uint32_copy(msg, (u32 *)message, msg_bytes/4);

    //test 2-1
#ifdef AIC_HASH_ADDRESS_HIGH_LOW
    ret = hash_dma(hash_alg, addr_h_0, (u32)msg, msg_bytes, addr_h_0, (u32)digest, hash_call_manage);
#else
    ret = hash_dma(hash_alg, msg, msg_bytes, digest, hash_call_manage);
#endif
    if(HASH_SUCCESS != ret || memcmp_(std_digest2, (u8 *)digest, digest_byte_len))
    {
        printf("\r\n %s DMA test 2-1 failure, ret=%x \r\n", name[hash_alg], ret);//
        print_buf_U8(message, msg_bytes, "message");
        print_buf_U8((u8 *)digest, output_word_len<<2, "digest");
        print_buf_U8(std_digest2, digest_byte_len, "std digest");
        return 1;
    }
    else
    {
        printf("\r\n %s DMA test 2-1 success ", name[hash_alg]);//
    }

    //test 2-2
    uint32_clear(digest, 64/4);
    ret = hash_dma_init(ctx, hash_alg, hash_call_manage);
    for(i=0; i<msg_bytes/block_byte_len; i++)
#ifdef AIC_HASH_ADDRESS_HIGH_LOW
    {
        ret |= hash_dma_update_blocks(ctx, addr_h_0, ((u32)msg)+i*block_byte_len, block_byte_len);
    }
    ret |= hash_dma_final(ctx, addr_h_0, ((u32)msg)+i*block_byte_len, msg_bytes-i*block_byte_len, addr_h_0, (u32)digest);
#else
    {
        ret |= hash_dma_update_blocks(ctx, msg+i*block_word_len, block_byte_len);
    }
    ret |= hash_dma_final(ctx, msg+i*block_word_len, msg_bytes-i*block_byte_len, digest);
#endif
    if(HASH_SUCCESS != ret || memcmp_(std_digest2, (u8 *)digest, digest_byte_len))
    {
        printf("\r\n %s DMA test 2-2 failure, ret=%x \r\n", name[hash_alg], ret);//
        print_buf_U8(message, msg_bytes, "message");
        print_buf_U8((u8 *)digest, output_word_len<<2, "digest");
        print_buf_U8(std_digest2, digest_byte_len, "std digest");
        return 1;
    }
    else
    {
        printf("\r\n %s DMA test 2-2 success ", name[hash_alg]);//
    }

    //test 2-3
    uint32_clear(digest, 64/4);
    //step 1. init, input one block and stop, get iterator and backup it.
    ret = hash_dma_init(ctx, hash_alg, hash_call_manage);
#ifdef AIC_HASH_ADDRESS_HIGH_LOW
    ret |= hash_dma_update_blocks(ctx, addr_h_0, (u32)msg, 1*block_byte_len);
#else
    ret |= hash_dma_update_blocks(ctx, msg, 1*block_byte_len);
#endif
    if(HASH_SUCCESS != ret)
    {
        printf("\r\n %s DMA test 2-3 init failure, ret=%x \r\n", name[hash_alg], ret);//
        return 1;
    }
#ifdef AIC_HASH_MUL_THREAD
    memcpy((u8 *)iterator, (u8 *)ctx->iterator, iterator_word_len<<2);
#else
    hash_get_iterator((u8 *)iterator, iterator_word_len);
#endif
    //step 2. release resource(clear ctx) and do some other things
    memset((u8 *)ctx, 0, sizeof(HASH_DMA_CTX));
    //...
    //step 3. continue till end
    ret = hash_dma_init_with_iv_and_updated_length(ctx, hash_alg, iterator,
            0, block_byte_len, hash_call_manage);
#ifdef AIC_HASH_ADDRESS_HIGH_LOW
    ret |= hash_dma_update_blocks(ctx, addr_h_0, ((u32)msg)+block_byte_len, 1*block_byte_len);
    ret |= hash_dma_final(ctx, addr_h_0, ((u32)msg)+2*block_byte_len, msg_bytes-2*block_byte_len, addr_h_0, (u32)digest);
#else
    ret |= hash_dma_update_blocks(ctx, msg+block_word_len, 1*block_byte_len);
    ret |= hash_dma_final(ctx, msg+2*block_word_len, msg_bytes-2*block_byte_len, digest);
#endif
    if(HASH_SUCCESS != ret || memcmp_(std_digest2, (u8 *)digest, digest_byte_len))
    {
        printf("\r\n %s DMA test 2-3 failure, ret=%x \r\n", name[hash_alg], ret);//
        print_buf_U8(message, msg_bytes, "message");
        print_buf_U8((u8 *)digest, output_word_len<<2, "digest");
        print_buf_U8(std_digest1, digest_byte_len, "std digest");
        return 1;
    }
    else
    {
        printf("\r\n %s DMA test 2-3 success ", name[hash_alg]);//
    }

    //test 2-4...
    printf("\r\n");
    for(i=0; i<100; i++)
    {
        total = 0;
        uint32_clear(digest, 64/4);

        ret = hash_dma_init(ctx, hash_alg, hash_call_manage);

        while(total < msg_bytes)
        {
            j = hash_get_rand_number(500/block_byte_len);
            j = j*block_byte_len;
            if(j > msg_bytes-total)
                j = msg_bytes-total;

#ifdef AIC_HASH_ADDRESS_HIGH_LOW
            if(hash_get_rand_number(156) & 1)
            {
                if(0 == (j % block_byte_len))
                {
                    if(msg_bytes-total == j)
                        ret |= hash_dma_final(ctx, addr_h_0, ((u32)msg)+total, j, addr_h_0, (u32)digest);
                    else
                        ret |= hash_dma_update_blocks(ctx, addr_h_0, (u32)msg+total, j);
                }
                else
                {
                    j=msg_bytes-total;
                    ret |= hash_dma_final(ctx, addr_h_0, ((u32)msg)+total, j, addr_h_0, (u32)digest);
                }
            }
            else
            {
                j=msg_bytes-total;
                ret |= hash_dma_final(ctx, addr_h_0, ((u32)msg)+total, j, addr_h_0, (u32)digest);
            }
#else
            if(hash_get_rand_number(156) & 1)
            {
                if(0 == (j % block_byte_len))
                {
                    if(msg_bytes-total == j)
                        ret |= hash_dma_final(ctx, msg+total/4, j, digest);
                    else
                        ret |= hash_dma_update_blocks(ctx, msg+total/4, j);
                }
                else
                {
                    j=msg_bytes-total;
                    ret |= hash_dma_final(ctx, msg+total/4, j, digest);
                }
            }
            else
            {
                j=msg_bytes-total;
                ret |= hash_dma_final(ctx, msg+total/4, j, digest);
            }
#endif
            total += j;
        }
        if(HASH_SUCCESS != ret || memcmp_(std_digest2, (u8 *)digest, digest_byte_len))
        {
            printf("\r\n %s DMA test 2-%u failure, ret=%x \r\n", name[hash_alg], i+4, ret);//
            print_buf_U8(message, msg_bytes, "message");
            print_buf_U8((u8 *)digest, output_word_len<<2, "digest");
            print_buf_U8(std_digest1, digest_byte_len, "std digest");
            return 1;
        }
        else
        {
            printf(" 2-%u",i+4);//
        }
    }

#ifdef AIC_HASH_DMA_NODE
    //test 2-final
    uint32_clear(digest, 64/4);
#ifdef AIC_HASH_ADDRESS_HIGH_LOW
    hash_dma_node[0].msg_addr_h  = addr_h_0;
    hash_dma_node[0].msg_addr_l  = (u32)msg;
    hash_dma_node[0].msg_bytes   = block_byte_len;
    hash_dma_node[1].msg_addr_h  = addr_h_0;
    hash_dma_node[1].msg_addr_l  = hash_dma_node[0].msg_addr_l + hash_dma_node[0].msg_bytes;
    hash_dma_node[1].msg_bytes   = block_byte_len;
    hash_dma_node[2].msg_addr_h  = addr_h_0;
    hash_dma_node[2].msg_addr_l  = hash_dma_node[1].msg_addr_l + hash_dma_node[1].msg_bytes;
    hash_dma_node[2].msg_bytes   = msg_bytes - 2*block_byte_len;
    ret = hash_dma_node_steps(hash_alg, hash_dma_node, 3, addr_h_0, (u32)digest, hash_call_manage);
#else
    hash_dma_node[0].msg_addr  = msg;
    hash_dma_node[0].msg_bytes = block_byte_len;
    hash_dma_node[1].msg_addr  = hash_dma_node[0].msg_addr + hash_dma_node[0].msg_bytes/4;
    hash_dma_node[1].msg_bytes = block_byte_len;
    hash_dma_node[2].msg_addr  = hash_dma_node[1].msg_addr + hash_dma_node[1].msg_bytes/4;
    hash_dma_node[2].msg_bytes = msg_bytes - 2*block_byte_len;
    ret = hash_dma_node_steps(hash_alg, hash_dma_node, 3, digest, hash_call_manage);
#endif
    if(HASH_SUCCESS != ret || memcmp_(std_digest2, (u8 *)digest, digest_byte_len))
    {
        printf("\r\n %s DMA test 2-final(hash dma node style) failure, ret=%x \r\n", name[hash_alg], ret);//
        print_buf_U8(message, msg_bytes, "message");
        print_buf_U8((u8 *)digest, output_word_len<<2, "digest");
        print_buf_U8(std_digest1, digest_byte_len, "std digest");
        return 1;
    }
    else
    {
        printf("\r\n %s DMA test 2-final(hash dma node style)", name[hash_alg]);//
    }
#endif

    //test3  message: 0x000102..., 500 bytes.
    for(i = 0; i < 500; i++)
    {
        message[i] = i;
    }

    ret = 0;
    for(i = 0; i <= 500; i++)
    {
        memset(tmp_buf, 0, 500);
        memcpy(tmp_buf, message, i);

        uint32_clear(msg, 512/4);
        uint32_copy(msg, (u32 *)tmp_buf, (i+3)/4);

#ifdef AIC_HASH_ADDRESS_HIGH_LOW
        ret |= hash_dma(hash_alg, addr_h_0, (u32)msg, i, addr_h_0, (u32)digest, hash_call_manage);
#else
        ret |= hash_dma(hash_alg, msg, i, digest, hash_call_manage);
#endif

        uint32_copy((u32 *)message, digest, digest_byte_len/4);
    }

    if(HASH_SUCCESS != ret || memcmp_(std_digest3, (u8 *)digest, digest_byte_len))
    {
        printf("\r\n %s DMA test 3 failure, ret=%x \r\n", name[hash_alg], ret);//
        print_buf_U8(message, 500, "message");
        print_buf_U8((u8 *)digest, digest_byte_len, "digest");
        return 1;
    }
    else
    {
        printf("\r\n %s DMA test 3 success \r\n", name[hash_alg]);//
    }

    return 0;
}
#endif


#ifdef SUPPORT_HASH_MD5
u32 MD5_test(void)
{
    u8 std_digest1[32] =  {0x90,0x01,0x50,0x98,0x3C,0xD2,0x4F,0xB0,0xD6,0x96,0x3F,0x7D,0x28,0xE1,0x7F,0x72,};
    u8 std_digest2[32] =  {0xDE,0xC0,0x59,0x23,0x45,0xAC,0x3D,0xFA,0x17,0x2C,0x9E,0xC4,0x76,0xA2,0x4C,0x75,};
    u8 std_digest3[32] =  {0x1B,0x6B,0xF6,0x9F,0x9A,0x78,0x01,0x84,0xD8,0xF7,0x47,0x8C,0x27,0x96,0x22,0x77,};
    u32 ret;

    ret = hash_test(HASH_MD5, std_digest1, std_digest2, std_digest3);
    if(ret)
    {
        return 1;
    }

#ifdef AIC_HASH_DMA
    ret = hash_dma_test(HASH_MD5, std_digest1, std_digest2, std_digest3);
#endif

    return ret;
}
#endif


#ifdef AIC_HASH_SM3_SUPPORT
u32 SM3_test(void)
{
    u8 std_digest1[32] =  {0x66,0xC7,0xF0,0xF4,0x62,0xEE,0xED,0xD9,0xD1,0xF2,0xD4,0x6B,0xDC,0x10,0xE4,0xE2,
                                0x41,0x67,0xC4,0x87,0x5C,0xF2,0xF7,0xA2,0x29,0x7D,0xA0,0x2B,0x8F,0x4B,0xA8,0xE0};
    u8 std_digest2[32] =  {0xda,0xd1,0x46,0x5a,0x3f,0x51,0x12,0x8d,0x86,0xf8,0xda,0xaa,0x4f,0x4e,0x7e,0x1b,
                                0x3d,0x2e,0x70,0x4c,0x29,0x45,0x12,0x47,0xaf,0xd4,0x9d,0x7d,0x96,0xa5,0xac,0x31,};
    u8 std_digest3[32] =  {0xe8,0xb9,0x9c,0xdf,0x8c,0x46,0xa6,0x72,0xce,0xc9,0x9d,0x97,0x6e,0x31,0xa1,0x5c,
                                0x83,0x63,0x1f,0x57,0x47,0x05,0x29,0x5f,0xd3,0x0f,0x92,0x3a,0xe7,0xeb,0xbd,0x5e,};
    u32 ret;

    ret = hash_test(HASH_SM3, std_digest1, std_digest2, std_digest3);
    if(ret)
    {
        return 1;
    }

#ifdef AIC_HASH_DMA
    ret = hash_dma_test(HASH_SM3, std_digest1, std_digest2, std_digest3);
#endif

    return ret;
}
#endif


#ifdef AIC_HASH_SHA1_SUPPORT
u32 SHA1_test(void)
{
    u8 std_digest1[20] =  {0xA9,0x99,0x3E,0x36,0x47,0x06,0x81,0x6A,0xBA,0x3E,0x25,0x71,0x78,0x50,0xC2,0x6C,
                                0x9C,0xD0,0xD8,0x9D,};
    u8 std_digest2[20] =  {0x50,0x75,0x0F,0x6E,0xC4,0xBD,0xAA,0x13,0x43,0x69,0x29,0x9D,0xE5,0x3D,0x8D,0x8B,
                                0x87,0xD1,0xBA,0x63,};
    u8 std_digest3[20] =  {0x9F,0x14,0x31,0x91,0x13,0x17,0xD7,0x4D,0x2D,0xF1,0xBD,0xD2,0x52,0x82,0xA4,0xD8,
                                0x0E,0x66,0x42,0x7F,};
    u32 ret;

    ret = hash_test(HASH_SHA1, std_digest1, std_digest2, std_digest3);
    if(ret)
    {
        return 1;
    }

#ifdef AIC_HASH_DMA
    ret = hash_dma_test(HASH_SHA1, std_digest1, std_digest2, std_digest3);
#endif

    return ret;
}
#endif


#ifdef AIC_HASH_SHA256_SUPPORT
u32 SHA256_test(void)
{
    u8 std_digest1[32] =  {0xBA,0x78,0x16,0xBF,0x8F,0x01,0xCF,0xEA,0x41,0x41,0x40,0xDE,0x5D,0xAE,0x22,0x23,
                                0xB0,0x03,0x61,0xA3,0x96,0x17,0x7A,0x9C,0xB4,0x10,0xFF,0x61,0xF2,0x00,0x15,0xAD};
    u8 std_digest2[32] =  {0x6A,0x25,0x9D,0xA4,0xDA,0xCD,0xFB,0x0F,0x51,0x36,0x96,0x49,0xCB,0xF8,0x86,0x4D,
                                0x8E,0x2D,0x67,0x54,0x62,0xC8,0x62,0x5A,0x70,0x33,0x4B,0xFC,0x2C,0x50,0xD1,0xAF};
    u8 std_digest3[32] =  {0xC3,0xFB,0x71,0x00,0xFE,0x3F,0x3F,0x9F,0xAF,0x19,0xD2,0x8E,0xE4,0x4B,0x5A,0x87,
                                0xCF,0x6F,0xB6,0xD1,0x55,0x6C,0x5A,0x04,0x45,0xE5,0x84,0xF6,0xAE,0x9D,0xB8,0x6A};
    u32 ret;

    ret = hash_test(HASH_SHA256, std_digest1, std_digest2, std_digest3);
    if(ret)
    {
        return 1;
    }

#ifdef AIC_HASH_DMA
    ret = hash_dma_test(HASH_SHA256, std_digest1, std_digest2, std_digest3);
#endif

    return ret;
}
#endif


#ifdef AIC_HASH_SHA224_SUPPORT
u32 SHA224_test(void)
{
    u8 std_digest1[28] =  {0x23,0x09,0x7D,0x22,0x34,0x05,0xD8,0x22,0x86,0x42,0xA4,0x77,0xBD,0xA2,0x55,0xB3,
                                0x2A,0xAD,0xBC,0xE4,0xBD,0xA0,0xB3,0xF7,0xE3,0x6C,0x9D,0xA7};
    u8 std_digest2[28] =  {0xDE,0xFC,0x2F,0x15,0x71,0x32,0x50,0xE4,0xEF,0x6B,0xCC,0xEB,0x4F,0xAC,0x74,0x42,
                                0xDF,0x56,0x51,0xFC,0x17,0xA0,0xBC,0x56,0xA9,0xEF,0x67,0xF2};
    u8 std_digest3[28] =  {0xFE,0x56,0x7D,0xC1,0x15,0xA9,0xA1,0x62,0xA4,0xB7,0x92,0x1A,0x76,0x6E,0x4F,0x35,
                                0x67,0x65,0x74,0xE7,0xCD,0x00,0x4E,0x2E,0xEA,0xE2,0x73,0x99};
    u32 ret;

    ret = hash_test(HASH_SHA224, std_digest1, std_digest2, std_digest3);
    if(ret)
    {
        return 1;
    }

#ifdef AIC_HASH_DMA
    ret = hash_dma_test(HASH_SHA224, std_digest1, std_digest2, std_digest3);
#endif

    return ret;
}
#endif


#ifdef SUPPORT_HASH_SHA384
u32 SHA384_test(void)
{
    u8 std_digest1[48] =  {0xCB,0x00,0x75,0x3F,0x45,0xA3,0x5E,0x8B,0xB5,0xA0,0x3D,0x69,0x9A,0xC6,0x50,0x07,
                                0x27,0x2C,0x32,0xAB,0x0E,0xDE,0xD1,0x63,0x1A,0x8B,0x60,0x5A,0x43,0xFF,0x5B,0xED,
                                0x80,0x86,0x07,0x2B,0xA1,0xE7,0xCC,0x23,0x58,0xBA,0xEC,0xA1,0x34,0xC8,0x25,0xA7};
    u8 std_digest2[48] =  {0x32,0x1E,0x84,0x33,0xE8,0xE1,0xE4,0x61,0xBD,0xC5,0xB1,0x49,0x58,0x2F,0x4A,0x80,
                                0xB3,0xA8,0x8D,0x75,0x10,0x42,0x08,0xA2,0xC4,0xF8,0xA5,0x35,0xB4,0x5D,0xDB,0x27,
                                0x34,0xEB,0xAC,0x7C,0x89,0x96,0xC8,0x60,0x4F,0xF0,0x3A,0x5A,0xEE,0x3D,0xA5,0xCB};
    u8 std_digest3[48] =  {0xCC,0x2B,0x35,0xE0,0x00,0xE8,0x54,0x8B,0x17,0xAD,0x89,0xC1,0xA9,0x15,0xBB,0x36,
                                0x5A,0xE8,0xBF,0xFF,0x89,0xBA,0x12,0xD1,0x9B,0x5E,0x3F,0xE8,0x56,0xC5,0x1A,0xAE,
                                0xC0,0x45,0x9B,0x80,0x25,0xD8,0x99,0x96,0xE1,0x2F,0x44,0x8F,0xD0,0x2E,0x46,0xF9};
    u32 ret;

    ret = hash_test(HASH_SHA384, std_digest1, std_digest2, std_digest3);
    if(ret)
    {
        return 1;
    }

#ifdef AIC_HASH_DMA
    ret = hash_dma_test(HASH_SHA384, std_digest1, std_digest2, std_digest3);
#endif

    return ret;
}
#endif


#ifdef SUPPORT_HASH_SHA512
u32 SHA512_test(void)
{
    u8 std_digest1[64] =  {0xDD,0xAF,0x35,0xA1,0x93,0x61,0x7A,0xBA,0xCC,0x41,0x73,0x49,0xAE,0x20,0x41,0x31,
                                0x12,0xE6,0xFA,0x4E,0x89,0xA9,0x7E,0xA2,0x0A,0x9E,0xEE,0xE6,0x4B,0x55,0xD3,0x9A,
                                0x21,0x92,0x99,0x2A,0x27,0x4F,0xC1,0xA8,0x36,0xBA,0x3C,0x23,0xA3,0xFE,0xEB,0xBD,
                                0x45,0x4D,0x44,0x23,0x64,0x3C,0xE8,0x0E,0x2A,0x9A,0xC9,0x4F,0xA5,0x4C,0xA4,0x9F};
    u8 std_digest2[64] =  {0xF8,0xD1,0xC9,0x17,0xDA,0x2E,0x66,0xBE,0x92,0x02,0x2D,0x6A,0x49,0xEC,0xEE,0x0F,
                                0x0D,0x89,0x7C,0x6D,0x97,0x92,0x7A,0xDC,0x65,0x4C,0x73,0xD7,0x35,0xDC,0xA4,0x20,
                                0xDD,0x80,0x8B,0xE0,0x67,0x94,0xF5,0x14,0xD5,0xA8,0x8F,0xA2,0xE2,0x4C,0x0C,0x0F,
                                0x08,0x17,0xC9,0x5A,0xD4,0x48,0x37,0x1B,0x4C,0xD6,0xA8,0xCB,0x6A,0xA6,0x56,0xB3};
    u8 std_digest3[64] =  {0xB5,0xA7,0xB2,0x22,0x2E,0x91,0x6B,0x74,0xA7,0xDD,0x59,0x3A,0x07,0x8A,0x33,0xAC,
                                0xF5,0x3D,0xD9,0x55,0xF7,0x66,0x10,0x89,0x80,0x49,0xC3,0xB3,0x1D,0x16,0x50,0xBB,
                                0x5D,0x63,0x53,0xC0,0x35,0x70,0xFB,0x30,0xBC,0x1F,0xBA,0x0C,0x02,0xDF,0x4E,0xB9,
                                0x37,0x87,0xED,0x79,0x0B,0x1E,0x74,0x8A,0x3A,0xA6,0xA7,0x15,0xEC,0x41,0x02,0xFA};
    u32 ret;

    ret = hash_test(HASH_SHA512, std_digest1, std_digest2, std_digest3);
    if(ret)
    {
        return 1;
    }

#ifdef AIC_HASH_DMA
    ret = hash_dma_test(HASH_SHA512, std_digest1, std_digest2, std_digest3);
#endif

    return ret;
}
#endif


#ifdef SUPPORT_HASH_SHA512_224
u32 SHA512_224_test(void)
{
    u8 std_digest1[28] =  {0x46,0x34,0x27,0x0F,0x70,0x7B,0x6A,0x54,0xDA,0xAE,0x75,0x30,0x46,0x08,0x42,0xE2,
                                0x0E,0x37,0xED,0x26,0x5C,0xEE,0xE9,0xA4,0x3E,0x89,0x24,0xAA};
    u8 std_digest2[28] =  {0xEB,0x41,0x69,0x3D,0x78,0x4B,0x87,0x25,0xC1,0xFB,0x06,0xA6,0x92,0x2E,0x86,0x9B,
                                0x1C,0xF0,0x34,0x64,0x04,0x8C,0xCF,0x72,0x0F,0x62,0x34,0xFD};
    u8 std_digest3[28] =  {0x8A,0x48,0x1F,0xBD,0x48,0x94,0xD7,0x2C,0x63,0xA7,0x3A,0xF9,0x5C,0xB4,0x4F,0x34,
                                0x6E,0xAF,0x0E,0xF0,0x5E,0x25,0x17,0xBB,0x47,0x3A,0xBE,0xF4};
    u32 ret;

    ret = hash_test(HASH_SHA512_224, std_digest1, std_digest2, std_digest3);
    if(ret)
    {
        return 1;
    }

#ifdef AIC_HASH_DMA
    ret = hash_dma_test(HASH_SHA512_224, std_digest1, std_digest2, std_digest3);
#endif

    return ret;
}
#endif


#ifdef SUPPORT_HASH_SHA512_256
u32 SHA512_256_test(void)
{
    u8 std_digest1[32] =  {0x53,0x04,0x8E,0x26,0x81,0x94,0x1E,0xF9,0x9B,0x2E,0x29,0xB7,0x6B,0x4C,0x7D,0xAB,
                                0xE4,0xC2,0xD0,0xC6,0x34,0xFC,0x6D,0x46,0xE0,0xE2,0xF1,0x31,0x07,0xE7,0xAF,0x23};
    u8 std_digest2[32] =  {0x81,0xDC,0x4F,0x56,0x1A,0x2A,0x4C,0x29,0x6B,0xD3,0xAA,0x3F,0x67,0xC1,0x73,0x5F,
                                0x86,0xDA,0xBE,0xB6,0x2D,0xBB,0xCE,0xC3,0x90,0xBD,0x58,0x16,0xE5,0x78,0x10,0xEA};
    u8 std_digest3[32] =  {0xFE,0xB9,0x98,0x10,0xCC,0xE3,0xB3,0x8F,0x3E,0x12,0xC1,0xF6,0xA6,0x5B,0x5A,0x1C,
                                0xE4,0x9A,0x5E,0xBD,0xD7,0x35,0x70,0x9F,0x7F,0x4E,0xD4,0x16,0x34,0x25,0x4C,0x74};
    u32 ret;

    ret = hash_test(HASH_SHA512_256, std_digest1, std_digest2, std_digest3);
    if(ret)
    {
        return 1;
    }

#ifdef AIC_HASH_DMA
    ret = hash_dma_test(HASH_SHA512_256, std_digest1, std_digest2, std_digest3);
#endif

    return ret;
}
#endif


#ifdef SUPPORT_HASH_SHA3_224
u32 SHA3_224_test(void)
{
    u8 std_digest1[28] =  {0xE6,0x42,0x82,0x4C,0x3F,0x8C,0xF2,0x4A,0xD0,0x92,0x34,0xEE,0x7D,0x3C,0x76,0x6F,
                                0xC9,0xA3,0xA5,0x16,0x8D,0x0C,0x94,0xAD,0x73,0xB4,0x6F,0xDF};
    u8 std_digest2[28] =  {0xEB,0x7D,0x43,0xDF,0xD5,0x50,0xEE,0x5A,0xFF,0x61,0x59,0xB3,0xCC,0xAE,0xE0,0x71,
                                0x5C,0x9C,0x36,0xBB,0xCE,0x18,0x22,0xAD,0x7E,0x26,0x70,0x2E};
    u8 std_digest3[28] =  {0x12,0xa6,0x09,0xd1,0x1b,0xee,0xad,0x6b,0xb7,0xd7,0xda,0x31,0x83,0x08,0xad,0xd4,
                                0x87,0x5f,0x30,0xa5,0x87,0xb0,0x8e,0xee,0x53,0xb8,0x9e,0xdf};
    u32 ret;

    ret = hash_test(HASH_SHA3_224, std_digest1, std_digest2, std_digest3);
    if(ret)
    {
        return 1;
    }

#ifdef AIC_HASH_DMA
    ret = hash_dma_test(HASH_SHA3_224, std_digest1, std_digest2, std_digest3);
#endif

    return ret;
}
#endif


#ifdef SUPPORT_HASH_SHA3_256
u32 SHA3_256_test(void)
{
    u8 std_digest1[32] =  {0x3A,0x98,0x5D,0xA7,0x4F,0xE2,0x25,0xB2,0x04,0x5C,0x17,0x2D,0x6B,0xD3,0x90,0xBD,
                                0x85,0x5F,0x08,0x6E,0x3E,0x9D,0x52,0x5B,0x46,0xBF,0xE2,0x45,0x11,0x43,0x15,0x32};
    u8 std_digest2[32] =  {0x0F,0x96,0x25,0x9F,0x82,0xFB,0x8C,0x30,0xD3,0xD7,0x02,0xE8,0xA8,0x9A,0x47,0x5D,
                                0x2E,0x96,0x69,0x0A,0x5E,0xE9,0x89,0x2A,0x48,0x35,0x18,0x64,0xFB,0x49,0x2A,0xA5};
    u8 std_digest3[32] =  {0x28,0xcf,0xc5,0x5e,0x9b,0x31,0xe5,0x86,0xd6,0x82,0xb1,0x5c,0x90,0x67,0xd8,0x98,
                                0xfd,0x50,0x6c,0x7f,0x37,0x61,0x00,0xb1,0xa0,0x99,0x04,0x11,0x16,0x1f,0x53,0x53};
    u32 ret;

    printf("\r\n -------------- HASH_SHA3_256 -------------- \r\n");

    ret = hash_test(HASH_SHA3_256, std_digest1, std_digest2, std_digest3);
    if(ret)
    {
        return 1;
    }

#ifdef AIC_HASH_DMA
    ret = hash_dma_test(HASH_SHA3_256, std_digest1, std_digest2, std_digest3);
#endif

    return ret;
}
#endif


#ifdef SUPPORT_HASH_SHA3_384
u32 SHA3_384_test(void)
{
    u8 std_digest1[48] =  {0xEC,0x01,0x49,0x82,0x88,0x51,0x6F,0xC9,0x26,0x45,0x9F,0x58,0xE2,0xC6,0xAD,0x8D,
                                0xF9,0xB4,0x73,0xCB,0x0F,0xC0,0x8C,0x25,0x96,0xDA,0x7C,0xF0,0xE4,0x9B,0xE4,0xB2,
                                0x98,0xD8,0x8C,0xEA,0x92,0x7A,0xC7,0xF5,0x39,0xF1,0xED,0xF2,0x28,0x37,0x6D,0x25};
    u8 std_digest2[48] =  {0x88,0x58,0x7B,0x39,0x6E,0x42,0x97,0xAB,0x27,0x90,0x8F,0xB4,0x74,0x1A,0x32,0x59,
                                0x37,0x2E,0x6D,0xD8,0xFA,0x62,0xB2,0x29,0x12,0xD0,0xC6,0x0A,0xFC,0x4E,0xB8,0x9C,
                                0xFD,0x6A,0x7B,0xAA,0x7F,0x86,0x90,0xF6,0xBE,0x87,0x41,0x40,0xB1,0xD5,0x27,0x13};
    u8 std_digest3[48] =  {0xf0,0xc8,0x8e,0x74,0x20,0x4d,0xf2,0x27,0x1f,0xfe,0x4f,0xbb,0x21,0xbb,0xdd,0x7e,
                                0x29,0x96,0xc8,0xba,0x4e,0x46,0x45,0x94,0x1b,0xea,0xcb,0xb6,0x76,0x06,0x2a,0x93,
                                0x50,0xb6,0x28,0x57,0x92,0x06,0x7c,0xfb,0x05,0xd0,0xa3,0xd9,0x86,0x3a,0x3d,0xd9};
    u32 ret;

    ret = hash_test(HASH_SHA3_384, std_digest1, std_digest2, std_digest3);
    if(ret)
    {
        return 1;
    }

#ifdef AIC_HASH_DMA
    ret = hash_dma_test(HASH_SHA3_384, std_digest1, std_digest2, std_digest3);
#endif

    return ret;
}
#endif


#ifdef SUPPORT_HASH_SHA3_512
u32 SHA3_512_test(void)
{
    u8 std_digest1[64] =  {0xB7,0x51,0x85,0x0B,0x1A,0x57,0x16,0x8A,0x56,0x93,0xCD,0x92,0x4B,0x6B,0x09,0x6E,
                                0x08,0xF6,0x21,0x82,0x74,0x44,0xF7,0x0D,0x88,0x4F,0x5D,0x02,0x40,0xD2,0x71,0x2E,
                                0x10,0xE1,0x16,0xE9,0x19,0x2A,0xF3,0xC9,0x1A,0x7E,0xC5,0x76,0x47,0xE3,0x93,0x40,
                                0x57,0x34,0x0B,0x4C,0xF4,0x08,0xD5,0xA5,0x65,0x92,0xF8,0x27,0x4E,0xEC,0x53,0xF0};
    u8 std_digest2[64] =  {0x5C,0x37,0x18,0x29,0xF7,0x74,0x95,0x9D,0x04,0xAC,0x4B,0x4E,0x49,0x55,0x7E,0x43,
                                0x55,0x42,0x74,0x07,0xCA,0x96,0x0A,0x3A,0xCC,0x96,0x43,0xBB,0x5B,0x13,0xCB,0x69,
                                0x2B,0xBE,0x28,0x1F,0x3F,0x41,0x37,0x78,0x45,0x51,0x09,0x2B,0x46,0x13,0xE0,0x16,
                                0xA4,0xF1,0xFF,0x27,0x14,0xF8,0xF6,0x7F,0x97,0xD1,0x1E,0x32,0x33,0xA9,0xB6,0x62};
    u8 std_digest3[64] =  {0xed,0xa3,0xb5,0x78,0xd3,0x6e,0x63,0x10,0x53,0x77,0x02,0x0b,0xb4,0x83,0x0b,0xdd,
                                0x97,0x4d,0x65,0xd9,0x36,0xee,0xc3,0xe1,0x6e,0x78,0x34,0x64,0x44,0x32,0x13,0xee,
                                0x04,0x88,0xf1,0x94,0x1d,0x0b,0x6b,0x62,0x4a,0x5f,0x23,0xc1,0x5d,0x7e,0xba,0xc2,
                                0xd2,0x84,0x12,0xa4,0xe6,0xa0,0xd7,0x58,0xe0,0x34,0x2e,0x81,0xaf,0x13,0xdb,0x7b};
    u32 ret;

    ret = hash_test(HASH_SHA3_512, std_digest1, std_digest2, std_digest3);
    if(ret)
    {
        return 1;
    }

#ifdef AIC_HASH_DMA
    ret = hash_dma_test(HASH_SHA3_512, std_digest1, std_digest2, std_digest3);
#endif

    return ret;
}
#endif


u32 HASH_all_test(void)
{
    printf("\r\n\r\n =================== HASH test =================== ");

#if 0
    hash_speed_test();
#endif

#if 0
#ifdef AIC_HASH_DMA
    hash_dma_speed_test();
#endif
#endif

#if 1
#ifdef AIC_HASH_SM3_SUPPORT
    if(SM3_test())
        return 1;
#endif

#ifdef SUPPORT_HASH_MD5
    if(MD5_test())
        return 1;
#endif

#ifdef AIC_HASH_SHA256_SUPPORT
    if(SHA256_test())
        return 1;
#endif

#ifdef SUPPORT_HASH_SHA384
    if(SHA384_test())
        return 1;
#endif

#ifdef SUPPORT_HASH_SHA512
    if(SHA512_test())
        return 1;
#endif

#ifdef AIC_HASH_SHA1_SUPPORT
    if(SHA1_test())
        return 1;
#endif

#ifdef AIC_HASH_SHA224_SUPPORT
    if(SHA224_test())
        return 1;
#endif

#ifdef SUPPORT_HASH_SHA512_224
    if(SHA512_224_test())
        return 1;
#endif

#ifdef SUPPORT_HASH_SHA512_256
    if(SHA512_256_test())
        return 1;
#endif

#if (defined(AIC_HASH_MUL_THREAD) && defined(AIC_HASH_SHA256_SUPPORT) && defined(SUPPORT_HASH_SHA512))
    if(SHA256_SHA512_cross_test())
        return 1;
#endif

#ifdef SUPPORT_HASH_SHA3_224
    if(SHA3_224_test())
        return 1;
#endif

#ifdef SUPPORT_HASH_SHA3_256
    if(SHA3_256_test())
        return 1;
#endif

#ifdef SUPPORT_HASH_SHA3_384
    if(SHA3_384_test())
        return 1;
#endif

#ifdef SUPPORT_HASH_SHA3_512
    if(SHA3_512_test())
        return 1;
#endif
#endif

    printf("\r\n\r\n HASH test success \r\n\r\n");

    return 0;
}

