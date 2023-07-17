#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tlib.h"
#include "queue.h"
#include "player.h"
#include "map.h"
#include "priority_queue.h"

//Process functions

queue_t q;

int dw = 0,de = 0,dn = 0,ds = 0;
int wcells, market, store;
int distance;

int offset_r[8] = {0, 0, 1, -1, -1, 1, -1, 1};
int offset_c[8] = {1, -1, 0, 0, -1, 1, 1, -1};
int cost[4][9] = {
    {10, 10, 10, 20, 10, INF, INF, INF, 10},
    {10, 50, 50, 15, 10, 15, 15, INF, INF},
    {10, 50, 50, 20, 10, INF, INF, INF,INF},
    {INF, INF, INF, INF, INF, INF, INF, 7, INF}
};

//Compare functions

static int cmp_trnr_pos(const void *p1, const void *p2){
    npc p1_t = *(npc *)p1;
    npc p2_t = *(npc *)p2;

    if(p1_t.r == p2_t.r){
        return (int)!(p1_t.c < p2_t.c);
    }

    return (int)!(p1_t.r < p2_t.r);
}

//Bldr Tree Path PMart PCntr TGras SGras Mtn Forest Water Gate

char terr_type[11] = {'%', '^', '#', 'M', 'C', ':', '.', '%', '^', '~', '#'};

void map::dijkstra(int rp, int cp, int t, unsigned long long int dist[ROWS][COLUMNS]){ 
    int r, c, obj, rt, ct, i,round_water = 0;
    
    unsigned long long int state, u, v, d;
    unsigned long long int * s;

    priority_queue_t pq;
    priority_queue_init(&pq);

    for(r = 0; r < ROWS; r++){
        for(c = 0; c < COLUMNS-1; c++){
            dist[r][c] = INF;
        }
    }
    

    for(i = 0; i < 8; i++){
        rt = rp + offset_r[i];
        ct = cp + offset_c[i];

        if(terr[rt][ct] == '~')
            round_water++;
    }

    dist[rp][cp] = INF * (t == SWIMMER && !round_water);

    if(t == SWIMMER) t = 3;

    s = (unsigned long long int *)malloc(sizeof(*s));
    *s = rp * COLUMNS + cp;
    priority_queue_push(&pq, (void *)s);


    while(!priority_queue_empty(&pq)){
        s = (unsigned long long int *)priority_queue_top(&pq);

        state = *s;

        free(s);
        
        d = state >> 32;
        u = (state % (1LL << 32));
        
        priority_queue_pop(&pq);

        r = u / COLUMNS;
        c = u % COLUMNS;

        if(dist[r][c] < d) continue;
        
        for(i = 0; i < 8; i++){
            rt = r + offset_r[i];
            ct = c + offset_c[i];

            if(rt < 1 || rt > ROWS - 2 || ct < 1 || ct > COLUMNS -3) continue; 
            switch(terr[rt][ct]){
                case '#':
                    obj = 0;
                    break;
                case 'M':
                    obj = 1;
                    break;
                case 'C':
                    obj = 2;
                    break;
                case ':':
                    obj = 3;
                    break;
                case '.':
                    obj = 4;
                    break;
                case '%':
                    obj = 5;
                    break;
                case '^':
                    obj = 6;
                    break;
                case '~':
                    obj = 7;
                    break;
                default:
                    obj = 8;
            }

            if(dist[r][c] + cost[t][obj] < dist[rt][ct]){
                dist[rt][ct] = dist[r][c] + cost[t][obj];
                v = rt * COLUMNS + ct;
                s = (unsigned long long int *)malloc(sizeof(*s));
                *s = v + (dist[rt][ct] << 32);
                priority_queue_push(&pq, (void *)s);

            }
        }
    }
}

pos_t map::get_rand_pos(char cx){
    bool flag = false;
    pos_t t;
    int r,c;    
    if(cx == 'M' && !market) cx = ':';
    if(cx == 'C' && !store)  cx = '.';
    do{
        r = 1 + rand()%(ROWS - 2);
        c = 1 + rand()%(COLUMNS-3);

        if(terr[r][c] == cx){
            t.r = r;
            t.c = c;
            flag = true;
        }
    }while(!flag);


    return t;
}

