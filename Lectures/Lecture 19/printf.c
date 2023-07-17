#include <stdint.h>
#include <stdarg.h>
#include <dlfcn.h>
#include <stdlib.h>

int (*myprintf)(const char * format, ...);
static void * handle;

__attribute__((constructor))
void init() {
    printf("Running %s\n", __FUNCTION__);
}

__attribute__((destructor))
void destroy() {
    printf("Running %s\n", __FUNCTION__);
}

void init()
{
    printf("RUnning %s\n", __FUNCTION__);

    if(!(handle = dlopen("libc.so.6", RTLD_LAZY))){
        fprintf(stderr, "%s\n", dlerror());

        exit(-1);
    }
    if(!(myprintf = dlsym(handle, "printf"))){
        fprintf(stderr, "%s\n", dlerror());
        exit(1);
    }
}

/*int printf(const  char * format, ...){*/
    /*va_list ap;*/
    /*int retval;*/

    /*va_start(ap, format);*/

    /*puts("I'm in your program, fiddling with your bitz.\n");*/

    /*retval = vprintf(format, ap);*/

    /*va_end(ap);*/

    /*return retval;*/
/*}*/


