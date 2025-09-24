#include "pool.h"
#include "sdl.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


int shot_count = 0;
bool game_won = false;
bool showing_win_screen = false;

struct Bumper bumpers[MAX_BUMPERS];

void init_stick(struct Stick *stick) {
    memset(stick, 0, sizeof(struct Stick));
    stick->x = WINDOW_W / 2;
    stick->y = WINDOW_H / 2;
    stick->angle = 0.0f;
    stick->length = 80.0f;
    stick->charge = 0.0f;
    stick->charging = false;
    stick->visible = true;
}

void init_ball(struct Ball *ball) {
    memset(ball, 0, sizeof(struct Ball));
    ball->x = WINDOW_W / 2;
    ball->y = WINDOW_H / 2;
    ball->vx = 0.0f;
    ball->vy = 0.0f;
    ball->radius = 10.0f;
    ball->moving = false;
}

void init_target(struct Target *target) {
    memset(target, 0, sizeof(struct Target));
    target->width = 75;
    target->height = 75;
    target->ball_in_target = false;
    
    bool valid_position = false;
    int attempts = 0;
    
    while (!valid_position && attempts < 50) {
        target->x = 20 + rand() % (WINDOW_W - (int)target->width - 40);
        target->y = 20 + rand() % (WINDOW_H - (int)target->height - 40);
        valid_position = true;
        float center_x = WINDOW_W / 2;
        float center_y = WINDOW_H / 2;
        float target_center_x = target->x + target->width / 2;
        float target_center_y = target->y + target->height / 2;
        
        float dist_from_center = sqrtf((target_center_x - center_x) * (target_center_x - center_x) + 
                                      (target_center_y - center_y) * (target_center_y - center_y));
        
        if (dist_from_center < 120.0f) {
            valid_position = false;
            attempts++;
            continue;
        }
        
        for (int i = 0; i < MAX_BUMPERS; i++) {
            float bumper_dist_x = fabs(target_center_x - bumpers[i].x);
            float bumper_dist_y = fabs(target_center_y - bumpers[i].y);
            
            if (bumper_dist_x < (target->width/2 + bumpers[i].radius + 20) &&
                bumper_dist_y < (target->height/2 + bumpers[i].radius + 20)) {
                valid_position = false;
                break;
            }
        }
        
        attempts++;
    }
    
    if (!valid_position) {
        int corner = rand() % 4;
        switch (corner) {
            case 0: 
                target->x = 20;
                target->y = 20;
                break;
            case 1: 
                target->x = WINDOW_W - target->width - 20;
                target->y = 20;
                break;
            case 2: 
                target->x = 20;
                target->y = WINDOW_H - target->height - 20;
                break;
            case 3: 
                target->x = WINDOW_W - target->width - 20;
                target->y = WINDOW_H - target->height - 20;
                break;
        }
    }
}

