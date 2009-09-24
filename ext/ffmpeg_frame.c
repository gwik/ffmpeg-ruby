/* Copyright (c)2008 Antonin Amand.
 * Licensed under the Ruby License. See LICENSE for details.
 *
 */
#include "ffmpeg.h"
#include "ffmpeg_utils.h"

VALUE rb_cFFMPEGFrame;

static AVFrame *
alloc_picture(int pix_fmt, int width, int height)
{
    AVFrame *picture;
    uint8_t *picture_buf;
    int size;
    
    picture = avcodec_alloc_frame();
    if (!picture)
        return NULL;
    size = avpicture_get_size(pix_fmt, width, height);
    picture_buf = av_malloc(size);
    if (!picture_buf) {
        av_free(picture);
        return NULL;
    }
    avpicture_fill((AVPicture *)picture, picture_buf,
        pix_fmt, width, height);
    return picture;
}

static VALUE
frame_to_rgb24(VALUE self)
{
    int width = NUM2INT(rb_iv_get(self, "@width"));
    int height = NUM2INT(rb_iv_get(self, "@height"));
    int pixel_format = NUM2INT(rb_iv_get(self, "@pixel_format"));
    
    struct SwsContext *img_convert_ctx = NULL;
    img_convert_ctx = sws_getContext(width, height, pixel_format,
        width, height, PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);
    
    AVFrame * from = get_frame(self);
    AVFrame * to = alloc_picture(PIX_FMT_RGB24, width, height);
    
    sws_scale(img_convert_ctx, from->data, from->linesize,
        0, height, to->data, to->linesize);
    
    av_free(img_convert_ctx);
    
    return build_frame_object(to, width, height, PIX_FMT_RGB24);
}

static VALUE
frame_to_ppm(VALUE self)
{
    VALUE rb_frame = frame_to_rgb24(self);
    AVFrame * frame = get_frame(rb_frame);
    
    int width = NUM2INT(rb_iv_get(self, "@width"));
    int height = NUM2INT(rb_iv_get(self, "@height"));
    
    char header[255];
    sprintf(header, "P6\n%d %d\n255\n", width, height);
    
    int size = strlen(header) + frame->linesize[0] * height;
    char * data_string = malloc(size);
    strcpy(data_string, header);
    
    memcpy(data_string + strlen(header), frame->data[0], frame->linesize[0] * height);
    
    return rb_str_new(data_string, size);
}

static void
free_frame(AVFrame * frame)
{
    //fprintf(stderr, "will free frame\n");
    av_free(frame);
}

static VALUE
alloc_frame(VALUE klass)
{
    AVFrame * frame = avcodec_alloc_frame();
    VALUE obj;
    obj = Data_Wrap_Struct(klass, 0, free_frame, frame);
    return obj;
}

static VALUE
frame_initialize(VALUE self, VALUE width, VALUE height, VALUE pixel_format)
{
    //fprintf(stderr, "new frame : %dx%d, pix:%d\n", NUM2INT(width), NUM2INT(height), NUM2INT(pixel_format));
    rb_iv_set(self, "@width", width);
    rb_iv_set(self, "@height", height);
    rb_iv_set(self, "@pixel_format", pixel_format);
    return self;
}

VALUE
build_frame_object(AVFrame * frame, int width, int height, int pixel_format)
{
    VALUE obj = Data_Wrap_Struct(rb_cFFMPEGFrame, 0, free_frame, frame);
    
    return frame_initialize(obj,
        INT2FIX(width),
        INT2FIX(height),
        INT2FIX(pixel_format));
}

void
Init_FFMPEGFrame() {
    rb_cFFMPEGFrame = rb_define_class_under(rb_mFFMPEG, "Frame", rb_cObject);
    
    rb_define_alloc_func(rb_cFFMPEGFrame, alloc_frame);
    rb_define_method(rb_cFFMPEGFrame, "initialize", frame_initialize, 3);
    
    rb_funcall(rb_cFFMPEGFrame, rb_intern("attr_reader"), 1, rb_sym("width"));
    rb_funcall(rb_cFFMPEGFrame, rb_intern("attr_reader"), 1, rb_sym("height"));
    rb_funcall(rb_cFFMPEGFrame, rb_intern("attr_reader"), 1, rb_sym("pixel_format"));
    
    rb_define_method(rb_cFFMPEGFrame, "to_rgb24", frame_to_rgb24, 0);
    rb_define_method(rb_cFFMPEGFrame, "to_ppm", frame_to_ppm, 0);
}
