/* Copyright (c)2008 Antonin Amand.
 * Licensed under the Ruby License. See LICENSE for details.
 *
 */
#include "ffmpeg.h"
#include "ffmpeg_utils.h"

VALUE rb_cFFMPEGOutputFormat;

static VALUE output_format_add_video_stream(VALUE self, VALUE codec_id_or_sym)
{
    AVFormatContext * format_context = get_format_context(self);
    AVCodecContext * codec_context = NULL;
    AVStream * stream = NULL;
    AVCodec * codec = NULL;
    unsigned int codec_id = CODEC_ID_NONE;
    
    if (SYMBOL_P(codec_id_or_sym)) {
        fprintf(stderr, "get available codecs\n");
        // VALUE rb_h_available_codecs = rb_funcall(rb_const_get(rb_mFFMPEG, rb_intern("Codec")), rb_intern("available_codecs"), 0);
        
        //VALUE rb_h_codec = rb_hash_aref(rb_h_available_codecs, codec_id_or_sym);
        
        if (Qnil == rb_h_codec)
            rb_raise(rb_eArgError, "invalid codec symbol / codec not found in FFMPEG::Codec.available_codecs");
        
        //if (Qfalse == rb_hash_aref(rb_h_codec, rb_sym("encoder")))
        //    rb_raise(rb_eArgError, "codec is not available for encoding purpose");
        fprintf(stderr, "codec %s : %s\n", STR2CSTR(rb_funcall(codec_id_or_sym, rb_intern("inspect"), 0)),
            STR2CSTR(rb_funcall(rb_h_codec, rb_intern("inspect"), 0)));
        

        codec_id = NUM2INT(rb_hash_aref(rb_h_codec, rb_sym("id")));
        fprintf(stderr, "codec id : %d\n", codec_id);
    }
    
    if (FIXNUM_P(codec_id_or_sym)) {
        //fprintf(stderr, "fixnum mode\n");
        codec_id = NUM2INT(codec_id_or_sym);
    }
    
    if (format_context->nb_streams + 1 > MAX_STREAMS)
        rb_raise(rb_eRuntimeError, "over max stream count");
    
    stream = av_new_stream(format_context, format_context->nb_streams);
    
    if (!stream) {
        rb_raise(rb_eRuntimeError, "no memory");
    }
    
    codec_context = stream->codec;
    codec_context->codec_id = codec_id;
    codec_context->codec_type = CODEC_TYPE_VIDEO;
    
    strcat(format_context->filename, "test");
    
    // some formats want stream headers to be separate
    if(!strcmp(format_context->oformat->name, "mp4") || 
            !strcmp(format_context->oformat->name, "mov") || 
            !strcmp(format_context->oformat->name, "3gp"))
        format_context->flags |= CODEC_FLAG_GLOBAL_HEADER;
    
    codec = avcodec_find_encoder(codec_context->codec_id);
    
    if (!codec) {
        rb_raise(rb_eRuntimeError, "codec not found");
    }
    
    avcodec_get_context_defaults2(codec_context, CODEC_TYPE_VIDEO);
    
    if (av_set_parameters(format_context, NULL) < 0) {
        // FIXME raise error
    }
    
    //codec_context->bit_rate = 400000;
    //fprintf(stderr, "bitrate : %d kb/s\n", codec_context->bit_rate / 1024);
    
    //codec_context->width = 320;
    //codec_context->height = 240;
    
    /* resolution must be a multiple of two */
    // codec_context->width = 624;
    // codec_context->height = 352;
    
    codec_context->bit_rate_tolerance = 1000000000;
    //codec_context->gop_size = 12; /* emit one intra frame every twelve frames at most */

    if(codec && codec->pix_fmts){
        const enum PixelFormat *p= codec->pix_fmts;
        for(; *p!=-1; p++){
            if(*p == codec_context->pix_fmt)
                break;
        }
        if(*p == -1)
            codec_context->pix_fmt = codec->pix_fmts[0];
    }
    // if (codecodec_context->supported_framerates)
    //     codec_context->time_base = codecodec_context->supported_framerates[0];
    // else
    //     codec_context->time_base = (AVRational){0,1};
    /* set the output parameters (must be done even if no
       parameters). */
    AVRational fps = (AVRational){25,1};
    codec_context->time_base = fps;
    
    if(codec && codec->supported_framerates){
        const AVRational *p = codec->supported_framerates;
        const AVRational *best = NULL;
        AVRational best_error= (AVRational){INT_MAX, 1};
        for(; p->den!=0; p++) {
            AVRational error= av_sub_q(fps, *p);
            if(error.num < 0) error.num *= -1;
            if(av_cmp_q(error, best_error) < 0) {
                best_error= error;
                best= p;
            }
        }
        codec_context->time_base.den = best->den;
        codec_context->time_base.num = best->num;
    }
    
    fprintf(stderr, "\ncodec name : %s, codec id : %d\n", codec->name, codec->id);
    fprintf(stderr, "codec time base : %d/%d\n", (int)codec_context->time_base.num, (int)codec_context->time_base.den);
    // open the codec
    if (avcodec_open(codec_context, codec) < 0) {
        rb_raise(rb_eRuntimeError, "error while opening codec for encoding (%d)",
            codec_context->codec_id);
    }
    
    return self;
}

