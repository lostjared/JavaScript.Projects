/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/



#ifndef __MX_TYPES_H__
#define __MX_TYPES_H__

#include<iostream>
#include<string>
#include<cstdlib>

#include "SDL.h"
#include "mxf.h"
#include "mx_exception.h"
#include "mxsurface.h"
#include "mxtype.h"


namespace mx
{

	SDL_Surface *CreateBuffer(size_t w = 640, size_t h = 480);

	/*!
	 * \union __Color union to hold 32bit RGBA value
	 */
	typedef union __Color
	{
		/*! 32bit unsigned integer value */
		unsigned int  value;
		/*! array of characters to assign any byte of the integer to a specific value */
		unsigned char colors[4];
		/*! Default Constructor */
		__Color() { value = 0; }
		/*! Constructor set to 32bit integer value
		 * \param v 32bit integer
		 */
		__Color(unsigned int v) { value = v; }
		/*! Constructor to set each byte of the RGBA value to
		 * \param r Red Value
		 * \param g Green Value
		 * \param b Blue Value
		 */
		__Color( unsigned char r, unsigned char g, unsigned char b)
		{
			colors[0] = r, colors[1] = g, colors[2] = b;
		}
	} _Color;

	/*! \class Color Color class, encapsulate RGB colors into a easy to use Object */
	class Color {

	public:
		/*! Default Constructor */
		Color();
		/*! Constructor to init Color object
		 *  \param r  Red Value
		 *  \param g  Green Value
		 *  \param b  Blue Value
		 */
		Color(unsigned char r, unsigned char g, unsigned char b);
		/*! Constructor to init Color object
		 * \param value 32bit unsigned integer value
		 */
		Color(const unsigned int value);
		/*! Copy Constructor
		 * \param c Color value to set this object to
		 */
		Color(const Color &c);
		/*! Constructor
		 * \param c of type __Color sets this color to
		 */
		Color(const __Color &c);
		/*! Construct to take type SDL_Color
		 * \param c of type SDL_Color
		 */
		Color(const SDL_Color &c);



		/*! overloaded assignment operator set this object to Color type
		 *  \param c set this to object of type Color
		 *  \return  this object
		 */
		Color& operator=(const Color &c);
		/*! overloaded assignment operator set this object's Color value to
		 *  \param long_x value to set color to
		 *  \return this object
		 */
		Color& operator=(const unsigned int long_x);
		/*! overloaded assignment operator to this this object's Color value to
		 * \param c SDL_Color value to set this Color objects value to
		 * \return this object
		 */
		Color& operator=(const SDL_Color &c);

		/*! turns the value of the internal color union to a unsigned integer
		 *  \return 32bit RGBA value
		 */
		const unsigned int toInteger() const;
		/*! turns the value of the internal color union to a SDL_Color
		 * \return SDL_Color structure
		 */
		const SDL_Color toSDL_Color() const;
		/*! Color union */
		 __Color color;
		 /*! mapRGB map a color value to a format of a specific surface
		  *  \param surface surface for pixel format
		  *  \param r for red value
		  *  \param g for green value
		  *  \param b for blue value
		  *  \return unsigned integer for the value mapped
		  */
		static unsigned int mapRGB(mx::mxSurface  &surface, unsigned char r, unsigned char g, unsigned char b)
		{
			return SDL_MapRGB(surface.getSurface()->format, r, g, b);
		}
		/*! mapRGB map a color value from a __Color union by a mxSurface's pixel format
		 * \param surface surface from which to obtain the pixelformat
		 * \param c __Color value to map
		 * \return mapped unsigned integer
		 */
		static unsigned int mapRGB(mx::mxSurface &surface, const __Color &c)
		{
			return SDL_MapRGB(surface.getSurface()->format, c.colors[0], c.colors[1], c.colors[2]);
		}
		/*! mapRGB map a color from a Color object by a mxSurface's pixel format
		 * \param surface surface from whiich to obtain the pixel format
		 * \param c color object from which to map the return value
		 * \return 32bit unsigned integer mapped by the surface's pixel format.
		 */
		static unsigned int mapRGB(mx::mxSurface &surface, const  Color &c)
		{
			return mapRGB(surface, c.color);
		}

		/*! mapRGB map a color from a Color object by a SDL_Surface's pixel format
		 * \param surface a SDL_Surface from which to obtain the SDL_Surface 's pixel format
		 * \param c the color from which to obtain the RGB values from
		 * \return the mapped 32bit rgb value
		 */
		static unsigned int  mapRGB(SDL_Surface *surface, const Color &c) {
			return SDL_MapRGB(surface->format, c.color.colors[0],  c.color.colors[1],    c.color.colors[2]);
		}
	};

	/*! \typedef s8_t signed 8bit type */
	typedef signed char s8_t;
	/*! \typedef u8_t unsigned 8bit type */
	typedef unsigned char u8_t;
	/*! \typedef s16_t signed 16bit type */
	typedef signed short s16_t;
	/*! \typedef u16_t unsigned 16bit type */
	typedef unsigned short u16_t;
	/*! \typedef s32_t signed 32bit type */
	typedef signed int s32_t;
	/*! \typedef u32_t unsigned 32bit type */
	typedef unsigned int u32_t;
}


#endif


