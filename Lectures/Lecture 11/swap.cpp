#include <iostream>

using namespace std;

void cswap(int * x, int * y){
    int tmp = *x;
    *x = *y;
    *y = tmp;
}

void cppswap(int &x, int &y){
    int tmp = x;
    x = y;
    y = tmp;
}

/* By convention, many (most?) programmers will pass by address if 
 * they intend to change the value of an item in the called function, and pass by const reference
 * if they will not be changing the referenced data
 */

struct foo{
    int i;
    float f;
} f;

int main(int argc, char * argv[]){

    int i, j;

    i = 0;
    j = 1;

    cout << i << " " << j << endl;

    cswap(&i,&j);

    cout << i << " " << j << endl;

    cppswap(i,j);

    cout << i << " " << j << endl;

    return 0;
}
