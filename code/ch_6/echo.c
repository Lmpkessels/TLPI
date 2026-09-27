#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error_helpers.h"

// Echo: used to return a text
// if no text provided than a blank is returned
int main(int argc, char *argv[])
{
    if (argc < 2 || strcmp(argv[0], "--help") == 0) {
        usage_err(("%s Pathname", argv[0]));
    }

    for (int i = 0; i < argc; i++) {
        fputs(argv[i], stdout);

        if (i < argc - 1) {
            putchar(' ');
        }
    }

    putchar('\n');
    return EXIT_SUCCESS;
}