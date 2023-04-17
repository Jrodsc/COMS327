//Net-ID: jrodsc
//Rodrigo Santamaria
//COM S 327 Assignment1.02

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include <climits>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>
#include "tlib.h"
#include "queue.h"
#include "priority_queue.h"
#include "player.h"
#include "map.h"
#include "world.h"
#include "data_classes.h"

World universe;
Data db;
int numtrnrs_sw = 0, curr_x, curr_y;

void print_centered(WINDOW * win, int start_row, std::string text, int color){

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_BLUE);

    int adjusted_col = (COLUMNS - 1)/2 - (int)text.size()/2;

    move(start_row, adjusted_col);
    attron(COLOR_PAIR(color));
    printw(text.c_str());
    attroff(COLOR_PAIR(color));
}

int gen_rand_level(){
    int dist = abs(200 - curr_x) + abs(200 - curr_y); 
    
    return (dist <= 200) ? rand() % (dist/2 + 1) + 1: (dist - 200)/2 + rand() % (101 - (dist - 200)/2);
}

int scroll_list(std::string * arr, int sz){
    int selected, i, start;
    wchar_t c;

    init_pair(1, COLOR_BLACK, COLOR_BLUE);

    selected = start = 0;

    do{

        for(i = start; i < start + 3; i++){
            move(21 + i - start, 0);

            clrtoeol();
            if(i == selected)
                attron(COLOR_PAIR(1));
            printw("%s", arr[i].c_str());
            refresh();
            if(i == selected)
                attroff(COLOR_PAIR(1));
        }

        c = getch();

        if(c == KEY_UP){
            if(selected > 0 && selected < sz - 1) start = max(0, start - 1);
            selected = max(0, selected - 1);
        }
        if(c == KEY_DOWN){
            if(selected > 0 && selected < sz - 1) start = min(max(sz - 3,0), start + 1); 
            selected = min(sz - 1, selected +1);
        }                
        if(c == 'o'){
            return selected;
        }

    }while(c != 27);

    return -1;
}

void notify(char lc, int x, int y, int succs){
    refresh();
    move(21, 0);
    deleteln();
    switch(lc){
        case 'n':
            if(succs){
                printw("You moved north!");
            }else{
                printw("You are already in the top");    
            }
            break;
        case 's':
            if(succs){
                printw("You moved south!");
            }else{
                printw("You are already in the bottom");    
            }
            break;
        case 'e':
            if(succs){
                printw("You moved east!");
            }else{
                printw("You are already in the right border!");    
            }
            break;
        case 'w':
            if(succs){
                printw("You moved west!");
            }else{
                printw("You are already in the left border!");    
            }
            break;
        case 'f':
            if(succs){
                printw("You flew to <%d,%d>!", x,y);
            }else{
                printw("It looks like you tried to go out of the world!");    
            }
            break;
        case 'q':
            printf("Thank you for playing!");
            exit(1);
            break;
        default:
            printf("\n");
    }
    refresh();
    usleep(1000000);
    deleteln();
}

void notify(char lc, int succs){
    notify(lc, -1, -1, succs);
}

