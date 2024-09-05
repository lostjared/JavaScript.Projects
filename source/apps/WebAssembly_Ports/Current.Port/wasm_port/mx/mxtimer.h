/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/
#ifndef __MX_TIMERX_H_X
#define __MX_TIMERX_H_X

#include "SDL.h"

namespace mx
{
	unsigned int timer_exec(unsigned int val, void *param);
	unsigned int timer_execution(unsigned int val, void *param);
	/*! \class mxTimer_Exec
	 * timer base class
	 */
	class mxTimer_Exec {

	public:
		/*! virtual destructor */
		virtual ~mxTimer_Exec() { }
		/*! default constructor */
		mxTimer_Exec() { }
		/*! timerExecution pure virtual function
		 * \param x milliseconds interval
		 */
		virtual unsigned int timerExecution(unsigned int x) = 0;
	};
	/*! \class mxTimer timer base class */
	class mxTimer : public mxTimer_Exec {

	public:
		/*! timer constructor */
		mxTimer();
		/*! timer destructor */
		virtual ~mxTimer() { stopTimer(); }
		/*! setTimer sets the timer
		 * \param interval interval in milliseconds
		 */
		void setTimer(unsigned int interval);
		/*! stopTimer stops the timer
		 */
		void stopTimer();
	protected:
		SDL_TimerID tid;

	};
	/*! \class mxTimerObject timer template */
	template<typename Class>
	class mxTimerObject : public mxTimer_Exec {

	public:
		/*!  constructor
		 * \param ptr of type Class
		 * \param member function from Class
		 */
		mxTimerObject(Class *ptr, unsigned int (Class::*timer)(unsigned int mil))
		{
			ptr = cls;
			timer_func = timer;
			tid = 0;
		}

		/*! setTimer sets the timer
		 * \param interval interval in milliseconds
		 */
		void setTimer(unsigned int interval);
		/*! stopTimer stops the timer */
		void stopTimer();
		/*! timerExecution overrided virtual function
		 * \param x interval
		 */
		virtual unsigned int timerExecution(unsigned int x);
	protected:
		Class *cls;
		unsigned int (Class::*timer_func)(unsigned int mili);
		SDL_TimerID tid;
	};

	template<typename Class>
	void mxTimerObject<Class>::setTimer(unsigned int interval)
	{
		tid = SDL_AddTimer(interval, timer_execution, (void*)this);
	}


	template<typename Class>
	void mxTimerObject<Class>::stopTimer()
	{

		SDL_RemoveTimer(tid);
		tid = 0;

	}

	template<typename Class>
	unsigned int mxTimerObject<Class>::timerExecution(unsigned int x)
	{
		return (*cls.*timer_func)(x);
	}

}

#endif

