/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/





#ifndef __MXE__H__
#define __MXE__H__

#include <iostream>
#include <string>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>


/*!

	\namespace mx namespace for libmx

*/
namespace mx
{



	using std::ostream;
	using std::string;

	/*!
	 *  \class mxException, class used for internal exception handling
	 */
	template<typename Type>
	class mxException {

	public:
		/*!
		 * constructor takes param of type T
		 * \param t of Type
		 */
		explicit mxException(Type t)
		{
			data = t;
		}

		/*!
		 *
		 * printError prints error to a ostream stream
		 * \param out ostream object that error of type Type is streamed to
		 */
		const void printError(ostream &out) const
		{

			out << data << "\n";
		}

		/*!
		 * data of type Type
		 *
		 */
		Type data;
	};
}

#endif
