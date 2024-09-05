#include"main_window.h"



namespace current {


	mainWindow *mainWindow::window = 0;

	void mainWindow::eventPassed(SDL_Event &e) {
		if((e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) && current == &gamescr) {
			setScreen(&startscr);
		} else if(e.type == SDL_KEYDOWN && current == &onlinescr && e.key.keysym.sym == SDLK_ESCAPE) {
			setScreen(&startscr);
		} else if(e.type == SDL_KEYDOWN && current == &onlinescr) {
			onlinescr.keypress(e.key.keysym.sym, e.key.keysym.scancode);
		} else mxWnd::eventPassed(e);
	}

	void mainWindow::renderScreen() {
		current->render();
		current->event();
		front.Flip();
	}

	void mainWindow::setScreen(gameScreen *screen) {
		current = screen;
	}

}



