/* masterkong ++ using MasterX SDK
 written by jared bruni
 www.lostsidedead.com
 open source, open mind
*/




#include "masterx.h"
#include "resource.h"
#include <time.h>
#include <stdio.h>
#include <fstream>

using namespace std;

#define COLORREF Uint32

extern MasterXHWND mxhwnd;

// callback functions
void update(MASTERSCREEN);
long MasterProc(unsigned int,unsigned int,unsigned int,unsigned int);
int rows(int x);
void thegameover(bool win);





// enumerated constants



// structure for the games player
struct Player
{
	int difficulty;
	int lives;
	bool joystick;

	inline Player()
	{
		difficulty = 3;
		lives = 5;
		joystick = false;
	}
};

extern Player player;

// ADT game object


// sound handler

const int SOUND_MAX = 2;

enum { OPEN = 0, FIRE = 1};

// the sound handler for playing game sounds
class SoundHandle : public MasterGameObject
{
public:

	MasterSound sound[SOUND_MAX];
	
	virtual void load()
	{
	}

	virtual void update()
	{
	}
	
	void Play(int);
	bool Status(int);
};


extern SoundHandle soundhandle;

// size of the tile map
const int LEVEL_SIZE = 700-4+24;

// level block structure
struct LevelBlock
{
	int block;
	bool solid;
};
// levelmap structure
struct LevelMap
{
	int gsp;
	int hsp;
	LevelBlock blocks[LEVEL_SIZE];
	int items[LEVEL_SIZE];

	// constructor for level map
	inline LevelMap()
	{
		for(int i = 0; i < LEVEL_SIZE; i++)
		{
			blocks[i].block = 0;
			blocks[i].solid = false;
			items[i] = 0;
		}
	}

	// calculate the position of an object in the grid, by its position
	inline void GetPointFromPos(int pos,int& x, int& y)
	{
		x = 0;
		y = 0;

		int startby = 75;
		
		int bx,by;
		bx = 75; by = startby;
		int gcount = 0;
		
		
		for(int i = 0; i < 700-4+24; i++)
		{
			if(i == pos)
			{
				x = bx;
				y = by;
				return;
			}
			
			by = by + 16;
			gcount++;
			if(gcount > 23)
			{
				gcount = 0;
				by = startby;
				bx = bx + 16;
			}
			
		}
	}
};

extern LevelMap level;


// objects , to go inside of the game objects
// the grandma of death
class Grandma : public MasterGameObject
{
public:

	bool gdir;
	int  gx;
	int  gy;
	int  pos;
	int  spos;
	bool bpos;
	int  rcount;
	int  oldpos;
	int  oc;



	// load up the graphics
	virtual void load()
	{
		gdir = true;
		gx = 100;
		gy = 100;

		// load up the pictures of grandma
		mxhwnd.LoadGraphic(&granny[0],"grandma_1.bmp");
		mxhwnd.LoadGraphic(&granny[1],"grandma_2.bmp");
		mxhwnd.LoadGraphic(&granny[2],"grandma_3.bmp");
		mxhwnd.LoadGraphic(&granny[3],"grandma_4.bmp");
		mxhwnd.LoadGraphic(&granny[4],"grandma_5.bmp");

		mxhwnd.LoadGraphic(&granny[5],"grandma_die1.bmp");
		mxhwnd.LoadGraphic(&granny[6],"grandma_die2.bmp");
		mxhwnd.LoadGraphic(&granny[7],"grandma_die3.bmp");
		mxhwnd.LoadGraphic(&granny[8],"grandma_die4.bmp");
		mxhwnd.LoadGraphic(&granny[9],"grandma_die5.bmp");
		mxhwnd.LoadGraphic(&granny[10],"grandma_die6.bmp");
		

		mxhwnd.LoadGraphic(&grannyl[0],"grandmal_1.bmp");
		mxhwnd.LoadGraphic(&grannyl[1],"grandmal_2.bmp");
		mxhwnd.LoadGraphic(&grannyl[2],"grandmal_3.bmp");
		mxhwnd.LoadGraphic(&grannyl[3],"grandmal_4.bmp");
		mxhwnd.LoadGraphic(&grannyl[4],"grandmal_5.bmp");

		mxhwnd.LoadGraphic(&grannyl[5],"grandma_die1_l.bmp");
		mxhwnd.LoadGraphic(&grannyl[6],"grandma_die2_l.bmp");
		mxhwnd.LoadGraphic(&grannyl[7],"grandma_die3_l.bmp");
		mxhwnd.LoadGraphic(&grannyl[8],"grandma_die4_l.bmp");
		mxhwnd.LoadGraphic(&grannyl[9],"grandma_die5_l.bmp");
		mxhwnd.LoadGraphic(&grannyl[10],"grandma_die6_l.bmp");


		img_len = 10;


		for(int i = 0; i <= img_len; i++)
		{
	
		granny[i].SetColorKey(SDL_MapRGB(front->format,248,0,248));
		grannyl[i].SetColorKey(SDL_MapRGB(front->format,248,0,248));


		}


		cur_granny = 0;
		oldpos = 0;

		alive = true;
		death = false;
		spos = 0;

	}

