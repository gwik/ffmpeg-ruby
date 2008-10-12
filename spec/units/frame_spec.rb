require File.dirname(__FILE__) + '/../spec_helper'

describe FFMPEG::Frame do
  
  before :each do
    GC.start
    @testfile = File.dirname(__FILE__) + '/../data/alligator.mp4'
    @iformat = InputFormat.new(@testfile)
    @outfile = '/tmp/ffmpeg-test-frame.out'
    @iformat.first_video_stream.seek(9)
    @frame = @iformat.first_video_stream.decode_frame
    
  end
  
  it "has a width" do
    @frame.width.should == 240
  end
  
  it "has a height" do
    @frame.height.should == 176
  end
  
  it "convert to Netpbm PPM \"rawbits\" image data format" do
    File.open(@outfile, 'w') do |f|
      f.write @frame.to_ppm
    end
    
    %x{/usr/bin/env file #{@outfile}}.should match(/ppm/i)
  end
  
end