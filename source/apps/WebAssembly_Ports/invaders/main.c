#include<SDL2/SDL.h>
#include<stdlib.h>
#include"sdl.h"
#include"ship.h"
#include "projectile.h"

int active = 1;
SDL_Event e;
struct Ship ship;
struct pNode *projectiles = NULL;


void render(void);
void loop(void);
void keyscan(void);

int main(int argc, char **argv) {

    if(!initSDL("[space]", 640, 360, 1280, 720)) {
        fprintf(stderr, "Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    ship_init(&ship);
    
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(loop, 0, 1);
#else
    while(active == 1) {
        loop();
    }
#endif

    pnode_free(projectiles);
    releaseSDL();
    return 0;
}

void loop(void) {

    while(SDL_PollEvent(&e)) {
        switch(e.type) {
            case SDL_QUIT:
                active = 0;
                break;
            case SDL_KEYDOWN:
                if(e.key.keysym.sym == SDLK_ESCAPE) {
                    active = 0;
                    continue;
                }
                break;
        }
    }
    keyscan();
    render();
    SDL_Delay(16);
}

void render(void) {
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    ship_draw(&ship);
    projectiles = pnode_display(projectiles);
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

}

void keyscan(void) {
    const Uint8 *keys = SDL_GetKeyboardState(0);
    if (keys[SDL_SCANCODE_LEFT]) {
        ship_left(&ship);
    }
    if(keys[SDL_SCANCODE_RIGHT]) {
        ship_right(&ship);
    }
    if(keys[SDL_SCANCODE_SPACE]) {
        projectiles = pnode_add(projectiles, ship.x+(ship.w/2), ship.y-ship.h, 0);
    }
}