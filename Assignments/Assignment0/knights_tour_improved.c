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

int* ans;

void tour(int r,int c){

    
    if(p >= R * C){
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

    int t = -1;

    do{
        printf("Is your board a cylinder? (1) Yes, (0) No ");

        scanf("%d",&t);
    }
    while(t < 0 || t > 1);

    if(t == 0){
        t = -1;
        do{
            printf("Is your board a torus? (1) Yes, (0) No ");

            scanf("%d",&t);
        }
        while(t < 0 || t > 1);

        if(t == 1){
            cylr = cylc = true;
        }
    }else{
        t = -1;
        do{
            printf("Which type of cylinder is your board? (1) Vertical, (0) Horizontal ");

            scanf("%d",&t);
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

    


    return 0;
}
