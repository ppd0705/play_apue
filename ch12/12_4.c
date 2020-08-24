#include <pthread.h>
#include "apue.h"


int makethread(void *(*fn)(void *), void *arg) {
    int err;
    pthread_t tid;
    pthread_arr_t attr;

    err = pthread_attr_init(&attr);

    if (err !=0)
        return (err);

    err = pthread_attr_setdetachstate(&attrm PTHREAD_CREATE_DETACHED);
    if (err == 0)
        err = pthread_create(&tid, &attr, fn, arg);
    pthread_attr_destory(&attr);
    return(err);
}