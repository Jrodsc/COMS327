#include <stdlib.h>
#include <stdio.h>
#include "tlib.h"
#include "player.h"
#include "map.h"
#include "priority_queue.h"
#include "queue.h"
#include "world.h"

int offsett_r[8] = {0, 0, 1, -1, -1, 1, -1, 1};
int offsett_c[8] = {1, -1, 0, 0, -1, 1, 1, -1};

void map_print_terrain(world_t * w_t, map_t * m){

    int i,j;
    system("clear");

    for(i = 0; i < ROWS; i++){
        for(j = 0; j < COLUMNS - 1; j++){
            
            if(w_t -> trainers[i][j] != 0) {
                reset();
                printf("%c", w_t -> trainers[i][j]);
                continue;
            }

            switch(m -> terr[i][j]){
                case '%':
                    yellow(); 
                    break;
                case '#':
                    white();
                    break;
                case ':':
                    purple();
                    break;
                case '.':
                    green();
                    break;
                case '^':
                    green();
                    break;
                case '~':
                    cyan();
                    break;
                case 'M':
                    blue();
                    break;
                case 'C':
                    red();
                    break;
                default:
                    reset();
            }
            printf("%c",m -> terr[i][j]);
        }
        printf("\n");
    }
    reset();
}

int set_trnrs_map(world_t * w_t, map_t * m){
    int i,j,k = 0;

    for(i = 0; i < ROWS; i++){
        for(j = 0; j < COLUMNS; j++){
            if(k < m -> n_trnrs && i == m -> arr_trnr[k].r && j == m -> arr_trnr[k].c){
                w_t -> trainers[i][j] = m -> arr_trnr[k].txt;
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

int update_trnrs_map(world_t * w_t, map_t * m){
    int i,j,rt,ct,r,c,isposs;
    unsigned long long int mini;

    char next_move;

    pc_rand_move(w_t, m);

    for(i = 0; i < m -> n_trnrs; i++){
        printf("%d\n", m -> arr_trnr[i].type);
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

                    if(w_t -> cost_hiker[rt][ct] < mini && w_t -> trainers[rt][ct] == 0){
                        mini = w_t -> cost_hiker[rt][ct];
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
                w_t -> trainers[r][c] = 0;
                m -> arr_trnr[i].r = rt; 
                m -> arr_trnr[i].c = ct;
                w_t -> trainers[rt][ct] = 'h';
                m -> arr_trnr[i].last_move = next_move;
                break;
            case RIVAL:
                for(j = 0; j < 8; j++){
                    rt = r + offsett_r[j];
                    ct = c + offsett_c[j];

                    if(w_t -> cost_rival[rt][ct] < mini && w_t -> trainers[rt][ct] == 0){
                        mini = w_t -> cost_rival[rt][ct];
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

                w_t -> trainers[r][c] = 0;
                m -> arr_trnr[i].r = rt;
                m -> arr_trnr[i].c = ct;
                w_t -> trainers[rt][ct] = 'r';
                m -> arr_trnr[i].last_move = next_move;
                break;
            case PACER:
                rt = r + offsett_r[(int)next_move];
                ct = c + offsett_c[(int)next_move];
                if(OUT_OF_TERR_LIMITS(rt,ct) || w_t -> cost_rival[rt][ct] == INF || w_t -> trainers[rt][ct] != 0){
                    rt = r;
                    ct = c;
                    next_move = m -> arr_trnr[i].last_move ^ 1; 
                }
                w_t -> trainers[r][c] = 0;
                m -> arr_trnr[i].r = rt;
                m -> arr_trnr[i].c = ct;
                w_t -> trainers[rt][ct] = 'p';
                m -> arr_trnr[i].last_move = next_move;   
                break;
            case WANDERER:
                rt = r + offsett_r[(int)next_move];
                ct = c + offsett_c[(int)next_move];
                if(OUT_OF_TERR_LIMITS(rt,ct) || m -> terr[rt][ct] != m -> terr[r][c] || w_t -> trainers[rt][ct] != 0){
                    rt = r;
                    ct = c;
                    next_move = (m -> arr_trnr[i].last_move + rand()%8)%8; 
                }
                w_t -> trainers[r][c] = 0;
                m -> arr_trnr[i].r = rt;
                m -> arr_trnr[i].c = ct;
                w_t -> trainers[rt][ct] = 'w';
                m -> arr_trnr[i].last_move = next_move;
                break;
            case SENTRIE:
                break;
            case EXPLORER:
                rt = r + offsett_r[(int)next_move];
                ct = c + offsett_c[(int)next_move];
                if(OUT_OF_TERR_LIMITS(rt,ct) || w_t -> trainers[rt][ct] != 0){
                    rt = r;
                    ct = c;
                    next_move = (m -> arr_trnr[i].last_move + rand()%8)%8; 
                }
                w_t -> trainers[r][c] = 0;
                m -> arr_trnr[i].r = rt;
                m -> arr_trnr[i].c = ct;
                w_t -> trainers[rt][ct] = 'e';
                m -> arr_trnr[i].last_move = next_move;
                break;
            case SWIMMER:
                if(w_t -> cost_swimmer[r][c] == INF){
                    rt = r + offsett_r[(int)next_move];
                    ct = c + offsett_c[(int)next_move];
                    if(OUT_OF_TERR_LIMITS(rt,ct) || w_t -> trainers[rt][ct] != 0 || m -> terr[rt][ct] != '~'){
                        rt = r;
                        ct = c;
                        next_move = (m -> arr_trnr[i].last_move + rand()%8)%8; 
                    }
                }else{
                    for(j = 0; j < 8; j++){
                        rt = r + offsett_r[j];
                        ct = c + offsett_c[j];

                        if(w_t -> cost_swimmer[rt][ct] < mini && w_t -> trainers[rt][ct] == 0 &&  m -> terr[rt][ct] == '~'){
                            mini = w_t -> cost_swimmer[rt][ct];
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
                w_t -> trainers[r][c] = 0;
                m -> arr_trnr[i].r = rt;
                m -> arr_trnr[i].c = ct;
                if(m -> terr[rt][ct] != '#') w_t -> trainers[rt][ct] = 'm';
                m -> arr_trnr[i].last_move = next_move;
                break;
        }    
    }
    return 0;
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
