#include <stdio.h>
#include <string.h>
#include <sha1.h>
#include <utility.h>

void hash_call_manage(void);

#ifdef AIC_HASH_SHA1_SUPPORT

u32 sample_test_sha1(void)
{
    u8 msg[80] = {
        0xCB,0xAC,0x28,0x1C,0xAC,0xD3,0x03,0x5F,0x81,0x5A,0x0E,0x82,0xF0,0xA5,0xEA,0x7A,
        0xC5,0x3C,0x4F,0xF7,0x0B,0xB7,0x9F,0xA6,0x1E,0x80,0xEA,0xF6,0xE6,0x03,0x0F,0x7F,
        0xF8,0x3E,0xF7,0x36,0x0F,0xC4,0x10,0xB0,0x1E,0xDC,0x20,0xD1,0xA1,0xB2,0xBD,0xAF,
        0xBE,0xC0,0x21,0xB4,0xF3,0x68,0xE3,0x06,0x91,0x34,0xE0,0x12,0xC2,0xB4,0x30,0x70,
        0x83,0xD3,0xA9,0xBD,0xD2,0x06,0xE2,0x4E,0x5F,0x0D,0x86,0xE1,0x3D,0x66,0x36,0x65,
    };
    u8 std_digest[20] = {
        0x8F,0x6F,0x86,0x53,0x31,0x4A,0x3B,0x0F,0xBD,0x88,0x03,0xFD,0xC2,0xC9,0x7E,0x26,
        0x5E,0x7A,0xCE,0x48
    };
    u8 digest[20];
    u8 msg_byte_len = 80;
    u8 digest_byte_len = 20;
    u8 block_byte_len = 64;

    u32 ret;

    SHA1_CTX sha1_ctx[1];
    HASH_CTX *hash_ctx = sha1_ctx;

#ifdef AIC_HASH_NODE
    HASH_NODE node[3];
#endif

#ifdef AIC_HASH_DMA_NODE
    HASH_DMA_NODE dma_node[3];
#endif

#ifdef AIC_HASH_DMA
    u32 *msg_buf = (u32 *)(AIC_HASH_DMA_RAM_BASE);
    u32 *digest_buf = msg_buf+512;
    u32 blocks_bytes = msg_byte_len - (msg_byte_len%block_byte_len);
    u32 remainder_bytes = msg_byte_len - blocks_bytes;
    SHA1_DMA_CTX sha1_dma_ctx[1];
    HASH_DMA_CTX *hash_dma_ctx = sha1_dma_ctx;
#ifdef AIC_HASH_ADDRESS_HIGH_LOW
    u32 addr_h_0 = 0;
#endif

    uint32_copy(msg_buf, (u32 *)msg, (msg_byte_len+3)/4);
#endif

    printf("\r\n\r\n -------------- SHA1 sample test -------------- ");

    //the following 6 calculations are equivalent

    /**************** test 1 ****************/
    memset(digest, 0, digest_byte_len);
    ret = sha1_init(sha1_ctx);
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

    ret = sha1_update(sha1_ctx, msg, 4);
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

    ret = sha1_update(sha1_ctx, msg+4, 44);
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

    ret = sha1_update(sha1_ctx, msg+48, 32);
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

    ret = sha1_final(sha1_ctx, digest);
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

    if(memcmp_(std_digest, digest, digest_byte_len))
    {
        printf("\r\n SHA1 sample test 1 error");
        return 1;
    }
    else
    {
        printf("\r\n SHA1 sample test 1 success");
    }

    /**************** test 2 ****************/
    memset(digest, 0, digest_byte_len);
    ret = sha1(msg, msg_byte_len, digest);
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

    if(memcmp_(std_digest, digest, digest_byte_len))
    {
        printf("\r\n SHA1 sample test 2 error");
        return 1;
    }
    else
    {
        printf("\r\n SHA1 sample test 2 success");
    }

    /**************** test 3 ****************/
#ifdef AIC_HASH_NODE
    memset(digest, 0, digest_byte_len);

    node[0].msg_addr  = msg;
    node[0].msg_bytes = 4;
    node[1].msg_addr  = msg + 4;
    node[1].msg_bytes = 44;
    node[2].msg_addr  = msg +48;
    node[2].msg_bytes = 32;
    ret = sha1_node_steps(node, 3, digest);
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

    if(memcmp_(std_digest, digest, digest_byte_len))
    {
        printf("\r\n SHA1 sample test 3 error");
        return 1;
    }
    else
    {
        printf("\r\n SHA1 sample test 3 success");
    }
#endif

    /**************** test 4 ****************/
    memset(digest, 0, digest_byte_len);
    ret = hash_init(hash_ctx, HASH_SHA1);
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

    ret = hash_update(hash_ctx, msg, 4);
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

    ret = hash_update(hash_ctx, msg+4, 44);
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

    ret = hash_update(hash_ctx, msg+48, 32);
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

    ret = hash_final(hash_ctx, digest);
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

    if(memcmp_(std_digest, digest, digest_byte_len))
    {
        printf("\r\n SHA1 sample test 4 error");
        return 1;
    }
    else
    {
        printf("\r\n SHA1 sample test 4 success");
    }

    /**************** test 5 ****************/
    memset(digest, 0, digest_byte_len);
    ret = hash(HASH_SHA1, msg, msg_byte_len, digest);
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

    if(memcmp_(std_digest, digest, digest_byte_len))
    {
        printf("\r\n SHA1 sample test 5 error");
        return 1;
    }
    else
    {
        printf("\r\n SHA1 sample test 5 success");
    }

    /**************** test 6 ****************/
#ifdef AIC_HASH_NODE
    memset(digest, 0, digest_byte_len);

    node[0].msg_addr  = msg;
    node[0].msg_bytes = 4;
    node[1].msg_addr  = msg + 4;
    node[1].msg_bytes = 44;
    node[2].msg_addr  = msg +48;
    node[2].msg_bytes = 32;
    ret = hash_node_steps(HASH_SHA1, node, 3, digest);
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

    if(memcmp_(std_digest, digest, digest_byte_len))
    {
        printf("\r\n SHA1 sample test 6 error");
        return 1;
    }
    else
    {
        printf("\r\n SHA1 sample test 6 success");
    }
#endif


#ifdef AIC_HASH_DMA
    //the following 6 DMA calculations are equivalent

    /**************** dma test 1 ****************/
    uint32_clear(digest_buf, digest_byte_len/4);
    ret = sha1_dma_init(sha1_dma_ctx, hash_call_manage);
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

#ifdef AIC_HASH_ADDRESS_HIGH_LOW
    ret = sha1_dma_update_blocks(sha1_dma_ctx, addr_h_0, (u32)msg_buf, blocks_bytes);
#else
    ret = sha1_dma_update_blocks(sha1_dma_ctx, msg_buf, blocks_bytes);
#endif
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

#ifdef AIC_HASH_ADDRESS_HIGH_LOW
    ret = sha1_dma_final(sha1_dma_ctx, addr_h_0, ((u32)msg_buf)+blocks_bytes, remainder_bytes, addr_h_0, (u32)digest_buf);
#else
    ret = sha1_dma_final(sha1_dma_ctx, msg_buf+blocks_bytes/4, remainder_bytes, digest_buf);
#endif
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

    if(uint32_BigNumCmp((u32 *)std_digest, digest_byte_len/4, digest_buf, digest_byte_len/4))
    {
        printf("\r\n SHA1 dma sample test 1 error");
        return 1;
    }
    else
    {
        printf("\r\n SHA1 dma sample test 1 success");
    }

    /**************** dma test 2 ****************/
    uint32_clear(digest_buf, digest_byte_len/4);
#ifdef AIC_HASH_ADDRESS_HIGH_LOW
    ret = sha1_dma(addr_h_0, (u32)msg_buf, msg_byte_len, addr_h_0, (u32)digest_buf, hash_call_manage);
#else
    ret = sha1_dma(msg_buf, msg_byte_len, digest_buf, hash_call_manage);
#endif
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

    if(uint32_BigNumCmp((u32 *)std_digest, digest_byte_len/4, digest_buf, digest_byte_len/4))
    {
        printf("\r\n SHA1 dma sample test 2 error");
        return 1;
    }
    else
    {
        printf("\r\n SHA1 dma sample test 2 success");
    }

    /**************** dma test 3 ****************/
    uint32_clear(digest_buf, digest_byte_len/4);
#ifdef AIC_HASH_ADDRESS_HIGH_LOW
    dma_node[0].msg_addr_h  = addr_h_0;
    dma_node[0].msg_addr_l  = (u32)msg_buf;
    dma_node[0].msg_bytes   = blocks_bytes;
    dma_node[1].msg_addr_h  = addr_h_0;
    dma_node[1].msg_addr_l  = dma_node[0].msg_addr_l + dma_node[0].msg_bytes;
    dma_node[1].msg_bytes   = remainder_bytes;
    ret = sha1_dma_node_steps(dma_node, 2, addr_h_0, (u32)digest_buf, hash_call_manage);
#else
    dma_node[0].msg_addr  = msg_buf;
    dma_node[0].msg_bytes = blocks_bytes;
    dma_node[1].msg_addr  = dma_node[0].msg_addr + dma_node[0].msg_bytes/4;
    dma_node[1].msg_bytes = remainder_bytes;
    ret = sha1_dma_node_steps(dma_node, 2, digest_buf, hash_call_manage);
#endif

    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

    if(uint32_BigNumCmp((u32 *)std_digest, digest_byte_len/4, digest_buf, digest_byte_len/4))
    {
        printf("\r\n SHA1 dma sample test 3 error");
        return 1;
    }
    else
    {
        printf("\r\n SHA1 dma sample test 3 success");
    }

    /**************** dma test 4 ****************/
    uint32_clear(digest_buf, digest_byte_len/4);
    ret = hash_dma_init(hash_dma_ctx, HASH_SHA1, hash_call_manage);
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

#ifdef AIC_HASH_ADDRESS_HIGH_LOW
    ret = hash_dma_update_blocks(hash_dma_ctx, addr_h_0, (u32)msg_buf, blocks_bytes);
#else
    ret = hash_dma_update_blocks(hash_dma_ctx, msg_buf, blocks_bytes);
#endif
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

#ifdef AIC_HASH_ADDRESS_HIGH_LOW
    ret = hash_dma_final(hash_dma_ctx, addr_h_0, ((u32)msg_buf)+blocks_bytes, remainder_bytes, addr_h_0, (u32)digest_buf);
#else
    ret = hash_dma_final(hash_dma_ctx, msg_buf+blocks_bytes/4, remainder_bytes, digest_buf);
#endif
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

    if(uint32_BigNumCmp((u32 *)std_digest, digest_byte_len/4, digest_buf, digest_byte_len/4))
    {
        printf("\r\n SHA1 dma sample test 4 error");
        return 1;
    }
    else
    {
        printf("\r\n SHA1 dma sample test 4 success");
    }

    /**************** dma test 5 ****************/
    uint32_clear(digest_buf, digest_byte_len/4);
#ifdef AIC_HASH_ADDRESS_HIGH_LOW
    ret = hash_dma(HASH_SHA1, addr_h_0, (u32)msg_buf, msg_byte_len, addr_h_0, (u32)digest_buf, hash_call_manage);
#else
    ret = hash_dma(HASH_SHA1, msg_buf, msg_byte_len, digest_buf, hash_call_manage);
#endif
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

    if(uint32_BigNumCmp((u32 *)std_digest, digest_byte_len/4, digest_buf, digest_byte_len/4))
    {
        printf("\r\n SHA1 dma sample test 5 error");
        return 1;
    }
    else
    {
        printf("\r\n SHA1 dma sample test 5 success");
    }

    /**************** dma test 6 ****************/
    uint32_clear(digest_buf, digest_byte_len/4);
#ifdef AIC_HASH_ADDRESS_HIGH_LOW
    dma_node[0].msg_addr_h  = addr_h_0;
    dma_node[0].msg_addr_l  = (u32)msg_buf;
    dma_node[0].msg_bytes   = blocks_bytes;
    dma_node[1].msg_addr_h  = addr_h_0;
    dma_node[1].msg_addr_l  = dma_node[0].msg_addr_l + dma_node[0].msg_bytes;
    dma_node[1].msg_bytes   = remainder_bytes;
    ret = hash_dma_node_steps(HASH_SHA1, dma_node, 2, addr_h_0, (u32)digest_buf, hash_call_manage);
#else
    dma_node[0].msg_addr  = msg_buf;
    dma_node[0].msg_bytes = blocks_bytes;
    dma_node[1].msg_addr  = dma_node[0].msg_addr + dma_node[0].msg_bytes/4;
    dma_node[1].msg_bytes = remainder_bytes;
    ret = hash_dma_node_steps(HASH_SHA1, dma_node, 2, digest_buf, hash_call_manage);
#endif

    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

    if(uint32_BigNumCmp((u32 *)std_digest, digest_byte_len/4, digest_buf, digest_byte_len/4))
    {
        printf("\r\n SHA1 dma sample test 6 error");
        return 1;
    }
    else
    {
        printf("\r\n SHA1 dma sample test 6 success");
    }

#endif

    return 0;
}


#endif

