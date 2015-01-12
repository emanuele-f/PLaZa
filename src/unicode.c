#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <wchar.h>
#include "unicode.h"
#include "utils.h"

/*
 * Call me before ncurses initialization.
 * In order to work properly, a utf based locale is required!
 */
void plazach_unicode_enable()
{
    char * lang;

    lang = getenv("LANG");

    if ( lang == NULL || strstr(lang, "UTF") == NULL )
        // No unicode locale found
        FATAL_MESSAGE("Current locale is no an UTF locale");

    // Initializes LC* variables from LANG
    setlocale(LC_ALL, lang);
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
bool plazach_gets(WINDOW * w, PLAZA_CHAR * buf, int maxlength)
{
    if ( wgetn_wstr(w, buf, maxlength) != OK )
        return false;
    return true;
}

bool plazach_puts(WINDOW * w, PLAZA_CHAR * buf)
{
    if ( waddwstr(w, (wchar_t *) buf) != OK )
        return false;
    return true;
}

bool plazach_putns(WINDOW * w, PLAZA_CHAR * buf, int length)
{
    if ( waddnwstr(w, (wchar_t *) buf, length) != OK )
        return false;
    return true;
}
