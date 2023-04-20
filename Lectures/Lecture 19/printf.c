#include <stdint.h>

int printf(const  char * format, ...){
    va_list ap;

    va_start(ap, format);

    puts("I'm in your program, fiddling with your bitz.\n");
    vprintf(format, ap);

    va_end(ap);
}
