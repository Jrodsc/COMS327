#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include "tlib.h"
#include "player.h"
#include "map.h"
#include "priority_queue.h"
#include "queue.h"
#include "world.h"

int offsett_r[8] = {0, 0, 1, -1, -1, 1, -1, 1};
int offsett_c[8] = {1, -1, 0, 0, -1, 1, 1, -1};

void World::print_terrain(map * m, int color){

    int i,j;

    clear();

    if(has_colors() == TRUE && color){
        start_color();

        init_pair(1, COLOR_YELLOW, COLOR_BLACK);
        init_pair(2, COLOR_WHITE, COLOR_BLACK);
        init_pair(3, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(4, COLOR_GREEN, COLOR_BLACK);
        init_pair(5, COLOR_CYAN, COLOR_BLACK);
        init_pair(6, COLOR_BLUE, COLOR_BLACK);
        init_pair(7, COLOR_RED, COLOR_BLACK);
        init_pair(8, COLOR_BLACK, COLOR_WHITE);
        for(i = 0; i < ROWS; i++){
            for(j = 0; j < COLUMNS - 1; j++){
                
                if(trainers[i][j] != 0) {
                    attron(COLOR_PAIR(2)); 
                    if(trainers[i][j] == '@')
                        addch('@');
                    else
                        addch(m -> arr_trnr[trainers[i][j]-1].txt);
                    attroff(COLOR_PAIR(2)); 
                    continue;
                }
                switch(m -> terr[i][j]){
                    case '%':
                        attron(COLOR_PAIR(1)); 
                        break;
                    case '#':
                        attron(COLOR_PAIR(2)); 
                        break;
                    case ':':
                        attron(COLOR_PAIR(3)); 
                        break;
                    case '.':
                        attron(COLOR_PAIR(4)); 
                        break;
                    case '^':
                        attron(COLOR_PAIR(4)); 
                        break;
                    case '~':
                        attron(COLOR_PAIR(5)); 
                        break;
                    case 'M':
                        attron(COLOR_PAIR(6)); 
                        break;
                    case 'C':
                        attron(COLOR_PAIR(7)); 
                        break;
                    default:
                        attron(COLOR_PAIR(2)); 
                }
                addch(m -> terr[i][j]);
                switch(m -> terr[i][j]){
                    case '%':
                        attroff(COLOR_PAIR(1)); 
                        break;
                    case '#':
                        attroff(COLOR_PAIR(2)); 
                        break;
                    case ':':
                        attroff(COLOR_PAIR(3)); 
                        break;
                    case '.':
                        attroff(COLOR_PAIR(4)); 
                        break;
                    case '^':
                        attroff(COLOR_PAIR(4)); 
                        break;
                    case '~':
                        attroff(COLOR_PAIR(5)); 
                        break;
                    case 'M':
                        attroff(COLOR_PAIR(6)); 
                        break;
                    case 'C':
                        attroff(COLOR_PAIR(7)); 
                        break;
                    default:
                        attroff(COLOR_PAIR(2)); 
                }
            }
            /*addch('\n');*/
        } 
    }else if(has_colors()){
        attron(COLOR_PAIR(8));
        for(i = 0; i < ROWS; i++){
            for(j = 0; j < COLUMNS - 1; j++){
                if(trainers[i][j] != 0) 
                    if(trainers[i][j] == '@')
                        addch('@');
                    else
                        addch(m -> arr_trnr[trainers[i][j]-1].txt);
                else
                    addch(m -> terr[i][j]);
            }
        }
        attroff(COLOR_PAIR(8));
    }else{
    
        for(i = 0; i < ROWS; i++){
            for(j = 0; j < COLUMNS - 1; j++){
                if(trainers[i][j] != 0) 
                    if(trainers[i][j] == '@')
                        addch('@');
                    else
                        addch(m -> arr_trnr[trainers[i][j]-1].txt);
                else
                    addch(m -> terr[i][j]);
            }
        }
    }

    refresh();
}

int World::set_trnrs_map(map * m){
    int i,j,k;

    for(i = 0; i < ROWS; i++){
        for(j = 0; j < COLUMNS; j++){
            trainers[i][j] = (char)0;
        }
    }

    for(k = 0; k < m -> n_trnrs; k++){
        i = m -> arr_trnr[k].r;
        j = m -> arr_trnr[k].c;
        trainers[i][j] = k+1;
        m -> arr_trnr[k].state = 0;
    }

    trainers[(int)pc.r][(int)pc.c] = '@';

    return 1;
}

void pc_rand_move(World * w_t, map * m){
    int rt,ct,r,c;

    char next_move = rand()%8;

    r = w_t -> pc.r;
    c = w_t -> pc.c;

    rt = r + offsett_r[(int)next_move];
    ct = c + offsett_c[(int)next_move];

    if(!OUT_OF_TERR_LIMITS(rt,ct) && w_t -> trainers[rt][ct] == 0){
        w_t -> trainers[r][c] = 0;
        w_t -> pc.r = rt;
        w_t -> pc.c = ct;
        w_t -> pc.last_move = next_move;
        w_t -> trainers[rt][ct] = '@';
        r = w_t -> pc.r;
        c = w_t -> pc.c;
    } 
    w_t -> pc.cell_type = m -> terr[r][c];
}

int World::update_pc_map(map * m, char cx){
    int rt, ct, r, c, move;

    r = pc.r;
    c = pc.c;


    switch(cx){
        case '7':
        case 'y':
            move = 4;
            break;
        case '8':
        case 'k':
            move = 3;
            break;
        case '9':
        case 'u':
            move = 6;
            break;
        case '6':
        case 'l':
            move = 0;
            break;
        case '3':
        case 'n':
            move = 5;
            break;
        case '2':
        case 'j':
            move = 2;
            break;
        case '1':
        case 'b':
            move = 7;
            break;
        case '4':
        case 'h':
            move = 1;
            break;
        case ' ':
        case '5':
        case '.':
            return 0;
        case '>':
            if(pc.cell_type == 'M'){
                return 2;
            }else if(pc.cell_type == 'C'){
                return 3;
            }else{
                return 1;
            }
            break;
        case 't':
            return 4;
            break;
        case 'Q':
            return -1;
        case 'f':
            return 5;
        default:
            return 1;
    }


    rt = r + offsett_r[move];
    ct = c + offsett_c[move];


    if(!OUT_OF_TERR_LIMITS(rt,ct) && trainers[rt][ct] == 0){
        trainers[r][c] = 0;
        pc.r = rt;
        pc.c = ct;
        pc.last_move = move;
        trainers[rt][ct] = '@';
        r = pc.r;
        c = pc.c;
        

        if(m -> terr[r][c] == ':' && rand()%100  < 10){
            pc.cell_type = m -> terr[r][c];
            return RAN_POKEMON;
        }            
    }else if(!OUT_OF_TERR_LIMITS(rt,ct) && trainers[rt][ct] != 0){
        return 1000000 + trainers[rt][ct] - 1;
    }else{
        if(m -> terr[rt][ct] == '#'){
            if(ct == COLUMNS - 2)
                return 10;
            else if(rt == ROWS - 1)
                return 11;
            else if(ct == 0)
                return 12;
            else 
                return 13;
        }else
            return 1;
    }
    pc.cell_type = m -> terr[r][c];

    return 0;

}

int World::update_trnrs_map(map * m){
    int i,j,rt,ct,r,c,isposs;
    int in_battle = 0;
    int mini;

    char next_move;

    /*pc_rand_move(w_t, m);*/

    for(i = 0; i < m -> n_trnrs; i++){
        mini = INF;
        rt = r = (int) m -> arr_trnr[i].r;
        ct = c = (int) m -> arr_trnr[i].c;
        next_move = m -> arr_trnr[i].last_move;
        isposs = 0;
        switch((int)m -> arr_trnr[i].type){
            case HIKER: 
                for(j = 0; j < 8; j++){
                    rt = r + offsett_r[j];
                    ct = c + offsett_c[j];

                    if(!OUT_OF_TERR_LIMITS(rt,ct) && (int)(1 - 2 * (int) (m -> arr_trnr[i].state == 1)) * (int)cost_hiker[rt][ct] < mini && (trainers[rt][ct] == 0 || (trainers[rt][ct] == '@' && m -> arr_trnr[i].state == 0 && !in_battle))){
                        if(trainers[rt][ct] == '@' && m -> arr_trnr[i].state == 0 && !in_battle) 
                            in_battle = i+1;
                        mini = (int)(1 - 2 * (int) (m -> arr_trnr[i].state == 1)) * (int)cost_hiker[rt][ct];
                        next_move = (char)j;
                        isposs = 1;
                    }
                }

                if(isposs){
                    rt = r + offsett_r[(int)next_move];
                    ct = c + offsett_c[(int)next_move];
                }else{
                    rt = r;
                    ct = c;
                }
                if(in_battle == i + 1) 
                    continue;
                trainers[r][c] = 0;
                m -> arr_trnr[i].r = rt; 
                m -> arr_trnr[i].c = ct;
                trainers[rt][ct] = i+1;
                m -> arr_trnr[i].last_move = next_move;
                break;
            case RIVAL:
                for(j = 0; j < 8; j++){
                    rt = r + offsett_r[j];
                    ct = c + offsett_c[j];

                    if(!OUT_OF_TERR_LIMITS(rt,ct) && (1 - 2 * (int) m -> arr_trnr[i].state) * (int) cost_rival[rt][ct] < mini && (trainers[rt][ct] == 0 || (trainers[rt][ct] == '@' && m -> arr_trnr[i].state == 0 && !in_battle))){
                        if(trainers[rt][ct] == '@' && m -> arr_trnr[i].state == 0 && !in_battle)
                            in_battle = i+1;
                        mini = (1 - 2 * (int) m -> arr_trnr[i].state) * (int) cost_rival[rt][ct];
                        next_move = (char)j;
                        isposs = 1;
                    }
                }
                
                if(isposs){
                    rt = r + offsett_r[(int)next_move];
                    ct = c + offsett_c[(int)next_move];
                }else{
                    rt = r;
                    ct = c;
                }

                if(in_battle == i + 1) 
                    continue;

                trainers[r][c] = 0;
                m -> arr_trnr[i].r = rt;
                m -> arr_trnr[i].c = ct;
                trainers[rt][ct] = i+1;
                m -> arr_trnr[i].last_move = next_move;
                break;
            case PACER:
                rt = r + offsett_r[(int)next_move];
                ct = c + offsett_c[(int)next_move];
                if(OUT_OF_TERR_LIMITS(rt,ct) || cost_rival[rt][ct] == INF || trainers[rt][ct] != 0){
                    if(trainers[rt][ct] == '@' && m -> arr_trnr[i].state == 0 && !in_battle)
                        in_battle = i+1;
                    rt = r;
                    ct = c;
                    next_move = m -> arr_trnr[i].last_move ^ 1; 
                }
                if(in_battle == i + 1) 
                    continue;
                trainers[r][c] = 0;
                m -> arr_trnr[i].r = rt;
                m -> arr_trnr[i].c = ct;
                trainers[rt][ct] = i+1;
                m -> arr_trnr[i].last_move = next_move;   
                break;
            case WANDERER:
                rt = r + offsett_r[(int)next_move];
                ct = c + offsett_c[(int)next_move];
                if(OUT_OF_TERR_LIMITS(rt,ct) || m -> terr[rt][ct] != m -> terr[r][c] || trainers[rt][ct] != 0){
                    if(trainers[rt][ct] == '@' && m -> arr_trnr[i].state == 0 && !in_battle)
                        in_battle = i+1;
                    rt = r;
                    ct = c;
                    next_move = (m -> arr_trnr[i].last_move + rand()%8)%8; 
                }
                if(in_battle == i + 1) 
                    continue;
                trainers[r][c] = 0;
                m -> arr_trnr[i].r = rt;
                m -> arr_trnr[i].c = ct;
                trainers[rt][ct] = i+1;
                m -> arr_trnr[i].last_move = next_move;
                break;
            case SENTRIE:
                break;
            case EXPLORER:
                rt = r + offsett_r[(int)next_move];
                ct = c + offsett_c[(int)next_move];
                if(OUT_OF_TERR_LIMITS(rt,ct) || trainers[rt][ct] != 0){
                    if(trainers[rt][ct] == '@' && m -> arr_trnr[i].state == 0 && !in_battle)
                        in_battle = i+1;
                    rt = r;
                    ct = c;
                    next_move = (m -> arr_trnr[i].last_move + rand()%8)%8; 
                }
                if(in_battle == i + 1) 
                    continue;
                trainers[r][c] = 0;
                m -> arr_trnr[i].r = rt;
                m -> arr_trnr[i].c = ct;
                trainers[rt][ct] = i+1;
                m -> arr_trnr[i].last_move = next_move;
                break;
            case SWIMMER:
                if(cost_swimmer[r][c] == INF){
                    rt = r + offsett_r[(int)next_move];
                    ct = c + offsett_c[(int)next_move];
                    if(OUT_OF_TERR_LIMITS(rt,ct) || trainers[rt][ct] != 0 || m -> terr[rt][ct] != '~'){
                        if(trainers[rt][ct] == '@' && m -> arr_trnr[i].state == 0 && !in_battle)
                            in_battle = i+1;
                        rt = r;
                        ct = c;
                        next_move = (m -> arr_trnr[i].last_move + rand()%8)%8; 
                    }
                }else{
                    for(j = 0; j < 8; j++){
                        rt = r + offsett_r[j];
                        ct = c + offsett_c[j];

                        if(!OUT_OF_TERR_LIMITS(rt,ct) && (1 - 2 * (int) m -> arr_trnr[i].state) * (int) cost_swimmer[rt][ct] < mini && (trainers[rt][ct] == 0 || (trainers[rt][ct] == '@' && m -> arr_trnr[i].state == 0 && !in_battle)) &&  m -> terr[rt][ct] == '~'){
                            if(trainers[rt][ct] == '@' && m -> arr_trnr[i].state == 0 && !in_battle) 
                                in_battle = i + 1;
                            mini = (1 - 2 * (int) m -> arr_trnr[i].state) * cost_swimmer[rt][ct];
                            next_move = (char)j;
                            isposs = 1;
                        }
                    }

                    if(isposs){
                        rt = r + offsett_r[(int)next_move];
                        ct = c + offsett_c[(int)next_move];
                    }else{
                        rt = r;
                        ct = c;
                    }
                }
                if(in_battle == i + 1) 
                    continue;
                trainers[r][c] = 0;
                m -> arr_trnr[i].r = rt;
                m -> arr_trnr[i].c = ct;
                if(m -> terr[rt][ct] != '#') trainers[rt][ct] = i+1;
                m -> arr_trnr[i].last_move = next_move;
                break;
        }    
    }
    return in_battle;
}


int World::go_world(int x, int y, int numtrnrs_sw, int move_t){
    if(x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return 0;
    
    if(NULL == world[y][x]){
        world[y][x] = (map *)malloc(sizeof(*world[x][y]));
        world[y][x] -> set_exits('a', -1);
        if(x + 1 >= WIDTH){
             world[y][x] -> set_exits('e', -2);
        }else{
            if(NULL != world[y][x + 1]){
                world[y][x] -> set_exits('e', world[y][x + 1] -> w_exit);
            }
        }
        if(x - 1 < 0){
             world[y][x] -> set_exits('w', -2);
        }else{
            if(NULL != world[y][x - 1]){
                world[y][x] -> set_exits('w', world[y][x - 1] -> e_exit);
            } 
        }
        if(y + 1 >= HEIGHT){
             world[y][x] -> set_exits('s', -2);
        }else{
            if(NULL != world[y + 1][x]){
                world[y][x] -> set_exits('s', world[y + 1][x] -> n_exit);
            }
        }
        if(y - 1 < 0){ 
             world[y][x] -> set_exits('n', -2);
        }else{
            if(NULL != world[y - 1][x]){
                world[y][x] -> set_exits('n', world[y - 1][x] -> s_exit);
            }
        }
        world[y][x] -> set(200 - x,200 - y, numtrnrs_sw, (move_t == 4));
    }
    
    if(move_t == 0)
        pc.c = 1, pc.r = world[y][x] -> w_exit;
    else if(move_t == 1)
        pc.r = 1, pc.c = world[y][x] -> n_exit;
    else if(move_t == 2)
        pc.c = COLUMNS - 3, pc.r = world[y][x] -> e_exit;
    else if(move_t == 3)
        pc.r = ROWS - 2, pc.c = world[y][x] -> s_exit;
    else
        pc.r = world[y][x] -> rand_pos.r, pc.c = world[y][x] -> rand_pos.c;

    set_trnrs_map(world[y][x]);

    return 1;
}
