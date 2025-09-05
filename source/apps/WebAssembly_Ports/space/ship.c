#include "ship.h"
#include "sdl.h"

#define MAX_PROJECTILES 30
#define MAX_ASTEROIDS 20
#define PROJECTILE_SPEED 5.0f
#define PROJECTILE_LIFETIME 60  
#define FIRE_COOLDOWN 30
#define SHOTS_PER_BURST 3    
#define FIRE_DELAY 8         

#define EXPLOSION_DURATION 90     
#define RESPAWN_DELAY 120         
#define INVULNERABLE_TIME 180     

#define MIN_ASTEROID_RADIUS 8.0f  
#define LARGE_ASTEROID_POINTS 20
#define MEDIUM_ASTEROID_POINTS 50
#define SMALL_ASTEROID_POINTS 100

#define ASTEROID_VERTICES 8  

Ship the_ship;
bool keyLeft = false, keyRight = false, keyThrust = false;

Projectile projectiles[MAX_PROJECTILES];
Asteroid asteroids[MAX_ASTEROIDS];
Particle explosion_particles[MAX_PARTICLES];


void init_projectiles(void) {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        projectiles[i].active = false;
    }
}

void update_projectiles(void) {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (projectiles[i].active) {
            
            projectiles[i].x += projectiles[i].vx;
            projectiles[i].y += projectiles[i].vy;
            
            
            if (projectiles[i].x < 0 || projectiles[i].x >= SCALE_W || 
                projectiles[i].y < 0 || projectiles[i].y >= SCALE_H) {
                projectiles[i].active = false;
                continue;
            }
            
            
            projectiles[i].angle--;
            if (projectiles[i].angle <= 0) {
                projectiles[i].active = false;
            }
        }
    }
}

void draw_projectiles(void) {
    
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (projectiles[i].active) {
    
            float endX = projectiles[i].x - projectiles[i].vx * 0.5;
            float endY = projectiles[i].y - projectiles[i].vy * 0.5;
            SDL_RenderDrawLine(renderer, 
                              (int)projectiles[i].x, (int)projectiles[i].y,
                              (int)endX, (int)endY);
        }
    }
}

void fire_projectile(float x, float y, float angle) {
    
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!projectiles[i].active) {
            
            float c = cosf(angle);
            float s = sinf(angle);
            float lx1 = 0, ly1 = -12;  
            
            
            projectiles[i].x = x + (lx1 * c - ly1 * s);
            projectiles[i].y = y + (lx1 * s + ly1 * c);
            
            projectiles[i].vx = sinf(angle) * PROJECTILE_SPEED;
            projectiles[i].vy = -cosf(angle) * PROJECTILE_SPEED; 
            projectiles[i].angle = PROJECTILE_LIFETIME;  
            projectiles[i].active = true;
            break;
        }
    }
}


void init_asteroids(void) {
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        asteroids[i].active = false;
    }

    for (int i = 0; i < 4; i++) {
        float x, y;
        
        do {
            x = rand() % SCALE_W;
            y = rand() % SCALE_H;
        } while (hypotf(x - the_ship.x, y - the_ship.y) < 100);
        
        float vx = ((rand() % 100) - 50) / 50.0f;  
        float vy = ((rand() % 100) - 50) / 50.0f;  
        float radius = 20.0f + (rand() % 20);      
        
        spawn_asteroid(x, y, vx, vy, radius);
    }
}

void update_asteroids(void) {
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (asteroids[i].active) {
            
            asteroids[i].x += asteroids[i].vx;
            asteroids[i].y += asteroids[i].vy;
            
            
            if (asteroids[i].x < 0) asteroids[i].x += SCALE_W;
            if (asteroids[i].x >= SCALE_W) asteroids[i].x -= SCALE_W;
            if (asteroids[i].y < 0) asteroids[i].y += SCALE_H;
            if (asteroids[i].y >= SCALE_H) asteroids[i].y -= SCALE_H;
        }
    }
}

void draw_asteroids(void) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (asteroids[i].active) {
            for (int j = 0; j < ASTEROID_VERTICES; j++) {
                int next = (j + 1) % ASTEROID_VERTICES;
                
                int x1 = (int)(asteroids[i].x + asteroids[i].vertices[j][0]);
                int y1 = (int)(asteroids[i].y + asteroids[i].vertices[j][1]);
                int x2 = (int)(asteroids[i].x + asteroids[i].vertices[next][0]);
                int y2 = (int)(asteroids[i].y + asteroids[i].vertices[next][1]);
                
                SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
            }
        }
    }
}

