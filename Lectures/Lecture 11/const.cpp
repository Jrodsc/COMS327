#include <iostream>

/*
const int * p;          //The addessed data cannot be changed
int * const p;          //The pointer variable cannot be changed
const int * const p;    //Neither can be changed
*/

using namespace std;

int main(int argc, char * argv[]){
    const int i = 0;

    *((int *) &i) = 1;

    cout << i << endl;

    return 0;
}
