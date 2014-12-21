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
#include <ncursesw/ncurses.h>
#include "unicode.h"
#include "gui.h"
#include "utils.h"
#include "settings.h"
#include "globals.h"
#include "io.h"
#include "message.h"

plazaui_info PlazaUiInfo;
static bool _PLAZAUI_CMD_MUST_CLEAN = false;
static bool _PLAZAUI_MUST_RESIZE = false;

static void plaza_update_uiinfo()
{
    // Fill in PlazaUiInfo structure
    getmaxyx(stdscr, PlazaUiInfo.term.h, PlazaUiInfo.term.w);
    PlazaUiInfo.cmdbox.h = PLAZA_CMD_HEIGHT;
    PlazaUiInfo.title.h = PLAZA_TITLE_HEIGHT;
    PlazaUiInfo.msgbox.h = PlazaUiInfo.term.h - PLAZA_CMD_HEIGHT -
        PlazaUiInfo.title.h;
    PlazaUiInfo.title.w = PlazaUiInfo.term.w;
    PlazaUiInfo.msgbox.w = PlazaUiInfo.term.w;
    PlazaUiInfo.cmdbox.w = PlazaUiInfo.term.w;

    PlazaUiInfo.msgwin.h = PlazaUiInfo.msgbox.h-PLAZA_OUT_PADDING_Y*2;
    PlazaUiInfo.msgwin.w = PlazaUiInfo.msgbox.w-PLAZA_OUT_PADDING_X*2;
    PlazaUiInfo.cmdwin.h = PlazaUiInfo.cmdbox.h-PLAZA_CMD_PADDING_Y*2;
    PlazaUiInfo.cmdwin.w = PlazaUiInfo.term.w-PLAZA_CMD_PADDING_X*2;
}

static void init_windows_content()
{
    wborder(PlazaUiInfo.title.win, '|', '|', ' ',' ','/','\\','\\','/');
    wmove(PlazaUiInfo.title.win, 1, (PlazaUiInfo.title.w-16)/2);
    plaza_use_palette(PlazaUiInfo.title.win, PLAZA_PALETTE_TITLE, true);
    wprintw(PlazaUiInfo.title.win, "PLaZa chat - 0.I");
    plaza_drop_palette(PlazaUiInfo.title.win, PLAZA_PALETTE_TITLE);
    wmove(PlazaUiInfo.title.win, 0, PlazaUiInfo.title.w-PlazaUsernick_L-2);
    plaza_use_palette(PlazaUiInfo.title.win, PLAZA_PALETTE_SELF, true);
    plazach_puts(PlazaUiInfo.title.win, PlazaUsernick);
    plaza_drop_palette(PlazaUiInfo.title.win, PLAZA_PALETTE_SELF);
    wrefresh(PlazaUiInfo.title.win);

    //~ box(PlazaUiInfo.msgbox.win, 0x0,0x0);
    wborder(PlazaUiInfo.msgbox.win, ';', ';', ':',':','+','+','+','+');
    //~ wborder(PlazaUiInfo.cmdbox.win, '*', '*', '*','*','*','*','*','*');

    box(PlazaUiInfo.cmdbox.win, 0x0,0x0);

    // Refresh with borders
    wrefresh(PlazaUiInfo.msgbox.win);
    wrefresh(PlazaUiInfo.cmdbox.win);
}

static void plaza_create_windows()
{
    // Title window
    PlazaUiInfo.title.win = newwin(
        PlazaUiInfo.title.h, PlazaUiInfo.title.w,
        0, 0);

    // Create output window
    PlazaUiInfo.msgbox.win = newwin(
        PlazaUiInfo.msgbox.h, PlazaUiInfo.msgbox.w,
        PlazaUiInfo.title.h, 0);

    PlazaUiInfo.msgwin.win = derwin(PlazaUiInfo.msgbox.win,
        PlazaUiInfo.msgwin.h,
        PlazaUiInfo.msgwin.w,
        PLAZA_OUT_PADDING_Y, PLAZA_OUT_PADDING_X);

    // Create command window
    PlazaUiInfo.cmdbox.win = newwin(
        PlazaUiInfo.cmdbox.h, PlazaUiInfo.cmdbox.w,
        PlazaUiInfo.msgbox.h+PlazaUiInfo.title.h, 0);

    PlazaUiInfo.cmdwin.win = derwin(PlazaUiInfo.cmdbox.win,
        PlazaUiInfo.cmdwin.h,
        PlazaUiInfo.cmdwin.w,
        PLAZA_CMD_PADDING_Y, PLAZA_CMD_PADDING_X);

    // Set attributes
    scrollok(PlazaUiInfo.msgwin.win, TRUE);
    //~ scrollok(PlazaUiInfo.cmdwin.win, TRUE);
    plaza_use_palette(PlazaUiInfo.cmdwin.win, PLAZA_PALETTE_CMD, true);

    init_windows_content();
}

