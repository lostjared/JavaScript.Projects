#include<SDL2/SDL.h>
#include<stdlib.h>
#include<math.h>
#include"sdl.h"
#include"ship.h"
#include"projectile.h"
#include"alien.h"
#ifdef __EMSCRIPTEN__
#include<emscripten.h>
#endif
int active = 1;
SDL_Event e;
struct Ship ship;
struct pNode *projectiles = NULL;
struct Alien *aliens = NULL;
int alien_direction = 1; 
int alien_move_timer = 0;
int alien_move_speed = 45; 
int explosion_active = 0;
int explosion_x = 0;
int explosion_y = 0;
int explosion_timer = 0;
int explosion_duration = 30; 
int score = 0;
int lives = 3;


int game_over = 0;

int projectile_cooldown = 0;
int projectile_cooldown_max = 12; 
int max_projectiles = 1; 

void render(void);
void loop(void);
void keyscan(void);
void update_aliens(void);
void check_collisions(void);
void check_ship_collision(void);
void draw_explosion(int x, int y, int frame);
int count_living_aliens(void);
void reset_game(void);
void draw_game_over(void);
void reset_alien_positions(void); 
void check_alien_invasion(void); 

int main(int argc, char **argv) {
    if(!initSDL("[space]", 640, 360, 1280, 720)) {
        fprintf(stderr, "Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    ship_init(&ship);
    aliens = alien_create_grid(13, 6); 
    
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(loop, 0, 1);
#else
    while(active == 1) {
        loop();
    }
#endif

    alien_free(aliens);
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
                if(e.key.keysym.sym == SDLK_r) {
                    reset_game();
                    continue;
                }
                
                if(e.key.keysym.sym == SDLK_RETURN && game_over) {
                    reset_game();
                    continue;
                }
                break;
        }
    }
    
    
    if (game_over) {
        render();
        SDL_Delay(16);
        return;
    }
    
    if (projectile_cooldown > 0) {
        projectile_cooldown--;
    }
    
    if (!game_over) {
        keyscan();
        update_aliens();
        check_collisions();
        check_ship_collision();
        check_alien_invasion(); 
    }
    
    if (explosion_active) {
        if (explosion_timer >= explosion_duration) {
            explosion_active = 0;
            explosion_timer = 0;
        }
    }
    
    
    if (!explosion_active && count_living_aliens() == 0) {
        alien_free(aliens);
        aliens = alien_create_grid(13, 6); 
        if (alien_move_speed > 10) {
            alien_move_speed -= 8; 
        }
    }
    
    render();
    SDL_Delay(16);
}

void update_aliens(void) {
    int living_aliens = count_living_aliens();
    int base_speed = 45; 
    int total_aliens = 78; 
    
    
    alien_move_speed = base_speed - ((total_aliens - living_aliens) / 3); 
    if (alien_move_speed < 8) alien_move_speed = 8; 
    
    alien_move_timer++;
    if (alien_move_timer >= alien_move_speed) {
        alien_move_timer = 0;
        
        int hit_edge = 0;
        struct Alien *current = aliens;
        while (current != NULL) {
            if (current->alive) {
                
                if ((current->x <= 5 && alien_direction == -1) || 
                    (current->x >= 585 && alien_direction == 1)) {
                    hit_edge = 1;
                    break;
                }
            }
            current = current->next;
        }
        
        if (hit_edge) {
            alien_direction *= -1;
            aliens = alien_update_all(aliens, 0, 25); 
        } else {
            aliens = alien_update_all(aliens, alien_direction * 12, 0); 
        }
    }
}

void check_collisions(void) {
    struct pNode *current_projectile = projectiles;
    struct pNode *prev_projectile = NULL;
    
    while (current_projectile != NULL) {
        int hit = 0;
        struct Alien *current_alien = aliens;
        
        while (current_alien != NULL && !hit) {
            if (current_alien->alive) {
                if (current_projectile->x >= current_alien->x - 2 && 
                    current_projectile->x <= current_alien->x + 22 &&
                    current_projectile->y >= current_alien->y - 2 && 
                    current_projectile->y <= current_alien->y + 17) {
                    
                    current_alien->alive = 0;
                    hit = 1;
                    
                    
                    switch(current_alien->type) {
                        case 0: score += 30; break; 
                        case 1: score += 20; break; 
                        case 2: score += 10; break; 
                        case 3: score += 300; break; 
                        default: score += 10; break;
                    }
                }
            }
            current_alien = current_alien->next;
        }
        
        if (hit) {
            if (prev_projectile == NULL) {
                projectiles = current_projectile->next;
                free(current_projectile);
                current_projectile = projectiles;
            } else {
                prev_projectile->next = current_projectile->next;
                free(current_projectile);
                current_projectile = prev_projectile->next;
            }
        } else {
            prev_projectile = current_projectile;
            current_projectile = current_projectile->next;
        }
    }
}

void check_ship_collision(void) {
    struct Alien *current = aliens;
    while (current != NULL) {
        if (current->alive) {
            if (current->x < ship.x + ship.w &&
                current->x + 20 > ship.x &&
                current->y < ship.y + ship.h &&
                current->y + 15 > ship.y) {
                
                explosion_active = 1;
                explosion_x = ship.x + ship.w / 2;
                explosion_y = ship.y + ship.h / 2;
                explosion_timer = 0;
                
                current->alive = 0;
                lives--;
                
                reset_alien_positions();
                
                if(lives <= 0) {
                    game_over = 1;
                }
                
                break;
            }
        }
        current = current->next;
    }
}

