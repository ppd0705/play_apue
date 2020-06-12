#include <sys/types.h>
#include <pwd.h>
#include "apue.h"

int main(void) {
    struct passwd *ptr;

    if ((ptr = getpwnam("pengpeidong")) == NULL)
        err_sys("getpwnam error");


    printf("pw = %s\n", ptr->pw_passwd == NULL || ptr->pw_passwd[0] == 0 ? "(null)" : ptr->pw_passwd);

    exit(0);
}