static void plaza_update_windows()
{
    // From PlazaUiInfo structure
    wresize(PlazaUiInfo.title.win, PlazaUiInfo.title.h,
        PlazaUiInfo.title.w);

    wclear(PlazaUiInfo.msgbox.win);
    wresize(PlazaUiInfo.msgbox.win, PlazaUiInfo.msgbox.h,
        PlazaUiInfo.msgbox.w);
    mvwin(PlazaUiInfo.msgbox.win, PlazaUiInfo.title.h, 0);
    wresize(PlazaUiInfo.msgwin.win, PlazaUiInfo.msgwin.h,
        PlazaUiInfo.msgwin.w);
    mvderwin(PlazaUiInfo.msgwin.win,
        PLAZA_OUT_PADDING_Y, PLAZA_OUT_PADDING_X);

    wclear(PlazaUiInfo.cmdbox.win);
    wresize(PlazaUiInfo.cmdbox.win,
        PlazaUiInfo.cmdbox.h, PlazaUiInfo.cmdbox.w);
    mvwin(PlazaUiInfo.cmdbox.win,
        PlazaUiInfo.msgbox.h+PlazaUiInfo.title.h, 0);

    // Command window must be deleted due to cursor
    delwin(PlazaUiInfo.cmdwin.win);
    PlazaUiInfo.cmdwin.win = derwin(PlazaUiInfo.cmdbox.win,
        PlazaUiInfo.cmdwin.h,
        PlazaUiInfo.cmdwin.w,
        PLAZA_CMD_PADDING_Y, PLAZA_CMD_PADDING_X);
    plaza_use_palette(PlazaUiInfo.cmdwin.win, PLAZA_PALETTE_CMD, true);
    /*
    wresize(PlazaUiInfo.cmdwin.win, PlazaUiInfo.cmdwin.h,
        PlazaUiInfo.cmdwin.w);
    mvderwin(PlazaUiInfo.cmdwin.win,
        PLAZA_CMD_PADDING_Y, PLAZA_CMD_PADDING_X);
    */

    init_windows_content();
}