	// frame update
	virtual void update()
	{
		if(alive == true)
		{
	

		draw();
		if(mxhwnd.SyncChange())
		{
			logic();
		}

		}
	}

	// grandma draw
	void draw()
	{
		if(bpos == true)
		{
			level.GetPointFromPos(pos,gx,gy);
		}

		if(gdir == true)
		{

		granny[cur_granny].DisplayGraphic(gx,gy);

		}
		else
		{
			grannyl[cur_granny].DisplayGraphic(gx,gy);

		}
	}

	// on logic
	void logic()
	{
		if(bpos == true)
		{
			if(death == false)
			{

			procblock();
			ai();

			}
			else
			{
				int x;
				x = getnextdie();
				if(x == -1)
				{
					death = false;
					alive = false;
					cur_granny = 0;
					thegameover(true);
				}
				else
				{
					cur_granny = x;
				}
		}
		}
	}

	// set grandma position
	void SetPos(bool dir, int x, int y, int img)
	{
		cur_granny = img;
		gdir = dir;
		gx = x;
		gy = y;
	}

	void procblock();
	void ai();
	void moveleft();
	void moveright();
	void releaseprojectile();
	void die();
	int  getnextwalk();
	int  getnextdie();
	void birth();
	int  st_count;

	bool alive;
	bool death;



private:

	MasterGraphic granny[11];
	MasterGraphic grannyl[11];
	int cur_granny;
	int img_len;

};


// intro game object
class Intro : public MasterGameObject
{
public:
	virtual void load()
	{
		mxhwnd.LoadGraphic(&introg,"intro.bmp");
	}

	virtual void update()
	{
		drawbg();
		if(mxhwnd.SyncChange())
		{
			onlogic();
		}
	}

	void drawbg();
	void onlogic();
private:

	MasterGraphic introg;
	int gpos;


};

// start game object
class Start : public MasterGameObject
{
public:

	virtual void load()
	{
		mxhwnd.LoadGraphic(&startg,"start.bmp");
		mxhwnd.LoadGraphic(&starta[0],"m1.bmp");
		mxhwnd.LoadGraphic(&starta[1],"m2.bmp");
		mxhwnd.LoadGraphic(&bridge,"bridge.bmp");

		grandmax.load();
		gdir = true;
		gx = 10;
		gy = 150;
		gi = 0;
		grandmax.SetPos(gdir,gx,gy,gi);
		stx = 190;
		sty = 290;
		menupos = 0;

	}

	virtual void update()
	{
		ondraw();
		if(mxhwnd.SyncChange())
		{
			onlogic();
		} 
	
	}

	void ondraw();
	void drawbridge();
	void onlogic();
	void logicgrandma();
	void drawstart();
	void keypress(unsigned int wParam);


private:

	MasterGraphic startg;
	MasterGraphic starta[2];
	MasterGraphic bridge;
	Grandma grandmax;

	bool startgo;
	bool gdir;
	int  gx;
	int  gy;
	int  gi;
	int  sx;
	int  si;
	int  stx;
	int  sty;
	int  menupos;

 
};

// options game object
class Options : public MasterGameObject
{
public:

	virtual void load()
	{
	
		mxhwnd.LoadGraphic(&starta[0],"m1.bmp");
		mxhwnd.LoadGraphic(&starta[1],"m2.bmp");
		mxhwnd.LoadGraphic(&lostx,"lostx.bmp");

		bx = 0;
		cpos = 0;

	}

