#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>


#define BUFFER_SIZE 1024


int main(int argc, char** argv) {
    if(argc < 2) {
        return 1;
    }

    DIR* dir_ptr = opendir(argv[1]);
    if(dir_ptr == NULL){
        return 1;
    }

    size_t path_length = strlen(argv[1]);
    char path[BUFFER_SIZE];
    memcpy(path, argv[1], path_length);
    struct stat buffor_stat;
    long long total_size = 0;
    struct dirent* dir_file = readdir(dir_ptr);

    while(dir_file != NULL){
        size_t file_length = strlen(dir_file->d_name);
        memcpy(&path[path_length], dir_file->d_name, file_length);
        path[path_length + file_length] = 0;
        stat(path, &buffor_stat);

        if(!S_ISDIR(buffor_stat.st_mode)){
            printf("%s %8ld\n", path, buffor_stat.st_size);
            total_size += buffor_stat.st_size;
        }

        dir_file = readdir(dir_ptr);
    }

    printf("total: %lld\n", total_size);
    closedir(dir_ptr);
    return 0;
}
