/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/


#ifndef _MX_JPEG_H_
#define _MX_JPEG_H_
#include<stdio.h>
#include<stdlib.h>

extern "C" {
#include"jpeglib.h"
}

#ifdef HAVE_STDLIB_H
#undef HAVE_STDLIB_H
#endif

#ifdef HAVE_PROTOTYPES
#undef HAVE_PROTOTYPES
#endif


#include "mx_types.h"
#include<string>


namespace mx {


	using std::string;

	/*! \class mxJpeg  Jpeg loader class */
	class mxJpeg {
	public:

		/*! default constructor */
		mxJpeg() {
			data = 0;
		}
		/*! destructor */
		~mxJpeg() {
			if(data) delete [] data;
			data = 0;
		}
		/*! jpgOpen opens image file
		 * \param fileName name of file to open
		 * \return boolean value true for success false for failure
		 */
		bool jpgOpen(string fileName);
		/*! jpgOpen open image file
		 * \param fptr file pointer for opened file
		 * \return boolean value true for success false for failure
		 */
		bool jpgOpen(FILE *fptr);
		/*! jpgClose close image File */
		void jpgClose();

		/*! LoadJPG  loads a jpeg to a SDL_Surface call this function directly after jpgOpen and before jpgClose
		 * \return SDL_Surface *
		 */
		SDL_Surface *LoadJPG();
	protected:
		/*! raw image data */
		unsigned char *data;
		int w,h,bytes;
	private:
		mxJpeg(const mxJpeg &);
		mxJpeg &operator=(const mxJpeg &);

	};

}

#endif


