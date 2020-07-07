#include "apue.h"

int main(void) {
    pid_t pid;
    if ((pid = fork()) < 0)
        err_sys("fork erro");
    else if (pid == 0) {
        printf("pid %d\n", getpid());
        exit(0);
    }

    sleep(2);
    system("ps u");
    exit(0);
}