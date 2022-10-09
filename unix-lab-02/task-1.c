#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define SLEEP_FOR 5

int main(void) {
    pid_t child_pid[2];
    printf("parent pid: %d, group: %d\n", getpid(), getpgrp());
    for (int i = 0; i < 2; i++) {
        child_pid[i] = fork();
        if (child_pid[i] == -1) {
            perror("can't fork\n");
            return 1;
        }
        else if (child_pid[i] == 0) {
            sleep(SLEEP_FOR);
            printf("child pid: %d, group %d, ppid: %d\n", getpid(), getpgrp(), getppid());
            return 0;
        }
    }
    printf("parent pid: %d\nchildren %d %d\n", getpid(), child_pid[0], child_pid[1]);
    return 0;
}