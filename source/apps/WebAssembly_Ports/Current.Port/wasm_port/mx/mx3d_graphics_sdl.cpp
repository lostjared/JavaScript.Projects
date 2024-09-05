/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/


#include "mx3d_graphics.h"
#include "SDL.h"
#include<iostream>



namespace mx3d
{


	static int max_clip_x = 640, min_clip_x = 0, max_clip_y = 480, min_clip_y = 0;


	void Swap(int& a, int& b)
	{
		int temp = a;
		a = b;
		b = temp;
	}

	void Swap(float& a, float& b)
	{
		float temp = a;
		a = b;
		b = temp;
	}







	void *buffer = 0;
	bool is_locked = false;



	graphicsInterface::graphicsInterface(SDL_Surface *f)
	{


		if(f == 0) front = SDL_GetVideoSurface();
		else front = f;


		const struct SDL_VideoInfo *vi;
		vi = SDL_GetVideoInfo();

		w = vi->current_w;
		h = vi->current_h;
		rect.x = 0;
		rect.y = 0;
		rect.w = w;
		rect.h = h;
		rect.Pitch = front->pitch;

		std::cout << "gfx iface: " << w << ":" << h << ":" << front->pitch <<  "\n";
		SDL_FillRect(front, 0, SDL_MapRGB(front->format, 255, 255, 255));


	}

	graphicsInterface::~graphicsInterface()
	{



	}



	void graphicsInterface::lock()
	{

		SDL_LockSurface(front);
		buffer = (void*) front->pixels;
		is_locked = true;

	}


	void graphicsInterface::unlock()
	{
		SDL_UnlockSurface(front);
		is_locked = false;

	}



	void graphicsInterface::fast_sp(int x, int y, int, unsigned int color)
	{

		if(is_locked == false) { return; }

		unsigned int *ptr = (unsigned int *) buffer;
		ptr[x+(y*w)] = color;
	}

	void graphicsInterface::setpixel(int x, int y, unsigned int color)
	{

			fast_sp(x,y,4,color);
	}

	unsigned int graphicsInterface::fast_gp(int x, int y)
	{

		unsigned int *ptr = (unsigned int *) buffer;
		unsigned int *p = &ptr[x+(y*w)];
		return *p;

	}


	void graphicsInterface::drawrect(int x, int y, int w, int h, unsigned int color)
	{

		SDL_Rect rc = { x,y,w,h };
		SDL_FillRect(front, &rc, color);

	}

