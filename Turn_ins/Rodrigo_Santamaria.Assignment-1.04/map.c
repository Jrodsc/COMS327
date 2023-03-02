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
    trainer_t p1_t = *(trainer_t *)p1;
    trainer_t p2_t = *(trainer_t *)p2;

    if(p1_t.r == p2_t.r){
        return (int)!(p1_t.c < p2_t.c);
    }

    return (int)!(p1_t.r < p2_t.r);
}

//Bldr Tree Path PMart PCntr TGras SGras Mtn Forest Water Gate

char terr_type[11] = {'%', '^', '#', 'M', 'C', ':', '.', '%', '^', '~', '#'};

void dijkstra(int rp,int cp,map_t * m, int t, unsigned long long int dist[ROWS][COLUMNS]){
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

        if(m -> terr[rt][ct] == '~')
            round_water++;
    }

    dist[rp][cp] = INF * (t == SWIMMER && !round_water);

    if(t == SWIMMER) t = 3;

    s = malloc(sizeof(*s));
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
            switch(m -> terr[rt][ct]){
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
                s = malloc(sizeof(*s));
                *s = v + (dist[rt][ct] << 32);
                priority_queue_push(&pq, (void *)s);

            }
        }
    }
    
    /* 
    int ten,uni;

    for(r = 0; r < ROWS; r++){
        for(c = 0; c < COLUMNS - 1; c++){
            printf("%llu\t", dist[r][c]);
            continue;
            if(dist[r][c] == INF)
                printf("   ");
            else{
                ten = ((int)(dist[r][c])%100)/10;
                uni = ((int)(dist[r][c])%10);
                printf("%d%d ", ten,uni);
            }
        }
        printf("\n");
    }
    */
    
}

pos_t get_rand_pos(map_t * m, char cx){
    bool flag = false;
    pos_t t;
    int r,c;    
    do{
        r = 1 + rand()%(ROWS - 2);
        c = 1 + rand()%(COLUMNS-3);

        if(m -> terr[r][c] == cx){
            t.r = r;
            t.c = c;
            flag = true;
        }
    }while(!flag);


    return t;
}

void map_paths_generation(map_t * m){

    int ancestor[ROWS][COLUMNS];
    int dist[ROWS][COLUMNS];

    int u,v;

    for(u = 0; u < ROWS; u++){
        for(v = 0; v < COLUMNS; v++){
            ancestor[u][v] = -1;
            dist[u][v] = INF;
        }
    }

    dist[m -> cross_r][m -> cross_c] = 0;
    ancestor[m -> cross_r][m -> cross_c] = -2;

    queue_init(&q);

    queue_enqueue_front(&q, m -> cross_r * COLUMNS + m -> cross_c);

    int tmp_r, tmp_c,r,c;

    while(!queue_is_empty(&q)){
        queue_dequeue(&q, &u);

        r = u / COLUMNS;
        c = u % COLUMNS;

        for(v = 0; v < 4 ; v++){
            tmp_r = r + offset_r[v];
            tmp_c = c + offset_c[v];

            if(tmp_r < 0 || tmp_r > ROWS - 1 || tmp_c < 0 || tmp_c >= COLUMNS-1) continue;

            if(m -> terr[tmp_r][tmp_c] == '%') continue;
            if(dist[r][c] + (int)m -> terr[tmp_r][tmp_c] < dist[tmp_r][tmp_c]){
                ancestor[tmp_r][tmp_c] = r * COLUMNS + c;
                dist[tmp_r][tmp_c] = dist[r][c] + (int)m -> terr[tmp_r][tmp_c];
                if(m -> terr[tmp_r][tmp_c] == '#') continue;
                if(0 == m -> terr[tmp_r][tmp_c]){
                    queue_enqueue_front(&q, tmp_r * COLUMNS + tmp_c);
                }else{
                    queue_enqueue(&q, tmp_r * COLUMNS + tmp_c);
                }
            }
        }
    }


    queue_enqueue(&q, m -> w_exit * COLUMNS + dw);
    queue_enqueue(&q, m -> e_exit * COLUMNS + COLUMNS - 2 + de);
    queue_enqueue(&q, m -> n_exit + dn * COLUMNS);
    queue_enqueue(&q, COLUMNS * (ROWS - 1 + ds) + m -> s_exit);

    while(!queue_is_empty(&q)){
        queue_dequeue(&q, &u);

        r = u / COLUMNS;
        c = u % COLUMNS;

        while(!(r == m -> cross_r && c == m -> cross_c)){
            m -> terr[r][c] = '#';
            u = ancestor[r][c];
            r = u / COLUMNS;
            c = u % COLUMNS;

            if(m -> terr[r][c] == '#') break;
        }
    }
        
    queue_destroy(&q);
    
}

