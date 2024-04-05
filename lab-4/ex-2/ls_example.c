#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int global = 0;

int main(int argc, char *argv[]) {
    if(argc!=2) return -1;

    char* path_name = argv[1];

    printf("programme's name: %s\n", argv[0]);

    int local = 0;

    pid_t pid = fork();

    if(pid == 0) {
        printf("child process\n");
        global++;
        local++;
        printf("child pid = %d, parent pid = %d\n", getpid(), getppid());
        printf("child's local = %d, child's global = %d\n", local, global);
        int status = execl("/bin/ls", "ls", path_name, NULL);
        exit(status);
    }
    else {
        int status;
        waitpid(pid, &status, 0);
        printf("parent process\n");
        printf("parent pid = %d, child pid = %d\n", getpid(), pid);
        printf("child exit code: %d\n", status);
        printf("parent's local = %d, parent's global = %d\n", local, global);
        return status;
    }
}