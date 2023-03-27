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

class character{
    public:
        char r, c, txt, last_move;
};

class npc: public character{
    public:
        char type, state;
        int init(char type, int r, int c, char txt, char last_move);
};

class Pc: public character{
    public:
        char cell_type;
        int init(int r,int c, char txt, char last_move);
};

#endif 