static VALUE
output_format_init(VALUE self, VALUE format_string)
{
    VALUE obj = rb_call_super(0, NULL);
    AVFormatContext * format_context = NULL;
    AVOutputFormat * ofmt = NULL;
    
    Data_Get_Struct(obj, AVFormatContext, format_context);
    
    ofmt = guess_format(StringValuePtr(format_string), NULL, NULL);
    
    if (!ofmt) {
        rb_raise(rb_eRuntimeError, "Format '%s' not found or not available",
            StringValuePtr(format_string));
    }
    
    format_context->oformat = ofmt;
    
    return obj;
}

static void 
free_format(AVFormatContext * format_context)
{
    // int i;
    // for (i = 0; i < format_context->nb_streams; i++) {
    //     av_free(format_context->streams[i]->codec);
    //     av_free(format_context->streams[i]);
    // }
    // av_free(format_context);
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

static VALUE
output_format_available_ouput_formats(VALUE klass)
{
    return rb_cv_get(klass, "@@available_output_formats");
}

void Init_FFMPEGOutputFormat() {
    //fprintf(stderr, "init FFMPEG Input Format\n");
    // rb_mFFMPEG = rb_const_get(rb_cObject, rb_intern("FFMPEG"));
    // rb_cFFMPEGFormat = rb_const_get(rb_mFFMPEG, rb_intern("Format"));
    // rb_cFFMPEGOutputFormat = rb_define_class_under(rb_mFFMPEG, "OutputFormat", rb_cFFMPEGFormat);
    // 
    // //instance methods
    // rb_define_method(rb_cFFMPEGOutputFormat, "initialize", output_format_init, 1);
    // rb_define_method(rb_cFFMPEGOutputFormat, "add_video_stream",
    //     output_format_add_video_stream, 1);
    
    //AVOutputFormat * p = first_oformat;
    
    // rb_available_output_formats = rb_hash_new();
    // 
    // while(p) {
    //  VALUE rb_h_format = rb_hash_new();
    //  rb_hash_aset(rb_h_format, rb_sym("name"), rb_str_new2(p->long_name));
    //  if (p->mime_type)
    //      rb_hash_aset(rb_h_format, rb_sym("mime-type"), rb_str_new2(p->mime_type));
    //  if (p->extensions)
    //      rb_hash_aset(rb_h_format, rb_sym("extensions"),
    //          rb_funcall(rb_str_new2(p->extensions), rb_intern("split"), 1, rb_str_new2(",")));
    //  rb_hash_aset(rb_available_output_formats, rb_sym(p->name), rb_h_format);
    //  p = p->next;
    //     }
    // 
    // rb_cv_set(rb_cFFMPEGOutputFormat, "@@available_output_formats", rb_available_output_formats);
    // rb_define_singleton_method(rb_cFFMPEGOutputFormat, "available_output_formats", output_format_available_ouput_formats, 0);
}
