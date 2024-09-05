/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/


#ifndef __MX_SURFACE__H__
#define __MX_SURFACE__H__

#include"mxf.h"
#include<iostream>
#include "mxtype.h"


namespace mx
{

	using std::cout;
	/*! \struct _mxPoint 2D Point structure */
	typedef struct _mxPoint { /*! x coordinate */ int x; /*! y coordinate */ int y; } mxPoint;

	/*! \class mxSurface surface encapsulation class */
	class mxSurface {

	public:

		/*! default constructor,
		 * \param surface has default argument of 0 */
		mxSurface(SDL_Surface *surface = 0);
		/*! copy constructor
		 * \param c of type mxSurface
		 */
		mxSurface(const mxSurface &c);
		/*! virtual Destructor */
		virtual ~mxSurface();
		/*! overloaded assignment operator =
		 * \param c of type mxSurface
		 * \return of type mxSurface Reference of this object
		 */
		mxSurface& operator=(const mxSurface &c);
		/*! overloaded assignment operator =
		* \param surface of type SDL_Surface * assign a surface to this object
		* \return of type mxSurface Reference of this object
		*/
		mxSurface& operator=(SDL_Surface *surface);
		/*! getSurface returns this objects encapsulated surface
		 * \return returns this surface
		 */
		SDL_Surface *getSurface() const;
		/*! surface returns this objects encapsulated surface
		 * \return this surface
		 */
		SDL_Surface *surface() const { return object_surface; }
		/*! setCopySurface sets the surface by copying it to a newly created surface of 32bpp
		 *  \param surface a raw SDL_Surface
		 */
		void setCopySurface(SDL_Surface *surface); // copy surface, turn into 32bpp
		/*! width return width of this surface
		 * \return width of this surface
		 */
		const int width() const;
		 /*! height of this surface
		  * \return height of this surface
		  */
		const int height() const;
		/*! returns pitch
		 * \return pitch of image
		 */
		const int pitch() const;
		/*! operator SDL_Surface for use in expressions
		 *  \return SDL_Surface *
		 */
		operator SDL_Surface *();
		/*! operator of SDL_PixelFormat for use in expressions
		 * \return SDL_PixelFormat *
		 */
		operator SDL_PixelFormat *();
		/*! buildResizeTable - builds a table for a stretch reallocation to increase speed of already slow stretch function
		 *  \param nw new width
		 *  \param nh new height
		 *  \param w old width
		 *  \param h old height
		 *  \param buffer array of array of mxPoint pointers
		 *  \return newly allocated table
		 */
		mxPoint **buildResizeTable(int nw, int nh, int w, int h, mxPoint **buffer);

		/*! overloaded operator[] for use in getting and plotting pixels for a surface be sure to lockSurface before
		 * \param pos the position of the data to be put or get aligned by 4 bytes
		 * \return data at position pos
		 */
		unsigned int &operator[](unsigned int pos); // get and plot pixels by using the [] operator
							   // just be sure to lock your surface before you do
							  // and to unlock the surface when your complete.

		/*!
		 * lockSurface - locks the surface for plotting pixels or manipulating the surface's raw data
		 *
		 */
		void lockSurface(); // locks the surface
		/*!
		 * isLocked - is this surface locked ?
		 * \return boolean value for locked or unlocked
		 */
		const bool isLocked() const; // returns whether is locked
		/*!
		 * unlockSurface - unlock the Surface
		 */
		void unlockSurface(); // unlocks
		/*!
		 * rawData return the surface's raw pixel data, be sure to lockSurface first
		 * \return void * pointing to the surface's raw data
		 */
		void *rawData();
		/*! createSurface - Create a New Surface
		 *  \param w width in pixels
		 *  \param h height in pixels
		 *  \return surface creation successful? true yes false no
		 */
		bool createSurface(int w, int h);
		/*! createSurface - Create a New Surface
		 * \param s Size object  Size(w,h) in pixels
		 * \return surface creation successful? true yes false no
		 */
		bool createSurface( Size &s );

		/*! copy surface to new surface by stretching it with resizeTable or with SDL_Stretch if enabled
		 *  \param c surface to copy to
		 *  \return true for success false for failure
		 */
		bool copyResizeSurface(const mxSurface &c);
		/*! copy surface from this object to size of surface referenced by param c stretch from src rect to dest rect
		 * \param c dest surface
		 * \param src source rectangle
		 * \param dst destination rectangle
		 * \return true for success false for failure
		 */
		bool copyResizeSurface(const mxSurface &c, SDL_Rect *src, SDL_Rect *dst);
		/*! copySurface - Copy a surface to this surface
		 *  \param c surface to copy from
		 *  \param source source rectangle
		 *  \param dest destination rectangle in this surface
		 *  \return boolean value for true on success false for failure
		 */
		bool copySurface(const mxSurface &c, SDL_Rect *source, SDL_Rect *dest);
		/*! copySurface - Copy a surface to this surface
		 * \param c for surface
		 * \param source for Rect object to contain source rectangle
		 * \param dest for Rect object to contain destination rectangle
		 * \return boolean value for true on success false on failure
		 */
		inline bool copySurface(const mxSurface &c, mx::Rect source, mx::Rect dest) { SDL_Rect src = { source.x(), source.y(), source.width(), source.height() }; SDL_Rect dst = { dest.x(), dest.y(), dest.width(), dest.height() }; return copySurface(c, &src, &dst); }
		/*! copySurfaceColorKey - Copy a surface to this surface with a color key
		 * \param c surface to copy
		 * \param source SDL_Rect for source
		 * \param dest SDL_Rect for dest
		 * \param color_key SDL_Color for color key
		 * \return boolean value to stand for true on success and false on failure
		 */
		bool copySurfaceColorKey(const mxSurface &c, SDL_Rect *source, SDL_Rect *dest, SDL_Color color_key);

		/*! Flip this surface if surface is front surface */
		void Flip() const;

		/*! Use this function if you do not want this object's destructor to release the surface on destruction
		 * \param noremove true for removal, false for no free
		 */
		virtual void noZero(bool noremove);
		/*! Use this function to manually clear the surface (free) it */
		void cleanSurface();

		/*! Update Rectangle within Surface
		 * \param x x coordinate
		 * \param y y coordinate
		 * \param w width in pixels
		 * \param h height in pixels
		 */
		void updateRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h) const;

		/*! Clear this surface */
		void Clear() const;

		/*! Load a bitmap from Memory versus from file
		 * \param memory pointer to stream of bytes for bitmap
		 * \return true/false value for success
		 */
		const bool loadBitmapFromMemory(void *memory);
		/*!
		 * line pos get the position in the 4byte alinged stream for X,Y coordinate pair , used for pixel plotting
		 * \param x x coordinate
		 * \param y y coordinate
		 * \return returns the offset for setting the data
		 */
		inline const int linepos(const int x, const int y) const { return (x+(y*width())); }

		/*! size returns Size object variable for Size of surface
		 * \return Size object
		 */
		Size size();

		/*! creates a copy of this surface
		 * \return mxSurface pointer for newly allocated surface
		 */
		mxSurface *copySurface();

	protected:
		/*! pointer to the encapsulated SDL_Surface */
		SDL_Surface *object_surface;
		 /*! internal cleanup */
		void clean_up();
	private:
		bool protected_noremove;
		mxPoint **p_buffer;
		bool is_locked;
	};

}

#endif

