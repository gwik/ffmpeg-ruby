/* Copyright (c)2008 Antonin Amand.
 * Licensed under the Ruby License. See LICENSE for details.
 *
 */
#include "ffmpeg.h"

VALUE rb_mFFMPEG;

void
Init_FFMPEG_core()
{
    //fprintf(stderr, "----------------- init FFMPEG ------------------\n");
    // initialize ffmpeg
    
    av_register_all();
    avcodec_register_all();

    // initialize Module and Class
    rb_mFFMPEG = rb_define_module("FFMPEG");
    rb_define_const(rb_mFFMPEG, "LIBAVCODEC_VERSION", LONG2NUM((long)avcodec_version()));
    
    Init_FFMPEGFormat();
    Init_FFMPEGInputFormat();
    //Init_FFMPEGOutputFormat();
    Init_FFMPEGStream();
    Init_FFMPEGCodec();
    Init_FFMPEGFrame();
}
