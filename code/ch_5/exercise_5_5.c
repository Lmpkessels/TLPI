#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "error_helpers.h"

// Check if duplicated file descriptors share a file offset, 
// and open status flag.
int main(int argc, char *argv[])
{
    int open_fd;
    int duplicated;
    int flags_ofd;
    int flags_dfd;
    int access_mode_ofd;
    int access_mode_dfd;
    char *file;
    off_t off;
    off_t open_off;
    off_t dup_off;

    file = argv[1];

    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        usage_err(("%s Pathname\n", file));
    }

    open_fd = open(file, O_RDWR | O_CREAT , 0644);
    if (open_fd == -1) {
        err_exit("open");
    }

    // Set offset for open fd
    if ((open_off = lseek(open_fd, 10, SEEK_END)) == -1) {
        err_exit("seek");
    }

    if ((duplicated = fcntl(open_fd, F_DUPFD, 0)) == -1) {
        err_exit("fcntl");
    }

    // Get offset for duplicate fd
    if ((dup_off = lseek(duplicated, 0, SEEK_END)) == -1) {
        err_exit("seek");
    }

    // Check if the offset is the same for both files
    printf("Open offset: (%ld)\n", (long) open_off);
    printf("Duplicated offset: (%ld)\n", (long) dup_off);

    flags_ofd = fcntl(open_fd, F_GETFL);
    if (flags_ofd == -1) {
        err_exit("fcntl");
    }

    flags_dfd = fcntl(duplicated, F_GETFL);
    if (flags_dfd == -1) {
        err_exit("fcntl");
    }

    // Check in what mode the open file is opened
    access_mode_ofd = flags_ofd & O_ACCMODE;
    if (access_mode_ofd == O_RDONLY) {
        printf("Open FD is open for reading\n");
    } else if (access_mode_ofd == O_WRONLY) {
        printf("Open FD is open for writing\n");
    } else if (access_mode_ofd == O_RDWR) {
        printf("Open FD is open for reading and writing\n");
    }

    // Check in what mode the duplicated file is opened
    access_mode_dfd = flags_dfd & O_ACCMODE;
    if (access_mode_dfd == O_RDONLY) {
        printf("Duplicated FD is open for reading\n");
    } else if (access_mode_dfd == O_WRONLY) {
        printf("Duplicated FD is open for writing\n");
    } else if (access_mode_dfd == O_RDWR) {
        printf("duplicated FD is open for reading and writing\n");
    }

    if (close(open_fd) == -1) {
        err_exit("close");
    }
    if (close(duplicated) == -1) {
        err_exit("close");
    }

    return EXIT_SUCCESS;
}