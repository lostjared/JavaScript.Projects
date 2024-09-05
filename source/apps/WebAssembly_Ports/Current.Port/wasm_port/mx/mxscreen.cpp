/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/



#include "mxscreen.h"
#include"mxpng.h"
#include"mx_exception.h"


namespace mx
{

	void mxScreen::loadSurface ( mx::mxSurface &surf, std::string filename )
	{
		SDL_Surface *surface = 0;
		std::ostringstream stream;

		mx::mxPng p;
		if( p.pngOpen ( filename ) )
		{
			surface = p.LoadPNG();
			p.pngClose();
			if ( surface == 0 )
				throw mx::mxException<std::string> ( " Error loading surface.. " + filename + "\n");
			surf = surface;
		} else throw mx::mxException<std::string>(" Error loading surface " + filename + "\n");
	}

}


