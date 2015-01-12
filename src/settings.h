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
 * settings.h :: The right place to tweak the program to your needs
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

#ifndef __PLAZA_SETTINGS_H
#define __PLAZA_SETTINGS_H

#define PLAZA_TITLE_HEIGHT 2
#define PLAZA_CMD_HEIGHT 4
#define PLAZA_OUT_PADDING_X 2
#define PLAZA_OUT_PADDING_Y 1
#define PLAZA_CMD_PADDING_X 2
#define PLAZA_CMD_PADDING_Y 1
#define PLAZA_MSG_MAXLENGTH 512
#define PLAZA_NICK_MAXLENGTH 40
#define PLAZA_SYNC_FILE "plazapool"
#define PLAZA_LOG_FILE "plaza.log"
#define PLAZA_SYNC_LINES 40
#define PLAZA_UPDATE_DELAY 1
#define PLAZA_MOUSE_STEP 3
#define PLAZA_COLOR_BG COLOR_BLACK
#define PLAZA_COLOR_TITLE COLOR_RED
#define PLAZA_COLOR_TEXT COLOR_CYAN
#define PLAZA_COLOR_CMD COLOR_WHITE
#define PLAZA_COLOR_SELF COLOR_GREEN
#define PLAZA_COLOR_OTHERS COLOR_YELLOW
#define PLAZA_MESSAGE_BEEP

//#define PLAZA_DEBUGON

#endif
