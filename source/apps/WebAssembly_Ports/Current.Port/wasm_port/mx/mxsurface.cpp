/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/



#include "mxsurface.h"
#include "mx_types.h"
#include "mxinterface.h"
#include<sstream>
#ifdef HAVE_CONFIG_H
#include<config.h>
#endif
#include<cmath>



#if (HAVE_SDL_STRETCH == 1)
extern "C" {
#include<SDL_stretch.h>
}
#endif


namespace mx
{


	int stretchX(int cur_w ,int x, int nw) {
		float xp = (float)x * (float)cur_w / (float)nw;
		return (int)xp;
	}

	int stretchY(int cur_h, int y, int nh) {
		float yp = (float)y * (float)cur_h / (float)nh;
		return (int)yp;
	}




	mxSurface::mxSurface(SDL_Surface *surface) : is_locked(false)
	{
		p_buffer = 0;

		object_surface = surface;
		noZero(false);
	}

	mxSurface::mxSurface(const mxSurface &c) : is_locked(false)
	{

		p_buffer = 0;
		clean_up();
		object_surface = c.object_surface;
		noZero(false);
	}


	mxSurface::~mxSurface()
	{
		clean_up();
	}


	mxSurface &mxSurface::operator=(const mxSurface &c)
	{
		clean_up();
		object_surface = c.object_surface;
		return *this;
	}


	void mxSurface::setCopySurface(SDL_Surface *surface)
	{
		clean_up();
		createSurface(surface->w, surface->h);
		copySurface(surface, 0, 0);
	}



	mxSurface &mxSurface::operator=(SDL_Surface *surface)
	{

		if(surface == 0) throw mx::mxException<std::string>("libmx: error operator= on mxsurface: surface is null");


		clean_up();
		object_surface = surface;
		return *this;
	}

	SDL_Surface *mxSurface::getSurface() const
	{
		return object_surface;
	}

	bool mxSurface::createSurface( Size &s )
	{
		return createSurface(s.width, s.height);
	}

	bool mxSurface::createSurface(int w, int h)
	{

   		static int rmask, gmask, bmask, amask;

		#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    		rmask = 0xff000000;
    		gmask = 0x00ff0000;
    		bmask = 0x0000ff00;
    		amask = 0x000000ff;
		#else
    		rmask = 0x000000ff;
    		gmask = 0x0000ff00;
    		bmask = 0x00ff0000;
    		amask = 0xff000000;
		#endif


		clean_up();

		object_surface =  SDL_CreateRGBSurface(SDL_SWSURFACE,w,h,32, rmask, gmask, bmask, amask);

		std::cout << "libmx: created surface  @" << w << "/" << h << "\n";

		if(object_surface)
		return true;

		throw mx::mxException<std::string>("libmx: Error surface creation failed\n");

		return false;
	}

	bool mxSurface::copySurface(const mxSurface &c, SDL_Rect *source, SDL_Rect *dest)
	{

		if(object_surface == 0) {

			throw mx::mxException<std::string>("libmx: Error cannot copy null surface\n");

			return false;
		}

		SDL_BlitSurface(c.object_surface, source, object_surface, dest);
		return true;

	}

	bool mxSurface::copySurfaceColorKey(const mxSurface &c, SDL_Rect *source, SDL_Rect *dest, SDL_Color color_key)
	{

		if(object_surface == 0) return false;

		SDL_SetColorKey(c.object_surface, SDL_SRCCOLORKEY, SDL_MapRGB(c.object_surface->format, color_key.r, color_key.g, color_key.b));
		copySurface(c, source, dest);

		return true;

	}

	void mxSurface::cleanSurface()
	{
		clean_up();
	}

	void mxSurface::clean_up()
	{

		if(p_buffer != 0)
		{

			int i;
			for(i = 0; i < getSurface()->w; i++)
			{

				delete [] p_buffer[i];
			}

			delete [] p_buffer;
			p_buffer = 0;
		}



		if(object_surface && protected_noremove == false)
		{

			cout << "libmx: removing surface: " << (unsigned long) object_surface <<  " ";
			SDL_FreeSurface(object_surface);
			object_surface = 0;
			cout << "removed\n";
		}


	}

	void mxSurface::Flip() const
	{
		SDL_Flip(object_surface);
	}

	mxSurface::operator SDL_Surface *()
	{
		return object_surface;
	}

	mxSurface::operator SDL_PixelFormat *()
	{
		if(object_surface == 0) throw mx::mxException<std::string>( " error null surface trying to use pixel format for implicit conversion");

		return object_surface->format;

	}


	void mxSurface::noZero(bool zero)
	{
		protected_noremove = zero;
	}





