# --with-ffmpeg-dir=/opt/ffmpeg

require 'mkmf'


ffmpeg_include, ffmpeg_lib = dir_config("ffmpeg")
dir_config("libswscale")

$CFLAGS << " -W -Wall"
#$LDFLAGS << " -rpath #{ffmpeg_lib}"

if have_library("avformat") and have_header('libavformat/avformat.h') and
  have_library("avcodec") and have_header('libavutil/avutil.h') and
  have_library("avutil") and have_header('libavcodec/avcodec.h') and
  have_library("swscale") and have_header('libswscale/swscale.h') then

$objs = %w(ffmpeg.o ffmpeg_format.o ffmpeg_input_format.o ffmpeg_stream.o ffmpeg_utils.o ffmpeg_frame.o ffmpeg_codec.o)

create_makefile("FFMPEG_core")

else
  STDERR.puts "missing library"
  exit 1
end
