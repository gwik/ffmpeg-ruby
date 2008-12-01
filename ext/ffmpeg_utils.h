/* Copyright (c)2008 Antonin Amand.
 * Licensed under the Ruby License. See LICENSE for details.
 *
 */
#ifndef RUBY_FFMPEG_UTILS_H__
#define RUBY_FFMPEG_UTILS_H__

#include "ffmpeg.h"

AVFormatContext * get_format_context(VALUE self);
AVStream * get_stream(VALUE self);
AVCodecContext * get_codec_context(VALUE self);
AVFrame * get_frame(VALUE self);

VALUE rb_sym(const char *s);
VALUE codec_type_id_to_sym(int codec_type);

#endif
