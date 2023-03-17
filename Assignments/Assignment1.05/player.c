#include <stdlib.h>
#include <stdio.h>
#include "player.h"

int trainer_init(trainer_t * t, char type, int r, int c,char txt, char last_move){
   // if(!(t = malloc(sizeof(t)))){
   //     return 1;
   // }
    
    t -> type = type;
    t -> r = r;
    t -> c = c;
    t -> txt = txt;
    t -> last_move = last_move;
    return 0;
}

int pc_init(pc_t * t, int r, int c, char txt, char last_move){
    t -> r = r;
    t -> c = c;
    t -> txt = txt;
    t -> last_move = last_move;
    t -> cell_type = '#';

    return 0;
}
