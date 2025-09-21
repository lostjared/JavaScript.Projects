/* coded by Jared Bruni */
#include "ship.h"
#include "sdl.h"

#define MAX_PROJECTILES 50
#define MAX_ASTEROIDS 20
#define PROJECTILE_SPEED 5.0f
#define PROJECTILE_LIFETIME 60  
#define FIRE_COOLDOWN 5        
#define SHOTS_PER_BURST 5      
#define FIRE_DELAY 3           

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
    
    for (int i = 0; i < MAX_PARTICLES; i++) {
        explosion_particles[i].active = false;
    }
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
            float min_distance = asteroids[i].radius + asteroids[j].radius;
            
            if (distance < min_distance && distance > 0) {
                float nx = dx / distance;
                float ny = dy / distance;
                
                float overlap = min_distance - distance;
                float separation = overlap * 0.5f;
                
                asteroids[i].x += nx * separation;
                asteroids[i].y += ny * separation;
                asteroids[j].x -= nx * separation;
                asteroids[j].y -= ny * separation;
                
                float rel_vx = asteroids[i].vx - asteroids[j].vx;
                float rel_vy = asteroids[i].vy - asteroids[j].vy;
                
                float vel_along_normal = rel_vx * nx + rel_vy * ny;
                
                if (vel_along_normal > 0) continue;
                
                float restitution = 0.8f;
                float impulse = -(1 + restitution) * vel_along_normal;
                
                float mass_ratio = 0.5f;
                
                asteroids[i].vx += impulse * mass_ratio * nx;
                asteroids[i].vy += impulse * mass_ratio * ny;
                asteroids[j].vx -= impulse * mass_ratio * nx;
                asteroids[j].vy -= impulse * mass_ratio * ny;
            }
        }
    }
}


void check_ship_asteroid_collision(void) {
    if (the_ship.exploding) return; 
    
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (asteroids[i].active) {
            float dx = the_ship.x - asteroids[i].x;
            float dy = the_ship.y - asteroids[i].y;
            float distance = sqrtf(dx * dx + dy * dy);
            
            float ship_radius = 8.0f;
            
            if (distance < asteroids[i].radius + ship_radius) {
                start_ship_explosion();
                
                float explosion_x = the_ship.x + dx * 0.5f;
                float explosion_y = the_ship.y + dy * 0.5f;
                create_asteroid_explosion(explosion_x, explosion_y);
                
                float bounce_force = 2.0f;
                float norm_x = -dx / distance;
                float norm_y = -dy / distance;
                
                asteroids[i].vx += norm_x * bounce_force;
                asteroids[i].vy += norm_y * bounce_force;
                
                break;
            }
        }
    }
}

void start_ship_explosion(void) {
    if (!the_ship.exploding) {
        the_ship.exploding = true;
        the_ship.explosion_timer = EXPLOSION_DURATION;
        the_ship.lives--; 
        
        for (int i = 0; i < MAX_PARTICLES; i++) {
            explosion_particles[i].x = the_ship.x + ((rand() % 20) - 10);
            explosion_particles[i].y = the_ship.y + ((rand() % 20) - 10);
            
            float angle = (rand() % 628) / 100.0f;
            float speed = 0.5f + (rand() % 20) / 10.0f;
            
            explosion_particles[i].vx = cosf(angle) * speed;
            explosion_particles[i].vy = sinf(angle) * speed;
            
            explosion_particles[i].lifetime = 30 + (rand() % 60);
            explosion_particles[i].active = true;
        }
    }
}

void create_asteroid_explosion(float x, float y) {
    int particles_to_create = 15 + (rand() % 10); 
    int created = 0;
    
    for (int i = 0; i < MAX_PARTICLES && created < particles_to_create; i++) {
        if (!explosion_particles[i].active) {
            explosion_particles[i].x = x + ((rand() % 16) - 8);
            explosion_particles[i].y = y + ((rand() % 16) - 8);
            
            float angle = (rand() % 628) / 100.0f;
            float speed = 1.0f + (rand() % 20) / 10.0f; 
            
            explosion_particles[i].vx = cosf(angle) * speed;
            explosion_particles[i].vy = sinf(angle) * speed;
            
            explosion_particles[i].lifetime = 15 + (rand() % 25); 
            explosion_particles[i].active = true;
            created++;
        }
    }
}

void update_explosion(void) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (explosion_particles[i].active) {
            explosion_particles[i].x += explosion_particles[i].vx;
            explosion_particles[i].y += explosion_particles[i].vy;
            
            explosion_particles[i].vx *= 0.97f;
            explosion_particles[i].vy *= 0.97f;
            
            explosion_particles[i].lifetime--;
            if (explosion_particles[i].lifetime <= 0) {
                explosion_particles[i].active = false;
            }
            
            if (explosion_particles[i].x < 0) explosion_particles[i].x += SCALE_W;
            if (explosion_particles[i].x >= SCALE_W) explosion_particles[i].x -= SCALE_W;
            if (explosion_particles[i].y < 0) explosion_particles[i].y += SCALE_H;
            if (explosion_particles[i].y >= SCALE_H) explosion_particles[i].y -= SCALE_H;
        }
    }
    
    if (the_ship.exploding) {
        the_ship.explosion_timer--;
        if (the_ship.explosion_timer <= 0) {
            the_ship.exploding = false;
        }
    }
}

