#include "apue.h"


int
main(void) {
    char buf[2];

    while (fgets(buf, MAXLINE, stdin) != NULL)
        if (fputs(buf, stdout) == EOF)
            err_sys("output error");


    if (ferror(stdin))
        err_sys("input error");
    exit(0);
}