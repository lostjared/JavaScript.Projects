/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/



#include "mxmemory.h"


namespace mx
{


	scoped_memory::~scoped_memory() {
		release();
	}

	void scoped_memory::release() {
		std::vector<void *>::iterator i, e = allocations.end();
		for(i = allocations.begin(); i != e; i++) {
			void *ptr = *i;
			free(ptr);
		}
		allocations.clear();
	}

	void *scoped_memory::alloc(size_t bytes) {
		void *buf = malloc(bytes);
		memset(buf, 0, bytes);
		allocations.push_back(buf);
		return buf;
	}

	void *scoped_memory::sized_alloc(size_t num, size_t size) {
		void *buf = calloc(num, size);
		allocations.push_back(buf);
		return buf;
	}


}















