/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/

#include "mx_font.h"


namespace mx
{

		mxRegularFont::mxRegularFont()
		{
			font = 0;
		}



		mxRegularFont::mxRegularFont(const mxRegularFont &fontX)
		{
			setFont(fontX);

		}

		mxRegularFont &mxRegularFont::operator=(const mxRegularFont &fontX)
		{
			setFont(fontX);
			return *this;
		}

		void mxRegularFont::setFont(const mxRegularFont &fontX)
		{
			freeFont();
			font = fontX.font;
		}


		bool mxRegularFont::loadFont(std::string font_name)
		{
			font = SDL_InitFont(font_name.c_str());
			if(font == 0)
				throw mx::mxException<std::string>(" could not load font: " + font_name + "\n");
			return true;
		}


		void mxRegularFont::freeFont()
		{
			if(font != 0)
				SDL_FreeFont(font);

		}


		void mxRegularFont::printText(mxSurface &surface, unsigned int x, unsigned int y, unsigned int color, std::string text)
		{
			if(font == 0) return;
			SDL_PrintText(surface.getSurface(), font, x, y, color, text.c_str());
		}

		void mxRegularFont::printSizedText(mxSurface &surface,unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int color, std::string text)
		{
			if(font == 0) return;
			SDL_PrintTextScaled(surface.getSurface(), font, x,y,w,h,color,text.c_str());
		}

		int mxRegularFont::printTextWidth_64(int *depth, mxSurface &surface, int x, int y, int width, unsigned int color, const char *src) {
			SDL_PrintTextWidth(depth, surface.getSurface(), font, x, y, width, color, src);
			return *depth;
		}

		int mxRegularFont::printTextDepth(mxSurface &surface, unsigned int x, unsigned int y, unsigned int color, std::string text) {
			int  depth = 0;
			SDL_PrintTextDepth(&depth,surface.getSurface(), font,  x, y, color, text.c_str());
			return depth;
		}

}







