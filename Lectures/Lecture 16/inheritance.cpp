#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

class shape {
    public:
        virtual double area() const = 0;
        virtual double perimeter() const = 0;
        virtual ostream &print(ostream &o) const = 0;
        virtual ~shape() {}
};

ostream &operator<<(ostream &o, const shape &s){
    return s.print(o);
}

class rectangle : public shape {
    protected:
        double width, height;

    public:
        rectangle(double w, double h) : width(w), height(h) {}
        virtual double area() const {
            return width * height;
        }
        virtual double perimeter() const {
            return 2 * (width + height);
        }
        virtual ostream &print(ostream &o) const {
            return o << "rectangle[" << width << "," << height << "]";
        }
        virtual ~rectangle() {}
};
        
class square : public rectangle {
    public:
        square(double s) : rectangle(s, s) {}
        virtual ostream &print(ostream &o) const {
            return o << "square[" << width << "]";
        }
        virtual ~square() {};
};

class circle : public shape {
    private:
        double radius;
    public:
        circle(double r) : radius(r) {}
        virtual double area() const {
            return M_PI * radius * radius;
        }
        virtual double perimeter() const {
            return 2 * M_PI * radius;
        }
        virtual ostream &print(ostream &o) const {
            return o << "circle(" << radius << ")";
        }
        virtual ~circle() {}
};

int main(int argc, char * argv[]){
    
    square sq(5);
    int i,j;
    vector<shape *> v;
    shape * s;

    cout << sq << " A:" << sq.area() << " P:" << sq.perimeter() << endl;

    rectangle *rp = &sq;

    cout << *rp << " A:" << rp -> area() << " P:" << rp -> perimeter() << endl;

    rectangle &rr = sq;

    cout << rr << " A:" << rr.area() << " P:" << rr.perimeter() << endl;
    
    srand(time(NULL));

    for(i = 0; i < 1000; i++){
        switch(rand() % 3){
            case 0:
                v.push_back(new rectangle(rand() / ((double) RAND_MAX),rand() / ((double) RAND_MAX)));
                break;
            case 1:
                v.push_back(new square(rand() / ((double) RAND_MAX))); 
                break;
            case 2:
                v.push_back(new circle(rand() / ((double) RAND_MAX)));
                break;
        }
    } 

    vector<shape *>::iterator vi;
    for(vi = v.begin(); vi != v.end(); vi++){
        cout << **vi << " A:" << (*vi) -> area() << " P:" << (*vi) -> perimeter() << endl;
    }

    return 0;
}
