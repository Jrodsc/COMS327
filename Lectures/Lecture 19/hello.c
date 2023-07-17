#include <stdio.h>
/*__attribute__((constructor))*/
/*void init() {*/
    /*printf("Running %s\n", __FUNCTION__);*/
/*}*/

/*__attribute__((destructor))*/
/*void destroy() {*/
    /*printf("Running %s\n", __FUNCTION__);*/
/*}*/

int main(int argc, char * argv[]){
    /*printf("Running %s\n", __FUNCTION__);*/

    printf("Hello world!\n");

    return 0;
}
