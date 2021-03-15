# Introduction
Singer is a simple, easy-to-use audio player library written in C on the top of GStreamer. 
# Build
First, you need to install [GStreamer](https://gstreamer.freedesktop.org).

After that, you can use [Meson](https://mesonbuild.com) to build the library:
```
meson build -Ddefault_library=static
cd build
ninja
```
This command builds a static library.

If you want to build a shared library or both shared and static, change `-Ddefualt_library` argument to `shared` or `both`.

The Meson build also generates a pkg-config file(.pc) that can be found in the `meson-private` sub-directory of the build directory.
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
    player->err = NULL;
}
```
Set audio file:
```
singer_set_track(player, "/path/to/audio/file.ogg");
if (player->err != NULL) {
    printf("Unable to set audio track: %s\n", player->err);
    player->err = NULL;
}
```
Play audio:
```
singer_play(player);
if (player->err != NULL) {
    printf("Unable to play audio: %s\n", player->err);
    player->err = NULL;
}
```
Pause audio:
```
singer_pause(player);
if (player->err != NULL) {
    printf("Unable to pause audio: %s\n", player->err);
    player->err = NULL;
}
```
Check if player is playing audio:
```
if (singer_playing(player)) {
    printf("Player is playing");
}
```
Get current track position in nanoseconds:
```
gint64 pos = singer_track_pos(player);
if (player->err != NULL) {
    printf("Unable to get track position: %s\n", player->err);
    player->err = NULL;
}
printf("Track position: %ldd\n", pos);
```
Get current track lenght in nanoseconds:
```
gint64 len = singer_track_len(player);
if (player->err != NULL) {
    printf("Unable to get track length: %s\n", player-err);
    player->err = NULL;
}
printf("Track lenght: %ldd\n", len);
```
Set current track position in nanoseconds:
```
singer_track_set_pos(player, 60000000000);
if (player->err != NULL) {
    printf("Unable to set track position: %s\n", player->err);
    player->err = NULL;
}
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
Copyright (C) 2020-2021 Dariusz Sikora <<dev@isangeles.pl>>

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
