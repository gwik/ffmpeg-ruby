require 'rubygems'
require 'ffmpeg'
require 'RMagick'

video = FFMPEG::InputFormat.new('spec/data/alligator.mp4')
stream = video.first_video_stream
stream.seek 12

i = 0
image_list = Magick::ImageList.new

# pts is presentation timestamp
# dts is decoding timestamp
stream.decode_frame do |frame, pts, dts|
  i += 1
  # stop when decoding timestamp (~position) reach 18
  break if dts > 18
  # decode 1 frame for 5
  next unless i % 5 == 0
  image_list.from_blob(frame.to_ppm)
end

puts i 
image_list.delay = 20
image_list.write('animated.gif')
