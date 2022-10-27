#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define SLEEP_FOR 5

int main(void) {
    pid_t child_pid[2];
    int status;
    pid_t child;

   // printf("parent pid: %d, group: %d\n", getpid(), getpgrp());
    for (int i = 0; i < 2; i++) 
    {
        child_pid[i] = fork();
        if (child_pid[i] == -1) 
        {
            perror("can't fork\n");
            exit(1);
        }
        if (child_pid[i] == 0) 
        {
            printf("child pid: %d, group %d, ppid: %d\n", getpid(), getpgrp(), getppid());
            return 0;
        }
        else
        {
            printf("parent pid: %d, child %d, group %d\n", getpid(), child_pid[i], getpgrp());
        }
    }

    for (int i = 0; i < 2; i++) 
    {
        child = wait(&status);
       // printf("parent pid: %d\nchildren %d\n", getpid(), child_pid[i]);
        printf("child pid: %d, parent %d, status: %d, group %d\n", child, getpid(), status, getpgrp());
        if (WIFEXITED(status)) 
        {
            printf("child exited with code %d, child pid: %d\n", WEXITSTATUS(status), child);
        }
        else if (WIFSIGNALED(status)) 
        {
            printf("child terminated with un-intercepted signal number %d, child pid: %d\n", WTERMSIG(status), child);
        }
        else if (WIFSTOPPED(status)) 
        {
            printf("child stopped with signal number %d, child pid: %d\n", WSTOPSIG(status), child);
        }
    }
   
   
    return 0;
}