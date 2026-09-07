/*
Implement the program of listing 5.3 and use #define _FILE_OFFSET_BITS 64
to get a 64-bit file (large file)
*/

#define _FILE_OFFSET_BITS 64 // Set bytes in file to 64 bits

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "error_helpers.h"

void err_exit(const char *);
void usage_err(const char *, ...);

void main(int argc, char *argv[])
{
    int fd;
    off_t off;

    if (argc != 3 || strcmp(argv[1], "--help") == 0) {
        usage_err("%s pathname offset\n", argv[0]);
    }

    fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        err_exit("open");
    }

    off = atoll(argv[2]);
    if (lseek(fd, off, SEEK_SET) == -1) {
        err_exit("seek");
    } 

    if (write(fd, "test", 4) == -1) {
        err_exit("write");
    }

    if (close(fd) == -1) {
        err_exit("close");
    }
    exit(EXIT_SUCCESS);
}
