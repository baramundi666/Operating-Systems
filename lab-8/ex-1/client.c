#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
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
pid_t parent;
mqd_t queue;

int main(int argc, char **argv) {
    if (argc < 3) return 1;

    clients_num = atoi(argv[1]);
    printers_num = atoi(argv[2]);

    printf("Number of clients: %d\nNumber of printers: %d\n", clients_num, printers_num);

    struct mq_attr attr;
    attr.mq_curmsgs = 0;
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MSG;
    attr.mq_msgsize = 2 * sizeof(Data);

    queue = mq_open(QUEUE_NAME, O_RDWR, S_IWUSR | S_IRUSR, &attr);

    for(int i=0; i<printers_num; ++i) {
        char sem_name[BUFF_SIZE];
        char sh_name[BUFF_SIZE];

        sprintf(sem_name, SEMAPHORE_PREFIX, i);
        sprintf(sh_name, SHARED_MEMORY_PREFIX, i);
        semaphores[i] = sem_open(sem_name, O_RDWR, S_IRUSR | S_IWUSR, 1);

        shared_mem[i] = shm_open(sh_name, O_RDWR, S_IRUSR | S_IWUSR);

        ftruncate(shared_mem[i], sizeof(Data));
        addresses[i] = mmap(NULL, sizeof(Data), PROT_READ | PROT_WRITE,
                             MAP_SHARED, shared_mem[i], 0);
    }
    parent = getpid();

    setpgid(getpid(), getpid());

    for(int i=0; i<clients_num; ++i) {
        pid_t child = fork();
        if(child == 0) {
            srand(time(NULL) + i);
            while(!close_) {
                char message[MESSAGE_SIZE];
                sleep(rand()%15 + 2);

                for(int j=0; j<MESSAGE_SIZE-1; ++j) {
                    message[j] = 'a' + rand()%26;
                }
                message[MESSAGE_SIZE - 1] = '\0';

                int id = 0;
                int success = false;
                while(id < printers_num) {
                    int a = sem_trywait(semaphores[id]);
                    if(a == 0) {
                        strcpy((addresses[id]->message), message);
                        addresses[id]->client_num = i;

                        printf("Client: %d, Print: %s\n", addresses[id]->client_num, addresses[id]->message);
                        success = true;
                        break;
                    }
                    id++;
                }

                if(!success) {
                    printf("Client %d: sending message %s to the queue\n", i, message);
                    Data data;
                    memset(&data, 0, sizeof(Data));
                    data.client_num = i;
                    memcpy(data.message, message, strlen(message));
                    struct mq_attr attr;

                    mq_getattr(queue, &attr);

                    mq_send(queue, (char *)&data, sizeof(data), 10);
                }
            }
            exit(0);
        }
    }

    while (wait(NULL));

    for(int i=0; i<printers_num; ++i) {
        sem_close(semaphores[i]);
        char sem_name[BUFF_SIZE];
        char sh_name[BUFF_SIZE];

        sprintf(sem_name, SEMAPHORE_PREFIX, i);
        sprintf(sh_name, SHARED_MEMORY_PREFIX, i);
        sem_unlink(sem_name);

        munmap(addresses[i], sizeof(Data));

        shm_unlink(sh_name);
    }
}