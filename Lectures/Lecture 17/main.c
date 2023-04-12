#include "cpp-function.h"

int main(int argc, char *argv[]){
    ostream *o = get_cout();
    use_cout(o, "Hello World!\n");

    return 0;
}
