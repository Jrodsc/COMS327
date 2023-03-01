#include <stdlib.h>
#include <stdio.h>
#include "player.h"

int trainer_init(trainer_t * t, char type, int r, int c,char txt){
   // if(!(t = malloc(sizeof(t)))){
   //     return 1;
   // }
    
    t -> type = type;
    t -> r = r;
    t -> c = c;
    t -> txt = txt;
    return 0;
}


