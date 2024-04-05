#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv) {
    if(argc!=2) return -1;

    int process_count = atoi(argv[1]);

    if(process_count<0) return -1;

    for(int i=0; i<process_count; ++i) {
        pid_t child_pid = fork();
        if(child_pid == 0) {
            printf("parent pid = %d, child pid = %d\n", getppid(), getpid());
            exit(0);
        }
    }

    for(int i=0; i<process_count; ++i) wait(NULL);

    printf("argv[1] = %s\n", argv[1]);
}