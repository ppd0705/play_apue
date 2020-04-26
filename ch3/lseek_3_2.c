#include "apue.h"
#include <fcntl.h>
#include <sys/stat.h>

char buf1[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";


int
main(void)
{
    int fd;
    int c;

    if ((fd = open("file.hole", O_CREAT | O_RDWR | O_TRUNC, S_IWUSR | S_IRUSR)) < 0)
        err_sys("create error");

    if (write(fd, buf1, 10) != 10)
        err_sys("buf1 write error");

    if ((c = lseek(fd, 16384, SEEK_SET)) == -1) {
        err_sys("lseek error");

    } else {
        printf("offset: %d\n", c);
    }

    if (write(fd, buf2, 10) != 10)
        err_sys("buf2 write error");

    exit(0);
}