void plazaui_init()
{
    plazach_unicode_enable();
    initscr();
    noecho();

    // Nonblocking getch: return after PLAZA_UPDATE_DELAY/10 seconds
    halfdelay(PLAZA_UPDATE_DELAY);
    curs_set(true);

    // Colors init where available
    if(has_colors() == TRUE)
        plaza_init_colors();

    plaza_update_uiinfo();
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

void plazaui_async_cmd_clear()
{
    _PLAZAUI_CMD_MUST_CLEAN = true;
}

void plazaui_async_resize()
{
    _PLAZAUI_MUST_RESIZE = true;
}

void plazaui_refresh_windows() {
    wrefresh(PlazaUiInfo.msgwin.win);
    // Mantain focus on cmd
    wrefresh(PlazaUiInfo.cmdwin.win);
}

static int print_message(PLAZA_CHAR * msg)
{
    // Returns written lines
    wchar_t * dots;
    const int maxl = PlazaUiInfo.msgwin.w-(PlazaUsernick_L+2);
    //~ char oldch;
    int lines=0;
    //~ int x, y, i;
    //~ getyx(PlazaUiInfo.msgwin.win, y, i);
    //~ x = PlazaUsernick_L+2;

    dots = wcsstr((wchar_t *) msg, L": ");
    if (dots != NULL) {
        wchar_t * x = wcsstr((wchar_t *)msg, (wchar_t *)PlazaUsernick);
        if ( (x == NULL) || (x != (wchar_t *)msg) ||
         (dots != (wchar_t *)(msg+PlazaUsernick_L)) ) {
            PLAZA_USE_MSG_PALETTE(PLAZA_PALETTE_OTHERS, true);
            plazach_putns(PlazaUiInfo.msgwin.win, msg, dots-(wchar_t *)msg);
            PLAZA_DROP_MSG_PALETTE(PLAZA_PALETTE_OTHERS);
        } else {
            PLAZA_USE_MSG_PALETTE(PLAZA_PALETTE_SELF, true);
            plazach_putns(PlazaUiInfo.msgwin.win, msg, dots-(wchar_t *)msg);
            PLAZA_DROP_MSG_PALETTE(PLAZA_PALETTE_OTHERS);
        }

        PLAZA_USE_MSG_PALETTE(PLAZA_PALETTE_TEXT, false);
        lines = wcslen(dots) / maxl + ((wcslen(dots) % maxl) && 1);
        plazach_puts(PlazaUiInfo.msgwin.win, (PLAZA_CHAR *)dots);

        // This can be used to 'shift' the multiline messages of some space

        //~ for (i=0; i<lines; i++) {
            //~ oldch = dots[maxl];
            //~ dots[maxl] = '\0';
            //~ wprintw(PlazaUiInfo.msgwin.win, dots);
            //~ y++;
            //~ wmove(PlazaUiInfo.msgwin.win, y, x);
            //~ dots[maxl] = oldch;
            //~ dots += maxl;
        //~ }
        //~ if (strlen(dots) % maxl) {
            // Last line
            //~ wprintw(PlazaUiInfo.msgwin.win, dots);
            //~ lines++;
        //~ }

        PLAZA_DROP_MSG_PALETTE(PLAZA_PALETTE_TEXT);
    }

    return lines;
}

static void plaza_show_messages(int delta)
{
    // NB. In order to implement scrolling correctly, we would need to be
    // able to read records in reverse order.

    PLAZA_CHAR * msg;
    int lines;
    static int offset = 0;
    int i;

    lines = plazaio_begin();
    plaza_scroll_offset(PlazaUiInfo.msgwin.h, lines, &offset, delta);
    wclear(PlazaUiInfo.msgwin.win);

    i = -offset;
    while ( (msg=plazaio_next()) != NULL && i<PlazaUiInfo.msgwin.h) {
        if (i >= 0)
            print_message(msg);
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
    plaza_message msg;
    PLAZA_CHAR * chs;
    int ch, chs_l, i;
    int x, y;
    bool run = true;

    plazamsg_init(&msg);
    plaza_show_messages(PLAZAUI_SCROLL_BOTTOM);

    while (run) {
        wmove(PlazaUiInfo.cmdwin.win, 0, 0);
        wclear(PlazaUiInfo.cmdwin.win);

        ch = '\0';
        i = 0;
        while(ch != '\n' && run) {
            if(_PLAZAUI_CMD_MUST_CLEAN) {
                wclear(PlazaUiInfo.cmdwin.win);
                i=0;
                _PLAZAUI_CMD_MUST_CLEAN = false;
            } else if (_PLAZAUI_MUST_RESIZE) {
                // Save current buffer
                mvwinnwstr(PlazaUiInfo.cmdwin.win, 0, 0, msg.text, i);
                endwin();
                initscr();
                refresh();
                plaza_update_uiinfo();
                plaza_update_windows();
                plaza_show_messages(PLAZAUI_SCROLL_BOTTOM);
                // Restore buffer
                waddnwstr(PlazaUiInfo.cmdwin.win, msg.text, i);
                _PLAZAUI_MUST_RESIZE = false;

                // Eat the resize character
                //~ ch = wgetch(PlazaUiInfo.cmdwin.win);
                continue;
            }

            chs = plaza_get_multibyte(PlazaUiInfo.cmdwin.win);
            chs_l = wcslen((wchar_t *)chs);

            if (chs_l == 0) {
                if (plazaio_incoming())
                    plaza_show_messages(PLAZAUI_SCROLL_BOTTOM);
                continue;
            }
            if (chs_l >= 1) {
                ch = chs[0];
                getyx(PlazaUiInfo.cmdwin.win, y, x);

                switch(ch) {
                    case KEY_RETURN:
                        break;
                    case KEY_BACKSPACE:
                        if (i > 0) {
                            if (x==0 && y>0)
                                wmove(PlazaUiInfo.cmdwin.win, y-1,
                                    PlazaUiInfo.msgwin.w-1);
                            else
                                wmove(PlazaUiInfo.cmdwin.win, y, x-1);

                            if ( wdelch(PlazaUiInfo.cmdwin.win) == OK )
                                i--;
                        }
                        break;
                    case KEY_ESCAPE:
                        switch(plaza_get_escaped_key(chs)) {
                            case KEY_UP:
                                //~ wmove(PlazaUiInfo.cmdwin.win, y-1, x);
                                break;
                            case KEY_DOWN:
                                //~ wmove(PlazaUiInfo.cmdwin.win, y+1, x);
                                break;
                            case KEY_RIGHT:
                                //~ wmove(PlazaUiInfo.cmdwin.win, y, x+1);
                                break;
                            case KEY_LEFT:
                                //~ wmove(PlazaUiInfo.cmdwin.win, y, x-1);
                                break;
                            case KEY_HOME:
                                //~ wmove(PlazaUiInfo.cmdwin.win, y, 0);
                                break;
                            case KEY_END:
                                //~ wmove(PlazaUiInfo.cmdwin.win, y, i);
                                break;
                            case KEY_PAGEDOWN:
                                plaza_show_messages(PLAZA_MOUSE_STEP);
                                break;
                            case KEY_PAGEUP:
                                plaza_show_messages(-PLAZA_MOUSE_STEP);
                                break;
                            case KEY_ESCAPE:// '\0':
                                run=false;
                                break;
                        }
                        break;
                    default:
                        if (i+chs_l <= PLAZA_MSG_MAXLENGTH) {
                            plazach_puts(PlazaUiInfo.cmdwin.win, chs);
                            //~ plazaui_refresh_windows();
                            i += chs_l;
                        }
                }
            }
        }

        if (run) {
            mvwinnwstr(PlazaUiInfo.cmdwin.win, 0, 0, msg.text, i);
            plazaio_send_message(&msg);
        }
    }

    plazamsg_destroy(&msg);
}
