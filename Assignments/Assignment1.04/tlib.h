#ifndef TLIB_H
#define TLIB_H

//Structs

typedef struct pos{
    int r,c;
} pos_t;


//Basic functions

int min(int  a, int  b);
int max(int a, int b);
void swap(void * a,void * b);

//Color functions

void red();
void black();
void green();
void yellow();
void blue();
void purple();
void cyan();
void white();
void reset();

#endif
