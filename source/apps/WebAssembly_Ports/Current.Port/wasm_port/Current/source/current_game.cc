#include "current_game.h"
#include "mxpng.h"
#include<sstream>
#include<fstream>
#include<algorithm>
#include"main_window.h"
#ifndef _WIN32
#include<unistd.h>
#endif


namespace current {
	mx::mxRegularFont font;

	gScreen::gScreen(gameController &c, mx::mxSurface &f) : gameScreen(f), controller(c) {
		init();
	}


	gScreen::~gScreen() {
		free();
	}

	void gScreen::event() {
		static int  wait2=0;
		static int ticks = SDL_GetTicks();
		wait2 = SDL_GetTicks();
		if((ticks+50 > wait2) == 0) {
			if(controller.pollController(LEFT))
				game.moveLeft();
			if(controller.pollController(RIGHT))
				game.moveRight();

			if(game.current_direction == Current<S_W, S_H>::MOVE_DOWN && controller.pollController(DOWN))
				game.timer_update();
			if(game.current_direction == Current<S_W, S_H>::MOVE_UP && controller.pollController(UP))
				game.timer_update();

			ticks = SDL_GetTicks();
		}
		if(game.gameOver() == true && normal == true) {
			mainWindow::window->overscr.setScore( game.score, game.num );
			mainWindow::window->setScreen(&mainWindow::window->overscr);
		}
		static int wait3 = 0;
		static int ticks1 = SDL_GetTicks();
		wait3 = SDL_GetTicks();
		if((ticks1+750 > wait3)==0) {
			game.timer_update();
			ticks1 = SDL_GetTicks();
			if(controller.pollController(BTN_A)) {
					game.switchColors();
			}
		}
		static int wait4 = 0;
		static int ticks2 = SDL_GetTicks();
		wait4 = SDL_GetTicks();
		if((ticks2+200 > wait3)==0) {
			ticks2 = SDL_GetTicks();
			if(controller.pollController(BTN_A)) {
					game.switchColors();
			}
		}

	}

	void gScreen::render() {
		game.update();
		const int start_x = 100, start_y = 100;
		front.copySurface(surfaces[0], 0, 0);
		for(int i = 0; i < S_W; i++)
			for(int z = 0; z < S_H; z++) {
				int col = game(i,z);
				if(col > 0) {
					SDL_Rect rc = { start_x+(i*16), start_y+(z*16), 16, 16 };
					front.copySurface(surfaces[col], 0, &rc);
				}
			}
			SDL_Rect rc1 = { start_x+(game.gameblock.x*16), start_y+(game.gameblock.y*16), 16, 16 };
			SDL_Rect rc2 = { start_x+(game.gameblock.x*16), start_y+((game.gameblock.y+1)*16), 16, 16 };
			SDL_Rect rc3 = { start_x+(game.gameblock.x*16), start_y+((game.gameblock.y+2)*16), 16, 16 };
			front.copySurface(surfaces[game.gameblock.blocks[0]], 0, &rc1);
			front.copySurface(surfaces[game.gameblock.blocks[1]], 0, &rc2);
			front.copySurface(surfaces[game.gameblock.blocks[2]], 0, &rc3);
			{
				SDL_Rect rcX = { 900, 100, 125, 200 };
				SDL_Rect rc1 = { 950, 200, 16, 16 };
				SDL_Rect rc2 = { 950, 216, 16, 16 };
				SDL_Rect rc3 = { 950, 232, 16, 16 };
				SDL_FillRect(front, &rcX, 0);
				front.copySurface(surfaces[game.nextblock.blocks[0]], 0, &rc1);
				front.copySurface(surfaces[game.nextblock.blocks[1]], 0, &rc2);
				front.copySurface(surfaces[game.nextblock.blocks[2]], 0, &rc3);
				std::ostringstream ss;
				ss << "Score: " << game.getScore();
				font.printText(front, mx::Point(910, 110), mx::Color(255, 0, 0), ss.str());
				ss.str("");
				ss << "Cleared: " << game.getClears();
				font.printText(front, mx::Point(910, 130), mx::Color(255, 255, 255), ss.str());
				font.printText(front, mx::Point(920, 170), mx::Color(0, 255, 0), "Next Block");
			}
	}


