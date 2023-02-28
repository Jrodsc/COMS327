#include <stdlib.h>
#include <stdio.h>
#include "tlib.h"
#include "player.h"
#include "map.h"
#include "priority_queue.h"
#include "queue.h"
#include "world.h"


int go_world(world_t * w_t, int x, int y){
    if(x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return 0;
    
    if(NULL == w_t -> world[y][x]){
        w_t -> world[y][x] = malloc(sizeof(*w_t -> world[x][y]));
        map_set_exits(w_t -> world[y][x], 'a', -1);
        if(x + 1 >= WIDTH){
             map_set_exits(w_t -> world[y][x], 'e', -2);
        }else{
            if(NULL != w_t -> world[y][x + 1]){
                map_set_exits(w_t -> world[y][x], 'e', w_t -> world[y][x + 1] -> w_exit);
            }
        }
        if(x - 1 < 0){
             map_set_exits(w_t -> world[y][x], 'w', -2);
        }else{
            if(NULL != w_t -> world[y][x - 1]){
                map_set_exits(w_t -> world[y][x], 'w', w_t -> world[y][x - 1] -> e_exit);
            } 
        }
        if(y + 1 >= HEIGHT){
             map_set_exits(w_t -> world[y][x], 's', -2);
        }else{
            if(NULL != w_t -> world[y + 1][x]){
                map_set_exits(w_t -> world[y][x], 's', w_t -> world[y + 1][x] -> n_exit);
            }
        }
        if(y - 1 < 0){ 
             map_set_exits(w_t -> world[y][x], 'n', -2);
        }else{
            if(NULL != w_t -> world[y - 1][x]){
                map_set_exits(w_t -> world[y][x], 'n', w_t -> world[y - 1][x] -> s_exit);
            }
        }
        map_set(w_t -> world[y][x],200 - x,200 - y);
    }

    return 1;
}
