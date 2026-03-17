#include <stdio.h>
#include <string.h>

#include <hmac_sha256.h>
#include <utility.h>

void hash_call_manage(void);

#ifdef AIC_HASH_SHA256_SUPPORT

u32 sample_test_hmac_sha256(void)
{
    u8 msg[80] = {
        0xCB,0xAC,0x28,0x1C,0xAC,0xD3,0x03,0x5F,0x81,0x5A,0x0E,0x82,0xF0,0xA5,0xEA,0x7A,
        0xC5,0x3C,0x4F,0xF7,0x0B,0xB7,0x9F,0xA6,0x1E,0x80,0xEA,0xF6,0xE6,0x03,0x0F,0x7F,
        0xF8,0x3E,0xF7,0x36,0x0F,0xC4,0x10,0xB0,0x1E,0xDC,0x20,0xD1,0xA1,0xB2,0xBD,0xAF,
        0xBE,0xC0,0x21,0xB4,0xF3,0x68,0xE3,0x06,0x91,0x34,0xE0,0x12,0xC2,0xB4,0x30,0x70,
        0x83,0xD3,0xA9,0xBD,0xD2,0x06,0xE2,0x4E,0x5F,0x0D,0x86,0xE1,0x3D,0x66,0x36,0x65,
    };
    u8 key[16] = {
        0x92,0x8C,0x8A,0xBE,0xCB,0xCB,0x38,0x5B,0x8E,0xEA,0xA0,0x05,0x9A,0xC8,0x05,0x84,
    };
    u8 std_mac[32] = {
        0x8E,0x11,0x01,0x63,0x24,0xBF,0xE8,0xA6,0xF3,0x71,0x4A,0xF1,0x38,0xE6,0x26,0xA9,
        0x50,0x30,0x79,0x76,0x8E,0x38,0xCD,0xC2,0xB0,0x42,0xEA,0x5D,0x47,0x67,0x3E,0xAC
    };
    u8 mac[32];
    u8 msg_byte_len = 80;
    u8 key_byte_len = 16;
    u8 mac_byte_len = 32;  //digest_byte_len
    u8 block_byte_len = 64;

    u32 ret;

    HMAC_SHA256_CTX hmac_sha256_ctx[1];
    HMAC_CTX *hmac_ctx = hmac_sha256_ctx;

#ifdef AIC_HASH_NODE
    HASH_NODE node[3];
#endif

#ifdef AIC_HASH_DMA_NODE
    HASH_DMA_NODE dma_node[3];
#endif

#ifdef AIC_HASH_DMA
    u32 *msg_buf = (u32 *)(AIC_HASH_DMA_RAM_BASE);
    u32 *mac_buf = msg_buf+512;
    u32 blocks_bytes = msg_byte_len - (msg_byte_len%block_byte_len);
    u32 remainder_bytes = msg_byte_len - blocks_bytes;
    HMAC_SHA256_DMA_CTX hmac_sha256_dma_ctx[1];
    HMAC_DMA_CTX *hmac_dma_ctx = hmac_sha256_dma_ctx;
#ifdef AIC_HASH_ADDRESS_HIGH_LOW
    u32 addr_h_0 = 0;
#endif

    uint32_copy(msg_buf, (u32 *)msg, (msg_byte_len+3)/4);
#endif

    printf("\r\n\r\n -------------- HMAC-SHA256 sample test -------------- ");

    //the following 6 calculations are equivalent

    /**************** test 1 ****************/
    memset(mac, 0, mac_byte_len);
    ret = hmac_sha256_init(hmac_sha256_ctx, key, 0, key_byte_len);
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

    ret = hmac_sha256_update(hmac_sha256_ctx, msg, 4);
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

    ret = hmac_sha256_update(hmac_sha256_ctx, msg+4, 44);
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

    ret = hmac_sha256_update(hmac_sha256_ctx, msg+48, 32);
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

    ret = hmac_sha256_final(hmac_sha256_ctx, mac);
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

    if(memcmp_(std_mac, mac, mac_byte_len))
    {
        printf("\r\n HMAC-SHA256 sample test 1 error");
        return 1;
    }
    else
    {
        printf("\r\n HMAC-SHA256 sample test 1 success");
    }

    /**************** test 2 ****************/
    memset(mac, 0, mac_byte_len);
    ret = hmac_sha256(key, 0, key_byte_len, msg, msg_byte_len, mac);
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

    if(memcmp_(std_mac, mac, mac_byte_len))
    {
        printf("\r\n HMAC-SHA256 sample test 2 error");
        return 1;
    }
    else
    {
        printf("\r\n HMAC-SHA256 sample test 2 success");
    }

    /**************** test 3 ****************/
#ifdef AIC_HASH_NODE
    memset(mac, 0, mac_byte_len);

    node[0].msg_addr  = msg;
    node[0].msg_bytes = 4;
    node[1].msg_addr  = msg + 4;
    node[1].msg_bytes = 44;
    node[2].msg_addr  = msg +48;
    node[2].msg_bytes = 32;
    ret = hmac_sha256_node_steps(key, 0, key_byte_len, node, 3, mac);
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

    if(memcmp_(std_mac, mac, mac_byte_len))
    {
        printf("\r\n HMAC-SHA256 sample test 3 error");
        return 1;
    }
    else
    {
        printf("\r\n HMAC-SHA256 sample test 3 success");
    }
#endif

    /**************** test 4 ****************/
    memset(mac, 0, mac_byte_len);
    ret = hmac_init(hmac_ctx, HASH_SHA256, key, 0, key_byte_len);
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

    ret = hmac_update(hmac_sha256_ctx, msg, 4);
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

    ret = hmac_update(hmac_sha256_ctx, msg+4, 44);
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

    ret = hmac_update(hmac_sha256_ctx, msg+48, 32);
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

    ret = hmac_final(hmac_sha256_ctx, mac);
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

    if(memcmp_(std_mac, mac, mac_byte_len))
    {
        printf("\r\n HMAC-SHA256 sample test 4 error");
        return 1;
    }
    else
    {
        printf("\r\n HMAC-SHA256 sample test 4 success");
    }

    /**************** test 5 ****************/
    memset(mac, 0, mac_byte_len);
    ret = hmac(HASH_SHA256, key, 0, key_byte_len, msg, msg_byte_len, mac);
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

    if(memcmp_(std_mac, mac, mac_byte_len))
    {
        printf("\r\n HMAC-SHA256 sample test 5 error");
        return 1;
    }
    else
    {
        printf("\r\n HMAC-SHA256 sample test 5 success");
    }

    /**************** test 6 ****************/
#ifdef AIC_HASH_NODE
    memset(mac, 0, mac_byte_len);

    node[0].msg_addr  = msg;
    node[0].msg_bytes = 4;
    node[1].msg_addr  = msg + 4;
    node[1].msg_bytes = 44;
    node[2].msg_addr  = msg +48;
    node[2].msg_bytes = 32;
    ret = hmac_node_steps(HASH_SHA256, key, 0, key_byte_len, node, 3, mac);
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

    if(memcmp_(std_mac, mac, mac_byte_len))
    {
        printf("\r\n HMAC-SHA256 sample test 6 error");
        return 1;
    }
    else
    {
        printf("\r\n HMAC-SHA256 sample test 6 success");
    }
#endif


#ifdef AIC_HASH_DMA
    //the following 6 DMA calculations are equivalent

    /**************** dma test 1 ****************/
    uint32_clear(mac_buf, mac_byte_len/4);
    ret = hmac_sha256_dma_init(hmac_sha256_dma_ctx, key, 0, key_byte_len, hash_call_manage);
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

#ifdef AIC_HASH_ADDRESS_HIGH_LOW
    ret = hmac_sha256_dma_update_blocks(hmac_sha256_dma_ctx, addr_h_0, (u32)msg_buf, blocks_bytes);
#else
    ret = hmac_sha256_dma_update_blocks(hmac_sha256_dma_ctx, msg_buf, blocks_bytes);
#endif
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

#ifdef AIC_HASH_ADDRESS_HIGH_LOW
    ret = hmac_sha256_dma_final(hmac_sha256_dma_ctx, addr_h_0, ((u32)msg_buf)+blocks_bytes, remainder_bytes, addr_h_0, (u32)mac_buf);
#else
    ret = hmac_sha256_dma_final(hmac_sha256_dma_ctx, msg_buf+blocks_bytes/4, remainder_bytes, mac_buf);
#endif
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

    if(memcmp_(std_mac, (u8 *)mac_buf, mac_byte_len))
    {
        printf("\r\n HMAC-SHA256 dma sample test 1 error");
        return 1;
    }
    else
    {
        printf("\r\n HMAC-SHA256 dma sample test 1 success");
    }

    /**************** dma test 2 ****************/
    uint32_clear(mac_buf, mac_byte_len/4);
#ifdef AIC_HASH_ADDRESS_HIGH_LOW
    ret = hmac_sha256_dma(key, 0, key_byte_len, addr_h_0, (u32)msg_buf, msg_byte_len, addr_h_0, (u32)mac_buf, hash_call_manage);
#else
    ret = hmac_sha256_dma(key, 0, key_byte_len, msg_buf, msg_byte_len, mac_buf, hash_call_manage);
#endif
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

    if(memcmp_(std_mac, (u8 *)mac_buf, mac_byte_len))
    {
        printf("\r\n HMAC-SHA256 dma sample test 2 error");
        return 1;
    }
    else
    {
        printf("\r\n HMAC-SHA256 dma sample test 2 success");
    }

    /**************** dma test 3 ****************/
    uint32_clear(mac_buf, mac_byte_len/4);
#ifdef AIC_HASH_ADDRESS_HIGH_LOW
    dma_node[0].msg_addr_h  = addr_h_0;
    dma_node[0].msg_addr_l  = (u32)msg_buf;
    dma_node[0].msg_bytes   = blocks_bytes;
    dma_node[1].msg_addr_h  = addr_h_0;
    dma_node[1].msg_addr_l  = dma_node[0].msg_addr_l + dma_node[0].msg_bytes;
    dma_node[1].msg_bytes   = remainder_bytes;
    ret = hmac_sha256_dma_node_steps(key, 0, key_byte_len, dma_node, 2, addr_h_0, (u32)mac_buf, hash_call_manage);
#else
    dma_node[0].msg_addr  = msg_buf;
    dma_node[0].msg_bytes = blocks_bytes;
    dma_node[1].msg_addr  = dma_node[0].msg_addr + dma_node[0].msg_bytes/4;
    dma_node[1].msg_bytes = remainder_bytes;
    ret = hmac_sha256_dma_node_steps(key, 0, key_byte_len, dma_node, 2, mac_buf, hash_call_manage);
#endif

    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

    if(memcmp_(std_mac, (u8 *)mac_buf, mac_byte_len))
    {
        printf("\r\n HMAC-SHA256 dma sample test 3 error");
        return 1;
    }
    else
    {
        printf("\r\n HMAC-SHA256 dma sample test 3 success");
    }

    /**************** dma test 4 ****************/
    uint32_clear(mac_buf, mac_byte_len/4);
    ret = hmac_dma_init(hmac_dma_ctx, HASH_SHA256, key, 0, key_byte_len, hash_call_manage);
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

#ifdef AIC_HASH_ADDRESS_HIGH_LOW
    ret = hmac_dma_update_blocks(hmac_dma_ctx, addr_h_0, (u32)msg_buf, blocks_bytes);
#else
    ret = hmac_dma_update_blocks(hmac_dma_ctx, msg_buf, blocks_bytes);
#endif
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

#ifdef AIC_HASH_ADDRESS_HIGH_LOW
    ret = hmac_dma_final(hmac_dma_ctx, addr_h_0, ((u32)msg_buf)+blocks_bytes, remainder_bytes, addr_h_0, (u32)mac_buf);
#else
    ret = hmac_dma_final(hmac_dma_ctx, msg_buf+blocks_bytes/4, remainder_bytes, mac_buf);
#endif
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

    if(memcmp_(std_mac, (u8 *)mac_buf, mac_byte_len))
    {
        printf("\r\n HMAC-SHA256 dma sample test 4 error");
        return 1;
    }
    else
    {
        printf("\r\n HMAC-SHA256 dma sample test 4 success");
    }

    /**************** dma test 5 ****************/
    uint32_clear(mac_buf, mac_byte_len/4);
#ifdef AIC_HASH_ADDRESS_HIGH_LOW
    ret = hmac_dma(HASH_SHA256, key, 0, key_byte_len, addr_h_0, (u32)msg_buf, msg_byte_len, addr_h_0, (u32)mac_buf, hash_call_manage);
#else
    ret = hmac_dma(HASH_SHA256, key, 0, key_byte_len, msg_buf, msg_byte_len, mac_buf, hash_call_manage);
#endif
    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

    if(memcmp_(std_mac, (u8 *)mac_buf, mac_byte_len))
    {
        printf("\r\n HMAC-SHA256 dma sample test 5 error");
        return 1;
    }
    else
    {
        printf("\r\n HMAC-SHA256 dma sample test 5 success");
    }

    /**************** dma test 6 ****************/
    uint32_clear(mac_buf, mac_byte_len/4);
#ifdef AIC_HASH_ADDRESS_HIGH_LOW
    dma_node[0].msg_addr_h  = addr_h_0;
    dma_node[0].msg_addr_l  = (u32)msg_buf;
    dma_node[0].msg_bytes   = blocks_bytes;
    dma_node[1].msg_addr_h  = addr_h_0;
    dma_node[1].msg_addr_l  = dma_node[0].msg_addr_l + dma_node[0].msg_bytes;
    dma_node[1].msg_bytes   = remainder_bytes;
    ret = hmac_dma_node_steps(HASH_SHA256, key, 0, key_byte_len, dma_node, 2, addr_h_0, (u32)mac_buf, hash_call_manage);
#else
    dma_node[0].msg_addr  = msg_buf;
    dma_node[0].msg_bytes = blocks_bytes;
    dma_node[1].msg_addr  = dma_node[0].msg_addr + dma_node[0].msg_bytes/4;
    dma_node[1].msg_bytes = remainder_bytes;
    ret = hmac_dma_node_steps(HASH_SHA256, key, 0, key_byte_len, dma_node, 2, mac_buf, hash_call_manage);
#endif

    if(HASH_SUCCESS != ret)
    {
        return 1;
    }

    if(memcmp_(std_mac, (u8 *)mac_buf, mac_byte_len))
    {
        printf("\r\n HMAC-SHA256 dma sample test 6 error");
        return 1;
    }
    else
    {
        printf("\r\n HMAC-SHA256 dma sample test 6 success");
    }

#endif

    return 0;
}


#endif

