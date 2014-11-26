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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ncurses.h>
#include "gui.h"
#include "utils.h"
#include "settings.h"
#include "globals.h"
#include "io.h"
#include "message.h"

plazaui_info PlazaUiInfo;
static bool _PLAZAUI_CMD_MUST_CLEAN = false;

static void plaza_create_windows()
{
    // Title window
    PlazaUiInfo.title.win = newwin(
        PlazaUiInfo.title.h, PlazaUiInfo.title.w,
        0, 0);
    wborder(PlazaUiInfo.title.win, '|', '|', ' ',' ','/','\\','\\','/');
    wmove(PlazaUiInfo.title.win, 1, (PlazaUiInfo.title.w-16)/2);
    plaza_use_palette(PlazaUiInfo.title.win, PLAZA_PALETTE_TITLE, true);
    wprintw(PlazaUiInfo.title.win, "PLaZa chat - 0.I");
    plaza_drop_palette(PlazaUiInfo.title.win, PLAZA_PALETTE_TITLE);
    wmove(PlazaUiInfo.title.win, 0, PlazaUiInfo.title.w-PlazaUsernick_L-2);
    plaza_use_palette(PlazaUiInfo.title.win, PLAZA_PALETTE_SELF, true);
    wprintw(PlazaUiInfo.title.win, PlazaUsernick);
    plaza_drop_palette(PlazaUiInfo.title.win, PLAZA_PALETTE_SELF);
    wrefresh(PlazaUiInfo.title.win);

    // Create output window
    PlazaUiInfo.msgbox.win = newwin(
        PlazaUiInfo.msgbox.h, PlazaUiInfo.msgbox.w,
        PlazaUiInfo.title.h, 0);
    //~ box(PlazaUiInfo.msgbox.win, 0x0,0x0);
    wborder(PlazaUiInfo.msgbox.win, ';', ';', ':',':','+','+','+','+');
    PlazaUiInfo.msgwin.win = derwin(PlazaUiInfo.msgbox.win,
        PlazaUiInfo.msgwin.h,
        PlazaUiInfo.msgwin.w,
        PLAZA_OUT_PADDING_Y, PLAZA_OUT_PADDING_X);
    scrollok(PlazaUiInfo.msgwin.win, TRUE);

    // Create command window
    PlazaUiInfo.cmdbox.win = newwin(
        PlazaUiInfo.cmdbox.h, PlazaUiInfo.cmdbox.w,
        PlazaUiInfo.msgbox.h+PlazaUiInfo.title.h, 0);
    box(PlazaUiInfo.cmdbox.win, 0x0,0x0);
    //~ wborder(PlazaUiInfo.cmdbox.win, '*', '*', '*','*','*','*','*','*');
    PlazaUiInfo.cmdwin.win = derwin(PlazaUiInfo.cmdbox.win,
        PlazaUiInfo.cmdwin.h,
        PlazaUiInfo.cmdwin.w,
        PLAZA_CMD_PADDING_Y, PLAZA_CMD_PADDING_X);
    scrollok(PlazaUiInfo.cmdwin.win, TRUE);
    plaza_use_palette(PlazaUiInfo.cmdwin.win, PLAZA_PALETTE_CMD, true);

    // Refresh with borders
    wrefresh(PlazaUiInfo.msgbox.win);
    wrefresh(PlazaUiInfo.cmdbox.win);
}

void plazaui_init()
{
    initscr();
    noecho();

    // Nonblocking getch: return after PLAZA_UPDATE_DELAY/10 seconds
    halfdelay(PLAZA_UPDATE_DELAY);
    curs_set(true);

    // Colors init where available
    if(has_colors() == TRUE)
        plaza_init_colors();

    // Fill in PlazaUiInfo structure
    getmaxyx(stdscr, PlazaUiInfo.term.h, PlazaUiInfo.term.w);
    PlazaUiInfo.cmdbox.h = PLAZA_CMD_HEIGHT;
    PlazaUiInfo.title.h = PLAZA_TITLE_HEIGHT;
    PlazaUiInfo.msgbox.h = PlazaUiInfo.term.h - PLAZA_CMD_HEIGHT -
        PlazaUiInfo.title.h;
    PlazaUiInfo.msgbox.w = PlazaUiInfo.cmdbox.w = PlazaUiInfo.title.w =
        PlazaUiInfo.term.w;

    PlazaUiInfo.msgwin.h = PlazaUiInfo.msgbox.h-PLAZA_OUT_PADDING_Y*2;
    PlazaUiInfo.msgwin.w = PlazaUiInfo.msgbox.w-PLAZA_OUT_PADDING_X*2;
    PlazaUiInfo.cmdwin.h = PlazaUiInfo.cmdbox.h-PLAZA_CMD_PADDING_Y*2;
    PlazaUiInfo.cmdwin.w = PlazaUiInfo.cmdbox.w-PLAZA_CMD_PADDING_X*2;

    plaza_create_windows();
    plazaio_init();
}

void plazaui_destroy()
{
    plazaio_destroy();
    delwin(PlazaUiInfo.title.win);
    delwin(PlazaUiInfo.msgwin.win);
    delwin(PlazaUiInfo.cmdwin.win);
    delwin(PlazaUiInfo.msgbox.win);
    delwin(PlazaUiInfo.cmdbox.win);
    endwin();
}

void plazaui_async_cmd_clear() {
    _PLAZAUI_CMD_MUST_CLEAN = true;
}

