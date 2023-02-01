#include <stdio.h>

#include "queue.h"

int main(int argc, char * argv[]){

    int i;
    queue_t q;

    queue_init(&q);

    printf("Length: %d\n", queue_length(&q));

    for(i = 0; i < 100; i++){
        queue_enqueue(&q, i);
    }

    printf("Length: %d\n", queue_length(&q));
    printf("Queue is %s\n", queue_is_empty(&q) ? "empty" : "not empty");

    queue_front(&q, &i);

    printf("Front: %d\n", i);

    while(!queue_dequeue(&q, &i)){
        printf("%d\n", i);
    }

    queue_destroy(&q);

    printf("Queue is %s\n", queue_is_empty(&q) ? "empty" : "not empty");

    return 0;
}
