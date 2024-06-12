#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <arpa/inet.h>

#define SV_SOCK_PATH "/tmp/us_xfr"
#define BUF_SIZE 100
#define BACKLOG 5
#define PORT 5000


int main() {
    struct sockaddr_in addr;

    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("Client socket: %d\n", sfd);

    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;


    connect(sfd, (struct sockaddr*) &addr, sizeof(struct sockaddr_in));

    size_t numRead;
    char buf[BUF_SIZE];
    while((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0) {
        write(sfd, buf, numRead);
    }
}