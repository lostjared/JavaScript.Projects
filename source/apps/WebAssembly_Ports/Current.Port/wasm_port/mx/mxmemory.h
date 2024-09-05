/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/


#ifndef _MX_MEMORY_H_
#define _MX_MEMORY_H_

#include<cstdio>
#include<cstdlib>
#include<vector>
#include<cstring>


namespace mx
{
	/*!
	 * \class scoped_memory
	 * a scoped memory pool
	 */
	class scoped_memory {
	public:
		/*! scoped memory pool constructor */
		scoped_memory() { }
		/*! scoped_memory pool destructor */
		virtual ~scoped_memory();

		/*! alloc data on the heap of len bytes
		 * \param len size of memory chunk in bytes
		 * \return void * to memory allocated
		 */
		virtual void *alloc(size_t len);
		/*! sized alloc (calloc)
		 *
		 * \param num number of objects
		 * \param size size of object
		 * \return void * to memory allocated
		 */
		virtual void *sized_alloc(size_t num, size_t size);
		/*! release this memory pool's allocated resources */
		virtual void release();
	protected:
		/*! allocated pointers */
		std::vector<void *> allocations;
	private: // no copying allowed
		scoped_memory(const scoped_memory &);
		scoped_memory &operator=(const scoped_memory &);
	};
}















#endif


