#include "apue.h"


int globvar = 6;
char buf[] = "a write to stdout\n";

int main(void) {
    int var, i;
    pid_t pid;
    var = 88;
    if (write(STDOUT_FILENO, buf, sizeof(buf) - 1) != sizeof(buf) - 1) /* write不带缓冲，所以不会复制到子进程*/
        err_sys("write error");

    printf("before fork\n");
    /* fflush(stdout); */

    pid = vfork();
    if (pid == 0) {
        globvar++;
        var++;
        exit(0);
    }

    i = printf("pid = %ld, glob = %d, var = %d\n", (long) getpid(), globvar, var);
    sprintf(buf, "%d\n", i);
    write(STDOUT_FILENO, buf, strlen(buf));
    exit(0);
}
