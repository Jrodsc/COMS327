#ifndef WORLD_H
#define WORLD_H

#define HEIGHT 401
#define WIDTH 401
#define INF 1000000000

class World{
    public:
        map * world[HEIGHT][WIDTH];
        Pc pc;
        char trainers[ROWS][COLUMNS];
        unsigned long long int cost_pc      [ROWS][COLUMNS];
        unsigned long long int cost_hiker   [ROWS][COLUMNS];
        unsigned long long int cost_rival   [ROWS][COLUMNS];
        unsigned long long int cost_swimmer [ROWS][COLUMNS];
        
        void print_terrain(map * m, int color);
        int go_world(int x, int y, int numtrnrs_sw, int move_t);
        int set_trnrs_map(map * m);
        int update_trnrs_map(map * m);
        int update_pc_map(map * m, char cx);
};


#endif
