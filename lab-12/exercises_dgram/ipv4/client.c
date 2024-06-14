#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <arpa/inet.h>

#define BUF_SIZE 100
#define BACKLOG 5
#define PORT 3000


int main() {
    struct sockaddr_in addr;

    int sfd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    socklen_t len;

    connect(sfd, (struct sockaddr*) &addr, sizeof(struct sockaddr_in));

    size_t numRead;
    char buf[BUF_SIZE];
    char msg[BUF_SIZE];
    numRead = read(STDIN_FILENO, buf, BUF_SIZE-1);
    buf[numRead] = '\0';
    sendto(sfd, buf, numRead+1, 0,
               (struct sockaddr*) &addr, sizeof(struct sockaddr_in));

    numRead = recvfrom(sfd, msg, BUF_SIZE-1, 0, NULL, NULL);
    msg[numRead] = '\0';
    write(STDOUT_FILENO, msg, numRead);

    close(sfd);
    return 0;
}