#include <stdio.h>

#ifdef DYNAMIC
    #include <dlfcn.h>
    #include <stddef.h>
#endif

int main() {
#ifndef DYNAMIC
    int collatz_conjecture(int input);
    int test_collatz_convergence(int input, int max_iter);
#endif
#ifdef DYNAMIC
    void* handle = dlopen("collatz/build/libcollatz_shared.so", RTLD_LAZY);
    if(!handle){
        printf("Error!");
        return 1;
    }

    int (*collatz_conjecture)(int input);
    int (*test_collatz_convergence)(int input, int max_iter);

    collatz_conjecture = dlsym(handle, "collatz_conjecture");
    test_collatz_convergence = dlsym(handle, "test_collatz_convergence");

    if(dlerror() != NULL){
        printf("Error!");
        return 1;
    }
#endif

    printf("%d\n", collatz_conjecture(5));
    printf("%d\n", test_collatz_convergence(10, 100));

#ifdef DYNAMIC
    dlclose(handle);
#endif
    return 0;
}

