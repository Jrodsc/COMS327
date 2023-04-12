#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

typedef struct priority_queue_node{
    void * data;
    struct priority_queue_node * left, * right;
}priority_queue_node_t;

typedef struct priority_queue{
    int size;
    priority_queue_node_t * top;
} priority_queue_t;

    
priority_queue_node_t * merge(priority_queue_node_t * t1, priority_queue_node_t * t2);
int priority_queue_init(priority_queue_t * pq);
int priority_queue_push(priority_queue_t * pq, void * val);
void * priority_queue_top(priority_queue_t * pq);
int priority_queue_size(priority_queue_t * pq);
int priority_queue_empty(priority_queue_t * pq);
int priority_queue_pop(priority_queue_t * pq);
int priority_queue_destroy();

#endif