void map::paths_generation(){

    int ancestor[ROWS][COLUMNS];
    int dist[ROWS][COLUMNS];

    int u,v;

    for(u = 0; u < ROWS; u++){
        for(v = 0; v < COLUMNS; v++){
            ancestor[u][v] = -1;
            dist[u][v] = INF;
        }
    }

    dist[cross_r][cross_c] = 0;
    ancestor[cross_r][cross_c] = -2;

    queue_init(&q);

    queue_enqueue_front(&q, cross_r * COLUMNS + cross_c);

    int tmp_r, tmp_c,r,c;

    while(!queue_is_empty(&q)){
        queue_dequeue(&q, &u);

        r = u / COLUMNS;
        c = u % COLUMNS;

        for(v = 0; v < 4 ; v++){
            tmp_r = r + offset_r[v];
            tmp_c = c + offset_c[v];

            if(tmp_r < 0 || tmp_r > ROWS - 1 || tmp_c < 0 || tmp_c >= COLUMNS-1) continue;

            if(terr[tmp_r][tmp_c] == '%') continue;
            if(dist[r][c] + (int)terr[tmp_r][tmp_c] < dist[tmp_r][tmp_c]){
                ancestor[tmp_r][tmp_c] = r * COLUMNS + c;
                dist[tmp_r][tmp_c] = dist[r][c] + (int)terr[tmp_r][tmp_c];
                if(terr[tmp_r][tmp_c] == '#') continue;
                if(0 == terr[tmp_r][tmp_c]){
                    queue_enqueue_front(&q, tmp_r * COLUMNS + tmp_c);
                }else{
                    queue_enqueue(&q, tmp_r * COLUMNS + tmp_c);
                }
            }
        }
    }


    queue_enqueue(&q, w_exit * COLUMNS + dw);
    queue_enqueue(&q, e_exit * COLUMNS + COLUMNS - 2 + de);
    queue_enqueue(&q, n_exit + dn * COLUMNS);
    queue_enqueue(&q, COLUMNS * (ROWS - 1 + ds) + s_exit);

    while(!queue_is_empty(&q)){
        queue_dequeue(&q, &u);

        r = u / COLUMNS;
        c = u % COLUMNS;

        while(!(r == cross_r && c == cross_c)){
            terr[r][c] = '#';
            u = ancestor[r][c];
            r = u / COLUMNS;
            c = u % COLUMNS;

            if(terr[r][c] == '#') break;
        }
    }
        
    queue_destroy(&q);
    
}

