#include "apue.h"
#include <fcntl.h>

int main(void) {

    int fd;
    int n = 9;
    char buf1[] = "hello foo";
    char buf2[n];

    fd = open("file.hole", O_APPEND | O_RDWR);

    if (fd < 0)
        err_sys("open file error");

    write(fd, buf1, n);
    lseek(fd, 0, SEEK_SET);
    read(fd, buf2, n);
    write(STDOUT_FILENO, buf2, n);
}