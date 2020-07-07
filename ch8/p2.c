#include <sys/types.h>
#include "apue.h"


static void f1(void);

static void f2(void);


int main(void) {
    f1();
    printf("main pid %d ppid %d\n", getpid(), getppid());
    f2();
    _exit(0);
}


static void f1(void) {
    pid_t pid;

    if ((pid = vfork()) < 0)
        err_sys("vfork error");

    printf("f1 pid %d ppid %d\n", getpid(), getppid());

}


static void f2(void) {
    char buf[1000];
    int i;
    printf("f2 pid %d ppid %d\n", getpid(), getppid());
    for (i = 0; i < sizeof(buf); i++)
        buf[i] = 0;
}