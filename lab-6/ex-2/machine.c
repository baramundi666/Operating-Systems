#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

double f(double x);
double integral(double (*f)(double) , double start, double end, double step);

int main() {
    double a, b;
    int n;

    int read_fd = open("pipeline", O_RDONLY);
    read(read_fd, &a, sizeof(a));
    read(read_fd, &b, sizeof(b));
    read(read_fd, &n, sizeof(n));
    close(read_fd);

    double step = (b-a)/n;
    double result = integral(f, a, b, step);

    int write_fd = open("pipeline", O_WRONLY);
    write(write_fd, &result, sizeof(result));
    close(write_fd);

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