#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "unicode.h"

static int _PLAZACH_MAXLEN = 0;
static wint_t * _PLAZACH_WINT_BUFFER;
static PLAZA_CHAR * _PLAZACH_CHAR_BUFFER;

void plazach_newline(WINDOW * w)
{
    int y, x;

    getyx(w, y, x);
    wmove(w, y+1, 0);
}

void plazach_init(int maxlength)
{
    _PLAZACH_MAXLEN = maxlength;
    maxlength++;
    _PLAZACH_WINT_BUFFER = (wint_t*) malloc(maxlength * sizeof(wint_t));
    _PLAZACH_CHAR_BUFFER = (PLAZA_CHAR*) malloc(maxlength * sizeof(PLAZA_CHAR));
    setlocale(LC_ALL, "");
}

void plazach_destroy()
{
    free(_PLAZACH_WINT_BUFFER);
    free(_PLAZACH_CHAR_BUFFER);
    _PLAZACH_MAXLEN = 0;
}

PLAZA_CHAR plazach_getch(WINDOW * w)
{
    wint_t ch;

    wget_wch(w, &ch);
    return (PLAZA_CHAR)ch;
}

void plazach_putch(WINDOW * w, PLAZA_CHAR ch)
{
    PLAZA_CHAR buf[2];
    buf[0] = ch;
    buf[1] = '\0';
    waddwstr(w, buf);
}

PLAZA_CHAR * plazach_gets(WINDOW * w)
{
    int i;

    wgetn_wstr(w, _PLAZACH_WINT_BUFFER, _PLAZACH_MAXLEN);

    for (i=0; _PLAZACH_WINT_BUFFER[i] != 0; i++) {
        _PLAZACH_CHAR_BUFFER[i] = _PLAZACH_WINT_BUFFER[i];
    }
    _PLAZACH_CHAR_BUFFER[i] = 0;
    return _PLAZACH_CHAR_BUFFER;
}

void plazach_puts(WINDOW * w, PLAZA_CHAR * buf)
{
    waddwstr(w, buf);
}
