#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define BUF_SIZE 4096

void err_exit(const char *);
int tee(char, char *[]);

int main()
{
    int argv;

    //argv = 2;
    //char *argc1[] = {"file.txt", "file_to_copy_to.txt"};

    //tee(argv, argc1);

    argv = 3;
    char *argc2[] = {"-a", "file.txt", "Hello, new world!"};

    tee(argv, argc2);
}

void err_exit(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

int tee(char argv, char *argc[])
{
    int output_fd, o_flags;
    int input_fd, i_flags;
    ssize_t num_read;
    char buffer[BUF_SIZE];

    if (argv == 2) {
        i_flags = O_RDONLY;
        // Truncate if it already exists
        o_flags = O_WRONLY | O_CREAT | O_TRUNC;

        input_fd = open(argc[0], i_flags);
        output_fd = open(argc[1], o_flags, 0644);

        if (output_fd == -1) {
            err_exit(("open output file %s", argc[1]));
        }
    }

    else if (argv == 3 && strcmp(argc[0], "-a") == 0) {
        o_flags = O_WRONLY | O_CREAT | O_APPEND;

        output_fd = open(argc[1], o_flags, 0644);

        if (output_fd == -1) {
            err_exit(("open output file %s", argc[2]));
        }
    }

    else {
        fprintf(stderr, "Usage: %s [-a] file\n", argc[0]);
        exit(EXIT_FAILURE);
    }

    while (num_read = read(input_fd, buffer, BUF_SIZE) > 0) {
        if (write(output_fd, buffer, num_read) != num_read) {
            err_exit("write stdout");
        }

        if (write(output_fd, buffer, BUF_SIZE) != num_read) {
            err_exit("write file");
        } 
    }

    if (num_read == 1) {
        err_exit("read");
    }
    if (close(input_fd) == -1) {
        err_exit("closing file");
    }
    if (close(output_fd) == -1) {
        err_exit("closing file");
    }

    exit(EXIT_SUCCESS);
}