	virtual void update()
	{
		ondraw();
		if(mxhwnd.SyncChange())
		{
			onlogic();
		}
	}

	void ondraw();
	void drawop();
	void onlogic();
	void keypress(unsigned int wParam);
private:
	MasterGraphic starta[2];
	int bx;
	int cpos;
	MasterGraphic lostx;



};

void loadlevel(char* level);
const int TILEMAX = 5;

// particle structure and handler (engine), for all the games particles (bullets, && || projectiles)

enum PARTICLETYPE { BULLET , BALL };

struct Particle
{
	int pos;// position
	bool dir; // direction its traveling (for bullets)
	bool udir;
	PARTICLETYPE ptype;// particle type
	bool on; // whether particle is active

	inline Particle()
	{
		pos = 0;
    	ptype = BULLET;
		on = false;
	}
};

const int MAX_PARTICLE = 100;

// ParticleHandler, proccess all the particles (projectiles)
class ParticleHandler : public MasterGameObject
{
public:

	Particle particles[MAX_PARTICLE];

	virtual void load()
	{


	}

	virtual void update()
	{
		drawparticles();
		if(mxhwnd.SyncChange())
		{
			procparticles();
		}
	}

	int getoffparticle();
	void drawparticles();
	void procparticles();
	void releaseparticle(int pos, bool dir, PARTICLETYPE ptype);
	void releaseparticle(int pos, bool dir, bool xdir, PARTICLETYPE ptype);
	void killparticle(int pos);


private:

};

extern ParticleHandler hparticle;


// games hero object

const int HEROMAX = 10;

// hero class
class Hero : public MasterGameObject
{
public:

	virtual void load()
	{
		mxhwnd.LoadGraphic(&herog[0],"hero1.bmp");
		mxhwnd.LoadGraphic(&herog[1],"hero2.bmp");
		mxhwnd.LoadGraphic(&herog[2],"hero3.bmp");
		mxhwnd.LoadGraphic(&herog[3],"hero4.bmp");
		mxhwnd.LoadGraphic(&herog[4],"hero_jump1.bmp");

		mxhwnd.LoadGraphic(&herog[5],"hero_shot1.bmp");
		mxhwnd.LoadGraphic(&herog[6],"hero_shot2.bmp");
		mxhwnd.LoadGraphic(&herog[7],"hero_shot3.bmp");
		mxhwnd.LoadGraphic(&herog[8],"hero_shot4.bmp");
		mxhwnd.LoadGraphic(&herog[9],"hero_shot5.bmp");

		mxhwnd.LoadGraphic(&herogl[5],"hero_shot1_l.bmp");
		mxhwnd.LoadGraphic(&herogl[6],"hero_shot2_l.bmp");
		mxhwnd.LoadGraphic(&herogl[7],"hero_shot3_l.bmp");
		mxhwnd.LoadGraphic(&herogl[8],"hero_shot4_l.bmp");
		mxhwnd.LoadGraphic(&herogl[9],"hero_shot5_l.bmp");

		mxhwnd.LoadGraphic(&herogl[0],"hero1_l.bmp");
		mxhwnd.LoadGraphic(&herogl[1],"hero2_l.bmp");
		mxhwnd.LoadGraphic(&herogl[2],"hero3_l.bmp");
		mxhwnd.LoadGraphic(&herogl[3],"hero4_l.bmp");
		mxhwnd.LoadGraphic(&herogl[4],"hero_jump1_l.bmp");

		hero_pos = 19+24;
		start_pos = 19+24;
		hdir = true;
		cur_hero = 0;
		int i;


		for(i = 0; i < 4; i++)
		{
			herog[i].SetColorKey(SDL_MapRGB(front->format,255,0,255));
			herogl[i].SetColorKey(SDL_MapRGB(front->format,255,0,255));
		}

		herog[4].SetColorKey(SDL_MapRGB(front->format,248,0,248));
		herogl[4].SetColorKey(SDL_MapRGB(front->format,248,0,248));

		for(i = 5; i <= 9; i++)
		{
			herog[i].SetColorKey(SDL_MapRGB(front->format,248,0,248));
			herogl[i].SetColorKey(SDL_MapRGB(front->format,248,0,248));
		}

		speedc = 0;
		speed = 1;
		ijump = false;
		ifire = false;
		firepos = 0;

	}

