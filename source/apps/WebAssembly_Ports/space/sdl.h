#ifndef __SDL_H__
#define __SDL_H__

#include<SDL2/SDL.h>


int initSDL(const char *app, int w, int h, int sx, int sy);
void releaseSDL(void);

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Texture *texture;

#endif