/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/

#include "mxjoystick.h"

#include<iostream>


namespace mx
{

	mxJoystick::mxJoystick(int id)
	{
		stick = 0;

		initJoystick(id);

	}

	mxJoystick::mxJoystick(SDL_Joystick *stick)
	{

		if(this->stick != 0)
		{
			freeJoystick();
		}

		this->stick = stick;
		index = SDL_JoystickIndex(this->stick);
	}


	mxJoystick::~mxJoystick() // no-throw guarantee
	{
		freeJoystick();
	}


	void mxJoystick::initJoystick(int id)
	{

		stick = SDL_JoystickOpen(id);
		SDL_JoystickEventState(SDL_ENABLE);

		if(stick == 0)
		{
			std::cout << "could not open joystick id: " << id << "\n";
		}

		index = id;

	}


	void mxJoystick::freeJoystick()
	{
		SDL_JoystickClose(stick);
		stick = 0;
	}

	const bool mxJoystick::joystickGetButton(int button) const
	{
		return SDL_JoystickGetButton(stick, button) ? true : false;
	}

	const int mxJoystick::joystickGetAxis(int axis) const
	{
		return static_cast<int>(SDL_JoystickGetAxis(stick, axis));
	}

	const int mxJoystick::joystickGetHat(int hat) const
	{
		return static_cast<int>(SDL_JoystickGetHat(stick, hat));
	}

	const int mxJoystick::joystickGetBall(int ball, int &x, int &y)
	{
		return static_cast<int>( SDL_JoystickGetBall(stick, ball, &x, &y) );
	}

}



