/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/

#include "mximage.h"


namespace mx
{

	SDL_Surface *mxImage::loadIMG(string filename) {

		std::string prefix = filename.substr(filename.rfind(".")+1, filename.length() - filename.rfind(".")-1 );

		unsigned int i;
		for(i = 0; i < prefix.length(); i++) prefix[i] = tolower(prefix[i]);

		if(prefix == "bmp")
			return SDL_LoadBMP(filename.c_str());

		else if(prefix == "jpg" || prefix == "jpeg") {

			mx::mxJpeg j;
			if( j.jpgOpen(filename) == false ) return 0;
			SDL_Surface *surf = j.LoadJPG();
			j.jpgClose();
			return surf;
		}

		else if(prefix == "png") {
			mx::mxPng p;

			if( p.pngOpen(filename) == false ) return 0;
			SDL_Surface *surf = p.LoadPNG();
			p.pngClose();
			return surf;
		}

		return 0;

	}


	bool mxImage::loadIMG(mx::mxSurface &surf, string filename) {

		SDL_Surface *surface = loadIMG(filename);
		if(surface == 0) return false;
		surf = surface;
		return true;
	}

	void mxImage::saveJPG(SDL_Surface *surf, string filename) {

		jpeg_compress_struct comp_info;
		jpeg_error_mgr jerr;

		JSAMPROW row_ptr[1];

		FILE *fptr = fopen(filename.c_str(), "w");
		if(!fptr) return;

		comp_info.err = jpeg_std_error(&jerr);
		jpeg_create_compress(&comp_info);
		jpeg_stdio_dest(&comp_info, fptr);

		comp_info.image_width = surf->w;
		comp_info.image_height = surf->h;
		comp_info.input_components = 3;
		comp_info.in_color_space = JCS_RGB;

		jpeg_set_defaults(&comp_info);
		jpeg_start_compress(&comp_info, TRUE);



		if(SDL_MUSTLOCK(surf)) SDL_LockSurface(surf);


		unsigned char *buffer = (unsigned char*) calloc (1,  surf->w * surf->h * 3 );
		unsigned int counter = 0;

		unsigned char *ptr = buffer;
		unsigned char *pix = (unsigned char*)surf->pixels;

		while(counter < (surf->w*surf->h)) {


			unsigned int *ptr_val = (unsigned int *) pix;
			SDL_GetRGB(*ptr_val, surf->format, &ptr[0], &ptr[1], &ptr[2]);
			pix += surf->format->BytesPerPixel;
			ptr += 3;
			counter++;
		}

		while( comp_info.next_scanline < comp_info.image_height ) {
			row_ptr[0] = &buffer[ comp_info.next_scanline * comp_info.image_width * comp_info.input_components];
			jpeg_write_scanlines (&comp_info, row_ptr, 1);
		}


		free(buffer);


		if(SDL_MUSTLOCK(surf)) SDL_UnlockSurface(surf);

		jpeg_finish_compress(&comp_info);
		jpeg_destroy_compress(&comp_info);
		fclose(fptr);

	}

}




