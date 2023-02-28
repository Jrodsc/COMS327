#ifndef WORLD_H
#define WORLD_H

#define HEIGHT 401
#define WIDTH 401
#define INF 1000000000

typedef struct World{
    map_t * world[HEIGHT][WIDTH];
} world_t;

int go_world(world_t * w_t, int x, int y);

#endif
