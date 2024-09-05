/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/

#include "mxfunction.h"



namespace mx
{

	mxShared::mxShared(std::string module)
	{
		ptr = 0;

		if(module.length()>0)
			loadModule(module);

	}


	mxShared::~mxShared()
	{

		unloadModule();

	}

	bool mxShared::loadModule(std::string module)
	{

		ptr = SDL_LoadObject(module.c_str());
		if(ptr == 0) return false;

		return true;
	}


	bool mxShared::unloadModule()
	{

		if(ptr != 0)
			SDL_UnloadObject(ptr);

		ptr = 0;

		return true;
	}

	void *mxShared::loadFunction(std::string name)
	{

		if(ptr == 0) return 0;

		return SDL_LoadFunction(ptr, name.c_str());

	}


}

