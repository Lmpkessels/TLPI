#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define BUF_SIZE 4094 // Maximum bytes allowed in file

int cp(int, char *[]);

int main(void)
{
    int argc;
    char *argv[] = {"cp_helper_file.txt", "cp_helper_file_copy.txt"};
    
    argc = 2;
    
    cp(argc, argv);

    return 0;
}

// Helper function for error
int err_exit(const char *msg) 
{
    perror(msg);
    exit(EXIT_FAILURE);
}

// cp(): make a copy of argv[0] and write it to argv[1], create a hole in both
// the given file and target file
int cp(int argc, char *argv[])
{
    int input_fd, i_flags;
    int output_fd, o_flags;
    ssize_t num_read;
    int buffer[BUF_SIZE];

    // Open both folder for reading data from and writing data to
    if (argc == 2) {
        i_flags = O_RDONLY;
        input_fd = open(argv[0], i_flags, 0644);

        if (input_fd == -1) {
            err_exit("open input fd");
        }

        o_flags = O_WRONLY | O_CREAT | O_TRUNC;
        output_fd = open(argv[1], o_flags, 0644);

        if (output_fd == -1) {
            err_exit("open output fd");
        }
    } else {
        err_exit("to little arguments");
    }

    // Make the copy (get the data from argv[0] and write it to argv[1])
    while ((num_read = read(input_fd, buffer, BUF_SIZE)) > 0) {
        if ((write(output_fd, buffer, num_read)) != 0) {
            err_exit(("writing to %s", argv[1]));
        }
    }

    if (close(input_fd) == -1) {
        err_exit(("closing %s", argv[0]));
    }
    if (close(output_fd) == -1) {
        err_exit(("closing %s", argv[1]));
    }
}