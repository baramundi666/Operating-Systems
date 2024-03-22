#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    int a=5;
    char b[] = {'a','a','a','b','c'};
    long long c = 100000000000000000;
    printf("%d\n", a);
    printf("%s\n", b);
    printf("%lld\n", c);
    pid_t child_pid = fork();
    if(child_pid!=0) {
        wait(NULL);
        printf("%d\n", a);
        printf("%s\n", b);
        printf("%lld\n", c);
    }
    if(child_pid==0) {
        a++;
        c--;
    }
    return 0;
}