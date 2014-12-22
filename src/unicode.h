#ifndef __PLAZA_UNICODE_H
#define __PLAZA_UNICODE_H

#define _XOPEN_SOURCE_EXTENDED
#include <ncursesw/ncurses.h>
#include <wchar.h>

typedef wint_t PLAZA_CHAR;

void plazach_unicode_enable();
void plazach_newline(WINDOW * w);
bool plazach_getch(WINDOW * w, PLAZA_CHAR * ch);
bool plazach_putch(WINDOW * w, PLAZA_CHAR ch);
bool plazach_gets(WINDOW * w, PLAZA_CHAR * buf, int maxlength);
bool plazach_puts(WINDOW * w, PLAZA_CHAR * buf);
bool plazach_putns(WINDOW * w, PLAZA_CHAR * buf, int length);

#endif
