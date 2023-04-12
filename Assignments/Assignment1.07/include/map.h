#ifndef MAP_H
#define MAP_H

#define MAX_TRAINERS 15
#define INF 1000000000
#define ROWS 21
#define COLUMNS 81 

class map{
    public:
        char terr[ROWS][COLUMNS];
        int rx, ry;
        int w_exit, e_exit, n_exit, s_exit, cross_r, cross_c;
        pos_t rand_pos;
        int n_trnrs;
        npc arr_trnr[MAX_TRAINERS];
        
        void init();
        void paths_generation();
        void elements();
        void set_exits(char c, int val);
        void set(int a, int b, int numtrnrs_sw, int rand_pc);

        pos_t get_rand_pos(char c);
        void dijkstra(int rp, int cp, int t, unsigned long long int dist[ROWS][COLUMNS]);

        void set_trainers(int num);
};

#endif
