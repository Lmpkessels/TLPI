// Helper functions for error termination

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void err_exit(const char *msg) 
{
    perror(msg);
    exit(EXIT_FAILURE);
}

void usage_err(const char *format, ...)
{
    va_list argList;

    fflush(stdout);

    fprintf(stderr, "Usage: ");

    va_start(argList, format);
    vfprintf(stderr, format, argList);
    va_end(argList);

    exit(EXIT_FAILURE);
}