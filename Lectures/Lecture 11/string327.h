#ifndef STRING327_H
#define STRING327_H

class string327 {
    private:
        char * s;
    
    public:
        string327() {}
        string327(const char * p){}
        string327(const string327 &p){}

        ~string327() {} //destructor

        bool operator == (const string327 &p) {}
        bool operator <= (const string327 &p) {}
        bool operator >= (const string327 &p) {}
        bool operator != (const string327 &p) {}
        bool operator < (const string327 &p) {}
        bool operator > (const string327 &p) {}

        string327 & operator = (const string327 &p) {}
        string327 & operator = (const char * p) {}
        string327 operator + (const string327 &p) {}
        string327 operator + (const char * p) {}
        string327 & operator += (const string327 &p) {}

        //Add methods to print; overload << and >>
};
#endif
