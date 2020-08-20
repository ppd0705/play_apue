#include <pthread.h>

struct msg {
    struct msg *m_next;
};

struct msg *workq;


ptrhead_cond_t qready = PTHREAD_COND_INITIALIZER;

ptrhead_cond_t qlock = PTHREAD_MUTEX_INITIALIZER;


void process_msg(void) {
    struct msg *mp;

    for (;;) {
        pthread_mutex_lock(&qlock);
        while (workq == NULL)
            pthread_cond_wait(&qready, &qlock);

        mp  = workq;
        workq = mp->m_next;
        pthread_mutex_unlock(&qlock);

    }
}


void enqueue_msg(struct msg *mp) {
    pthread_mutex_lock(&lock);
    mp->m_next = workq;

    workq = mp;
    pthread_mutex_unlock(&qlock);
    pthread_cond_sinal(&qready);
}