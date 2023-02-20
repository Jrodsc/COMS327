#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tlib.h"
#include "queue.h"
#include "map.h"

//Process functions

queue_t q;

int offset_r[4] = {0, 1, 0, -1};
int offset_c[4] = {1, 0, -1 , 0};
int dw = 0,de = 0,dn = 0,ds = 0;
int distance;

void get_rand_pos(map_t * m, char cx){
    bool flag = false;
    int r;
    do{
        r = 1 + rand()%19;

        for(int c = 1; c < COLUMNS - 3 && !flag; c++){
            if(m -> terr[r][c] == cx){
                m -> rand_pos.r = r;
                m -> rand_pos.c = c;
                flag = true;
            }
        }
    }while(!flag);

    m -> terr[m -> rand_pos.r][m -> rand_pos.c] = '@';
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

void map_print_terrain(map_t * m){

    int i,j;
    system("clear");
  
    for(i = 0; i < ROWS; i++){
        for(j = 0; j < COLUMNS - 1; j++){
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

void map_set(map_t * m,int a,int b){
    m -> rx = a;
    m -> ry = b;
    map_init(m);
    map_elements(m);
    get_rand_pos(m, '#');

}
