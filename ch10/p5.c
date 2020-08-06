#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>


struct timer_t {
    unsigned int started_at;
    unsigned int ended_at;

    void (*proc)();

    struct timer_t *next;
};

struct timer_t *head = NULL;

unsigned int push_to_timer_list(struct timer_t *node) {
    unsigned int cur_time;
    time((time_t *)&cur_time);

    struct timer_t *cur = head;


    if (cur == NULL) {
        head = node;
        alarm(head->ended_at - cur_time);
        return (head->ended_at - cur_time);
    } else {
        if (head->ended_at > node->ended_at) {
            node->next = head;
            head = node;
            alarm(head->ended_at - cur_time);
            return (head->ended_at - cur_time);
        } else {
            while (cur->next && cur->next->ended_at < node->ended_at)
                cur = cur->next;
            node->next = cur->next;
            cur->next = node;
            return (head->ended_at - cur_time);
        }
    }
}

void alarm_proc(int signo) {
    struct timer_t *cur;
    cur = head;
    cur->proc();

    if (cur->next) {
        alarm(cur->next->ended_at - cur->ended_at);
        free(head);
        head = cur->next;
    } else {
        free(head);
        head = NULL;
    }
}

void settimeout(unsigned int seconds, void (*proc)()) {
    if (seconds == 0) {
        proc();
        return;
    }

    struct timer_t *t = (struct timer_t *) malloc(sizeof(struct timer_t));
    time((time_t *)&(t->started_at));

    t->ended_at = t->started_at + seconds;
    t->next = NULL;
    t->proc = proc;

    if (signal(SIGALRM, alarm_proc) == SIG_ERR) {
        printf("signal (SIGALRM) error\n");
        exit(0);
    }

    push_to_timer_list(t);
}

void print1() {
    printf("foo\n");
}

void print2() {
    printf("bar\n");
}


int main() {
//    settimeout(2, print1);
    settimeout(10, print2);
//    settimeout(4, print1);
    for (;;) {
        pause();
    }
}