#include <stdio.h> 
#include <stdlib.h>

struct foo{
    int i;
    float f;
    char *s;
};

void use_foo(struct foo *f){
    f -> i = 1;
    f -> f = 1.41;
    f -> s = "Goodbye, cruel world.";
}

int main(int argc, char * argv[]){

    struct foo f;
    struct foo *p;

    f.i = 0;
    f.f = 3.14;
    f.s = "Hello world!";

    printf("%d %f %s\n", f.i, f.f, f.s);

    use_foo(&f);

    printf("%d %f %s\n", f.i, f.f, f.s);

    p = &f;

    printf("%d %f %s\n", p->i, p->f, p->s);

    p = malloc(sizeof(*p));

    printf("%d %f %s\n", p->i, p->f, p->s);

    use_foo(p);

    printf("%d %f %s\n", p->i, p->f, p->s);
    
    free(p);

    //real numbers
    
    srand(time(NULL));

    printf("%d\n", rand());
    printf("%d\n", rand());
    printf("%d\n", rand());
    printf("%d\n", rand());
    printf("%d\n", rand());
    return 0;
}
