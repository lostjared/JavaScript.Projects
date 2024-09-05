#ifdef HAVE_CONFIG_H
#include<config.h>
#endif

#if (MX_TTF == 1)
#include"mxttf.h"
#include"mx_exception.h"


namespace mx
{

	mxTTF_Font::mxTTF_Font(std::string name, int size) {
		font = TTF_OpenFont(name.c_str(), size);
		if(!font) font = 0;

	}

	mxTTF_Font::~mxTTF_Font() {
		if(font)
		TTF_CloseFont(font);
	}

	SDL_Surface *mxTTF_Font::renderText(std::string text, SDL_Color col) {

		if(!font)
			throw mx::mxException<std::string>("Error font not loaded: \n");

		SDL_Surface *surf = TTF_RenderText_Solid(font, text.c_str(), col);
		return surf;
	}

	void mxTTF_Font::renderText(mxSurface &surface, std::string text, SDL_Color col) {

		if(!font)
			throw mx::mxException<std::string>("Error font not loaded mxTTF_Font::renderText(string, SDL_Color)\n");

			surface = TTF_RenderText_Solid(font, text.c_str(), col);
	}
}





#endif

