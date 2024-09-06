// about.cpp
// written by jared bruni
// www.lostsidedead.com

#include "thehead.h"


void About::loadfonts()
{

}

void About::ondraw()
{
	// draw background
	mxhwnd.paint.mxdrawrect(0,0,640,480,back_color,back_color);
	mxhwnd.text.setbkcolor(back_color);
	mxhwnd.text.settextcolor(SDL_MapRGB(front->format,0,0,255));
	mxhwnd.text.printtextunderline("www.lostsidedead.com",10,10);
	
	mxhwnd.text.settextcolor(SDL_MapRGB(front->format,0,255,0));
	mxhwnd.text.printtext("press enter to return",10,25);
	
	mxhwnd.text.settextcolor(SDL_MapRGB(front->format,200,0,0));
	mxhwnd.text.printtext("Greets to",100,100);
	
	mxhwnd.text.settextcolor(SDL_MapRGB(front->format,0,200,0));
	mxhwnd.text.printtext( " planetsourcecode.com, best free code site on the net ",100,150);
	mxhwnd.text.printtext(" lostsidedead,chris,allen,vulcan,derick,Jean-Ren� B�dard  ",100,175);
	mxhwnd.text.printtext("issac, john, source, bonk, spine, xeek, micheal,joseph cohen, nukem ",100,200);
	mxhwnd.text.printtext(" and everyone else that I know or have met",100,225);

	// draw mx logo

	mx.DisplayGraphic(5,350);


	// now for some effects 

	for(int i = 0; i < 100; i++) 
	{
		int rand_x = rand()%640;
		int rand_y = rand()%480;
		int rx = rand()%255;
		COLORREF rcolor = SDL_MapRGB(front->format,rx,rx,rx);
		mxhwnd.paint.mxsetpixel(rand_x,rand_y,rcolor);
		mxhwnd.paint.mxsetpixel(rand_x-1,rand_y-1,0x0);
		mxhwnd.paint.mxsetpixel(rand_x+1,rand_y+1,SDL_MapRGB(front->format,rand()%255,rand()%255,rand()%255));
	}

}

void About::onlogic()
{
	if(fade_dir)
	{
		back_fade++;
		if(back_fade > 255)
		{
			back_fade = 255;
			fade_dir = false;
		}
	}
	else
	{
		back_fade--;
		if(back_fade < 0)
		{
			back_fade = 0;
			fade_dir = true;
		}
	}
}

void About::keypress(unsigned int wParam)
{
	switch(wParam)
	{
	case VK_RETURN:
		mxhwnd.SetScreen(ID_START);
		break;
	}

}