	mxPoint **mxSurface::buildResizeTable(int nw, int nh, int w, int h, mxPoint **buffer)
	{

		int i,z;

		std::cout << "building resize table\n";

		buffer = new mxPoint*[w];

		for(i =  0; i < w; i++)
		{

			buffer[i] = new mxPoint[ h ];

		}


		for(i = 0; i < w; i++)
		{

			for(z = 0; z < h; z++)
			{
				mxPoint *ptr = &buffer[i][z];
				ptr->x = stretchX(nw, i, w);
				ptr->y = stretchY(nh, z, h);

			}

		}

		return buffer;

	}


	bool mxSurface::copyResizeSurface(const mxSurface &c)
	{

#if (HAVE_SDL_STRETCH == 0)

		if(p_buffer == 0)
		{
			p_buffer = buildResizeTable(c.getSurface()->w, c.getSurface()->h, getSurface()->w, getSurface()->h, p_buffer);
		}

		mx::mxPainter pobj1(c), pobj2(*this);

		register int i,z;

		pobj1.lock(); pobj2.lock();

		for(i = 0; i < object_surface->w; i++)
		{

			for(z = 0; z < object_surface->h; z++)
			{

				static Color color;
				mxPoint *cur_pixel = &p_buffer[i][z];
				color =  pobj1.getpixel(cur_pixel->x, cur_pixel->y);
				pobj2.setpixel(i,z, color);
			}
		}

		pobj1.unlock(); pobj2.unlock();

#else
		SDL_StretchSurfaceRect( c.getSurface(), 0, getSurface(), 0);
#endif

		return true;
	}

	bool mxSurface::copyResizeSurface(const mxSurface &c, SDL_Rect *src, SDL_Rect *dst) {

#if (HAVE_SDL_STRETCH == 0)
#warning "you should have sdl-stretch compiled for this function to work\n"

		return false;
#else
		SDL_StretchSurfaceRect( c.getSurface(), src, getSurface(), dst );
#endif

		return true;
	}

	void mxSurface::updateRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h) const
	{
		SDL_UpdateRect(object_surface, x, y, w, h);
	}

	void mxSurface::Clear() const // clear to black
	{
		SDL_FillRect(object_surface, 0, SDL_MapRGB(object_surface->format, 0, 0, 0));
	}

	void mxSurface::lockSurface()
	{

		if(object_surface == 0) throw mx::mxException<std::string>(" null surface passed to lock operation\n ");


		if(SDL_MUSTLOCK(object_surface))
		{
			SDL_LockSurface(object_surface);
		}

		is_locked = true;

	}

	const bool mxSurface::isLocked() const
	{
		return (is_locked);
	}

	void mxSurface::unlockSurface()
	{
		if(object_surface == 0) throw mx::mxException<std::string>(" null surface passed to unlock operation\n ");

		if(SDL_MUSTLOCK(object_surface))
		{
			SDL_UnlockSurface(object_surface);
		}
		is_locked = false;
	}


	void *mxSurface::rawData()
	{
		if(is_locked == true) return object_surface->pixels;
		return 0;
	}

	// throws mxException type string, on error
	const bool mxSurface::loadBitmapFromMemory(void *memory)
	{
		SDL_RWops *opz;
		opz = SDL_RWFromMem(memory, sizeof(memory));
		if(opz == 0) throw mx::mxException<std::string>(" Error loading memory chunk to bitmap ");
		object_surface = SDL_LoadBMP_RW(opz, 1);
		SDL_FreeRW(opz);
		return true;
	}

	// easy method to plot pixels. no longer does bounds checking, due to slowdown

	unsigned int &mxSurface::operator[](unsigned int pos)
	{
			if(is_locked == false) throw mx::mxException<std::string>( " Error trying to manipulate pixels on surface which is not locked ");
			void  *r = rawData();
			unsigned int *ptr = (unsigned int *)r;
			return ptr[pos];
	}

	// convience functions

	const int mxSurface::width() const
	{
		if(object_surface == 0) return 0;
		return object_surface->w;
	}

	const int mxSurface::height() const
	{
		if(object_surface == 0) return 0;
		return object_surface->h;
	}

	const int  mxSurface::pitch() const
	{
		if(object_surface == 0) return 0;
		return object_surface->pitch;
	}

	Size mxSurface::size()
	{
		return Size(width(), height());
	}


	mxSurface *mxSurface::copySurface()
	{

		mxSurface *surf = 0;

		try
		{

			surf = new mxSurface();
			surf->createSurface(width(), height());
			surf->copySurface(*this, 0, 0);
			return surf;

		}
		catch(...)
		{
			delete surf;
		}

		return 0;
	}

}

