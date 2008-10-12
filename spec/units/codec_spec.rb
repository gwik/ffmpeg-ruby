require File.dirname(__FILE__) + '/../spec_helper'

describe FFMPEG::Codec do
  
  before :each do
    GC.start
    @testfile = File.dirname(__FILE__) + '/../data/alligator.mp4'
    @iformat = InputFormat.new(@testfile)
    @codec = @iformat.first_video_stream.codec
  end
  
  it "has a name" do
    @codec.name == 'mp4'
  end
  
  it "has an id" do
    @codec.id.should == 13
  end
  
  it "has a type" do
    @codec.type.should == :video
    @iformat.streams[1].codec.type.should == :audio
  end
  
end