#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void insertion_sort(int * a, int n){
    int i,j,t;

    for(i = 1; i < n; i++){
        for(t = a[i], j = i - 1; j > -1 && a[j] > t; j--){
            a[j + 1] = a[j];
        }
        a[j + 1] = t;
    }
}

void generic_insertion_sort(void *a, int n, int size, int (*compare)(const void * v1, const void * v2)){

    int i, j;
    void * t = malloc(size);
    char * p = a;
    for(i = 1; i < n; i++){
        for(memcpy(t,p + i * size, size), j = i - 1; j > -1 && compare(p + (j * size), t) > 0; j--){
            memcpy(p + (j + 1) * size, p + j * size, size);
        }
        memcpy(p + (j + 1) * size, t, size);
    }
    free(t);
}

int reverse_int_cmp(const void * v1, const void * v2){
    return *((int *) v2) - *((int *) v1);
}

int string_cmp(const void * v1, const void * v2){
    return strcmp(*(char **) v1, *(char **) v2);
}

int main(int argc, char * argv[]){
    int a[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    int i,n = sizeof(a)/sizeof(a[0]);

    insertion_sort(a,n);

    for(i = 0; i < n; i++){
        printf("%d ", a[i]);
    }
    printf("\n");

    generic_insertion_sort(a, n, sizeof(a[0]), reverse_int_cmp);

    for(i = 0; i < n; i++){
        printf("%d ", a[i]);
    }
    printf("\n");

    return 0;
}    
