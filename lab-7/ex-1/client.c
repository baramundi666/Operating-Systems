#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <mqueue.h>
#include <string.h>
#include <unistd.h>

#define MAX_SIZE 100

int main() {
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    char client_queue_name[50];
    sprintf(client_queue_name, "/client_queue_%d", getpid());

    mqd_t client_queue = mq_open(client_queue_name, O_CREAT | O_RDWR, 0666, &attr);

    mqd_t server_queue = mq_open("/queue", O_WRONLY);

    char buffer[MAX_SIZE];
    sprintf(buffer, "INIT %s", client_queue_name);
    mq_send(server_queue, buffer, MAX_SIZE, 0);

    mq_receive(client_queue, buffer, MAX_SIZE, NULL);

    int client_id = atoi(buffer);
    printf("Connected to the server - client no %d\n", client_id);

    pid_t child_pid = fork();
    if (child_pid == 0) { // child
        while (1) {
            if (mq_receive(client_queue, buffer, MAX_SIZE, NULL) != -1) {
                printf("Client no %d received: %s\n", client_id, buffer);
            }
        }
    }
    else { // parent
        printf("Enter message to send:\n");
        while(1) {
            fgets(buffer, MAX_SIZE, stdin);
            if ((strlen(buffer) > 0) && (buffer[strlen(buffer) - 1] == '\n')) {
                buffer[strlen(buffer) - 1] = '\0';
            }

            mq_send(server_queue, buffer, MAX_SIZE, 0);
        }
    }
}

