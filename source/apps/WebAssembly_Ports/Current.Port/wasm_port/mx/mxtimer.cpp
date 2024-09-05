/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/


#include "mxtimer.h"





namespace mx
{


	unsigned int timer_exec(unsigned int val, void *param)
	{
		mxTimer *timer = (mxTimer*)param;
		return timer->timerExecution(val);
	}

	unsigned int timer_execution(unsigned int val, void *param)
	{

		mxTimer_Exec *e = (mxTimer_Exec*) param;
		return e->timerExecution(val);
	}


	mxTimer::mxTimer()
	{

		tid = 0;

	}

	void mxTimer::setTimer(unsigned int interval)
	{
		tid = SDL_AddTimer(interval, timer_exec, (void*)this);


	}

	void mxTimer::stopTimer()
	{

		if(tid != 0)
			SDL_RemoveTimer(tid);

		tid = 0;

	}





}











