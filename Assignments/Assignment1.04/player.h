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
}trainer_t;

int trainer_init(trainer_t * t, char type, int r, int c, char txt);

#endif 
