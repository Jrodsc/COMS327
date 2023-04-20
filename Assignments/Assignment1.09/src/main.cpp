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
#include <cmath>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>
#include <algorithm>
#include <utility>
#include "tlib.h"
#include "queue.h"
#include "priority_queue.h"
#include "player.h"
#include "map.h"
#include "world.h"
#include "data_classes.h"

WINDOW * mssg_win,* bottom_left,* bottom_right,* trnr_pok_win,* pc_pok_win;
World universe;
Data db;
bool is_rand_pk;
std::string mssg_text[3];
std::string trnr_text[17];
std::string pc_text[17];
int numtrnrs_sw = 0, curr_x, curr_y;

void clear_loop_wins(){
    wclear(universe.win);
    wclear(mssg_win);
    wrefresh(mssg_win);
    wrefresh(universe.win);
}

void print_centered(WINDOW * win, int start_row, std::string text, int color){

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_BLUE);

    int adjusted_col = (win -> _maxx)/2 - (int)text.size()/2;

    wattron(win, COLOR_PAIR(color));
    mvwprintw(win, start_row, adjusted_col, text.c_str());
    wattroff(win, COLOR_PAIR(color));

    wrefresh(win);
}

int gen_rand_level(){
    int dist = abs(200 - curr_x) + abs(200 - curr_y); 
    
    return (dist <= 200) ? rand() % (dist/2 + 1) + 1: (dist - 200)/2 + rand() % (101 - (dist - 200)/2);
}

