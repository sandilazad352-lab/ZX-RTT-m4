/*
*
*  Desc: mov tags
*/

#ifndef __MOV_TAGS__
#define __MOV_TAGS__

#include "aic_tag.h"

extern const struct codec_tag mov_audio_tags[];
extern const struct codec_tag mov_video_tags[];
extern const struct codec_tag mp4_obj_type[];

enum CodecID codec_get_id(const struct codec_tag *tags, unsigned int tag);

#endif
