#ifndef __CURRENT_GAME__H_
#define __CURRENT_GAME__H_

#include"game_screen.h"
#include"current.h"
#include<mxsocket.h>


namespace current {


	/* these screens are tightly coupled to one another */
	extern mx::mxRegularFont font;

	class gScreen : public gameScreen, public mx::mxTimer {

	public:

		enum { S_W=800/16, S_H=600/16 };

		gScreen(gameController &c, mx::mxSurface &front);
		~gScreen();
		virtual void event();
		virtual void render();
		virtual void init();
		virtual void free();
		virtual unsigned int timerExecution(unsigned int);
		void newGame();
		mx::mxSurface *getBlocks() { return surfaces; }
	protected:
		gameController &controller;
		Current<S_W, S_H> game;
		unsigned int colors[8];
		mx::mxSurface surfaces[9];
		unsigned int speed;
		bool normal;

	};

	typedef struct _Scores {
			char name[10];
			int score, num;
	} Scores;

	class gameOver_Screen : public gameScreen {
	public:
		gameOver_Screen(gameController &c, mx::mxSurface &front);
		~gameOver_Screen();
		virtual void event();
		virtual void render();
		virtual void init();
		virtual void free();
		void setScore(int scr, int number) { view = false; cur_n = 'A'; name_offset = 0; name[0] = 0; score = scr, num = number; }
		void viewScores() { view = true; }
	protected:
		gameController &controller;
		mx::mxSurface bg;
	private:
		int score, num;
		Scores scores[10];
		char name[10];
		int name_offset;
		char cur_n;
		bool view;

		void acceptInput(const char *n);
	};

	class start_Screen : public gameScreen {
	public:
		start_Screen(gameController &c, mx::mxSurface &front);
		~start_Screen();
		virtual void event();
		virtual void render();
		virtual void init();
		virtual void free();
	protected:
		gameController &controller;
	private:
		mx::mxSurface bg;
		int cursor_pos;
		mx::mxSurface  block;
		mx::mxSurface sdl_logo;
		mx::mxSurface panel[5];
	};

	class online_Screen : public gScreen, public mx::mxThread {
	public:
		online_Screen(gameController &c, mx::mxSurface &surf);
		~online_Screen();
		virtual void event();
		virtual void render();
		virtual void init();
		virtual void free();
		virtual int threadExec();
		void keypress(unsigned int, unsigned int);
		void connectTo(std::string ip);
		void listenTo();
	protected:
		Current<S_W, S_H> player2;
	private:
		bool connected;
		int cursor_pos, depth;
		std::string ip_str;
		mx::mxSocket sock;
		bool mode;
		mx::mxMutex mutex;
		std::string err_str;
		int winner;
		unsigned int colors[9];
	};

	class options_Screen : public gameScreen {
	public:
		options_Screen(gameController &ctrl, mx::mxSurface &surf);
		~options_Screen();
		virtual void event();
		virtual void render();
		virtual void init();
		virtual void free();
	protected:
		gameController &controller;
	private:
		mx::mxSurface bg;
		bool full_screen;

	};
}







#endif