	void gScreen::init() {
		std::ostringstream ss;
		mx::mxPng p;
		p.pngOpen("/assets/images/background.png");
		surfaces[0] = p.LoadPNG();
		p.pngClose();
		for(int i = 1; i <= 8; i++) {
			ss << "/assets/images/block" << i << ".png";
			p.pngOpen(ss.str());
			surfaces[i] = p.LoadPNG();
			p.pngClose();
			ss.str("");
		}
		newGame();
		font.loadFont("/assets/fonts/verdana.mxf");
		normal = true;
	}

	void gScreen::free() {
	}

	unsigned int gScreen::timerExecution(unsigned int timeout) {
		game.timer_update();
		return game.speed;
	}

	void gScreen::newGame() {
		game.initgraph();
		game.speed = 800;
		setTimer(game.speed);
		game.current_direction = Current<S_W, S_H>::MOVE_DOWN;
	}


	gameOver_Screen::gameOver_Screen(gameController &c, mx::mxSurface &f) : gameScreen(f), controller(c) {
		init();
	}


	gameOver_Screen::~gameOver_Screen() {
		free();
	}


	void gameOver_Screen::event() {
		static int wt = 0;
		if((++wt%6)==0) {
			if(controller.pollController(UP) && view == false) {
				if(cur_n < 'Z')
					cur_n++;
			}
			if(controller.pollController(DOWN) && view == false) {
				if(cur_n > 'A')
					cur_n--;
			}
			if(controller.pollController(BTN_A) && view == false) {
				if(name_offset < 3) {
					name[name_offset++] = cur_n;
					name[name_offset] = 0;

					if(name_offset == 3)
						acceptInput(name);
				}
			}
			if(controller.pollController(BTN_B)) {
				if(name_offset > 0) {
					name[--name_offset] = 0;
				}
			}
			if(controller.pollController(BTN_A)) {
				mainWindow::window->setScreen(&mainWindow::window->startscr);
			}
		}

	}

	void gameOver_Screen::render() {
		front.copySurface(bg, 0, 0);
		std::ostringstream ss;
		if(view == false) {
			ss << "Your Score: " << score << " Cleared " << num;
			font.printText(front, mx::Point(75, 200), mx::Color(255, 0, 0), ss.str());
		}
		int pos = 240;
		for(int i = 0; i < 10; i++) {
			ss.str("");
			ss << scores[i].name << " Score: " << scores[i].score << " Cleared: " << scores[i].num;
			font.printText(front, mx::Point(100, pos), mx::Color(255,255,255), ss.str());
			pos += 25;
		}
		font.printText(front, mx::Point(400, 400), mx::Color(255,255,255), "Press A or Button 1 to return to Start Screen");
		if(view == true) return;
		if(score > scores[9].score && view == false) {
			font.printText(front, mx::Point(400, 240), mx::Color(0,0,255), "New High Score Enter Name");
			char n[10];
			static int cnt = 0;
			name[name_offset] = cur_n;
			name[name_offset+1] = 0;
			if((++cnt % 4) == 0) sprintf(n, "%s_", name);
			else sprintf(n, "%s", name);
			font.printText(front, mx::Point(400, 290), mx::Color(255, 0, 0), n);
			font.printText(front, mx::Point(400, 350), mx::Color(255, 0, 0), "Press up and down arrow to change letter\nPress A or button 1 to select\nPress S or button 2 to erase");

		} else {
			if(name_offset == 0)
			font.printText(front, mx::Point(400, 240), mx::Color(0,0,255), "You did not make it into the High Scores try again");
		}
	}


	void gameOver_Screen::init() {

		mx::mxPng p;
		if(p.pngOpen("/assets/images/gameover.png")) {
			bg = p.LoadPNG();
			p.pngClose();
		}
		// load scores
		std::fstream file;
		file.open("scores.dat", std::ios::in | std::ios::binary);
		if(!file.is_open()) {

			for(int i = 0; i < 10; i++) {
				scores[i].score = 0;
				scores[i].num = 0;
				sprintf(scores[i].name, "%s", "Unknown");
			}
			free();
		}
		else {
			file.read((char*)scores, sizeof(scores));
			file.close();
		}
	}

	void gameOver_Screen::free() {
		// save scores
		std::fstream file;
		file.open("scores.dat", std::ios::out | std::ios::binary);
		if(file.is_open()) {
			file.write((char*)scores, sizeof(scores));
			file.close();
		}
	}



