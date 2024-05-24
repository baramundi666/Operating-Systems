#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

void handle_SIGUSR1(int sig_num) {
    printf("Ok, sygnal otrzymany\n");
}

void* func(void *arg) {
    sigset_t mask;
    sigemptyset(&mask);
//    sigaddset(&mask, SIGUSR1);

    pthread_sigmask(SIG_SETMASK, &mask, NULL);
    signal(SIGUSR1, handle_SIGUSR1);
}

int main() {
    pthread_t thread;

    pthread_create(&thread, NULL, func, NULL);

    pthread_kill(thread, SIGUSR1);

    pthread_join(thread, NULL);
    return 0;
}