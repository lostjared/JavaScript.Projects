/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/


#ifndef __MXFONT__H_
#define __MXFONT__H_

#include"SDL.h"
#include<string.h>

#ifdef __cplusplus
extern "C" {
#endif


/*! \struct SDL_Font font to hold data for mxFont's */
struct SDL_Font {
	/*! width */
	int mx;
	/*! height */
	int my;
	/*! transparent color */
	int tcolor;
	/*! \struct letter the fonts letters */
	struct letter {
		int **fnt_ptr;
	} letters[128];
};


extern int SDL_PrintTextDepth(int *depth, struct SDL_Surface *surf, struct SDL_Font *fnt, int x, int y, Uint32 color, const char *src);
extern struct SDL_Font *SDL_InitFont(const char *src);
extern void SDL_FreeFont(struct SDL_Font *m);
extern int  SDL_PrintText(struct SDL_Surface *surf, struct SDL_Font *m, int x, int y, Uint32 color, const char *src);
extern void SDL_PrintTextScaled(struct SDL_Surface *surf, struct SDL_Font *m, int x, int y, int w, int h, Uint32 color, const char *src);
extern void *lock(SDL_Surface *surf, Uint8 type);
extern void unlock(SDL_Surface *surf);
extern void setpixel(void *buff, Uint32 x, Uint32 y, Uint32 color, Uint8 type, Uint16 pitch);
extern Uint32 getpixel(SDL_Surface *surf, int x, int y, Uint8 type, Uint16 pitch, SDL_Color *c) ;
int SDL_PrintTextWidth(int   *depth,struct SDL_Surface *surf, struct SDL_Font *fnt, int x, int y, int width, Uint32 color, const char *src);
extern int SDL_PrintTextIdle(struct SDL_Surface *surf, struct SDL_Font *fnt, int  x, int y, int size_width,Uint32 color, const char *src);

#ifdef __cplusplus
}
#endif

#endif

