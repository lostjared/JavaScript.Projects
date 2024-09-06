// gintro (game intro) written by jared bruni
// www.lostsidedead.com

#include "thehead.h"



void GIntro::keypress(unsigned int wParam)
{
	// initilize level information structures

	switch(wParam)
	{
	case VK_RETURN:

		char fn[4096];
		sprintf(fn, "level%d.mxk", curmenu+1);
		loadlevel(fn);
		mxhwnd.SetScreen(ID_GAME);
		break;
	case VK_UP:
		if(curmenu > 0)
		{
			curmenu--;
		}
		break;
	case VK_DOWN:
		if(curmenu < 4)
		{
			curmenu++;
		}
		break;
	}
}

void GIntro::ondraw()
{
	COLORREF gc = SDL_MapRGB(front->format,180,180,180);
	mxhwnd.paint.mxdrawrect(0,0,640,480,gc,gc);

	int addx = 2;
	for(int i = 0; i < 4; i++)
	{
		mxhwnd.paint.mxdrawrect(0+addx,0+addx,640-addx,480-addx,SDL_MapRGB(front->format,addx+180,addx+180,addx+180),SDL_MapRGB(front->format,180-addx,180-addx,180-addx));

		addx += 10;

	}

	mxhwnd.paint.mxdrawrect(42,42,640-42,480-42,0,0);

	granl.DisplayGraphic(50,480-42-64-5);
	herol.DisplayGraphic(640-42-32,480-42-64-5);

	// draw introduction story

	mxhwnd.text.settextcolor(SDL_MapRGB(front->format,200,0,0));
	mxhwnd.text.setbkcolor(0x0);
	mxhwnd.text.printtext("select your battle ground....",50,75);
	mxhwnd.text.printtext( "level 1",100,75+25);
	mxhwnd.text.printtext( "level 2",100,75+50);
	mxhwnd.text.printtext( "level 3",100,75+50+25);
	mxhwnd.text.printtext( "level 4",100,75+50+25+25);
	mxhwnd.text.printtext( "level 5",100,75+50+25+25+25);
	mxhwnd.text.settextcolor(SDL_MapRGB(front->format,rand()%255,rand()%255,rand()%255));

	int mx;
	int my;
	mx = 70;
	switch(curmenu)
	{
	case 0:
		my = 75+25;
		break;
	case 1:
		my = 75+50;
		break;
	case 2:
		my = 75+50+25;
		break;
	case 3:
		my = 75+50+25+25;
		break;
	case 4:
		my = 75+50+25+25+25;
		break;
	}

	mxhwnd.text.printtext("=)>",mx,my);
	
	
	mxhwnd.text.printtext("select level, and press return",50,300);

}

void GIntro::onlogic()
{



}

/*************************************************************************** lostsidedead.com */