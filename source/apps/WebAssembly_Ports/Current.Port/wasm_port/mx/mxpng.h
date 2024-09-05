/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/



#ifndef _MX_PNG_H_
#define _MX_PNG_H_


#include<png.h>
#include<string>
#include "SDL.h"
#include "mx_types.h"

namespace mx
{

	/*! \class mxPng png image loader class
	 */
	class mxPng {

	public:
		/*! mxPng constructor */
		mxPng();
		/*! constructor
		 * \param fname name of file to load
		 */
		mxPng(std::string fname);

		/*! pngOpen open a png graphic
		 * \param fname file name to load
		 */
		int pngOpen(std::string fname);
		/*! pngClose close png file */
		int pngClose();
		/*! LoadPNG Load the PNG File should be called after pngOpen and before pngClose
		 * \return SDL_Surface * of png graphic
		 */
		SDL_Surface *LoadPNG();
	private:
		int x, y;
		int width, height;
		png_byte color_type;
		png_byte bit_depth;
		png_structp png_ptr;
		png_infop info_ptr;
		int number_of_passes;
		png_bytep * row_pointers;

	};

}

#endif




