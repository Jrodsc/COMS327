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
int numtrnrs_sw = 0, curr_x, curr_y;

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

int navigation(int x, int y){
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

void in_market(){
    printw("You are inside the Poke Market,\t (Press < to exit)\n");
    refresh();
    while(getch() != '<'){
        move(22,0);
        printw("Invalid command");
        refresh();
        usleep(500000);
        deleteln();
    }
}

void in_center(){
    printw("You are inside of the Poke Center,\t (Press < to exit)\n");
    refresh();
    while(getch() != '<'){
        move(22,0);
        printw("Invalid command");
        refresh();
        usleep(500000);
        deleteln();
    }
}

void show_list(){
    int start = 0, selected = 0, i, r, c, rt, ct, vert_loc, hor_loc, move;
    char type;
    wchar_t rep = 'a';

    init_pair(1, COLOR_BLACK, COLOR_RED);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    init_pair(3, COLOR_BLACK, COLOR_BLUE);
    map_t * m = universe.world[curr_y][curr_x];
    r = universe.pc.r;
    c = universe.pc.c;

    move(r,c);

    attron(COLOR_PAIR(3));
    addch('@');
    attroff(COLOR_PAIR(3));

    move = 1;
    move(21,0);
    printw("Trainers:\n");
    do{
        if(move == 1){
            for(i = 0; i < 3 && start + i < m -> n_trnrs; i++){
                rt = m -> arr_trnr[start + i].r;
                ct = m -> arr_trnr[start + i].c;
                type = m -> arr_trnr[start + i].txt; 
                vert_loc = r - rt;
                hor_loc = c - ct;
                move(21 + i, 12);
                clrtoeol();
                if(start + i == selected)
                    attron(COLOR_PAIR(1));
                printw("%c, %d %s, %d %s",type, abs(vert_loc), ((vert_loc < 0) ? "south" : "north"), abs(hor_loc), ((hor_loc < 0) ? "east" : "west"));
                if(start + i == selected)
                    attroff(COLOR_PAIR(1));
            }
        }

        move(m -> arr_trnr[selected].r, m -> arr_trnr[selected].c);
        
        attron(COLOR_PAIR(1));
        addch(m -> arr_trnr[selected].txt);
        attroff(COLOR_PAIR(1));
    
        rep = getch();

        move(m -> arr_trnr[selected].r, m -> arr_trnr[selected].c);

        attron(COLOR_PAIR(2));
        addch(m -> arr_trnr[selected].txt);
        attroff(COLOR_PAIR(2));

        if(rep == KEY_UP){
            if(selected) move = 1;
            else move = 0;
            if(selected > 0 && selected < m -> n_trnrs - 1) start = max(0, start - 1);
            selected = max(0, selected - 1);
        }else if(rep == KEY_DOWN){
            if(selected < m -> n_trnrs - 1) move = 1;
            else move = 0;
            if(selected > 0 && selected < m -> n_trnrs - 1) start = min(max(m -> n_trnrs - 3,0), start + 1); 
            selected = min(m -> n_trnrs - 1, selected +1);
        }

        refresh();
    }while(rep != 27);
}

void in_fight(trainer_t * tr){
    init_pair(1, COLOR_BLACK, COLOR_RED);
    
    move(tr -> r, tr -> c);
    attron(COLOR_PAIR(1));
    addch(tr -> txt);
    attron(COLOR_PAIR(2));

    move(21,0);

    printw("You are n a Pokemon battle,\t (Press ESC to exit and defeat your rival!)");
    refresh();
    while(getch() != 27){
        move(22,0);
        printw("Invalid command");
        refresh();
        usleep(500000);
        deleteln();
    }

    tr -> state = 1;

}

void game_loop(){
    int result, in_b;
    char c;
    trainer_t * tr;

    initscr();
    ESCDELAY = 1;
    curs_set(0);
    keypad(stdscr, TRUE);
    resizeterm(ROWS + 3, COLUMNS - 1);
    noecho();
    getch();


    do{
        map_print_terrain(&universe, universe.world[curr_y][curr_x],1);
        
        dijkstra((int)universe.pc.r, (int)universe.pc.c,universe.world[curr_y][curr_x], PC, universe.cost_pc);
        if(universe.pc.cell_type != '~'){ 
            dijkstra((int)universe.pc.r, (int)universe.pc.c,universe.world[curr_y][curr_x], RIVAL, universe.cost_rival),
            dijkstra((int)universe.pc.r, (int)universe.pc.c,universe.world[curr_y][curr_x], HIKER, universe.cost_hiker);
        }
        dijkstra((int)universe.pc.r, (int)universe.pc.c,universe.world[curr_y][curr_x], SWIMMER, universe.cost_swimmer);

        c = getch();

        result = update_pc_map(&universe, universe.world[curr_y][curr_x], c);
        switch(result){
            case 0:
                in_b = update_trnrs_map(&universe, universe.world[curr_y][curr_x]);
                if(in_b){
                    tr = &universe.world[curr_y][curr_x] -> arr_trnr[in_b - 1];
                    map_print_terrain(&universe, universe.world[curr_y][curr_x],0);
                    in_fight(tr);
                    deleteln();
                }
                break;
            case 1:
                move(21,0);
                printw("Invalid command");
                refresh();
                usleep(500000);
                deleteln();
                break;
            case 2:
                map_print_terrain(&universe, universe.world[curr_y][curr_x],0);
                in_market();
                break;
            case 3:
                map_print_terrain(&universe, universe.world[curr_y][curr_x],0);
                in_center();
                break;
            case 4:
                map_print_terrain(&universe, universe.world[curr_y][curr_x],0);
                show_list();
                break;
            default:
                if(result >= 1000000){
                    tr = &universe.world[curr_y][curr_x] -> arr_trnr[result - 1000000];
                    if(!tr -> state){
                        map_print_terrain(&universe, universe.world[curr_y][curr_x],0);
                        in_fight(tr);
                    }else{
                        move(21,0);
                        printw("You already defeated this trainer!");
                        refresh();
                        usleep(500000);
                        deleteln();
                    }
                }else{
                     
                }
        }
    }while(result != -1);

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
    curr_x = curr_y = 200;

    universe.world[200][200] = malloc(sizeof(*universe.world[200][200]));

    map_set_exits(universe.world[200][200],'a',-1);
    map_set(universe.world[200][200], 0, 0, numtrnrs_sw); 
    set_trnrs_map(&universe, universe.world[200][200]);
    pc_init(&universe.pc, universe.world[200][200] -> rand_pos.r, universe.world[200][200] -> rand_pos.c, '@', (char)(rand()%8));
    universe.trainers[(int)universe.pc.r][(int)universe.pc.c] = '@'; 
    


    game_loop();

    return 0;
}