	virtual void update()
	{
		ondraw();
		if(mxhwnd.SyncChange())
		{
			onlogic();
		}
	}

	int hero_pos;
	bool hdir;
	int cur_hero;


	void ondraw();
	void onlogic();
	void setpos(bool hdir, int hero_pos);
	void moveleft();
	void moveright();
	void movedown();
	void moveup();
	void fire();
	void jump();
	void keypress(unsigned int wParam);
	void keyrelease(unsigned int wParam);
	void hlogic();
	void procblock();
	void procfire();
	void die();

	int start_pos;

private:

	MasterGraphic herog[HEROMAX];
	MasterGraphic herogl[HEROMAX];

	int speed;
	int speedc;
	bool ijump;
	int  jcount;
	bool ifall;
	bool ifire;
	int  firepos;

	int getnextwalk();



};


// game, game object
class Game : public MasterGameObject
{
public:

	virtual void load()
	{
		mxhwnd.LoadGraphic(&tiles[0],"brick.bmp");
		mxhwnd.LoadGraphic(&tiles[1],"grass.bmp");
		mxhwnd.LoadGraphic(&tiles[2],"bluebrick.bmp");
		mxhwnd.LoadGraphic(&tiles[3],"bluesky.bmp");
		mxhwnd.LoadGraphic(&tiles[4],"eblock.bmp");
		mxhwnd.LoadGraphic(&tiles[5],"black.bmp");

		hero.load();
		grandma.load();
		grandma.bpos = true;
		grandma.pos = rows(3) + 2;

		player.joystick = stick.SetupJoystick();
	}

	virtual void update()
	{
		ondraw();
		if(mxhwnd.SyncChange())
		{
	
		}
		hero.update();
		grandma.update();
		hparticle.update();
	}

	void ondraw();
	void drawmap();

	Hero hero;
	Grandma grandma;

	MasterJoyStick stick;


private:
	MasterGraphic tiles[TILEMAX]; // tiles

};

enum { ID_WIN, ID_LOSE };

// game over, game object
class GameOver : public MasterGameObject
{
public:
	virtual void load()
	{
		screen = ID_WIN;
		mxhwnd.LoadGraphic(&graph,"m1.bmp");
	}

	virtual void update()
	{
		ondraw();
		if(mxhwnd.SyncChange())
		{
			onlogic();
		}

	}

	void ondraw();
	void onlogic();
	void passgameover( bool win );
	void keypress(unsigned int wParam);

private:

	MASTERSCREEN screen;
	MasterGraphic graph;



};

// credits structure


// about game object
class About : public MasterGameObject
{
public:
	virtual void load()
	{
		loadfonts();
		mxhwnd.LoadGraphic(&mx,"mx.bmp");
	}

	virtual void update()
	{
		back_color = SDL_MapRGB(front->format,back_fade,back_fade,back_fade);
		ondraw();
		if(mxhwnd.SyncChange())
		{
			onlogic();
		}

	}

	inline About()
	{
		back_fade = 0;
		fade_dir = true;
	}

	void keypress(unsigned int wParam);


private:

	void ondraw();
	void onlogic();
	void loadfonts();
	

	int back_fade;
	bool fade_dir;
	COLORREF back_color;
	MasterGraphic mx;


}; 

// game intro

class GIntro : public MasterGameObject
{
public:

	virtual void load()
	{

		mxhwnd.LoadGraphic(&granl,"grandma_1.bmp");
		mxhwnd.LoadGraphic(&herol,"hero1_l.bmp");

		granl.SetColorKey(SDL_MapRGB(front->format,248,0,248));
		herol.SetColorKey(SDL_MapRGB(front->format,255,0,255));

		curmenu = 0;
	}

	virtual void update()
	{
		ondraw();
		if(mxhwnd.SyncChange())
		{
			onlogic();
		}
	}

	void keypress(unsigned int wParam);

private:

	void ondraw();
	void onlogic();

	MasterGraphic herol;
	MasterGraphic granl;

	int curmenu;
};


Hero* GetHero();
Grandma* GetGrandma();
MasterJoyStick* getstick();



// *********************************************************************** www.lostsidedead.com
