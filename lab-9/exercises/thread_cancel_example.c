#include <stdio.h>
#include <pthread.h>

void* func(void *arg) {
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
    int i = 1;
    int thread = *((int*) arg);
    while(1) {
        sleep(1);
        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
        printf("msg1: Hello tu watek %d, Iteracja %d\n", thread, i);
        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
        printf("msg2: Hello tu watek %d, Iteracja %d\n", thread, i);
        i++;
    }
}

int main() {
    int N = 7;
    int n = 5;
    pthread_t threads[n];
    int ids[n];
    for(int i=0; i<n; ++i) {
        ids[i] = i;
    }
    for(int i=0; i<n; ++i) {
        pthread_create(&threads[i], NULL, func, &ids[i]);
    }
    int i = 1;
    while(1) {
        sleep(1);
        printf("Hello tu glowny watek, Iteracja %d\n", i);
        if(i == N) {
            for(int j=0; j<n; ++j) {
                pthread_cancel(threads[j]);
            }
            break;
        }
        i++;
    }
    return 0;
}