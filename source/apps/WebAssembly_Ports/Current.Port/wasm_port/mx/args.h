/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/


#ifndef __ARGS__H_
#define __ARGS__H_

#include<string>
#include<vector>
#include<iostream>

namespace mx
{
	using std::string;
	/*! \class mxArgs class to proccess arguments passed on the command line */
	class mxArgs {
	public:
		/*! mxArgs explicit constructor, is required to init this class
		 *	\param argc the argc ( int ) first parameter from the main function
		 *  \param argv the argv ( char ** ) second parameter from the main function
		 * */
		explicit mxArgs(int &argc, char **argv);
		/*! testForSwitchAndRemove test internal object created for a specific string and remove it
		 * from the object so it cannot be found again
		 * \param what string to search for
		 * \return boolean value specifying whether string exists
		 */
		bool testForSwitchAndRemove(string what);
		/*! testForSwitchAndLeave test internal object created for a specific string and leave it there
		 * \param what string to search for
		 * \return boolean value for if string was found
		 */
		bool testForSwitchAndLeave(string what);
		/*! testForItem tests internal object for a special type of identifier which has a string right after or a assignment statement
		 * \param what variable for value to be assigned to
		 * \return the string value, empty string if not found
		 */
		string testForItem(string what);
	private:
		/*! vector for holding the strings */
		std::vector<std::string> arglist;
	};

}


#endif