	void graphicsInterface::drawbottomtri(int x1, int y1, int x2, int , int x3, int y3, unsigned int color) {
		float dx_right,dx_left,xs,xe,height;
		int temp_x,temp_y,right,left;
		if (x3 < x2) {
			temp_x = x2;
			x2     = x3;
			x3     = temp_x;
		}
		height = float(y3-y1);
		dx_left  = (x2-x1)/height;
		dx_right = (x3-x1)/height;

		xs = (float)x1;
		xe = (float)x1;
		if (y1 < min_clip_y) {
			xs = xs+dx_left*(float)(-y1+min_clip_y);
			xe = xe+dx_right*(float)(-y1+min_clip_y);
			y1 = min_clip_y;
		}

		if (y3 > max_clip_y)
			y3 = max_clip_y;

		if (x1>=min_clip_x && x1<=max_clip_x && x2>=min_clip_x && x2<=max_clip_x && x3>=min_clip_x && x3<=max_clip_x) {
			for (temp_y=y1; temp_y<=y3; temp_y++) {
				drawhline(int(xs),(int)xs+(int(xe-xs+1)),temp_y,color);
				xs+=dx_left;
				xe+=dx_right;

			}
		}
		else
		{
			for (temp_y=y1; temp_y<=y3; temp_y++) {
				left  = (int)xs;
				right = (int)xe;
				xs+=dx_left;
				xe+=dx_right;

				if (left < min_clip_x){
					left = min_clip_x;

				if (right < min_clip_x)
					continue;
			}

			if (right > max_clip_x)
			{
				right = max_clip_x;

				if (left > max_clip_x)
					continue;
			}
			drawhline(int(left),(int)left+(int(right-left+1)),temp_y,color);
		}
	}
}


void graphicsInterface::drawtoptri(int x1, int y1,int x2, int , int x3, int y3, unsigned int color) {

	float dx_right,dx_left,xs,xe,height;

	int temp_x,temp_y,right,left;

	if (x2 < x1) {
		temp_x = x2;
		x2     = x1;
		x1     = temp_x;
	}

	height = float(y3-y1);

	dx_left  = (x3-x1)/height;
	dx_right = (x3-x2)/height;

	xs = (float)x1;
	xe = (float)x2;
	if (y1 < min_clip_y) {
		xs = xs+dx_left*(float)(-y1+min_clip_y);
		xe = xe+dx_right*(float)(-y1+min_clip_y);
		y1=min_clip_y;
	}

	if (y3>max_clip_y)
		y3=max_clip_y;

	if (x1>=min_clip_x && x1<=max_clip_x && x2>=min_clip_x && x2<=max_clip_x && x3>=min_clip_x && x3<=max_clip_x) {
		for (temp_y=y1; temp_y<=y3; temp_y++) {
			drawhline(int(xs),int(xs+(int)xe-xs+1),temp_y,color);
			xs+=dx_left;
			xe+=dx_right;
		}
	}
	else {
		for (temp_y=y1; temp_y<=y3; temp_y++) {
			left  = (int)xs;
			right = (int)xe;
			xs+=dx_left;
			xe+=dx_right;
			if (left < min_clip_x) {
				left = min_clip_x;
				if (right < min_clip_x)
					continue;
			}

			if (right > max_clip_x) {
				right = max_clip_x;

				if (left > max_clip_x)
					continue;
			}
			drawhline((int)left,(int)left+(int)right-left+1,temp_y,color);
		}

	}


}


void graphicsInterface::drawtri(int x1, int y1,int x2, int y2, int x3, int y3, unsigned int color) {
	int temp_x,temp_y,new_x;

	if ((x1==x2 && x2==x3)  ||  (y1==y2 && y2==y3))
		return;

	if (y2<y1) {
		temp_x = x2;
		temp_y = y2;
		x2     = x1;
		y2     = y1;
		x1     = temp_x;
		y1     = temp_y;
	}

	if (y3<y1) {
		temp_x = x3;
		temp_y = y3;
		x3     = x1;
		y3     = y1;
		x1     = temp_x;
		y1     = temp_y;
	}

	if (y3<y2) {
		temp_x = x3;
		temp_y = y3;
		x3     = x2;
		y3     = y2;
		x2     = temp_x;
		y2     = temp_y;

	}
	if ( y3<min_clip_y || y1>max_clip_y || (x1<min_clip_x && x2<min_clip_x && x3<min_clip_x) || (x1>max_clip_x && x2>max_clip_x && x3>max_clip_x) )
		return;


	if (y1==y2)  {
		this->drawtoptri(x1,y1,x2,y2,x3,y3,color);
	}
	else
		if (y2==y3){
			this->drawbottomtri(x1,y1,x2,y2,x3,y3,color);
		}
		else {
			new_x = x1 + (int)(0.5+(float)(y2-y1)*(float)(x3-x1)/(float)(y3-y1));
			this->drawbottomtri(x1,y1,new_x,y2,x2,y2,color);
			this->drawtoptri(x2,y2,new_x,y2,x3,y3,color);
		}
}

void graphicsInterface::drawhline(int x,int x2, int y,unsigned int color) {
	for(int p = x; p <= x2; p++)
		fast_sp(p,y,rect.Pitch,color);
}

void graphicsInterface::drawtri2(float , float , float , float , float , float , unsigned int ) {

}

void graphicsInterface::drawgentri(int x0, int y0, int x1, int y1, int x2, int y2, unsigned int color) {

	if( y1 < y0 )
	{
		Swap(y1, y0);
		Swap(x1, x0);
	}
	if( y2 < y0 )
	{
		Swap(y2, y0);
		Swap(x2, x0);
	}

	if( y1 < y2 )
	{
		Swap(y2, y1);
		Swap(x2, x1);
	}


	float xl_edge = (float)x0;
	float xr_edge = (float)x0;
	float dxldy;
	float dxrdy;
	float dxdy1 = (float)(x2-x0)/(y2-y0);
	float dxdy2 = (float)(x1-x0)/(y1-y0);
	if( dxdy1 < dxdy2 )
	{
		dxldy = dxdy1;
		dxrdy = dxdy2;
	}
	else
	{
		dxldy = dxdy2;
		dxrdy = dxdy1;
	}

	for(int y=y0; y<y2; y++)
	{
		for(int x=int(xl_edge); x<int(xr_edge); x++)
		{
			this->fast_sp(x, y, this->rect.Pitch, color);
		}

		xl_edge = xl_edge + dxldy;
		xr_edge = xr_edge + dxrdy;

	}

	if( dxdy1 < dxdy2 )
	{
		dxldy = (float)(x2-x1)/(y2-y1);
	}
	else
	{
		dxrdy = (float)(x2-x1)/(y2-y1);
	}
	for(int y=y2; y<y1; y++)
	{
		for(int x=int(xl_edge); x<int(xr_edge); x++)
		{
			this->fast_sp(x,y,this->rect.Pitch,color);
		}

		xl_edge = xl_edge + dxldy;
		xr_edge = xr_edge + dxrdy;
	}
}



void graphicsInterface::drawline(int start_x, int start_y, int stop_x, int stop_y, unsigned int color) {

	//Lock(); assumes its locked

	int y_unit,x_unit;
	int ydiff = stop_y-start_y;
	if(ydiff<0)
	{
		ydiff = -ydiff;
		y_unit=-1;
	}
	else
	{
		y_unit =1;
	}
	int xdiff=stop_x-start_x;
	if(xdiff<0)
	{
		xdiff=-xdiff;
		x_unit = -1;
	}
	else
	{
		x_unit = 1;
	}

	int error_term=0;
	if(xdiff>ydiff)
	{
		int length=xdiff+1;
		for(int i = 0; i <length; i++)
		{
			//SetPixel(dc,start_x,start_y,color);
			fast_sp(start_x,start_y,rect.Pitch,color);
			start_x += x_unit;
			error_term+=ydiff;
			if(error_term>xdiff)
			{
				error_term-=xdiff;
				start_y+=y_unit;
			}
		}
	}
	else
	{
		int length = ydiff+1;
		for(int i = 0; i < length; i++)
		{
			//SetPixel(dc,start_x,start_y,color);
			fast_sp(start_x,start_y,rect.Pitch,color);
			start_y += y_unit;
			error_term+=xdiff;
			if(error_term>0)
			{
				error_term-=ydiff;
				start_x += x_unit;
			}

		}
	}

}



}
