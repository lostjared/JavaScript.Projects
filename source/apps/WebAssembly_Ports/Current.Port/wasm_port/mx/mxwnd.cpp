/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/


#include "mxwnd.h"
#include<ctime>

#ifndef DNOGL
#include"SDL_opengl.h"
#endif
#if __APPLE__
#include<OpenGL/gl.h>
#include<OpenGL/glu.h>
#else
#include<GL/gl.h>
#include<GL/glu.h>
#endif

#include<emscripten/emscripten.h>

namespace mx
{

	Uint8 keyState[SDL_NUM_SCANCODES];
	mxWnd *mxWnd::mxwnd = 0;



	mxWnd::mxWnd( Size window,  Size s, Uint32 flags, SDL_Surface *ico)
	{
		initWnd(window.width,window.height,flags,ico,false);
		createSizedBackground( s );
	}


	mxWnd::mxWnd(int w, int h, Uint32 flags, SDL_Surface *ico, bool opengl_init)
	{
		initWnd(w,h,flags,ico,opengl_init);
	}

	void mxWnd::initWnd(int w, int h, Uint32 flags, SDL_Surface *ico, bool opengl_init)
	{
		err_Type = 0;
		active = false;
		mxwnd = this;
		if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK) < 0) {
			err_Type = 1;
			return;
		}

			if(ico != 0)
				SDL_WM_SetIcon(ico, 0);

			opengl_ = false;

			const SDL_VideoInfo *vi = SDL_GetVideoInfo();

			Uint8 bpp=0;

			bpp = vi->vfmt->BitsPerPixel;

			bool hw_ = true;

			if(vi->hw_available != 0)
				flags |= SDL_HWSURFACE;
			else {
				flags |= SDL_SWSURFACE;
				hw_ = false;
			}

			if(vi->blit_hw)
				flags |= SDL_HWACCEL;

#ifndef NOGL
			if(opengl_init == true)
			{
				SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
				d_type = ORTHO;
			}
#endif


			if(w == -1)
			{
				w = vi->current_w;
				h = vi->current_h;
				flags |= SDL_FULLSCREEN;
			}

			if(opengl_init == true) {
				flags |= SDL_OPENGL;
				opengl_ = true;
			}
			// set a icon here

			SDL_Surface *temp = SDL_SetVideoMode(w,h,32,flags);

			if(!temp) {
				err_Type = 2;
				return;
			}

			front = temp;
			vid_flags = flags;
			front.noZero(true); // no zero here , because the surface is freed by the call to SDL Quit



			std::cout << "application activated @ ";

			time_t the_time;
			struct tm *time_info;

			time(&the_time);

			time_info = localtime(&the_time);
			std::cout << asctime(time_info);
			std::cout << "SDL: subsystem initalized\n";
			std::cout << "Video " << front.width() << "x" << front.height() << "x" << int(front.getSurface()->format->BitsPerPixel) << "\n";
			std::cout << "Render Mode: " <<  ((hw_ == true) ? "Direct Rendering" : "Software Rendering")   << "\n";
			std::cout << "Hardware Blit Acceleration: " << ((vi->blit_hw == 0) ? "No": "Yes") << "\n";
			sizevar = Size(front.width(), front.height());
	}

	void mxWnd::createSizedBackground ( Size &s )
	{
		background.createSurface( s );
	}

	mxWnd::~mxWnd()
	{

		SDL_Quit();
		std::cout << "application shutdown @ ";
		time_t the_time;
		struct tm *time_info;
		time(&the_time);
		time_info = localtime(&the_time);
		std::cout << asctime(time_info);
		std::cout << "SDL: subsystem shutdown\n";

	}


	void mxWnd::procEvent() {

			static SDL_Event e;

			while(SDL_PollEvent(&e))
			{


				switch(e.type) {
					case SDL_VIDEORESIZE:
					{
						const SDL_VideoInfo *vi = SDL_GetVideoInfo();
						Uint8 bpp = vi->vfmt->BitsPerPixel;
						front = SDL_SetVideoMode( e.resize.w, e.resize.h, bpp, vid_flags );
						resizeWindow(e.resize.w, e.resize.h);

					}
					break;
					case SDL_KEYDOWN:
						keyState[e.key.keysym.scancode] = 1;
						break;
					case SDL_KEYUP:
						keyState[e.key.keysym.scancode] = 0;
						break;
				}


				eventPassed(e);
			}

			renderScreen();

	}

	void mxWnd::eventMessage() {
		mxwnd->procEvent();
	}

	int mxWnd::messageLoop()
	{

		if(err_Type != 0) {

			switch(err_Type) {
			case 1:
				throw mx::mxException<std::string>("Error could not init SDL: " + std::string(SDL_GetError()) + "\n");
				break;
			case 2:
				throw mx::mxException<std::string>("Error could not set video mode: " + std::string(SDL_GetError()) + "\n");
				break;
			}

		}

		static SDL_Event e;

		active = true;

		emscripten_set_main_loop(mxWnd::eventMessage, 0, 1);

		return EXIT_SUCCESS;
	}

	void mxWnd::quit()
	{
		active = false;
		std::cout << "System: A Quit Signal has been sent\n\n";
	}


	void mxWnd::eventPassed(SDL_Event &e)
	{
		if(e.type == SDL_QUIT) active = false;
		else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) active = false;
	}



	void mxWnd::renderScreen()
	{

		SDL_FillRect(front, 0, SDL_MapRGB(front.getSurface()->format, 255, 255, 255));
		front.Flip();

#ifndef NOGL
		if(opengl_) {

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			SDL_GL_SwapBuffers();
		}
#endif

	}

	void mxWnd::setTitle(string title) const
	{
		SDL_WM_SetCaption(title.c_str(), 0);
	}

	void mxWnd::minimizeWindow() const
	{
		SDL_WM_IconifyWindow();
	}

	void mxWnd::pumpEvents() const
	{
		SDL_PumpEvents();
	}


	void mxWnd::enableKeyRepeat(int code, int interval)
	{
		SDL_EnableKeyRepeat(code, interval);
	}

	Size &mxWnd::size()
	{

		return sizevar;

	}

	void mxWnd::sizedFlip() {
		front.copyResizeSurface( background );
		front.Flip();
	}

	void mxWnd::resizeWindow(int w, int h)
	{
		if(opengl_ == false) return;


#ifndef NOGL
		if(d_type == ORTHO) {

			GLfloat range = 100.0f;

			if(h == 0) h = 1;

			glViewport(0,0,w,h);

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

		}
		else {

			if(h == 0)
					h = 1;

			GLfloat ratio = 0.0f;
			ratio = static_cast<GLfloat>( w / h );
			glViewport( 0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(45.0f, ratio, 0.1f, 200.0f);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
		}
#endif

	}

	void mxWnd::setMode(enum lookType type) {
		this->d_type = type;
	}

}



