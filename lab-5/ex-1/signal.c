#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

void handle_SIGUSR1(int sig);

int main(int argc, char** argv) {

    if(argc != 2) return 1;

    char* input = argv[1];
    int case_no = 0;

    if(strcmp(input, "ignore") == 0) case_no = 1;
    else if(strcmp(input, "handler") == 0) case_no = 2;
    else if(strcmp(input, "mask") == 0) case_no = 3;

    switch(case_no) {
        case 1:
            signal(SIGUSR1, SIG_IGN);
            raise(SIGUSR1);
        case 2:
            signal(SIGUSR1, handle_SIGUSR1);
            raise(SIGUSR1);
        case 3:
            sigset_t mask;

            sigemptyset(&mask);
            sigaddset(&mask, SIGUSR1);

            sigprocmask(SIG_SETMASK, &mask, NULL);
            raise(SIGUSR1);

            sigset_t pending;
            sigpending(&pending);
            if(sigismember(&pending, SIGUSR1)) {
                printf("Pending SIGUSR1 signal is visible\n");
            }
            else {
                printf("Pending SIGUSR1 signal is not visible\n");
            }
        default:
            signal(SIGUSR1, SIG_DFL);
            raise(SIGUSR1);
    }

    return 0;
}

void handle_SIGUSR1(int sig_num) {
    printf("Received SIGUSR1 signal: %d\n", sig_num);
}