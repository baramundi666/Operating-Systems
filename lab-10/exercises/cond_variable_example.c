#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

static int glob = 0;
static int N = 5;

pthread_mutex_t mutex;
pthread_mutexattr_t mutexattr;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;


void* do_stuff(void *arg) {
//    int which = *((int*) arg);
    pthread_mutex_lock(&mutex);
    while(glob<N) {
        pthread_cond_wait(&cond, &mutex);
    }
    printf("Done waiting!\n");
    pthread_mutex_unlock(&mutex);
}


int main() {
    pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_ERRORCHECK);
    pthread_mutex_init(&mutex, &mutexattr);

    int n = 10;
    pthread_t* threads = calloc(sizeof(pthread_t), n);
    int ids[n];
    for(int i=0; i<n; ++i) {
        ids[i] = i;
    }
    for(int i=0; i<n; ++i) {
        pthread_create(threads+i, NULL, do_stuff, ids+i);
    }

    pthread_mutex_lock(&mutex);
    while(1) {
        sleep(1);
        glob++;
        if(glob == N) {
            pthread_cond_broadcast(&cond);
            printf("Broadcast!\n");
            break;
        }
    }
    pthread_mutex_unlock(&mutex);

    for(int i=0; i<n; ++i) {
        pthread_join(threads[i], NULL);
    }

    free(threads);
    return 0;
}