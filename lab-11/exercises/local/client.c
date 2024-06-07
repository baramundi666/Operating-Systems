#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <arpa/inet.h>

#define SV_SOCK_PATH "/tmp/us_xfr"
#define BUF_SIZE 100
#define BACKLOG 5


int main() {
    struct sockaddr_un addr;

    int sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    printf("Client socket: %d\n", sfd);

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

    connect(sfd, (struct sockaddr*) &addr, sizeof(struct sockaddr_un));

    size_t numRead;
    char buf[BUF_SIZE];
    while((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0) {
        printf("%d/n", (int) numRead);
        write(sfd, buf, numRead);
    }
}