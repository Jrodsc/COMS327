#ifndef MAP_H
#define MAP_H

#define MAX_TRAINERS 15
#define INF 1000000000
#define ROWS 21
#define COLUMNS 81 


//Global variables

typedef struct map{
    //terrain 
    char terr[ROWS][COLUMNS];
    int rx, ry;
    int w_exit, e_exit, n_exit, s_exit, cross_r, cross_c;
    pos_t rand_pos;
    int n_trnrs;
    trainer_t arr_trnr[MAX_TRAINERS];
} map_t;
//Process functions

void map_paths_generation(map_t * m);
void map_elements(map_t * m);
void map_init(map_t * m);
void map_set_exits(map_t * m, char c, int val);
void map_set(map_t * m,int a,int b, int numtrnrs_sw);
pos_t get_rand_pos(map_t * m, char c);
void dijkstra(int rp,int cp,map_t * m, int t,unsigned long long int dist[ROWS][COLUMNS]);

#endif
