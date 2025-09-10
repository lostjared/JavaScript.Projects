#include<SDL2/SDL.h>
#include<stdlib.h>
#include"sdl.h"

int active = 1;
SDL_Event e;

void render(void);
void loop(void);

int main(int argc, char **argv) {

    if(!initSDL("[space]", 640, 360, 1280, 720)) {
        fprintf(stderr, "Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(loop, 0, 1);
#else
    while(active == 1) {
        loop();
    }
#endif

    releaseSDL();
    return 0;
}

void loop(void) {

    while(SDL_PollEvent(&e)) {
        switch(e.type) {
            case SDL_QUIT:
                active = 0;
                break;
        }
    }
    render();
}

void render(void) {


}