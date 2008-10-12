/* Copyright (c)2008 Antonin Amand.
 * Licensed under the Ruby License. See LICENSE for details.
 *
 */
#include "ffmpeg.h"
#include "ffmpeg_utils.h"

VALUE rb_cFFMPEGCodec;

static VALUE codec_type(VALUE self) {
    AVCodecContext * codec_context = get_codec_context(self);
    return codec_type_id_to_sym(codec_context->codec_type);
}

static VALUE
codec_open_decoder(VALUE self)
{
    int err = 0;
    AVCodecContext * codec_context = get_codec_context(self);
    AVCodec * codec = avcodec_find_decoder(codec_context->codec_id);
    
    if (NULL == codec)
        rb_raise(rb_eRuntimeError, "not enough memory to open codec");
    
    //fprintf(stderr, "opening codec\n");
    err = avcodec_open(codec_context, codec);
    
    if (err < 0)
        rb_raise(rb_eRuntimeError, "unable to open codec");
    
    return self;
}

static VALUE
codec_open_encoder(VALUE self)
{
    int err = 0;
    AVCodecContext * codec_context = get_codec_context(self);
    AVCodec * codec = avcodec_find_encoder(codec_context->codec_id);
    
    if (NULL == codec)
        rb_raise(rb_eRuntimeError, "not enough memory to open codec");
    
    //fprintf(stderr, "opening codec\n");
    err = avcodec_open(codec_context, codec);
    
    if (err < 0)
        rb_raise(rb_eRuntimeError, "unable to open codec");
    
    return self;
}

static VALUE
codec_name(VALUE self)
{
    AVCodecContext * codec_context = get_codec_context(self);
    
    // open codec if needed
    if (NULL == codec_context->codec)
        codec_open_decoder(self);
    
    return rb_str_new2(codec_context->codec_name);
}

static VALUE
codec_id(VALUE self)
{
    AVCodecContext * codec_context = get_codec_context(self);
    return INT2FIX(codec_context->codec_id);
}

// build a Stream Object from an existing AVStream structure
VALUE build_codec_object(AVCodecContext * codec)
{
    //fprintf(stderr, "build stream\n");
    // will be freed by owner
    VALUE obj = Data_Wrap_Struct(rb_cFFMPEGCodec, 0, 0, codec);
    return obj;
}

void
Init_FFMPEGCodec()
{   
    rb_cFFMPEGCodec = rb_define_class_under(rb_mFFMPEG, "Codec", rb_cObject);
    //rb_define_alloc_func(rb_cFFMPEGCodec, alloc_codec);
    rb_define_method(rb_cFFMPEGCodec, "name", codec_name, 0);
    rb_define_method(rb_cFFMPEGCodec, "id", codec_id, 0);
    rb_define_method(rb_cFFMPEGCodec, "type", codec_type, 0);
}
