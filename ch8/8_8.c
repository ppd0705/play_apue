#include "apue.h"
#include <sys/wait.h>


int main(void) {
    pid_t pid;

    if ((pid = fork()) < 0) {
        err_sys("forl error");
    } else if (pid == 0) {
        if ((pid = fork()) < 0)
            err_sys("fork error");
        else if (pid > 0) {
            printf("first child exit,  pid = %ld\n", (long)getpid());
            exit(0);
        }


        sleep(2);
        printf("second child pid %ld parent pid = %ld\n", (long)getpid(), (long)getppid());
        exit(0);
    }

    if (waitpid(pid, NULL, 0) != pid)
        err_sys("waitpid error");
    printf("exit pid = %ld\n", (long)getpid());
    exit(0);
}