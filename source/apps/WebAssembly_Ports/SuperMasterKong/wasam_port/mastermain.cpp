/*

   - SuperMasterKong -
  written by Jared Bruni
  www.LostSideDead.com

  "Open Source, Open Mind"

  this program is freeware, you may redistrubite it any way you 
  see fit. yes that means you can change it , and say you wrote it.
  change the graphics, and do whatever you want with it

  you cant put a $ on thoughts, they are free
  
	a beautiful MasterPeace (-<)

*/

// -LostSideDead-
#include"thehead.h"

MasterXHWND mxhwnd;// the global instance of the MasterXHWND class concept is similar to unversial mind
// the MasterXHWND class is the main class used throughout the code
// to manipulate the computers hardware, it provides one easy to use
// encapsulated interface to get at the keyboard, render to the screen
// and manipulate the computer in other ways 
// one global instance of this class is made
// and all the other classes use it 

MasterXHWND *MasterXHWND::wnd_instance = 0;

Intro intro;// instance of the Intro class ; for Intro screen
Start start;// instance of the Start class ; for Start screen
GameUniverse game; // instance of the GameUniverse ;
// this class is where all the games objects reside, the particle emiter,
// hero, grandma, and other objects exisit in the GameUniverse.

SoundHandler sound; // the sound handler , plays the wav's

// global variables
int current_y = 0, id_screen = 0, id_about = 0;
score user_score;

// function prototypes
inline void init();
long event(unsigned int, unsigned int msg, unsigned int wparam , unsigned int lparam);


// MasterMain - program entry point
// this is where are program begins
// we initalize the mxhwnd object 
// if it returns succesfull we pass it a pointer to the render 
// function and put it into a loop
int main()
{
	mxhwnd.setfullflag(false);
	if(mxhwnd.CreateMasterX("SuperMasterKong",1024,768,COLOR_DEFAULT,event, 0, 0, 0))
	{
		init();
		return (mxhwnd.InitLoop(render));
	}

	return (0);
}

// initiliaze - this is called if CreateMasterX returns succesfull
// it loads the intro, start, and game objects
// hides the cursor, initalizes the timer for sync changes
// and sets the current screen
inline void init()
{
#ifdef WIN2K_XP_2003
	mxhwnd.InitTimer(1,50);// initalize the Timer for Sync changes
	// set the speed for windows 2000 / XP / 2003
#else
	mxhwnd.InitTimer(1,1); // initalize the Timer for Sync changes
	// set the speed for windows 98
#endif
	mxhwnd.SetScreen(ID_INTRO);
	intro.load();
	start.load();
	game.load();
	sound.load();
	mxhwnd.HideCursor();
}


long event(unsigned int, unsigned int msg, unsigned int wparam , unsigned int lparam)
{
	
	switch(msg)
	{
	case 1:
		{
			if(wparam == SDLK_ESCAPE)
				mxhwnd.Kill();

			switch(mxhwnd.GetScreen())
			{
			case ID_START:
				start.keypress(wparam);
				start.onchar(wparam);
				break;
			case ID_GAME:
				game.keypress(wparam);
				break;
			}
			
		}
		break;
	break;
	default:
	break;
	//	return DefWindowProc(hwnd,msg,wparam,lparam);
	}
	return (0);
}


// render function
// this function is called during the idle proccessing time
// it draws the apporpiate screen, based on the screen variable
void render(int screen)
{
	mxhwnd.SetTimeFlag(); // set the time flag for Sync changes
	
	switch(screen)
	{
	case ID_INTRO:
		intro.update();
		break;
	case ID_START:
		start.update();
		break;
	case ID_GAME:
	    SDL_Delay(20);
		game.update();
		break;
	}
}


// imperfections in the game engine
// since C++ is so flexible we can still get it to work
// by creating global functions
// we can then use them inside the classes to manipulate
// the instances of the classes
// its kind of a way to get around how I designed the code
// since most of the classes are nested
// I tried using forward refrences, but it kept giving me 
// a undefined type error

// this_callback is called when the hero shoots the grandma
// on the about screen, after the animation is completed
// a particle is released by calling the start object's
// release_particle method
inline void this_callback()
{
	start.release_particle();
}

// newgame
// this function is called when a new game is going to be started
// it calls the game object's newgame method
inline void newgame()
{
	game.newgame();
}

// scores - release input
// this is for the highscores menu
// it calls the start object's release input method which 
// will allow the user to input there name if they made it into the
// highscores menu 
inline void scores_releaseinput()
{
	start.releaseinput();
}

// release bullet
// this is called when the Hero shoots his shot gun, and the animation
// is over so the bullet particle needs to be released
inline void releasebullet()
{
	game.release_bullet();
}

// grandma died
// this is called when the grandma dies
// ex: she is shoot by the Hero
// this function is called by the grandma callback
// so that the animation will display
inline void grandma_died()
{
	game.grandmadied();
}

// set the credits
// so that it will say something different if they won the game
// or if they just selected about
inline void about_setcredits()
{
	start.setcredits();
}
// clear the credits 
// by calling the start objects clear credits method
// which really just calls the about object (which is made a instance of inside of the start object)
// clearcredits method
inline void about_clearcredits()
{
	start.clearcredits();
	start.grannyreset();
}

// lives cheat
// function to return the variable livescheat within the game object
// its to see whether or not they entered the 99 lives cheat code
// if they entered the cheat code they cannot put there name in the
// highscores, they can however view the special credits when they 
// beat the game
const inline bool livescheat() 
{
	return (game.lives_cheat);
}

// getstartbmp
// function to return a pointer to a MasterGraphic object
// basicly the start screen image
// this is so I dont have to load it twice, to save ram
// so I can load it in the start object and then use it
// in any one of the other classes, specificly the enter level class
MasterGraphic *getstartbmp()
{
	return ((MasterGraphic*)&start.start);
}