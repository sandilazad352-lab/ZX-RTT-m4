/*
 */

#ifndef AIC_DEC_H
#define AIC_DEC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <mpp_list.h>

/* use fake image to fill color */
#define FAKE_IMAGE_DECLARE(name) char fake_##name[128];
#define FAKE_IMAGE_INIT(name, w, h, blend, color) \
                sprintf(fake_##name, "L:/%dx%d_%d_%08x.fake",\
                 w, h, blend, color);
#define FAKE_IMAGE_NAME(name) (fake_##name)

struct framebuf_info {
    unsigned int addr;
    unsigned int align_addr;
    int size;
};

struct framebuf_head {
	struct mpp_list list;
	struct framebuf_info buf_info;
};

void aic_dec_create();

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif //AIC_DEC_H
