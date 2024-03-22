#include <unistd.h>
#include <wait.h>

int main() {
    pid_t child_pid = fork();
    if(child_pid!=0) {
        execvp("./HelloWorld", NULL);
    }
    return 0;
}