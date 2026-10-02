/*
Compile the program in listing 6-1 and list it in long version,
explain why the executable file is much shorter than the 10MB 
size of the buffer (mbuf)
*/

#include <stdio.h>
#include <stdlib.h>

#define KEY 9973
#define TEN_MB_BUF 10240000

static int square(int);
static void do_calc(int);

int main(int argv, int *argc[])
{
    static int key = KEY;
    static char mbuf[TEN_MB_BUF];
    char *p;

    p = malloc(1024);

    do_calc(key);

    return EXIT_SUCCESS;
}

static int square(int x)
{
    int result;

    result = x * x;

    return result;
}

static void do_calc(int val)
{
    printf("The square of %d = %d\n", val, square(val));

    if (val < 1000) {
        int t;

        t = val * val * val;

        printf("The cube of %d is %d\n", val, t);
    }
}

/*
EXPLANATION:

Trough listing the file with ls -l I get the indication that it uses
16152 bytes, which is way less than 10MB.

The reason is, because the mbuf is not initialized at all
and only initialized data in the executable file is written on the
disk.
*/