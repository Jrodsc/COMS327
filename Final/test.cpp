#include <iostream>

using namespace std;

int main(int argc, char * v[]){
    try{
        throw string("sjdhfskdhf");
    }catch(string s){
        cout << s << endl;
    }

    return 0;
}
