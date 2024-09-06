// written by jared bruni
// www.lostsidedead.com

/*<; for educational purposes only ;>*/


#include "thehead.h"


void Options::ondraw()
{
	starta[bx].DisplayGraphic(0,0);
	drawop();
}

void Options::onlogic()
{
	if(bx == 0)
	{
		bx = 1;
	}
	else
	{
		bx = 0;
	}
}


void Options::drawop()
{
	mxhwnd.paint.mxdrawrect(50,50,640-50,480-50,0x0,0x0);
	mxhwnd.text.settextcolor(SDL_MapRGB(front->format,rand()%255,rand()%255,rand()%255));
	mxhwnd.text.setbkcolor(0x0);
	mxhwnd.text.printtext("Options Menu",60,60);
	mxhwnd.text.settextcolor(SDL_MapRGB(front->format,0,0,200));

	lostx.DisplayGraphic(225,80);

	char dstr[100];
	sprintf(dstr,"Difficulty: %i", player.difficulty);
	char lstr[100];
	sprintf(lstr,"Lives: %i",player.lives);
	char jstr[100];
	if(player.joystick == true)
	{
		strcpy(jstr,"Joystick active");
	}
	else
	{
		strcpy(jstr,"Joystick inactive");
	}

	mxhwnd.text.printtext(dstr,80,100);
	mxhwnd.text.printtext(lstr,80,125);
	mxhwnd.text.printtext(jstr,80,150);
	mxhwnd.text.printtext("All Done, Return",80,300);
	mxhwnd.text.printtext("visit lostsidedead.com",80,325);

	// draw cursor

	int cx = 80-20;
	int cy = 0;

	switch(cpos)
	{
	case 0:
		cy = 100;
		break;
	case 1:
		cy = 125;
		break;
	case 2:
		cy = 150;
		break;
	case 3:
		cy = 300;
		break;
	case 4:
		cy = 325;
		break;
	}
	mxhwnd.text.settextcolor(SDL_MapRGB(front->format,200,0,0));
	mxhwnd.text.printtext("=)>",cx,cy);
}

void Options::keypress(unsigned int wParam)
{
	switch(wParam)
	{
	case VK_RETURN:
		{
			switch(cpos)
			{
			case 3:
				mxhwnd.SetScreen(ID_START);
				break;
			case 4:
				break;
			}
		}
		break;
	case VK_LEFT:
		{
			switch(cpos)
			{
			case 1:
				if(player.lives > 1)
				{
					player.lives--;
				}
				break;
			case 0:
				if(player.difficulty > 1)
				{
					player.difficulty--;
				}

				break;
			}

		}
		break;
	case VK_RIGHT:
		{
			switch(cpos)
			{
			case 1:
				if(player.lives < 10)
				{
					player.lives++;
				}
				break;
			case 0:
				if(player.difficulty < 5)
				{
					player.difficulty++;
				}

				break;
			}

		}
		break;
	case VK_DOWN:
		{

			if(cpos < 4)
			{
				cpos++;
			}
		}
		break;
	case VK_UP:
		{
			if(cpos > 0)
			{
				cpos--;
			}
		}
		break;
	}
}

/*************************************************************************** lostsidedead.com */