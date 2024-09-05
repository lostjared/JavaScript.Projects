#include "game_controller.h"
#include"SDL/SDL.h"

namespace current {




	gameController::gameController() {
		stick = SDL_JoystickOpen(0);
	}

	gameController::~gameController() {
		if(stick != 0)
			SDL_JoystickClose(stick);

		stick = 0;
	}


	bool gameController::pollController(BUTTONS button) {

		const Uint8 *keyState = SDL_GetKeyboardState(0);

		switch(button) {
		case LEFT:
			if(keyState[SDLK_LEFT])
				return true;
			if(stick != 0 && (SDL_JoystickGetHat(stick, 0) & SDL_HAT_LEFT))
				return true;
			break;
		case RIGHT:
			if(keyState[SDLK_RIGHT])
				return true;
			if(stick != 0 && (SDL_JoystickGetHat(stick, 0) & SDL_HAT_RIGHT))
				return true;
			break;
		case UP:
			if(keyState[SDLK_UP])
				return true;
			if(stick != 0 && (SDL_JoystickGetHat(stick, 0) & SDL_HAT_UP))
				return true;
			break;
		case DOWN:
			if(keyState[SDLK_DOWN])
				return true;
			if(stick != 0 && (SDL_JoystickGetHat(stick, 0) & SDL_HAT_DOWN))
				return true;
			break;
		case BTN_A:
			if(keyState[SDLK_a])
				return true;
			if(stick != 0 && SDL_JoystickGetButton(stick, 1))
				return true;
			break;
		case BTN_B:
			if(keyState[SDLK_s])
				return true;
			if(stick != 0 && SDL_JoystickGetButton(stick, 2))
				return true;
			break;
		case BTN_START:
			if(keyState[SDLK_RETURN])
				return true;
			if(stick != 0 && SDL_JoystickGetButton(stick, 0))
				return true;
		}
		return false;
	}


}

