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

void map_print_terrain(world_t * w_t, map_t * m, int color){

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
                
                if(w_t -> trainers[i][j] != 0) {
                    attron(COLOR_PAIR(2)); 
                    if(w_t -> trainers[i][j] == '@')
                        addch('@');
                    else
                        addch(m -> arr_trnr[w_t -> trainers[i][j]-1].txt);
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
                if(w_t -> trainers[i][j] != 0) 
                    if(w_t -> trainers[i][j] == '@')
                        addch('@');
                    else
                        addch(m -> arr_trnr[w_t -> trainers[i][j]-1].txt);
                else
                    addch(m -> terr[i][j]);
            }
        }
        attroff(COLOR_PAIR(8));
    }else{
    
        for(i = 0; i < ROWS; i++){
            for(j = 0; j < COLUMNS - 1; j++){
                if(w_t -> trainers[i][j] != 0) 
                    if(w_t -> trainers[i][j] == '@')
                        addch('@');
                    else
                        addch(m -> arr_trnr[w_t -> trainers[i][j]-1].txt);
                else
                    addch(m -> terr[i][j]);
            }
        }
    }

    refresh();
}

int set_trnrs_map(world_t * w_t, map_t * m){
    int i,j,k = 0;

    for(i = 0; i < ROWS; i++){
        for(j = 0; j < COLUMNS; j++){
            if(k < m -> n_trnrs && i == m -> arr_trnr[k].r && j == m -> arr_trnr[k].c){
                w_t -> trainers[i][j] = k+1;
                m -> arr_trnr[k].state = 0;
                k++;
            }
            else
                w_t -> trainers[i][j] = (char)0;
        }
    }

    return 1;
}

