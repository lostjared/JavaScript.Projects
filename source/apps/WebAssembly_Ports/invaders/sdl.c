#include "sdl.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;

int initSDL(const char *app, int w, int h, int sx, int sy) {

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Error could not open SDL: %s", SDL_GetError());
        return 0;
    }

    window = SDL_CreateWindow(app, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, sx, sy, SDL_WINDOW_SHOWN);
    if(!window) {
        fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer) {
        fprintf(stderr, "Error render device failed to init: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
    }
    
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, w, h);
    if (!texture) {
        fprintf(stderr, "Error creating texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
    }

    return 1;
}

void releaseSDL(void) {
    if (texture) SDL_DestroyTexture(texture);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}