int navigation(){
    int x,y,succs;
    init_pair(1, COLOR_BLACK, COLOR_BLUE);

    move(universe.pc.r,universe.pc.c);

    attron(COLOR_PAIR(1));
    addch('@');
    attroff(COLOR_PAIR(1));

    refresh();
    move(21,0);
    printw("You are about to fly with Charizard! Insert the coordinates (x,y)");
    refresh();
    move(22,0);
    echo();
    curs_set(1);
    scanw("%d %d", &x, &y);
    deleteln();
    noecho(); 
    curs_set(0);

    if(universe.go_world(x + 200,200 - y, numtrnrs_sw, 4)){
        curr_y = 200 - y;
        curr_x = x + 200;
        succs = 1;
    }else{
        succs = 0;
    }

    notify('f',x,y,succs);
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

void show_pokemon_info(Pokemon * pkm_t){
    std::string text[15];
    text[0] = "Pokemon name : " + (std::string)pkm_t -> name;
    text[1] = "Level        : " + std::to_string(pkm_t -> level);
    text[2] = "Pokemon stats:";
    text[3] = "+ HP              : " + std::to_string(pkm_t -> stats -> hp);
    text[4] = "+ Attack          : " + std::to_string(pkm_t -> stats -> attack);
    text[5] = "+ Defense         : " + std::to_string(pkm_t -> stats -> defense);
    text[6] = "+ Special attack  : " + std::to_string(pkm_t -> stats -> sp_attack);
    text[7] = "+ Special defense : " + std::to_string(pkm_t -> stats -> sp_defense);
    text[8] = "+ Speed           : " + std::to_string(pkm_t -> stats -> speed);
    text[9] = "+ Accuracy        : " + std::to_string(pkm_t -> stats -> accuracy);
    text[10]= "+ Evasion        : " + std::to_string(pkm_t -> stats -> evasion);
    text[11]= "Move 1       : " + ((pkm_t -> moves_t[0] != INT_MAX) ? db.arr_mvs[pkm_t -> moves_t[0]] -> identifier : "-");
    text[12]= "Move 2       : " + ((pkm_t -> moves_t[1] != INT_MAX) ? db.arr_mvs[pkm_t -> moves_t[1]] -> identifier : "-");
    text[13]= "Move 3       : " + ((pkm_t -> moves_t[2] != INT_MAX) ? db.arr_mvs[pkm_t -> moves_t[2]] -> identifier : "-");
    text[14]= "Move 4       : " + ((pkm_t -> moves_t[3] != INT_MAX) ? db.arr_mvs[pkm_t -> moves_t[3]] -> identifier : "-");

    scroll_list(text, 15);
}

void show_list(){
    int start = 0, selected = 0, i, r, c, rt, ct, vert_loc, hor_loc, move_t;
    char type;
    wchar_t rep = 'a';

    init_pair(1, COLOR_BLACK, COLOR_RED);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    init_pair(3, COLOR_BLACK, COLOR_BLUE);
    map * m = universe.world[curr_y][curr_x];
    r = universe.pc.r;
    c = universe.pc.c;

    move(r,c);

    attron(COLOR_PAIR(3));
    addch('@');
    attroff(COLOR_PAIR(3));

    move_t = 1;
    move(21,0);
    printw("Trainers:\n");
    do{
        if(move_t == 1){
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
            if(selected) move_t = 1;
            else move_t = 0;
            if(selected > 0 && selected < m -> n_trnrs - 1) start = max(0, start - 1);
            selected = max(0, selected - 1);
        }else if(rep == KEY_DOWN){
            if(selected < m -> n_trnrs - 1) move_t = 1;
            else move_t = 0;
            if(selected > 0 && selected < m -> n_trnrs - 1) start = min(max(m -> n_trnrs - 3,0), start + 1); 
            selected = min(m -> n_trnrs - 1, selected +1);
        }

        refresh();
    }while(rep != 27);
}


int show_pokemon_list(Pokemon * * arr){
    std::string text[6];

    text[0] = "Pokemon 1: " + ((arr[0] != NULL) ? arr[0] -> name : "-");
    text[1] = "Pokemon 2: " + ((arr[1] != NULL) ? arr[1] -> name : "-");
    text[2] = "Pokemon 3: " + ((arr[2] != NULL) ? arr[2] -> name : "-");
    text[3] = "Pokemon 4: " + ((arr[3] != NULL) ? arr[3] -> name : "-");
    text[4] = "Pokemon 5: " + ((arr[4] != NULL) ? arr[4] -> name : "-");
    text[5] = "Pokemon 6: " + ((arr[5] != NULL) ? arr[5] -> name : "-");

    return scroll_list(text, 6);
}

void in_fight(npc * tr){
    int val;

    init_pair(1, COLOR_BLACK, COLOR_RED);
    
    move(tr -> r, tr -> c);
    attron(COLOR_PAIR(1));
    addch(tr -> txt);
    attron(COLOR_PAIR(2));

    while(tr -> add_pokemon(&db, 1 + rand() %1092, gen_rand_level())){
        if(rand() % 100 >= 60) break;
    }

    move(21, 0);
    printw("You are in a Pokemon battle,\t (Press ESC to exit and defeat your rival!)");
    refresh();

    usleep(250000);
    
    do{
        val = show_pokemon_list(tr -> pokemons);

        if(val != -1){
            if(tr -> pokemons[val] != NULL) 
                show_pokemon_info(tr -> pokemons[val]);
        }
    }while(val != -1); 

    tr -> state = 1;
}

void spawn_pokemon(){

    Pokemon * pkm_t = new Pokemon(&db, rand() % 1092 + 1, gen_rand_level());

    show_pokemon_info(pkm_t);
}

void select_initial(Pc * pc_t){
    int pks[3], i, selected;
    wchar_t c;


    print_centered(stdscr, 0, "Choose one of the following pokemon", 1);
    refresh();
    print_centered(stdscr, 1, "Use the arrows UP and DOWN to move the cursor", 1);
    refresh();
    print_centered(stdscr, 2, "Then press Enter to choose your pokemon!", 1);
    refresh();

    i = 0; 
    do{
        do{
            pks[i] = rand() % 1092 + 1;
        }while((i > 0 && pks[i - 1] == pks[i]) || (i > 1 && pks[i - 2] == pks[i]));
        i++;
    }while(i < 3);


    selected = 0;
    do{
        for(i = 0; i < 3; i++){
            if(i == selected)
                print_centered(stdscr, 4 + (i << 1), (db.arr_pkm[pks[i]] -> identifier).c_str(), 2);
            else 
                print_centered(stdscr, 4 + (i << 1), (db.arr_pkm[pks[i]] -> identifier).c_str(), 1);
        }

        c = getch();
        
        if(c == KEY_UP)
            selected = max(0, selected - 1);
        if(c == KEY_DOWN)
            selected = min(2, selected + 1);
    }while(c != '\n');

    pc_t -> add_pokemon(&db, pks[selected], 1);

    show_pokemon_info(pc_t -> pokemons[0]);
}

void game_loop(){
    int result, in_b;
    char c;
    npc * tr;

    initscr();
    ESCDELAY = 1;
    curs_set(0);
    keypad(stdscr, TRUE);
    resizeterm(ROWS + 3, COLUMNS - 1);
    noecho();
    getch();

    universe.print_terrain(universe.world[curr_y][curr_x],1);
    clear();
    select_initial(&universe.pc);


    do{
        universe.print_terrain(universe.world[curr_y][curr_x],1);
        
        universe.world[curr_y][curr_x] -> dijkstra((int)universe.pc.r, (int)universe.pc.c, PC, universe.cost_pc);
        if(universe.pc.cell_type != '~'){ 
            universe.world[curr_y][curr_x] -> dijkstra((int)universe.pc.r, (int)universe.pc.c, RIVAL, universe.cost_rival),
            universe.world[curr_y][curr_x] -> dijkstra((int)universe.pc.r, (int)universe.pc.c, HIKER, universe.cost_hiker);
        }
        universe.world[curr_y][curr_x] -> dijkstra((int)universe.pc.r, (int)universe.pc.c, SWIMMER, universe.cost_swimmer);

        c = getch();

        result = universe.update_pc_map(universe.world[curr_y][curr_x], c);
        switch(result){
            case 0:
                in_b = universe.update_trnrs_map(universe.world[curr_y][curr_x]);
                if(in_b){
                    tr = &universe.world[curr_y][curr_x] -> arr_trnr[in_b - 1];
                    universe.print_terrain(universe.world[curr_y][curr_x],0);
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
                universe.print_terrain(universe.world[curr_y][curr_x],0);
                in_market();
                break;
            case 3:
                universe.print_terrain(universe.world[curr_y][curr_x],0);
                in_center();
                break;
            case 4:
                universe.print_terrain(universe.world[curr_y][curr_x],0);
                show_list();
                break;
            case 5:
                universe.print_terrain(universe.world[curr_y][curr_x],0);
                navigation();
                break;
            case RAN_POKEMON:
                universe.print_terrain(universe.world[curr_y][curr_x],0);
                spawn_pokemon();
                break;
            default:
                if(result >= 1000000){
                    tr = &universe.world[curr_y][curr_x] -> arr_trnr[result - 1000000];
                    if(!tr -> state){
                        universe.print_terrain(universe.world[curr_y][curr_x],0);
                        in_fight(tr);
                    }else{
                        move(21,0);
                        printw("You already defeated this trainer!");
                        refresh();
                        usleep(500000);
                        deleteln();
                    }
                }
                else if(result >= 10){
                    result %= 10;
                    if(result == 0){
                        if(universe.go_world(curr_x + 1, curr_y, numtrnrs_sw, 0)){
                            curr_x++;
                            universe.print_terrain(universe.world[curr_y][curr_x],1);
                            notify('e',true);
                        }
                        else{
                            notify('e', false);
                        }
                    }else if(result == 1){
                        if(universe.go_world(curr_x, curr_y + 1, numtrnrs_sw, 1)){
                            curr_y++;
                            universe.print_terrain(universe.world[curr_y][curr_x],1);
                            notify('s', true);
                        }
                        else{
                            notify('s', false);
                        }
                    }else if(result == 2){
                        if(universe.go_world(curr_x - 1, curr_y, numtrnrs_sw, 2)){
                            curr_x--;
                            universe.print_terrain(universe.world[curr_y][curr_x],1);
                            notify('w', true);
                        }
                        else{
                            notify('w', false);
                        }
                    }else{
                        if(universe.go_world(curr_x, curr_y - 1, numtrnrs_sw, 3)){
                            curr_y--;
                            universe.print_terrain(universe.world[curr_y][curr_x],1);
                            notify('n', true);
                        }
                        else{
                            notify('n', false);
                        }
                    }
                }
        }
    }while(result != -1);

    endwin();

}

int main(int argc, char * argv[]){
    priority_queue_t pq;
    int i, j;

    numtrnrs_sw = MAX_TRAINERS;

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

    universe.world[200][200] = (map *)malloc(sizeof(*universe.world[200][200]));
    universe.pc.r = -1;
    universe.pc.c = -1;

    universe.world[200][200] -> set_exits('a',-1);
    universe.world[200][200] -> set(0, 0, numtrnrs_sw, 1); 
    universe.pc.init(universe.world[200][200] -> rand_pos.r, universe.world[200][200] -> rand_pos.c, '@', (char)(rand()%8));
    universe.set_trnrs_map(universe.world[200][200]);
    
    db.read_files();

    game_loop();
    
    return 0;
}
