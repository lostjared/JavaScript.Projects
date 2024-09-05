/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/

#ifndef _GINTER_H_
#define _GINTER_H_


#include "SDL.h"

namespace mx3d
{


	/*! \struct graphRect
	 *	local Rect structure with pitch variable
	 */
	typedef struct _Rect {

		int x,y,w,h;
		int Pitch;
	} grapRect ;

	/*! \class graphicsInterface
	 *  interface for drawing 3D library in SDL
	 */
	class graphicsInterface
	{
	public:

		/*! graphicsInterace constructor (default)
		 * \param f surface to draw to
		 */
		graphicsInterface(SDL_Surface *f = 0);
		~graphicsInterface();

		/*! lock function */
		void lock();
		/*! unlock function */
		void unlock();

		/*! fast setpixel
		 *  \param x x coordinate
		 *  \param y y coordinate
		 *  \param pitch pitch
		 *  \param color 32bit RGBA color
		 */
		void fast_sp(int x, int y,int pitch, unsigned int color);
		/*! setpixel (slow)
		 * \param x x coordinate
		 * \param y y coordinate
		 * \param color 32bit RGBA color
		 */
		void setpixel(int x, int y, unsigned int color);
		/*! fast getpixel
		 * \param x x coordinate
		 * \param y y coordinate
		 * \return unsigned int containg 32bit RGBA color value
		 */
		unsigned int fast_gp(int x, int y);

		/*!  drawrect drawings a rectangle */
		void  drawrect(int x, int y, int w , int h, unsigned int color);
		/*! drawline draws a line */
		void  drawline(int start_x, int start_y, int stop_x, int stop_y,unsigned int color);
		/*! drawtoptri draws top triangle */
		void  drawtoptri(int x1, int y1, int x2, int y2, int x3, int y3, unsigned int color);
		/*! drawgentri draws a general triangle */
		void  drawgentri(int x1, int y1, int x2, int y2, int x3, int y3, unsigned int color);
		/*! drawtoptri2 another top triangle function */
		void  drawtoptri2(float x1, float y1, float x2, float y2, float x3, float y3, unsigned int color);
		/*! drawbottomtri2 draws a bottom triangle */
		void  drawbottomtri2(float x1, float y1, float x2, float y2, float x3, float y3, unsigned int color);
		/*! drawtri2 draws a triangle */
		void  drawtri2(float x1, float y1, float x2, float y2, float x3, float y3, unsigned int color);
		/*! draw bottom triangle */
		void  drawbottomtri(int x1, int y1, int x2, int y2, int x3, int y3, unsigned int color);
		/*! draws a triangle */
		void  drawtri(int x1, int y1, int x2, int y2, int x3, int y3, unsigned int color);
		/*!  draws a horizontal line */
		void  drawhline(int x, int x2, int y, unsigned int color);
		/*! screen width and height variables */
		int w,h;
		/*! rect of surface */
		grapRect rect;
		/*! front (drawing) surface */
		SDL_Surface *front;
	};
}
#endif
