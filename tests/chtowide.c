/*
 * Utility to convert 8 bits char based text files to 32 bits wide char text
 * files.
 */

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include <libgen.h>

int main(int argc, char * argv[])
{
    FILE *fin, *fout;
    char * fname;
    wchar_t ch;
    const char * suffix = "_wchar";
    int l = strlen(suffix);
    int lf;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s filename\n", basename(argv[0]));
        return 1;
    }

    fin = fopen(argv[1], "r");
    if (fin == NULL) {
        perror("Cannot open input file");
        return 1;
    }
    lf = strlen(argv[1]);

    fname = malloc(lf+l+1);
    strcpy(fname, argv[1]);
    strcpy(fname+lf, suffix);
    fout = fopen(fname, "w");
    free(fname);
    if (fout == NULL) {
        perror("Cannot open output file");
        return 1;
    }

    while(! feof(fin)) {
        ch = fgetc(fin);
        if (! feof(fin)) {
            fputwc(ch, fout);
        }
    }

    fclose(fin);
    fclose(fout);
    return 0;
}
