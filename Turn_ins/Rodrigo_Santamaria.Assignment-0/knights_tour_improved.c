//Net-ID: jrodsc
//Rodrigo Santamaria
//COM S 327 Assignment0-extrachallenges

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

FILE *ftour;

int R,C;

bool* visited;
bool cylr,cylc,boolt;
int rs[8] = {-2,-2,-1,-1,1,1,2,2};
int cs[8] = {-1,1,-2,2,-2,2,-1,1};
int p;
long long int total = 0;

int* ans;

void tour(int r,int c){

    
    if(p >= R * C){
        total++;
        fprintf(ftour,"%d", ans[0]);
        for(int j = 1; j < R * C; j++){
            fprintf(ftour,",%d", ans[j]);
        }
        fprintf(ftour,"\n");
        return;
    }

    int j,i,rt,ct;

    visited[r * C + c] = true;

    for(i = 0; i < 8; i++){
        rt = r + rs[i];
        ct = c + cs[i];

        if(cylc){
            ct += C;
            ct %= C;
        }
        if(cylr){
            rt += R;
            rt %= R;
        }

        if(rt < 0 || rt > R-1 || ct < 0 || ct > C-1 || visited[rt * C + ct]) continue;

        if(cylc || cylr){

            int cx,rx;
            boolt = false;
            for(j = 0; j < i && !boolt; j++){
                cx = c + cs[j];
                rx = r + rs[j];

                if(cylc){
                    cx += C;
                    cx %= C;
                }
                if(cylr){
                    rx += R;
                    rx %= R;
                }

                if(ct == cx && rt == rx) boolt = true;
            }
            if(boolt) continue;
        }

        ans[p++] = rt * C + ct + 1;
        tour(rt,ct);
        p--;
    }
    visited[r * C + c] = false;
}

int main(int argc, char *argv[]){
    
    printf("Please, insert the number of rows: ");
    scanf("%d", &R);
    printf("Please, insert the number of columns: ");
    scanf("%d", &C);

    long long int t = -1;

    do{
        printf("Is your board a cylinder? (1) Yes, (0) No ");

        scanf("%lld",&t);
    }
    while(t < 0 || t > 1);

    if(t == 0){
        t = -1;
        do{
            printf("Is your board a torus? (1) Yes, (0) No ");

            scanf("%lld",&t);
        }
        while(t < 0 || t > 1);

        if(t == 1){
            cylr = cylc = true;
        }
    }else{
        t = -1;
        do{
            printf("Which type of cylinder is your board? (1) Vertical, (0) Horizontal ");

            scanf("%lld",&t);
        }
        while(t < 0 || t > 1);

        if(t == 0){
            cylc = true;
        }else{
            cylr = true;
        }
    }



    int i,j;    

    visited = (bool*)malloc(((R+1) * (C+1)) * sizeof(bool));
    ans = (int*)malloc(((R+1) * (C+1)) * sizeof(int));
    ftour = fopen("tours.txt", "w");

    for(i = 0; i < R; i++){
        for(j = 0; j < C; j++){
            p = 1;
            ans[0] = i * C + j + 1;
            tour(i,j);
        }
    }

    fclose(ftour);

    if(total == 0){
        printf("There are no valid tours for this configuration\n");

        return 0;
    }

    printf("The tours have been succesfully printed in tours.txt!\n");

    do{
        printf("\nDo you want to print any specific tour? (1) Yes (0) No" );
        
        scanf("%lld", &t);
    }while(t < 0 || t > 1);

    if(t == 1){
        do{
            printf("\n Choose a number between 1 and %lld: ",total);
            scanf("%lld", &t);
        }while(t < 1 || t > total);

        ftour = fopen("tours.txt","r");
        
        char * st;
        long unsigned int size = R * C - 1, tt = 1,cc = 1;

        for(int x = 1; x < R * C + 1; x++){
            if(x >= tt * 10){
                tt *= 10;
                cc++;
            }
            size += cc;
        }

        st = (char *) malloc(size);

        for(long long int x = 0; x < t; x++){
            getline(&st,&size, ftour);
        }

        printf("%s",st);
        printf("\n");

        int y = 0,c = 0;

        for(long unsigned int x = 0; x < size && st[x] != '\n'; x++){
            //printf("%d\n", y);
            if(st[x] == ','){
                ans[y - 1] = ++c;
                y = 0;
            }else{
                y *= 10;
                y += st[x] - '0';
            }
        }

        ans[y - 1] = ++c;

        for(i = 0; i < R; i++){
            for(j = 0; j < C; j++){
                printf("%3d", ans[i * R + j]);
            }
            printf("\n");
        }
    }


    return 0;
}
