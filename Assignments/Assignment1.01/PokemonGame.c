//Net-ID: jrodsc
//Rodrigo Santamaria
//COM S 327 Assignment1.01

#include <stdio.h>

#define ROWS 21
#define COLUMNS 84

char terrain[ROWS][COLUMNS];

int main(int argc, char * argv[]){

    int i,j;

    for(i = 0; i < ROWS; i++){
//        terrain[i][COLUMNS-1] = '\0';
    }

    for(i = 0; i < COLUMNS; i++){
        terrain[0][i] = terrain[ROWS-1][i] = '%';
    }

    for(i = 0; i < ROWS; i++){
        terrain[i][0] = terrain[i][COLUMNS-1] = '%';
    }

    for(i = 1; i < ROWS-1; i++){
        for(j = 1; j < COLUMNS-1; j++){
            terrain[i][j] = ':';
        }
    }

    for(i = 0; i < ROWS; i++){
        printf("%s\n",terrain[i]);
    }
    return 0;
}
