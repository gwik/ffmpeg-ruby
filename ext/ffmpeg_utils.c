/* Copyright (c)2008 Antonin Amand.
 * Licensed under the Ruby License. See LICENSE for details.
 *
 */
#include "ffmpeg.h"
#include "ffmpeg_utils.h"

AVFormatContext * get_format_context(VALUE self)
{
    AVFormatContext * format_context = NULL;
    Data_Get_Struct(self, AVFormatContext, format_context);
    if (NULL == format_context) {
        rb_fatal("FFMPEG internal error\n");
    }
    return format_context;
}

AVStream * get_stream(VALUE self)
{
    AVStream * stream = NULL;
    Data_Get_Struct(self, AVStream, stream);
    if (NULL == stream) {
        rb_fatal("FFMPEG internal error\n");
    }
    return stream;
}

AVCodecContext * get_codec_context(VALUE self)
{
    AVCodecContext * codec_context = NULL;
    Data_Get_Struct(self, AVCodecContext, codec_context);
    if (NULL == codec_context) {
        rb_fatal("FFMPEG internal error\n");
    }
    return codec_context;
}

AVFrame * get_frame(VALUE self)
{
     AVFrame * frame = NULL;
    Data_Get_Struct(self, AVFrame, frame);
    if (NULL == frame) {
        rb_fatal("FFMPEG internal error\n");
    }
    return frame;
}

VALUE rb_sym(const char *s) {
    return rb_str_intern(rb_str_new2(s));
}

VALUE codec_type_id_to_sym(int codec_type)
{
    VALUE type_sym;
    switch(codec_type) {
        case CODEC_TYPE_AUDIO:
            type_sym = rb_sym("audio");
            break;
        
        case CODEC_TYPE_VIDEO:
            type_sym = rb_sym("video");
            break;
        
        case CODEC_TYPE_SUBTITLE:
            type_sym = rb_sym("subtitle");
            break;
        
        case CODEC_TYPE_DATA:
            type_sym = rb_sym("data");
            break;
        
        case CODEC_TYPE_ATTACHMENT:
            type_sym = rb_sym("attachment");
            break;
        
        case CODEC_TYPE_NB:        
            type_sym = rb_sym("nb");
            break;

        default:
            type_sym = rb_sym("unknown");
            break;
    }
    
    return type_sym;
}
