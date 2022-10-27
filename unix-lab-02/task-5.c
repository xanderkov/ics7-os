#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define SLEEP_FOR 2

char *msg[2][100] = {"xxx", "aabbccddeeffggkkllmmnn" };
int sig_flag = 0;

void sigCatch() { sig_flag = 1; }

int main(void) {
    pid_t child_pid[2];
    int fd[2];
    int status;
    int child;

    signal(SIGTSTP, sigCatch);
    printf("\n");
    printf("ctr + z, if you want to pipe\n");
    sleep(SLEEP_FOR);
    printf("parent pid: %d, group: %d\n", getpid(), getpgrp());
    if (pipe(fd) == -1) 
    {
        perror("cant pipe\n");
        exit(1);
    }
    for (int i = 0; i < 2; i++) 
    {
        child_pid[i] = fork();
        if (child_pid[i] == -1) 
        {
            perror("cant fork\n");
            exit(1);
        } 
        if (child_pid[i] == 0) 
        {
            
            if (sig_flag) 
            {
                close(fd[0]);
                write(fd[1], (*msg)[i], strlen((*msg)[i]));
                printf("msg from child (pid = %d) %s sent to parent\n", getpid(), (*msg)[i]);
            } 
            else 
            {
                printf("no signal, no msg will be recieved\n");
            }
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
    close(fd[1]);
    read(fd[0], *msg, sizeof(*msg));
    printf("parent (pid: %d) recieved msg: %s\n", getpid(), *msg); 
    return 0;
}
