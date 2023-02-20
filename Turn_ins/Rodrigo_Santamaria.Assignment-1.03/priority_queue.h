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
int priority_queue_init();
int priority_queue_push();
void * priority_queue_top();
int priority_queue_size();
int priority_queue_empty();
int priority_queue_pop();
int priority_queue_destroy();

#endif
