#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>

#define BUFFER_SIZE 1024
#define BACKLOG 5

volatile sig_atomic_t flag = 0;
int sfd;
char name[32];

void handle_sigint(int sig) {
    char msg[5] = "STOP\0";
    sendto(sfd, msg, strlen(msg), 0, NULL, 0);
    flag = 1;
}
void handle_send();
void handle_receive();

int main(int argc, char* argv[]) {
    if(argc != 4) return 1;

    strcpy(name, argv[1]);
    char *server_ip = argv[2];
    int server_port = atoi(argv[3]);

    signal(SIGINT, handle_sigint);

    struct sockaddr_in server_addr;

    sfd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    connect(sfd, (struct sockaddr*) &server_addr, sizeof(server_addr));

    sendto(sfd, name, sizeof(name), 0,
           (struct sockaddr*) &server_addr, sizeof(server_addr));

    pthread_t send_thread;
    pthread_create(&send_thread, NULL, (void *) handle_send, NULL);

    pthread_t receive_thread;
    pthread_create(&receive_thread, NULL, (void *) handle_receive, NULL);

    while(1) {
        if(flag) {
            printf("\nDisconnected!\n");
            break;
        }
    }

    close(sfd);
}

void handle_send() {
    char buffer[BUFFER_SIZE];

    while(1) {
        int size;
        fgets(buffer, BUFFER_SIZE, stdin);
        for(int i=0; i<BUFFER_SIZE; ++i) {
            if(buffer[i] == '\n') {
                buffer[i] = '\0';
                size = i+1;
                break;
            }
        }

        if(strcmp(buffer, "STOP") == 0) {
            break;
        }
        else {
            send(sfd, buffer, size, 0);
        }

        bzero(buffer, BUFFER_SIZE);
    }
    handle_sigint(0);
}

void handle_receive() {
    char message[BUFFER_SIZE];
    while(1) {
        int receive = recvfrom(sfd, message, BUFFER_SIZE, 0, NULL, NULL);
        if(receive > 0) {
            printf("%s", message);
            bzero(message, BUFFER_SIZE);
        }
    }
}