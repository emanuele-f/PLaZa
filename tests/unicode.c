#include "../src/unicode.h"

int main(int argc, char **argv)
{
    PLAZA_CHAR ch;
    PLAZA_CHAR * buf;

    // Locale definition first!
    plazach_init(5);
    initscr();
    cbreak();

    refresh();

    addstr("èèèè "); // implicit unicode
    addstr("\xe2\x9c\x93"); // explicit checkmark unicode
    plazach_newline(stdscr);

    addstr("Singolo carattere: ");
    ch = plazach_getch(stdscr);
    plazach_newline(stdscr);
    addstr("Hai inserito: ");
    plazach_putch(stdscr, ch);
    plazach_newline(stdscr);
    refresh();

    addstr("Intera linea: ");
    buf = plazach_gets(stdscr);
    plazach_newline(stdscr);
    addstr("Hai inserito: ");
    plazach_puts(stdscr, buf);
    plazach_newline(stdscr);
    refresh();

    addstr("Premi un tasto per uscire...");
    plazach_getch(stdscr);

    plazach_destroy();
    endwin();
    return 0;
}
