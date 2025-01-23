#ifndef _MASTERX_H_
#define _MASTERX_H_

#include<SDL2/SDL.h>
#include"SDL_mxf.h"
#include<stdarg.h>
#include<stdlib.h>

#ifdef FOR_WASM
#include<emscripten/emscripten.h>
#endif

extern int MasterMain(char *line);
#define MASTERSCREEN int
#define VK_DOWN SDLK_DOWN
#define VK_UP SDLK_UP
#define VK_LEFT SDLK_LEFT
#define VK_RIGHT SDLK_RIGHT
#define VK_A 'a'
#define VK_S 's'
#define VK_BACK SDLK_BACKSPACE
#define VK_RETURN SDLK_RETURN
#define DIK_A SDL_SCANCODE_A
#define DIK_S SDL_SCANCODE_S
#define DIK_UP SDL_SCANCODE_UP
#define DIK_LEFT SDL_SCANCODE_LEFT
#define DIK_RIGHT SDL_SCANCODE_RIGHT
#define DIK_DOWN SDL_SCANCODE_DOWN

extern SDL_Surface *front;

enum { ID_INTRO ,ID_START, ID_OPTIONS, ID_GAME, ID_GAMEOVER, ID_ABOUT, ID_GINTRO, ID_LAUNCH };

class MasterScreen {
public:

	SDL_Surface *front;
	SDL_Surface *back;
	MasterScreen() {
		front = 0;
	}
	~MasterScreen() {
	}
};

class MasterText
{
public:
	MasterScreen*  scr;
	SDL_Font *font;
	Uint32 bk;
	Uint32 text;

	MasterText() {
		font = SDL_InitFont("t.mxf");
	}

	~MasterText() {
		if(font) {
			SDL_FreeFont(font);
			font = 0;
		}
	}


	 void init(MasterScreen *scr) {
		this->scr = scr;
	}

	 void printtext(char *str, int x, int y) {
		SDL_PrintText(scr->front, font, x,y, text, str);
	}

	 void sprinttext(int x, int y, char *str, ...) {
		char text[5000];
		va_list list;
		va_start(list, str);
		vsprintf((char*)text,str,list);
		va_end(list);
		printtext(text, x,y);
	}

	 void printtextrect(char *str, int x, int y, int w, int h) {
		printtext(str, x,y);
	}

	 void printtextunderline(char *str, int x, int y) {
		printtext(str, x,y);
	}

	 void setbkcolor(Uint32 color) {
		// removed only left here for backwards compatbility
	}

	 void settextcolor(Uint32 color) {
		this->text = color;
	}

	 void setfont(SDL_Font *font) {
		this->font = font;
	}

	 void setdefaultfont() {
		// to do

	}

	 void setunderfont(SDL_Font *font) {
		// to do
	}
};

typedef class MasterGraphic
{
	MasterScreen* scr;
	char  filename[50];
	bool ckey;
	Uint32 color_key = 0 ;
public:
	int w;
	int h;
	SDL_Surface *surf;

	 void init(MasterScreen* scr) {
		this->scr = scr;
		color_key = 0;
	}
	 bool LoadGraphic(char* filename) {

#ifdef __EMSCRIPTEN__
		char buffer[4096];
		sprintf(buffer, "/assets/%s", filename);
		surf = SDL_LoadBMP(buffer);
		
		if(surf == NULL) {
			fprintf(stderr, "Error loading image: %s\n", buffer);
			exit(1);
		}

		printf("Loaded: %s\n", buffer);

#else
		surf = SDL_LoadBMP(filename);
		
		if(surf == NULL) {
			fprintf(stderr, "Error loading image: %s\n", filename);
			exit(1);
		}
#endif


		w = surf->w;
		h = surf->h;
		return true;
	}
	 bool LoadGraphic(char* filename,MasterScreen* scr) {
		init(scr);
		this->LoadGraphic(filename);
		w = surf->w;
		h = surf->h;
		return true;
	}
	 void ReLoadGraphic() {
		// left for backwards compatiblity
	}
	 void SetColorKey(Uint32 r) {
		color_key = r;
			SDL_SetColorKey(surf, SDL_TRUE, color_key);

	}
	 void DisplayGraphic(int x, int y) {
		if(surf) {
			SDL_Rect rc = { x,y,surf->w, surf->h };
			SDL_BlitSurface(surf, 0, scr->front, &rc);
		}
	}
	 void DisplayGraphicEx(int bx, int by, int bw, int bh,int image_x, int image_y) {
		if(surf) {
		SDL_Rect rc = { bx,by,bw,bh };
		SDL_Rect rc2 = { image_x, image_y, bw, bh };
		SDL_BlitSurface(surf, &rc, scr->front, &rc2);
		}
	}
	~MasterGraphic() {  if(surf) SDL_FreeSurface(surf); surf = 0; }
	MasterGraphic() { surf = 0; }
	 void Release() {

	}
}MasterSprite;