void init_bumpers(struct Bumper *bumpers) {
    for (int i = 0; i < MAX_BUMPERS; i++) {
        bool valid_position = false;
        int attempts = 0;
        
        while (!valid_position && attempts < 50) {
            bumpers[i].x = 60 + rand() % (WINDOW_W - 120);
            bumpers[i].y = 60 + rand() % (WINDOW_H - 120);
            bumpers[i].radius = 15.0f;
            
            valid_position = true;
            
            float center_x = WINDOW_W / 2;
            float center_y = WINDOW_H / 2;
            float dist_from_center = sqrtf((bumpers[i].x - center_x) * (bumpers[i].x - center_x) + 
                                          (bumpers[i].y - center_y) * (bumpers[i].y - center_y));
            
            if (dist_from_center < 80.0f) {
                valid_position = false;
                attempts++;
                continue;
            }
            
            // Check distance from other bumpers
            for (int j = 0; j < i; j++) {
                float dist = sqrtf((bumpers[i].x - bumpers[j].x) * (bumpers[i].x - bumpers[j].x) + 
                                  (bumpers[i].y - bumpers[j].y) * (bumpers[i].y - bumpers[j].y));
                if (dist < 60.0f) { 
                    valid_position = false;
                    break;
                }
            }
            
            attempts++;
        }
        
        if (!valid_position) {
            bumpers[i].x = 100 + (i * 150);
            bumpers[i].y = 100 + ((i % 2) * 200);
            bumpers[i].radius = 15.0f;
        }
    }
}
void ensure_bumpers_avoid_target(struct Bumper *bumpers, struct Target *target) {
    for (int i = 0; i < MAX_BUMPERS; i++) {
        bool needs_relocation = false;
        

        float buffer = 30.0f; 
        
        if (bumpers[i].x >= target->x - buffer && 
            bumpers[i].x <= target->x + target->width + buffer &&
            bumpers[i].y >= target->y - buffer && 
            bumpers[i].y <= target->y + target->height + buffer) {
            needs_relocation = true;
        }
        
        if (needs_relocation) {

            bool found_new_position = false;
            int attempts = 0;
            
            while (!found_new_position && attempts < 30) {
                float new_x = 60 + rand() % (WINDOW_W - 120);
                float new_y = 60 + rand() % (WINDOW_H - 120);
                
            
                float center_x = WINDOW_W / 2;
                float center_y = WINDOW_H / 2;
                float dist_from_center = sqrtf((new_x - center_x) * (new_x - center_x) + 
                                              (new_y - center_y) * (new_y - center_y));
                
                if (dist_from_center < 80.0f) {
                    attempts++;
                    continue;
                }
                
            
                if (new_x >= target->x - buffer && 
                    new_x <= target->x + target->width + buffer &&
                    new_y >= target->y - buffer && 
                    new_y <= target->y + target->height + buffer) {
                    attempts++;
                    continue;
                }
                
            
                bool too_close_to_other = false;
                for (int j = 0; j < MAX_BUMPERS; j++) {
                    if (i != j) {
                        float dist = sqrtf((new_x - bumpers[j].x) * (new_x - bumpers[j].x) + 
                                          (new_y - bumpers[j].y) * (new_y - bumpers[j].y));
                        if (dist < 60.0f) {
                            too_close_to_other = true;
                            break;
                        }
                    }
                }
                
                if (!too_close_to_other) {
                    bumpers[i].x = new_x;
                    bumpers[i].y = new_y;
                    found_new_position = true;
                } else {
                    attempts++;
                }
            }
            
            if (!found_new_position) {
                float corners[4][2] = {
                    {100, 100},                          
                    {WINDOW_W - 100, 100},               
                    {100, WINDOW_H - 100},               
                    {WINDOW_W - 100, WINDOW_H - 100}     
                };
                
                
                float max_dist = 0;
                int best_corner = 0;
                for (int c = 0; c < 4; c++) {
                    float target_center_x = target->x + target->width / 2;
                    float target_center_y = target->y + target->height / 2;
                    float dist = sqrtf((corners[c][0] - target_center_x) * (corners[c][0] - target_center_x) +
                                      (corners[c][1] - target_center_y) * (corners[c][1] - target_center_y));
                    if (dist > max_dist) {
                        max_dist = dist;
                        best_corner = c;
                    }
                }
                
                bumpers[i].x = corners[best_corner][0] + (i * 20); 
                bumpers[i].y = corners[best_corner][1] + ((i % 2) * 20);
            }
        }
    }
}

void update_stick(struct Stick *stick) {
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if (keys[SDL_SCANCODE_LEFT]) {
        stick->angle -= 0.05f;
    }
    if (keys[SDL_SCANCODE_RIGHT]) {
        stick->angle += 0.05f;
    }
    
    if (keys[SDL_SCANCODE_SPACE]) {
        if (!stick->charging) {
            stick->charging = true;
            stick->charge = 0.0f;
        }
        
        if (stick->charge < 100.0f) {
            stick->charge += 2.0f; 
        }
    }
}

