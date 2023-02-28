#include <stdio.h>
#include <stdarg.h>

/*printf knockoff which converts ints (d), chars(c), string (s), and 
 * floats (f). Only formats specifiers in format. No % */

void foo(const char * format, ...){
    va_list ap;
    int i;
    char c;
    char * s;
    float f;


    va_start(ap, format);

    while(*format){
        switch(*format){
            case 'd':
                i = va_arg(ap, int);
                printf("%d ", i);
                break;
            case 's':
                s = va_arg(ap, char *);
                printf("%s ", s);
                break;
            case 'c':
                c = va_arg(ap, int);
                printf("%c ", c);
                break;
            case 'f':
                f = va_arg(ap, double);
                printf("%f ", f);
                break;
        }
        format++;
    }

    printf("\n");

    va_end(ap);
}

int main(int argc, char * argv[]){
    foo("ssd", "hello", "world!", 5);
    foo("f", 3.14);
    foo("ccc", 'a', 'b', 'c');
    return 0;
}
