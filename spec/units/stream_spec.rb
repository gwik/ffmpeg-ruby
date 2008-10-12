require File.dirname(__FILE__) + '/../spec_helper'

describe FFMPEG::Stream, "attributes" do
  
  before :each do
    GC.start
    @testfile = File.dirname(__FILE__) + '/../data/alligator.mp4'
    @iformat = InputFormat.new(@testfile)
  end
  
  it "has a duration" do
    @iformat.streams.each do |stream|
      stream.duration.to_i.should == 23
    end
  end
  
  it "has the index of its format" do
    @iformat.streams.each_with_index do |stream, index|
      stream.index.should == index
    end
  end
  
  it "has a codec" do
    @iformat.streams.each do |stream|
      stream.codec.should be_kind_of(FFMPEG::Codec)
    end
  end
  
  it "has a frame rate" do
    @iformat.first_video_stream.frame_rate.should == 30
  end
  
end

describe FFMPEG::Stream, "seeking and decoding frame" do
  
  before :each do
    GC.start
    @testfile = File.dirname(__FILE__) + '/../data/alligator.mp4'
    @iformat = InputFormat.new(@testfile)
    @stream = @iformat.first_video_stream
  end
  
  it "decodes a frame" do
    @stream.decode_frame.should be_kind_of(FFMPEG::Frame)
  end
  
  it "has a position" do
    @stream.position.should == 0
  end
  
  it "seeks to a position" do
    @stream.seek(10)
    @stream.position.to_i.should == 10
  end
  
  it "raises when seek to far" do
    lambda do
      @stream.seek(100)
    end.should raise_error(RangeError)
  end
  
  it "decode each frame" do
    
    @stream.seek(10)
    i = 0
    frames = []
    @stream.decode_frame do |frame, pts, dts|
      i += 1
      frames << frame
      frame.should be_kind_of(FFMPEG::Frame)
      break if dts > 12
    end
    
    frames.size.should > 0
  end

  
end