#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

void handle_SIGUSR1(int sig_num);

int main(int argc, char** argv) {

    if(argc != 3) return 1;

    signal(SIGUSR1,  handle_SIGUSR1);

    pid_t catcher_pid = atoi(argv[1]);
    int mode = atoi(argv[2]);

    if (mode < 1 || mode > 3) return 1;

    union sigval value;
    value.sival_int = atoi(argv[2]);
    sigqueue(catcher_pid, SIGUSR1, value);

    sigset_t mask;
    sigfillset(&mask);
    sigdelset(&mask, SIGUSR1);
    sigdelset(&mask, SIGINT);

    sigsuspend(&mask);

    return 0;
}

void handle_SIGUSR1(int sig_num) {
    printf("Received SIGUSR1 signal: %d\n", sig_num);
}