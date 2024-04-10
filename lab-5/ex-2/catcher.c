#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

int mode = 0;
int change_count = 0;

void action_SIGUSR1(int sig, siginfo_t *info, void *context);

int main() {
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = action_SIGUSR1;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);

    sigaction(SIGUSR1, &sa, NULL);

    printf("Catcher PID: %d\n", getpid());

    while(1) {
        while (mode == 0) {
            sigsuspend(&(sigset_t){});
        }
        switch(mode) {
            case 1:
                for(int i=1; i<101; ++i){
                    printf("%d\n", i);
                }
                break;
            case 2:
                printf("Mode changes count: %d\n", change_count);
                break;
            case 3:
                printf("Received exit signal\n");
                exit(0);
                break;
        }

        mode = 0;
    }

    return 0;
}

void action_SIGUSR1(int sig, siginfo_t *info, void *context) {
    mode = info->si_value.sival_int;
    change_count++;

    printf("Received mode: %d from pid: %d\n", mode, info->si_pid);

    kill(info->si_pid, SIGUSR1);
}