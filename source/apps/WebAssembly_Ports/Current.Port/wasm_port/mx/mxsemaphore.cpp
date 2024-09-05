/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/


#include "mxsemaphore.h"

// simple wrapper around SDL functions :(


namespace mx
{

	mxSem::mxSem(unsigned int value)
	{

		sem = SDL_CreateSemaphore(value);


	}

	mxSem::~mxSem()
	{

		if(sem != 0)
			SDL_DestroySemaphore(sem);

	}

	const unsigned int mxSem::currentValue() const
	{

		return SDL_SemValue(sem);

	}

	int mxSem::wait()
	{

		return SDL_SemWait(sem);

	}

	int mxSem::tryWait()
	{

		return SDL_SemTryWait(sem);

	}

	int mxSem::waitTimeout(unsigned int interval)
	{

		return SDL_SemWaitTimeout(sem, interval);

	}

	int mxSem::post()
	{
		return SDL_SemPost(sem);
	}
}


