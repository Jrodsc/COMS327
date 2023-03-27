#ifndef PLAYER_H
#define PLAYER_H
#define PC 0
#define HIKER 1
#define RIVAL 2
#define PACER 4
#define WANDERER 8
#define SENTRIE 16
#define EXPLORER 32
#define SWIMMER 64

typedef struct trainer{
    char type, r, c, txt;
    char last_move;
    char state;
}trainer_t;

typedef struct pc{
    char r,c, txt;
    char last_move;
    char cell_type;
}pc_t;

int trainer_init(trainer_t * t, char type, int r, int c, char txt, char last_move);
int pc_init(pc_t * t, int r,int c, char txt, char last_move);
#endif 
