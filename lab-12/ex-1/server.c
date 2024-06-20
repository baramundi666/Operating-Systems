#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <pthread.h>

#define BACKLOG 5
#define MAX_CLIENTS 3
#define BUFFER_SIZE 1024

typedef struct {
    struct sockaddr_in address;
    socklen_t address_size;
    int id;
    char name[32];
} client_t;

int sfd;
int num_clients = 0;
client_t *clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void handle_client(char* buf, int size, client_t* client);


int main(int argc, char *argv[]) {
    if(argc != 2) return 1;

    int port = atoi(argv[1]);


    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;


    sfd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    int option = 1;
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (char*) &option, sizeof(option));

    bind(sfd, (struct sockaddr*) &server_addr, sizeof(server_addr));


    for (;;) {
        // accept new client
        char buf[BUFFER_SIZE];
        client_t* client;
        socklen_t client_len = sizeof(client_addr);;
        int size = recvfrom(sfd, buf, BUFFER_SIZE, 0,
                       (struct sockaddr*) &client_addr, &client_len);
        int found = 0;
        for(int i=0; i<MAX_CLIENTS; ++i) {
            if(clients[i] &&
            memcmp(&clients[i]->address, &client_addr, sizeof(struct sockaddr_in)) == 0) {
                client = clients[i];
                found = 1;
                break;
            }
        }
        if(found == 1) { // old client
            buf[size] = '\0';
            handle_client(buf, size+1, client);
        }
        else { // new client
            num_clients++;

            // too many clients
            if(num_clients > MAX_CLIENTS) {
                num_clients--;
                printf("Max clients!\n");
                continue;
            }

            // create client
            client = (client_t*) malloc(sizeof(client_t));
            client->address = client_addr;
            client->address_size = client_len;
            strcpy(client->name, buf);

            // add client
            pthread_mutex_lock(&clients_mutex);
            for(int i=0; i<MAX_CLIENTS; ++i) {
                if(!clients[i]) {
                    client->id = i;
                    clients[i] = client;
                    break;
                }
            }
            pthread_mutex_unlock(&clients_mutex);

            sprintf(buf, "%s has joined!\n", client->name);
            printf("%s", buf);

            pthread_mutex_lock(&clients_mutex);
            for(int i=0; i<MAX_CLIENTS; ++i) {
                if(clients[i] && clients[i]->id != client->id) {
                    sendto(sfd, buf, strlen(buf), 0,
                           (struct sockaddr*) &clients[i]->address, clients[i]->address_size);
                }
            }
            pthread_mutex_unlock(&clients_mutex);
        }
    }

    return 0;
}

void handle_client(char* buffer, int size, client_t* client) {
    char msg[BUFFER_SIZE];


    if(size > 0) {
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

                sendto(sfd, client_list, strlen(client_list), 0,
                       (struct sockaddr*) &client->address, client->address_size);
            }
            else if(strcmp(command, "2ALL\0") == 0) {
                sprintf(msg, "%s", strtok(NULL, ""));

                pthread_mutex_lock(&clients_mutex);
                for(int i=0; i<MAX_CLIENTS; ++i) {
                    if(clients[i] && clients[i]->id != client->id) {
                        sprintf(buffer, "%s: %s\n", client->name, msg);
                        sendto(sfd, buffer, strlen(buffer), 0,
                               (struct sockaddr*) &clients[i]->address,
                                       clients[i]->address_size);
                    }
                }
                pthread_mutex_unlock(&clients_mutex);
            }
            else if(strcmp(command, "2ONE\0") == 0) {
                char *other_name = strtok(NULL, " ");
                strcpy(msg, strtok(NULL, ""));
                int other_id = -1;

                pthread_mutex_lock(&clients_mutex);
                for(int i=0; i<MAX_CLIENTS; ++i) {
                    if(clients[i] && strcmp(clients[i]->name, other_name) == 0) {
                        other_id = clients[i]->id;
                        sprintf(buffer, "%s: %s\n", client->name, msg);
                        sendto(sfd, buffer, strlen(buffer), 0,
                               (struct sockaddr*) &clients[i]->address,
                               clients[i]->address_size);
                        break;
                    }
                }
                pthread_mutex_unlock(&clients_mutex);

                if(other_id == -1) {
                    sprintf(buffer, "User not found!\n");
                    sendto(sfd, buffer, strlen(buffer), 0,
                           (struct sockaddr*) &client->address,
                           client->address_size);
                }
            }
            else if(strcmp(command, "STOP") == 0) {
                sprintf(buffer, "%s has left!\n", client->name);
                printf("%s\n", buffer);
                pthread_mutex_lock(&clients_mutex);
                for(int i=0; i<MAX_CLIENTS; ++i) {
                    if(clients[i] && clients[i]->id != client->id) {
                        sendto(sfd, buffer, strlen(buffer), 0,
                               (struct sockaddr*) &clients[i]->address,
                               clients[i]->address_size);
                    }
                }
                pthread_mutex_unlock(&clients_mutex);

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

            }
            else {
                sprintf(buffer, "Wrong command!\n");
                sendto(sfd, buffer, strlen(buffer), 0,
                       (struct sockaddr*) &client->address,
                       client->address_size);
            }
        }
    }
        bzero(buffer, BUFFER_SIZE);
        bzero(msg, BUFFER_SIZE);
}