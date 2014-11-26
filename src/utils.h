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
 * utils :: Exposes handy symbols and provides ncurses interface functions
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

#ifndef  __PLAZA_UTILS_H
#define __PLAZA_UTILS_H

#include <ncurses.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define min(x,y) (x < y ? x : y)
#define _STR_HELPER(x) #x
#define STR(x) _STR_HELPER(x)
#define KEY_ESCAPE 0x1B
#undef  KEY_BACKSPACE
#define KEY_BACKSPACE 0x7F
#define KEY_DELETE KEY_DC
#define KEY_RETURN '\n'
#define KEY_PAGEUP KEY_PPAGE
#define KEY_PAGEDOWN KEY_NPAGE

/* PALETTES */
#define PLAZA_PALETTE_TITLE 1
#define PLAZA_PALETTE_SELF 2
#define PLAZA_PALETTE_OTHERS 3
#define PLAZA_PALETTE_TEXT 4
#define PLAZA_PALETTE_CMD 5

#define PLAZA_USE_MSG_PALETTE(palette, bold)\
    plaza_use_palette(PlazaUiInfo.msgwin.win, palette, bold)
#define PLAZA_DROP_MSG_PALETTE(palette)\
    plaza_drop_palette(PlazaUiInfo.msgwin.win, palette)

int plaza_get_escaped_key(WINDOW *);
void plaza_printn(WINDOW *, char *, int);
void plaza_use_palette(WINDOW * win, int palette, bool bold);
void plaza_drop_palette(WINDOW * win, int palette);
void plaza_init_colors();
bool plaza_scroll_offset(int, int, int*, int);
void FATAL_ERROR(char*);
void LOG_MESSAGE(char * msg)

#endif
