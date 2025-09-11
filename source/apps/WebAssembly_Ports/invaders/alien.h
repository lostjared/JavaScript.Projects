#ifndef ALIEN_H
#define ALIEN_H

#include <stdlib.h>

struct Alien {
    int x, y;
    int type;
    int alive;
    struct Alien *next;
};

void alien_init(struct Alien *a);
void alien_free(struct Alien *a);
void alien_draw(struct Alien *a);
struct Alien *alien_new(int x, int y, int type);
struct Alien *alien_add(struct Alien *head, int x, int y, int type);
struct Alien *alien_display_all(struct Alien *head);
struct Alien *alien_update_all(struct Alien *head, int move_x, int move_y);

#endif