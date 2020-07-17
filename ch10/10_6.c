#include <sys/wait.h>
#include "apue.h"

static void sig_chld(int);


int main() {
    pid_t pid;

    if (signal(SIGCHLD, sig_chld) == SIG_ERR)
        perror("signal error");

    if ((pid = fork()) < 0) {
        perror("fork error");
    } else if (pid == 0) {
        sleep(2);
        abort();
    }

    pause();
    exit(0);
}


static void sig_chld(int signo) {
    pid_t pid;
    int status;


    printf("SIGCLD reveived\n");

    if (signal(SIGCHLD, sig_chld) == SIG_ERR)
        perror("signal error");

    if ((pid = wait(&status)) < 0)
        perror("wait error");


    printf("pid = %d, status = %d\n", pid, status);
}



