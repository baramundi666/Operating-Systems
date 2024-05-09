#include <stdio.h>
#include <fcntl.h>
#include <mqueue.h>
#include <string.h>

#define MAX_SIZE 100
#define MAX_CLIENTS 10

struct client {
    mqd_t queue_id;
    int client_id;
};

int main() {
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;

    mqd_t server_queue = mq_open("/queue", O_CREAT | O_RDWR, 0666, &attr);

    struct client clients[MAX_CLIENTS];
    int clients_count = 0;

    printf("Server started!\n");

    while(1) {
        char buffer[MAX_SIZE];
        mq_receive(server_queue, buffer, MAX_SIZE, NULL);

        if(strncmp(buffer, "INIT", 4) == 0) {
            char *client_queue_name = strtok(buffer + 5, " ");
            mqd_t client_queue = mq_open(client_queue_name, O_WRONLY);

            clients[clients_count].queue_id = client_queue;
            clients[clients_count].client_id = clients_count+1;
            clients_count++;
            char buffer[MAX_SIZE];
            sprintf(buffer, "%d", clients_count);
            mq_send(client_queue, buffer, MAX_SIZE, 0);
            printf("Client no %d connected to the server!\n", clients_count);
        }
        else {
            printf("Sending \"%s\" to all the clients\n", buffer);
            for(int i=0; i<clients_count; ++i) {
                mq_send(clients[i].queue_id, buffer, MAX_SIZE, 0);
            }
        }
    }
}