void map_elements(map_t * m){
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
        }while(m -> terr[r][c] == '~' || m -> terr[r][c] == '#');
        m -> terr[r][c] = '~';
        queue_enqueue(&q, r * COLUMNS + c);
    }

    for(i = 0; i < rgn_tllg; i++){
        do{
            r = 1 + rand()%(ROWS-2);
            c = 1 + rand()%(COLUMNS - 3);
        }while(m -> terr[r][c] == '~' || m -> terr[r][c] == '#' || m -> terr[r][c] == ':');
        m -> terr[r][c] = ':';
        queue_enqueue(&q, r * COLUMNS + c);
    }

    for(i = 0; i < rgn_shg; i++){
        do{
            r = 1 + rand()%(ROWS-2);
            c = 1 + rand()%(COLUMNS - 3);
        }while(m -> terr[r][c] == '.' || m -> terr[r][c] == '#' || m -> terr[r][c] == ':' || m -> terr[r][c] == '~');
        m -> terr[r][c] = '.';
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

            if(m -> terr[tmp_r][tmp_c] == '#' || m -> terr[tmp_r][tmp_c] == '%') continue;

            if(m -> terr[tmp_r][tmp_c] == ','){
                m -> terr[tmp_r][tmp_c] = m -> terr[r][c];
                queue_enqueue(&q, tmp_r * COLUMNS + tmp_c);
            }else{
                t = rand()%353;
                if(t == 0){
                    m -> terr[tmp_r][tmp_c] = '%';
                }else if(t == 1){
                    m -> terr[tmp_r][tmp_c] = '^';
                }else if(t == 2){
                    m -> terr[tmp_r][tmp_c] = '.';
                }else if(t == 3){
                    m -> terr[tmp_r][tmp_c] = '~';
                }else if(t == 4){
                    m -> terr[tmp_r][tmp_c] = ':';
                }
            }
        }
    }

    for(r = 0; r < ROWS; r++){
        for(c = 0; c < COLUMNS; c++){
            if(m -> terr[r][c] == ','){
                t = rand()%163;
                if(t < 90){
                    m -> terr[r][c] = '.';
                }
                else{
                    m -> terr[r][c] = ':';
                }
            }
        }
    }

    
    distance = abs(m -> rx) + abs(m -> ry);
    if(rand()%101 > (400 - distance)/4) return;

    bool pokeC = false, pokeM = false,find = false;

    int x,y,tr,tc;

    do{
        r = 1 + rand()%18;

        find = false;
        
        for(c = 1; c < COLUMNS - 3 && !find; c++){
            if(m -> terr[r][c] == '#' || m -> terr[r+1][c] == '#' || m -> terr[r][c+1] == '#' || m -> terr[r+1][c+1] == '#') continue;
            if(m -> terr[r][c] == 'M' || m -> terr[r+1][c] == 'M' || m -> terr[r][c+1] == 'M' || m -> terr[r+1][c+1] == 'M') continue;
            if(m -> terr[r][c] == 'C' || m -> terr[r+1][c] == 'C' || m -> terr[r][c+1] == 'C' || m -> terr[r+1][c+1] == 'C') continue;
            for(x = 0; x < 2 && !find; x++){
                for(y = 0; y < 2 && !find; y++){
                    for(i = 0; i < 4 && !find ; i++){
                        tr = r + x + offset_r[i];
                        tc = c + y + offset_c[i];

                        if(m -> terr[tr][tc] == '#'){
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
                m -> terr[r][c] = m -> terr[r+1][c] = m -> terr[r][c+1] = m -> terr[r+1][c+1] = 'C';
            }else{
                pokeM = true;
                m -> terr[r][c] = m -> terr[r+1][c] = m -> terr[r][c+1] = m -> terr[r+1][c+1] = 'M'; 
            }
        }

    }while(!pokeC || !pokeM);

    queue_destroy(&q);
}

void map_init(map_t * m){
    int i,j;

    for(i = 0; i < ROWS; i++){
        m -> terr[i][COLUMNS-1] = '\0';
    }

    for(i = 0; i < COLUMNS-1; i++){
        m -> terr[0][i] = m -> terr[ROWS-1][i] = '%';
    }

    for(i = 0; i < ROWS; i++){
        m -> terr[i][0] = m -> terr[i][COLUMNS-2] = '%';
    }
    dw = de = dn = ds = 0;

    if(m -> w_exit == -1) m -> w_exit = min(4 + rand()%ROWS, ROWS - 4);
    if(m -> e_exit == -1) m -> e_exit = min(4 + rand()%ROWS, ROWS - 4);
    if(m -> n_exit == -1) m -> n_exit = min(4 + rand()%COLUMNS, COLUMNS - 5);
    if(m -> s_exit == -1) m -> s_exit = min(4 + rand()%COLUMNS, COLUMNS - 5);

    if(m -> w_exit == -2) m -> w_exit = m -> e_exit, dw = COLUMNS - 2;
    if(m -> e_exit == -2) m -> e_exit = m -> w_exit, de = 2 - COLUMNS;
    if(m -> n_exit == -2) m -> n_exit = m -> s_exit, dn = ROWS - 1;
    if(m -> s_exit == -2) m -> s_exit = m -> n_exit, ds = 1 - ROWS; 

    for(i = 1; i < ROWS-1; i++){
        for(j = 1; j < COLUMNS-2; j++){
            m -> terr[i][j] = rand()%2;
        }
    }

    m -> terr[m -> w_exit][0 + dw] = m -> terr[m -> e_exit][COLUMNS-2 + de] = m -> terr[0 + dn][m -> n_exit] = m -> terr[ROWS-1 + ds][m -> s_exit] = '#';

    int t = rand()%4;

    if(t == 0){ 
        m -> cross_r = m -> w_exit;
        m -> cross_c = 0 + dw;
    }else if(t == 1){ 
        m -> cross_r = m -> e_exit;
        m -> cross_c = COLUMNS-2 + de;
    }else if(t == 2){ 
        m -> cross_r = 0 + dn;
        m -> cross_c = m -> n_exit;
    }else{
        m -> cross_r = ROWS - 1 + ds;
        m -> cross_c = m -> s_exit;
    }

    m -> terr[m -> cross_r][m -> cross_c] = 'X';
    
    map_paths_generation(m);
    
    for(i = 1; i < ROWS-1; i++){
        for(j = 1; j < COLUMNS-2; j++){
            if(m -> terr[i][j] == 0 || m -> terr[i][j] == 1) m -> terr[i][j] = ',';
        }
    }
    m -> terr[m -> cross_r][m -> cross_c] = '#';
}


void map_set_exits(map_t * m, char c, int val){
    switch(c){
        case 'n':
            m -> n_exit = val;
            break;
        case 's':
            m -> s_exit = val;
            break;
        case 'w':
            m -> w_exit = val;
            break;
        case 'e':
            m -> e_exit = val;
            break;
        case 'a':
            m -> n_exit = m -> s_exit = m -> e_exit = m -> w_exit = val;
            break;
        //default:
            //fprintf(stderr,"Error, valid direction");
    }
}

void set_trainers(map_t * m){
    m -> n_trnrs = 1 + rand()%MAX_TRAINERS;

    int type = 1,n_tmp = m -> n_trnrs, t, i, j = 0;
    char cx;
    pos_t pos;

    while(n_tmp > 0){ 
        t = (int)(type <= 2) + rand()%(n_tmp + 1);
        if(t > n_tmp) t = n_tmp;
        if(t == n_tmp && type == 1 && t > 1) t >>= 1;
        n_tmp -= t;
        while(t-- > 0){
            while(true){
                switch(type){
                    case HIKER:
                        cx = 'h';
                        pos = get_rand_pos(m, terr_type[2 + rand()%7]); 
                        break;
                    case RIVAL:
                        cx = 'r';
                        pos = get_rand_pos(m, terr_type[2 + rand()%5]); 
                        break;
                    case PACER:
                        cx = 'p';
                        pos = get_rand_pos(m, terr_type[2 + rand()%5]); 
                        break;
                    case WANDERER:
                        cx = 'w';
                        pos = get_rand_pos(m, terr_type[2 + rand()%5]); 
                        break;
                    case SENTRIE:
                        cx = 's';
                        pos = get_rand_pos(m, terr_type[2 + rand()%5]); 
                        break;
                    case EXPLORER:
                        cx = 'e';
                        pos = get_rand_pos(m, terr_type[2 + rand()%5]); 
                        break;
                    case SWIMMER:
                        cx = 'm';
                        pos = get_rand_pos(m, terr_type[9]); 
                        break;
                    default:
                        cx = 'h';
                        type = 1;
                }
                
                for(i = 0; i < j; i++){
                    if(pos.r == m -> arr_trnr[i].r && pos.c == m -> arr_trnr[i].c) break;
                }

                if (i < j) continue;
                
                j++;
                break;
            }
            trainer_init(&m -> arr_trnr[j - 1], type, pos.r, pos.c, cx, (char)(rand()%8)); 
        }
        type <<= 1;
    }

    qsort(&m -> arr_trnr, m -> n_trnrs, sizeof(trainer_t), cmp_trnr_pos);
}

void map_set(map_t * m,int a,int b){
    m -> rx = a;
    m -> ry = b;
    map_init(m);
    map_elements(m);
    pos_t t = get_rand_pos(m, '#');

    m -> rand_pos = t;
    set_trainers(m);
}
