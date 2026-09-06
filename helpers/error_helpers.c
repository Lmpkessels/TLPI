#include <stdio.h>
#include <stdlib.h>

// Helper function for error
void err_exit(const char *msg) 
{
    perror(msg);
    exit(EXIT_FAILURE);
}