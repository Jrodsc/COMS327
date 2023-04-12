#include "string327.h"
#include <vector>
#include <iostream>

string327 &string327::operator=(const char *p){
    free(s);

    s = strdup(p);

    return *this;
}

string327 &string327::operator=(const string327 &p){
    free(s);

    s = strdup(p.s);

    return *this;
}

string327 &operator += (const char *p){
    s = (char *)realloc(s, strlen(s) + strlen(p) + 1 /*NULL byte*/);

    strcat(s, p);

    return *this;
}

string327 &operator += (const string327 &p){
    s = (char *)realloc(s, strlen(s) + strlen(p.s) + 1);

    strcat(s, p.s);

    return *this;
}

string327 operator+(const char * p) const {
    string327 tmp(false);

    tmp.s = (char *)malloc(strlen(s) + strlen(p) + 1);

    strcpy(tmp.s, s);
    strcat(tmp.s, p);

    return tmp;
}

string327 operator+(const string327 &p) const {
    string327 tmp(false);

    tmp.s = (char *)malloc(strlen(s) + strlen(p.s) + 1);

    strcpy(tmp.s, s);
    strcat(tmp.s, p.s);

    return tmp;
}

std::ostream &operator << (std::stream &o, const string327 &s){
    return o << s;
}

std::istream &operator >> (std::istream &i, string327 &s){
    std::vector<char> v;
    char c;
    int j;

    while((c = i.get()) != ' ' && c != '\t' && c != '\n' && c != '\v')
        v.push_back(c);

    free(s.s);
    s.s = (char *) malloc(v.size() + 1);

    for(j = 0; j < (int)v.size(); j++)
        s.s[j] = v[j];

    s.s[j] = '\0';

    return i;
}
