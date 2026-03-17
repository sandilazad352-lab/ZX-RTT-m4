#include "zx_ui_entry.h"
#include "mpp_types.h"
#include <stdio.h>
#include <stdlib.h>
#include "aic_core.h"

typedef struct _lv_ll_img_dsc_t {
    int index;
    lv_img_dsc_t img_dsc;
} lv_ll_img_dsc_t;

lv_ll_t _lv_sw_img_ll;    //lvgl链表

static void _lv_sw_png_init(lv_ll_t *_lv_sw_img_ll)
{
    _lv_ll_init(_lv_sw_img_ll, sizeof(lv_ll_img_dsc_t));
}

//从链表释放图片资源
static void _lv_sw_png_src_free(lv_ll_t *_lv_sw_img_ll)
{
    lv_ll_img_dsc_t * img_ll;

    img_ll = _lv_ll_get_head(_lv_sw_img_ll);
    while(img_ll != NULL) {
        free((void *)(img_ll->img_dsc.data));
        img_ll = _lv_ll_get_next(_lv_sw_img_ll, img_ll);
    }
    _lv_ll_clear(_lv_sw_img_ll);
}

 //根据下标从链表获取图片资源
static lv_img_dsc_t *_lv_get_sw_png_src_index(lv_ll_t *_lv_sw_img_ll, int index)
{
    lv_ll_img_dsc_t * img_ll;
    
    img_ll = _lv_ll_get_head(_lv_sw_img_ll);
    while(img_ll != NULL) {
        if(img_ll->index == index)
            break;
        img_ll = _lv_ll_get_next(_lv_sw_img_ll, img_ll);
    }
    
    if(img_ll != NULL)
        return &(img_ll->img_dsc);
    return NULL;
}
//软解
void _lv_sw_png_src_add(lv_ll_t *_lv_sw_img_ll, char *src)
{
    lv_ll_img_dsc_t * img_ll_dsc = NULL;
    lv_ll_img_dsc_t * img_ll_head;
    lv_img_decoder_dsc_t dsc;
    lv_color_t *src_data;

    img_ll_head = _lv_ll_get_head(_lv_sw_img_ll);    //获取头指针
    img_ll_dsc = _lv_ll_ins_head(_lv_sw_img_ll);
    //申请并插入新节点，返回新节点地址

    if(img_ll_dsc == NULL) return;
    lv_memset_00(img_ll_dsc, sizeof(lv_ll_img_dsc_t));

//软解
    lv_res_t open_res = lv_img_decoder_open(&dsc, src, lv_color_black(), -1);
    extern lv_res_t decoder_open(lv_img_decoder_t * dec, lv_img_decoder_dsc_t * dsc);
    decoder_open(NULL, &dsc);


    if(open_res == LV_RES_INV) {
        printf("Image draw cannot open the image resource");
        return;
    }
    if(dsc.error_msg != NULL) {
        printf("Image draw error");
    }

    src_data = (lv_color_t *)rt_malloc(sizeof(lv_color_t) * dsc.header.w * dsc.header.h);
    memset(src_data, 0, sizeof(lv_color_t) * dsc.header.w * dsc.header.h);
    memcpy((unsigned char *)src_data, dsc.img_data, dsc.header.w * dsc.header.h * sizeof(lv_color_t));


    printf("dsc.header.w = %d dsc.header.h = %d\n", dsc.header.w, dsc.header.h);
    printf("color = %lu dsc.header.cf = %u\n", sizeof(lv_color_t), dsc.header.cf);
    printf("src = %s\n", src);
    
    if(img_ll_head != NULL)
        img_ll_dsc->index = img_ll_head->index + 1;    //设置新节点下标
    else
        img_ll_dsc->index = 0;

    img_ll_dsc->img_dsc.data = (uint8_t *)src_data;
    img_ll_dsc->img_dsc.header.h = dsc.header.h;
    img_ll_dsc->img_dsc.header.w = dsc.header.w;
    img_ll_dsc->img_dsc.data_size = dsc.header.w * dsc.header.h * sizeof(lv_color_t);
    img_ll_dsc->img_dsc.header.always_zero = 0;
    img_ll_dsc->img_dsc.header.cf = dsc.header.cf;
    lv_img_decoder_close(&dsc);
}
//硬解
void _lv_hw_png_src_add(lv_ll_t *_lv_sw_img_ll, char *src)
{
    lv_ll_img_dsc_t * img_ll_dsc = NULL;
    lv_ll_img_dsc_t * img_ll_head;
    lv_img_decoder_dsc_t dsc;

    img_ll_head = _lv_ll_get_head(_lv_sw_img_ll);
    img_ll_dsc = _lv_ll_ins_head(_lv_sw_img_ll);

    if(img_ll_dsc == NULL) {
        printf("_lv_ll_ins_head failed\n");
        return;
    }
    lv_memset_00(img_ll_dsc, sizeof(lv_ll_img_dsc_t));

    lv_res_t open_res = lv_img_decoder_open(&dsc, src, lv_color_black(), -1);

    if(open_res == LV_RES_INV) {
        printf("Image draw cannot open the image resource\n");
        return;
    }
    if(dsc.error_msg != NULL) {
        printf("Image draw error\n");
        return;
    }

    struct mpp_frame *frame = (struct mpp_frame *)(dsc.img_data);   //帧数据
    unsigned int stride = frame->buf.stride[0];                 //步进值(内存对齐)
    printf("stride[0] = %u\n", frame->buf.stride[0]);
    uint32_t buf_size = dsc.header.w * dsc.header.h;            //总大小
    printf("buf_size = %u\n", buf_size);

    uint8_t * src_tmp8 = (uint8_t *)(frame->buf.phy_addr[0]);    //获取硬解后的物理地址
#if LV_COLOR_DEPTH == 16
    lv_color32_t * cbuf = aicos_malloc(MEM_CMA, buf_size * sizeof(lv_color32_t));
#else
    lv_color_t * cbuf = (lv_color_t *)aicos_malloc(MEM_CMA, buf_size * sizeof(lv_color_t));
#endif
    
    //  mpp_frame -> lv_color_t  将数据帧 转为 LVGL 的图片帧
#if LV_COLOR_DEPTH == 16
    
    uint8_t * tmp_cbuf = (uint8_t *)cbuf;
    for(int y = 0; y < dsc.header.h; y++)       
    {
        for(int x = 0; x < dsc.header.w * 4; x++) 
            tmp_cbuf[x + y * dsc.header.w * 4] = src_tmp8[x + stride * y];
    }
    lv_color32_t * img_argb = (lv_color32_t *)tmp_cbuf;
    lv_color_t c;
    uint32_t i;
    for(i = 0; i < buf_size; i++) {
        c = lv_color_make(img_argb[i].ch.red, img_argb[i].ch.green, img_argb[i].ch.blue);
        tmp_cbuf[i * 3 + 2] = img_argb[i].ch.alpha;
        tmp_cbuf[i * 3 + 1] = c.full >> 8;
        tmp_cbuf[i * 3 + 0] = c.full & 0xFF;
    }
    
#elif LV_COLOR_DEPTH == 32
    uint8_t * tmp_cbuf = (uint8_t *)cbuf;
    for(int y = 0; y < dsc.header.h; y++)       
    {
        for(int x = 0; x < dsc.header.w * LV_IMG_PX_SIZE_ALPHA_BYTE; x++) 
            tmp_cbuf[x + y * dsc.header.w * LV_IMG_PX_SIZE_ALPHA_BYTE] = src_tmp8[x + stride * y];
    }
#endif


    printf("dsc.header.w = %d dsc.header.h = %d\n", dsc.header.w, dsc.header.h);
    printf("color = %lu dsc.header.cf = %u\n", sizeof(lv_color_t), dsc.header.cf);
    printf("src = %s\n", src);
    
    if(img_ll_head != NULL)
        img_ll_dsc->index = img_ll_head->index + 1;
    else
        img_ll_dsc->index = 0;
        
    img_ll_dsc->img_dsc.data = (uint8_t *)cbuf;
    img_ll_dsc->img_dsc.header.h = dsc.header.h;
    img_ll_dsc->img_dsc.header.w = dsc.header.w;
    img_ll_dsc->img_dsc.data_size = dsc.header.w * dsc.header.h * LV_IMG_PX_SIZE_ALPHA_BYTE;
    img_ll_dsc->img_dsc.header.always_zero = 0;
    img_ll_dsc->img_dsc.header.cf = dsc.header.cf;
    lv_img_decoder_close(&dsc);
}



void zx_ui_entry(void)
{
	lv_obj_t * obj = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(obj);
    lv_obj_set_size(obj, LV_HOR_RES, LV_VER_RES);
    lv_obj_center(obj);
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), 0);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_clip_corner(obj, true, 0);

    //链表初始化
    _lv_sw_png_init(&_lv_sw_img_ll);

    //解码并添加图片资源到链表
    _lv_hw_png_src_add(&_lv_sw_img_ll, "L:/rodata/icon05.png");
    _lv_hw_png_src_add(&_lv_sw_img_ll, "L:/rodata/icon06.png");

    //获取链表资源
    lv_img_dsc_t *_icon05 = _lv_get_sw_png_src_index(&_lv_sw_img_ll, 0);
    lv_img_dsc_t *_icon06 = _lv_get_sw_png_src_index(&_lv_sw_img_ll, 1);
    
    lv_obj_t *img1 = lv_img_create(obj);
    lv_img_set_src(img1, _icon05);
    lv_obj_align(img1, LV_ALIGN_LEFT_MID, 0, 0);

	lv_obj_t *img2 = lv_img_create(obj);
    lv_img_set_src(img2, _icon06);
    lv_obj_align(img2, LV_ALIGN_RIGHT_MID, 0, 0);
}