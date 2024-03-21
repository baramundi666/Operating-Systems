#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BUFFER_SIZE 1024

void reverse(FILE* in_file, FILE* out_file);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        return 1;
    }

    clock_t start, cut_off;
    double cpu_time_used;
    FILE *in_file, *out_file;
    in_file = fopen(argv[1], "rb");
    out_file = fopen(argv[2], "wb");

    start = clock();
    reverse(in_file, out_file);
    cut_off = clock();
    cpu_time_used = ((double) (cut_off - start)) / CLOCKS_PER_SEC;
    printf("reverse_1: %f seconds\n", cpu_time_used);
    fclose(in_file);
    fclose(out_file);
    return 0;
}
#ifdef VER_1
void reverse(FILE* in_file, FILE* out_file){
    fseek(in_file, 0, SEEK_END);
    long size = ftell(in_file);

    for(long i=size-1; i>=0; i--) {
        fseek(in_file, i, SEEK_SET);
        char c;
        fread(&c, sizeof(char), 1, in_file);
        fwrite(&c, sizeof(char), 1, out_file);
    }
}
#endif
#ifndef VER_1
void reverse(FILE *in_file, FILE *out_file) {
    fseek(in_file, 0, SEEK_END);
    long size = ftell(in_file);
    long block_count = size/BUFFER_SIZE;
    long cut_off = size%BUFFER_SIZE;

    if(cut_off > 0) {
        fseek(in_file, -cut_off, SEEK_END);
        char buffer[cut_off];
        fread(buffer, sizeof(char), cut_off, in_file);
        char tmp;
        for(int i=0;i<cut_off/2;i++) {
            tmp = buffer[i];
            buffer[i] = buffer[cut_off-i-1];
            buffer[cut_off-i-1] = tmp;
        }
        fwrite(buffer, sizeof(char), cut_off, out_file);
    }

    for(long i=1; i<=block_count; i++) {
        fseek(in_file, -cut_off-i*BUFFER_SIZE, SEEK_END);
        char buffer[BUFFER_SIZE];
        fread(buffer, sizeof(char), BUFFER_SIZE, in_file);
        char tmp;
        for(int i=0;i<BUFFER_SIZE/2;i++) {
            tmp = buffer[i];
            buffer[i] = buffer[BUFFER_SIZE-i-1];
            buffer[BUFFER_SIZE-i-1] = tmp;
        }
        fwrite(buffer, sizeof(char), BUFFER_SIZE, out_file);
    }
}
#endif
