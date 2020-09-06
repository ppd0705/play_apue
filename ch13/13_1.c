#include <syslog.h>
#include <fcntl.h>
#include <sys/resource.h>
#include "apue.h"


void daemonize(const char *cmd) {
    int i, fd0, fd1, fd2;
    pid_t pid;
    struct rlimit rl;
    struct sigaction sa;

    // clean file creation mask
    umask(0);

    // get maximum number of file descriptions
    if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
        err_quit("%s: can't get file limit", cmd);

    // parent exit, child become a session leader to lose controlling TTY
    if ((pid = fork()) < 0)
        err_quit("%s: can't fork", cmd);
    else if (pid != 0)
        exit(0);
    setsid();

    // ensure future opens won't allocate controlling TTYS
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGHUP, &sa, NULL) < 0)
        err_quit("%s, can't ignore SIGUHP", cmd);
    if ((pid = fork()) < 0)
        err_quit("%s: can't fork", cmd);
    else if (pid != 0)
        exit(0);

    // change the current working directory to the root so we won't prevent file systems form being unmounted
    if (chdir("/") < 0)
        err_quit("%s: can't change directory to /", cmd);

    // close all open file descriptors
    if (rl.rlim_max == RLIM_INFINITY)
        rl.rlim_max = 1024;
    for (i = 0; i < rl.rlim_max; i++)
        close(i);

    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);

    // initialize the log file
    openlog(cmd, LOG_CONS, LOG_DAEMON);
    if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
        syslog(LOG_ERR, "unexpected file descriptors %d %d %d", fd0, fd1, fd2);
        exit(1);
    }
}
