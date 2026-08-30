#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

void o_r(const char *);

int main(void)
{
    const char *file = "file.txt";
    
    o_r(file);

    return 0;
}

// Return the descriptor thats used for the process
int get_descriptor(const char *file)
{
    int fd = open(file, O_RDONLY);

    if (fd == -1) {
        perror("Open");
        return;
    }

    close(of);

    return fd;
}