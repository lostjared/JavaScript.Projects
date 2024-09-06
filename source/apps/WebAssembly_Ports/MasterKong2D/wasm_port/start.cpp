// written by jared bruni
// www.lostsidedead.com

#include "thehead.h"


void Start::ondraw()
{
	if(startgo)
	{

	startg.DisplayGraphic(0,0);
	grandmax.update();
	drawbridge();
	drawstart();

	}
	else
	{
		starta[sx].DisplayGraphic(0,0);
	}

 
}

void Start::drawbridge()
{
	int rx = 1;
	int ry = 150+64;
	for(int i = 0; i < 29; i++)
	{
		bridge.DisplayGraphic(rx,ry);
		rx += 22;
	}
}

void Start::drawstart()
{
	switch(menupos)
	{
	case 0:// start
		stx = 190;
		sty = 290;
		break;
	case 1:// options
		stx = 190;
		sty = 290 + 35;
		break;
	case 2:// about
		stx = 190;
		sty = 290 + (35 * 2);
		break;
	case 3:// exit
		stx = 190;
		sty = 280 + (35 * 3) + 10;
		break;
	}
	mxhwnd.paint.mxdrawellipse( stx, sty, stx+25,sty+25,SDL_MapRGB(front->format,rand()%255,rand()%255,rand()%255),SDL_MapRGB(front->format,rand()%255,rand()%255,rand()%255));
}

void Start::onlogic()
{
	if(startgo == false)
	{
		if(sx == 0)
		{
			sx = 1;
		}
		else
		{
			sx = 0;
		}

		si++;

		if(si > 30)
		{
			si = 0;
			startgo = true;
		}
	}
	else
	{
		logicgrandma();
	}
}


void Start::logicgrandma()
{
	if(gdir == true)
	{
		gx += 5;

		if(gx > 640-32)
		{
			gx = 640-32;
			gdir = false;
		}
	}
	else
	{
		gx -= 5;
		if(gx < 32)
		{
			gx = 32;
			gdir = true;
		}
	}
	if(sx == 0)
	{


	gi++; 
	sx = 1;

	}
	else
	{
		sx = 0;
	}
	if(gi > 4)
	{
		gi = 0;
	}

	grandmax.SetPos(gdir,gx,gy,gi);
}

// uesr keypress

void Start::keypress(unsigned int wParam)
{
	switch(wParam)
	{
	case VK_DOWN:
		{
			if( menupos < 3 )
			{
				menupos++;
			}
		}
		break;
	case VK_UP:
		{
			if(menupos > 0)
			{
				menupos--;
			}

		}
		break;
	case VK_RETURN:
		{
			switch(menupos)
			{
			case 0:// start
				mxhwnd.SetScreen(ID_GINTRO);
				break;
			case 1:// optoins
				mxhwnd.SetScreen(ID_OPTIONS);
				break;
			case 2:// about
				mxhwnd.SetScreen(ID_ABOUT);
				break;
			case 3:// exit
				mxhwnd.Kill();
				break;
			}
		}
		break;
	}
}

/*************************************************************************** lostsidedead.com */