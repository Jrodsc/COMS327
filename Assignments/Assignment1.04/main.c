//Net-ID: jrodsc
//Rodrigo Santamaria
//COM S 327 Assignment1.02

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>
#include "tlib.h"
#include "queue.h"
#include "priority_queue.h"
#include "player.h"
#include "map.h"
#include "world.h"

world_t universe;

int main(int argc, char * argv[]){

    srand(time(NULL));
    
    priority_queue_t pq;

    priority_queue_init(&pq);
    
    int i,j;
    for(i = 0; i < 401; i++){
        for(j = 0; j < 401; j++){
            universe.world[i][j] = NULL;
        }
    }

    universe.world[200][200] = malloc(sizeof(*universe.world[200][200]));

    map_set_exits(universe.world[200][200],'a',-1);
    map_set(universe.world[200][200], 0,0); 
    set_trnrs_map(&universe, universe.world[200][200]);
    pc_init(&universe.pc, universe.world[200][200] -> rand_pos.r, universe.world[200][200] -> rand_pos.c, '@', (char)(rand()%8));
    universe.trainers[(int)universe.pc.r][(int)universe.pc.c] = '@'; 

    int nx,ny, x = 0,y = 0,curr_x = 200, curr_y = 200;
    char lc = 'z',c = 'z';
    bool succs = true;


    while(true){
        map_print_terrain(&universe, universe.world[curr_y][curr_x]);
        
        usleep(250000);
        dijkstra((int)universe.pc.r, (int)universe.pc.c,universe.world[curr_y][curr_x], PC, universe.cost_pc);
        if(universe.world[curr_y][curr_y] -> terr[(int)universe.pc.r][(int)universe.pc.c] != '~') 
            dijkstra((int)universe.pc.r, (int)universe.pc.c,universe.world[curr_y][curr_x], RIVAL, universe.cost_rival),
            dijkstra((int)universe.pc.r, (int)universe.pc.c,universe.world[curr_y][curr_x], HIKER, universe.cost_hiker);
        
        dijkstra((int)universe.pc.r, (int)universe.pc.c,universe.world[curr_y][curr_x], SWIMMER, universe.cost_swimmer);

        /*
        for(i = 0; i < ROWS; i++){
            for(j = 0; j < 20; j++){
                printf("%llu\t", universe.cost_rival[i][j]);
            }
            printf("\n");
        }

        */

        update_trnrs_map(&universe, universe.world[curr_y][curr_x]);

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
        //scanf("%c", &c);
        fflush(stdout);

        if(c == 'n' || c == 's' || c == 'w' || c == 'e' || c == 'f') lc = c;

        switch(c){
            case 'n':
                if(go_world(&universe,curr_x,curr_y - 1)){
                    curr_y--;
                    succs = true;
                    y++;
                }else{
                    succs = false;
                }
                break;
            case 's':
                if(go_world(&universe,curr_x,curr_y + 1)){
                    curr_y++;
                    y--;
                    succs = true;
                }else{
                    succs = false;
                }
                break;
            case 'w':
                if(go_world(&universe,curr_x - 1,curr_y)){
                    curr_x--;
                    x--;
                    succs = true;
                }else{
                    succs = false;
                }
                break;
            case 'e':
                if(go_world(&universe,curr_x + 1,curr_y)){
                    curr_x++;
                    x++;
                    succs = true;
                }else{
                    succs = false;
                }
                break;
            case 'f':
                scanf("%d %d", &nx,&ny);
                if(go_world(&universe,nx + 200,200 - ny)){
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
