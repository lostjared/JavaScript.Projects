#include<SDL2/SDL.h>
#include<stdlib.h>
#include<math.h>
#include"sdl.h"
#include"ship.h"
#include"projectile.h"
#include"alien.h"

int active = 1;
SDL_Event e;
struct Ship ship;
struct pNode *projectiles = NULL;
struct Alien *aliens = NULL;
int alien_direction = 1; 
int alien_move_timer = 0;
int alien_move_speed = 30;
int explosion_active = 0;
int explosion_x = 0;
int explosion_y = 0;
int explosion_timer = 0;
int explosion_duration = 60; 


int projectile_cooldown = 0;
int projectile_cooldown_max = 5; 

void render(void);
void loop(void);
void keyscan(void);
void update_aliens(void);
void check_collisions(void);
void check_ship_collision(void);
void draw_explosion(int x, int y, int frame);
int count_living_aliens(void);
void reset_game(void);

int main(int argc, char **argv) {
    if(!initSDL("[space]", 640, 360, 1280, 720)) {
        fprintf(stderr, "Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    ship_init(&ship);
    aliens = alien_create_grid(19, 10);
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
                break;
        }
    }
    
    if (projectile_cooldown > 0) {
        projectile_cooldown--;
    }
    
    keyscan();
    update_aliens();
    check_collisions();
    check_ship_collision();
    
    if (explosion_active) {
        if (explosion_timer >= explosion_duration) {
            reset_game();
        }
    }
    
    if (!explosion_active && count_living_aliens() == 0) {
        alien_free(aliens);
        aliens = alien_create_grid(19, 10);
        if (alien_move_speed > 15) {
            alien_move_speed -= 2;
        }
    }
    
    render();
    SDL_Delay(16);
}

void update_aliens(void) {
    alien_move_timer++;
    if (alien_move_timer >= alien_move_speed) {
        alien_move_timer = 0;
        
        int hit_edge = 0;
        struct Alien *current = aliens;
        while (current != NULL) {
            if (current->alive) {
                if ((current->x <= 10 && alien_direction == -1) || 
                    (current->x >= 590 && alien_direction == 1)) {
                    hit_edge = 1;
                    break;
                }
            }
            current = current->next;
        }
        
        if (hit_edge) {
            alien_direction *= -1;
            aliens = alien_update_all(aliens, 0, 20);
        } else {
            aliens = alien_update_all(aliens, alien_direction * 10, 0);
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
                break;
            }
        }
        current = current->next;
    }
}

void draw_explosion(int x, int y, int frame) {
    if (!explosion_active) return;
    
    explosion_timer++;
    
    
    if (explosion_timer >= explosion_duration) {
        explosion_active = 0;
        explosion_timer = 0;
        reset_game();
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

void reset_game(void) {
 
    ship_init(&ship);
    
 
    if (projectiles != NULL) {
        pnode_free(projectiles);
        projectiles = NULL;
    }
    
    if (aliens != NULL) {
        alien_free(aliens);
    }
    aliens = alien_create_grid(19, 10);
    
 
    explosion_active = 0;
    explosion_timer = 0;
    alien_direction = 1;
    alien_move_timer = 0;
    alien_move_speed = 30; 
    projectile_cooldown = 0;
}

void render(void) {
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    if (!explosion_active) {
        ship_draw(&ship);
    }
    
    projectiles = pnode_display(projectiles);
    aliens = alien_display_all(aliens);
    
    if (explosion_active) {
        draw_explosion(explosion_x, explosion_y, explosion_timer);
    }
    
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void keyscan(void) {
    const Uint8 *keys = SDL_GetKeyboardState(0);
    
    if (!explosion_active) {
        if (keys[SDL_SCANCODE_LEFT]) {
            ship_left(&ship);
        }
        if(keys[SDL_SCANCODE_RIGHT]) {
            ship_right(&ship);
        }
        if(keys[SDL_SCANCODE_SPACE] && projectile_cooldown == 0) {
            projectiles = pnode_add(projectiles, ship.x+(ship.w/2), ship.y-ship.h, 0);
            projectile_cooldown = projectile_cooldown_max; 
        }
    }
}