void plazaui_refresh_windows() {
    wrefresh(PlazaUiInfo.msgwin.win);
    // Mantain focus on cmd
    wrefresh(PlazaUiInfo.cmdwin.win);
}

static void print_message(char * msg)
{
    char *dots;

    dots = strstr(msg, ": ");
    if (dots != NULL) {
        if (strstr(msg, PlazaUsernick) == NULL) {
            PLAZA_USE_MSG_PALETTE(PLAZA_PALETTE_OTHERS, true);
            plaza_printn(PlazaUiInfo.msgwin.win, msg, dots-msg);
            PLAZA_DROP_MSG_PALETTE(PLAZA_PALETTE_OTHERS);
        } else {
            PLAZA_USE_MSG_PALETTE(PLAZA_PALETTE_SELF, true);
            plaza_printn(PlazaUiInfo.msgwin.win, msg, dots-msg);
            PLAZA_DROP_MSG_PALETTE(PLAZA_PALETTE_OTHERS);
        }

    }
    PLAZA_USE_MSG_PALETTE(PLAZA_PALETTE_TEXT, false);
    wprintw(PlazaUiInfo.msgwin.win, dots);
    PLAZA_DROP_MSG_PALETTE(PLAZA_PALETTE_TEXT);
}

static void plaza_show_messages(int delta)
{
    char * msg;
    int lines;
    static int offset = 0;
    int i;

    lines = plazaio_begin();
    plaza_scroll_offset(PlazaUiInfo.msgwin.h, lines, &offset, delta);
    wclear(PlazaUiInfo.msgwin.win);

    i = -offset;
    while ( (msg=plazaio_next()) != NULL && i<PlazaUiInfo.msgwin.h) {
        if (i >= 0) //{
            print_message(msg);
            //~ offset += strlen(msg)/PlazaUiInfo.msgwin.w+1;
        //~ }
        //~ else
        i++;
    }

    plazaio_end();
    plazaui_refresh_windows();
}

void plazaui_mainloop()
{
    /*
     *
     * @USES:
     *      PlazaUsernick
     */
    int ch, i;
    bool run = true;
    plaza_message msg;
    int x,y;

    plazamsg_init(&msg);

    // Overflow to scroll bottom
    plaza_show_messages(PlazaUiInfo.msgwin.h);

    while (run) {
        wmove(PlazaUiInfo.cmdwin.win, 0, 0);
        wclear(PlazaUiInfo.cmdwin.win);

        ch = '\0';
        i = 0;
        while(ch != '\n' && run) {
            if(_PLAZAUI_CMD_MUST_CLEAN) {
                wclear(PlazaUiInfo.cmdwin.win);
                wrefresh(PlazaUiInfo.cmdwin.win);
                i=0;
                _PLAZAUI_CMD_MUST_CLEAN = false;
            }

            // This spins at PLAZA_UPDATE_DELAY/10 seconds
            ch = wgetch(PlazaUiInfo.cmdwin.win);

            if (ch == ERR) {
                if (plazaio_incoming())
                    // Overflow to scroll bottom
                    plaza_show_messages(PlazaUiInfo.msgwin.h);
                continue;
            }

            switch(ch) {
                case KEY_RETURN:
                    break;
                case KEY_BACKSPACE:
                    if (i==0)
                        break;
                    i--;
                    msg.text[i] = '\0';
                    wmove(PlazaUiInfo.cmdwin.win, 0, 0);
                    wclear(PlazaUiInfo.cmdwin.win);
                    waddstr(PlazaUiInfo.cmdwin.win, msg.text);
                    break;
                case KEY_ESCAPE:
                    getyx(PlazaUiInfo.cmdwin.win, y, x);

                    switch(plaza_get_escaped_key(PlazaUiInfo.cmdwin.win)) {
                        case KEY_UP:
                            wmove(PlazaUiInfo.cmdwin.win, y-1, x);
                            break;
                        case KEY_DOWN:
                            wmove(PlazaUiInfo.cmdwin.win, y+1, x);
                            break;
                        case KEY_RIGHT:
                            //~ waddch(PlazaUiInfo.cmdwin.win, ' ');
                            wmove(PlazaUiInfo.cmdwin.win, y, x+1);
                            break;
                        case KEY_LEFT:
                            wmove(PlazaUiInfo.cmdwin.win, y, x-1);
                            break;
                        case KEY_HOME:
                            wmove(PlazaUiInfo.cmdwin.win, y, 0);
                            break;
                        case KEY_END:
                            wmove(PlazaUiInfo.cmdwin.win, y, i);
                            break;
                        case KEY_PAGEDOWN:
                            plaza_show_messages(PLAZA_MOUSE_STEP);
                            break;
                        case KEY_PAGEUP:
                            plaza_show_messages(-PLAZA_MOUSE_STEP);
                            break;
                        case KEY_ESCAPE:
                            run=false;
                            break;
                    }
                    break;
                case '\0':
                    break;
                default:
                    if (i <= PLAZA_MAX_MSGSIZE) {// && isprint(ch)) {
                        msg.text[i] = ch;
                        i++;
                        waddch(PlazaUiInfo.cmdwin.win, ch);
                    }
            }
        }

        if (run) {
            msg.text[i] = '\0';
            plazaio_send_message(&msg);
        }
    }

    plazamsg_destroy(&msg);
}
