//
// Created by mateusz-krol on 4/26/24.
//

#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <mqueue.h>
#include <fcntl.h>

int main() {
    // POSIX
    mq_unlink("/queue");
    mqd_t mq;

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 100;

    mq = mq_open("/queue", O_WRONLY | O_CREAT, 0666, &attr);

    char buffer[100];
    char message[100];

    strcpy(buffer, "Hello World!");
    mq_send(mq, buffer, sizeof(buffer), 0);
    printf("Sent: %s\n", buffer);

    strcpy(buffer, "Let's go ahead!");
    mq_send(mq, buffer, sizeof(buffer), 0);
    printf("Sent: %s\n", buffer);

    mq_close(mq);

    mq = mq_open("/queue", O_RDONLY, 0666, &attr);

    for(int i=1; i<=2; ++i) {
        mq_receive(mq, message, sizeof(message), NULL);
        printf("Received: %s\n", message);
    }

    mq_close(mq);
    mq_unlink("queue");

    return 0;
}
