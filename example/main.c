/*
 * main.c
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

#include <stdio.h>
#include <unistd.h>
#include <singer.h>

const char *PATH_SEPARATOR =
#ifdef _WIN32
    "\\";
#else
    "/";
#endif

int main(int argc, char **argv)
{
    /* Init Singer */
    singer_init(argc, argv);
    /* Create audio player */
    Player *player = singer_create_player();
    if (player->err != NULL) {
        fprintf(stderr, "Unable to create audio player: %s\n", player->err);
        return 1;
    }
    /* Set audio file as current track */
    char path[PATH_MAX];
    if (getcwd(path, sizeof(path)) == NULL) {
        fprintf(stderr, "Unable to retrievie current path\n");
        return 1;
    }
    strcat(path, PATH_SEPARATOR);
    strcat(path, "music.mp3");
    singer_set_track(player, path);
    if (player->err != NULL) {
        fprintf(stderr, "Unable to set track: %s\n", player->err);
        return 1;
    }
    /* Play audio */
    singer_play(player);
    if (player->err != NULL) {
        fprintf(stderr, "Unable to play audio: %s\n", player->err);
        return 1;
    }
    /* Control audio */
    while(1) {
        system("clear");
        printf("p - Pause, r - Resume, q - Quit, f - Forward 1 sec., b - Backward 1 sec.\n");
        char input;
        scanf("%c", &input);
        if (input == 'p') {
            singer_pause(player);
            if (player->err != NULL)
                fprintf(stderr, "Unable to pause audio: %s\n", player->err);
        } else if (input == 'r') {
            singer_play(player);
            if (player->err != NULL)
                fprintf(stderr, "Unable to play audio: %s\n", player->err);
        } else if (input == 'f') {
            singer_track_set_pos(player, singer_track_pos(player) + 60000000000);
            if (player->err != NULL)
                fprintf(stderr, "Unable to set track position: %s\n", player->err);
        } else if (input == 'b') {
            singer_track_set_pos(player, singer_track_pos(player) - 60000000000);
            if (player->err != NULL)
                fprintf(stderr, "Unable to set track position: %s\n", player->err);
        } else if (input == 'q')
            break;
    }
    return 0;
}
