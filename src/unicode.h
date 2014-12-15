#ifndef __PLAZA_UNICODE_H
#define __PLAZA_UNICODE_H

#define _XOPEN_SOURCE_EXTENDED
#include <ncursesw/ncurses.h>

typedef wchar_t PLAZA_CHAR;

void plazach_newline(WINDOW * w);
void plazach_init(int maxlength);
void plazach_destroy();
PLAZA_CHAR plazach_getch(WINDOW * w);
void plazach_putch(WINDOW * w, PLAZA_CHAR ch);
PLAZA_CHAR * plazach_gets(WINDOW * w);
void plazach_puts(WINDOW * w, PLAZA_CHAR * buf);

#endif
