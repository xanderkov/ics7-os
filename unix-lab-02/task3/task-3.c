#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define SLEEP_FOR 5

char *exec_params[2] = {"app1.exe", "app2.exe"};

int main(void) {
    pid_t child_pid[2];
    int rc;
    int child;
    int status;

    printf("parent pid: %d, group: %d\n", getpid(), getpgrp());
    for (int i = 0; i < 2; i++) {
        child_pid[i] = fork();
        if (child_pid[i] == -1) {
            perror("can't fork\n");
            return 1;
        } 
        else if (child_pid[i] == 0) {
            printf("child pid: %d, group %d, ppid: %d\n", getpid(), getpgrp(), getppid());
            printf("child (pid = %d) executed %s\n", getpid(), exec_params[i]);
            rc = execl(exec_params[i], NULL);
            if (rc == -1) {
                perror("cant exec\n");
                return 1;
            }
            return 0;
        }
    }
    for (int i = 0; i < 2; i++) {
        child = wait(&status);
        printf("child finished\nchild pid: %d, status: %d\n", child, status);
        if (WIFEXITED(status)) {
            printf("child exited with code %d\nchild pid: %d\n", WEXITSTATUS(status), child);
        }
        else if (WIFSIGNALED(status)) {
            printf("child terminated with un-intercepted signal number %d\nchild pid: %d\n", WTERMSIG(status), child);
        }
        else if (WIFSTOPPED(status)) {
            printf("child stopped with signal number %d\nchild pid: %d\n", WSTOPSIG(status), child);
        }
    }
    printf("parent pid: %d\nchildren %d %d\n", getpid(), child_pid[0], child_pid[1]);
    return 0;
}