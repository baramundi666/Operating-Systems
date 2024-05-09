//
// Created by mateusz-krol on 4/26/24.
//

#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <mqueue.h>

struct msgbuf {
    long mtype;
    char mtext[100];
};

int main() {
    // System V
    key_t key = ftok(".", 'p');
    int msgid = msgget(key, 0666 | IPC_CREAT);

    struct msgbuf buffer;
    struct msgbuf message;

    buffer.mtype = 1;
    strcpy(buffer.mtext, "Hello World!");
    msgsnd(msgid, (void*) &buffer, sizeof(buffer.mtext), 0);
    printf("Sent: %s\n", buffer.mtext);

    buffer.mtype = 2;
    strcpy(buffer.mtext, "Let's go ahead!");
    msgsnd(msgid, (void*) &buffer, sizeof(buffer.mtext), 0);
    printf("Sent: %s\n", buffer.mtext);

    struct msgid_ds buf;
    msgctl(msgid, IPC_STAT, &buf);

    for(int i=1; i<=2; ++i) {
        msgrcv(msgid, &message, sizeof(message), i, 0);
        printf("Received: %s\n", message.mtext);
    }

    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}
