#include <dlfcn.h>
#include <stddef.h>
int main() {
    void *handle = dlopen("libstringlibrary.so", RTLD_LAZY);
    if(!handle){/*error*/}

    void (*printMessage)();
    printMessage = (void (*)())dlsym(handle,"printMessage");

    if(dlerror() != NULL){/*error*/}

    (*printMessage)();

    dlclose(handle);
}