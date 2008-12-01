require 'rubygems'
require 'spec'
require 'spec/rake/spectask'

# Make tasks (originally from curb) -----------------------------------------------------
MAKECMD = ENV['MAKE_CMD'] || 'make'
MAKEOPTS = ENV['MAKE_OPTS'] || ''

FFMPEG_SO = "ext/FFMPEG_core.#{Config::MAKEFILE_CONFIG['DLEXT']}"

file 'ext/Makefile' => 'ext/extconf.rb' do
  Dir.chdir('ext') do
    ruby "extconf.rb #{ENV['EXTCONF_OPTS']}"
  end
end

def make(target = '')
  Dir.chdir('ext') do 
    pid = system("#{MAKECMD} #{MAKEOPTS} #{target}")
    $?.exitstatus
  end    
end
# Let make handle dependencies between c/o/so - we'll just run it. 
file FFMPEG_SO => (['ext/Makefile'] + Dir['ext/*.c'] + Dir['ext/*.h']) do
  m = make
  fail "Make failed (status #{m})" unless m == 0
end

desc "Compile the shared object"
task :compile => [FFMPEG_SO]

task :build => :compile do
  %x{cd 'ext' && make clean all && cd '..'}
  $?.success?
end

task :default => :build

desc "see spec:spec"
task :spec => 'spec:spec'

namespace :spec do
  desc "Print Specdoc for all specs"
  Spec::Rake::SpecTask.new(:doc) do |t|
    t.spec_opts = ["--format", "specdoc", "--dry-run"]
    t.spec_files = FileList['spec/**/*_spec.rb']
  end

  desc "run specs"
  Spec::Rake::SpecTask.new(:spec) do |t|
    t.spec_files = FileList["spec/**/*_spec.rb"]
  end
end
