#include <signal.h>
#include <unistd.h>
#include <stdio.h>

static void sig_alrm(int signo) {
    printf("received SIGALRM\n");
}


unsigned int sleep1(unsigned int seconds) {
    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
        return (seconds);

    alarm(seconds);

    pause();

    return (alarm(0));
}


int main (void) {
    sleep1(3);
}