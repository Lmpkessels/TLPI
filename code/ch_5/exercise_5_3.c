#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <error_helpers.h>

void err_exit(const char *);
void usage_err(const char *, ...);

// Writes nothing when x is used, appends text when x is not used.
int main(int argc, char *argv[])
{
    int open_fd;
    bool x_used;
    off_t off;
    long num_bytes;

    // Check for argc not being 3 and 4 which is a usage error
    if (argc != 3 && argc != 4 || strcmp(argv[1], "--help") == 0) {
        usage_err(("%s Pathname", argv[1]));
    }

    x_used = ((argc == 4) && (strcmp(argv[3], "x") == 0)) ? (true) : (false);
    if (x_used) {
        open_fd = open(argv[1], O_WRONLY | O_CREAT | S_IRUSR | S_IWUSR);
        if (open_fd == -1) {
            err_exit("open");
        }
    } else {
        // Open in append mode if "x" is not used
        open_fd = open(argv[1], O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
        if (open_fd == -1) {
            err_exit("open");
        }
    }

    num_bytes = atoll(argv[2]);
    off = 0;

    for (long i = 0; i < num_bytes; i++) {
        // When x is used nothing is written at the offset
        if (x_used) {
            if (lseek(open_fd, off, SEEK_END) == -1) {
                err_exit("seek");
            }
        }

        // While when x is not used "x" is written one byte at a time
        // for the given number of bytes
        if (write(open_fd, "x", 1) == -1) {
            err_exit("write");
        }
    }

    if (close(open_fd) == -1) {
        err_exit("close");
    }
 
    return EXIT_SUCCESS;
}