void pc_rand_move(world_t * w_t, map_t * m){
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

int update_pc_map(world_t * w_t, map_t * m, char cx){
    int rt, ct, r, c, move;

    r = w_t -> pc.r;
    c = w_t -> pc.c;


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
            if(w_t -> pc.cell_type == 'M'){
                return 2;
            }else if(w_t -> pc.cell_type == 'C'){
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
        default:
            return 1;
    }


    rt = r + offsett_r[move];
    ct = c + offsett_c[move];


    if(!OUT_OF_TERR_LIMITS(rt,ct) && w_t -> trainers[rt][ct] == 0){
        w_t -> trainers[r][c] = 0;
        w_t -> pc.r = rt;
        w_t -> pc.c = ct;
        w_t -> pc.last_move = move;
        w_t -> trainers[rt][ct] = '@';
        r = w_t -> pc.r;
        c = w_t -> pc.c;
    }else if(!OUT_OF_TERR_LIMITS(rt,ct) && w_t -> trainers[rt][ct] != 0){
        return 1000000 + w_t -> trainers[rt][ct] - 1;
    }else{
        return 1;
    }
    w_t -> pc.cell_type = m -> terr[r][c];

    return 0;

}

int update_trnrs_map(world_t * w_t, map_t * m){
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

                    if(!OUT_OF_TERR_LIMITS(rt,ct) && (int)(1 - 2 * (int) (m -> arr_trnr[i].state == 1)) * (int)w_t -> cost_hiker[rt][ct] < mini && (w_t -> trainers[rt][ct] == 0 || (w_t -> trainers[rt][ct] == '@' && m -> arr_trnr[i].state == 0 && !in_battle))){
                        if(w_t -> trainers[rt][ct] == '@' && m -> arr_trnr[i].state == 0 && !in_battle) 
                            in_battle = i+1;
                        mini = (int)(1 - 2 * (int) (m -> arr_trnr[i].state == 1)) * (int)w_t -> cost_hiker[rt][ct];
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
                w_t -> trainers[r][c] = 0;
                m -> arr_trnr[i].r = rt; 
                m -> arr_trnr[i].c = ct;
                w_t -> trainers[rt][ct] = i+1;
                m -> arr_trnr[i].last_move = next_move;
                break;
            case RIVAL:
                for(j = 0; j < 8; j++){
                    rt = r + offsett_r[j];
                    ct = c + offsett_c[j];

                    if(!OUT_OF_TERR_LIMITS(rt,ct) && (1 - 2 * (int) m -> arr_trnr[i].state) * (int) w_t -> cost_rival[rt][ct] < mini && (w_t -> trainers[rt][ct] == 0 || (w_t -> trainers[rt][ct] == '@' && m -> arr_trnr[i].state == 0 && !in_battle))){
                        if(w_t -> trainers[rt][ct] == '@' && m -> arr_trnr[i].state == 0 && !in_battle)
                            in_battle = i+1;
                        mini = (1 - 2 * (int) m -> arr_trnr[i].state) * (int) w_t -> cost_rival[rt][ct];
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

                w_t -> trainers[r][c] = 0;
                m -> arr_trnr[i].r = rt;
                m -> arr_trnr[i].c = ct;
                w_t -> trainers[rt][ct] = i+1;
                m -> arr_trnr[i].last_move = next_move;
                break;
            case PACER:
                rt = r + offsett_r[(int)next_move];
                ct = c + offsett_c[(int)next_move];
                if(OUT_OF_TERR_LIMITS(rt,ct) || w_t -> cost_rival[rt][ct] == INF || w_t -> trainers[rt][ct] != 0){
                    if(w_t -> trainers[rt][ct] == '@' && m -> arr_trnr[i].state == 0 && !in_battle)
                        in_battle = i+1;
                    rt = r;
                    ct = c;
                    next_move = m -> arr_trnr[i].last_move ^ 1; 
                }
                if(in_battle == i + 1) 
                    continue;
                w_t -> trainers[r][c] = 0;
                m -> arr_trnr[i].r = rt;
                m -> arr_trnr[i].c = ct;
                w_t -> trainers[rt][ct] = i+1;
                m -> arr_trnr[i].last_move = next_move;   
                break;
            case WANDERER:
                rt = r + offsett_r[(int)next_move];
                ct = c + offsett_c[(int)next_move];
                if(OUT_OF_TERR_LIMITS(rt,ct) || m -> terr[rt][ct] != m -> terr[r][c] || w_t -> trainers[rt][ct] != 0){
                    if(w_t -> trainers[rt][ct] == '@' && m -> arr_trnr[i].state == 0 && !in_battle)
                        in_battle = i+1;
                    rt = r;
                    ct = c;
                    next_move = (m -> arr_trnr[i].last_move + rand()%8)%8; 
                }
                if(in_battle == i + 1) 
                    continue;
                w_t -> trainers[r][c] = 0;
                m -> arr_trnr[i].r = rt;
                m -> arr_trnr[i].c = ct;
                w_t -> trainers[rt][ct] = i+1;
                m -> arr_trnr[i].last_move = next_move;
                break;
            case SENTRIE:
                break;
            case EXPLORER:
                rt = r + offsett_r[(int)next_move];
                ct = c + offsett_c[(int)next_move];
                if(OUT_OF_TERR_LIMITS(rt,ct) || w_t -> trainers[rt][ct] != 0){
                    if(w_t -> trainers[rt][ct] == '@' && m -> arr_trnr[i].state == 0 && !in_battle)
                        in_battle = i+1;
                    rt = r;
                    ct = c;
                    next_move = (m -> arr_trnr[i].last_move + rand()%8)%8; 
                }
                if(in_battle == i + 1) 
                    continue;
                w_t -> trainers[r][c] = 0;
                m -> arr_trnr[i].r = rt;
                m -> arr_trnr[i].c = ct;
                w_t -> trainers[rt][ct] = i+1;
                m -> arr_trnr[i].last_move = next_move;
                break;
            case SWIMMER:
                if(w_t -> cost_swimmer[r][c] == INF){
                    rt = r + offsett_r[(int)next_move];
                    ct = c + offsett_c[(int)next_move];
                    if(OUT_OF_TERR_LIMITS(rt,ct) || w_t -> trainers[rt][ct] != 0 || m -> terr[rt][ct] != '~'){
                        if(w_t -> trainers[rt][ct] == '@' && m -> arr_trnr[i].state == 0 && !in_battle)
                            in_battle = i+1;
                        rt = r;
                        ct = c;
                        next_move = (m -> arr_trnr[i].last_move + rand()%8)%8; 
                    }
                }else{
                    for(j = 0; j < 8; j++){
                        rt = r + offsett_r[j];
                        ct = c + offsett_c[j];

                        if(!OUT_OF_TERR_LIMITS(rt,ct) && (1 - 2 * (int) m -> arr_trnr[i].state) * (int) w_t -> cost_swimmer[rt][ct] < mini && (w_t -> trainers[rt][ct] == 0 || (w_t -> trainers[rt][ct] == '@' && m -> arr_trnr[i].state == 0 && !in_battle)) &&  m -> terr[rt][ct] == '~'){
                            if(w_t -> trainers[rt][ct] == '@' && m -> arr_trnr[i].state == 0 && !in_battle) 
                                in_battle = i + 1;
                            mini = (1 - 2 * (int) m -> arr_trnr[i].state) * w_t -> cost_swimmer[rt][ct];
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
                w_t -> trainers[r][c] = 0;
                m -> arr_trnr[i].r = rt;
                m -> arr_trnr[i].c = ct;
                if(m -> terr[rt][ct] != '#') w_t -> trainers[rt][ct] = i+1;
                m -> arr_trnr[i].last_move = next_move;
                break;
        }    
    }
    return in_battle;
}


int go_world(world_t * w_t, int x, int y, int numtrnrs_sw){
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
        map_set(w_t -> world[y][x],200 - x,200 - y, numtrnrs_sw);
    }

    return 1;
}
