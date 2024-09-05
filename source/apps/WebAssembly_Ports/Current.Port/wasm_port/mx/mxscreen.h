/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/


#ifndef __MX_SCREEN_H__X
#define __MX_SCREEN_H__X
#include "mxsurface.h"
#include<string>
#include<sstream>

namespace mx
{


	// abstract base class, can only be inherited.
	/*! \class mxScreen
	 *  abstract class used for dividing the logic of fullscreen applications into sections
	 */
	class mxScreen {

	public:
		explicit mxScreen(std::string path, mxSurface &rsurface) : front(rsurface), pathvar(path) {  }
		virtual ~mxScreen() { }
		virtual void init() = 0;
		virtual void free() = 0;
		virtual void render() = 0;
		virtual int logic() = 0;
		void loadSurface(mx::mxSurface &surf, std::string filename);
	protected:
		mxSurface &front;
		std::string pathvar;

	};
}


#endif
