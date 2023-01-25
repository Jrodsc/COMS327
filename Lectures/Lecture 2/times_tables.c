#include <stdio.h>

#define SIZE 10

void populate_times_table(int table[SIZE][SIZE]){
    int i,j;

    for(i = 0; i < SIZE; i++){
        for(j = 0; j < SIZE; j++){
            table[i][j] = (i+1) * (j+1);
        }
    }
}

int main(int argc, char *argv[]){
    int table[SIZE][SIZE];

    populate_times_table(table);

    int i,j;

    for(i = 0; i < SIZE; i++){
        for(j = 0; j < SIZE; j++){
            printf("%3d ", table[i][j]);
        }
        printf("\n");
    }
    return 0;
}