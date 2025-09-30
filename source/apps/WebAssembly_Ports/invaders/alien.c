#include"alien.h"
#include"projectile.h"
#include"sdl.h"
#include<SDL2/SDL.h>

int alien_lowest_y = 0;

void alien_init(struct Alien *a) {
    if (a == NULL) {
        return;
    }
    a->x = 0;
    a->y = 0;
    a->type = 0;
    a->alive = 1;
    a->next = NULL;
}

void alien_free(struct Alien *a) {
    if (a == NULL) {
        return;
    }
    
    if (a->next != NULL) {
        alien_free(a->next);
    }
    
    free(a);
}

void draw_alien(int x, int y, int type) {
    switch(type) {
        case 0: 
            SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);        
            SDL_RenderDrawLine(renderer, x+6, y+2, x+13, y+2);  
            SDL_RenderDrawLine(renderer, x+4, y+3, x+15, y+3);  
            SDL_RenderDrawLine(renderer, x+3, y+4, x+16, y+4);  
            SDL_RenderDrawLine(renderer, x+2, y+5, x+17, y+5);  
            SDL_RenderDrawPoint(renderer, x+6, y+6);
            SDL_RenderDrawPoint(renderer, x+13, y+6);
            SDL_RenderDrawLine(renderer, x+8, y+7, x+11, y+7);
            SDL_RenderDrawLine(renderer, x+2, y+8, x+4, y+10);   
            SDL_RenderDrawLine(renderer, x+6, y+8, x+7, y+11);   
            SDL_RenderDrawLine(renderer, x+12, y+8, x+13, y+11); 
            SDL_RenderDrawLine(renderer, x+15, y+8, x+17, y+10); 
            break;
            
        case 1: 
            SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255); 
            SDL_RenderDrawLine(renderer, x+7, y+2, x+12, y+2);  
            SDL_RenderDrawLine(renderer, x+5, y+3, x+14, y+3);  
            SDL_RenderDrawLine(renderer, x+3, y+4, x+16, y+4);  
            SDL_RenderDrawLine(renderer, x+4, y+5, x+15, y+5);  
            SDL_RenderDrawPoint(renderer, x+7, y+6);
            SDL_RenderDrawPoint(renderer, x+12, y+6);
            SDL_RenderDrawLine(renderer, x+1, y+3, x+3, y+5);   
            SDL_RenderDrawLine(renderer, x+16, y+5, x+18, y+3); 
            SDL_RenderDrawPoint(renderer, x+5, y+7);
            SDL_RenderDrawPoint(renderer, x+7, y+8);
            SDL_RenderDrawPoint(renderer, x+9, y+9);
            SDL_RenderDrawPoint(renderer, x+10, y+9);
            SDL_RenderDrawPoint(renderer, x+12, y+8);
            SDL_RenderDrawPoint(renderer, x+14, y+7);
            break;
            
        case 2:
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); 
            SDL_RenderDrawLine(renderer, x+8, y+3, x+11, y+3);  
            SDL_RenderDrawLine(renderer, x+6, y+4, x+13, y+4);  
            SDL_RenderDrawLine(renderer, x+7, y+5, x+12, y+5);  
            SDL_RenderDrawPoint(renderer, x+8, y+6);
            SDL_RenderDrawPoint(renderer, x+11, y+6);
            SDL_RenderDrawLine(renderer, x+3, y+2, x+6, y+4);   
            SDL_RenderDrawLine(renderer, x+2, y+4, x+6, y+5);   
            SDL_RenderDrawLine(renderer, x+3, y+6, x+6, y+6);   
            SDL_RenderDrawLine(renderer, x+4, y+8, x+7, y+7);   
            SDL_RenderDrawLine(renderer, x+13, y+4, x+16, y+2); 
            SDL_RenderDrawLine(renderer, x+13, y+5, x+17, y+4); 
            SDL_RenderDrawLine(renderer, x+13, y+6, x+16, y+6); 
            SDL_RenderDrawLine(renderer, x+12, y+7, x+15, y+8); 
            break;
            
        case 3: 
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); 
            SDL_RenderDrawLine(renderer, x+7, y+2, x+12, y+2);
            SDL_RenderDrawLine(renderer, x+6, y+3, x+13, y+3);
            SDL_RenderDrawLine(renderer, x+5, y+4, x+14, y+4);
            SDL_RenderDrawLine(renderer, x+3, y+5, x+16, y+5);
            SDL_RenderDrawLine(renderer, x+4, y+6, x+15, y+6);
            SDL_RenderDrawPoint(renderer, x+5, y+7);
            SDL_RenderDrawPoint(renderer, x+8, y+7);
            SDL_RenderDrawPoint(renderer, x+11, y+7);
            SDL_RenderDrawPoint(renderer, x+14, y+7);
            SDL_RenderDrawPoint(renderer, x+8, y+3);
            SDL_RenderDrawPoint(renderer, x+11, y+3);
            break; 
        default: 
            SDL_SetRenderDrawColor(renderer, rand()%255, rand() %255, rand()%255, 255); 
            SDL_RenderDrawLine(renderer, x+5, y+3, x+14, y+3);
            SDL_RenderDrawLine(renderer, x+4, y+4, x+15, y+4);
            SDL_RenderDrawLine(renderer, x+4, y+5, x+15, y+5);
            SDL_RenderDrawLine(renderer, x+5, y+6, x+14, y+6);
            SDL_RenderDrawPoint(renderer, x+7, y+5);
            SDL_RenderDrawPoint(renderer, x+12, y+5);
            break;
    }
}

void alien_draw(struct Alien *a) {
    if (a == NULL || !a->alive) {
        return;
    }
    draw_alien(a->x, a->y, a->type);
}

struct Alien *alien_new(int x, int y, int type) {
    struct Alien *alien = (struct Alien *) malloc(sizeof(struct Alien));
    if (alien == NULL) {
        return NULL;
    }
    
    alien->x = x;
    alien->y = y;
    alien->type = type;
    alien->alive = 1;
    alien->next = NULL;
    return alien;
}

struct Alien *alien_add(struct Alien *head, int x, int y, int type) {
    struct Alien *new_alien = alien_new(x, y, type);
    if (new_alien == NULL) {
        return head;
    }
    
    new_alien->next = head;
    return new_alien;
}


struct Alien *alien_display_all(struct Alien *head) {
    struct Alien *current = head;
    while (current != NULL) {
        if (current->alive) {
            alien_draw(current);
            if(current->y > alien_lowest_y) {
                alien_lowest_y = current->y;
            }
        }
        current = current->next;
    }
    return head;
}

struct Alien *alien_update_all(struct Alien *head, int move_x, int move_y) {
    struct Alien *current = head;
    while (current != NULL) {
        if (current->alive) {
            current->x += move_x;
            current->y += move_y;
            if(current->y > alien_lowest_y) {
                alien_lowest_y = current->y;
            }
        }
        current = current->next;
    }
    return head;
}

struct Alien *alien_create_grid(int cols, int rows) {
    struct Alien *head = NULL;
    int spacing_x = 45; 
    int spacing_y = 35; 
    int grid_width = cols * spacing_x;
    int start_x = (640 - grid_width) / 2;
    int start_y = 40; 

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            int x = start_x + col * spacing_x;
            int y = start_y + row * spacing_y;
            
            int type;
            if (row == 0) type = 0;      
            else if (row <= 2) type = 1; 
            else type = 2;               
            
            head = alien_add(head, x, y, type);
        }
    }
    return head;
}