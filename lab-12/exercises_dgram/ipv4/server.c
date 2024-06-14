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


int
main(int argc, char *argv[])
{
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int sfd;
    ssize_t numRead;
    char buf[BUF_SIZE];

    sfd = socket(AF_INET, SOCK_DGRAM, 0);
    printf("Server socket: %d\n", sfd);

    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    memset(&client_addr, 0, sizeof(struct sockaddr_in));
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(PORT);
    client_addr.sin_addr.s_addr = INADDR_ANY;

    socklen_t len = sizeof(struct sockaddr_in);

    bind(sfd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr_in));

    char msg[BUF_SIZE];

    numRead = recvfrom(sfd, buf, BUF_SIZE-1, 0,
                               (struct sockaddr *) &client_addr,&len);

    buf[numRead] = '\0';
    printf("Received: %s", buf);

    snprintf(msg, BUF_SIZE, "Confirmed message: %s", buf);

    sendto(sfd, msg, strlen(msg) + 1, 0,
               (struct sockaddr *) &client_addr, len);

    close(sfd);
    return 0;
}