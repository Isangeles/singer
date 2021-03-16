project('singer', 'c', version: '0.1.0')
pkgconfig = import('pkgconfig')
threads = dependency('threads')
gstreamer = dependency('gstreamer-1.0')
src = ['singer.h', 'singer.c']
singer = library('singer', sources: src, version: '0.1.0', dependencies : [threads, gstreamer])
pkgconfig.generate(singer)
