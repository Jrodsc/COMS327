#include <iostream>

using namespace std;

class A {
    public:
        A() { cout << __PRETTY_FUNCTION__ << endl;}
        virtual ~A() { cout << __PRETTY_FUNCTION__ << endl;}
        virtual print() { cout << __PRETTY_FUNCTION__ << endl;}
};
class B {
    public:
        B() { cout << __PRETTY_FUNCTION__ << endl;}
        virtual ~B() { cout << __PRETTY_FUNCTION__ << endl;}
        virtual print() { cout << __PRETTY_FUNCTION__ << endl;}
};
class C {
    public:
        C() { cout << __PRETTY_FUNCTION__ << endl;}
        virtual ~C() { cout << __PRETTY_FUNCTION__ << endl;}
        virtual print() { cout << __PRETTY_FUNCTION__ << endl;}
};
class D {
    public:
        D() { cout << __PRETTY_FUNCTION__ << endl;}
        virtual ~D() { cout << __PRETTY_FUNCTION__ << endl;}
        virtual print() { cout << __PRETTY_FUNCTION__ << endl;}
};

int main(int argc, char * argv[]){
    A a;

    a.print();

    return 0;
}
