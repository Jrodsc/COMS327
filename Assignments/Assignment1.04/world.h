#ifndef WORLD_H
#define WORLD_H

#define HEIGHT 401
#define WIDTH 401
#define INF 1000000000

typedef struct World{
    map_t * world[HEIGHT][WIDTH];
    char trainers[ROWS][COLUMNS];
    unsigned long long int cost_pc [ROWS][COLUMNS];
    unsigned long long int cost_hiker [ROWS][COLUMNS];
    unsigned long long int cost_rival [ROWS][COLUMNS];
    
} world_t;

void map_print_terrain(map_t * m);
int go_world(world_t * w_t, int x, int y);
int set_trnrs_map(world_t * w_t, map_t * m);
int update_trnrs_map(world_t * w_t, map_t * m);

#endif
