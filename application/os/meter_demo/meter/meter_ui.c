#include <stdio.h>
#include <stdlib.h>
#include "meter_ui.h"
#include "cpu_usage.h"
#include "mpp_types.h"
#include "aic_core.h"


lv_ll_t _lv_sw_img_ll;

void _lv_sw_png_init(lv_ll_t *_lv_sw_img_ll)
{
    _lv_ll_init(_lv_sw_img_ll, sizeof(lv_ll_img_dsc_t));
}

void _lv_sw_png_src_add(lv_ll_t *_lv_sw_img_ll, char *src)
{
	lv_ll_img_dsc_t * img_ll_dsc = NULL;
	lv_ll_img_dsc_t * img_ll_head;
    lv_img_decoder_dsc_t dsc;
    lv_color_t *src_data;
    lv_area_t argb_area;

    img_ll_head = _lv_ll_get_head(_lv_sw_img_ll);
    img_ll_dsc = _lv_ll_ins_head(_lv_sw_img_ll);

    if(img_ll_dsc == NULL) return NULL;
    lv_memset_00(img_ll_dsc, sizeof(lv_ll_img_dsc_t));

    lv_res_t open_res = lv_img_decoder_open(&dsc, src, lv_color_black(), NULL);
    extern lv_res_t decoder_open(lv_img_decoder_t * dec, lv_img_decoder_dsc_t * dsc);
    // decoder_open(NULL, &dsc);

    // extern lv_res_t aic_decoder_open(lv_img_decoder_t *decoder, lv_img_decoder_dsc_t *dsc);
    // aic_decoder_open(NULL, &dsc);

    if(open_res == LV_RES_INV) {
        LV_LOG_WARN("Image draw cannot open the image resource");
        return NULL;
    }
    if(dsc.error_msg != NULL) {
        LV_LOG_WARN("Image draw error");
    }
    src_data = (lv_color_t *)rt_malloc(sizeof(lv_color_t) * dsc.header.w * dsc.header.h);
	memset(src_data, 0, sizeof(lv_color_t) * dsc.header.w * dsc.header.h);


    memcpy((unsigned char *)src_data, dsc.img_data, dsc.header.w * dsc.header.h * sizeof(lv_color_t));

    printf("dsc.header.w = %d dsc.header.h = %d\n", dsc.header.w, dsc.header.h);
    printf("color = %d dsc.header.cf = %d\n", sizeof(lv_color_t), dsc.header.cf);
    printf("src = %s\n", src);
    

	if(img_ll_head != NULL)
		img_ll_dsc->index = img_ll_head->index + 1;
	else
		img_ll_dsc->index = 0;

	img_ll_dsc->img_dsc.data = src_data;
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
void _lv_sw_png_src_free(lv_ll_t *_lv_sw_img_ll)
{
    lv_ll_img_dsc_t * img_ll;

    img_ll = _lv_ll_get_head(_lv_sw_img_ll);
    while(img_ll != NULL) {
        free((void *)(img_ll->img_dsc.data));
        img_ll = _lv_ll_get_next(_lv_sw_img_ll, img_ll);
    }

    _lv_ll_clear(_lv_sw_img_ll);
}

lv_img_dsc_t *_lv_get_sw_png_src_index(lv_ll_t *_lv_sw_img_ll, int index)
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

static lv_obj_t *bg_cpu;
static lv_obj_t *bg_fps;

static void fps_callback(lv_timer_t *tmr)
{
    float value;

    (void)tmr;

    extern int fbdev_draw_fps();
    /* frame rate */
    lv_label_set_text_fmt(bg_fps, "%2d FPS", fbdev_draw_fps());

    /* cpu usage */
#ifdef LPKG_USING_CPU_USAGE
#include "cpu_usage.h"
    value = cpu_load_average();
#else
    value = 0;
#endif

    lv_label_set_text_fmt(bg_cpu, "%d%% CPU", (int)value);

    return;
}

static void sysinfo_disp_create(void)
{
    bg_cpu = lv_label_create(lv_layer_top());
    lv_obj_set_width(bg_cpu, LV_SIZE_CONTENT);
    lv_obj_set_height(bg_cpu, LV_SIZE_CONTENT);
    lv_obj_align(bg_cpu, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_obj_set_pos(bg_cpu, -20, 10);
    lv_label_set_text(bg_cpu, "");
    lv_obj_set_style_text_color(bg_cpu, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(bg_cpu, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(bg_cpu, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    bg_fps = lv_label_create(lv_layer_top());
    lv_obj_set_width(bg_fps, LV_SIZE_CONTENT);
    lv_obj_set_height(bg_fps, LV_SIZE_CONTENT);
    lv_obj_align(bg_fps, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_obj_set_pos(bg_fps, -20, 30);
    lv_label_set_text(bg_fps, "");
    lv_obj_set_style_text_color(bg_fps, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(bg_fps, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(bg_fps, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
}


void meter_ui_init(void)
{
    _lv_sw_png_init(&_lv_sw_img_ll);

    // lv_obj_t *src = lv_obj_create(NULL);

    // lv_scr_load(src);
    // lv_obj_set_size(src, 1024, 600);
    // lv_obj_set_style_bg_color(src, lv_color_black(), 0);
    // lv_obj_set_style_bg_opa(src, 255, 0);
    // lv_obj_set_style_pad_all(src, 0, 0);
    // lv_obj_clear_flag(src, LV_OBJ_FLAG_SCROLLABLE);

    meter_sec_ui_init(lv_scr_act());

    // sysinfo_disp_create();
    // lv_timer_create(fps_callback, 1000, 0);
}
