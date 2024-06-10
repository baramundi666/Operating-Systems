#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <bits/waitflags.h>
#include <stdbool.h>
#include <mqueue.h>
#include <string.h>

#define SHARED_MEMORY_PREFIX "printer_sh%d"
#define SEMAPHORE_PREFIX "printer_sem%d"
#define BUFF_SIZE 1024
#define MESSAGE_SIZE 11
#define MAX_PRINTERS 10
#define MAX_CLIENTS 10
#define QUEUE_NAME "/queue"
#define MAX_MSG 3

volatile bool close_ = false;

typedef struct {
    int client_num;
    char message[MESSAGE_SIZE];
} Data;

int clients_num;
int printers_num;
sem_t *semaphores[MAX_PRINTERS];
int shared_mem[MAX_PRINTERS];
Data *addresses[MAX_PRINTERS];
pid_t parent = NULL;
mqd_t queue;

void print_message(Data *data, int i, int is_from_queue) {
    for (int j=0; j<MESSAGE_SIZE-1; ++j) {
        if (is_from_queue) {
            printf("Client: %d (from the queue), Printer: %d, Char: %c\n", data->client_num, i, data->message[j]);
        }
        else {
            printf("Client: %d, Printer: %d, Char: %c\n", data->client_num, i, data->message[j]);
        }
        sleep(1);
    }
    sem_post(semaphores[i]);
}

int main(int argc, char **argv)
{
    if (argc < 3) return -1;

    clients_num = atoi(argv[1]);
    printers_num = atoi(argv[2]);

    printf("Number of clients: %d\nNumber of printers: %d\n", clients_num, printers_num);

    struct mq_attr attr;
    attr.mq_curmsgs = 0;
    attr.mq_flags = O_NONBLOCK;
    attr.mq_maxmsg = MAX_MSG;
    attr.mq_msgsize = 2 * sizeof(Data);

    queue = mq_open(QUEUE_NAME, O_RDONLY | O_CREAT, S_IWUSR | S_IRUSR, &attr);

    for(int i=0; i<printers_num; ++i) {
        char sem_name[BUFF_SIZE];
        char sh_name[BUFF_SIZE];

        sprintf(sem_name, SEMAPHORE_PREFIX, i);
        sprintf(sh_name, SHARED_MEMORY_PREFIX, i);

        semaphores[i] = sem_open(sem_name, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, 1);

        shared_mem[i] = shm_open(sh_name, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);


        ftruncate(shared_mem[i], sizeof(Data));

        addresses[i] = mmap(NULL, sizeof(Data), PROT_READ | PROT_WRITE, MAP_SHARED,
                            shared_mem[i], 0);
    }
    parent = getpid();

    for(int i=0; i<printers_num; ++i) {
        pid_t child = fork();

        if(child == 0) {
            while (!close_) {
                int valp;
                sem_getvalue(semaphores[i], &valp);
                if(valp == 0) {
                    print_message(addresses[i], i, false);
                }
                else {
                    Data msg;
                    memset(&msg, 0, sizeof(Data));
                    struct mq_attr attr;
                    mq_getattr(queue, &attr);
                    if (attr.mq_curmsgs > 0) {
                        mq_receive(queue, (char *)&msg, 2 * sizeof(Data), NULL);
                        sem_wait(semaphores[i]);
                        print_message(&msg, i, true);
                    }
                }
            }
            exit(0);
        }
    }

    while (wait(NULL) > 0);

    for(int i=0; i<printers_num; ++i){
        sem_close(semaphores[i]);

        char sem_name[BUFF_SIZE];
        char sh_name[BUFF_SIZE];

        sprintf(sem_name, SEMAPHORE_PREFIX, i);
        sprintf(sh_name, SHARED_MEMORY_PREFIX, i);
        sem_unlink(sem_name);
        munmap(addresses[i], sizeof(Data));
    }
}