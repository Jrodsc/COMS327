#include <iostream>

using namespace std;

class shape {
    public:
        virtual ~shape() {}
        virtual void draw() = 0;
};

class square : public shape {
    virtual ~square() {}
    virtual void draw() { cout << "[]" << endl; }
};

class circle : public shape {
    virtual ~circle() {}
    virtual void draw() { cout << "o"  << endl; }
};

class green_square : public square {
    virtual ~green_square() {}
    virtual void draw() { cout << "green-[]" << endl; }
};

class blue_circle : public circle {
    virtual ~blue_circle() {}
    virtual void draw() { cout << "blue-o" << endl; }
};

//Instead of extending (is-a), we can "decorate" (has-a)

int main(int argc, char * argv[]){

    return 0;
}
