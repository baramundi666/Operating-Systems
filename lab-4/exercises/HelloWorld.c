#include <unistd.h>
#include <stdio.h>


void HelloWorld() {
    printf("HelloWorld %d\n", (int) getpid());
}

int main() {
    HelloWorld();
    return 0;
}

