//Net-ID: jrodsc
//Rodrigo Santamaria
//COM S 327 Assignment1.02

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "tlib.h"
#include "queue.h"
#include "map.h"

#define HEIGHT 401
#define WIDTH 401

map_t * world[HEIGHT][WIDTH];

int go(int x, int y){
    if(x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return 0;
    
    if(NULL == world[y][x]){
        world[y][x] = malloc(sizeof(*world[x][y]));
        map_set_exits(world[y][x], 'a', -1);
        if(x + 1 >= WIDTH){
             map_set_exits(world[y][x], 'e', -2);
        }else{
            if(NULL != world[y][x + 1]){
                map_set_exits(world[y][x], 'e', world[y][x + 1] -> w_exit);
            }
        }
        if(x - 1 < 0){
             map_set_exits(world[y][x], 'w', -2);
        }else{
            if(NULL != world[y][x - 1]){
                map_set_exits(world[y][x], 'w', world[y][x - 1] -> e_exit);
            } 
        }
        if(y + 1 >= HEIGHT){
             map_set_exits(world[y][x], 's', -2);
        }else{
            if(NULL != world[y + 1][x]){
                map_set_exits(world[y][x], 's', world[y + 1][x] -> n_exit);
            }
        }
        if(y - 1 < 0){ 
             map_set_exits(world[y][x], 'n', -2);
        }else{
            if(NULL != world[y - 1][x]){
                map_set_exits(world[y][x], 'n', world[y - 1][x] -> s_exit);
            }
        }
        map_set(world[y][x]);
    }

    return 1;
}

int main(int argc, char * argv[]){

    srand(time(NULL));
    
    int i,j;
    for(i = 0; i < 401; i++){
        for(j = 0; j < 401; j++){
            world[i][j] = NULL;
        }
    }

    world[200][200] = malloc(sizeof(*world[200][200]));

    map_set_exits(world[200][200],'a',-1);
    map_set(world[200][200]);
    
    int nx,ny, x = 0,y = 0,curr_x = 200, curr_y = 200;
    char lc = 'z',c = 'z';
    bool succs = true;


    while(true){
        map_print_terrain(world[curr_y][curr_x]);
        
        switch(lc){
            case 'n':
                if(succs){
                    printf("You moved north!\n");
                }else{
                    printf("You are already in the top\n");    
                }
                break;
            case 's':
                if(succs){
                    printf("You moved south!\n");
                }else{
                    printf("You are already in the bottom\n");    
                }
                break;
            case 'e':
                if(succs){
                    printf("You moved east!\n");
                }else{
                    printf("You are already in the right border!\n");    
                }
                break;
            case 'w':
                if(succs){
                    printf("You moved west!\n");
                }else{
                    printf("You are already in the left border!\n");    
                }
                break;
            case 'f':
                if(succs){
                    printf("You flew to <%d,%d>!\n", x,y);
                }else{
                    printf("It looks like you tried to go out of the world!\n");    
                }
                break;
            case 'q':
                printf("Thank you for playing!\n");
                exit(1);
                break;
            default:
                printf("\n");
        }
        printf("Current coordinate: <%d, %d>", x,y);
        printf("\nIngrese un comando: ");
        fflush(stdout);
        scanf("%c", &c);

        if(c == 'n' || c == 's' || c == 'w' || c == 'e' || c == 'f') lc = c;

        switch(c){
            case 'n':
                if(go(curr_x,curr_y - 1)){
                    curr_y--;
                    succs = true;
                    y++;
                }else{
                    succs = false;
                }
                break;
            case 's':
                if(go(curr_x,curr_y + 1)){
                    curr_y++;
                    y--;
                    succs = true;
                }else{
                    succs = false;
                }
                break;
            case 'w':
                if(go(curr_x - 1,curr_y)){
                    curr_x--;
                    x--;
                    succs = true;
                }else{
                    succs = false;
                }
                break;
            case 'e':
                if(go(curr_x + 1,curr_y)){
                    curr_x++;
                    x++;
                    succs = true;
                }else{
                    succs = false;
                }
                break;
            case 'f':
                scanf("%d %d", &nx,&ny);
                if(go(nx + 200,200 - ny)){
                    curr_y = 200 - ny;
                    y = ny;
                    curr_x = nx + 200;
                    x = nx;
                    succs = true;
                }else{
                    succs = false;
                }
                break;
            case 'q':
                exit(1);
                break;
        
        }

    }
    return 0;
}
