// www.lostsidedead.com
// written by jared bruni
#include"thehead.h"


void GameOver::ondraw()
{
	graph.DisplayGraphic(0,0);
	mxhwnd.paint.mxdrawrect(50,50,640-50,480-50,0x0,0x0);
	mxhwnd.text.settextcolor(SDL_MapRGB(front->format,200,0,0));
	mxhwnd.text.setbkcolor(0x0);
	mxhwnd.text.printtext("Game Over", 80,80);
	
		switch(screen)
		{
		case ID_WIN:
			mxhwnd.text.settextcolor(SDL_MapRGB(front->format,0,0,255));
			mxhwnd.text.printtext("Yes!, you have won you have defeated the zombie grandmother",80,100+30);
			mxhwnd.text.printtext("what an outstanding preformance.",80,125+30);
			break;
		case ID_LOSE:
			mxhwnd.text.settextcolor(SDL_MapRGB(front->format,0,255,0));
			mxhwnd.text.printtext("You have lost to an old woman... this is to bad",80,100+30);
			mxhwnd.text.printtext("try it once more",80,125+30);
			break;
		}

		mxhwnd.text.settextcolor(SDL_MapRGB(front->format,rand()%255,rand()%255,rand()%255));
		mxhwnd.text.printtext("press enter to continue",80,200);
}

void GameOver::onlogic()
{
		switch(screen)
		{
		case ID_WIN:
			break;
		case ID_LOSE:
			break;
		}
}

void GameOver::passgameover(bool win)
{
	if(win == true)
	{
		screen = ID_WIN;
	}
	else
	{
		screen = ID_LOSE;
	}
}

void GameOver::keypress(unsigned int wParam)
{
	if(wParam == 13)
	{
		mxhwnd.SetScreen(ID_INTRO);
		player.lives = 5;
	}
}

/*************************************************************************** lostsidedead.com */