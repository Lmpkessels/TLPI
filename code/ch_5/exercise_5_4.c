#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "error_helpers.h"

void err_exit(const char *);
void usage_err(const char *, ...);
int mydup(int);
int mydup2(int, int);
 
int main(int argc, char *argv[])
{
    int oldfd;
    int newfd;
    int choice_for_newfd2;

    if (argc != 2 && argc != 3 || strcmp(argv[1], "--help") == 0) {
        usage_err(("%s Pathname", argv[1]));
    }

    oldfd = open(argv[1], O_RDWR | O_CREAT, 0644);

    if (oldfd == -1) {
        err_exit("open");
    }

    // Execute mydup()
    if (argc == 2) {
        // Create a duplicate fd and set it to the closest available
        // fd
        if ((newfd = mydup(oldfd)) == -1) {
            err_exit("dup");
        }

        printf("New FD = (%d)\n", newfd);

        if (close(oldfd) == -1) {
            err_exit("close");
        }
        if (close(newfd) == -1) {
            err_exit("close");
        }

        return EXIT_SUCCESS;
    }

    // Execute mydup2()
    if (argc == 3) {
        choice_for_newfd2 = atoi(argv[2]);

        // Create a duplicate fd and set it to the chosen fd
        if ((newfd = mydup2(oldfd, choice_for_newfd2)) == -1) {
            err_exit("dup2");
        }

        printf("New FD2 = (%d)\n", newfd);

        if (close(oldfd) == -1) {
            err_exit("close");
        }
        // If the new fd is not the old fd than close the file
        if (newfd != oldfd) {
            if (close(oldfd == -1)) {
                err_exit("close");
            }
        }

        return EXIT_SUCCESS;
    }
}

// Takes old open fd and generates a new fd that refers to the same file
int mydup(int oldfd)
{
    // Create a duplicate fd and set it to the lowest available fd
    return fcntl(oldfd, F_DUPFD, 0);
}

// Makes a duplicate of the oldfd and asigns the number to the given in newfd
int mydup2(int oldfd, int newfd)
{
    if (oldfd == newfd) {
        // Check if old file is valid
        if (fcntl(oldfd, F_GETFL) == -1) {
            return -1;
        } 
            
        return oldfd;
    }
    // Close newfd if it equals the old to prevent bad file descriptors
    close(newfd);

    return fcntl(oldfd, F_DUPFD, newfd);
}