// left for backwards compatiblity

class MasterSound
{
public:
	 bool LoadSound(void * pds,char *name) {
		return true;
	}

	 bool LoadSoundEx(char filename[]) {
		return true;
	}
	 void Play() {
	}
	 bool PlayStatus() {
	}
	 void Release() {
	}
	MasterSound() {}
	~MasterSound() {}
};

// i didnt implement all the drawing algoirthms i was too lazy
class MasterPaint
{
	MasterScreen* scr;
public:

	void init(MasterScreen *scr) {
		this->scr = scr;
	}

	void mxanglearc(int x, int y, long radius, float startangle, float sweepangle, Uint32 outline) {
	//	__mxhwnd.paint.drawanglearc (x,y,radius,startangle,sweepangle,outline,outline);
	}

	void  mxdrawrect(int x, int y, int x2, int y2,Uint32 fill,Uint32 outline) {
	//	__mxhwnd.paint.drawrect(x,y,x2,y2,D3DCOLOR_XSDL_MapRGB(front->format,GetRValue(fill),GetGValue(fill),GetBValue(fill)));
		SDL_Rect rc = {x,y,x2-x,y2-y};
		SDL_FillRect(scr->front, &rc, outline);
	}
	void  mxdrawroundrect(int x, int y, int x2, int y2,int cw, int ch, Uint32 fill,Uint32 outline) {
	//	__mxhwnd.paint.drawroundrect(x,y,x2,y2,cw,ch,fill,outline);
	}
	void  mxdrawpie(int x, int y, int x2, int y2, int nx, int ny, int nx2, int ny2,Uint32 fill, Uint32 outline) {
	//	__mxhwnd.paint.drawpie(x,y,x2,y2,nx,ny,nx2,ny2,fill,outline);
	}
	void  mxdrawchord(int x, int y, int x2, int y2, int nx, int ny, int nx2, int ny2, Uint32 fill, Uint32 outline) {
	//	__mxhwnd.paint.drawchord(x,y,x2,y2,nx,ny,nx2,ny2,fill,outline);
	}

	int screen_width = 1024; // Example screen width
	int screen_height = 768; // Example screen height

// Wrapper function for setpixel with bounds checking
	void mxsafe_setpixel(void *buf, int x, int y, Uint32 rgbval, int bpp, int pitch) {
    // Perform bounds checking
    	if (x >= 0 && x < screen_width && y >= 0 && y < screen_height) {
        // Call setpixel only if the coordinates are within bounds
        	setpixel(buf, x, y, rgbval, bpp, pitch);
    	}
	}
	void mxdrawellipse(int x, int y, int x2, int y2, Uint32 fill, Uint32 outline) {
		// Compute the center and radii of the ellipse
		int a = (x2 - x) / 2; // Semi-major axis (width / 2)
		int b = (y2 - y) / 2; // Semi-minor axis (height / 2)
		int xc = x + a;       // Center x-coordinate
		int yc = y + b;       // Center y-coordinate

		int a2 = a * a;
		int b2 = b * b;

		int x0, y0;
		int dx, dy;
		int d1, d2;

		// Lock the surface buffer before drawing
		void *buf = lock(scr->front, scr->front->format->BitsPerPixel);

		// Region 1 (0 to b)
		x0 = 0;
		y0 = b;
		d1 = b2 - a2 * b + a2 / 4;

		while (a2 * y0 > b2 * x0) {
			// Plot points in all 4 quadrants with bounds checking
			mxsafe_setpixel(buf, xc + x0, yc + y0, outline, scr->front->format->BitsPerPixel, scr->front->pitch);
			mxsafe_setpixel(buf, xc - x0, yc + y0, outline, scr->front->format->BitsPerPixel, scr->front->pitch);
			mxsafe_setpixel(buf, xc + x0, yc - y0, outline, scr->front->format->BitsPerPixel, scr->front->pitch);
			mxsafe_setpixel(buf, xc - x0, yc - y0, outline, scr->front->format->BitsPerPixel, scr->front->pitch);

			if (d1 < 0) {
				x0++;
				d1 += 2 * b2 * x0 + b2;
			} else {
				x0++;
				y0--;
				d1 += 2 * b2 * x0 - 2 * a2 * y0 + b2;
			}
		}

		// Region 2 (b to 0)
		d2 = b2 * (x0 + 0.5) * (x0 + 0.5) + a2 * (y0 - 1) * (y0 - 1) - a2 * b2;

		while (y0 >= 0) {
			// Plot points in all 4 quadrants with bounds checking
			mxsafe_setpixel(buf, xc + x0, yc + y0, outline, scr->front->format->BitsPerPixel, scr->front->pitch);
			mxsafe_setpixel(buf, xc - x0, yc + y0, outline, scr->front->format->BitsPerPixel, scr->front->pitch);
			mxsafe_setpixel(buf, xc + x0, yc - y0, outline, scr->front->format->BitsPerPixel, scr->front->pitch);
			mxsafe_setpixel(buf, xc - x0, yc - y0, outline, scr->front->format->BitsPerPixel, scr->front->pitch);

			if (d2 > 0) {
				y0--;
				d2 += a2 - 2 * a2 * y0;
			} else {
				x0++;
				y0--;
				d2 += 2 * b2 * x0 - 2 * a2 * y0 + a2;
			}
		}

		// Fill the ellipse (if the fill color is different from outline)
		if (fill != outline) {
			for (int ix = xc - a; ix <= xc + a; ix++) {
				for (int iy = yc - b; iy <= yc + b; iy++) {
					int dx = ix - xc;
					int dy = iy - yc;
					if ((dx * dx * b2 + dy * dy * a2) <= (a2 * b2)) {
						mxsafe_setpixel(buf, ix, iy, fill, scr->front->format->BitsPerPixel, scr->front->pitch);
					}
				}
			}
		}

		// Unlock the surface after drawing
		unlock(scr->front);
	}

