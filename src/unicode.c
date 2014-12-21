#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include "unicode.h"

/* Call me before ncurses initialization */
void plazach_unicode_enable()
{
    setlocale(LC_ALL, "");
}

void plazach_newline(WINDOW * w)
{
    int y, x;

    getyx(w, y, x);
    wmove(w, y+1, 0);
}

/*
 * Return value:
 *      true : ok
 *      false : error
 */
bool plazach_getch(WINDOW * w, PLAZA_CHAR * ch)
{
    if ( wget_wch(w, ch) != OK )
        return false;
    return true;
}

bool plazach_putch(WINDOW * w, PLAZA_CHAR ch)
{
    if (ch == WEOF) {
        //~ LOG_MESSAGE("Trying to output a WEOF character, aborting");
        return false;
    }

    wchar_t buf[2];
    buf[0] = ch;
    buf[1] = '\0';
    if ( ! waddwstr(w, buf) )
        return false;
    return true;
}

/*
 * @PARAMETERS
 *      (WINDOW *) w : window
 *      (PLAZA_CHAR *) buf : input buffer
 *      (int) maxlength : maximum number of non-null characters to read
 *
 * @RETURNS
 *      true : Success
 *      false : Unsuccessful read
 */
bool plazach_gets(WINDOW * w, PLAZA_CHAR * buf, int maxlenght)
{
    if ( wgetn_wstr(w, buf, maxlenght) != OK )
        return false;
    return true;
}

bool plazach_puts(WINDOW * w, PLAZA_CHAR * buf)
{
    if ( waddwstr(w, (wchar_t *) buf) != OK )
        return false;
    return true;
}

bool plazach_putns(WINDOW * w, PLAZA_CHAR * buf, int lenght)
{
    if ( waddnwstr(w, (wchar_t *) buf, lenght) != OK )
        return false;
    return true;
}
