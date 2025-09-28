#ifndef POOL_H
#define POOL_H
#include <stdbool.h>
#include"sdl.h"
#define WINDOW_W 640
#define WINDOW_H 360
extern int MAX_BUMPERS;
struct Stick {
    float x, y;           
    float angle;          
    float length;         
    float charge;         
    bool charging;        
    bool visible;         
};
struct Ball {
    float x, y;           
    float vx, vy;         
    float radius;         
    bool moving;          
    bool just_stopped;    
};
struct Target {
    float x, y;           
    float width, height;  
    bool ball_in_target;  
};
struct Bumper {
    float x, y;           
    float radius;         
};
extern int shot_count;
extern bool game_won;
extern bool showing_win_screen;
extern SDL_Texture *ball_tex, *bumber, *target_bmp;
void init_stick(struct Stick *stick);
void init_ball(struct Ball *ball);
void init_target(struct Bumper *bumpers, struct Target *target);  
void init_bumpers(struct Bumper **bumpers);  
void update_stick(struct Stick *stick);
void update_ball(struct Ball *ball);
void check_ball_target(struct Ball *ball, struct Target *target);
void draw_stick(struct Stick *stick);
void draw_ball(struct Ball *ball);
void draw_target(struct Target *target);
void draw_bumpers(struct Bumper *bumpers);
void draw_ui(struct Stick *stick, struct Target *target);
void draw_win_screen(void);
void hit_ball(struct Stick *stick, struct Ball *ball);
void check_ball_bumper_collision(struct Ball *ball, struct Bumper *bumpers);
void realign_stick_with_ball(struct Stick *stick, struct Ball *ball);
void check_win_condition(struct Ball *ball, struct Target *target);
void ensure_bumpers_avoid_target(struct Bumper *bumpers, struct Target *target);
void reset_game(struct Stick *stick, struct Ball *ball, struct Target *target);
#endif