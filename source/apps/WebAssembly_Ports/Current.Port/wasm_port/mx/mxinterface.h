/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/

#ifndef __INTERFACE__H__
#define __INTERFACE__H__
#include "mxf.h"



#include "mxsurface.h"
#include "mx_types.h"
#include "mx_exception.h"
#include "mx_font.h"


namespace mx
{

	class mxSurfacePainter {

	public:

		mxSurfacePainter(mxSurface *surface = 0);
		void resetSurface(mxSurface *surface);
	  	const bool paintLock();
		const bool paintUnlock();
		const bool setPixel(int x, int y, Color col);
		const bool setPixel(int x, int y, Uint32 color);

		const bool getPixel(int x, int y, SDL_Color *col, Color &return_col);
		const Uint32 getPixel(int x, int y, SDL_Color *col); // for 24-bit

		void setFont(SDL_Font *font) { this->font = font; }
		void printText(int x, int y, Color col,std::string text);
	private:
		mxSurface *surface;
		void      *pbuf;
		bool locked;
		SDL_Font *font;

	};

	class mxPainter {

	public:

		mxPainter() {

			this->surface = 0;

		}
		 mxPainter(const mxSurface &surface)
		 {
			 this->surface = (mxSurface*)&surface;
		 }
		 void   setSurface(const    mxSurface &surface) {
			 this->surface = (mxSurface*)&surface;
		 }
		void lock();
		void unlock();
		inline void setpixel(int x, int y, unsigned int color) { surface->operator[](surface->linepos(x,y)) = color; }
		inline void setpixel(int x, int y, Color color) { surface->operator[](surface->linepos(x,y)) = mx::Color::mapRGB(*surface, color); }
		inline unsigned int getpixel(int x, int y) { return surface->operator[](surface->linepos(x,y)); }
		void GetPixel(int x, int y, mx::Color &color);
		void fillRect(SDL_Rect *rc, mx::Color &color);
		void fillRect(int x, int y, int w, int h, mx::Color &color);
		void fillRect(Rect rc, Color col);
		void drawLine(int	 start_x, int start_y, int stop_x, int stop_y, Color color);
		void drawLine( Line line, Color color );
		void drawHorizontal(int orig_x, int start_y, int stop_y, Color color);
		void drawVertical(int start_x, int stop_x, int orig_y, Color color);
		void drawSquare(Rect rc);
		mxSurface *getSurface() { return surface; }
	protected:
		mxSurface *surface;
	};
}














#endif