	void  mxsetpixel(int x, int y, Uint32 rgbval) {
		void *buf = lock(scr->front,scr->front->format->BitsPerPixel);
		setpixel(buf, x,y,rgbval, scr->front->format->BitsPerPixel, scr->front->pitch);
		unlock(scr->front);
	//	__mxhwnd.paint.drawpixel(x,y,rgbval);
	}
	Uint32  mxgetpixel(int x, int y) {
	//	return __mxhwnd.paint.drawgetpixel(x,y);
		void *buf = lock(scr->front,scr->front->format->BitsPerPixel);
		SDL_Color col;
		Uint32 color = getpixel(scr->front, x,y, scr->front->format->BitsPerPixel, scr->front->pitch, &col);
		unlock(scr->front);
		return color;
	}
	void  mxarc(int x1,int x2, int x3, int x4, int x5, int x6, int x7, int x8,Uint32 fill,Uint32 outline) {
	//	__mxhwnd.paint.drawarc(x1,x2,x3,x4,x5,x6,x7,x8,fill,outline);
	}
	void  mxpolygon(const SDL_Rect* point,int n_points,Uint32 fill,Uint32 outline) {
	//	__mxhwnd.paint.drawpolygon(point,n_points,fill,outline);
	}
	void  mxlineto(int x, int y,Uint32 fill,Uint32 outline) {
		//__mxhwnd.paint.drawlineto(x,y,fill,outline);
	}
	void  mxhorizontalline(int x, int x2, int y,Uint32 color) {
    //	__mxhwnd.paint.drawrect(x,y,x2,y+1,D3DCOLOR_XSDL_MapRGB(front->format,GetRValue(color),GetGValue(color),GetBValue(color)));
	}
	void  mxverticalline(int y1,int y2,int x,Uint32 color) {
	//	__mxhwnd.paint.drawrect(x,y1,x+1,y2,D3DCOLOR_XSDL_MapRGB(front->format,GetRValue(color),GetGValue(color),GetBValue(color)));
	}
	void  mxline(int start_x, int start_y, int stop_x, int stop_y,Uint32 color) {
	//	__mxhwnd.paint.drawline(start_x,start_y,stop_x,stop_y,color);
	}
};

enum DisplayColor { COLOR_DEFAULT = 1 };

#define WM_KEYDOWN 1
#define WM_KEYUP 2
#define WM_LBUTTONDOWN 3
#define WM_LBUTTONUP 4
#define WM_ACTIVATEAPP 5

class MasterXHWND
{
	int mscr;

	int pre_time;
	int timer_count;

	bool time_flag;
	bool full_flag;

public:
    SDL_Window *sdl_window;

	int hwnd;
	MasterScreen scr;
	MasterText   text;
	MasterPaint  paint;
//	MasterMedia  media;
	SDL_Joystick *stick;
	SDL_Surface *back;
	long (*event)(unsigned int, unsigned int, unsigned int, unsigned int);
	bool alive;
	unsigned int activeapp;
	bool stretch;
	MasterXHWND() { wnd_instance = this; full_flag = true; }
	~MasterXHWND() { if(scr.front) SDL_FreeSurface(scr.front); SDL_DestroyWindow(sdl_window); SDL_Quit(); }
	void setfullflag(bool flag) {
		this->full_flag = flag;
	}