void update_ball(struct Ball *ball) {
    if (!ball->moving) return;
    
    
    ball->just_stopped = false;
    
    
    ball->x += ball->vx;
    ball->y += ball->vy;
    
    
    if (ball->x - ball->radius <= 0 || ball->x + ball->radius >= WINDOW_W) {
        ball->vx = -ball->vx * 0.8f;
        ball->x = (ball->x - ball->radius <= 0) ? ball->radius : WINDOW_W - ball->radius;
    }
    
    if (ball->y - ball->radius <= 0 || ball->y + ball->radius >= WINDOW_H) {
        ball->vy = -ball->vy * 0.8f;
        ball->y = (ball->y - ball->radius <= 0) ? ball->radius : WINDOW_H - ball->radius;
    }
    
    
    ball->vx *= 0.995f;
    ball->vy *= 0.995f;
    
    
    if (fabs(ball->vx) < 0.1f && fabs(ball->vy) < 0.1f) {
        ball->vx = 0.0f;
        ball->vy = 0.0f;
        ball->moving = false;
        ball->just_stopped = true; 
    }
}

void check_ball_target(struct Ball *ball, struct Target *target) {
    target->ball_in_target = (ball->x >= target->x && 
                             ball->x <= target->x + target->width &&
                             ball->y >= target->y && 
                             ball->y <= target->y + target->height);
}

void hit_ball(struct Stick *stick, struct Ball *ball) {
    if (!stick->charging) return;
    
    float power = stick->charge / 100.0f * 15.0f; 
    
    ball->vx = cosf(stick->angle) * power;
    ball->vy = sinf(stick->angle) * power;
    ball->moving = true;
    
    
    shot_count++;
    
    stick->charging = false;
    stick->charge = 0.0f;
    stick->visible = true;
}

void draw_stick(struct Stick *stick) {
    if (!stick->visible) return;
    
    
    float stick_offset = stick->charging ? (stick->charge / 100.0f * 30.0f) : 0.0f;
    float start_x = stick->x - cosf(stick->angle) * (stick->length + stick_offset);
    float start_y = stick->y - sinf(stick->angle) * (stick->length + stick_offset);
    float end_x = stick->x - cosf(stick->angle) * stick_offset;
    float end_y = stick->y - sinf(stick->angle) * stick_offset;
    
    
    if (stick->charging) {
        int red = 255;
        int green = 255 - (int)(stick->charge * 2.55f);
        int blue = 255 - (int)(stick->charge * 2.55f);
        SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
    } else {
        SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); 
    }
    
    
    for (int i = -2; i <= 2; i++) {
        for (int j = -2; j <= 2; j++) {
            SDL_RenderDrawLine(renderer, 
                             (int)(start_x + i), (int)(start_y + j),
                             (int)(end_x + i), (int)(end_y + j));
        }
    }
    
    
    if (stick->charging) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 128);
        float aim_end_x = stick->x + cosf(stick->angle) * 100.0f;
        float aim_end_y = stick->y + sinf(stick->angle) * 100.0f;
        SDL_RenderDrawLine(renderer, (int)stick->x, (int)stick->y, 
                          (int)aim_end_x, (int)aim_end_y);
    }
}

void draw_ball(struct Ball *ball) {
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    for (int y = -ball->radius; y <= ball->radius; y++) {
        for (int x = -ball->radius; x <= ball->radius; x++) {
            if (x*x + y*y <= ball->radius * ball->radius) {
                SDL_RenderDrawPoint(renderer, (int)(ball->x + x), (int)(ball->y + y));
            }
        }
    }
}

void draw_target(struct Target *target) {
    
    SDL_Rect rect = {(int)target->x, (int)target->y, (int)target->width, (int)target->height};
    
    if (target->ball_in_target) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 100); 
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 50); 
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    }
    
    SDL_RenderDrawRect(renderer, &rect);
}

