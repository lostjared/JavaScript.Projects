/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/




#ifndef __MX_FUNC_HX_
#define __MX_FUNC_HX_

#include"SDL.h"
#include"SDL_loadso.h"
#include<string>



namespace mx
{
	/*! \class mxShared class for loading shared modules
	 */
	class mxShared {

	public:
		/*! default constructor for class
		 * \param module_name module to load, .so, .dll, .dylib
		 */
		mxShared(std::string module_name = "");
		/*! destructor */
		~mxShared();

		/*! load a module by filename
		 * \param name filename of module to load
		 * \return true if successful
		 */
		bool loadModule(std::string name);
		/*! unload a module
		 * \return true if successful
		 * */
		bool unloadModule();

		/*! load a function by function name id
		 *  \param name function name
		 *  \return function pointer for function zero if not found or error
		 */
		void *loadFunction(std::string name);
	protected:
		void *ptr;

	private: // do not allow copy constructor or operator=()
		mxShared(const mxShared &);
		mxShared &operator=(const mxShared &);
	};


	// a function object for a function pointer to a member function of a class
	template<typename Class, typename Parameter, typename Return>
	/*! \class mxFunctor  member class function pointer template */
	class mxFunctor {

		Return (Class::*func)(Parameter p);
		Class *cls;
	public:
		/*! mxFunctor constructor
		 * \param cls class pointer to class of function pointer
		 * \param functor member function pointer
		 */
		explicit mxFunctor(Class *cls, Return (Class::*functor)(Parameter p))
		{
			this->cls = cls;
			func = functor;
		}
		/*! overloaded operator () for calling member function passed in on constructor
		 * \param p parameter to pass to mem function
		 * \return data returned by member function pointer
		 */
		Return operator()(Parameter p)
		{
			return (*cls.*func)(p);
		}
		/*! Function for calling member function passed in on constructor
		* \param p parameter to pass to mem function
		* \return data returned by member function pointer
		*/
		Return CallFunction(Parameter p)
		{
			return (*cls.*func)(p);
		}
	private:
		mxFunctor(const mxFunctor<Class, Parameter, Return> &f);
		mxFunctor<Class,Parameter,Return> &operator=(const mxFunctor<Class,Parameter,Return> &p);
	};

}



#endif


