project('singer-example', 'c')
configure_file(input: 'music.mp3', output:'music.mp3', copy: true)
threads = dependency('threads')
gstreamer = dependency('gstreamer-1.0')
cc = meson.get_compiler('c')
singer = cc.find_library('singer', dirs: [meson.current_source_dir() + '/singer'])
src = ['main.c']
executable('example', sources: src, dependencies : [threads, gstreamer, singer])