void draw_bumpers(struct Bumper *bumpers) {
    for (int i = 0; i < MAX_BUMPERS; i++) {
        SDL_SetRenderDrawColor(renderer, 0, 100, 255, 255);
        
        for (int y = -bumpers[i].radius; y <= bumpers[i].radius; y++) {
            for (int x = -bumpers[i].radius; x <= bumpers[i].radius; x++) {
                if (x*x + y*y <= bumpers[i].radius * bumpers[i].radius) {
                    SDL_RenderDrawPoint(renderer, (int)(bumpers[i].x + x), (int)(bumpers[i].y + y));
                }
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 0, 50, 200, 255);
        for (int angle = 0; angle < 360; angle += 5) {
            float rad = angle * M_PI / 180.0f;
            int edge_x = (int)(bumpers[i].x + cosf(rad) * bumpers[i].radius);
            int edge_y = (int)(bumpers[i].y + sinf(rad) * bumpers[i].radius);
            SDL_RenderDrawPoint(renderer, edge_x, edge_y);
            SDL_RenderDrawPoint(renderer, edge_x + 1, edge_y);
            SDL_RenderDrawPoint(renderer, edge_x, edge_y + 1);
        }
    }
}

void check_ball_bumper_collision(struct Ball *ball, struct Bumper *bumpers) {
    if (!ball->moving) return;
    
    for (int i = 0; i < MAX_BUMPERS; i++) {
        float dx = ball->x - bumpers[i].x;
        float dy = ball->y - bumpers[i].y;
        float distance = sqrtf(dx * dx + dy * dy);
        
        if (distance < ball->radius + bumpers[i].radius) {
            float nx = dx / distance;
            float ny = dy / distance;
            
            float overlap = ball->radius + bumpers[i].radius - distance;
            ball->x += nx * overlap;
            ball->y += ny * overlap;
            
            float dot_product = ball->vx * nx + ball->vy * ny;
            
            ball->vx = ball->vx - 2.0f * dot_product * nx;
            ball->vy = ball->vy - 2.0f * dot_product * ny;
            
            ball->vx *= 1.1f;
            ball->vy *= 1.1f;
            
            float speed = sqrtf(ball->vx * ball->vx + ball->vy * ball->vy);
            if (speed > 20.0f) {
                ball->vx = (ball->vx / speed) * 20.0f;
                ball->vy = (ball->vy / speed) * 20.0f;
            }
        }
    }
}

void draw_ui(struct Stick *stick, struct Target *target) {
    if (stick->charging) {
        SDL_Rect power_bg = {10, 10, 200, 20};
        SDL_Rect power_fill = {10, 10, (int)(stick->charge * 2), 20};
        
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderFillRect(renderer, &power_bg);
        
        SDL_SetRenderDrawColor(renderer, 255, 255 - (int)(stick->charge * 2.55f), 0, 255);
        SDL_RenderFillRect(renderer, &power_fill);
        
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &power_bg);
    }
    
 
    settextcolor(255, 255, 255, 255);
    printtext("Arrow Keys: Aim", 10, WINDOW_H - 60);
    printtext("Space: Hold to charge, Release to shoot", 10, WINDOW_H - 40);
    
    if (target->ball_in_target) {
        settextcolor(0, 255, 0, 255);
        printtext("TARGET HIT!", WINDOW_W/2 - 50, 50);
    } else {
        settextcolor(255, 255, 255, 255);
        printtext("Get ball in red target!", 10, WINDOW_H - 20);
    }
    settextcolor(255, 255, 255, 255);
    char sbuf[50];
    snprintf(sbuf, 49, "Tries: %d", shot_count);
    printtext(sbuf, 25, 35);
}

void realign_stick_with_ball(struct Stick *stick, struct Ball *ball) {
    stick->x = ball->x;
    stick->y = ball->y;
    stick->charge = 0.0f;
    stick->charging = false;
    stick->visible = true;
}

void check_win_condition(struct Ball *ball, struct Target *target) {
    if (target->ball_in_target && !ball->moving && !showing_win_screen) {
        game_won = true;
        showing_win_screen = true;
    }
}

void draw_win_screen(void) {
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    
    
    settextcolor(0, 255, 0, 255);
    printtext("CONGRATULATIONS!", WINDOW_W/2 - 80, WINDOW_H/2 - 60);
    
    settextcolor(255, 255, 255, 255);
    printtext("You got the ball in the target!", WINDOW_W/2 - 120, WINDOW_H/2 - 20);
    
    
    char shot_text[50];
    snprintf(shot_text, 50, "Shots taken: %d", shot_count);
    settextcolor(255, 255, 0, 255);
    printtext(shot_text, WINDOW_W/2 - 60, WINDOW_H/2 + 20);
    
    
    settextcolor(200, 200, 200, 255);
    printtext("Press R to play again", WINDOW_W/2 - 80, WINDOW_H/2 + 60);
    printtext("Press ESC to quit", WINDOW_W/2 - 70, WINDOW_H/2 + 80);
}