int scroll_list(WINDOW * win, std::string * arr, int sz, int lines, int row_start){
    int selected, i, start;
    wchar_t c;

    init_pair(1, COLOR_BLACK, COLOR_BLUE);

    selected = start = 0;

    do{

        for(i = start; i < start + lines && i < sz; i++){

            if(row_start == 0)wclrtoeol(win);
            if(i == selected)
                wattron(win, COLOR_PAIR(1));
            mvwprintw(win, row_start + i - start, row_start, "%s", arr[i].c_str());
            wrefresh(win);
            if(i == selected)
                wattroff(win, COLOR_PAIR(1));
        }

        c = wgetch(win);

        if(c == KEY_UP){
            if(sz > lines && selected > 0 && selected < sz - 1) start = max(0, start - 1);
            selected = max(0, selected - 1);
        }
        if(c == KEY_DOWN){
            if(sz > lines && selected > 0 && selected < sz - 1) start = min(max(sz - 3,0), start + 1); 
            selected = min(sz - 1, selected +1);
        }                
        if((c == 'o' || c == '\n') && arr[selected].back() != '-'){
            wclear(win);
            wrefresh(win);
            return selected;
        }

    }while(c != 27);

    wclear(win);
    wrefresh(win);

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

    wmove(universe.win, universe.pc.r,universe.pc.c);

    wattron(universe.win, COLOR_PAIR(1));
    waddch(universe.win, '@');
    wattroff(universe.win, COLOR_PAIR(1));

    wrefresh(universe.win);
    mvwprintw(mssg_win, 0, 0, "You are about to fly with Charizard! Insert the coordinates (x,y)");
    wmove(mssg_win, 1, 0);
    wrefresh(mssg_win);
    echo();
    curs_set(1);
    wscanw(mssg_win, "%d %d", &x, &y);
    wdeleteln(mssg_win);
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
    mvwprintw(mssg_win, 0, 0, "You are inside the Poke Market,\t (Press < to exit)\n");
    wrefresh(mssg_win);
    while(wgetch(mssg_win) != '<'){
        mvwprintw(mssg_win, 1, 0, "Invalid command");
        wrefresh(mssg_win);
        usleep(500000);
        wdeleteln(mssg_win);
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

    scroll_list(mssg_win, text, 15, 3, 0);
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

    wmove(universe.win, r,c);

    wattron(universe.win, COLOR_PAIR(3));
    waddch(universe.win, '@');
    wattroff(universe.win, COLOR_PAIR(3));

    move_t = 1;
    mvwprintw(mssg_win, 0, 0, "Trainers:\n");
    do{
        if(move_t == 1){
            for(i = 0; i < 3 && start + i < m -> n_trnrs; i++){
                rt = m -> arr_trnr[start + i].r;
                ct = m -> arr_trnr[start + i].c;
                type = m -> arr_trnr[start + i].txt; 
                vert_loc = r - rt;
                hor_loc = c - ct;
                wmove(mssg_win, i, 12);
                wclrtoeol(mssg_win);
                if(start + i == selected)
                    wattron(mssg_win, COLOR_PAIR(1));
                wprintw(mssg_win, "%c, %d %s, %d %s",type, abs(vert_loc), ((vert_loc < 0) ? "south" : "north"), abs(hor_loc), ((hor_loc < 0) ? "east" : "west"));
                if(start + i == selected)
                    wattroff(mssg_win, COLOR_PAIR(1));
            }
        }

        wmove(universe.win, m -> arr_trnr[selected].r, m -> arr_trnr[selected].c);
        
        wattron(universe.win, COLOR_PAIR(1));
        waddch(universe.win, m -> arr_trnr[selected].txt);
        wattroff(universe.win, COLOR_PAIR(1));
    
        wrefresh(universe.win);
        rep = wgetch(mssg_win);

        wmove(universe.win, m -> arr_trnr[selected].r, m -> arr_trnr[selected].c);

        wattron(universe.win, COLOR_PAIR(2));
        waddch(universe.win, m -> arr_trnr[selected].txt);
        wattroff(universe.win, COLOR_PAIR(2));

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

        wrefresh(universe.win);
        wrefresh(mssg_win);
    }while(rep != 27);
    wclear(mssg_win);
    wrefresh(mssg_win);
}


int show_pokemon_list(WINDOW * win, Pokemon * * arr, int offset){
    std::string text[6];

    text[0] = "Pokemon 1: " + ((arr[0] != NULL) ? arr[0] -> name : "-");
    text[1] = "Pokemon 2: " + ((arr[1] != NULL) ? arr[1] -> name : "-");
    text[2] = "Pokemon 3: " + ((arr[2] != NULL) ? arr[2] -> name : "-");
    text[3] = "Pokemon 4: " + ((arr[3] != NULL) ? arr[3] -> name : "-");
    text[4] = "Pokemon 5: " + ((arr[4] != NULL) ? arr[4] -> name : "-");
    text[5] = "Pokemon 6: " + ((arr[5] != NULL) ? arr[5] -> name : "-");

    return scroll_list(win, text, 6, 19, offset);
}

void show_player_info_batt(int pc_sel_pk){
    int pc_pk_idx = 0, pc_idx = 1;

    pc_text[0] = "Your information";
    print_centered(pc_pok_win, 1, pc_text[0].c_str(), 1);

    for(pc_pk_idx = 0; pc_pk_idx < 6; pc_pk_idx++){
        if(pc_pk_idx == pc_sel_pk){
            pc_text[pc_idx++] = "-Pokemon " + std::to_string(pc_pk_idx + 1) + "(in battle)";
            pc_text[pc_idx++] = "\t+Name  : " + universe.pc.pokemons[pc_sel_pk] -> name;
            pc_text[pc_idx++] = "\t+Level : " + std::to_string(universe.pc.pokemons[pc_sel_pk] -> level);
            pc_text[pc_idx++] = "\t+HP    : " + std::to_string(universe.pc.pokemons[pc_sel_pk] -> hp) + "/" + std::to_string(universe.pc.pokemons[pc_sel_pk] -> stats -> hp);
        }else{
            pc_text[pc_idx++] = "-Pokemon " + std::to_string(pc_pk_idx + 1);
        }
    }

    for(pc_idx = 1; pc_idx < 17; pc_idx++){
        mvwprintw(pc_pok_win, pc_idx + 1, 1, pc_text[pc_idx].c_str());
    }

    wrefresh(pc_pok_win);
}

void show_battle_info(npc * tr, int tr_sel_pk, int pc_sel_pk){
    wclear(trnr_pok_win);
    wclear(pc_pok_win);
    box(trnr_pok_win, 0, 0);
    box(pc_pok_win, 0, 0);
    trnr_text[0] = "Trainer ";
    trnr_text[0] += tr -> txt;
    trnr_text[0] +=" is challenging you!";

    
    print_centered(trnr_pok_win, 1, trnr_text[0].c_str(), 1);

    int tr_idx = 1, tr_pk_idx = 0;

    tr_idx = 1;
    for(tr_pk_idx = 0; tr_pk_idx < 6; tr_pk_idx++){
        if(tr_pk_idx == tr_sel_pk){
            trnr_text[tr_idx++] = "-Pokemon " + std::to_string(tr_pk_idx + 1) + "(in battle)";
            trnr_text[tr_idx++] = "\t+Name  : " + tr -> pokemons[tr_sel_pk] -> name;
            trnr_text[tr_idx++] = "\t+Level : " + std::to_string(tr -> pokemons[tr_sel_pk] -> level);
            trnr_text[tr_idx++] = "\t+HP    : " + std::to_string(tr -> pokemons[tr_sel_pk] -> hp) + "/" + std::to_string(tr -> pokemons[tr_sel_pk] -> stats -> hp);
        }else{
            trnr_text[tr_idx++] = "-Pokemon " + std::to_string(tr_pk_idx + 1);
        }
    }

    for(tr_idx = 1; tr_idx < 17; tr_idx++){
        mvwprintw(trnr_pok_win, tr_idx + 1, 1, trnr_text[tr_idx].c_str());
    }

    wrefresh(trnr_pok_win);

    show_player_info_batt(pc_sel_pk);
}

void show_spawn_info(Pokemon * rand_pk, int pc_sel_pk){
    wclear(trnr_pok_win);
    wclear(pc_pok_win);
    box(trnr_pok_win, 0, 0);
    box(pc_pok_win, 0, 0);
    trnr_text[0] = rand_pk -> name;
    trnr_text[0] +=" is challenging you!";

    print_centered(trnr_pok_win, 1, trnr_text[0].c_str(), 1);

    trnr_text[1] = "";
    trnr_text[2] = "-Pokemon " + rand_pk -> name + "(in battle)";
    trnr_text[3] = "\t+Level : " + std::to_string(rand_pk -> level);
    trnr_text[4] = "\t+HP    : " + std::to_string(rand_pk -> hp) + "/" + std::to_string(rand_pk -> stats -> hp);

    for(int tr_idx = 1; tr_idx < 4; tr_idx++){
        mvwprintw(trnr_pok_win, tr_idx + 1, 1, trnr_text[tr_idx].c_str());
    }

    wrefresh(trnr_pok_win);

    show_player_info_batt(pc_sel_pk);
}

void show_turn_mssg(){
    int i = 0;
    wclear(bottom_left);
    box(bottom_left, 0, 0);

    for(; i < 3; i++){
        mvwprintw(bottom_left, 1 + i, 1, mssg_text[i].c_str());
    }
    wrefresh(bottom_left);
}

int process_move(Pokemon * att, int mov_id, Pokemon * def, int critical){
    int pkm_tps_idx = std::lower_bound(db.arr_pkm_tps + 1, db.arr_pkm_tps + 1676, att -> id, cmp_pkm_tps_ids) - db.arr_pkm_tps; 
    
    double damage = (double)(((double) 2 * att -> level)/5 + 2) * db.arr_mvs[mov_id] -> power * ((double)att -> stats -> attack / def -> stats -> defense);
    damage = ((damage/50) + 2) * ((85 + rand() % 16)/ 100) * (1 + 0.5 * critical);

    if(db.arr_pkm_tps[pkm_tps_idx] -> type_id == db.arr_mvs[mov_id] -> type_id){
        damage = 1.5;
    }else if(pkm_tps_idx + 1 < 1676 && db.arr_pkm_tps[pkm_tps_idx + 1] -> pokemon_id == att -> id 
            && db.arr_pkm_tps[pkm_tps_idx] -> type_id == db.arr_mvs[mov_id] -> type_id){
        damage *= 1.5;
    }

    damage = max(0, min(def -> hp, round(damage)));

    def -> hp -= (int)damage;
    return damage;
}

std::pair<int,int> process_item(character  * play, int is_wild){
    std::string item_options[4];
    int res, pok;

    item_options[0] = "BAG";
    print_centered(pc_pok_win, 1, item_options[0], 1);
    item_options[1] = "+Potion   : " + std::to_string(play -> item[0]);
    item_options[2] = "+Revive   : " + std::to_string(play -> item[1]);
    item_options[3] = "+Pokeball : " + std::to_string(play -> item[2]);

    do{
        wclear(pc_pok_win);
        box(pc_pok_win, 0, 0);
        res = scroll_list(pc_pok_win, item_options + 1, 3, 19, 2);

        if(res == -1)
            break;
        if(play -> item[res] == 0 || (res == 2 && !is_wild))
            continue;

        if(res == 2 && is_wild){
            play -> item[2]--;
            return std::make_pair(-4, 0);
        }

        wclear(pc_pok_win);
        box(pc_pok_win, 0, 0);

        item_options[0] = "Which Pokemon?";
        print_centered(pc_pok_win, 1, item_options[0], 1);
        pok = show_pokemon_list(pc_pok_win, universe.pc.pokemons, 2); 

        if(res == 1 && play -> pokemons[pok] -> hp > 0)
            continue;
        if(res == 0 && (play -> pokemons[pok] -> hp == 0 || play -> pokemons[pok] -> hp == play -> pokemons[pok] -> stats -> hp))
            continue;
        break;
    }while(true);
    
    if(res == 0){
        play -> pokemons[pok] -> hp = min(15 + play -> pokemons[pok] -> hp, play -> pokemons [pok] -> stats -> hp);
        play -> item[0]--;
        res = -1;
    }else if(res == 1){
        play -> pokemons[pok] -> hp = round((double)play -> pokemons[pok] -> stats -> hp/2);
        play -> item[1]--;
        res = -3;
    }else{
        return std::make_pair(-5, 0);
    }

    wclear(pc_pok_win);
    box(pc_pok_win, 0, 0);
    wrefresh(pc_pok_win);
    return std::make_pair(res, pok);
}

std::pair<int,int> get_tr_rand_move(npc * tr, Pokemon * pk, int pki){
    int mv, pok;
    do{
        if(is_rand_pk)
            mv = rand() % pk -> n_moves;
        else
            mv = -3 + rand() % (3 + pk -> n_moves); 

        switch(mv){
            case -3:
                if(tr -> item[1]){
                    pok = rand() % tr -> n_pkmn;
                    if(tr -> pokemons[pok] -> hp == 0){
                        tr -> item[1]--;
                        tr -> pokemons[pok] -> hp = round((double)tr -> pokemons[pok] -> stats -> hp/2);
                        return std::make_pair(mv, pok);
                    }
                }
                break;
            case -2:
                pok = rand() % tr -> n_pkmn;
                if(pok != pki){
                    if(tr -> pokemons[pok] -> hp)
                        return std::make_pair(mv, pok);
                }
                break;
            case -1:
                if(tr -> item[0]){
                    pok = rand() % tr -> n_pkmn;
                    if(tr -> pokemons[pok] -> hp < tr -> pokemons[pok] -> stats -> hp){
                        tr -> item[0]--;
                        tr -> pokemons[pok] -> hp = min(15 + tr -> pokemons[pok] -> hp, tr -> pokemons [pok] -> stats -> hp);
                        return std::make_pair(mv, pok);
                    }
                }
                break;
            default:
                return std::make_pair(pk -> moves_t[mv],pki);
        }
    }while(true);
}

std::string get_action(int mv){
    switch(mv){
        case -1:
            return "used a potion on ";
        case -2:
            return "switch out!, go ";
        case -3:
            return "used a revive on ";
        case -4:
            return "PC used a pokeball and ";
        default:
            return "";
    }
}


int turn(Pokemon * pc_pk, int mv_pc, Pokemon * tr_pk, int mv_tr){
    int damage;

    wclear(bottom_left);
    box(bottom_left, 0, 0);

    mssg_text[0] = mssg_text[1] = mssg_text[2] = "";

    if(mv_tr < 0 || mv_pc < 0){//Potion, switch, 
        if(mv_tr < 0){
            mssg_text[0] = ((is_rand_pk) ? "" : "Trainer: ") + get_action(mv_tr) + tr_pk -> name;
            if(mv_pc < 0){
                mssg_text[2] = "PC: " + get_action(mv_pc) + pc_pk -> name;
                return 0;
            }else{
                mssg_text[2] = "PC: " + pc_pk -> name + " used " + db.arr_mvs[mv_pc] -> identifier;
                if(rand() % 100 < db.arr_mvs[mv_pc] -> accuracy){
                    damage = process_move(pc_pk, mv_pc, tr_pk, rand()%256 < pc_pk -> stats -> speed >> 1); 
                    mssg_text[2] += ", it took away " + std::to_string(damage) + " HP.";
                }else{
                    mssg_text[2] += ", but it failed.";
                }

                if(tr_pk -> hp == 0){
                    mssg_text[1] = tr_pk -> name + " is fainted"; 
                    return 1;
                }else{
                    return 0;
                }
            }
        }else{
            mssg_text[0] = "PC: " + get_action(mv_pc) + pc_pk -> name;
            mssg_text[1] = ((is_rand_pk) ? "" : "Trainer: ") + tr_pk -> name + " used " + db.arr_mvs[mv_tr] -> identifier;
            if(rand() % 100 < db.arr_mvs[mv_tr] -> accuracy){
                damage = process_move(tr_pk, mv_tr, pc_pk, rand()%256 < tr_pk -> stats -> speed >> 1); 
                mssg_text[1] += ", it took away " + std::to_string(damage) + " HP.";
            }else{
                mssg_text[1] += ", but it failed.";
            }

            if(pc_pk -> hp == 0){
                mssg_text[2] = pc_pk -> name + " is fainted"; 
                return -1;
            }else{
                return 0;
            }
        }
    }else{
        if(db.arr_mvs[mv_pc] -> priority > db.arr_mvs[mv_tr] -> priority){
            mssg_text[0] = "PC: " + pc_pk -> name + " used " + db.arr_mvs[mv_pc] -> identifier;
            if(rand() % 100 < db.arr_mvs[mv_pc] -> accuracy){
                damage = process_move(pc_pk, mv_pc, tr_pk, rand()%256 < pc_pk -> stats -> speed >> 1); 
                mssg_text[0] += ", it took away " + std::to_string(damage) + " HP.";
            }else{
                mssg_text[0] += ", but it failed.";
            }

            if(tr_pk -> hp == 0){
                mssg_text[1] = tr_pk -> name + " is fainted"; 
                return 1;
            }

            mssg_text[1] = ((is_rand_pk) ? "" : "Trainer: ") + tr_pk -> name + " used " + db.arr_mvs[mv_tr] -> identifier;
            if(rand() % 100 < db.arr_mvs[mv_tr] -> accuracy){
                damage = process_move(tr_pk, mv_tr, pc_pk, rand()%256 < tr_pk -> stats -> speed >> 1); 
                mssg_text[1] += ", it took away " + std::to_string(damage) + " HP.";
            }else{
                mssg_text[1] += ", but it failed.";
            }

            if(pc_pk -> hp == 0){
                mssg_text[2] = pc_pk -> name + " is fainted"; 
                return -1;
            }
        }else{
            mssg_text[0] = ((is_rand_pk) ? "" : "Trainer: ") + tr_pk -> name + " used " + db.arr_mvs[mv_tr] -> identifier;
            if(rand() % 100 < db.arr_mvs[mv_tr] -> accuracy){
                damage = process_move(tr_pk, mv_tr, pc_pk, rand()%256 < tr_pk -> stats -> speed >> 1); 
                mssg_text[0] += ", it took away " + std::to_string(damage) + " HP.";
            }else{
                mssg_text[0] += ", but it failed.";
            }

            if(pc_pk -> hp == 0){
                mssg_text[1] = pc_pk -> name + " is fainted"; 
                return -1;
            }
        
            mssg_text[1] = "PC: " + pc_pk -> name + " used " + db.arr_mvs[mv_pc] -> identifier;
            if(rand() % 100 < db.arr_mvs[mv_pc] -> accuracy){
                damage = process_move(pc_pk, mv_pc, tr_pk, rand()%256 < pc_pk -> stats -> speed >> 1); 
                mssg_text[1] += ", it took away " + std::to_string(damage) + " HP.";
            }else{
                mssg_text[1] += ", but it failed.";
            }

            if(tr_pk -> hp == 0){
                mssg_text[2] = tr_pk -> name + " is fainted"; 
                return 1;
            }
        }
    }

    return 0;
}

int scroll_list_two_two(WINDOW * win, std::string * txt){
    wchar_t c;
    int sel = 0, i, j, center = (win -> _maxx - 2)/2;

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_BLUE);

    wclear(win);
    box(win, 0, 0);
    
    do{
        for(i = 0; i < 2; i++){
            for(j = 0; j < 2; j++){
                if(i * 2 + j == sel)
                    wattron(win, COLOR_PAIR(2));
                mvwprintw(win,1 + (i << 1), 1 + j * center, txt[(i << 1) + j].c_str()); 
                if(i * 2 + j == sel)
                    wattroff(win, COLOR_PAIR(2));
            }
        }

        wrefresh(win);

        c = wgetch(win);

        if(c == KEY_UP){
            sel = max(0, sel - 2);
        }else if(c == KEY_DOWN){
            sel = min(3, sel + 2);
        }else if(c == KEY_LEFT){
            sel = max(0, sel - 1);
        }else if(c == KEY_RIGHT){
            sel = min(3, sel + 1);
        }else if(c == '\n' || c == 'o'){
            if(strcmp(txt[sel].c_str(),"-") != 0){
                wclear(win);
                box(win, 0, 0);
                return sel;
            }
        }
            
    }while(c != 27);

    wclear(win);
    box(win, 0, 0);

    return -1;
}

