//Net-ID: jrodsc
//Rodrigo Santamaria
//COM S 327 Assignment1.01

// >= 2 TG :
// >= 1 W ~
// >= 2 SG
// boulders or trees


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "queue.h"

#define INF 1000000000
#define ROWS 21
#define COLUMNS 81 // real max 83

//Basic functions

int min(int  a, int  b){ 
    if( a < b) return a; 
    return b;
}

int max(int a, int b){ 
    if( a > b) return a; 
    return b;
}

//Global variables

char terrain[ROWS][COLUMNS];
int ancestor[ROWS][COLUMNS];
int dist[ROWS][COLUMNS];
int offset_r[4] = {0, 1, 0, -1};
int offset_c[4] = {1, 0, -1 , 0};
int west_exit, east_exit, north_exit, south_exit, cross_r, cross_c;

//Process functions

void search_ancestor(int r, int c){
    int a;
    while(!(r == cross_r && c == cross_c)){
        printf("%d %d\n", r, c);
        terrain[r][c] = '#';
        a = ancestor[r][c];
        r = a / COLUMNS;
        c = a % COLUMNS;

        if(terrain[r][c] == '#') break;
    }
}
        

void paths_generation(){

    int u,v;

    for(u = 0; u < ROWS; u++){
        for(v = 0; v < COLUMNS; v++){
            ancestor[u][v] = -1;
            dist[u][v] = INF;
        }
    }

    dist[cross_r][cross_c] = 0;
    ancestor[cross_r][cross_c] = -2;

    queue_t q;

    queue_init(&q);

    queue_enqueue_front(&q, cross_r * COLUMNS + cross_c);

    int tmp_r, tmp_c,r,c;

    while(!queue_is_empty(&q)){
        queue_dequeue(&q, &u);

        r = u / COLUMNS;
        c = u % COLUMNS;

        printf("%d, %d\n", r, c);

        for(v = 0; v < 4 ; v++){
            tmp_r = r + offset_r[v];
            tmp_c = c + offset_c[v];

            if(tmp_r < 0 || tmp_r > ROWS - 1 || tmp_c < 0 || tmp_c >= COLUMNS-1) continue;

            if(terrain[tmp_r][tmp_c] == '%') continue;
            if(dist[r][c] + (int)(terrain[tmp_r][tmp_c] - '0') < dist[tmp_r][tmp_c]){
                ancestor[tmp_r][tmp_c] = r * COLUMNS + c;
                dist[tmp_r][tmp_c] = dist[r][c] + (int)(terrain[tmp_r][tmp_c] - '0');
                if(terrain[tmp_r][tmp_c] == '#') continue;
                if(terrain[tmp_r][tmp_c] == '0'){
                    queue_enqueue_front(&q, tmp_r * COLUMNS + tmp_c);
                }else{
                    queue_enqueue_back(&q, tmp_r * COLUMNS + tmp_c);
                }
            }
        }
    }

    search_ancestor(west_exit, 0);
    search_ancestor(east_exit, COLUMNS -2);
    search_ancestor(0, north_exit);
    search_ancestor(ROWS - 1, south_exit);

    
}



void init(){
    int i,j;

    for(i = 0; i < ROWS; i++){
        terrain[i][COLUMNS-1] = '\0';
    }

    for(i = 0; i < COLUMNS-1; i++){
        terrain[0][i] = terrain[ROWS-1][i] = '%';
    }

    for(i = 0; i < ROWS; i++){
        terrain[i][0] = terrain[i][COLUMNS-2] = '%';
    }

    west_exit = min(4 + rand()%ROWS, ROWS - 4);
    east_exit = min(4 + rand()%ROWS, ROWS - 4);
    north_exit = min(4 + rand()%COLUMNS, COLUMNS - 5);
    south_exit = min(4 + rand()%COLUMNS, COLUMNS - 5);

    for(i = 1; i < ROWS-1; i++){
        for(j = 1; j < COLUMNS-2; j++){
            terrain[i][j] = '0' + rand()%2;
        }
    }

    terrain[west_exit][0] = terrain[east_exit][COLUMNS-2] = terrain[0][north_exit] = terrain[ROWS-1][south_exit] = '#';

//    cross_r = min(4 + rand()%ROWS, ROWS - 4);
//    cross_c = min(4 + rand()%COLUMNS, COLUMNS - 5);

    int t = rand()%4;

    if(t == 0){ //west
        cross_r = west_exit;
        cross_c = 0;
    }else if(t == 1){ //east
        cross_r = east_exit;
        cross_c = COLUMNS-2;
    }else if(t == 2){ // north
        cross_r = 0;
        cross_c = north_exit;
    }else{
        cross_r = ROWS - 1;
        cross_c = south_exit;
    }

    terrain[cross_r][cross_c] = 'X';
    
    paths_generation();
    
    for(i = 1; i < ROWS-1; i++){
        for(j = 1; j < COLUMNS-2; j++){
            if(terrain[i][j] == '0' || terrain[i][j] == '1') terrain[i][j] = ':';
        }
    }
    terrain[cross_r][cross_c] = '#';
}



int main(int argc, char * argv[]){

    srand(time(NULL));

    init();

    int i;
  
    for(i = 0; i < ROWS; i++){
        printf("%s\n",terrain[i]);
    }
    return 0;
}
