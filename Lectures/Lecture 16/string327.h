#ifndef STRING327_H
#define STRING327_H

#include <cstdlib>
#include <cstring>
#include <iostream>

class string327{
    private:
        char * s;

        string327(bool alloc) { s = (alloc ? strdup("") : NULL); }

    public:
        string327() { s = strdup(""); }
        string327(const char * p) { s = strdup(p); }
        string327(const string327 &p) { s = strdup(p.s); }
        ~string327() { free(s); }

        int length() const { return strlen(s); }

        bool operator==(const string327 &t) const { return strcmp(s, t.s) == 0; }
        bool operator!=(const string327 &t) const { return strcmp(s, t.s) != 0; }
        bool operator<(const string327 &t) const { return strcmp(s, t.s) < 0; }
        bool operator<=(const string327 &t) const { return strcmp(s, t.s) <= 0; }
        bool operator>(const string327 &t) const { return strcmp(s, t.s) > 0; }
        bool operator>=(const string327 &t) const { return strcmp(s, t.s) >= 0; }
        
        string327 &operator=(const char *p);
        string327 &operator=(const string327 &p);
        string327 &operator += (const char *p);
        string327 &operator += (const string327 &p);
        string327 operator+(const char * p) const;
        string327 operator+(const string327 &p) const;

        char &operator[](int i) { return *(s + i);}

        const char *c_str() { return s; }

        friend std::ostream &operator << (std::ostream &o, const string327 &s);
};

#endif
