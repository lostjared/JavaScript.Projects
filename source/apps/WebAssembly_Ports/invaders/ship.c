#include "ship.h"


void ship_init(struct Ship *s) {
    s->w = 25;
    s->h = 25;
    s->x = 640/2-(s->w/2);
    s->y = 360-(s->h/2);
}

void ship_draw(struct Ship *s) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); 
    SDL_RenderDrawLine(renderer, s->x + 12, s->y + 2, s->x + 12, s->y + 2);  
    SDL_RenderDrawLine(renderer, s->x + 10, s->y + 4, s->x + 14, s->y + 4);  
    SDL_RenderDrawLine(renderer, s->x + 8, s->y + 6, s->x + 16, s->y + 6);   
    SDL_RenderDrawLine(renderer, s->x + 6, s->y + 8, s->x + 18, s->y + 8);   
    SDL_RenderDrawLine(renderer, s->x + 4, s->y + 10, s->x + 20, s->y + 10); 
    SDL_RenderDrawLine(renderer, s->x + 2, s->y + 12, s->x + 6, s->y + 12);   
    SDL_RenderDrawLine(renderer, s->x + 18, s->y + 12, s->x + 22, s->y + 12); 
    SDL_RenderDrawLine(renderer, s->x + 1, s->y + 14, s->x + 5, s->y + 14);   
    SDL_RenderDrawLine(renderer, s->x + 19, s->y + 14, s->x + 23, s->y + 14); 
    
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 
    SDL_RenderDrawPoint(renderer, s->x + 11, s->y + 6);   
    SDL_RenderDrawPoint(renderer, s->x + 13, s->y + 6);   
    SDL_RenderDrawLine(renderer, s->x + 10, s->y + 8, s->x + 14, s->y + 8);  
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); 
    SDL_RenderDrawPoint(renderer, s->x + 8, s->y + 16);   
    SDL_RenderDrawPoint(renderer, s->x + 12, s->y + 16);  
    SDL_RenderDrawPoint(renderer, s->x + 16, s->y + 16);  
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); 
    SDL_RenderDrawPoint(renderer, s->x + 3, s->y + 10);   
    SDL_RenderDrawPoint(renderer, s->x + 21, s->y + 10);  
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); 
    SDL_RenderDrawLine(renderer, s->x + 6, s->y + 10, s->x + 6, s->y + 12);   
    SDL_RenderDrawLine(renderer, s->x + 18, s->y + 10, s->x + 18, s->y + 12); 
    
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); 
    SDL_RenderDrawPoint(renderer, s->x + 12, s->y + 3); 
}

void ship_left(struct Ship *s) {
    if(s->x > 0)
        s->x -= 3; 
    ship_clamp(s);
}

void ship_right(struct Ship *s) {
    if(s->x < 640-(s->w/2))
        s->x += 3; 
    ship_clamp(s);
}

void ship_clamp(struct Ship *s) {
    if(s->x < 0)
        s->x = 0;
    if(s->x > 640-s->w)
        s->x = 640-s->w;
}