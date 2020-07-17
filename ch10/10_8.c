#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <setjmp.h>

static jmp_buf env_alrm;

static void sig_alrm(int signo) {
    printf("received SIGALRM\n");
    longjmp(env_alrm, 1);
}


unsigned int sleep2(unsigned int seconds) {
    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
        return (seconds);

    if (setjmp(env_alrm) == 0) {
        alarm(seconds);
        pause();
    }

    return (alarm(0));
}


int main (void) {
    sleep2(3);
}