	void gameOver_Screen::acceptInput(const char *n) {
		snprintf(scores[9].name, 9, "%s", n);
		scores[9].score = score;
		scores[9].num = num;
		int max = 10;
		int pas,compare;
			for(pas = 0; pas < max - 1; pas++) {
				for(compare = 0; compare < max - 1; compare++) {
					if(scores[compare].score < scores[compare+1].score) {
						Scores  temp;
						temp = scores[compare];
						scores[compare] = scores[compare+1];
						scores[compare+1] = temp;
					}
				}
			}
		score = 0;
		num = 0;
		name[0] = 0;
	}

	start_Screen::start_Screen(gameController &c, mx::mxSurface &front) : gameScreen(front), controller(c) {
		init();
	}

	start_Screen::~start_Screen() {
		free();
	}

	void start_Screen::event() {
		static int  wait2=0;
		static int ticks = SDL_GetTicks();
		wait2 = SDL_GetTicks();
		if((ticks+150 > wait2) == 0) {
			if(controller.pollController(UP)) {
					if(cursor_pos > 0) --cursor_pos;
			}
			if(controller.pollController(DOWN)) {
					if(cursor_pos < 4) ++cursor_pos;
			}
			ticks = SDL_GetTicks();
		}
		if(controller.pollController(BTN_START)) {

			switch(cursor_pos) {
			case 0:
				mainWindow::window->gamescr.newGame();
				mainWindow::window->setScreen(&mainWindow::window->gamescr);
				break;
			case 1:
				std::cout << "Threads not supported on this Web Platform\n";
				//mainWindow::window->onlinescr.newGame();
				//mainWindow::window->setScreen( &mainWindow::window->onlinescr );
				break;
			case 2:
				mainWindow::window->setScreen ( &mainWindow::window->options );
				break;
			case 3:
				mainWindow::window->overscr.viewScores();
				mainWindow::window->setScreen( &mainWindow::window->overscr );
				break;
			case 4:
				mainWindow::window->quit();
				break;
			}
		}
	}

	void start_Screen::render() {
		front.copySurface(bg, 0, 0);

		SDL_Rect rcPanel = { 25, 200, 250, 250 };


		front.copySurface(panel[cursor_pos], 0, &rcPanel);

		mx::Color white(255,255,255);
		mx::Color red(255, 0, 0);
		mx::Color cur_color[5];
		for(int i = 0; i < 5; i++)
			cur_color[i] = (i == cursor_pos) ? red : white;
		font.printText(front, mx::Point(250, 225), cur_color[0], "Single Player");
		font.printText(front, mx::Point(250, 250), cur_color[1], "Multiplayer");
		font.printText(front, mx::Point(250, 275), cur_color[2], "Options");
		font.printText(front, mx::Point(250, 300), cur_color[3], "High Scores");
		font.printText(front, mx::Point(250, 325), cur_color[4], "Exit");
		SDL_Rect rcX = { 225, 225+(cursor_pos*25), 16, 16 };
		front.copySurface(block, 0, &rcX);
		SDL_Rect logoX = { 400, 225, sdl_logo.width(), sdl_logo.height() };
		front.copySurface(sdl_logo, 0, &logoX);
		font.printText(front, mx::Point(400, 225), mx::Color(255, 0, 0), "Powered By");

	}

	void start_Screen::init() {
		loadImage(bg, "/assets/images/startscr.png");
		cursor_pos = 0;
		loadImage(block, "/assets/images/block4.png");
		loadImage(sdl_logo, "/assets/images/SDL_logo.png");
		loadImage(panel[0], "/assets/images/single.png");
		loadImage(panel[1], "/assets/images/multi.png");
		loadImage(panel[2], "/assets/images/options.png");
		loadImage(panel[3], "/assets/images/scores.png");
		loadImage(panel[4], "/assets/images/exit.png");
	}

	void start_Screen::free() {

	}

	online_Screen::online_Screen(gameController &ctrl, mx::mxSurface &f) : gScreen(ctrl, f) {
		init();
	}

	online_Screen::~online_Screen() {
		free();
	}