void in_fight(npc * tr){
    if(!is_rand_pk){
        while(tr -> add_pokemon(&db, 1 + rand() %1092, gen_rand_level())){
            if(rand() % 100 >= 60) break;
        }
    }

    clear_loop_wins();

    bottom_left = newwin(5, 57, 19, 0);
    bottom_right = newwin(5, 23, 19, 57);
    trnr_pok_win = newwin(19, 40, 0, 0);
    pc_pok_win = newwin(19, 40, 0, 40);

    int c,m_c, result;
    int pc_sel_pk = 0, tr_sel_pk = 0;

    std::pair<int,int> trnr_move, pc_bag;

    std::string options[4] = {"FIGHT", "BAG", "POKEMON", "RUN"};
    std::string move_options[4];
    std::string item_options[4];


    box(bottom_left, 0, 0);
    box(bottom_right, 0, 0);
    box(trnr_pok_win, 0, 0);
    box(pc_pok_win, 0, 0);
    keypad(bottom_left, TRUE);
    keypad(bottom_right, TRUE);
    keypad(trnr_pok_win, TRUE);
    keypad(pc_pok_win, TRUE);
    wrefresh(bottom_left);
    wrefresh(bottom_right);
    wrefresh(trnr_pok_win);
    wrefresh(pc_pok_win);


    do{
        wclear(bottom_left);
        wclear(bottom_right);
        wclear(pc_pok_win);
        wclear(trnr_pok_win);
        box(bottom_left, 0, 0);
        box(bottom_right, 0, 0);
        box(pc_pok_win, 0, 0);
        box(trnr_pok_win, 0, 0);

        if(!is_rand_pk) 
            show_battle_info(tr, tr_sel_pk, pc_sel_pk); 
        else
            show_spawn_info(tr -> pokemons[0], pc_sel_pk);

        mssg_text[0] = "What will " + universe.pc.pokemons[pc_sel_pk] -> name + " do?";
        mvwprintw(bottom_left, 1, 1, mssg_text[0].c_str());
        wrefresh(bottom_left);

        c = scroll_list_two_two(bottom_right, options);

        if(c == -1 || c == 3){
            if(is_rand_pk){
                if(rand() % 100 < 60){
                    mssg_text[0] = "You ran away";
                    mvwprintw(bottom_left, 1, 1, mssg_text[0].c_str());
                    wclrtoeol(bottom_left);
                    box(bottom_left, 0, 0); 
                    wrefresh(bottom_left);
                    usleep(500000);
                    flushinp();

                    return;
                }

            }
            mssg_text[0] = "You can't run";
            if(is_rand_pk) mssg_text[0] = "You couldn't escape";
            mvwprintw(bottom_left, 1, 1, mssg_text[0].c_str());
            wclrtoeol(bottom_left);
            box(bottom_left, 0, 0); 
            wrefresh(bottom_left);
            usleep(500000);
            flushinp();
        }else if(c == 0){
            //moves
            move_options[0]= ((universe.pc.pokemons[pc_sel_pk] -> moves_t[0] != INT_MAX) ? db.arr_mvs[universe.pc.pokemons[pc_sel_pk] -> moves_t[0]] -> identifier : "-");
            move_options[1]= ((universe.pc.pokemons[pc_sel_pk] -> moves_t[1] != INT_MAX) ? db.arr_mvs[universe.pc.pokemons[pc_sel_pk] -> moves_t[1]] -> identifier : "-");
            move_options[2]= ((universe.pc.pokemons[pc_sel_pk] -> moves_t[2] != INT_MAX) ? db.arr_mvs[universe.pc.pokemons[pc_sel_pk] -> moves_t[2]] -> identifier : "-");
            move_options[3]= ((universe.pc.pokemons[pc_sel_pk] -> moves_t[3] != INT_MAX) ? db.arr_mvs[universe.pc.pokemons[pc_sel_pk] -> moves_t[3]] -> identifier : "-");

            m_c = scroll_list_two_two(bottom_left, move_options);

            if(m_c == -1) continue;

            trnr_move = get_tr_rand_move(tr, tr -> pokemons[tr_sel_pk], tr_sel_pk);

            if(trnr_move.first == -2) tr_sel_pk = trnr_move.second;

            //std::cout << trnr_move.first << " " << trnr_move.second << std::endl;

            m_c = turn(universe.pc.pokemons[pc_sel_pk],universe.pc.pokemons[pc_sel_pk] -> moves_t[m_c],tr -> pokemons[trnr_move.second], trnr_move.first);

            show_turn_mssg();
            getch();

            wclear(bottom_left);
            box(bottom_left, 0, 0);
            if(m_c == -1){
                pc_sel_pk = universe.pc.has_pkm_alive();

                if(pc_sel_pk == -1){
                    result = -100;
                    break;
                }

                do{
                    wclear(pc_pok_win);
                    box(pc_pok_win, 0, 0);
                    pc_sel_pk = show_pokemon_list(pc_pok_win, universe.pc.pokemons, 1);

                    if(pc_sel_pk == -1) continue;
                    if(!universe.pc.pokemons[pc_sel_pk] -> hp){
                        mssg_text[0] = universe.pc.pokemons[pc_sel_pk] -> name + " is fainted";
                        mvwprintw(bottom_left, 1, 1, mssg_text[0].c_str());
                        wrefresh(bottom_left);
                        box(pc_pok_win, 0, 0);
                        wrefresh(pc_pok_win);
                        usleep(500000);
                        flushinp();
                    }
                    box(pc_pok_win, 0, 0);
                    wrefresh(pc_pok_win);
                }while(!universe.pc.pokemons[pc_sel_pk] -> hp);
            }

            if(m_c == 1){
                tr_sel_pk = tr -> has_pkm_alive();

                if(tr_sel_pk == -1){
                    result = 100;
                    break;
                }
            }

        }else if(c == 1){
            pc_bag = process_item(&universe.pc, false);
            if(pc_bag.first == -5){
                continue;
            }
            if(pc_bag.first == -4){
                int cont = 0;
                do{

                }while(cont < 2);
            }

            trnr_move = get_tr_rand_move(tr, tr -> pokemons[tr_sel_pk], tr_sel_pk);
            if(trnr_move.first == -2) tr_sel_pk = trnr_move.second;
            
            m_c = turn(universe.pc.pokemons[pc_bag.second], pc_bag.first, tr -> pokemons[trnr_move.second], trnr_move.first);
        }else{
            m_c = pc_sel_pk;
            do{
                wclear(pc_pok_win);
                box(pc_pok_win, 0, 0);
                pc_sel_pk = show_pokemon_list(pc_pok_win, universe.pc.pokemons, 1);

                if(pc_sel_pk == -1) continue;
                if(!universe.pc.pokemons[pc_sel_pk] -> hp){
                    mssg_text[0] = universe.pc.pokemons[pc_sel_pk] -> name + " is fainted";
                    mvwprintw(bottom_left, 1, 1, mssg_text[0].c_str());
                    wrefresh(bottom_left);
                    box(pc_pok_win, 0, 0);
                    wrefresh(pc_pok_win);
                    usleep(500000);
                    flushinp();
                }
            }while(!universe.pc.pokemons[pc_sel_pk] -> hp);

            if(m_c == pc_sel_pk)
                continue;

            if(!is_rand_pk) 
                show_battle_info(tr, tr_sel_pk, pc_sel_pk); 
            else
                show_spawn_info(tr -> pokemons[0], pc_sel_pk);

            trnr_move = get_tr_rand_move(tr, tr -> pokemons[tr_sel_pk], tr_sel_pk);

            if(trnr_move.first == -2) tr_sel_pk = trnr_move.second;
            
            m_c = turn(universe.pc.pokemons[pc_sel_pk], -2, tr -> pokemons[trnr_move.second], trnr_move.first);

            show_turn_mssg();
            getch();
        }


    }while(true);

    if(result){
        return;
    }

    delwin(bottom_left);
    delwin(bottom_right);
    delwin(trnr_pok_win);
    delwin(pc_pok_win);
}

