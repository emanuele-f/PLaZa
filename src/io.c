/*
 * =:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=
 * =                       P L a Z a   c h a t                           =
 * =                                                                     =
 * = A chatting client with ncurses interface for system local users     =
 * =                                                                     =
 * =:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=
 *
 * Emanuele Faranda                               black.silver@hotmail.it
 *
 * =======================================================================
 * io :: A module to handle stream message exange and notifications
 * =======================================================================
 *
 * ------------------------------------------------------------------------
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * ------------------------------------------------------------------------
 */

#include <stdio.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <poll.h>
#include "settings.h"
#include "globals.h"
#include "message.h"
#include "utils.h"
#include "gui.h"

static char * MSG_LINE_BUFFER = NULL;
static int MSG_MAX_LENGTH = -1;
static FILE * MSG_STREAM = NULL;
static int MSG_NOTIFIER = -1;

static int seek_line()
{
    // Read at most last PLAZA_SYNC_LINES
    int lines=-1;
    int i;

    while(!feof(MSG_STREAM)) {
        lines++;
        fgets(MSG_LINE_BUFFER, MSG_MAX_LENGTH, MSG_STREAM);
    }
    fseek(MSG_STREAM, 0, SEEK_SET);

    if (lines > PLAZA_SYNC_LINES) {
        for(i=0; i<lines-PLAZA_SYNC_LINES; i++)
            fgets(MSG_LINE_BUFFER, MSG_MAX_LENGTH, MSG_STREAM);
        return PLAZA_SYNC_LINES;
    }
    return lines;
}

static void plazaio_file_error()
{
    FATAL_ERROR("Cannot open message stream '" PLAZA_SYNC_FILE "'");
}

void plazaio_init()
{
    /*
     * Initializes the library to be ready to handle messages.
     */

    MSG_MAX_LENGTH = plaza_message_maxlength();
    MSG_LINE_BUFFER = malloc(MSG_MAX_LENGTH+1);
    MSG_STREAM = fopen(PLAZA_SYNC_FILE, "a+");
    if (MSG_STREAM == NULL)
        plazaio_file_error();
    fseek(MSG_STREAM, 0, SEEK_SET);
    seek_line();

    MSG_NOTIFIER = inotify_init();
    inotify_add_watch(MSG_NOTIFIER, PLAZA_SYNC_FILE, IN_MODIFY);
}

void plazaio_destroy()
{
    MSG_MAX_LENGTH = -1;
    free(MSG_LINE_BUFFER);
    MSG_LINE_BUFFER = NULL;

    if (MSG_STREAM != NULL) {
        fclose(MSG_STREAM);
        MSG_STREAM = NULL;
    }

    if (MSG_NOTIFIER != -1) {
        close(MSG_NOTIFIER);
        MSG_NOTIFIER = -1;
    }
}

bool plazaio_incoming()
{
    /*
     * Checks for incoming messages.
     * @RETURN
     *      True: new messages available
     *      False: no new messages available
     */
    struct pollfd pfd = {MSG_NOTIFIER, POLLIN, 0};
    struct inotify_event event;

    if ( poll(&pfd, 1, 0)>0 ) {
        read(MSG_NOTIFIER, &event, sizeof(event));
        return true;
    }
    return false;
}

int plazaio_reopen()
{
    // Return number of lines read
    MSG_STREAM = freopen(NULL, "r", MSG_STREAM);
    if (MSG_STREAM == NULL)
        plazaio_file_error();
    return seek_line();
}

char * plazaio_next()
{
    /*
     * Next message line or NULL.
     */
    fgets(MSG_LINE_BUFFER, MSG_MAX_LENGTH, MSG_STREAM);

    if (feof(MSG_STREAM))
        return NULL;
    return MSG_LINE_BUFFER;
}

void plazaio_send_message(plaza_message *msg)
{
    /*
     * Sends message data. After this call, msg data is no more available!
     */

    char *data;

    data = plazamsg_sign(msg);

    MSG_STREAM = freopen(NULL, "a", MSG_STREAM);
    if (MSG_STREAM == NULL)
        plazaio_file_error();

    if ( fprintf(MSG_STREAM, "%s", data) == 0)
        FATAL_ERROR("Cannot write message");

    plazaio_reopen();
    plazamsg_clean(msg);
}
