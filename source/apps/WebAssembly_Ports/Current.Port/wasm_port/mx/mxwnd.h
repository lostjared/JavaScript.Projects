/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/


#ifndef __MXWND__H__
#define __MXWND__H__

#include"SDL/SDL.h"
#include "mxsurface.h"
#include "mx_types.h"
#ifndef NOGL
#include"SDL_opengl.h"
#endif
#include "mximage.h"

#ifndef SDL_NUM_SCANCODES
#define SDL_NUM_SCANCODES 0xFF
#endif


#define STATIC_MAIN(func) int main(int argc, char **argv) { return func (argc, argv); }

namespace mx {

	
	extern Uint8 keyState[SDL_NUM_SCANCODES];
	/*! \enum lookType for view Type of OpenGL projection
	 *
	 */
	enum lookType{ LOOKAT, ORTHO };

	/*! \class mxWnd the mx Window class, this class is what you inhert from to create graphical applications with libmx, supports 2D and 3D rendering */
	class mxWnd {
		

		void procEvent();
		static void eventMessage();

		/*! Default Constructor has default arguments for each argument if you do not wish to supply any
		 *  \param w the width of the window
		 *  \param h the height of the window
		 *  \param flags contains the SDL window flags, SDL_FULLSCREEN for full screen windows etc
		 *  \param ico the Bitmap for the windows icon
		 *  \param opengl_init for whether or not to init this window as a OpenGL window
		 */
		mxWnd(int w=640, int h=480, Uint32 flags=0, SDL_Surface *ico = 0,  bool opengl_init=false);
		/*! Secondary Constructor
		 *  \param window the size of the window as a Size object
		 *  \param canvas size for stretching to window size, example window size is 640x480 canvas size is 480x272 canvas size will be stretched to screen size before being rendered
		 *  \param flags SDL SetVideoMode Flags
		 *	\param ico the bitmap for the Icon of the Window
		 */
		mxWnd( Size window, Size canvas,  Uint32 flags = 0, SDL_Surface *ico = 0 );
		/*! initWnd this function is for if you wish to reinitalize the application window, it is what the constructor calls to set the video mode
		 * \param w the windows width in pixels
		 * \param h the windows height in pixels
		 * \param flags the SDL flags for SetVideoMode
		 * \param ico the icon bitmap
		 * \param opengl_init whether or not to init OpenGL
		 */
		void initWnd(int w, int h, Uint32 flags, SDL_Surface *ico, bool opengl_init);
		/*! setMode, sets the current Projects for OpenGL windows
		 *  \param type lookType of current projection
		 */
		void setMode(lookType type);
		/*! Virtual destructor to cleanup after the object leaves scope */
		virtual ~mxWnd();
		/*! messageLoop this function is called when the app is ready to enter its messageLoop
		 * \return application exit code (success is 0 or failure exit code)
		 */
		int messageLoop();
		/*! virtual function for event processing
		 *  overload this function in your subclass and the app's messages will be propagated to this function at the correct times.
		 *  \param e SDL_Event event structure http://libsdl.org for more info
		 */
		virtual void eventPassed(SDL_Event &e);
		/*! virtual function for idle rendering
		 *  overload this function in your subclass and the app will call this function when it has time to draw to the screen
		 */
		virtual void renderScreen();
		/*! setTitle set the window's title string
		 * \param title a string containing the new title of the window
		 */
		void setTitle(string title) const;
		/*! quit exit this application freeing all resources */
		void quit();
		/*! minimize this window */
		void minimizeWindow() const;
		/*! push the event pump forward a notch */
		void pumpEvents() const;
		/*! enable key repeat for when holding down a key to a specific interval
		 * \param code  code
		 * \param interval time
		 * */
		void enableKeyRepeat(int code, int interval);
		/*! return the SDL_Surface* for this window
		 * \return this app's front SDL_Surface*
		 */
		SDL_Surface *renderSurface() const { return front.getSurface(); }
		/*! return the app's mxSurface for the front main surface
		 * \return mxSurface for front surface
		 */
		mxSurface &mxrenderSurface() { return front; }
		/*! return this window's width and height as a Size object
		 * \return window's Size
		 */
		Size &size();
		/*! return this app's background sized surface for when init with Secondary constructor with Canvas parameter
		 * \return sized background surface
		 */
		mxSurface &sizedSurface() { return background; }
		/*! sized Flip */
		void sizedFlip();
		/*! create a sized background by Size (width,height)
		 * \param s a Size object for width/height
		 *  */
		void createSizedBackground ( Size &s );
		/*! resize the Window, virtual can be overloaded for custom perspectives
		 * \param w width in pixels
		 * \param h height in pixels
		 */
		virtual void resizeWindow(int w, int h);
	protected:
		/*! front surface object */
		mxSurface front;
		/*! background sized surface */
		mxSurface background;
		/*! active flag */
		bool active;
		/*! video flags for current state of the window */
		Uint32 vid_flags;
		/*! size of window */
		Size sizevar;
		/*! the main window */
		static mxWnd *mxwnd;
		/*! mainWindow static get method
		 * \return returns last initialised mxwnd
		 */
		static mxWnd *get_MX() { return mxwnd; } // static function to return last initialised mxWnd
		/*! holds state of OpenGL init */
		bool opengl_;
		/*! holds current perspective */
		lookType d_type;

	private:
		int err_Type;
	};
}

#endif


