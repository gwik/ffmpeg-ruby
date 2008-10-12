task :build do
  %x{cd 'ext' && make clean all && cd '..'}
  $?.success?
end

require 'rubygems'
require 'spec'
require 'spec/rake/spectask'

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