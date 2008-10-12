Gem::Specification.new do |spec|
  spec.name = 'ffmpeg-ruby'
  spec.version = '0.1.0'
  spec.summary = 'Ruby C bindings to ffmpeg/av library, especially usefull to extract thumbnails'
  spec.author = "Antonin Amand"
  spec.email = 'antonin.amand@gmail.com'
  spec.files = Dir['ext/*.{rb,c,h}']
  spec.homepage = 'http://github.com/gwik/ffmpeg-ruby'
  spec.has_rdoc = false
  spec.extensions << 'ext/extconf.rb'
  spec.require_path = 'ext'
  spec.date = Time.now
  spec.requirements << 'ffmpeg library patched for ffmpeg-ruby'
end