	void online_Screen::init() {

#ifndef _WIN32
		//signal(SIGPIPE, 0);
#endif

		winner = 0;

		connected = false;
		stopTimer();
		cursor_pos = 0;
		depth = 0;
		normal = false;

		colors[0] = SDL_MapRGB(front, 0, 0, 0);
		colors[1] = SDL_MapRGB(front, 255, 255, 255);
		colors[2] = SDL_MapRGB(front, 255, 0, 0);
		colors[3] = SDL_MapRGB(front, 0, 255, 0);
		colors[4] = SDL_MapRGB(front, 0, 0, 255);
		colors[5] = SDL_MapRGB(front, 0, 255, 255);
		colors[6] = SDL_MapRGB(front, 255, 0, 255);
		colors[7] = SDL_MapRGB(front, 255, 255, 0);
	}

	void online_Screen::free() {

	}

	void online_Screen::event() {
		if(connected == true)
			gScreen::event();
		else {

		}
	}

	void online_Screen::render() {

		front.copySurface(surfaces[0], 0, 0);
		SDL_Rect rcX = { 125, 125, 450, 450 };

		if(connected == true) {
			gScreen::render();
			mutex.lockMutex();
			static const int offset_x = 924;
			int offset_y = 648;
			SDL_Rect rc = { offset_x, offset_y, 100, 120 };
			offset_y += 20;
			SDL_FillRect(front, &rc, 0x0);
			for(int i = 0; i < S_W; i++) {
				for(int z = 0; z < S_H; z++) {
					SDL_Rect rcX = { offset_x+(i*2), offset_y+(z*2), 1, 1 };
					int col = player2(i,z);
					if(col != -1) {
						if(col != 0) {
							SDL_FillRect(front, &rcX, colors[col]);
						}
					}
				}
				font.printText(front,mx::Point(offset_x+3, offset_y-17), mx::Color(255, 255, 255), "Opponent");
			}
			mutex.unlockMutex();
		}
		// draw oponnent's screen
		else if(depth == 0) {
			SDL_Rect rcX = { 125, 125, 450, 450 };
			SDL_FillRect(front, &rcX, 0x0);
			font.printText(front, mx::Point(165, 135), mx::Color(255, 0, 0), "Listen" );
			font.printText(front, mx::Point(165, 160), mx::Color(0, 0, 255), "Connect");
			font.printText(front, mx::Point(145, 280), mx::Color(255, 0, 0), err_str);
			font.printText(front, mx::Point(135, 135+(cursor_pos*25)), mx::Color(0,255,0), "[*]");
			font.printText(front, mx::Point(300, 250), mx::Color(255,255,255), "Select how to connect\nIf listening forward port 1024\nIf connecting give IP address");
		} else if(depth == 1) {
			SDL_FillRect(front, &rcX, 0x0);
			font.printText(front, mx::Point(165, 135), mx::Color(255,255,255), "Listening on Port 1024. . . ");
		} else if(depth == 2) {

			std::ostringstream ss;
			static int wt = 0;
			ss << "Enter IP with Keyboard, Connect to: " << ip_str << ((++wt % 4) == 0 ? "_" : "");
			font.printText(front, mx::Point(165, 135), mx::Color(255,255,255), ss.str());
			font.printText(front, mx::Point(165, 160), mx::Color(255, 0, 0), "Press Enter to connect");
		} else if(depth == 3) {
			if(winner == 1) font.printText(front, mx::Point(165, 135), mx::Color(255, 255, 255), "You Win!");
			if(winner == 2) font.printText(front, mx::Point(165, 135), mx::Color(255, 255, 255), "You Lose!");

		} else if(depth == 4) {
			font.printText(front, mx::Point(165, 135), mx::Color(255,255,255), "Connection Interrupted");
		}
	}