void spawn_pokemon(){

    npc * phantom_tr = new npc();

    phantom_tr -> add_pokemon(&db, 1 + rand() % 1092, gen_rand_level());

    is_rand_pk = true;
    in_fight(phantom_tr);
    is_rand_pk = false;
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
            pks[i] = 1 + rand() % 1092;
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
    
    universe.win = newwin(21, 80, 0, 0);
    mssg_win = newwin(3, 80, 21, 0);

    ESCDELAY = 1;
    curs_set(0);
    keypad(universe.win, TRUE);
    keypad(stdscr, TRUE);
    keypad(mssg_win, TRUE);
    resizeterm(ROWS + 3, COLUMNS - 1);
    noecho();

    universe.print_terrain(universe.world[curr_y][curr_x],1);
    
    wclear(universe.win);
    wrefresh(universe.win);

    select_initial(&universe.pc);

    do{
        universe.print_terrain(universe.world[curr_y][curr_x],1);
        
        universe.world[curr_y][curr_x] -> dijkstra((int)universe.pc.r, (int)universe.pc.c, PC, universe.cost_pc);
        if(universe.pc.cell_type != '~'){ 
            universe.world[curr_y][curr_x] -> dijkstra((int)universe.pc.r, (int)universe.pc.c, RIVAL, universe.cost_rival),
            universe.world[curr_y][curr_x] -> dijkstra((int)universe.pc.r, (int)universe.pc.c, HIKER, universe.cost_hiker);
        }
        universe.world[curr_y][curr_x] -> dijkstra((int)universe.pc.r, (int)universe.pc.c, SWIMMER, universe.cost_swimmer);

        c = wgetch(universe.win);

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
                mvwprintw(mssg_win, 0, 0, "Invalid command");
                wrefresh(mssg_win);
                usleep(500000);
                wdeleteln(mssg_win);
                wrefresh(mssg_win);
                flushinp();
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
                        in_fight(tr);
                    }else{
                        mvwprintw(mssg_win, 21, 0, "You already defeated this trainer!");
                        wrefresh(mssg_win);
                        usleep(500000);
                        wdeleteln(mssg_win);
                        wrefresh(mssg_win);
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
