#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "error_helpers.h"

void err_exit(const char *);
void usage_err(const char *, ...);

// Opens a existing file to which it can append text then seeks the beginning 
// of the file before writing data to it.
//
// The text is added to the end of the file, even tough seeking is performed
// it's because the file is opened with the append flag.
void main(int argc, char *argv[])
{
    int open_fd;
    off_t start_off;

    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        usage_err(("%s pathname\n", argv[0]));
    }

    open_fd = open(argv[1], O_WRONLY | O_APPEND | S_IWUSR);
    if (open_fd == -1) {
        err_exit("open");
    }

    // Seek beginning of file (byte 0)
    start_off = 0;
    if (lseek(open_fd, start_off, SEEK_SET) == -1) {
        err_exit("seek");
    }

    // Write "test" to the file at the offset which
    // remains the EOF
    if (write(open_fd, "test", 4) == -1) {
        err_exit("write");
    }

    if (close(open_fd) == -1) {
        err_exit("close");
    }

    exit(EXIT_SUCCESS);
}