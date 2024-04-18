#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

double f(double x);
double integral(double (*f)(double) , double start, double end, double step);

int main(int argc, char* argv[]) {
    if(argc != 3) return 1;

    double step = atof(argv[1]);
    int n = atoi(argv[2]);

    int fds[n][2];

    double a = 0.0, b = 1.0;
    double h = (b-a)/n;

    clock_t start_time, end_time;
    start_time = clock();

    for(int i=0; i<n; ++i) {
        pipe(fds[i]);
        pid_t child_pid = fork();
        if(child_pid == 0) {
            close(fds[i][0]);
            double y = integral(f, a+i*h, a+(i+1)*h, step);
            write(fds[i][1], &y, sizeof(y));
            close(fds[i][1]);
            exit(0);
        }
        else {
            close(fds[i][1]);
        }
    }

    double result = 0.0;

    for (int i=0; i<n; ++i) {
        double y;
        if(read(fds[i][0], &y, sizeof(y))<0) {
            printf("Failed to read\n");
            return 1;
        }
        close(fds[i][0]);
        result+=y;
    }

    end_time = clock();
    double time = (double)(end_time-start_time)/CLOCKS_PER_SEC;

    printf("Result: %lf\n", result);
    printf("Time: %lf\n\n", time);

    return 0;
}

double f(double x) {
    return 4/(x*x+1);
}

double integral(double (*f)(double) , double start, double end, double step) {
    double result = 0.0;
    for(double x=start; x<=end; x+=step) {
        result+=f(x+step/2.0)*step;
    }

    return result;
}