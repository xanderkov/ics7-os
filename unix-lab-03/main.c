#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <fcntl.h>
#include <sys/resource.h>

static void my_daemonize()
{
    pid_t pid;
    struct rlimit rl;
    struct sigaction sa;
    int i, fd0, fd1, fd2;

    /*
        Сбросить маску создания файлов.
    */
    umask(0);

    pid = fork();

    if (pid < 0)
        exit(1);
    if (pid > 0)
        exit(0);

    /* Сделать потомка лиюером группы и лидером сессии*/
    if (setsid() < 0)
        exit(1);
    
    if (chdir("/") < 0)
        exit(1);
    
    if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
        exit(1);

    if (rl.rlim_max == RLIMIT_NOFILE)
        rl.rlim_max = 1024;
    for (int i = 0; i < rl.rlim_max; i++)
    {
        close(i);
    }
    
    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);

    openlog ("daemon", LOG_PID, LOG_DAEMON);

    if (fd0 != 0 || fd1 != 1 || fd2 != 2)
        syslog(LOG_ERR, "false in file desctiptors");
}

int main()
{
    my_daemonize();

    while (1)
    {
        syslog(LOG_NOTICE, "daemon started.");
        sleep (20);
        break;
    }

    syslog (LOG_NOTICE, "daemon terminated.");
    closelog();

    return 0;
}