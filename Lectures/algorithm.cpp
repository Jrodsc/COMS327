#include <iostream>
#include <algorithm>

using namespace std;

typedef void (*fp1)(void);
typedef fp1 (*fp2)(void);

void f1(void){
    cout << __PRETTY_FUNCTION__ << endl;
}

fp1 f2(void){
    cout << __PRETTY_FUNCTION__ << endl;

    return f1;
}

fp2 f3(void){
    cout << __PRETTY_FUNCTION__ << endl;

    return f2;
}

class lots_of_parens{
    public:
        lots_of_parens operator()(){ 
            cout << __PRETTY_FUNCTION__ << endl;
            return *this; 
        }
};

bool is_greater_than_6(int i){
    return i > 6;
}
bool is_greater_than_4(int i){
    return i > 4;
}

class greater_than{
    private:
        int value;
    public:
        greater_than(int i) : value(i) {}
        bool operator()(int i) {
            return i > value;
        }
};

int main(int argc, char * argv[]){
    f2()()();

    return 0;
}