void spawn_asteroid(float x, float y, float vx, float vy, float radius) {
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (!asteroids[i].active) {
            asteroids[i].x = x;
            asteroids[i].y = y;
            asteroids[i].vx = vx;
            asteroids[i].vy = vy;
            asteroids[i].radius = radius;
            asteroids[i].active = true;
            
            for (int j = 0; j < ASTEROID_VERTICES; j++) {
                float angle = (float)j / ASTEROID_VERTICES * 2 * PI;
                
                float vertex_radius = radius * (0.7f + ((rand() % 30) / 100.0f));
                
                asteroids[i].vertices[j][0] = cosf(angle) * vertex_radius;
                asteroids[i].vertices[j][1] = sinf(angle) * vertex_radius;
            }
            
            break;
        }
    }
}

void init_ship(void) {
    the_ship.x = SCALE_W / 2.0f;
    the_ship.y = SCALE_H / 2.0f;
    the_ship.vx = 0.0f;
    the_ship.vy = 0.0f;
    the_ship.angle = 0.0f;
    the_ship.lives = 3;
    the_ship.fire_cooldown = 0;
    the_ship.burst_count = 0;
    the_ship.exploding = false;
    the_ship.explosion_timer = 0;
    the_ship.score = 0;
}

void draw_ship(void) {
    if (the_ship.exploding) {
        draw_explosion();
        return;
    }
    
    float lx1 = 0, ly1 = -12;
    float lx2 = -8, ly2 = 8;
    float lx3 = 8, ly3 = 8; 
    
    int x1, y1, x2, y2, x3, y3;
    float c = cosf(the_ship.angle);
    float s = sinf(the_ship.angle);
    
    
    x1 = (int)(the_ship.x + (lx1 * c - ly1 * s));
    y1 = (int)(the_ship.y + (lx1 * s + ly1 * c));
    x2 = (int)(the_ship.x + (lx2 * c - ly2 * s));
    y2 = (int)(the_ship.y + (lx2 * s + ly2 * c));
    x3 = (int)(the_ship.x + (lx3 * c - ly3 * s));
    y3 = (int)(the_ship.y + (lx3 * s + ly3 * c));
    
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    SDL_RenderDrawLine(renderer, x2, y2, x3, y3);
    SDL_RenderDrawLine(renderer, x3, y3, x1, y1);
    
    
    if (keyThrust) {
        SDL_SetRenderDrawColor(renderer, 255, 160, 0, 255);
        x1 = (int)(the_ship.x + ((-4) * c - 8 * s));
        y1 = (int)(the_ship.y + ((-4) * s + 8 * c));
        x2 = (int)(the_ship.x + (0 * c - 16 * s));
        y2 = (int)(the_ship.y + (0 * s + 16 * c));
        x3 = (int)(the_ship.x + (4 * c - 8 * s));
        y3 = (int)(the_ship.y + (4 * s + 8 * c));
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
        SDL_RenderDrawLine(renderer, x2, y2, x3, y3);
    }
}

