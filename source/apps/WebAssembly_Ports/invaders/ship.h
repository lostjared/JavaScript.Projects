#ifndef _SHIP_H_
#define _SHIP_H_

#include"sdl.h"


struct Ship {
    int x,y;
    int w,h;
    // other stuff
};


void ship_init(struct Ship *s);
void ship_draw(struct Ship *s);
void ship_left(struct Ship *s);
void ship_right(struct Ship *s);
void ship_clamp(struct Ship *s);
int ship_inrect(struct Ship *s, int x, int y, int w, int h);
#endif