void map::elements(){
    int rgn_water = 1 + rand()%4;
    int rgn_tllg  = 2 + rand()%3;
    int rgn_shg   = 2 + rand()%3;
    
    int i;

    queue_init(&q);

    int r,c;

    for(i = 0; i < rgn_water; i++){
        do{
            r = 1 + rand()%(ROWS-2);
            c = 1 + rand()%(COLUMNS - 3);
        }while(terr[r][c] == '~' || terr[r][c] == '#');
        terr[r][c] = '~';
        wcells++;
        queue_enqueue(&q, r * COLUMNS + c);
    }

    for(i = 0; i < rgn_tllg; i++){
        do{
            r = 1 + rand()%(ROWS-2);
            c = 1 + rand()%(COLUMNS - 3);
        }while(terr[r][c] == '~' || terr[r][c] == '#' || terr[r][c] == ':');
        terr[r][c] = ':';
        queue_enqueue(&q, r * COLUMNS + c);
    }

    for(i = 0; i < rgn_shg; i++){
        do{
            r = 1 + rand()%(ROWS-2);
            c = 1 + rand()%(COLUMNS - 3);
        }while(terr[r][c] == '.' || terr[r][c] == '#' || terr[r][c] == ':' || terr[r][c] == '~');
        terr[r][c] = '.';
        queue_enqueue(&q, r * COLUMNS + c);
    }
    
    int u,t;
    int tmp_r, tmp_c;

    while(!queue_is_empty(&q)){
        queue_dequeue(&q, &u);
        
        r = u / COLUMNS;
        c = u % COLUMNS;

        for(i = 0; i < 4; i++){
            tmp_r = r + offset_r[i];
            tmp_c = c + offset_c[i];

            if(terr[tmp_r][tmp_c] == '#' || terr[tmp_r][tmp_c] == '%') continue;

            if(terr[tmp_r][tmp_c] == ','){
                terr[tmp_r][tmp_c] = terr[r][c];
                queue_enqueue(&q, tmp_r * COLUMNS + tmp_c);
            }else{
                t = rand()%353;
                if(t == 0){
                    terr[tmp_r][tmp_c] = '%';
                }else if(t == 1){
                    terr[tmp_r][tmp_c] = '^';
                }else if(t == 2){
                    terr[tmp_r][tmp_c] = '.';
                }else if(t == 3){
                    terr[tmp_r][tmp_c] = '~';
                    wcells++;
                }else if(t == 4){
                    terr[tmp_r][tmp_c] = ':';
                }
            }
        }
    }

    for(r = 0; r < ROWS; r++){
        for(c = 0; c < COLUMNS; c++){
            if(terr[r][c] == ','){
                t = rand()%163;
                if(t < 90){
                    terr[r][c] = '.';
                }
                else{
                    terr[r][c] = ':';
                }
            }
        }
    }

    
    distance = abs(rx) + abs(ry);
    if(rand()%101 > (400 - distance)/4) return;

    bool pokeC = false, pokeM = false,find = false;

    poke_c_c = poke_c_r = -1;
    int x,y,tr,tc;

    do{
        r = 1 + rand()%18;

        find = false;
        
        for(c = 1; c < COLUMNS - 3 && !find; c++){
            if(terr[r][c] == '#' || terr[r+1][c] == '#' || terr[r][c+1] == '#' || terr[r+1][c+1] == '#') continue;
            if(terr[r][c] == 'M' || terr[r+1][c] == 'M' || terr[r][c+1] == 'M' || terr[r+1][c+1] == 'M') continue;
            if(terr[r][c] == 'C' || terr[r+1][c] == 'C' || terr[r][c+1] == 'C' || terr[r+1][c+1] == 'C') continue;
            for(x = 0; x < 2 && !find; x++){
                for(y = 0; y < 2 && !find; y++){
                    for(i = 0; i < 4 && !find ; i++){
                        tr = r + x + offset_r[i];
                        tc = c + y + offset_c[i];

                        if(terr[tr][tc] == '#'){
                            find = true;
                        }
                    }
                }
            }
            if(find) break;
        }

        if(find){
            if(!pokeC){
                pokeC = true;
                market = 1;
                terr[r][c] = terr[r+1][c] = terr[r][c+1] = terr[r+1][c+1] = 'C';
                poke_c_c = c;
                poke_c_r = r;
            }else{
                store = 1;
                pokeM = true;
                terr[r][c] = terr[r+1][c] = terr[r][c+1] = terr[r+1][c+1] = 'M'; 
            }
        }

    }while(!pokeC || !pokeM);

    queue_destroy(&q);
}

