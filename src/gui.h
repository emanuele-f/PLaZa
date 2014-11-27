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
 * gui :: Interface builder and mainloop provider
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

#ifndef  __PLAZA_GUI_H
#define __PLAZA_GUI_H

#include <ncurses.h>
#include "globals.h"
#include "settings.h"

typedef struct plazaui_window {
    WINDOW *win;
    int w;
    int h;
} plazaui_window;

#define PLAZAUI_SCROLL_BOTTOM max(PlazaUiInfo.msgwin.h, PLAZA_SYNC_LINES)

typedef struct plazaui_info {
    plazaui_window term;

    plazaui_window title;
    plazaui_window cmdbox;
    plazaui_window cmdwin;
    plazaui_window msgwin;
    plazaui_window msgbox;
} plazaui_info;

void plazaui_init();
void plazaui_destroy();
void plazaui_mainloop();
void plazaui_async_cmd_clear();
void plazaui_refresh_windows();

extern plazaui_info PlazaUiInfo;

#endif
