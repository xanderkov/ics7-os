#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define SLEEP_FOR 2

int main(void) {
    pid_t child_pid[2];
    printf("parent pid: %d, group: %d\n", getpid(), getpgrp());
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
            sleep(SLEEP_FOR);
            printf("child pid: %d, group %d, ppid: %d\n", getpid(), getpgrp(), getppid());
            return 0;
        }
        else
        {
            printf("parent pid: %d, child %d, group %d\n", getpid(), child_pid[i], getpgrp());
        }
    }
   
    return 0;
}