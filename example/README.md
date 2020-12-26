# Introduction
This is an example project that uses the Singer library to play and control audio.
# Build
First, you need to install [GStreamer](https://gstreamer.freedesktop.org) and [Singer](https://github.com/isangeles/singer) library.

After that, you can use [Meson](https://mesonbuild.com) to build the project:
```
$ meson build && cd build && ninja
```
# Usage
After successful build run `example` executable:
```
$ ./example
```