void reset_alien_positions(void) {
    struct Alien *current = aliens;
    int row = 0;
    int col = 0;
    int spacing_x = 45; 
    int spacing_y = 35; 
    int cols = 13; 
    int grid_width = cols * spacing_x;
    int start_x = (640 - grid_width) / 2;
    int start_y = 40; 
    
    while (current != NULL) {
        current->x = start_x + (col * spacing_x);
        current->y = start_y + (row * spacing_y);
        
        col++;
        if (col >= cols) { 
            col = 0;
            row++;
        }
        
        current = current->next;
    }
    
    alien_direction = 1;
    alien_move_timer = 0;
}

void draw_explosion(int x, int y, int frame) {
    if (!explosion_active) return;
    
    explosion_timer++;
    
    if (explosion_timer >= explosion_duration) {
        explosion_active = 0;
        explosion_timer = 0;
        return;
    }
    
    int intensity = 255 - (explosion_timer * 255 / explosion_duration);
    SDL_SetRenderDrawColor(renderer, 255, intensity, intensity / 2, 255);
    
    int num_lines = 12;
    int radius = explosion_timer * 2; 
    
    for (int i = 0; i < num_lines; i++) {
        float angle = (i * 2.0f * 3.14159f) / num_lines;
        int end_x = x + (int)(radius * cos(angle));
        int end_y = y + (int)(radius * sin(angle));
        
        SDL_RenderDrawLine(renderer, x, y, end_x, end_y);
        
        if (explosion_timer % 3 == 0) {
            int spark_x = x + (rand() % 40) - 20;
            int spark_y = y + (rand() % 40) - 20;
            SDL_RenderDrawPoint(renderer, spark_x, spark_y);
        }
    }
    
    int circle_points = 16;
    for (int i = 0; i < circle_points; i++) {
        float angle1 = (i * 2.0f * 3.14159f) / circle_points;
        float angle2 = ((i + 1) * 2.0f * 3.14159f) / circle_points;
        
        int x1 = x + (int)(radius * cos(angle1));
        int y1 = y + (int)(radius * sin(angle1));
        int x2 = x + (int)(radius * cos(angle2));
        int y2 = y + (int)(radius * sin(angle2));
        
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }
}

void draw_game_over(void) {
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
    SDL_Rect overlay = {0, 0, 640, 360};
    SDL_RenderFillRect(renderer, &overlay);
    
    
    settextcolor(255, 0, 0, 255);
    printtext("GAME OVER", 280, 150);
    
    settextcolor(255, 255, 255, 255); 
    char final_score[64];
    snprintf(final_score, 64, "Final Score: %d", score);
    printtext(final_score, 260, 180);
    
    settextcolor(255, 255, 0, 255); 
    printtext("Press ENTER to continue", 200, 210);
}

int count_living_aliens(void) {
    int count = 0;
    struct Alien *current = aliens;
    while (current != NULL) {
        if (current->alive) {
            count++;
        }
        current = current->next;
    }
    return count;
}


int count_projectiles(void) {
    int count = 0;
    struct pNode *current = projectiles;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

void reset_game(void) {
    ship_init(&ship);
    
    if (projectiles != NULL) {
        pnode_free(projectiles);
        projectiles = NULL;
    }
    
    if (aliens != NULL) {
        alien_free(aliens);
    }
    aliens = alien_create_grid(13, 6); 
    
    explosion_active = 0;
    explosion_timer = 0;
    alien_direction = 1;
    alien_move_timer = 0;
    alien_move_speed = 45; 
    projectile_cooldown = 0;
    
    game_over = 0;
    score = 0;
    lives = 3;
}


void check_alien_invasion(void) {
    struct Alien *current = aliens;
    while (current != NULL) {
        if (current->alive && current->y >= ship.y - 30) { 
    
            explosion_active = 1;
            explosion_x = ship.x + ship.w / 2;
            explosion_y = ship.y + ship.h / 2;
            explosion_timer = 0;
            
            lives--;
            
    
            reset_alien_positions();
            
    
            if(lives <= 0) {
                game_over = 1;
            }
            
            break; 
        }
        current = current->next;
    }
}

void render(void) {
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    if (!explosion_active && !game_over) {
        ship_draw(&ship);
    }
    
    projectiles = pnode_display(projectiles);
    aliens = alien_display_all(aliens);
    
    if (explosion_active) {
        draw_explosion(explosion_x, explosion_y, explosion_timer);
    }
    
    
    char score_buf[1024];
    snprintf(score_buf, 1024, "Score: %d", score);
    settextcolor(255,255,255,255);
    printtext(score_buf, 15, 15);
    snprintf(score_buf,1024, "Lives: %d", lives);
    printtext(score_buf, 150, 15);
    
    
    if (game_over) {
        draw_game_over();
    }
    
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void keyscan(void) {
    const Uint8 *keys = SDL_GetKeyboardState(0); 
    
    if (!explosion_active && !game_over) {
        if (keys[SDL_SCANCODE_LEFT]) {
            ship_left(&ship);
        }
        if(keys[SDL_SCANCODE_RIGHT]) {
            ship_right(&ship);
        }
        
        if(keys[SDL_SCANCODE_SPACE] && projectile_cooldown == 0 && count_projectiles() < max_projectiles) {
            projectiles = pnode_add(projectiles, ship.x+(ship.w/2), ship.y-ship.h, 0);
            projectile_cooldown = projectile_cooldown_max; 
        }
    }
}