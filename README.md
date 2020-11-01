# Introduction
Singer is a simple, easy-to-use audio player library written in C on the top of GStreamer. 

Currently in an early development stage.
# Build
First, you need to install [GStreamer](https://gstreamer.freedesktop.org).

After that, you can use [Meson](https://mesonbuild.com) to build the library:
```
$ meson build -Ddefault_library=static && cd build && ninja
```
This command builds a static library.

If you want to build a shared library or both shared and static, change `-Ddefualt_library` argument to `shared` or `both`.
# Usage
Initialize Singer:
```
singer_init(argc, argv);
```
Create audio player:
```
Player *player = singer_create_player();
if (player->err != NULL) {
	printf("Unable to create player: %s\n", player->err);
}
```
Set audio file:
```
singer_set_track(player, "/path/to/audio/file.ogg");
if (player->err != NULL) {
	printf("Unable to set audio track: %s\n", player->err);
}
```
Play audio:
```
singer_play(player);
if (player->err != NULL) {
	printf("Unable to play audio: %s\n", player->err);
}
```
Pause audio:
```
singer_pause(player);
if (player->err != NULL) {
	printf("Unable to pause audio: %s\n", player->err);
}
```
Get current track position in milliseconds:
```
gint64 pos = singer_track_pos(player);
printf("Track position: %ldd\n", pos);
```
Get current track lenght in milliseconds:
```
gint64 len = singer_track_len(player);
printf("Track lenght: %ldd\n", len);
```
# Examples
Check the `example` directory for an example project that uses the Singer library.
# Contributing
You are welcome to contribute to project development.

If you looking for things to do, then check TODO file or contact maintainer(dev@isangeles.pl).

When you find something to do, create a new branch for your feature. After you finish, open a pull request to merge your changes with master branch.
# Contact
* Isangeles <<dev@isangeles.pl>>
# License
Copyright (C) 2020 Dariusz Sikora <<dev@isangeles.pl>>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