	 bool CreateMasterX(char* mytitle,int width,int height,DisplayColor color,long (*event)(unsigned int, unsigned int, unsigned int, unsigned int),int instance,int icon,int cursor) {
		wnd_instance = this;
		SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
		sdl_window = SDL_CreateWindow("MasterKong",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          960, 720,
                                          SDL_WINDOW_SHOWN);
		this->scr.front = create_buffer(640, 480, SDL_GetWindowSurface(sdl_window));
		front = scr.front;
		this->event = event;
		text.init(&scr);
		paint.init(&scr);
		if(scr.front) {

			SDL_FillRect(scr.front, 0, 0);
			text.settextcolor(0xFFFFFFFF);
			text.printtext("Loading...", 0,0);
			SDL_BlitScaled(scr.front, 0, SDL_GetWindowSurface(sdl_window), 0);
			//SDL_BlitScaled(scr.front, 0, SDL_GetWindowSurface(sdl_window), 0);
			SDL_UpdateWindowSurface(sdl_window);
			return true;
		}
		return false;
	}

	static MasterXHWND *wnd_instance;;

	static void eventProc() {
		wnd_instance->eventProccess();
	}

	void eventProccess() {
			static SDL_Event e;

			while(SDL_PollEvent(&e)) {
				switch(e.type) {
					case SDL_QUIT:
						alive = false;
						break;
					case SDL_KEYDOWN:
						{
							this->event(0, WM_KEYDOWN, e.key.keysym.sym, 0);
						}
						break;
					case SDL_KEYUP:
						{
							this->event(0, WM_KEYUP, e.key.keysym.sym, 0);
						}
                        break;
				
					case SDL_JOYBUTTONDOWN:
						{
							/*
						0 Triangle
						1 Circle
						2 Cross
						3 Square
						4 Left trigger
						5 Right trigger
						6 Down
						7 Left
						8 Up
						9 Right
						10 Select
						11 Start
						12 Home
						13 Hold
						*/
							int key = 0;
							switch(e.jbutton.button) {
							case 0:
								key  = VK_RETURN;
								break;

							case 2:
								key = VK_A;
								break;
							case 3:
								key = VK_S;
								break;
							case 7:
								key = VK_LEFT;
								break;
							case 8:
								key = VK_UP;
								break;
							case 9:
								key = VK_RIGHT;
								break;
							case 6:
								key = VK_DOWN;
							}
							this->event(0, WM_KEYDOWN, key, 0);

						}
						break;
					case SDL_JOYBUTTONUP:
						{
							int key = 0;
						switch(e.jbutton.button) {
							case 0:
								key  = VK_RETURN;
								break;

							case 2:
								key = VK_A;
								break;
							case 3:
								key = VK_S;
								break;
							case 7:
								key = VK_LEFT;
								break;
							case 8:
								key = VK_UP;
								break;
							case 9:
								key = VK_RIGHT;
								break;
							case 6:
								key = VK_DOWN;
							}
							this->event(0, WM_KEYUP, key, 0);

						}
						break;
				}

			}
			SDL_FillRect(scr.front, 0, 0);
			render(mscr);
			SDL_BlitScaled(scr.front, 0, SDL_GetWindowSurface(sdl_window), 0);
			SDL_UpdateWindowSurface(sdl_window);
	}

	void (* render) (int xscr);

	 unsigned int InitLoop(void (* renderx) (int xscr) ) {
		render = renderx;
		static SDL_Event e;
		alive = true;

#ifndef FOR_WASM
		while( alive == true ) {
			eventProccess();
		}
#else
		emscripten_set_main_loop(eventProc, 0, 1);
#endif
		SDL_JoystickClose(stick);
		SDL_FreeSurface(back);
		SDL_FreeSurface(scr.front);
		SDL_Quit();
		return 0;

	}
	 void Kill() {
		alive = false;
	}
	 int GetScreen() {
		return this->mscr;
	}
	 void SetScreen(int xmscr) {
		this->mscr = xmscr;
	}

	 void trade_surfaces() {

	}

/*	 LRESULT SendMasterMessage(UINT msg,unsigned int wParam,LPARAM lParam) {
		return SendMessage(__mxhwnd.hwnd,msg,unsigned int,lParam);
	}*/
	 void clear() {}
	 void mx_update() {}

