/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/




#ifndef __MX_JOYSTICK_H_
#define __MX_JOYSTICK_H_


#include "SDL.h"
#include<string>


namespace mx
{

	/*! \class mxJoystick class to encapsulate SDL_Joystick * */
	class mxJoystick {

	public:
		/*! default constructor */
		mxJoystick(int joy_index=0);
		/*! constructor
		 * \param already opened Joystick
		 * */
		mxJoystick(SDL_Joystick *stick);
		/*! destructor */
		~mxJoystick();
		/*! initJoystick initialize joystick */
		void initJoystick(int id);
		/*! freeJoystick free the Joystick */
		void freeJoystick();
		/*! isOpen is Joystick already Open? */
		const bool isOpen() const { return SDL_JoystickOpened(index) ? true : false; }
		/*! joystickName \return Joystick Name */
		const std::string joystickName() const { return SDL_JoystickName(index); }
		/*! joystickGetButton get if button is down
		 * \param button  which button
		 * \return boolean for if  button is down
		 */
		const bool joystickGetButton(int button) const;
		/*! joystickGetAxis get axis position
		 * \param axis which axis
		 * \return axis position
		 */
		const int  joystickGetAxis(int axis) const;
		/*! joystickGetHat get position of hat
		 * \param hat which hat
		 * \return position of hat
		 */
		const int  joystickGetHat(int hat) const;
		/*! joystickGetBall
		 * \param ball which ball
		 * \param x x pos
		 * \param y y pos
		 */
		const int  joystickGetBall(int ball, int &x, int &y);

		/*! joystickNum
		 * \return number of Joystick's
		 */
		static const int joystickNum() { return SDL_NumJoysticks(); }
		/*!  joystickUpdate update joystick's
		 */
		static const void joystickUpdate() { SDL_JoystickUpdate(); }

	protected:
		/*! SDL_Joystick * data*/
		SDL_Joystick *stick;
		/*! current joystick index */
		int index;
	private: // no copy constructor or operator= allowed
		mxJoystick(const mxJoystick &j);
		mxJoystick &operator=(const mxJoystick &j);

	};
}
#endif

