#include "../src/unicode.h"
#include "../src/utils.h"
#include <wchar.h>

int main(int argc, char **argv)
{
    PLAZA_CHAR ch;
    PLAZA_CHAR buf[7];

    // Locale definition first!
    plazach_unicode_enable();
    initscr();
    cbreak();

    refresh();

    addstr("èèèè "); // implicit unicode
    addstr("\xe2\x9c\x93"); // explicit checkmark unicode
    /*
     * uint_t :
     *
     * Typedef of a type able to represent any value of type wchar_t that is
     * a member of the extended character set, as well as an additional
     * value (not part of that set): WEOF
     */
    printw("\nsizeof(w_char_t)=%i sizeof(wint_t)=%i\n", sizeof(wchar_t), sizeof(wint_t));
    plazach_newline(stdscr);

    addstr("Singolo carattere: ");
    plazach_getch(stdscr, &ch);
    plazach_newline(stdscr);
    addstr("Hai inserito: ");
    plazach_putch(stdscr, ch);
    plazach_newline(stdscr);
    refresh();

    addstr("Intera linea: ");
    plazach_gets(stdscr, buf, lengthof(buf));
    plazach_newline(stdscr);
    addstr("Hai inserito: ");
    plazach_puts(stdscr, buf);
    plazach_newline(stdscr);
    refresh();

    FILE * f;
    f = fopen("unicode_test.txt", "w");
    //~ fwprintf(f, "%s\n", (wchar_t*) buf);
    fputws((wchar_t*) buf, f);
    fputws(L"\n", f);
    fclose(f);

    addstr("Premi un tasto per uscire...");
    plazach_getch(stdscr, &ch);

    endwin();
    return 0;
}