void check_asteroid_collisions(void) {
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (!asteroids[i].active) continue;
        
        for (int j = i + 1; j < MAX_ASTEROIDS; j++) {
            if (!asteroids[j].active) continue;
            
            float dx = asteroids[i].x - asteroids[j].x;
            float dy = asteroids[i].y - asteroids[j].y;
            float distance = sqrtf(dx * dx + dy * dy);
            float min_dist = asteroids[i].radius + asteroids[j].radius;
            
            if (distance < min_dist) {
                float nx = dx / distance;
                float ny = dy / distance;
                
                float dvx = asteroids[i].vx - asteroids[j].vx;
                float dvy = asteroids[i].vy - asteroids[j].vy;
                
                float dot_product = nx * dvx + ny * dvy;
                
                if (dot_product < 0) {
                    float impulse = -1.0f * dot_product;
                    
                    float speed1 = sqrtf(asteroids[i].vx * asteroids[i].vx + 
                                        asteroids[i].vy * asteroids[i].vy);
                    float speed2 = sqrtf(asteroids[j].vx * asteroids[j].vx + 
                                        asteroids[j].vy * asteroids[j].vy);
                    
                    asteroids[i].vx += nx * impulse;
                    asteroids[i].vy += ny * impulse;
                    asteroids[j].vx -= nx * impulse;
                    asteroids[j].vy -= ny * impulse;
                    
                    float new_speed1 = sqrtf(asteroids[i].vx * asteroids[i].vx + 
                                           asteroids[i].vy * asteroids[i].vy);
                    float new_speed2 = sqrtf(asteroids[j].vx * asteroids[j].vx + 
                                           asteroids[j].vy * asteroids[j].vy);
                    
                    if (new_speed1 > 0) {
                        float scale = speed1 / new_speed1;
                        asteroids[i].vx *= scale;
                        asteroids[i].vy *= scale;
                    }
                    
                    if (new_speed2 > 0) {
                        float scale = speed2 / new_speed2;
                        asteroids[j].vx *= scale;
                        asteroids[j].vy *= scale;
                    }
                    
                    float overlap = min_dist - distance;
                    float separation = overlap * 0.5f;
                    
                    asteroids[i].x += nx * separation;
                    asteroids[i].y += ny * separation;
                    asteroids[j].x -= nx * separation;
                    asteroids[j].y -= ny * separation;
                }
            }
        }
    }
}


void check_ship_asteroid_collision(void) {
    if (the_ship.exploding) return;
    
    float ship_radius = 10.0f;
    
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (!asteroids[i].active) continue;
        
        float dx = the_ship.x - asteroids[i].x;
        float dy = the_ship.y - asteroids[i].y;
        float distance = sqrtf(dx * dx + dy * dy);
        
        if (distance < (ship_radius + asteroids[i].radius)) {
            start_ship_explosion();
            return;
        }
    }
}

void start_ship_explosion(void) {
    the_ship.exploding = true;
    the_ship.explosion_timer = EXPLOSION_DURATION;
    for (int i = 0; i < MAX_PARTICLES; i++) {
        explosion_particles[i].x = the_ship.x;
        explosion_particles[i].y = the_ship.y;
        float angle = (rand() % 628) / 100.0f;  
        float speed = 0.5f + (rand() % 20) / 10.0f;  
        
        explosion_particles[i].vx = cosf(angle) * speed;
        explosion_particles[i].vy = sinf(angle) * speed;
        
        explosion_particles[i].lifetime = 30 + (rand() % 60);
        explosion_particles[i].active = true;
    }
}


void update_explosion(void) {
    if (!the_ship.exploding) return;
    
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (explosion_particles[i].active) {
            explosion_particles[i].x += explosion_particles[i].vx;
            explosion_particles[i].y += explosion_particles[i].vy;
            
            explosion_particles[i].vx *= 0.98f;
            explosion_particles[i].vy *= 0.98f;
            
            explosion_particles[i].lifetime--;
            if (explosion_particles[i].lifetime <= 0) {
                explosion_particles[i].active = false;
            }
        }
    }
    
    the_ship.explosion_timer--;
    if (the_ship.explosion_timer <= 0) {
        the_ship.lives--;  
        the_ship.exploding = false;
        
        the_ship.x = SCALE_W / 2.0f;
        the_ship.y = SCALE_H / 2.0f;
        the_ship.vx = 0.0f;
        the_ship.vy = 0.0f;
        the_ship.angle = 0.0f;
    }
}

void draw_explosion(void) {
    if (!the_ship.exploding) return;
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);  
    
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (explosion_particles[i].active) {
            if (explosion_particles[i].lifetime > 60) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  
            } else if (explosion_particles[i].lifetime > 40) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);    
            } else if (explosion_particles[i].lifetime > 20) {
                SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);    
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);      
            }
            
            SDL_RenderDrawPoint(renderer, (int)explosion_particles[i].x, 
                               (int)explosion_particles[i].y);
        }
    }
}

bool can_fire(void) {
    if (the_ship.fire_cooldown > 0) {
        the_ship.fire_cooldown--;
        return false;
    }
    if (the_ship.burst_count >= SHOTS_PER_BURST) {
        the_ship.fire_cooldown = FIRE_COOLDOWN;
        the_ship.burst_count = 0;
        return false;
    }
    the_ship.burst_count++;
    the_ship.fire_cooldown = FIRE_DELAY;
    return true;
}

