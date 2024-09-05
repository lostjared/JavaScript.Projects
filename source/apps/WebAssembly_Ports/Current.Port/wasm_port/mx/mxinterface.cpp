/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/


#include "mxinterface.h"

namespace mx
{

	mxSurfacePainter::mxSurfacePainter(mxSurface *surface)
	{
		this->surface = surface;
		pbuf = 0;
		locked = false;
		font = 0;

	}

	const bool mxSurfacePainter::paintLock()
	{

		pbuf = lock(this->surface->getSurface(), this->surface->getSurface()->format->BytesPerPixel);

		if(pbuf) locked = true;


		return true;

	}


	const bool mxSurfacePainter::paintUnlock()
	{
		unlock(this->surface->getSurface());

		locked = false;

		return true;
	}

	const bool mxSurfacePainter::getPixel(int x, int y, SDL_Color *col, Color &return_color)
	{


		if(locked == false) return false;




		getpixel(this->surface->getSurface(), x  , y ,   surface->getSurface()->format->BitsPerPixel, surface->getSurface()->pitch, col);

		return_color.color.colors[0] = col->r;
		return_color.color.colors[1] = col->g;
		return_color.color.colors[2] = col->b;


		return true;
	}

	const Uint32 mxSurfacePainter::getPixel(int x, int y, SDL_Color *col)
	{
		Uint32 rt_val = 0;

		rt_val = getpixel(surface->getSurface(), x, y, surface->getSurface()->format->BitsPerPixel, surface->getSurface()->pitch, col);


		return rt_val;
	}

	const bool mxSurfacePainter::setPixel(int x, int y, Color colz)
	{


		if(locked == false) return false;


			setpixel(pbuf, x,y, SDL_MapRGB(this->surface->getSurface()->format, colz.color.colors[0], colz.color.colors[1], colz.color.colors[2]), this->surface->getSurface()->format->BytesPerPixel, this->surface->getSurface()->pitch);

		return true;

	}


	const bool mxSurfacePainter::setPixel(int x, int y, Uint32 color)
	{
		if(locked == false) return false;

		setpixel(pbuf, x, y, color, surface->getSurface()->format->BytesPerPixel, surface->getSurface()->pitch);

		return true;

	}

	void mxSurfacePainter::resetSurface(mxSurface *surf)
	{
		surface = surf;
	}




	void mxSurfacePainter::printText(int x, int y, Color col,std::string text)
	{

		if(font == 0) throw mxException<string>(" No found font set on printtext [:" + text + ":]");

		SDL_PrintText(surface->getSurface(), font, x,y, SDL_MapRGB(surface->getSurface()->format, col.color.colors[0], col.color.colors[1], col.color.colors[2]), text.c_str());

	}





	void mxPainter::lock()
	{
		surface->lockSurface();
	}


	void mxPainter::unlock()
	{
		surface->unlockSurface();
	}

	void mxPainter::GetPixel(int x, int y, mx::Color &col) {

		if(!(x >= 0 && x < surface->size().width  && y >= 0 && y < surface->size().height))
			return;

		unsigned int color = this->getpixel(x,y);
		unsigned char r,g,b;
		SDL_GetRGB(color,surface->getSurface()->format, &r,&g,&b);
		col = mx::Color(r,g,b);
	}

	void mxPainter::fillRect(SDL_Rect *rc, mx::Color &color)
	{
		SDL_FillRect(surface->getSurface(), rc, mx::Color::mapRGB(*surface, color));
	}

	void mxPainter::fillRect(int x, int y, int w, int h, mx::Color &color)
	{
		SDL_Rect rc = { x,y,w,h };
		fillRect(&rc, color);
	}

	void mxPainter::fillRect(Rect rc, Color col)
	{
		SDL_Rect rcx = { rc.x(), rc.y(), rc.width(), rc.height() };
		SDL_FillRect(surface->getSurface(), &rcx, mx::Color::mapRGB(*surface, col));
	}

	void mxPainter::drawLine( Line line, Color color )
	{
		drawLine(line.x1(), line.y1(), line.x2(), line.y2(), color);
	}

	void mxPainter::drawSquare(Rect rc)
	{
		Color color = Color(255,255,255);

		drawVertical(rc.x(), rc.x()+rc.width(), rc.y(), color);
		drawVertical(rc.x(), rc.x()+rc.width(), rc.y()+rc.height(), color);
		drawHorizontal(rc.x(), rc.y(), rc.y()+rc.height(), color);
		drawHorizontal(rc.x()+rc.width(), rc.y(), rc.y()+rc.height(), color);
	}

	void mxPainter::drawHorizontal(int orig_x, int start_y, int stop_y, Color color)
	{
		register int y;
		for(y = start_y; y < stop_y; y++)
			setpixel(orig_x, y, color);
	}

	void mxPainter::drawVertical(int start_x, int stop_x, int orig_y, Color color)
	{
		register int x;
		for(x = start_x; x < stop_x; x++)
			setpixel(x,orig_y,color);
	}

	// Breezeham's Line drawing algorthim
	void mxPainter::drawLine(int start_x, int start_y, int stop_x, int stop_y, Color color)
	{
		unsigned int cur_color = mx::Color::mapRGB(*surface, color);
        int y_unit,x_unit;
        int ydiff = stop_y-start_y;
        if(ydiff<0)
        {
                ydiff = -ydiff;
                y_unit=-1;
        }
        else
        {
                y_unit =1;
        }
        int xdiff=stop_x-start_x;
        if(xdiff<0)
        {
                xdiff=-xdiff;
                x_unit = -1;
        }
        else
        {
                x_unit = 1;
        }

        int error_term=0;
        if(xdiff>ydiff)
        {
                int length=xdiff+1;
                for(int i = 0; i <length; i++)
                {
                 		setpixel(start_x, start_y, cur_color);

                        start_x += x_unit;
                        error_term+=ydiff;
                        if(error_term>xdiff)
                        {
                                error_term-=xdiff;
                                start_y+=y_unit;
                        }
                }
        }
        else
        {
                int length = ydiff+1;
                for(int i = 0; i < length; i++)
                {
                        setpixel(start_x, start_y, cur_color);
                		start_y += y_unit;
                        error_term+=xdiff;
                        if(error_term>0)
                        {
                                error_term-=ydiff;
                                start_x += x_unit;
                        }

                }
        }

	}

}








