#include "string327.h"

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

