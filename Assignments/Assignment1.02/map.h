#ifndef MAP_H
#define MAP_H

#define INF 1000000000
#define ROWS 21
#define COLUMNS 81 


//Global variables

typedef struct map{
    char terr[ROWS][COLUMNS];
    int w_exit, e_exit, n_exit, s_exit, cross_r, cross_c;
} map_t;
//Process functions

void paths_generation(map_t * m);
void elements(map_t * m);
void init(map_t * m);
void print_terrain(map_t * m);


#endif
