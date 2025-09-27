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

SDL_Texture *table;

SDL_Event e;
int active = 1;
struct Bumper *bumpers_arr = NULL;
SDL_Texture *bumber = NULL;
SDL_Texture *target_bmp = NULL;


void update(void);
void render(void);
void load(void);
void cleanup(void);

void reset_game(struct Stick *stick, struct Ball *ball, struct Target *target) {
    shot_count = 0;
    game_won = false;
    showing_win_screen = false;
    
    init_stick(stick);
    init_ball(ball);
    init_bumpers(&bumpers_arr);
    init_target(bumpers_arr, target);    
    ensure_bumpers_avoid_target(bumpers_arr, target); 
}

void load(void) {

    SDL_Surface *surface = SDL_LoadBMP(getPath("table.bmp"));
    if(!surface) {
        fprintf(stderr, "Error could not load surface: %s\n", SDL_GetError());
        fprintf(stderr, "Aborting..\n");
        exit(1);
    }
    table = SDL_CreateTextureFromSurface(renderer, surface);
    if(!table) {
        fprintf(stderr, "Error converting surface: %s\n", SDL_GetError());
        fprintf(stderr, "Aborting... ");
        exit(1);
    }
    SDL_FreeSurface(surface);
    SDL_Surface *bsurf = SDL_LoadBMP(getPath("ball.bmp"));
    if(!bsurf) {
        fprintf(stderr, "Error could not load surface: %s %s\n", "ball.bmp", SDL_GetError());
        fprintf(stderr, "Aborting..\n");
        exit(1);
    }
    SDL_SetColorKey(bsurf, SDL_TRUE, SDL_MapRGB(bsurf->format, 0, 0, 0));
    ball_tex = SDL_CreateTextureFromSurface(renderer, bsurf);
    SDL_FreeSurface(bsurf);
    if(!ball_tex) {
        fprintf(stderr, "Error converting surface: %s\n", SDL_GetError());
        fprintf(stderr, "Aborting..\n");
        exit(1);
    }
    SDL_Surface *csurf = SDL_LoadBMP(getPath("bumber.bmp"));
    if(!csurf) {
        fprintf(stderr, "Error could not load surface: %s %s\n", "bumer.bmp", SDL_GetError());
        fprintf(stderr, "Aborting..\n");
        exit(1);
    }
    SDL_SetColorKey(csurf, SDL_TRUE, SDL_MapRGB(csurf->format, 0, 0, 0));
    bumber = SDL_CreateTextureFromSurface(renderer, csurf);
    SDL_FreeSurface(csurf);
    if(!bumber) {
        fprintf(stderr, "Error converting surface: %s\n", SDL_GetError());
        fprintf(stderr, "Aborting..\n");
        exit(1);
    }
    SDL_Surface *tsurf = SDL_LoadBMP(getPath("target.bmp"));
    if(!tsurf) {
        fprintf(stderr, "Error could not load surface: %s %s\n", "target.bmp", SDL_GetError());
        fprintf(stderr, "Aborting..\n");
        exit(1);
    }
    target_bmp = SDL_CreateTextureFromSurface(renderer, tsurf);
    SDL_FreeSurface(tsurf);
    if(!target_bmp) {
        fprintf(stderr, "Error converting surface: %s\n", SDL_GetError());
        fprintf(stderr, "Aborting..\n");
        exit(1);
    }
}

void cleanup(void) {
    if(table != NULL)
        SDL_DestroyTexture(table);
    if(ball_tex != NULL) 
        SDL_DestroyTexture(ball_tex);
    if(target_bmp != NULL)
        SDL_DestroyTexture(target_bmp);
}

int main(int argc, char **argv) {
    srand(time(NULL));
    if (!initSDL("Pool Game", WINDOW_W, WINDOW_H, WINDOW_SX, WINDOW_SY)) {
        return 1;
    }
    load();
    init_stick(&stick);
    init_ball(&ball);
    init_bumpers(&bumpers_arr); 
    init_target(bumpers_arr, &target);
    ensure_bumpers_avoid_target(bumpers_arr, &target); 
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(update, 0, 1);
#else
    while (active) {
        update();
    }
#endif
    if(bumpers_arr != NULL)
        free(bumpers_arr);
    cleanup();
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
    check_ball_bumper_collision(&ball, bumpers_arr); 
    
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
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, texture);
    if (showing_win_screen) {
        draw_win_screen();
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255); 
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, table, NULL, NULL);
        draw_bumpers(bumpers_arr);
        draw_target(&target);
        draw_ball(&ball);
        draw_stick(&stick);
        draw_ui(&stick, &target);
    }
    
    SDL_SetRenderTarget(renderer, NULL);
    int window_w, window_h;
    SDL_GetRendererOutputSize(renderer, &window_w, &window_h);
    int tex_w, tex_h;
    SDL_QueryTexture(texture, NULL, NULL, &tex_w, &tex_h);
    float window_aspect = (float)window_w / (float)window_h;
    float texture_aspect = (float)tex_w / (float)tex_h;
    SDL_Rect dst;
    if (texture_aspect > window_aspect) {
        dst.w = window_w;
        dst.h = (int)(window_w / texture_aspect);
        dst.x = 0;
        dst.y = (window_h - dst.h) / 2;
    } else {
        dst.h = window_h;
        dst.w = (int)(window_h * texture_aspect);
        dst.y = 0;
        dst.x = (window_w - dst.w) / 2;
    }
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_RenderPresent(renderer);
    const Uint32 target_frame_time = 1000 / 60; 
    Uint32 frame_time = current_time - last_time;
    if (frame_time < target_frame_time) {
        SDL_Delay(target_frame_time - frame_time);
    }
    last_time = SDL_GetTicks();
}