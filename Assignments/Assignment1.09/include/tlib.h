#ifndef TLIB_H
#define TLIB_H
#define N_trnrs_sw "--numtrainers"
#define _CRITICAL_HIT_MSSG "A critical hit!"

#include "player.h"

#define max(x, y) ({            \
        typeof (x) _x = x;      \
        typeof (y) _y = y;      \
        (_x > _y ? _x : _y);    \
})

#define min(x, y) ({            \
        typeof (x) _x = x;      \
        typeof (y) _y = y;      \
        (_x < _y ? _x : _y);    \
})

#define OUT_OF_TERR_LIMITS(r,c) (r < 1 || r > ROWS - 2 || c < 1 || c > COLUMNS - 3) 

//Comparators

bool cmp_pkm_tps_ids(Pokemon_Types_data * t1, int val);

//Structs

typedef struct pos{
    int r,c;
} pos_t;


//Basic functions

void swap(void * a,void * b);
std::string PKM_USE(std::string, std::string);

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
