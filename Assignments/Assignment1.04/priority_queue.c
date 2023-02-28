#include <stdlib.h>
#include <stdio.h>
#include "priority_queue.h"
#include "tlib.h"


int priority_queue_init(priority_queue_t * pq){
    pq -> size = 0;
    pq -> top  = NULL;
    return 0;
}

priority_queue_node_t * merge(priority_queue_node_t * t1, priority_queue_node_t * t2){
    if(NULL == t1 || NULL == t2)
        return NULL == t1 ? t2 : t1;
    priority_queue_node_t * tmp;
    if(*(unsigned long long int *)(t2 -> data) < *(unsigned long long int *)(t1 -> data)){
        tmp = t2;
        t2 = t1;
        t1 = tmp;
    }
    if(rand() & 1){
        tmp = t1 -> left;
        t1 -> left = t1 -> right;
        t1 -> right = tmp;
    }
    t1 -> left = merge(t1 -> left, t2);
    return t1;
}


int priority_queue_push(priority_queue_t * pq, void * val){
    priority_queue_node_t * n;

    if(!(n = malloc(sizeof(*n))))
        return 1;

    n -> data = val;
    n -> left = n -> right = NULL;

    pq -> top = merge(pq -> top, n);

    pq -> size++;

    return 0;
}

void * priority_queue_top(priority_queue_t * pq){
    return pq -> top -> data;
}

int priority_queue_size(priority_queue_t * pq){
    return pq -> size;
}

int priority_queue_empty(priority_queue_t * pq){
    return !pq -> size;
}

int priority_queue_pop(priority_queue_t * pq){
    if(!pq -> top) return 1;

    priority_queue_node_t * old;

    old = pq -> top;

    pq -> top = merge(pq -> top -> left, pq -> top -> right);

    pq -> size--;

    old -> data = NULL;
    old -> left = old -> right = NULL;

    free(old);

    return 0;
}

int priority_queue_destroy(priority_queue_t * pq){

    while(!priority_queue_pop(pq));

    return 0;
}




