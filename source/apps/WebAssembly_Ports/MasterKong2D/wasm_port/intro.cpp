//written by jared bruni
// www.lostsidedead.com

#include "thehead.h"


void Intro::drawbg()
{
	introg.DisplayGraphic(0,0);
}

void Intro::onlogic()
{
	gpos++;
	if(gpos > 100)
	{
		gpos = 0;
		mxhwnd.SetScreen(ID_START);
	}

}

/*************************************************************************** lostsidedead.com */