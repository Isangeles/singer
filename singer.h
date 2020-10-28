/*
 * singer.h
 *
 * Copyright (C) 2020 Dariusz Sikora <dev@isangeles.pl>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 *
 */

#ifndef PLAYER_H
#define PLAYER_H
#include <gst/gst.h>

/* Structure for audio player. */
typedef struct _Player {
    GstElement *pipeline;
    GstElement *source;
    GstElement *convert;
    GstElement *resample;
    GstElement *sink;
    char *err;
} Player;

void singer_init(int argc, char **argv);
Player *singer_create_player();
void singer_close_player();
void singer_set_track(Player *player, const char *path);
void singer_play(Player *player);
void singer_pause(Player *player);
gint64 singer_track_pos(Player *player);
gint64 singer_track_len(Player *player);
#endif
