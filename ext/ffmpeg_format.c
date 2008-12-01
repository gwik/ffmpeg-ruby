/* Copyright (c)2008 Antonin Amand.
 * Licensed under the Ruby License. See LICENSE for details.
 *
 */
#include "ffmpeg.h"
#include "ffmpeg_utils.h"

VALUE rb_cFFMPEGFormat;

// ############## ATTRIBUTE METHODS ############################

static VALUE
format_filename(VALUE self)
{
    AVFormatContext * format_context = get_format_context(self);
    if (NULL == format_context->filename) {
        return Qnil;
    }
    return rb_str_new2(format_context->filename);
}

static VALUE 
format_bit_rate(VALUE self)
{
    AVFormatContext * format_context = get_format_context(self);
    return INT2NUM(format_context->bit_rate);
}

static VALUE
format_duration(VALUE self)
{
    AVFormatContext * format_context = get_format_context(self);
    
    if (format_context->duration == AV_NOPTS_VALUE) {
        return Qnil;
    }
    
    return rb_float_new(format_context->duration / (double)AV_TIME_BASE);
}

static VALUE
format_streams(VALUE self)
{
    int i;
    AVFormatContext * format_context = get_format_context(self);
    volatile VALUE streams = rb_ary_new2(format_context->nb_streams);
    
    for (i = 0; i < format_context->nb_streams; i++) {
        AVStream * format_context_entry = format_context->streams[i];
        AVStream * streams_entry = NULL;
        VALUE entry = rb_ary_entry(streams, i);
        
        if (entry != Qnil)
            Data_Get_Struct(entry, AVStream, streams_entry);
        
        if (streams_entry != format_context_entry)
            rb_ary_store(streams, i, build_stream_object(format_context_entry, self));
    }
    
    return streams;
}

static VALUE
format_stream_count(VALUE self)
{
    AVFormatContext * format_context = get_format_context(self);
    return INT2FIX(format_context->nb_streams);
}

//TODO : translate codec types to symbols
static VALUE
format_count_stream_with_codec_type(VALUE self, int codec_type)
{
    AVFormatContext * format_context = get_format_context(self);
    int i, count = 0;
    
    for (i = 0; i < format_context->nb_streams; i++) {
        if (format_context->streams[i]->codec->codec_type == codec_type)
            ++count;
    }
    return INT2FIX(count);
}

static VALUE
format_video_stream_count(VALUE self)
{
    return format_count_stream_with_codec_type(self, CODEC_TYPE_VIDEO);
}

static VALUE
format_audio_stream_count(VALUE self)
{
    return format_count_stream_with_codec_type(self, CODEC_TYPE_AUDIO);
}

static VALUE
format_has_stream_with_codec_type(VALUE self, int codec_type)
{
    AVFormatContext * format_context = get_format_context(self);
    int i;
    
    for (i = 0; i < format_context->nb_streams; i++) {
        if (format_context->streams[i]->codec->codec_type == codec_type)
            return Qtrue;
    }
    return Qfalse;
}

static VALUE
format_has_video(VALUE self)
{
    return format_has_stream_with_codec_type(self, CODEC_TYPE_VIDEO);
}

static VALUE
format_has_audio(VALUE self)
{
    return format_has_stream_with_codec_type(self, CODEC_TYPE_AUDIO);
}

// copied from avformat util.c
static VALUE
format_duration_human(VALUE self)
{
    AVFormatContext * format_context = get_format_context(self);
    
    if (format_context->duration == AV_NOPTS_VALUE) {
        return Qnil;
    }
    
    int hours, mins, secs, us;
    char cstr[64] = "";
    
    secs = format_context->duration / AV_TIME_BASE;
    us = format_context->duration % AV_TIME_BASE;
    mins = secs / 60;
    secs %= 60;
    hours = mins / 60;
    mins %= 60;
    sprintf(cstr, "%02d:%02d:%02d.%01d", hours, mins, secs,
           (10 * us) / AV_TIME_BASE);
    
    return rb_str_new2(cstr);
}

// ####################  CONSTRUCT AND DESTROY ###################

static VALUE
init_format(VALUE self)
{
    return self;
}

static void 
free_format(AVFormatContext * format_context)
{
    if (NULL == format_context)
        return;
    
    int i;
    for(i = 0; i < format_context->nb_streams; i++) {
        if (NULL != format_context->streams[i]->codec->codec) {
            avcodec_close(format_context->streams[i]->codec);
        }
    }
    
    if (format_context->iformat) {
        av_close_input_file(format_context);
    } else {
        av_free(format_context);
    }
}

static VALUE
alloc_format(VALUE klass)
{
    //fprintf(stderr, "allocating Format\n");
    AVFormatContext * format_context = av_alloc_format_context();
    VALUE obj;
    format_context->oformat = NULL;
    format_context->iformat = NULL;
    
    obj = Data_Wrap_Struct(klass, 0, free_format, format_context);
    return obj;
}

void
Init_FFMPEGFormat() {
    //fprintf(stderr, "init FFMPEG Format\n");
    rb_cFFMPEGFormat = rb_define_class_under(rb_mFFMPEG, "Format", rb_cObject);
    
    //fprintf(stderr, "init FFMPEG Format allocation\n");
    rb_define_alloc_func(rb_cFFMPEGFormat, alloc_format);
    
    //fprintf(stderr, "init FFMPEG Format initialize\n");
    rb_define_method(rb_cFFMPEGFormat, "initialize", init_format, 0);
    rb_define_method(rb_cFFMPEGFormat, "filename", format_filename, 0);
    rb_define_method(rb_cFFMPEGFormat, "bit_rate", format_bit_rate, 0);
    rb_define_method(rb_cFFMPEGFormat, "duration", format_duration, 0);
    rb_define_method(rb_cFFMPEGFormat, "human_duration", format_duration_human, 0);
    
    rb_define_method(rb_cFFMPEGFormat, "streams", format_streams, 0);
    rb_define_method(rb_cFFMPEGFormat, "video_stream_count", format_video_stream_count, 0);
    rb_define_method(rb_cFFMPEGFormat, "audio_stream_count", format_audio_stream_count, 0);
    rb_define_method(rb_cFFMPEGFormat, "has_stream_with_codec_type?", format_has_stream_with_codec_type, 1);
    rb_define_method(rb_cFFMPEGFormat, "has_video?", format_has_video, 0);
    rb_define_method(rb_cFFMPEGFormat, "has_audio?", format_has_audio, 0);
}
