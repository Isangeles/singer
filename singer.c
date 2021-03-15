/*
 * singer.c
 *
 * Copyright (C) 2020-2021 Dariusz Sikora <dev@isangeles.pl>
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

#include "singer.h"
#include <stdio.h>
#include <pthread.h>

static struct _Player PLAYER_NEW = {.pipeline = NULL, .source = NULL, .convert = NULL,
           .resample = NULL, .sink = NULL, .err = "Not initialized"
};
/* Handler for the pad-added signal. */
static void pad_added_handler(GstElement *src, GstPad *pad, Player *player);
void singer_listen(Player *player);
void singer_background_listen(void *player);

/* Initializes singer library. */
void singer_init(int argc, char **argv)
{
    /* Init gstreamer */
    gst_init(&argc, &argv);
}

/* Creates new audio player.
 * In case of an error, err variable in returned Player struct
 * is not null. */
Player *singer_create_player()
{
    /* Create the elements */
    Player *player = &PLAYER_NEW;
    player->source = gst_element_factory_make("uridecodebin", "source");
    player->convert = gst_element_factory_make("audioconvert", "convert");
    player->resample = gst_element_factory_make("audioresample", "resample");
    player->sink = gst_element_factory_make("autoaudiosink", "sink");
    /* Create the empty pipeline */
    player->pipeline = gst_pipeline_new("test-pipeline");
    if (!player->pipeline || !player->source || !player->convert ||
            !player->resample || !player->sink) {
        player->err = "Not all elements could be created.";
        return player;
    }
    /* Build the pipeline. */
    gst_bin_add_many(GST_BIN(player->pipeline), player->source, player->convert,
                     player->resample, player->sink, NULL);
    if (!gst_element_link_many(player->convert, player->resample, player->sink,
                               NULL)) {
        gst_object_unref(player->pipeline);
        player->err = "Elements could not be linked.";
        return player;
    }
    /* Connect to the pad-added signal */
    g_signal_connect(player->source, "pad-added", G_CALLBACK(pad_added_handler),
                     player);
    /* Create background thread for player listener */
    pthread_t thread;
    int err = pthread_create(&thread, NULL, singer_background_listen,
                             (void *)player);
    if (err) {
        sprintf(player->err, "Unable to create thread for player listener: %d", err);
        return player;
    }
    player->err = NULL;
    return player;
}

/* Closes specified audio player. */
void singer_close_player(Player *player)
{
    gst_element_set_state(player->pipeline, GST_STATE_NULL);
    gst_object_unref(player->pipeline);
}

/* Function for player listener background thread. */
void singer_background_listen(void *player)
{
    singer_listen(player);
}

/* Listens to the audio player bus and handles messages and
 * state changes. */
void singer_listen(Player *player)
{
    GstBus *bus;
    GstMessage *msg;
    gboolean terminate = FALSE;

    /* Listen to the bus */
    bus = gst_element_get_bus(player->pipeline);
    do {
        msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE,
                                         GST_MESSAGE_STATE_CHANGED |
                                         GST_MESSAGE_ERROR | GST_MESSAGE_EOS);
        /* Parse message */
        if (msg != NULL) {
            GError *err;
            gchar *debug_info;

            switch (GST_MESSAGE_TYPE(msg)) {
            case GST_MESSAGE_ERROR:
                gst_message_parse_error(msg, &err, &debug_info);
                g_printerr("Error received from element %s: %s\n",
                           GST_OBJECT_NAME(msg->src), err->message);
                g_printerr("Debugging information: %s\n",
                           debug_info ? debug_info : "none");
                g_clear_error(&err);
                g_free(debug_info);
                terminate = TRUE;
                break;
            case GST_MESSAGE_EOS:
                terminate = TRUE;
                break;
            case GST_MESSAGE_STATE_CHANGED:
                break;
            default:
                /* We should not reach here */
                g_printerr("Unexpected message received.\n");
                break;
            }
            gst_message_unref(msg);
        }
    } while (!terminate);
    gst_object_unref(bus);
    singer_close_player(player);
    return;
}

/* Sets audio file from specified path as current track. */
void singer_set_track(Player *player, const char *path)
{
    GstStateChangeReturn ret = gst_element_set_state(player->pipeline, GST_STATE_NULL);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        player->err = "Unable to set the pipeline to null state";
        return;
    }
    char uri[7 + strlen(path)];
    sprintf(uri, "file://%s", path);
    g_object_set(player->source, "uri", uri, NULL);
}

/* Starts playing audio file from specified path. */
void singer_play(Player *player)
{
    GstStateChangeReturn ret = gst_element_set_state(player->pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        player->err = "Unable to set the pipeline to the playing state";
    }
}

/* Pauses audio player */
void singer_pause(Player *player)
{
    GstStateChangeReturn ret = gst_element_set_state(player->pipeline, GST_STATE_PAUSED);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        player->err = "Unable to set the pipeline to the paused state";
    }
}

/* Sets specified position in nanoseconds for current track. */
void singer_track_set_pos(Player *player, long pos)
{
    if (!gst_element_seek (player->pipeline, 1.0, GST_FORMAT_TIME,
                           GST_SEEK_FLAG_FLUSH, GST_SEEK_TYPE_SET, (gint64)pos,
                           GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE)) {
        player->err = "Unable to seek pipeline position";
    }
}

/* Returns player position in nanoseconds for current track. */
long singer_track_pos(Player *player)
{
    gint64 pos;
    if (!gst_element_query_position(player->pipeline, GST_FORMAT_TIME, &pos))
        player->err = "Unable to query pipeline position";
    return (long)pos;
}

/* Returns lenght in nanoseconds for current track. */
long singer_track_len(Player *player)
{
    gint64 len;
    if (!gst_element_query_duration(player->pipeline, GST_FORMAT_TIME, &len))
        player->err = "Unable to query pipeline duration";
    return (long)len;
}

/* Chcecks if player is in playing state. */
bool singer_playing(Player *player)
{
    GstState state;
    gst_element_get_state(player->pipeline, &state, NULL, GST_CLOCK_TIME_NONE);
    return state == GST_STATE_PLAYING;
}

/* This function will be called by the pad-added signal. */
static void pad_added_handler(GstElement *src, GstPad *new_pad,
                              Player *player)
{
    GstPad *sink_pad = gst_element_get_static_pad(player->convert, "sink");
    GstPadLinkReturn ret;
    GstCaps *new_pad_caps = NULL;
    GstStructure *new_pad_struct = NULL;
    const gchar *new_pad_type = NULL;

    /* If our converter is already linked, we have nothing to do here */
    if (gst_pad_is_linked(sink_pad)) {
        g_print("We are already linked. Ignoring.\n");
        goto exit;
    }

    /* Check the new pad's type */
    new_pad_caps = gst_pad_get_current_caps(new_pad);
    new_pad_struct = gst_caps_get_structure(new_pad_caps, 0);
    new_pad_type = gst_structure_get_name(new_pad_struct);
    if (!g_str_has_prefix(new_pad_type, "audio/x-raw")) {
        g_print("It has type '%s' which is not raw audio. Ignoring.\n",
                new_pad_type);
        goto exit;
    }

    /* Attempt the link */
    ret = gst_pad_link(new_pad, sink_pad);
    if (GST_PAD_LINK_FAILED(ret)) {
        g_print("Type is '%s' but link failed.\n", new_pad_type);
    }

exit:
    /* Unreference the new pad's caps, if we got them */
    if (new_pad_caps != NULL)
        gst_caps_unref(new_pad_caps);

    /* Unreference the sink pad */
    gst_object_unref(sink_pad);
}
