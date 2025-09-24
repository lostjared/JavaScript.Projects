#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "pool.h"
#include "sdl.h"
#include<stdlib.h>
#include<time.h>

#define WINDOW_W 640
#define WINDOW_H 360
#define WINDOW_SX 1280
#define WINDOW_SY 720

struct Stick stick;
struct Ball ball;
struct Target target;

SDL_Event e;
int active = 1;

void update(void);
void render(void);

void reset_game(struct Stick *stick, struct Ball *ball, struct Target *target) {
    shot_count = 0;
    game_won = false;
    showing_win_screen = false;
    
    init_stick(stick);
    init_ball(ball);
    init_bumpers(bumpers);  
    init_target(target);    
    ensure_bumpers_avoid_target(bumpers, target); 
}

int main(int argc, char **argv) {
    srand(time(NULL));
    if (!initSDL("Pool Game", WINDOW_W, WINDOW_H, WINDOW_SX, WINDOW_SY)) {
        return 1;
    }
    init_stick(&stick);
    init_ball(&ball);
    init_target(&target);
    init_bumpers(bumpers); 
    ensure_bumpers_avoid_target(bumpers, &target); 
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(update, 0, 1);
#else
    while (active) {
        update();
    }
#endif
    releaseSDL();
    return 0;
}

void update(void) {
    static bool space_was_pressed = false;
    
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            active = 0;
            return;
        }
        
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
            active = 0;
            return;
        }
        
        if (showing_win_screen && e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_r) {
            reset_game(&stick, &ball, &target);
            return;
        }
        
        if (showing_win_screen) {
            render();
            return;
        }
        
        if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_SPACE) {
            if (space_was_pressed && stick.charging) {
                hit_ball(&stick, &ball);
            }
            space_was_pressed = false;
        }
        
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
            space_was_pressed = true;
        }
    }
    
    if (showing_win_screen) {
        render();
        return;
    }
    
    update_stick(&stick);
    update_ball(&ball);
    check_ball_bumper_collision(&ball, bumpers); 
    
    if (ball.just_stopped) {
        realign_stick_with_ball(&stick, &ball);
        ball.just_stopped = false;
    }
    
    check_ball_target(&ball, &target);
    check_win_condition(&ball, &target);
    
    render();
}

void render(void) {
    static Uint32 last_time = 0;
    Uint32 current_time = SDL_GetTicks();

    SDL_SetRenderTarget(renderer, texture);
    
    if (showing_win_screen) {
        draw_win_screen();
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255); 
        SDL_RenderClear(renderer);

        draw_bumpers(bumpers);
        draw_target(&target);
        draw_ball(&ball);
        draw_stick(&stick);
        draw_ui(&stick, &target);
    }
    
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    
    const Uint32 target_frame_time = 1000 / 60; 
    Uint32 frame_time = current_time - last_time;
    if (frame_time < target_frame_time) {
        SDL_Delay(target_frame_time - frame_time);
    }
    last_time = SDL_GetTicks();
}