void check_projectile_asteroid_collisions(void) {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!projectiles[i].active) continue;
        
        for (int j = 0; j < MAX_ASTEROIDS; j++) {
            if (!asteroids[j].active) continue;
            
            float dx = projectiles[i].x - asteroids[j].x;
            float dy = projectiles[i].y - asteroids[j].y;
            float distance = sqrtf(dx * dx + dy * dy);
            
            if (distance < asteroids[j].radius) {
                projectiles[i].active = false;
                split_asteroid(j);
                if (asteroids[j].radius >= 20.0f) {
                    the_ship.score += LARGE_ASTEROID_POINTS;
                } else if (asteroids[j].radius >= MIN_ASTEROID_RADIUS) {
                    the_ship.score += MEDIUM_ASTEROID_POINTS;
                } else {
                    the_ship.score += SMALL_ASTEROID_POINTS;
                }
                
                break;
            }
        }
    }
}


void split_asteroid(int index) {
    float x = asteroids[index].x;
    float y = asteroids[index].y;
    float radius = asteroids[index].radius / 2.0f;  
    
    
    if (radius >= MIN_ASTEROID_RADIUS) {
        int num_splits = 2 + (rand() % 2);  
        
        for (int i = 0; i < num_splits; i++) {
            float angle = (rand() % 628) / 100.0f;  
            float speed = 0.5f + (rand() % 15) / 10.0f;
            
            float vx = cosf(angle) * speed;
            float vy = sinf(angle) * speed;
            
            
            vx += asteroids[index].vx * 0.3f;
            vy += asteroids[index].vy * 0.3f;
            
            float offset = radius * 0.5f;
            float new_x = x + cosf(angle) * offset;
            float new_y = y + sinf(angle) * offset;
            
            spawn_asteroid(new_x, new_y, vx, vy, radius);
        }
    }
    
    asteroids[index].active = false;
}

void draw_score(void) {
    int dots = the_ship.score / 10;
    int x = 10;
    int y = 10;
    int spacing = 3;
    
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  
    
    for (int i = 0; i < dots && i < 100; i++) {
        SDL_Rect scoreRect = {x + (i % 25) * spacing, y + (i / 25) * spacing, 2, 2};
        SDL_RenderFillRect(renderer, &scoreRect);
    }
}

void update_ship(void) {
    
    if (the_ship.exploding) {
        update_explosion();
        return;
    }
    
    if (keyLeft)
        the_ship.angle -= 0.08f;
    if (keyRight)
        the_ship.angle += 0.08f;
    
    
    float accel = 0.14f;
    if (keyThrust) {
        the_ship.vx += sinf(the_ship.angle) * accel;
        the_ship.vy -= cosf(the_ship.angle) * accel;
    }
    
    
    float drag = 0.995f;
    the_ship.vx *= drag;
    the_ship.vy *= drag;
    
    
    the_ship.x += the_ship.vx;
    the_ship.y += the_ship.vy;
    
    
    if (the_ship.x < 0) 
        the_ship.x += SCALE_W;
    if (the_ship.x >= SCALE_W) 
        the_ship.x -= SCALE_W;
    if (the_ship.y < 0) 
        the_ship.y += SCALE_H;
    if (the_ship.y >= SCALE_H) 
        the_ship.y -= SCALE_H;
        

    check_ship_asteroid_collision();
}

void check_and_spawn_asteroids(void) {
    bool asteroids_exist = false;
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (asteroids[i].active) {
            asteroids_exist = true;
            break;
        }
    }
    
    if (!asteroids_exist) {
        int count = 4 + (the_ship.score / 200) % 3;  
        
        for (int i = 0; i < count; i++) {
            float x, y;
            
            do {
                x = rand() % SCALE_W;
                y = rand() % SCALE_H;
            } while (hypotf(x - the_ship.x, y - the_ship.y) < 100);
            
            float speed_factor = 1.0f + (the_ship.score / 1000.0f);  
            if (speed_factor > 2.0f) speed_factor = 2.0f;  
            
            float vx = ((rand() % 100) - 50) / 50.0f * speed_factor;
            float vy = ((rand() % 100) - 50) / 50.0f * speed_factor;
            float radius = 20.0f + (rand() % 20);
            
            spawn_asteroid(x, y, vx, vy, radius);
        }
    }
}