#ifndef QUEUE_H //If not defined
#define QUEUE_H

typedef struct queue_node{
    int data;
    struct queue_node * next;
} queue_node_t;

typedef struct queue{
    struct queue_node *front, *back;
    int length;
} queue_t;

int queue_enqueue_front(queue_t *q, int i);
int queue_enqueue_back(queue_t *q, int i);
int queue_dequeue(queue_t *q, int *i);
int queue_front(queue_t *q, int *i);
int queue_length(queue_t *q);
int queue_is_empty(queue_t *q); // return 1 is empty 
int queue_init(queue_t *q);
int queue_destroy(queue_t *q);

#endif