	int online_Screen::threadExec() {
		if(mode == false) {
			connected = false;
			sock.listenAt(1024);
			sock = sock.acceptsocket();
			connected = true;
			newGame();
			while( sock.isConnected()  && player2(0,0) != 12) {

				mutex.lockMutex();
				int val = sock.Read((char*)player2.graph, sizeof(player2.graph));
				mutex.unlockMutex();
				if(val == -1) {
					connected = false;
					depth = 4;
					return 0;
				}

				if(player2.graph[0][0] == 12) {
					winner = 1;
					connected = false;
					depth = 3;
					return 0;
				}

				if(game.gameOver()) game.graph[0][0] = 12;

				SDL_Delay( 100 );
				val = sock.Write((char*)game.graph, sizeof(game.graph));


				if(game.gameOver()) {
					winner = 2;
					connected = false;
					depth = 3;
				}


				if(val == -1) {
					connected = false;
					depth = 4;
					return 0;
				}
				SDL_Delay( 100 );
			}

		} else  {
			if(connected == true) {
				while( sock.isConnected() && player2(0, 0) != 12) {
					if(game.gameOver()) game.graph[0][0] = 12;
					int val = sock.Write((char*)game.graph, sizeof(game.graph));
					if(val == -1) {
						connected = false;
						depth = 4;
						return 0;
					}

					if(game.gameOver()) {
						winner = 2;
						depth = 3;
						connected = false;
						return 0;
					}

					SDL_Delay( 100 );
					mutex.lockMutex();
					val = sock.Read((char*)player2.graph, sizeof(player2.graph));
					mutex.unlockMutex();
					if(val == -1){
						connected = false;
						depth = 4;
						return 0;
					}
					if(player2.graph[0][0] == 12) {
						connected = false;
						depth = 3;
						winner = 1;
						return 0;
					} else if ( game.gameOver() == true ) {
						connected = false;
						depth = 3;
						winner = 2;
					}
					SDL_Delay( 100 );
				}
			}

		}

		return 0;
	}

	void online_Screen::keypress(unsigned int key, unsigned int uni_key) {

		switch(depth) {
		case 0:
			if(key == SDLK_DOWN) if(cursor_pos < 1) cursor_pos++;
			if(key == SDLK_UP) if(cursor_pos > 0) cursor_pos--;
			if(key == SDLK_RETURN) {
				switch(cursor_pos) {
				case 0:
					listenTo();
					depth = 1;
					break;
				case 1:
					depth = 2;
					break;
				}
			}
		case 2:
			if(key == SDLK_BACKSPACE) {
				if(ip_str.length() > 0) ip_str = ip_str.substr(0, ip_str.length()-1);
			} else if(key == SDLK_RETURN) {
				if(ip_str.length() > 0) {
					connectTo(ip_str);
				}
			}
			else if(isdigit(key) || key == '.') ip_str += key;
			break;
		case 3:
		case 4:

			mainWindow::window->setScreen( &mainWindow::window->startscr );

			break;

		}
	}

	void online_Screen::connectTo(std::string ip) {

		mode = true;

		sock.createSocket();
		sock.connectTo(ip, 1024);

		if(sock.isConnected() == false) {
			depth = 0;
			err_str = "Connection Failed";
		} else {
			err_str = "";
			ip_str = "";
			connected = true;
			threadRun();
			newGame();
		}


	}

	void online_Screen::listenTo() {

		mode = false;
		sock.createSocket();
		threadRun();


	}

	options_Screen::options_Screen(gameController &ctrl, mx::mxSurface &surf) : gameScreen(surf), controller(ctrl) {
		init();
	}

	options_Screen::~options_Screen() {
		free();
	}

	void options_Screen::init() {
		loadImage(bg, "/assets/images/startscr.png");
		full_screen = false;
	}

	void options_Screen::free() {

	}

	void options_Screen::event() {

		if(controller.pollController(LEFT)) {

			if(full_screen != false) {
				full_screen = false;
				front = SDL_SetVideoMode(1024, 768, 0, 0);
			}
		}
		if(controller.pollController(RIGHT)) {

			if(full_screen != true) {
				full_screen = true;
				front = SDL_SetVideoMode(1024, 768, 0, SDL_FULLSCREEN);
			}
		}

		if(controller.pollController(BTN_A)) {
			mainWindow::window->setScreen( &mainWindow::window->startscr );
		}
	}

	void options_Screen::render() {

		front.copySurface(bg, 0, 0);
		font.printText(front, mx::Point(50,225), mx::Color(255, 0, 0), "[ Options Screen ]");
		std::ostringstream ss;
		ss<<" Full screen [ " << ((full_screen == false) ? "False" : "True") << " ] ";
		font.printText(front, mx::Point(50, 250), mx::Color(255,255,255), ss.str());
		font.printText(front, mx::Point(350, 225), mx::Color(255, 0, 0), "Use arrow keys or directional pad to toggle options\npress button 1 or A to return\n");

	}

}

