#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main() {
    double start, end;
    int n;
    printf("start = ");
    scanf("%lf", &start);
    printf("end = ");
    scanf("%lf", &end);
    printf("n = ");
    scanf("%d", &n);
    printf("\n");

    mkfifo("pipeline", 0666);

    int write_fd = open("pipeline", O_WRONLY);
    write(write_fd, &start, sizeof(start));
    write(write_fd, &end, sizeof(end));
    write(write_fd, &n, sizeof(n));
    close(write_fd);

    double result;
    int read_fd = open("pipeline", O_RDONLY);
    read(read_fd, &result, sizeof(result));
    close(read_fd);

    printf("Result: %lf\n", result);

    return 0;
}