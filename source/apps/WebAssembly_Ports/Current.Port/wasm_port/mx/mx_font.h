/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/


#ifndef __MXFONTZ_H_X_
#define __MXFONTZ_H_X_

#include<string>
#include"mxf.h"
#include"mxsurface.h"
#include"mx_exception.h"
#include"mx_types.h"

/*! \namespace mx namespace for libmx */
namespace mx
{

	/*! \class mxRegularFont class for encapsulating handling mxFonts. */
	class mxRegularFont {
	public:
		/*! default constructor */
		mxRegularFont();
		/*! copy constructor
		 * \param font parameter to copy from
		 */
		mxRegularFont(const mxRegularFont &font);

		/*! overloaded operator = for assigning another font
		 *  \param font font to assign from
		 *  \return to allow operator chaining a mxRegularFont
		 */
		mxRegularFont &operator=(const mxRegularFont &font);

		/*! setFont set another font to this font, same as overloaded operator = but as a method
		 * \param font takes a font to assign to this object
		 */
		void setFont(const mxRegularFont &font);
		/*! load font from string on disk
		 *
		 * \param font_name the name of the mxFont in std::string form.
		 * \return boolean value for true if successful load, false on failure.
		 */
		bool loadFont(std::string font_name);
		/*!
		 * freeFont release this font from memory
		 */
		void freeFont();

		/*! getFont
		 *  \return return the font encapsulated by this object in its true form as a SDL_Font structure
		 */

		SDL_Font *getFont() { return font; }
		/*! operator()*
		 *  \return return the font encapsulated by this object in its true form as a SDL_Font structure
		 */
		SDL_Font *operator*() { return font; }

		/*! printText
		 *  \param surface surface to print to
		 *  \param x x coordinate in pixels
		 *  \param y y coordinate in pixels
		 *  \param color color value in 32bit RGBA
		 *  \param text string to print to surface
		 */
		void printText(mxSurface &surface, unsigned int x, unsigned int y, unsigned int color, std::string text);
		/*! printText overloaded method
		 * \param surface to print string to
		 * \param p Point object 2d coordinate
		 * \param color Color object
		 * \param text text to print to surface
		 */
		void printText(mxSurface &surface, Point p, Color color, std::string text) {
			printText(surface, p.x, p.y, Color::mapRGB(surface, color), text);
		}
		/*! printSizedText - print text stretched to a specific size
		 * \param surface to print to
		 * \param x x coordinate
		 * \param y y coordinate
		 * \param size_w width in pixels
		 * \param size_h height in pixels
		 * \param color 32bit RGBA value
		 * \param text string value
		 */
		void printSizedText(mxSurface &surface, unsigned int x, unsigned int y, unsigned int size_w, unsigned int size_h, unsigned int color, std::string text);

		/*! printTextWidth_64 - print text with coordinate offset returned in depth variable, used for getting the offset of console height in masterx project
		 * \param depth pointer to integer to contain coordinate
		 * \param surface surface to draw to
		 * \param x x coordinate in pixels
		 * \param y y coordinate in pixels
		 * \param size_width width in pixels
		 * \param color 32bit RGBA value
		 * \param src text to print to screen
		 */
		int printTextWidth_64(int *depth, mxSurface &surface, int x, int y, int size_width, unsigned int color , const char *src);

		/*! printTextDepth - print text with coordinate offset returned by the function
		 * \param surface surface to draw to
		 * \param x x coordinate
		 * \param y y coordinate
		 * \param color 32bit RGBA value
		 * \param text string to draw to surface
		 */

		int printTextDepth(mxSurface &surface, unsigned int x, unsigned int y, unsigned int color, std::string text);

	protected:

		/*! the SDL_Font structure to hold are font */
		struct SDL_Font *font;
	};
}



#endif

