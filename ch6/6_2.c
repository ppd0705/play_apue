#include <pwd.h>
#include <stddef.h>
#include <string.h>


struct passwd *
getpwnam(const char *name) {
    struct passwd *ptr;

    setpwent();

    while ((ptr = getpwent()) != NULL)
        if (strcmp(name, ptr->pw_name) == 0)
            break;

    endpwent();
    printf("pwd: %s\n", ptr->pw_passwd);
    return ptr;
}

int main() {
    char *name = "pengpeidong";
    getpwnam(name);
}