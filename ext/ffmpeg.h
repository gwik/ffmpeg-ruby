/* Copyright (c)2008 Antonin Amand.
 * Licensed under the Ruby License. See LICENSE for details.
 *
 */
#ifndef RUBY_FFMPEG_H__
#define RUBY_FFMPEG_H__

#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>

#ifdef RSHIFT
 #undef RSHIFT
#endif

#include "ruby.h"


/* Ruby classes and modules */
RUBY_EXTERN VALUE rb_mFFMPEG;
RUBY_EXTERN VALUE rb_cFFMPEGFormat;
RUBY_EXTERN VALUE rb_cFFMPEGInputFormat;
RUBY_EXTERN VALUE rb_cFFMPEGStream;
RUBY_EXTERN VALUE rb_cFFMPEGCodec;
RUBY_EXTERN VALUE rb_cFFMPEGFrame;

/* ERRORS */
RUBY_EXTERN VALUE rb_eUnsupportedFormat;

RUBY_EXTERN void Init_FFMPEG_core();
RUBY_EXTERN void Init_FFMPEGFormat();
RUBY_EXTERN void Init_FFMPEGInputFormat();
RUBY_EXTERN void Init_FFMPEGOutputFormat();
RUBY_EXTERN void Init_FFMPEGStream();
RUBY_EXTERN void Init_FFMPEGCodec();
RUBY_EXTERN void Init_FFMPEGFrame();

VALUE build_stream_object(AVStream * stream, VALUE format);
VALUE build_codec_object(AVCodecContext * codec);
VALUE build_frame_object(AVFrame * frame, int width, int height, int pixel_format);

#endif
