require File.dirname(__FILE__) + '/../spec_helper'

describe InputFormat, "loading files" do
  
  before :each do
    GC.start
    @testfile = File.dirname(__FILE__) + '/../data/alligator.mp4'
  end
  
  it "does not raise when opening a real supported file" do
    lambda do
      InputFormat.new(@testfile)
    end.should_not raise_error
  end
  
  it "raise a custom exception if format is not supported" do
    lambda do
      InputFormat.new(File.dirname(__FILE__) + '/../data/corrupt.avi')
    end.should raise_error(UnsupportedFormat)
  end
  
  it "raises argument error when file does not exists" do
    lambda do
      InputFormat.new('/thisfiledoesnotexists.mp4')
    end.should raise_error(ArgumentError)
  end
  
end

describe InputFormat, "querying information" do
  
  before :each do
    GC.start
    @testfile = File.dirname(__FILE__) + '/../data/alligator.mp4'
    @iformat = InputFormat.new(@testfile)
  end
  
  it "has a filename" do
    @iformat.filename.should == @testfile
  end
  
  it "has a bitrate" do
    (@iformat.bit_rate / 1000).should == 314
  end
  
  it "has a duration in seconds" do
    @iformat.duration.should == 23.2
  end
  
  it "has an human duration" do
    @iformat.human_duration.should == "00:00:23.2"
  end
  
end

describe InputFormat, "with streams" do

  before :each do
    GC.start
    @testfile = File.dirname(__FILE__) + '/../data/alligator.mp4'
    @iformat = InputFormat.new(@testfile)
  end
  
  it "has streams" do
    @iformat.streams.each do |stream| 
      stream.should be_kind_of(Stream)
    end
  end
  
  it "detects video streams" do
    @iformat.has_video?.should be(true)
  end
  
  it "detects audio streams" do
    @iformat.has_audio?.should be(true)
  end
  
  it "counts its video streams" do
    @iformat.video_stream_count.should == 1
  end
  
  it "counts its audio streams" do
    @iformat.audio_stream_count.should == 1
  end
  
  it "has a first video stream" do
    @iformat.first_video_stream.index.should == 0
  end
  
  it "has a first audio stream" do
    @iformat.first_audio_stream.index.should == 1
  end
  
end