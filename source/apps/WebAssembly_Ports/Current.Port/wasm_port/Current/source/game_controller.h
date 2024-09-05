#ifndef GAME_CONTROLLER_0
#define GAME_CONTROLLER_0

#include<mx.h>


namespace current {

	enum BUTTONS { LEFT, RIGHT, UP, DOWN, BTN_A, BTN_B, BTN_START };

	class gameController {
	public:
		gameController();
		~gameController();
		void event(SDL_Event &);
		void init() { stick = SDL_JoystickOpen(0); }
		bool pollController(BUTTONS button);
	private:
		SDL_Joystick *stick;
	};


}










#endif

