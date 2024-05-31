#include <ncurses.h>
#include <locale.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include "grid.h"

typedef struct {
    char **src;
    char **dst;
    int start_row;
    int end_row;
} ThreadData;

const int grid_width = 30;
const int grid_height = 30;
int num_threads;

void handler(int signo) {}


void *thread_func(void *arg) {
    signal(SIGUSR1, handler);
    ThreadData* data = (ThreadData*) arg;

    while(true) {
        pause();
        update_grid(*data->src, *data->dst, data->start_row, data->end_row);
    }
    return NULL;
}

void create_threads(pthread_t* threads, ThreadData* data) {
    for(int i=0; i<num_threads; ++i) {
        pthread_create(&threads[i], NULL, thread_func, &data[i]);
    }
}

int main(int argc, char **argv) {
    if (argc != 2) return 1;

    num_threads = atoi(argv[1]);

    if (num_threads <= 0 || num_threads > grid_height) return 1;

    srand(time(NULL));
    setlocale(LC_CTYPE, "");
    initscr();

    char *foreground = create_grid();
    char *background = create_grid();
    char *tmp;

    init_grid(foreground);

    ThreadData data[num_threads];
    int rows_per_thread = grid_height/num_threads;
    for(int i=0; i<num_threads; ++i) {
        data[i].src = &foreground;
        data[i].dst = &background;
        data[i].start_row = i*rows_per_thread;
        data[i].end_row = (i == num_threads-1) ? grid_height : (i+1)*rows_per_thread;
    }

    pthread_t threads[num_threads];
    create_threads(threads, data);

    while(true) {
        draw_grid(foreground);

        for(int i=0; i<num_threads; ++i) {
            pthread_kill(threads[i], SIGUSR1);
        }

        usleep(500 * 1000);

        tmp = foreground;
        foreground = background;
        background = tmp;
    }

    endwin(); // End curses mode
    destroy_grid(foreground);
    destroy_grid(background);

    return 0;
}
