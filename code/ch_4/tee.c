#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h> 

#define BUF_SIZE 4092

int tee(int, char *[]);

int main(void)
{
    //char *argc[3] = {"tee", "file.txt", "file2.txt"};
    //tee(3, argc);

    char *argc2[3] = {"tee", "-a", "file2.txt"};
    // FIX: -a no such file or directory at: input_fd
    tee(3, argc2);
}

void error_exit(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

// tee(): read input till the end of file, write a copy of the file to the file
// given in the command-line argument
//
// add a flag -a to append text append text to the EOF if it already exists
//
// TODO: add a possibility to add text, and handle it properly (only when the flag
// is used)
int tee(int argv, char *argc[])
{
    int input_fd, output_fd;
    int num_read;
    int *buffer[BUF_SIZE];
    
    // Open input file to read from
    input_fd = open(argc[1], O_RDONLY);
    if (input_fd == -1) {
        error_exit(("opening file %s", argc[1]));
    }

    // Open output file to write to
    output_fd = open(argc[2], O_WRONLY | O_CREAT);
    if (output_fd == -1) {
        error_exit(("opening file: %s", argc[2]));
    }

    // In case flag -a is used append text to the already existing given file
    switch (argc[1][0]) {
        case '-':
            if (argc[1][1] == 'a') {
                char *text = "Hello new world!";

                if (write(output_fd, text, strlen(text)) == -1) {
                    error_exit("write() returned error or partial append occured");
                }
            };
            break;
        default:
            // Write a copy of the file and save it in the given file
            while ((num_read = read(input_fd, buffer, BUF_SIZE)) > 0) {
                if (write(output_fd, buffer, num_read) != num_read) {
                    error_exit("write() returned error or partial write occured");
                }
            }
            break;
    } 

    close(input_fd);

    if (close(input_fd) == -1) {
        error_exit(("closing file: %d", argc[1]));
    } 
    if (close(output_fd) == -1) {
        error_exit(("closing file: %d", argc[2]));
    }

    exit(EXIT_SUCCESS);
}