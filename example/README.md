# Introduction
This is an example project that uses the Singer library to play and control audio.
# Build
First, you need to install [GStreamer](https://gstreamer.freedesktop.org).

After that, you can use [Meson](https://mesonbuild.com) to build the project:
```
$ meson build && cd build && ninja
```
# Usage
After successful build run `example` executable:
```
$ ./example
```