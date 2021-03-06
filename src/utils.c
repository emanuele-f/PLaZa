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

#include "unicode.h"
#include <ncurses.h>
#include "settings.h"
#include "utils.h"
#include "gui.h"

static PLAZA_CHAR _MULTIBYTE_BUFFER[25];

static FILE * open_logfile()
{
    FILE * f;
    if ((f = fopen(PLAZA_LOG_FILE, "a")) == NULL)
        return stderr;
    return f;
}

void LOG_MESSAGE(char * msg)
{
    FILE * f;

    f = open_logfile();
    fprintf(f, "[D] %s\n", msg);
    fclose(f);
}

void FATAL_MESSAGE(char * msg)
{
    FILE * f;

    f = open_logfile();
    fprintf(f, "[E] %s\n", msg);
    fclose(f);

    plazaui_destroy();
    exit(1);
}

/*
 * Like FATAL_MESSAGE, but alsa shows errno info
 */
void FATAL_ERROR(char * msg)
{
    FILE * f;

    f = open_logfile();
    fprintf(f, "[E] %s, errno %i: %s\n", msg, errno, strerror(errno));
    fclose(f);

    plazaui_destroy();
    exit(1);
}

int plaza_get_escaped_key(PLAZA_CHAR * chs)
{
    int i = wcslen((wchar_t *)chs) - 1;

    // Interprets escape sequences
    if (i<0 || chs[0] != KEY_ESCAPE)
        return ERR;

    // Skip escape code
    chs = chs+1;

    if (i==0)
        return KEY_ESCAPE;
    if (i>=11) {
        // Mouse wheel
        if (chs[0]=='[' && chs[2]==KEY_ESCAPE && chs[3]=='[' &&
            chs[5]==KEY_ESCAPE && chs[6]=='[' && chs[8]==KEY_ESCAPE &&
            chs[9]=='[') {
            if (chs[1]=='A' && chs[4]=='A' && chs[7]=='A' && chs[10]=='A')
                return KEY_PAGEUP;
            else if (chs[1]=='B' && chs[4]=='B' && chs[7]=='B' &&
                chs[10]=='B')
                return KEY_PAGEDOWN;
        }
    }
    if (i>=3) {
        if (chs[0]=='[') {
            if (chs[1]=='1')
                // F5-F8
                switch(chs[2]) {
                    case '5': return KEY_F(5);
                    case '7': return KEY_F(6);
                    case '8': return KEY_F(7);
                    case '9': return KEY_F(8);
                }
            else if (chs[1]=='2')
                // F9-F12
                switch (chs[2]) {
                    case '0': return KEY_F(9);
                    case '1': return KEY_F(10);
                    case '2':
                    case '3': return KEY_F(11);
                    case '4': return KEY_F(12);
                }
            else if (chs[1]=='3') {
                if (chs[2]=='~')
                    return KEY_DELETE;
            } else if (chs[1]=='A' && chs[2]==KEY_ESCAPE)
                return KEY_PAGEDOWN;
        }
    }
    if (i>=2) {
        if (chs[0]=='[')
            switch(chs[1]) {
                // Arrows
                case 'A': return KEY_UP;
                case 'B': return KEY_DOWN;
                case 'C': return KEY_RIGHT;
                case 'D': return KEY_LEFT;
                // Page
                case '5': return KEY_PAGEUP;
                case '6': return KEY_PAGEDOWN;
            }
        else if (chs[0]=='O')
            switch(chs[1]) {
                // F1-F4
                case 'P': return KEY_F(1);
                case 'Q': return KEY_F(2);
                case 'R': return KEY_F(3);
                case 'S': return KEY_F(4);
                // Home/End
                case 'H': return KEY_HOME;
                case 'F': return KEY_END;
            }
    }

    return ERR;
}

PLAZA_CHAR * plaza_get_multibyte(WINDOW * w)
{
    // tries to guess the character type (ascii/escape)
    // and reads from stdin acordingly, minimizing wait time.

    //~ halfdelay(1);
    PLAZA_CHAR ch;
    int i=0;
    int maxl=1;

    while(i<maxl && plazach_getch(w, &ch)) {
        _MULTIBYTE_BUFFER[i] = ch;

        if (i==0) {
            if (ch == KEY_ESCAPE) {
                // Escape character
                maxl = lengthof(_MULTIBYTE_BUFFER);
            }
        }

        i++;
    }
    //~ halfdelay(PLAZA_UPDATE_DELAY);

    _MULTIBYTE_BUFFER[i] = '\0';
    return _MULTIBYTE_BUFFER;
}

void plaza_init_colors()
{
    start_color();

    init_pair(PLAZA_PALETTE_TITLE, PLAZA_COLOR_TITLE, PLAZA_COLOR_BG);
    init_pair(PLAZA_PALETTE_SELF, PLAZA_COLOR_SELF, PLAZA_COLOR_BG);
    init_pair(PLAZA_PALETTE_OTHERS, PLAZA_COLOR_OTHERS, PLAZA_COLOR_BG);
    init_pair(PLAZA_PALETTE_TEXT, PLAZA_COLOR_TEXT, PLAZA_COLOR_BG);
    init_pair(PLAZA_PALETTE_CMD, PLAZA_COLOR_CMD, PLAZA_COLOR_BG);
}

void plaza_use_palette(WINDOW * win, int palette, bool bold)
{
    // Abstracts color support
    if(has_colors() == TRUE) {
        wattron(win, COLOR_PAIR(palette));
        if (bold)
            wattron(win, A_BOLD);
    }
}

void plaza_drop_palette(WINDOW * win, int palette)
{
    if(has_colors() == TRUE) {
        wattroff(win, COLOR_PAIR(palette));
        wattroff(win, A_BOLD);
    }
}

bool plaza_scroll_offset(int boxS, int contentS, int * offset, int delta)
{
    /*
     * A function to abstract the content scrolling into a box.
     *
     * @PARAMS
     *      boxS : container size
     *      contentS : content size
     *      offset : current offset
     *      delta : requested displacement
     *
     * @MODIFIES
     *      offset : new offset
     *
     * @RETURNS
     *      True : modified
     *      False : unaffected
     */
    int value;
    int maxoffset;
    int orig;

    if (contentS <= boxS )
        // No offset scroll
        value = 0;
    else {
        // Here (contentS - boxS) > 0
        maxoffset = contentS - boxS;

        if (*offset + delta < 0)
            value = 0;
        else if (*offset + delta >= maxoffset)
            value = maxoffset;
        else
            value = *offset + delta;
    }

    orig = *offset;
    *offset = value;

    #ifdef PLAZA_DEBUGON
        WINDOW * win = PlazaUiInfo.title.win;
        int x,y;
        getyx(win, x, y);
        wmove(win, PlazaUiInfo.title.h-2, 2);
        wprintw(win, "Hei=%3i", boxS);
        wprintw(win, " Lin=%3i", contentS);
        wprintw(win, " Off=%3i", *offset);
        wmove(win, x, y);
        wrefresh(win);
        plazaui_refresh_windows();
    #endif

    if (orig != value)
        return true;
    else
        return false;
}
