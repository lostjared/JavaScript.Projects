#ifndef _CUR_MAIN_WINDOW_
#define _CUR_MAIN_WINDOW_
#include<mx.h>
#include"game_screen.h"
#include"current_game.h"

namespace current {
	class mainWindow : public mx::mxWnd {
	public:
		static mainWindow *window;
		mainWindow() : mxWnd(1024, 768, 0), controller(), gamescr(controller, front), overscr(controller, front), startscr(controller,front),
		onlinescr(controller, front), options(controller, front) {
			window = this;
			setTitle("Current");
			setScreen(&startscr);
			controller.init();
		}
		void setScreen(gameScreen *);
		virtual void eventPassed(SDL_Event &);
		virtual void renderScreen();
	protected:
		gameScreen *current;
		gameController controller;
	public:
		gScreen gamescr;
		gameOver_Screen overscr;
		start_Screen startscr;
		online_Screen onlinescr;
		options_Screen options;
	};
}

#endif


