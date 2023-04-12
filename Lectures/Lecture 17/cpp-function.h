#ifndef CPP_FUNCTION_H
#define CPP_FUNCTION_H

#ifdef __cplusplus

#include <iostream>

using namespace std;

extern "C"{

#else

typedef void ostream;

#endif


ostream *get_cout();
void use_cout(ostream *, const char *);

#ifdef __cplusplus
}
#endif

#endif
