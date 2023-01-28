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

#define ROWS 21
#define COLUMNS 85 // real max 83

inline int min(int  a, int  b){ 
    if( a < b) return a; 
    return b;
}

inline int max(int a, int b){ 
    if( a > b) return a; 
    return b;
}

char terrain[ROWS][COLUMNS];

int west_exit, east_exit, north_exit, south_exit, cross_r, cross_c;

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

    west_exit = 1 + rand()%19;
    east_exit = 1 + rand()%19;
    north_exit = 1 + rand()%81;
    south_exit = 1 + rand()%81;

    for(i = 1; i < ROWS-1; i++){
        for(j = 1; j < COLUMNS-2; j++){
            terrain[i][j] = ':';
        }
    }

    terrain[west_exit][0] = terrain[east_exit][COLUMNS-2] = terrain[0][north_exit] = terrain[ROWS-1][south_exit] = '#';

    cross_r = min(3 + rand()%ROWS, ROWS -3);
    cross_c = min(3 + rand()%COLUMNS, COLUMNS -4);

    terrain[cross_r][cross_c] = 'X';
    
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
