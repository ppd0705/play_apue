#include "apue.h"
#include <fcntl.h>

#define RWRWRW (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

int main(void) {
    umask(0);
    if (open("foo", O_WRONLY|O_CREAT|O_TRUNC, RWRWRW)< 0)  {
        err_sys("create error foo");
    }
    umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if (open("bar", O_WRONLY|O_CREAT|O_TRUNC, RWRWRW)< 0) {
        err_sys("create error bar");
    }
    exit(0);
}