void draw_explosion(void) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (explosion_particles[i].active) {
            float life_percent = (float)explosion_particles[i].lifetime / 50.0f;
            
            if (life_percent > 0.7f) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 
            } else if (life_percent > 0.5f) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);   
            } else if (life_percent > 0.2f) {
                SDL_SetRenderDrawColor(renderer, 255, 128, 0, 255);   
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);     
            }
            
            SDL_Rect particle = {
                (int)explosion_particles[i].x, 
                (int)explosion_particles[i].y,
                2, 
                2
            };
            SDL_RenderFillRect(renderer, &particle);
        }
    }
}

bool can_fire(void) {
    if (the_ship.fire_cooldown <= 0) {
        if (the_ship.burst_count < SHOTS_PER_BURST) {
            the_ship.fire_cooldown = FIRE_DELAY;  
            the_ship.burst_count++;
            return true;
        } else {
            
            the_ship.fire_cooldown = FIRE_COOLDOWN;
            the_ship.burst_count = 0;
            return false;
        }
    }
    return false;
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
    
    if (radius < MIN_ASTEROID_RADIUS) {
        create_asteroid_explosion(x, y); 
        the_ship.score += SMALL_ASTEROID_POINTS;
        asteroids[index].active = false;
        return;
    }
    
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
    
    if (radius >= 25.0f) {
        the_ship.score += LARGE_ASTEROID_POINTS;
    } else {
        the_ship.score += MEDIUM_ASTEROID_POINTS;
    }
    
    create_asteroid_explosion(x, y);
    
    asteroids[index].active = false;
}

void check_and_spawn_asteroids(void) {
    int active_count = 0;
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (asteroids[i].active) {
            active_count++;
        }
    }
    
    
    int min_asteroids = 3; 
    if (active_count < min_asteroids) {
        int asteroids_to_spawn = min_asteroids - active_count;
        
        for (int spawn = 0; spawn < asteroids_to_spawn; spawn++) {
            float x, y;
            int attempts = 0;
            
            do {
                if (rand() % 2) {
                    x = (rand() % 2) ? -30.0f : SCALE_W + 30.0f;
                    y = rand() % SCALE_H;
                } else {
                    x = rand() % SCALE_W;
                    y = (rand() % 2) ? -30.0f : SCALE_H + 30.0f;
                }
                attempts++;
            } while (hypotf(x - the_ship.x, y - the_ship.y) < 100.0f && attempts < 10);
            
            float vx = ((rand() % 100) - 50) / 50.0f;
            float vy = ((rand() % 100) - 50) / 50.0f;
            
            float radius;
            int size_roll = rand() % 100;
            if (size_roll < 20) {
                radius = 30.0f + (rand() % 15); 
            } else if (size_roll < 60) {
                radius = 20.0f + (rand() % 10); 
            } else {
                radius = 12.0f + (rand() % 8);  
            }
            
            spawn_asteroid(x, y, vx, vy, radius);
        }
    }
}

void update_ship(void) {

    if (the_ship.exploding) return;
    
    if (keyLeft) {
        the_ship.angle -= 0.15f; 
    }
    if (keyRight) {
        the_ship.angle += 0.15f; 
    }
    
    
    if (keyThrust) {
        float thrust = 0.2f; 
        the_ship.vx += sinf(the_ship.angle) * thrust;
        the_ship.vy += -cosf(the_ship.angle) * thrust;
        
        float max_speed = 4.0f;
        float current_speed = sqrtf(the_ship.vx * the_ship.vx + the_ship.vy * the_ship.vy);
        if (current_speed > max_speed) {
            the_ship.vx = (the_ship.vx / current_speed) * max_speed;
            the_ship.vy = (the_ship.vy / current_speed) * max_speed;
        }
    }
    
    the_ship.vx *= 0.98f;
    the_ship.vy *= 0.98f;
    
    the_ship.x += the_ship.vx;
    the_ship.y += the_ship.vy;
    
    if (the_ship.x < 0) the_ship.x += SCALE_W;
    if (the_ship.x >= SCALE_W) the_ship.x -= SCALE_W;
    if (the_ship.y < 0) the_ship.y += SCALE_H;
    if (the_ship.y >= SCALE_H) the_ship.y -= SCALE_H;
    
    if (the_ship.fire_cooldown > 0) {
        the_ship.fire_cooldown--;
    }
}