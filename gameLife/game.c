#include "simulation.h"

typedef struct {
    int *current;
    int *next;
} World;


int get_neighbors(int x, int y, int *grid) {
    int count = 0;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;

            int nx = x + dx;
            int ny = y + dy;

            if (nx >= 0 && nx < SIM_X_SIZE && ny >= 0 && ny < SIM_Y_SIZE) {
                if (grid[ny * SIM_X_SIZE + nx]) count++;
            }
        }
    }
    return count;
}


void evolve(World *w) {
    for (int y = 0; y < SIM_Y_SIZE; y++) {
        for (int x = 0; x < SIM_X_SIZE; x++) {
            int idx = y * SIM_X_SIZE + x;
            int alive = w->current[idx];
            int neighbors = get_neighbors(x, y, w->current);

            if (neighbors == 3) w->next[idx] = 1;
            else if (neighbors == 2) w->next[idx] = alive;
            else w->next[idx] = 0;

            simPutPixel(x, y, w->next[idx] ? COLOR_ALIVE : COLOR_DEAD);
        }
    }

    int *temp = w->current;
    w->current = w->next;
    w->next = temp;
}

void app() {
    int grid_a[SIM_X_SIZE * SIM_Y_SIZE];
    int grid_b[SIM_X_SIZE * SIM_Y_SIZE];

    World world = { .current = grid_a, .next = grid_b };

 
    for (int i = 0; i < SIM_X_SIZE * SIM_Y_SIZE; i++) {
        world.current[i] = simRand();
    }

    while (1) {
        evolve(&world);
        simFlush();
    }
}