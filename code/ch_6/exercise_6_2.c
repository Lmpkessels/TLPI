/*Problem:
Write a program that tries to longjump into a function that already 
has been returned.
*/

#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

static void f1(void);

static jmp_buf env;

int main(void)
{
    printf("In main\n");
    printf("Calling f1()\n");
    f1();
    printf("Returned from f1()\n");
    printf("Perform longjump\n");
    longjmp(env, 1);
    printf("Back into main\n");
}

static void f1(void)
{
    printf("Calling setjump()");
    setjmp(env);
}

/*
There's an segmentation fault that arrives because an invalid
address is being accessed trough longjmp
*/