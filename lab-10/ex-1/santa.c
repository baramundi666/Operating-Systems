#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

pthread_mutex_t santa_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t santa_cond = PTHREAD_COND_INITIALIZER;

int reindeers = 0;
int deliveries = 0;

void* reindeer_thread(void* id) {
    int reindeer_id = *(int*)id;
    while(deliveries < 4) {
        sleep(rand()%6 + 5);

        pthread_mutex_lock(&santa_mutex);
        reindeers++;
        char form[20];
        switch(reindeers) {
            case 1: strcpy(form, "renifer");
                    break;
            case 2:
            case 3: strcpy(form, "renifery");
                    break;
            default: strcpy(form, "reniferów");
                    break;
        }
        printf("Renifer: %d %s czeka na Świętego Mikołaja, ID: %d\n", reindeers, form, reindeer_id);

        if(reindeers == 9) {
            printf("Renifer: wybudzam Świętego Mikołaja, ID: %d\n", reindeer_id);
            pthread_cond_signal(&santa_cond);
        }
        pthread_mutex_unlock(&santa_mutex);

        sleep(rand()%3 + 2);
    }
    return NULL;
}

void* santa_thread(void* arg) {
    while(deliveries < 4) {
        pthread_mutex_lock(&santa_mutex);

        while(reindeers < 9) {
            printf("Święty Mikołaj: śpię\n");
            pthread_cond_wait(&santa_cond, &santa_mutex);
        }

        reindeers = 0;
        deliveries++;
        printf("Święty Mikołaj: budzę się\n");
        printf("Święty Mikołaj: dostarczam zabawki\n");
        pthread_mutex_unlock(&santa_mutex);

        sleep(rand()%3 + 2);
        printf("Święty Mikołaj: zasypiam\n");
    }
    return NULL;
}

int main() {
    srand(time(NULL));

    pthread_t santa;
    pthread_t reindeers_threads[9];
    int reindeer_ids[9];

    pthread_create(&santa, NULL, santa_thread, NULL);

    for(int i=0; i<9; ++i) {
        reindeer_ids[i] = i;
        pthread_create(&reindeers_threads[i], NULL, reindeer_thread, &reindeer_ids[i]);
    }

    pthread_join(santa, NULL);

    for(int i=0; i<9; ++i) {
        pthread_join(reindeers_threads[i], NULL);
    }

    pthread_mutex_destroy(&santa_mutex);
    pthread_cond_destroy(&santa_cond);

    return 0;
}
