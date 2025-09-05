#ifdef __EMSCRIPTEN__
#include<emscripten.h>
#endif

#include"sdl.h"
#include"ship.h"

#define WINDOW_W 640
#define WINDOW_H 360
#define WINDOW_SX 1280
#define WINDOW_SY 720
#define STAR_COUNT 200
int initialized = 0;
struct { float x, y, speed; } stars[STAR_COUNT];



void update(void);
void render(void);

SDL_Event e;
int active = 1;

int main(int argc, char **argv) {

    if(!initSDL("[space]", WINDOW_W, WINDOW_H, WINDOW_SX, WINDOW_SY)) {
        fprintf(stderr, "Failed to init..\n");
        return 1;
    }

    init_ship();
    init_projectiles();
    init_asteroids();
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(update, 0, 1);
#else
    while(active) {
        update();
    }
#endif
    releaseSDL();
    return 0;
}   


void update(void) {
    while(SDL_PollEvent(&e)) {
            switch(e.type) {
                case SDL_QUIT:
                    active = 0;
                continue;
                case SDL_KEYDOWN:
                    if(e.key.keysym.sym == SDLK_ESCAPE) 
                        active = 0;      
                    if(e.key.keysym.sym == SDLK_LEFT)
                        keyLeft = true;
                    if(e.key.keysym.sym == SDLK_RIGHT)
                        keyRight = true;
                    if(e.key.keysym.sym == SDLK_UP)
                        keyThrust = true;
                    if(e.key.keysym.sym == SDLK_SPACE)
                        fire_projectile(the_ship.x,the_ship.y, the_ship.angle);
                break;
                case SDL_KEYUP:
                    if(e.key.keysym.sym == SDLK_LEFT)
                        keyLeft = false;
                    if(e.key.keysym.sym == SDLK_RIGHT)
                        keyRight = false;
                    if(e.key.keysym.sym == SDLK_UP)
                        keyThrust = false;
                break;                          
            }
        }
        render();
}

void render(void) {
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    if (!initialized) {
        for (int i = 0; i < STAR_COUNT; ++i) {
            stars[i].x = rand() % WINDOW_W;
            stars[i].y = rand() % WINDOW_H;
            stars[i].speed = 0.5f + (rand() % 100) / 50.0f; 
        }
        initialized = 1;
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i < STAR_COUNT; ++i) {
        stars[i].y += stars[i].speed;
        if (stars[i].y >= WINDOW_H) {
            stars[i].x = rand() % WINDOW_W;
            stars[i].y = 0;
            stars[i].speed = 0.5f + (rand() % 100) / 50.0f;
        }
        SDL_RenderDrawPoint(renderer, (int)stars[i].x, (int)stars[i].y);
    }

    draw_ship();
    draw_projectiles();
    draw_asteroids();
    SDL_SetRenderTarget(renderer, NULL);
    SDL_Rect destRect = {0, 0, WINDOW_SX, WINDOW_SY};
    SDL_RenderCopy(renderer, texture, NULL, &destRect);
    SDL_RenderPresent(renderer);
    update_ship();
    update_projectiles();
    update_asteroids();
    check_and_spawn_asteroids();
    check_asteroid_collisions();
    check_projectile_asteroid_collisions(); 

    SDL_Delay(16);
}