// masterkong
// written by jared bruni
// www.lostsidedead.com

// Open Source Open Mind

#include "thehead.h"

Player player;

MasterXHWND mxhwnd;

Intro intro;
Start start;
Game  game;
Options opt;
GameOver gameover;
About about;
GIntro gintro;
LevelMap level;
SoundHandle soundhandle;
ParticleHandler hparticle;
SDL_Surface *front;
MasterXHWND *MasterXHWND::wnd_instance = 0;

int main(int argc, char **argv)
{

	srand(time(NULL));

	if(mxhwnd.CreateMasterX("MasterKong",640,480,COLOR_DEFAULT,MasterProc,0, 0,0))
	{
		mxhwnd.InitTimer(1,1);
		mxhwnd.HideCursor();
		mxhwnd.SetScreen(ID_INTRO);
		intro.load();
		start.load();
		game.load();
		opt.load();
		about.load();
		gameover.load();
		gintro.load();
		soundhandle.load();
		hparticle.load();
		loadlevel("demo.mxk");
		soundhandle.Play(OPEN);
		return mxhwnd.InitLoop(update);
	}

	return 0;
}


long MasterProc(unsigned int,unsigned int msg,unsigned int wParam,unsigned int lParam)
{

	switch(msg)
	{
	case WM_KEYDOWN:
		{
			if(wParam == SDLK_ESCAPE)
			{
				mxhwnd.Kill();
			} else
			{
				switch(mxhwnd.GetScreen())
				{
				case ID_START:
					start.keypress(wParam);
					break;
				case ID_ABOUT:
					about.keypress(wParam);
					break;
				case ID_OPTIONS:
					opt.keypress(wParam);
					break;
				case ID_GINTRO:
					gintro.keypress(wParam);
					break;
				case ID_GAME:
					game.hero.keypress(wParam);
					break;
				case ID_GAMEOVER:
					gameover.keypress(wParam);
					break;
				}
			}
			
		}
		break;
	case WM_KEYUP:
		{
			switch(mxhwnd.GetScreen())
			{
			case ID_GAME:
				game.hero.keyrelease(wParam);
				break;
			}
		}
		break;
	}

	return 0;
}

void update(MASTERSCREEN screen)
{
	mxhwnd.SetTimeFlag();
	SDL_Delay(30);

	switch(screen)
	{
	case ID_INTRO:
		intro.update();
		break;
	case ID_START:
		start.update();
		break;
	case ID_GAME:
		game.update();
		break;
	case ID_GAMEOVER:
		gameover.update();
		break;
	case ID_OPTIONS:
		opt.update();
		break;
	case ID_ABOUT:
		about.update();
		break;
	case ID_GINTRO:
		gintro.update();
		break;
	}

}

// load the level
void loadlevel(char* levelstr)
{

	char buffer[4096];
#ifdef __EMSCRIPTEN__
	snprintf(buffer, 4095, "/assets/%s", levelstr);
#else
	snprintf(buffer, 4095, "%s", levelstr);
#endif

	std::fstream fin;
	fin.open(buffer, std::ios::in | std::ios::binary);
	if(!fin.is_open()) {
		printf("Error openining file: %s\n", buffer);
	}
	fin.read((char*)&level,sizeof(level));
	fin.close();

	game.hero.hero_pos = level.hsp;
	game.grandma.pos = level.gsp;
	game.grandma.spos = level.gsp;
	game.hero.start_pos = level.hsp;

	game.grandma.birth();

}

// how many rows over
int rows(int row)
{
	return (row*24);
}

Hero* GetHero()
{
	return (Hero*)&game.hero;
}

Grandma* GetGrandma()
{
	return (Grandma*)&game.grandma;
}

MasterJoyStick* getstick()
{
	return (MasterJoyStick*)&game.stick;
}

void thegameover(bool win)
{
	mxhwnd.SetScreen(ID_GAMEOVER);
	gameover.passgameover(win);
}

/*************************************************************************** lostsidedead.com */
