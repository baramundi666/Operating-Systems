#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

static int glob = 0;
pthread_mutex_t mutex;
pthread_mutexattr_t mutexattr;

void* do_stuff(void *arg) {
    int which = *((int*) arg);
    while(1) {
        pthread_mutex_lock(&mutex);
        if(pthread_mutex_lock(&mutex) != 0) {
            printf("Cannot lock mutex!\n");
        }
        glob++;
        printf("Thread no %d has incremented glob variable to %d\n", which, glob);
        pthread_mutex_unlock(&mutex);
        pthread_mutex_unlock(&mutex);
    }
}


int main() {
    pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_ERRORCHECK);
    pthread_mutex_init(&mutex, &mutexattr);
    int n = 2;
    pthread_t* threads = calloc(sizeof(pthread_t), n);
    int ids[n];
    for(int i=0; i<n; ++i) {
        ids[i] = i;
    }
    for(int i=0; i<n; ++i) {
        pthread_create(threads+i, NULL, do_stuff, ids+i);
    }

    for(int i=0; i<n; ++i) {
        pthread_join(threads[i], NULL);
    }

    free(threads);
    return 0;
}