	 bool LoadGraphic(MasterGraphic* g, char* filename) {
		return g->LoadGraphic(filename,&scr);
	}
	 bool LoadSound(MasterSound* s,char resourceID[]) {
		return s->LoadSoundEx(resourceID);
	}
	 bool KeyCheck(int key) {
//		return __mxhwnd.input.key.KeyCheck(key);
		const Uint8 *keys = SDL_GetKeyboardState(0);
		if(keys[key])
			return true;
		
		int button = 0;
		switch(key) {
			case VK_LEFT:
				button = 7;
				break;
			case VK_RIGHT:
				button = 9;
				break;
			case VK_DOWN:
				button = 6;
				break;
			case VK_UP:
				button = 8;
				break;
			case VK_A:
				button = 2;
				break;
			case VK_S:
				button = 3;
				break;
			case VK_RETURN:
				button = 0;
				break;
		}

		if(SDL_JoystickGetButton(stick,button)) 
			return true;
		

		return false;
	}

	int GetFX(int w,int x, int nw) {
		float xp = (float)x * (float)w / (float)nw;		
		return (int)xp;
	}

	int GetFZ(int h, int y, int nh) {
		float yp = (float)y * (float)h / (float)nh;
		return (int)yp;
	}

	void stretch_blt(SDL_Surface *surf, SDL_Rect *rc, SDL_Surface *where, SDL_Rect *rc2) {
		int i,z;
		void *buf, *buf2;

		SDL_FillRect(where, 0, 0);
		buf = lock(surf, surf->format->BitsPerPixel);
		buf2 = lock(where, surf->format->BitsPerPixel);

		for(i = 0; i < rc2->w; i++) {
			for(z = 0; z < rc2->h; z++) {
				static unsigned long color = 0;
				static int cx, cy;
				cx = GetFX(rc->w, i, rc2->w);
				cy = GetFZ(rc->h, z, rc2->h);
				SDL_Color col;
				color = getpixel(surf, cx,cy,surf->format->BitsPerPixel, surf->pitch, &col);
				if(color != 0)
				setpixel(buf2, i, z, color, where->format->BitsPerPixel, where->pitch);
			}
		}

		unlock(where);
		unlock(surf);

	}


	 bool AsyncKeyCheck(int key) {
		//if(GetAsyncKeyState(key))
		//	return true;
		const Uint8 *keys = SDL_GetKeyboardState(0);
		if(keys[key])
			return true;
		return false;
	}

	SDL_Surface *create_buffer(int w, int h, SDL_Surface *front) {
				return SDL_CreateRGBSurface(SDL_SWSURFACE, w,h, front->format->BitsPerPixel,front->format->Rmask, front->format->Gmask, front->format->Bmask, front->format->Amask);
	}

	 void InitTimer(int id, int interval) {
		//SetTimer(__mxhwnd.hwnd,id,interval,NULL);
	}
	 void DestroyTimer(int id) {
		//KillTimer(__mxhwnd.hwnd,id);
	}
	 void GetMousePosWin(long& x, long& y) {
		x = 0, y = 0;
	}
	 void HideCursor() {
	
	}
	 void RestoreCursor() {
	}
	 bool TimeChange() {
		if(pre_time == timer_count) {
			return false;
		}
		pre_time = timer_count;
		return true;
	}
	 void ChangeTime() {
		static int sys_clock = SDL_GetTicks();
		if(timer_count < sys_clock) {
			timer_count++;
		}
		else
		if(timer_count > sys_clock)
		{
			timer_count = SDL_GetTicks();
		}

	}
	 void SetTimeFlag() {
		time_flag = TimeChange();
	}
	 bool SyncChange() {
		return true;
	}

	 SDL_Font * CreateMasterFont(char* font_name,int size) {
		return SDL_InitFont(font_name);
	}
};


// left for backwards compatiblity
class MasterJoyStick {
public:

	bool JoyExisit() {
		return false;

	}
	bool SetupJoystick()  {
		return true;
	}
	bool JoyBackward() {
			return false;
	}

	bool JoyForward() {

		return false;
	}
	int JoyButtonDown() {

		return -1;
	}

};

struct ScreenObject
{
	MASTERSCREEN sub_screen;
	MasterXHWND* mxhwnd;
	
	 void Init(MasterXHWND* mxhwndx)
	{
		mxhwnd = mxhwndx;
	}
	
	 MASTERSCREEN getscreen()
	{
		return sub_screen;
	}
	
	 void setscreen(MASTERSCREEN xscreen)
	{
		sub_screen = xscreen;
	}
};

class MasterGameObject 
{
public:
	virtual void load() = 0;
	virtual void update() = 0;
};

#endif
