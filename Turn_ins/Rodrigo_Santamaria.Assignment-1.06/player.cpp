#include <stdlib.h>
#include <stdio.h>
#include "player.h"

int npc::init(char type, int r, int c,char txt, char last_move){
   // if(!(t = malloc(sizeof(t)))){
   //     return 1;
   // }
    
    this -> type = type;
    this -> r = r;
    this -> c = c;
    this -> txt = txt;
    this -> last_move = last_move;
    return 0;
}

int Pc::init(int r, int c, char txt, char last_move){
    this -> r = r;
    this -> c = c;
    this -> txt = txt;
    this -> last_move = last_move;
    this -> cell_type = '#';

    return 0;
}
