/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/





#ifndef _MXSEMAPHORE_H_X_X
#define _MXSEMAPHORE_H_X_X

#include "SDL.h"


namespace mx
{



	/*! \class mxSem */
	class mxSem {

	public:
		/*! constructor */
		explicit mxSem(unsigned int value);
		/*!  destructor */
		~mxSem();
		/*! currentValue */
		const unsigned int currentValue() const;
		/*! wait */
		int wait();
		/*! tryWait */
		int tryWait();
		/*! waitTimeout */
		int waitTimeout(unsigned int interval);
		/*! post   */
		int post();
	private:
		SDL_sem *sem;
	};














}


#endif


