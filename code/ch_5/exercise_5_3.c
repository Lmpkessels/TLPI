#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <error_helpers.h>

void err_exit(const char *);
void usage_err(const char *, ...);

// Void is incorrect
void main(int argc, char *argv[])
{
    int open_fd;
    bool x_used;
    off_t off;
    long num_bytes;

    // Argc is incorrect
    if (argc <= 2 || strcmp(argv[1], "--help") == 0) {
        usage_err(("%s Pathname", argv[1]));
    }

    // Here i should use strcmp instead because argv[3] is a pointer
    x_used = ((argv[3] == "x") ? (true) : (false));
    if (x_used) {
        open_fd = open(argv[1], O_WRONLY | O_CREAT | S_IRUSR | S_IWUSR);
        if (open_fd == -1) {
            err_exit("open");
        }
    } else {
        open_fd = open(argv[1], O_WRONLY | O_CREAT | O_APPEND | S_IRUSR | S_IWUSR);
        if (open_fd == -1) {
            err_exit("open");
        }
    }


    num_bytes = atoll(argv[2]);
    off = 0;
    for (long i = 0; i < num_bytes; i++) {
        if (x_used) {
            if (lseek(open_fd, off, SEEK_END) == -1) {
                err_exit("seek");
            }
        }

        // Number of bytes are written instead of 1 byte at a time
        if (write(open_fd, "x", num_bytes) == -1) {
            err_exit("write");
        }
    }

    if (close(open_fd) == -1) {
        err_exit("close");
    }
 
    exit(EXIT_SUCCESS);
}