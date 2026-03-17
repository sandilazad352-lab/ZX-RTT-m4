/*
*
*  Desc:  decode nalu (sps/pps/slice)
*
*/

#ifndef _H264_NAL_H_
#define _H264_NAL_H_

#include "h264_decoder.h"

int h264_decode_sps(struct h264_dec_ctx *s);

int h264_decode_pps(struct h264_dec_ctx *s);

int h264_decode_slice_header(struct h264_dec_ctx *s);
#endif
