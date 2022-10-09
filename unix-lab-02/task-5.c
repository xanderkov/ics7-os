#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define SLEEP_FOR 5

char *msgs[2] = {"xxx", "aabbccddeeffggkkllmmnn"};
int sig_flag = 0;

void sigCatch() { sig_flag = 1; }

int main(void) {
    pid_t child_pid[2];
    int fd[2];
    int status;
    int child;
    char buf[100] = {0};

    signal(SIGTSTP, sigCatch);
    printf("parent pid: %d, group: %d\n", getpid(), getpgrp());
    if (pipe(fd) == -1) {
        perror("cant pipe\n");
        return 1;
    }
    for (int i = 0; i < 2; i++) {
        child_pid[i] = fork();
        if (child_pid[i] == -1) {
            perror("cant fork\n");
            return 1;
        } 
        else if (child_pid[i] == 0) {
            sleep(SLEEP_FOR);
            if (sig_flag) {
                close(fd[0]);
                write(fd[1], msgs[i], strlen(msgs[i]));
                printf("msg from child (pid = %d) %s sent to parent\n", getpid(), msgs[i]);
            } 
            else {
                printf("no signal, no msg will be recieved\n");
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
    close(fd[1]);
    read(fd[0], buf, sizeof(buf));
    printf("parent (pid: %d) recieved msgs: %s\n", getpid(), buf);
    printf("parent pid: %d\nchildren %d %d\n", getpid(), child_pid[0], child_pid[1]);
    return 0;
}
