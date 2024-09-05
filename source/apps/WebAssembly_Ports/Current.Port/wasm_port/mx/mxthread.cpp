/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/


#include "mxthread.h"

namespace mx
{

	int thread_exec(void *ptr)
	{
		mxThread *thread_pointer = (mxThread*) ptr;
		return thread_pointer->threadExec();
	}

	int thread_execute(void *data)
	{

		mxExec *e = (mxExec*)data;
		return e->threadExec();
	}


	mxMutex::mxMutex()
	{
		m = SDL_CreateMutex();
	}

	mxMutex::~mxMutex()
	{
		if(m)
			SDL_DestroyMutex(m);
	}

	int mxMutex::lockMutex()
	{
		return SDL_mutexP(m);
	}

	int mxMutex::unlockMutex()
	{

		return SDL_mutexV(m);
	}

	mxThread::mxThread()
	{
		thread_identifier = 0;
	}

	mxThread::~mxThread()
	{
		threadStop();

	}

	void mxThread::threadRun()
	{
		thread_identifier = SDL_CreateThread(thread_exec, (void*)this);

	}

	void mxThread::threadStop()
	{

		if(thread_identifier != 0)
			SDL_KillThread(thread_identifier);

		thread_identifier = 0;

	}

	void mxThread::threadWait()
	{

		if(thread_identifier != 0)
			SDL_WaitThread(thread_identifier, 0);

	}






}



