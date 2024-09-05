/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/


#include "mxjpeg.h"
#include<iostream>


namespace mx
{

	bool mxJpeg::jpgOpen(FILE *fptr) {


		jpeg_decompress_struct deInfo;
		jpeg_error_mgr deErr;

		deInfo.err = jpeg_std_error(&deErr);
		jpeg_create_decompress(&deInfo);
		jpeg_stdio_src(&deInfo, fptr);
		jpeg_read_header(&deInfo, TRUE);
		jpeg_start_decompress(&deInfo);

		int width = deInfo.output_width, height = deInfo.output_height, depth = deInfo.num_components;
		int output_lines = deInfo.output_height;

		if(data != 0) delete [] data;
		data = 0;

		data = new unsigned char [ ( width * height ) * depth ];
		JSAMPROW row_ptr[1];

		row_ptr[0] = (unsigned char*) malloc(deInfo.output_width*deInfo.num_components);

		unsigned int i = 0;
		unsigned long location = 0;

		while (deInfo.output_scanline < deInfo.image_height)
		{
		    jpeg_read_scanlines(&deInfo, row_ptr, 1);
			for(i = 0; i <deInfo.image_width*deInfo.num_components; i++)
				data[location++] = row_ptr[0][i];
		 }

		jpeg_finish_decompress(&deInfo);
		jpeg_destroy_decompress(&deInfo);
		w = width, h = height, bytes = depth;
		fclose(fptr);
		free(row_ptr[0]);

		return true;
	}

	bool mxJpeg::jpgOpen(string fileName) {


		FILE *fptr = fopen(fileName.c_str(), "rb");

		if(fptr == 0) return false;

		return jpgOpen(fptr);

	}


	SDL_Surface *mxJpeg::LoadJPG() {

		SDL_Surface *surf = CreateBuffer(w,h);


		if(SDL_MUSTLOCK(surf)) SDL_LockSurface(surf);


		unsigned int *pixels = (unsigned int*) surf->pixels;
		unsigned int *buf = (unsigned int *) data;
		unsigned int pos = 0;
		unsigned char *dat = (unsigned char*) data;

		while ( pos < (w*h) ) {
			unsigned char *v = dat;
			pixels[pos++] = SDL_MapRGB(surf->format, v[0], v[1], v[2]);
			dat += bytes;
		}

		if(SDL_MUSTLOCK(surf)) SDL_UnlockSurface(surf);

		return surf;
	}

	void mxJpeg::jpgClose() {

		if(data) delete [] data;
		data = 0;
	}


}


