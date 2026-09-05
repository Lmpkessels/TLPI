#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUF_SIZE 4096

void cp(int, char *[]);

int main(void)
{
    int argc;
    char *argv[] = {"cp_helper_file.txt", "cp_helper_file_copy.txt"};
    
    argc = 2;
    
    cp(argc, argv);

    return 0;
}

// Helper function for error
void err_exit(const char *msg) 
{
    perror(msg);
    exit(EXIT_FAILURE);
}

// cp(): make a copy of argv[0] and write it to argv[1] and create a hole in out-
// put_fd
void cp(int argc, char *argv[])
{
    int input_fd, i_flags;
    int output_fd, o_flags;
    char *input_file;
    char *output_file;
    int hole_size;
    ssize_t num_read;
    char buffer[BUF_SIZE];
    
    if (argc != 2) {
        fprintf(stderr, "usage %s <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    input_file = argv[0];
    output_file = argv[1];

    // Open both folder for reading data from and writing data to
    i_flags = O_RDONLY;
    input_fd = open(input_file, i_flags);
        
    if (input_fd == -1) {
        err_exit("open input fd");
    }

    o_flags = O_WRONLY | O_CREAT | O_TRUNC;
    output_fd = open(output_file, o_flags, 0644);    
        
    if (output_fd == -1) {
            err_exit("open output fd");
    }

    hole_size = 0;
    while ((num_read = read(input_fd, buffer, BUF_SIZE)) > 0) {
        for (int i = 0; i < num_read; i++) {
            if (buffer[i] == '\0') {
                hole_size++;
            }
            else {
                if (hole_size > 0) {
                    if ((lseek(output_fd, hole_size, SEEK_CUR) == -1)) {
                        err_exit("lseek");
                    }

                    hole_size = 0;
                }

                if ((write(output_fd, buffer + i, 1)) == -1) {
                    err_exit("write");
                }
            }
        }
    }

    if (num_read == -1) {
        err_exit("read");
    }
    if (close(input_fd) == -1) {
        err_exit("closing");
    }
    if (close(output_fd) == -1) {
        err_exit("closing");
    }
}