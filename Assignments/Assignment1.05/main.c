//Net-ID: jrodsc
//Rodrigo Santamaria
//COM S 327 Assignment1.02

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
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
int numtrnrs_sw;

void notify(char lc, int x, int y, int succs){
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
}

int navigation(int curr_x, int curr_y,int x, int y){
    char lc,c;
    int nx,ny;
    printf("Current coordinate: <%d, %d>", x,y);
    printf("\nIngrese un comando: ");
    scanf("%c", &c);
    fflush(stdout);
    int succs;
    if(c == 'n' || c == 's' || c == 'w' || c == 'e' || c == 'f') lc = c;

    switch(c){
        case 'n':
            if(go_world(&universe,curr_x,curr_y - 1, numtrnrs_sw)){
                curr_y--;
                succs = 1;
                y++;
            }else{
                succs = 0;
            }
            break;
        case 's':
            if(go_world(&universe,curr_x,curr_y + 1, numtrnrs_sw)){
                curr_y++;
                y--;
                succs = 1;
            }else{
                succs = 0;
            }
            break;
        case 'w':
            if(go_world(&universe,curr_x - 1,curr_y, numtrnrs_sw)){
                curr_x--;
                x--;
                succs = 1;
            }else{
                succs = 0;
            }
            break;
        case 'e':
            if(go_world(&universe,curr_x + 1,curr_y, numtrnrs_sw)){
                curr_x++;
                x++;
                succs = 1;
            }else{
                succs = 0;
            }
            break;
        case 'f':
            scanf("%d %d", &nx,&ny);
            if(go_world(&universe,nx + 200,200 - ny, numtrnrs_sw)){
                curr_y = 200 - ny;
                y = ny;
                curr_x = nx + 200;
                x = nx;
                succs = 1;
            }else{
                succs = 0;
            }
            break;
        case 'q':
            exit(1);
            break;
    
    }
    notify(lc,x,y,succs);
    return succs;
}

void game_loop(int curr_x, int curr_y){
    char c;
    initscr();
    while(true){
        map_print_terrain(&universe, universe.world[curr_y][curr_x]);
        
        usleep(250000);
        dijkstra((int)universe.pc.r, (int)universe.pc.c,universe.world[curr_y][curr_x], PC, universe.cost_pc);
        if(universe.pc.cell_type != '~'){ 
            dijkstra((int)universe.pc.r, (int)universe.pc.c,universe.world[curr_y][curr_x], RIVAL, universe.cost_rival),
            dijkstra((int)universe.pc.r, (int)universe.pc.c,universe.world[curr_y][curr_x], HIKER, universe.cost_hiker);
        }
        dijkstra((int)universe.pc.r, (int)universe.pc.c,universe.world[curr_y][curr_x], SWIMMER, universe.cost_swimmer);

        c = getch();

        update_pc_map(&universe, universe.world[curr_y][curr_x], c);
        update_trnrs_map(&universe, universe.world[curr_y][curr_x]);
    }

    endwin();

}

int main(int argc, char * argv[]){
    priority_queue_t pq;
    int i, j;

    if(argc > 1){
        if(strcmp(argv[1], N_trnrs_sw) == 0){
            if(argc == 2){
                printf("One (int) parameter was expected after %s!", N_trnrs_sw );
            }else{
                numtrnrs_sw = atoi(argv[2]);
                if(numtrnrs_sw == 0){
                    printf("Invalid value for number of trainers, please insert a positive integer\n");
                }else
                    printf("%d trainers will be displayed!\n", numtrnrs_sw);
            }
        }else{
            printf("The switch %s is not valid, please read the manual for more information\n", argv[1]);
        }
        usleep(2000000);
    }


    srand(time(NULL));
    

    priority_queue_init(&pq);
    
    for(i = 0; i < 401; i++){
        for(j = 0; j < 401; j++){
            universe.world[i][j] = NULL;
        }
    }

    universe.world[200][200] = malloc(sizeof(*universe.world[200][200]));

    map_set_exits(universe.world[200][200],'a',-1);
    map_set(universe.world[200][200], 0, 0, numtrnrs_sw); 
    set_trnrs_map(&universe, universe.world[200][200]);
    pc_init(&universe.pc, universe.world[200][200] -> rand_pos.r, universe.world[200][200] -> rand_pos.c, '@', (char)(rand()%8));
    universe.trainers[(int)universe.pc.r][(int)universe.pc.c] = '@'; 
    
    game_loop(200,200);

    return 0;
}
