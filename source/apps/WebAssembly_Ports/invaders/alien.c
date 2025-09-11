#include"alien.h"
#include"sdl.h"

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

void alien_draw(struct Alien *a) {
    if (a == NULL || !a->alive) {
        return;
    }

    switch(a->type) {
        case 0:
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); 
            break;
        case 1: 
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); 
            break;
        case 2: 
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); 
            break;
        default:
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 
            break;
    }
    
    SDL_Rect alien_rect = {a->x, a->y, 20, 15}; 
    SDL_RenderFillRect(renderer, &alien_rect);
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
        }
        current = current->next;
    }
    return head;
}

struct Alien *alien_create_grid(int w, int h) {
    struct Alien *head = NULL;
    int rows = h;
    int cols = w;
    int spacing_x = 30;
    int spacing_y = 25;
    int grid_width = cols * spacing_x;
    int start_x = (640 - grid_width) / 2;
    int start_y = 50;

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            int x = start_x + col * spacing_x;
            int y = start_y + row * spacing_y;
            int type = row / 2;
            head = alien_add(head, x, y, type);
        }
    }

    return head;
}