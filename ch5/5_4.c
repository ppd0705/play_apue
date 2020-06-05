#include "apue.h"


int
main(void) {
    int c;
    while ((c = getc(stdin)) !=EOF)
        if(putc(c, stderr) == EOF)
            err_sys("out put error");

    if (ferror(stdin))
        err_sys("input error");

    exit(0);
}