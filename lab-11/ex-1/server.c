#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BACKLOG 5
#define MAX_CLIENTS 3
#define BUFFER_SIZE 1024

typedef struct {
    struct sockaddr_in address;
    int fd;
    int id;
    char name[32];
} client_t;

int num_clients = 0;
client_t *clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void *handle_client(void *arg);


int main(int argc, char *argv[]) {
    if(argc != 2) return 1;

    int port = atoi(argv[1]);

    int sfd, cfd;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;


    sfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("Server socket: %d\n", sfd);

    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    int option = 1;
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (char*) &option, sizeof(option));

    bind(sfd, (struct sockaddr*) &server_addr, sizeof(server_addr));

    listen(sfd, BACKLOG);

    pthread_t thread;

    for (;;) {
        // accept new client
        socklen_t client_len = sizeof(client_addr);
        cfd = accept(sfd, (struct sockaddr*) &client_addr, &client_len);
        num_clients++;

        // too many clients
        if(num_clients > MAX_CLIENTS) {\
            num_clients--;
            printf("Max clients!\n");
            close(cfd);
            continue;
        }

        // create client
        client_t* client = (client_t*) malloc(sizeof(client_t));
        client->address = client_addr;
        client->fd = cfd;
        client->id = num_clients;

        // add client
        pthread_mutex_lock(&clients_mutex);
        for(int i=0; i<MAX_CLIENTS; ++i) {
            if(!clients[i]) {
                clients[i] = client;
                break;
            }
        }
        pthread_mutex_unlock(&clients_mutex);

        // client thread
        pthread_create(&thread, NULL, &handle_client, (void *) client);
    }
}

void send_message_to_all_but(char *s, int id) {
    pthread_mutex_lock(&clients_mutex);
    for(int i = 0; i < MAX_CLIENTS; ++i) {
        if(clients[i] && clients[i]->id != id) {
            write(clients[i]->fd, s, strlen(s));
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void *handle_client(void *arg) {
    char buffer[BUFFER_SIZE];
    char name[32];
    int stopped = 0;

    client_t *client = (client_t *)arg;

    recv(client->fd, name, 32, 0);

    strcpy(client->name, name);
    sprintf(buffer, "%s has joined!\n", client->name);
    printf("%s", buffer);
    send_message_to_all_but(buffer, client->id);


    bzero(buffer, BUFFER_SIZE);

    while (1) {
        if(stopped) break;

        int receive = recv(client->fd, buffer, BUFFER_SIZE, 0);
        if(receive > 0) {
            if(strlen(buffer) > 0) {
                char *command = strtok(buffer, " ");
                if(strcmp(command, "LIST") == 0) {
                    char client_list[BUFFER_SIZE] = "Active clients: ";

                    pthread_mutex_lock(&clients_mutex);
                    for(int i=0; i<MAX_CLIENTS; ++i) {
                        if(clients[i]) {
                            strcat(client_list, clients[i]->name);
                            strcat(client_list, "; ");
                        }
                    }
                    strcat(client_list, "\n");
                    pthread_mutex_unlock(&clients_mutex);

                    send(client->fd, client_list, strlen(client_list), 0);
                }
                else if(strcmp(command, "2ALL") == 0) {
                    char *msg = strtok(NULL, "");
                    sprintf(buffer, "%s: %s\n", client->name, msg);
                    send_message_to_all_but(buffer, client->id);
                }
                else if(strcmp(command, "2ONE") == 0) {
                    char *other_name = strtok(NULL, " ");
                    char *msg = strtok(NULL, "");
                    int other_id = -1;

                    pthread_mutex_lock(&clients_mutex);
                    for(int i=0; i<MAX_CLIENTS; ++i) {
                        if(clients[i] && strcmp(clients[i]->name, other_name) == 0) {
                            other_id = clients[i]->id;
                            write(clients[i]->fd, msg, strlen(msg));
                            break;
                        }
                    }
                    pthread_mutex_unlock(&clients_mutex);

                    if(other_id == -1) {
                        sprintf(buffer, "User not found!\n");
                        send(client->fd, buffer, strlen(buffer), 0);
                    }
                }
                else if(strcmp(command, "STOP") == 0) {
                    sprintf(buffer, "%s has left!\n", client->name);
                    printf("%s", buffer);
                    send_message_to_all_but(buffer, client->id);
                    stopped = 1;
                }
                else {
                    sprintf(buffer, "Wrong command!\n");
                    send(client->fd, buffer, strlen(buffer), 0);
                }
            }
        }
        bzero(buffer, BUFFER_SIZE);
    }

    close(client->fd);

    // remove client
    pthread_mutex_lock(&clients_mutex);
    for(int i=0; i<MAX_CLIENTS; ++i) {
        if(clients[i] && clients[i]->id == client->id) {
            clients[i] = NULL;
            num_clients--;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    free(client);
    pthread_detach(pthread_self());

    return NULL;
}