void map::init(){
    int i,j;

    for(i = 0; i < ROWS; i++){
        terr[i][COLUMNS-1] = '\0';
    }

    for(i = 0; i < COLUMNS-1; i++){
        terr[0][i] = terr[ROWS-1][i] = '%';
    }

    for(i = 0; i < ROWS; i++){
        terr[i][0] = terr[i][COLUMNS-2] = '%';
    }
    dw = de = dn = ds = 0;

    if(w_exit == -1) w_exit = min(4 + rand()%ROWS, ROWS - 4);
    if(e_exit == -1) e_exit = min(4 + rand()%ROWS, ROWS - 4);
    if(n_exit == -1) n_exit = min(4 + rand()%COLUMNS, COLUMNS - 5);
    if(s_exit == -1) s_exit = min(4 + rand()%COLUMNS, COLUMNS - 5);

    if(w_exit == -2) w_exit = e_exit, dw = COLUMNS - 2;
    if(e_exit == -2) e_exit = w_exit, de = 2 - COLUMNS;
    if(n_exit == -2) n_exit = s_exit, dn = ROWS - 1;
    if(s_exit == -2) s_exit = n_exit, ds = 1 - ROWS; 

    for(i = 1; i < ROWS-1; i++){
        for(j = 1; j < COLUMNS-2; j++){
            terr[i][j] = rand()%2;
        }
    }

    terr[w_exit][0 + dw] = terr[e_exit][COLUMNS-2 + de] = terr[0 + dn][n_exit] = terr[ROWS-1 + ds][s_exit] = '#';

    int t = rand()%4;

    if(t == 0){ 
        cross_r = w_exit;
        cross_c = 0 + dw;
    }else if(t == 1){ 
        cross_r = e_exit;
        cross_c = COLUMNS-2 + de;
    }else if(t == 2){ 
        cross_r = 0 + dn;
        cross_c = n_exit;
    }else{
        cross_r = ROWS - 1 + ds;
        cross_c = s_exit;
    }

    terr[cross_r][cross_c] = 'X';
    
    paths_generation();
    
    for(i = 1; i < ROWS-1; i++){
        for(j = 1; j < COLUMNS-2; j++){
            if(terr[i][j] == 0 || terr[i][j] == 1) terr[i][j] = ',';
        }
    }
    terr[cross_r][cross_c] = '#';
}


void map::set_exits(char c, int val){
    switch(c){
        case 'n':
            n_exit = val;
            break;
        case 's':
            s_exit = val;
            break;
        case 'w':
            w_exit = val;
            break;
        case 'e':
            e_exit = val;
            break;
        case 'a':
            n_exit = s_exit = e_exit = w_exit = val;
            break;
    }
}

void map::set_trainers(int numtrnrs_sw){
    if(numtrnrs_sw == 0) n_trnrs = MAX_TRAINERS;
    else
        n_trnrs = numtrnrs_sw;

    int type = 1,n_tmp = n_trnrs, t, i, j = 0;
    char cx;
    pos_t pos;

    while(n_tmp > 0){ 
        t = (int)(type <= 2) + rand()%(n_tmp + 1);
        if(t > n_tmp) t = n_tmp;
        if(t == n_tmp && type == 1 && t > 1) t >>= 1;
        n_tmp -= t;
        while(t-- > 0){
            if(type == SWIMMER && wcells < 10) break;
            while(true){
                switch(type){
                    case HIKER:
                        cx = 'h';
                        pos = get_rand_pos(terr_type[2 + rand()%7]); 
                        break;
                    case RIVAL:
                        cx = 'r';
                        pos = get_rand_pos(terr_type[2 + rand()%5]); 
                        break;
                    case PACER:
                        cx = 'p';
                        pos = get_rand_pos(terr_type[2 + rand()%5]); 
                        break;
                    case WANDERER:
                        cx = 'w';
                        pos = get_rand_pos(terr_type[2 + rand()%5]); 
                        break;
                    case SENTRIE:
                        cx = 's';
                        pos = get_rand_pos(terr_type[2 + rand()%5]); 
                        break;
                    case EXPLORER:
                        cx = 'e';
                        pos = get_rand_pos(terr_type[2 + rand()%5]); 
                        break;
                    case SWIMMER:
                        cx = 'm';
                        pos = get_rand_pos(terr_type[9]); 
                        break;
                    default:
                        cx = 'h';
                        type = 1;
                }
                
                for(i = 0; i < j; i++){
                    if(pos.r == arr_trnr[i].r && pos.c == arr_trnr[i].c) break;
                }

                if (i < j) continue;
                
                j++;
                break;
            }
            arr_trnr[j - 1].init(type, pos.r, pos.c, cx, (char)(rand()%8)); 
        }
        type <<= 1;
    }

    qsort(&arr_trnr, n_trnrs, sizeof(npc), cmp_trnr_pos);
    
}
void map::set(int a, int b, int numtrnrs_sw, int rand_pc){
    rx = a;
    ry = b;
    wcells = 0;
    market = 0;
    store = 0;
    init();
    elements();
    if(rand_pc) rand_pos = get_rand_pos('#');

    set_trainers(numtrnrs_sw);
}
