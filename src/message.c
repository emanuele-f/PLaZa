/*
 * =:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=
 * =                       P L a Z a   c h a t                           =
 * =                                                                     =
 * = A chatting client with ncurses interface for system local users     =
 * =                                                                     =
 * =:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=
 *
 * Emanuele Faranda                                black.silver@hotmail.it
 *
 * =======================================================================
 * message :: Defines message structures and partial protocol implementation
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

#include <malloc.h>
#include <string.h>
#include "settings.h"
#include "message.h"
#include "globals.h"

void plazamsg_init(plaza_message *msg)
{
    msg->_buf = (PLAZA_CHAR *) malloc(plazamsg_maxsize());
    msg->text = msg->_buf + PLAZA_NICK_MAXLENGTH + 2;
}

void plazamsg_destroy(plaza_message *msg)
{
    free(msg->_buf);
    msg->_buf = NULL;
    msg->text = NULL;
}

void plazamsg_clean(plaza_message *msg)
{
    /*
     * Clean the message after a 'finalize' call
     */
    msg->_buf[0] = '\0';
    msg->text[0] = '\0';
}

PLAZA_CHAR * plazamsg_sign(plaza_message *msg)
{
    /*
     * Modifies the message internally to make it easy to be sent.
     * After using the return value, call 'clean' before reusing for new
     * data.
     */
    int text_l = wcslen((wchar_t *) msg->text);
    PLAZA_CHAR * begin = (msg->_buf)+PLAZA_NICK_MAXLENGTH-PlazaUsernick_L;

    wcsncpy((wchar_t *) begin, (wchar_t *) PlazaUsernick, PlazaUsernick_L);
    begin[PlazaUsernick_L] = ':';
    begin[PlazaUsernick_L+1] = ' ';
    msg->text[text_l] = '\n';
    msg->text[text_l+1] = '\0';

    return begin;
}

int plazamsg_maxsize()
{
    // +1 : for message text final \0
    // +1 : for message text final \n
    // +2 : for ': '
    return (PLAZA_NICK_MAXLENGTH + 2 + PLAZA_MSG_MAXLENGTH + 2) *
        sizeof(PLAZA_CHAR);
}
