#ifndef __GAME_SCREEN_H_
#define __GAME_SCREEN_H_

#include<mx.h>
#include"game_controller.h"


namespace current {

	enum GAME_SCREEN { INTRO, START, GAME, GAME_OVER, SCORES };

	class gameScreen {
	public:
		gameScreen(mx::mxSurface &);
		virtual ~gameScreen();
		virtual void init() = 0;
		virtual void render() = 0;
		virtual void event() = 0;
		virtual void free() = 0;
		bool loadImage(mx::mxSurface &, const char *);

		mx::mxSurface &front;
	};

}






#endif
