#include <SDL2/SDL.h>
#include <time.h>
#include <assert.h>
#include "simulation.h"

#define MS_PER_FRAME 50

static struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    Uint32 last_frame_time;
} ctx;

void simInit() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(SIM_X_SIZE, SIM_Y_SIZE, 0, &ctx.window, &ctx.renderer);
    srand((unsigned int)time(NULL));
    
    SDL_SetRenderDrawColor(ctx.renderer, 0, 0, 0, 255);
    SDL_RenderClear(ctx.renderer);
    ctx.last_frame_time = SDL_GetTicks();
}

void simExit() {
    SDL_Event e;
    while (SDL_WaitEvent(&e)) {
        if (e.type == SDL_QUIT) break;
    }
    SDL_DestroyRenderer(ctx.renderer);
    SDL_DestroyWindow(ctx.window);
    SDL_Quit();
}

void simFlush() {
    SDL_RenderPresent(ctx.renderer);
    
    // Ограничение частоты кадров
    Uint32 elapsed = SDL_GetTicks() - ctx.last_frame_time;
    if (elapsed < MS_PER_FRAME) {
        SDL_Delay(MS_PER_FRAME - elapsed);
    }

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) exit(0);
    }
    
    ctx.last_frame_time = SDL_GetTicks();
}

void simPutPixel(int x, int y, int argb) {
    Uint8 r = (argb >> 16) & 0xFF;
    Uint8 g = (argb >> 8) & 0xFF;
    Uint8 b = argb & 0xFF;
    Uint8 a = (argb >> 24) & 0xFF;

    SDL_SetRenderDrawColor(ctx.renderer, r, g, b, a);
    SDL_RenderDrawPoint(ctx.renderer, x, y);
}

int simRand() {
    return (rand() % 10) > 7;
}