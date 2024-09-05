#ifndef MX_TTF_H_
#define MX_TTF_H_
#include "SDL.h"
#include "SDL_ttf.h"
#include<string>
#include"mxsurface.h"

namespace mx
{

	/*! \class mxTTF_Font wrapper for TTF_Font */
	class mxTTF_Font {

	public:
		/*! mxTTF_Font explicit constructor
		 * \param font_name name of the true type font to be loaded
		 * \param font_size the size of the font to be loaded
		 */
		explicit mxTTF_Font(std::string font_name, int font_size);
		/*! destructor
		 */
		~mxTTF_Font();

		/*! renderText
		 * \param text string of characters to be printed onto a Surface
		 * \param col Color of text
		 * \return SDL_Surface * of text of color col
		 */
		SDL_Surface *renderText(std::string text, SDL_Color col);

		/*!
		 * renderText
		 * \param surface mx::mxSurface to hold surface of rendered text
		 * \param col the color of the text
		 */
		void renderText(mxSurface &surface, std::string text, SDL_Color col);


		/*! Init setup the SDL_ttf library */
		static void Init() {
			if(!TTF_WasInit())
				TTF_Init();
		}
		/*! Quit the SDL_ttf library */
		static void Quit() {
			TTF_Quit();
		}
	private:
		TTF_Font *font;
		mxTTF_Font &operator=(const mxTTF_Font &);
		mxTTF_Font(const mxTTF_Font &);
	};
}
#endif


