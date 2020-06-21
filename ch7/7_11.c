#include <setjmp.h>
#include "apue.h"

#define TOK_ADD 5

jmp_buf jmpbuffer;

void do_line(char *ptr);

int main(void) {
    char line[MAXLINE];
    if (setjmp(jmpbuffer) != 0)
        printf("error\n");

    while (fgets(line, MAXLINE, stdin) != NULL)
        do_line(line);

    exit(0);
}

void do_line(char *ptr) {
    if (ptr[0] > 'j')
        printf("char: %s", ptr);
    else
    {
        longjmp(jmpbuffer, 1);
        printf("jump: %s", ptr); // never reached
    }

}