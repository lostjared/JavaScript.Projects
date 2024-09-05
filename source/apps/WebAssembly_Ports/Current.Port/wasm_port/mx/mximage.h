/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/



#ifndef __MX_IMAGE_H_
#define __MX_IMAGE_H_
#include "mxjpeg.h"
#include "mxpng.h"
#include "mxsurface.h"


namespace mx {
	/*! \class mxImage
	 *  image class for jpeg / png graphics
	 */
	class mxImage {
	public:
		/*! mxImage default constructor */
		mxImage() { }
		/*! loadIMG loads a image proper loading function selected by filename extension
		 *  \param filename name of file to load
		 *  \return loaded SDL_Surface *
		 */
		SDL_Surface *loadIMG(string filename);

		/*! loadIMG load a image to a mxSurface
		 *  \param mxSurface surface to copy loaded image to
		 *  \param filename file of image to load
		 *  \return boolean value true for success false for failure
		 */
		bool loadIMG(mx::mxSurface &surface, string filename);
		/*! saveJPG save a SDL_Surface as a Jpeg graphic file
		 * \param surf SDL_Surface * to save
		 * \param filename filename to save as
		 */
		void saveJPG(SDL_Surface *surf, string filename);
	};
}








#endif


