/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/




#include "mxpng.h"



namespace mx
{

	mxPng::mxPng() {}


	mxPng::mxPng(std::string fname)
	{
		pngOpen(fname);

	}


	int mxPng::pngOpen(std::string fname)
	{

		png_byte header[8];


		FILE *fp = fopen(fname.c_str(), "rb");
		if (!fp)
			return 0;
		fread(header, 1, 8, fp);
		if (png_sig_cmp(header, 0, 8))
			return 0;



		png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

		if (!png_ptr)
			return 0;

		info_ptr = png_create_info_struct(png_ptr);
		if (!info_ptr)
			return 0;

		if (setjmp(png_jmpbuf(png_ptr)))
			return 0;

		png_init_io(png_ptr, fp);
		png_set_sig_bytes(png_ptr, 8);

		png_read_info(png_ptr, info_ptr);
		
		width = png_get_image_width(png_ptr, info_ptr);
		height = png_get_image_height(png_ptr, info_ptr);
		color_type = png_get_color_type(png_ptr, info_ptr);
		bit_depth = png_get_bit_depth(png_ptr, info_ptr);
		

		number_of_passes = png_set_interlace_handling(png_ptr);
		png_read_update_info(png_ptr, info_ptr);


		if (setjmp(png_jmpbuf(png_ptr)))
			return 0;

		row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
		for (y=0; y<height; y++)
			row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr, info_ptr));

		png_read_image(png_ptr, row_pointers);

	    fclose(fp);

		return 1;
	}



	int mxPng::pngClose()
	{

		for (y=0; y<height; y++)
			free(row_pointers[y]);

		free(row_pointers);

		// whoops forgot this
		png_destroy_info_struct (png_ptr, &info_ptr);
		png_destroy_read_struct (&png_ptr, 0, 0);


		return 1;
	}



	SDL_Surface *mxPng::LoadPNG()
	{

		SDL_Surface *surf;
		int pixel_space = 3;

        	if(color_type == PNG_COLOR_TYPE_RGB)
        	{

			pixel_space = 3;

		}

		if(color_type == PNG_COLOR_TYPE_RGBA)
			pixel_space = 4;

		surf = CreateBuffer(width,height);

		SDL_FillRect(surf, 0, SDL_MapRGB(surf->format, 255,255,255));

		int x,y;

		if (SDL_MUSTLOCK(surf))
		{
			if(SDL_LockSurface(surf) < 0)
				return 0;

		}

		Uint32 *buffer = (Uint32*) surf->pixels;


		for(y=0;y<height;y++)
		{

			png_byte *row = row_pointers[y];

			for(x=0; x<width;x++)
			{

				png_byte *ptr = &(row[x*pixel_space]);
				Uint32 *uptr = &buffer[x+y*width];
				Uint32 value = SDL_MapRGB(surf->format, ptr[0], ptr[1], ptr[2]);
				*uptr = value;
			}


		}

		if(SDL_MUSTLOCK(surf))
			SDL_UnlockSurface(surf);

		return surf;
	}

}










