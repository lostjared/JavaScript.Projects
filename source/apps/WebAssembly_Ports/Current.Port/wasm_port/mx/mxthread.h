/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/



#ifndef __MX_THREAD_H__
#define __MX_THREAD_H__

#include "SDL.h"
#include "SDL_thread.h"

namespace mx
{


	int thread_exec(void *data);
	int thread_execute(void *data);

	/*! \class mxMutex
	 *  encapsulation of SDL_mutex
	 */
	class mxMutex {
	public:
		/*! constructor */
		mxMutex();
		~mxMutex();
		/*! lockMutex locks the mutex */
		int lockMutex();
		/*! unlockMutex unlocks the mutex */
		int unlockMutex();
	protected:
		SDL_mutex *m;
	private: // no copy constructor or operator=() allowed
		mxMutex(const mxMutex &);
		mxMutex &operator=(const mxMutex &);

	};

	/*! \class mxExec abstract base class for thread execution
	 *
	 */
	class mxExec {

	public:
		/*! default constructor */
		mxExec() { }
		virtual ~mxExec() { }
		/*! threadExec
		 * thread function */
		virtual int threadExec() = 0;

	};

	/*! \class  mxThread  thread base class */
	class mxThread : public mxExec {

	public:
		/*!  constructor */
		mxThread();

		virtual ~mxThread();
		/*! start thread's execution */
		void threadRun();
		virtual int threadExec() = 0;
		/*! stop thread's execution */
		void threadStop();
		/*! wait for thread to finish */
		void threadWait();
	protected:
		SDL_Thread *thread_identifier;
	};


	/*! \class mxThreadObject */

	template<typename Class, typename Parameter>
	class mxThreadObject : public mxExec {

	public:

		/*! constructor */
		mxThreadObject(Class *ptr, int (Class::*f)(Parameter p))
		{
			cls = ptr;
			func = f;
			thread_identifier = 0;
		}

		~mxThreadObject();

		/*! threadRun with Parameter
		 * \param p paramter to pass to thread
		 */
		void threadRun(Parameter p);
		/*! threadStop stop thread
		 *
		 */
		void threadStop();
		/*! threadWait wait for thread    */
		void threadWait();
		/*! threadExec */
		virtual int threadExec();
		/*! isRunning is thread running? */
		const bool isRunning() const { return running; }
	protected:
		bool running;
		int (Class::*func)(Parameter p);
		Class *cls;
		SDL_Thread *thread_identifier;
		Parameter code_param;

	};
	template<typename Class, typename Parameter>
	mxThreadObject<Class,Parameter>::~mxThreadObject()
	{

		if(thread_identifier != 0)
			SDL_KillThread(thread_identifier);

		thread_identifier = 0;
	}
	template<typename Class, typename Parameter>
	void mxThreadObject<Class,Parameter>::threadRun(Parameter p)
	{

		code_param = p;
		running = true;
		thread_identifier = SDL_CreateThread(thread_execute, (void*)this);

	}
	template<typename Class, typename Parameter>
	int mxThreadObject<Class,Parameter>::threadExec()
	{

		if(cls == 0) return 0; // if for some reason you must delete the class that this
				      //  function object points to make sure to set it to zero
				     //  otherwise a segmentation fault will occour
		int rt = (*cls.*func)(code_param);
		running = false;
		return rt;
	}
	template<typename Class, typename Parameter>
	void mxThreadObject<Class,Parameter>::threadWait()
	{
		if(thread_identifier != 0)
			SDL_WaitThread(thread_identifier, 0);
	}
	template<typename Class, typename Parameter>
	void mxThreadObject<Class,Parameter>::threadStop()
	{
		if(thread_identifier != 0)
			SDL_KillThread(thread_identifier);

		thread_identifier = 0;
	}
}
#endif

