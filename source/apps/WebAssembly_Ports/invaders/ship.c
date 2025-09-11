#include "ship.h"


void ship_init(struct Ship *s) {
    s->w = 25;
    s->h = 25;
    s->x = 640/2-(s->w/2);
    s->y = 360-(s->h/2);
}

void ship_draw(struct Ship *s) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Point points[4] = {
        {s->x + s->w / 2, s->y},           
        {s->x, s->y + s->h},               
        {s->x + s->w, s->y + s->h},        
        {s->x + s->w / 2, s->y}            
    };
    SDL_RenderDrawLines(renderer, points, 4);
}
void ship_left(struct Ship *s) {
    if(s->x > 0)
        s->x -= 5;

    ship_clamp(s);
}
void ship_right(struct Ship *s) {
    if(s->x < 640-(s->w/2))
        s->x  += 5;

    ship_clamp(s);
}

void ship_clamp(struct Ship *s) {
    if(s->x < 0)
        s->x = 0;
    if(s->x > 640-s->w)
        s->x = 640-s->w;
}