/*

  - SuperMasterKong -
  written by Jared Bruni
  www.LostSideDead.com
  
	"Open Source, Open Mind"
	
	  this program is freeware, you may redistrubite it any way you 
	  see fit. yes that means you can change it , and say you wrote it.
	  change the graphics, and do whatever you want with it
	  
		you cant put a $ on thoughts , they are free
		
		  
			i sort of changed up my style for this one
			using nested classes, and multiple inheritance
			still the same old stuff tho
			C++ my fav ;]
			notice the Sync so works on windows 98/2000/2003/XP
			
			
			  this game was inspried by a game I found on SuSE 9.0
			  called MadBomber
			  I just changed it (totaly reprogrammed it
			  for windows) and added my favorite characters to the mix
			  Super Stoner, and the Evil Grandmother Zombie
			  changed up the style of the gameplay made it mario like
			  but the concept is the same, someone throws something
			  for the user to grab
			  
				enjoy =)
				
				  " shine on you crazy diamond " - pink floyd
				  
					
					  - Jared Bruni
					  
*/

#pragma once
#define Sync if(mxhwnd.SyncChange())
#define MASTERXSDK
#include"masterx.h"
#include<fstream>
#include"resource.h"
#define WIN2K_XP_2003

using namespace std;


// enumerated constats
enum SCREENT { ID_INTRO = 1 , ID_START, ID_GAME, ID_HIGHSCORES, ID_ABOUT, ID_ENTERLEVEL, ID_USERWON };
enum direction { LEFT = 1, RIGHT, UP, DOWN };
enum { BULLET = 1, ORB };

// function prototypes
long event(struct HWND__* hwnd, unsigned int msg, unsigned int wparam, long lparam);
void render(int screen);
inline void this_callback();
inline void newgame();
inline void scores_releaseinput();
inline void releasebullet();
inline void grandma_died();
inline void about_setcredits();
inline void about_clearcredits();
const inline bool livescheat();
MasterGraphic *getstartbmp();

// the users score
struct score {
	int user_score;
	int orbs;
	int orbs_missed;
};

// global variables
extern MasterXHWND mxhwnd; 
extern int current_y, id_screen,id_about;
extern score user_score;

enum { FIRE = 0, OPEN, ORB_SND, SCREAM }; // enumerated constants to stand for the sounds
// the SoundHandler class, handles playing the games wav files
// in one easy to use interface that all the games classes can access
class SoundHandler : public MasterGameObject{

public:

	virtual void load()
	{
		mxhwnd.LoadSound(&sounds[0],"fire.wav");
		mxhwnd.LoadSound(&sounds[1],"open.wav");
		mxhwnd.LoadSound(&sounds[2],"orb.wav");
		mxhwnd.LoadSound(&sounds[3],"scream.wav");
	}

	virtual void update(){}

	inline void play(int sound)
	{
		sounds[sound].Play();
	}

	inline int playstatus(int sound)
	{
		return sounds[sound].PlayStatus();
	}

private:

	MasterSound sounds[4];
};

extern SoundHandler sound; // global instance of the SoundHandler class


// the base object class that all the game's objects will inhert from 
// ex: the hero, grandma, particles, and other animated objects
class Object {
	
public:
	
	inline Object()
	{
		reset();
	}
	
	inline void setcords(int cur_x, int cur_y)
	{
		this->cur_x = cur_x;
		this->cur_y = cur_y;
	}
	
	inline void setpos(int cur_pos_i, int cur_pos_z)
	{
		this->cur_pos_i = cur_pos_i;
		this->cur_pos_z = cur_pos_z;
	}
	
	inline void setdir(direction dir)
	{
		this->dir = dir;
	}
	
	inline void setimg(int cur_img)
	{
		this->cur_img = cur_img;
	}
	
	inline void hide()
	{
		show = false;
	}
	
	inline void unhide()
	{
		show = true;
	}
	
	inline void turn_on()
	{
		on = true;
	}
	
	inline void turn_off()
	{
		on = false;
	}
	
	inline void reset()
	{
		cur_img = 0;
		cur_x = 0;
		cur_y = 0;
		dir = RIGHT;
		show = true;
		cur_pos_i = 0;
		cur_pos_z = 0;
		on = false;
	}
	
	int cur_img, cur_x, cur_y, cur_pos_i, cur_pos_z;
	direction dir;
	bool show;
	bool on;
	
};

// the Hero (Super Stoner)  class
// this class contains the data nessicary for the Hero
// to run, jump, and shoot his shot gun
// it is all encapsulated into a easy to use interface
// and since it inherts from MasterGameObject, all we have to do
// is call the load function on initaliziation, and the update
// function when we render, and the hero is on the screen 
class Hero : public MasterGameObject , public Object { // multiple inheritance =)
	
	
public:
	
	virtual void load()
	{


		mxhwnd.LoadGraphic(&images[0],"images/hero1.bmp");
		mxhwnd.LoadGraphic(&images[1],"images/hero2.bmp");
		mxhwnd.LoadGraphic(&images[2],"images/hero3.bmp");
		mxhwnd.LoadGraphic(&images[3],"images/hero4.bmp");
		
		mxhwnd.LoadGraphic(&images[4],"images/hero_jump1.bmp");
		
		mxhwnd.LoadGraphic(&images[5],"images/hero_shot1.bmp");
		mxhwnd.LoadGraphic(&images[6],"images/hero_shot2.bmp");
		mxhwnd.LoadGraphic(&images[7],"images/hero_shot3.bmp");
		mxhwnd.LoadGraphic(&images[8],"images/hero_shot4.bmp");
		mxhwnd.LoadGraphic(&images[9],"images/hero_shot5.bmp");
		
		
		
		mxhwnd.LoadGraphic(&images_l[0],"images/hero1_l.bmp");
		mxhwnd.LoadGraphic(&images_l[1],"images/hero2_l.bmp");
		mxhwnd.LoadGraphic(&images_l[2],"images/hero3_l.bmp");
		mxhwnd.LoadGraphic(&images_l[3],"images/hero4_l.bmp");
		
		mxhwnd.LoadGraphic(&images_l[4],"images/hero_jump1_l.bmp");
		
		mxhwnd.LoadGraphic(&images_l[5],"images/hero_shot1_l.bmp");
		mxhwnd.LoadGraphic(&images_l[6],"images/hero_shot2_l.bmp");
		mxhwnd.LoadGraphic(&images_l[7],"images/hero_shot3_l.bmp");
		mxhwnd.LoadGraphic(&images_l[8],"images/hero_shot4_l.bmp");
		mxhwnd.LoadGraphic(&images_l[9],"images/hero_shot5_l.bmp");
		
		for(i = 0; i < 10; i++)
		{
			images[i].SetColorKey(SDL_MapRGB(images[i].surf->format, 255,255,255));
			images_l[i].SetColorKey(SDL_MapRGB(images_l[i].surf->format, 255,255,255));
		}
		dir = RIGHT;
		
		
		
		cur_img = 0;
		cur_x = 0;
		cur_y = 0;
		run_on = false;
		jumping_on = false;
		shoot_on = false;
	}
	
	virtual void update()
	{
		if(show == true)
		{
			
			switch(dir)
			{
			case RIGHT:
				images[cur_img].DisplayGraphic(cur_x,cur_y);
				break;
			case LEFT:
				images_l[cur_img].DisplayGraphic(cur_x,cur_y);
				break;
			}
			
		}
		
		if(run_on == true)
		{
			cur_img++;
			if(cur_img >= 4)
			{
				stop_run_cycle();
			}
			return; 
		}
		
		if(shoot_wait == true)
		{
			shot_index++;
			
			if(shot_index > 25)
				shoot_wait = false;
		}
		
		if(shoot_on == true)
		{
			cur_img++;
			if(cur_img > 9)
			{
				cur_img = 0;
				shoot_on = false;
				shoot_wait = true;
				if(t_callback != 0)
					t_callback();
				return;
			}
		}
		
		if(jumping_on == true)
		{
			cur_img = 4;
			return;
		}
		
		
	}
	
	inline void shoot(void (*callback)())
	{
		cur_img = 5;
		shoot_on = true;
		t_callback = callback;
		shot_index = 0;
		shoot_wait = false;
	}
	
	inline void shoot()
	{
		cur_img = 5;
		shoot_on = true;
		t_callback = 0;
		shot_index = 0;
		shoot_wait = false;
	}
	
	inline void jump_on()
	{
		if(shoot_on == false)
		{
			jumping_on = true;
			cur_img = 4;
		}
	}
	
	inline void jump_off()
	{
		if(shoot_on == false && jumping_on == true)
		{
			jumping_on = false;
			cur_img = 0;
		}
	}
	
	const inline bool is_jump_on() const
	{
		return jumping_on;
	}
	
	const inline bool is_shooting() const
	{
		return shoot_on;
	}
	
	const inline bool is_shoot_wait() const
	{
		return shoot_wait;
	}
	
	
	inline void run_cycle()
	{
		if(run_on == false)
		{
			run_on = true;
			cur_img = 0;
		}
	}
	
	inline void stop_run_cycle()
	{
		run_on = false;
		cur_img = 0;
	}
	
	const inline bool is_running() const
	{
		return run_on;
	}
	
private:	
	int i;
	MasterGraphic images[10];
	MasterGraphic images_l[10];
	bool shoot_on, jumping_on, run_on,shoot_wait;
	int shot_index;
	void (*t_callback)();		
};


// this is the Granda Zombie class
// this class is
// dedicated to my grandma
// who bought me my first computer
// this class contains the data nessicary for the Grandma
// to run cycle, move, etc.
// its all encapsluated into a easy to use interface
// and she is displayed simply through the update function
// which is inherited from MasterGameObject
class Grandma : public MasterGameObject , public Object { // multiple inhertiance yay!
	
	
public:
	
	virtual void load()
	{

		mxhwnd.LoadGraphic(&images[0],"images/grandma_1.bmp");
		mxhwnd.LoadGraphic(&images[1],"images/grandma_2.bmp");
		mxhwnd.LoadGraphic(&images[2],"images/grandma_3.bmp");
		mxhwnd.LoadGraphic(&images[3],"images/grandma_4.bmp");
		mxhwnd.LoadGraphic(&images[4],"images/grandma_5.bmp");
		
		mxhwnd.LoadGraphic(&images[5],"images/grandma_die1.bmp");
		mxhwnd.LoadGraphic(&images[6],"images/grandma_die2.bmp");
		mxhwnd.LoadGraphic(&images[7],"images/grandma_die3.bmp");
		mxhwnd.LoadGraphic(&images[8],"images/grandma_die4.bmp");
		mxhwnd.LoadGraphic(&images[9],"images/grandma_die5.bmp");
		
		
		mxhwnd.LoadGraphic(&images_l[0],"images/grandma_1_l.bmp");
		mxhwnd.LoadGraphic(&images_l[1],"images/grandma_2_l.bmp");
		mxhwnd.LoadGraphic(&images_l[2],"images/grandma_3_l.bmp");
		mxhwnd.LoadGraphic(&images_l[3],"images/grandma_4_l.bmp");
		mxhwnd.LoadGraphic(&images_l[4],"images/grandma_5_l.bmp");
		
		mxhwnd.LoadGraphic(&images_l[5],"images/grandma_die1_l.bmp");
		mxhwnd.LoadGraphic(&images_l[6],"images/grandma_die2_l.bmp");
		mxhwnd.LoadGraphic(&images_l[7],"images/grandma_die3_l.bmp");
		mxhwnd.LoadGraphic(&images_l[8],"images/grandma_die4_l.bmp");
		mxhwnd.LoadGraphic(&images_l[9],"images/grandma_die5_l.bmp");
		
		for(i = 0; i < 10; i++)
		{
			images[i].SetColorKey(SDL_MapRGB(images[i].surf->format, 255,255,255));
			images_l[i].SetColorKey(SDL_MapRGB(images_l[i].surf->format, 255,255,255));
		}
		
		
		cur_img = 0;
		dir = RIGHT;
		cur_x = 0;
		cur_y = 0;
	}
	
	virtual void update()
	{
		if(show == true)
		{
			
			switch(dir)
			{
			case RIGHT:
				images[cur_img].DisplayGraphic(cur_x,cur_y);
				break;
			case LEFT:
				images_l[cur_img].DisplayGraphic(cur_x,cur_y);
				break;
			}
			
		}
		
		if(run_on == true)
		{
			cur_img ++;
			if(cur_img >= 5)
			{
				stop_run_cycle();
				return;
			}
		}
		
		if(die_on == true)
		{
			cur_img ++;
			if(cur_img > 9)
			{
				hide();
				die_on = false;
				
				if(t_callback != 0)
				{
					t_callback();
					return;
				}
			}
		}
	}
	
	inline void run_cycle()
	{
		if(run_on == false && die_on == false)
		{
			run_on = true;
			cur_img = 0;
		}
		
	}
	
	inline void stop_run_cycle()
	{
		run_on = false;
		cur_img = 0;
	}
	
	inline void die()
	{
		run_on = false;
		die_on = true;
		cur_img = 5;
		t_callback = 0;
		sound.play(SCREAM);
	}
	
	inline void die(void (*callback)())
	{
		t_callback = callback;
		run_on = false;
		die_on = true;
		cur_img = 5;
		sound.play(SCREAM);
	}
	
private:
	
	MasterGraphic images[10];
	MasterGraphic images_l[10];
	
	bool run_on;
	bool die_on;
	int i;
	
	void (*t_callback)();
	
};

// the Particle class
class Particle : public Object {
	
public:
	int p_type,speed;
	
	inline Particle()
	{
		reset();
	}
	
	inline void settype(int p_type)
	{
		this->p_type = p_type;
	}
	
	inline void setspeed(int speed)
	{
		this->speed = speed;
	}
	
	inline void reset()
	{
		p_type = 0;
		cur_x = 0;
		cur_y = 0;
		on = false;
		dir = RIGHT;
		speed = 0;
	}
};


// the Particle Emiter class
// contains the bullets, orbs, and other objects the fly around on
// the screen
// basicly the particle emiter is a few for loops
// that loops through the array of particles, and proccessing them
// displaying them, and manipulating the particles variables
class ParticleEmiter : public MasterGameObject {
	
	
public:
	
	virtual void load()
	{
		for(i = 0; i < 255; i++)
		{
			particles[i].reset();
		}
		mxhwnd.LoadGraphic(&bullet,"images/shot.bmp");
		bullet.SetColorKey(SDL_MapRGB(bullet.surf->format, 255,255,255));
		
	}
	
	virtual void update()
	{
		for(i = 0; i < 255; i++)
		{
			if(particles[i].on == true)
			{
				switch(particles[i].p_type)
				{
				case BULLET:
					bullet.DisplayGraphic(particles[i].cur_x,particles[i].cur_y);
					break;
				case ORB:
					mxhwnd.paint.mxdrawellipse(particles[i].cur_x,particles[i].cur_y,particles[i].cur_x+20,particles[i].cur_y+20,RGB(rand()%255,rand()%255,rand()%255),RGB(rand()%255,rand()%255,rand()%255));
					break;
				}
			}
		}
		
		Sync
		{
			logic();
		}
		
	}
	
	inline void releaseparticle(int cur_x, int cur_y, int p_type, direction dir, int speed)
	{
		int off = getoffparticle();
		if(off != -1)
		{
			particles[off].cur_x = cur_x;
			particles[off].cur_y = cur_y;
			particles[off].p_type = p_type;
			particles[off].dir = dir;
			particles[off].on = true;
			particles[off].speed = speed;
			particles[off].show = true;

			if(p_type == BULLET)
				sound.play(FIRE);
		}
	}
	
	inline void killparticle(int index)
	{
		particles[index].on = false;
		particles[index].show = false;
		particles[index].reset();
	}
	
	inline void killallparticles()
	{
		for(i = 0; i < 255; i++)
		{
			killparticle(i);
		}
	}
	
	inline int particlecount(int p_type)
	{
		int c = 0;
		
		for(i = 0; i < 255; i++)
		{
			if(particles[i].p_type == p_type)
			{
				c++;
			}
		}
		return (c);
	}
	
	Particle particles[255];
	
private:
	
	MasterGraphic bullet;
	
	
	inline void logic()
	{
		for(i = 0; i < 255; i++)
		{
			if(particles[i].on == true)
			{
				switch(particles[i].p_type)
				{
				case BULLET:
					{
						switch(particles[i].dir)
						{
						case LEFT:
							particles[i].cur_x -= particles[i].speed;
							break;
						case RIGHT:
							particles[i].cur_x += particles[i].speed;
							break;
						}
					}
					break;
				case ORB:
					{
						switch(particles[i].dir)
						{
						case LEFT:
							particles[i].cur_x -= particles[i].speed;
							break;
						case RIGHT:
							particles[i].cur_x += particles[i].speed;
							break;
						case UP:
							particles[i].cur_y -= particles[i].speed;
							break;
						case DOWN:
							particles[i].cur_y += particles[i].speed;
							break;
						}
					}
					break;
				}
			}
		}
	}
	
	const inline int getoffparticle()
	{
		for(i = 0; i < 255; i++)
		{
			if(particles[i].on == false)
			{
				return (i);
			}
		}
		
		return (-1);
	}
	
	int i;
	
};

// class wrapper for Intro screen
// can be used agian and agian
// however we only need 1 intro screen =)
class Intro : public MasterGameObject {
	
	
public:
	
	virtual void load()
	{
		mxhwnd.LoadGraphic(&intro,"images/intro.bmp");
		snd_play = false;
	}
	
	virtual void update()
	{
		if(snd_play == false)
		{
			sound.play(OPEN);
			snd_play = true;
		}

		intro.DisplayGraphic(0,0);
		Sync 
		{
			logic();
		}
	}
	
private:
	
	MasterGraphic intro;
	bool snd_play;
	
	inline void logic()
	{
		static int x = 0;
		if(++x > 60)
		{
			mxhwnd.SetScreen(ID_START);
			x = 0;
		}
	}
	
};

// class wrapper for Start , About, and Highscores screens
// contains nested classes ;]
class Start : public MasterGameObject {
	
public:
	
	virtual void load()
	{
		mxhwnd.LoadGraphic(&start,"images/start.bmp");
		objhero.load();
		objgranny.load();
		about.load();
		scores.load();
		emiter.load();
		id_screen = ID_START;
		granny_x = 550;
		granny_dir = RIGHT;
	}
	
	virtual void update()
	{
		mxhwnd.text.setbkcolor(RGB(0,0,0));
		start.DisplayGraphic(0,0);
		Sync // ticking away the moments that make up the dull day
		{
			logic();
		}
		
		switch(id_screen)
		{
		case ID_START:
			{
				int the_y = 0;
				switch(cur_y)
				{
				case 0:
					the_y = 280;
					break;
				case 1:
					the_y = 360;
					break;
				case 2:
					the_y = 420;
					break;
				case 3:
					the_y = 500;
					break;
				}
				objhero.setdir(RIGHT);
				objhero.setcords(340,the_y);
				objhero.update();
			}
			break;
		case ID_HIGHSCORES:
			{
				mxhwnd.paint.mxdrawrect(10,100,1014,758,RGB(0,0,0),RGB(0,0,0));
				scores.update();
				objhero.setdir(RIGHT);
				objhero.setcords(100,100);
				
				static int t = 0;
				
				
				if(++t > 25)
				{
					objhero.jump_on();
				}
				if(t > 50)
				{
					objhero.jump_off();
					t = 0;
				}
				
				
				objhero.update();
				
			}
			break;
		case ID_ABOUT:
			mxhwnd.paint.mxdrawrect(10,100,1014,758,RGB(0,0,0),RGB(0,0,0));
			about.update();
			objhero.setdir(LEFT);
			objhero.setcords(900,100);
			objhero.update();
			
			
			if(granny_dir == RIGHT)
			{
				granny_x += 5;
				if(granny_x > 700)
				{
					granny_dir = LEFT;
				}
			}
			else
			{
				if(granny_x < 550)
				{
					granny_dir = RIGHT;
				}
				granny_x -= 5;
			}
			
			objgranny.setdir(granny_dir);
			objgranny.setcords(granny_x,100);
			objgranny.run_cycle();
			objgranny.update();
			emiter.update();
			for(i = 0; i < 255; i++)
			{
				if(emiter.particles[i].on == true)
				{
					if(emiter.particles[i].cur_x > objgranny.cur_x && emiter.particles[i].cur_x < objgranny.cur_x + 32 && emiter.particles[i].cur_y > objgranny.cur_y && emiter.particles[i].cur_y < objgranny.cur_y + 64)
					{
						emiter.killparticle(i);
						objgranny.die(granny_callback);
					}
				}
			}
			
			break;
		}
	}
	
	static void granny_callback()
	{
		if(id_about == ID_USERWON)
		{
			id_screen = ID_HIGHSCORES;
			scores_releaseinput();
			id_about = 0;
			about_setcredits();
			about_clearcredits();
		}
		else
			id_screen = ID_START;
		
	}
	
	inline void keypress(int key)
	{
		switch(id_screen)
		{
		case ID_START:
			switch(key)
			{
			case VK_UP:
				if(cur_y > 0)
					cur_y--;
				break;
			case VK_DOWN:
				if(cur_y < 3)
					cur_y++;
				break;
			case VK_RETURN:
				
				objhero.shoot(callback);
				current_y = cur_y;
				switch(cur_y)
				{
				case 2:
					objgranny.reset();
					break;
				}
				
				break;
			}
			break;
			case ID_HIGHSCORES:
				scores.keypress(key);
				if(key == VK_RETURN && scores.isinput_on() == false)
				{
					objhero.jump_off();
					objhero.setimg(0);
				}
				
				break;
			case ID_ABOUT:
				if(key == VK_RETURN)
				{
					objhero.shoot(this_callback);
				}
				break;
				
		}
	}
	
	inline void onchar(int key)
	{
		switch(id_screen)
		{
		case ID_HIGHSCORES:
			scores.onchar(key);
			break;
		}
	}
	
	static void callback()
	{
		switch(current_y)
		{
		case 0:// new game
			mxhwnd.SetScreen(ID_GAME);
			newgame();
			break;
		case 1:// highscores
			id_screen = ID_HIGHSCORES;
			about_setcredits();
			break;
		case 2:// about
			id_screen = ID_ABOUT;
			break;
		case 3:// exit
			mxhwnd.Kill();
			break;
		}
	}
	
	// release the bullet from the hero's gun
	inline void release_particle()
	{
		emiter.releaseparticle(900,120,BULLET,objhero.dir,20);
	}
	
	// release the input - allow the user to input there name
	inline void releaseinput()
	{
		if(livescheat() == false)
		{
			
			if(scores.getlastscore() < user_score.user_score)
			{
				scores.resetinput();
			}
			
		}
	}
	
	inline void setcredits()
	{
		about.setcredits();
	}
	
	inline void clearcredits()
	{
		about.clearcredits();
	}
	
	inline void grannyreset()
	{
		objgranny.reset();
	}
	
	MasterGraphic start;

private:
	
	Hero objhero;
	Grandma objgranny;
	ParticleEmiter emiter;
	
	int cur_y, granny_x, i;
	direction granny_dir;	
	
	inline void logic()
	{
		
	}
	
	// the About class wrapper, contains the information
	// nessicary for the about screen
	// and the animation of the grandmother zombie getting blown up
	class About : public MasterGameObject {
		
	public:
		
		virtual void load()
		{
			mxhwnd.LoadGraphic(&jared,"images/jared.bmp");
			mxhwnd.LoadGraphic(&mx,"images/mx.bmp");
			setcredits();
			credit_max = 23;
			loadcredits();
			//t_font = MakeFont("Arial", 20);
			off_credit = 0;
		}
		
		virtual void update()
		{
			mxhwnd.text.setbkcolor(RGB(0,0,0));
		//	mxhwnd.text.setfont(t_font);
			for(i = 0; i < credit_max; i++)
			{
				if(credits[i].on == true)
				{
					if(credits[i].color != -1)
						mxhwnd.text.settextcolor(credits[i].color);
					else
						mxhwnd.text.settextcolor(RGB(rand()%255,rand()%255,rand()%255));
					
					mxhwnd.text.printtext(credits[i].str, 100, credits[i].cur_y);
				}
			}
			
			Sync
			{
				static int x = 0;
				if(++x > 10)
				{
					if(off_credit >= credit_max && credits[credit_max-1].on == true)
					{
						
					}
					else if( off_credit >= credit_max && credits[credit_max-1].on == false)
					{
						off_credit = 0;
					}
					else 
					{
						
						if(credits[off_credit].on == false)
						{
							credits[off_credit].on = true;
							credits[off_credit].cur_y = 730;
							off_credit++;
						}
					}
					x = 0;
				}
				
				for(i = 0; i < credit_max; i++)
				{
					if(credits[i].on == true)
					{
						credits[i].cur_y -= 2;
						if(credits[i].cur_y < 100)
						{
							credits[i].on = false;
							credits[i].cur_y = 730;
						}
					}
				}
			}
			
			jared.DisplayGraphic(500,250);
			mx.DisplayGraphic(500,500);
			
		}
		
		
		inline void keypress(int key)
		{
			
		}
		
		inline void setcredits()
		{
			if(id_about == ID_USERWON)
			{
				credits[0].set("SuperMasterKong [ You are Victorious ] ", -1);
				credits[1].set("written by Master", RGB(255,0,0));
				credits[19].set("\"in the end its only round and round\" - pink floyd", -1);
			}
			else
			{
				credits[0].set("SuperMasterKong", RGB(255,0,0));
				credits[1].set("written by Jared Bruni", RGB(255,0,0));
				credits[19].set("\"shine on you crazy diamond\" - pink floyd", -1);
			}
		}
		
		inline void clearcredits()
		{
			for(i = 0; i < credit_max; i++)
			{
				credits[i].on = false;
				credits[i].cur_y = 730;
			}
			off_credit = 0;
		}
		
	private:
		
		//struct HFONT__ *t_font;
		int credit_max, i, off_credit;
		MasterGraphic jared;
		MasterGraphic mx;
		
		// the Credits class
		// contains the String data for the Credits as well as 
		// its x,y cordinates and color inherts from Object
		class Credit : public Object {
			
		public:
			char str[255];
			int color;
			
			Credit()
			{
				reset();
				memset(str,0,255);
				strcpy(str,"");
				color = 0;
			}
			
			void set(const char *str, int color)
			{
				strcpy(this->str, str);
				this->color = color;
			}
			
			void reset()
			{
				on = false;
				cur_y = 730;
			}
		};
		
		Credit credits[23];// instance of the Credits structure
		
		inline void loadcredits()
		{
			
			credits[0].set("SuperMasterKong", RGB(255,0,0));
			credits[1].set("written by Jared Bruni", RGB(255,0,0));
			credits[2].set("soundtrack by critical affiliation", RGB(0,255,0));
			credits[3].set("the third game in the epic battle between", RGB(0,0,255));
			credits[4].set("Super Stoner , and the evil Grandma Zombie", RGB(0,255,0));
			credits[5].set("game written in C++ / DirectX ", RGB(255,255,255));
			credits[6].set("full source code avialable via LostSideDead.com", RGB(0,0,255));
			credits[7].set("notice this game is freeware, you may redistrube it ", RGB(200,0,0));
			credits[8].set("change it, screw with it, do whatever you see fit", RGB(0,255,0));
			credits[9].set("you cant put a $ on thoughts they are free", RGB(100,255,100));
			credits[10].set("Open Source, Open Mind", RGB(255,255,255));
			credits[11].set("Shout outs go to: ", RGB(0,255,255));
			credits[12].set("Johnathan Parker PHD, for his meditation tapes ", RGB(255,255,255));
			credits[13].set(" The \" Pathways to Mastership \" ", RGB(255,255,100));
			credits[14].set("my good friend allen ", RGB(255,0,0));
			credits[15].set("rich gonzalez, scott, billy" , RGB(100,100,100));
			credits[16].set("chris and mellisa",RGB(100,255,0));
			credits[17].set("pink floyd for writing such sweet music", RGB(100,255,255));
			credits[18].set("be sure to check out www.LostSideDead.com", RGB(255,255,255));
			credits[19].set("game built with MasterX SDK technologys", RGB(255,0,255));
			credits[20].set(" email me: jared@lostsidedead.com aim - xmasterxx3 ", RGB(255,255,150));
			credits[21].set(" \" shine on you crazy diamond \" - pink floyd ", -1);
			credits[22].set(" [ press enter to return ] ", RGB(255,255,255));

			
		}
	};
	
	About about;
	
	// the HighScores class contains the information
	// nessicary for the highscores menu, and the animation
	// of Super Stoner giving us a Peace Sign and the Finger
	// remind you of anyone?
	class Highscores : public MasterGameObject {
		
	public:
		
		virtual void load()
		{
			read_from_file();
		//	t_font = MakeFont("Arial",30);
	//		tt_font = MakeFont("Arial",25);
//			ttt_font = MakeFont("FixedSys", 40);
		//	input_font = MakeFont("Arial",15);
			input_on = false;
			input_index = 0;
			strcpy(input_data,"");
		}
		
		virtual void update()
		{
			mxhwnd.text.setbkcolor(RGB(0,0,0));
			mxhwnd.text.settextcolor(RGB(0,255,0));
			//mxhwnd.text.setfont(t_font);
			if(input_on == false)
			{
				mxhwnd.text.printtext("HighScores [ enter to return ]", 350,180);
			}
			else
			{
				mxhwnd.text.printtext("HighScores ",350,180);
			}
			int pcur_x = 220;
			//mxhwnd.text.setfont(tt_font);
			mxhwnd.text.settextcolor(RGB(255,0,0));
			for(i = 0; i < 10; i++)
			{
				mxhwnd.text.sprinttext(350,pcur_x,"%i) %s : %i ",i+1, hscores.hscore[i].user, hscores.hscore[i].score);
				pcur_x += 30;
			}
			mxhwnd.text.settextcolor(RGB(0,0,255));
	//		mxhwnd.text.setfont(ttt_font);
			mxhwnd.text.printtext("www.LostSideDead.com",300,600);
			
			if(input_on == true)
			{
			//	mxhwnd.text.setfont(input_font);
				mxhwnd.text.settextcolor(RGB(255,255,255));
				mxhwnd.text.sprinttext(650,250,"%s%s", input_data, (rand()%4 > 2) ? "" : "_");
				mxhwnd.text.printtext("enter your name", 650, 200);
			}
			
		}
		
		inline void adduser(const char *str, int score)
		{
			strcpy(hscores.hscore[9].user,str);
			hscores.hscore[9].score = score;
			int max = 10;
			int pas,compare;
			
			for(pas = 0; pas < max - 1; pas++)
			{ 
				
				for(compare = 0; compare < max - 1; compare++)
				{
					if(hscores.hscore[compare].score < hscores.hscore[compare+1].score)
					{
						Scores  temp;
						temp = hscores.hscore[compare];
						hscores.hscore[compare] = hscores.hscore[compare+1];
						hscores.hscore[compare+1] = temp;			
					}
				}
			}
			
			write_to_file();
		}
		
		inline void resetinput()
		{
			input_index = 0;
			memset(input_data,0,100);
			strcpy(input_data,"");
			input_on = true;
		}
		
		const inline int getlastscore() const
		{
			return hscores.hscore[9].score;
		}
		
		inline void keypress(int key)
		{
			if(input_on == false && key == VK_RETURN)
			{
				id_screen = ID_START;
			}
		}
		
		inline void onchar(int key)
		{
			if(input_on == true)
			{
				if(key == VK_RETURN)
				{
					if(strlen(input_data) > 0)
					{
						adduser(input_data,user_score.user_score);
						input_on = false;
						
					}
				}
				else if(key == VK_BACK)
				{
					input_index--;
					if(input_index < 0)
					{
						input_index = 0;
					}
					input_data[input_index] = 0;
					
				}
				else
				{
					input_data[input_index] = key;
					input_index ++;
				}
			}
		}
		
		const inline bool isinput_on() const
		{
			return input_on;
		}
		
		private:
			
			int i;
			//struct HFONT__ *t_font, *tt_font, *ttt_font, *input_font;
			bool input_on;
			char input_data[100];
			int input_index;
			
			
			
			// the Scores structure
			// contains the users score
			// the reason its a structure is so we can easily
			// save it to file as binary
			struct Scores {
				char user[255];
				int score;
				
				Scores()
				{
					strcpy(user,"Null User");
					score = 0;
				}
			};
			
			// the scores structure wrapper
			// so we can easily save it to file as binary
			struct Scoreswrapper {
				Scores hscore[11];
			};
			
			struct Scoreswrapper hscores;
			
			inline void write_to_file()
			{
				ofstream fout("scores.dat",ios::binary);
				fout.write((char*)&hscores,sizeof(hscores));
				fout.close();
			}
			
			inline void read_from_file()
			{
				ifstream fin("scores.dat",ios::binary);
				if(fin.is_open())
				{
					fin.read((char*)&hscores,sizeof(hscores));
					fin.close();
				}
			}
			
	};
	
	Highscores scores;	
	
};

// The Game Universe, this is were the actual game is
// the TileMap, ParticleEmiter, Hero, and Grandma classes are all made
// a instance of in this class
// this class still inherts from MasterGameObject
// since it needs a load and a update function to work with the engine
// the load function is called on initalization, and the update 
// function is called during the rendering process
class GameUniverse : public MasterGameObject {
	
	
public:
	
	virtual void load()
	{
		elevel.load();
		emiter.load();
		hero.load();
		grandma.load();
		hero.hide();
		grandma.hide();
		mxhwnd.LoadGraphic(&tiles[0],"images/black.bmp");
		mxhwnd.LoadGraphic(&tiles[1],"images/bluebrick.bmp");
		mxhwnd.LoadGraphic(&tiles[2],"images/sand1.bmp");
		mxhwnd.LoadGraphic(&tiles[3],"images/sand2.bmp");
		mxhwnd.LoadGraphic(&tiles[4],"images/stone.bmp");
		mxhwnd.LoadGraphic(&tiles[5],"images/stone2.bmp");
		mxhwnd.LoadGraphic(&tiles[6],"images/stone3.bmp");
		mxhwnd.LoadGraphic(&tiles[7],"images/stone4.bmp");
		mxhwnd.LoadGraphic(&tiles[8],"images/snow.bmp");
		mxhwnd.LoadGraphic(&tiles[9],"images/bluesky.bmp");
		mxhwnd.LoadGraphic(&tiles[10],"images/eblock.bmp");
		mxhwnd.LoadGraphic(&tiles[11],"images/red_brick.bmp");
		mxhwnd.LoadGraphic(&tiles[12],"images/grass.bmp");
		mxhwnd.LoadGraphic(&tiles[13],"images/brick.bmp");
		//t_font = MakeFont("Arial",20);
		jump_enabled = false;
		l_on = false, s_on = false;
		
	}
	
	virtual void update()
	{
		switch(p_id_screen)
		{
		case ID_GAME:
			{
				int cur_x = 0, cur_y = 0;
				
				for(i = 0; i < 64; i++)
				{
					for(z = 0; z < 48; z++)
					{
						if(matrix.map.tiles[i][z] != 0)
							tiles[matrix.map.tiles[i][z]].DisplayGraphic(cur_x,cur_y);
						
						cur_y += 16;
					}
					
					cur_x += 16;
					cur_y = 0;
				}
				
				if(lives_cheat == false)
					mxhwnd.text.setbkcolor(RGB(255,255,255));
				else
					mxhwnd.text.setbkcolor(RGB(0,0,0));
				
				mxhwnd.text.settextcolor(RGB(255,0,0));
			//	mxhwnd.text.setfont(t_font);
				mxhwnd.text.sprinttext(20,0,"-[ Score: %i        Orbs Caught: %i / %i     Orbs Missed: %i / %i       Level: < %s > ( %i )      Lives: %i %s ]-", user_score.user_score, user_score.orbs,matrix.map.orbs_total, user_score.orbs_missed, (matrix.map.orbs_total/2), matrix.map.level_name,level, lives, (lives_cheat == true) ? "[Cheat Enabled]" : "");
				
				Sync
				{
					logic();
				}
				
				hero.update();
				grandma.update();
				emiter.update();
			}
			break;
		case ID_ENTERLEVEL:
			
			elevel.update();
			hero.setcords(100,150);
			hero.setimg(0);
			hero.update();
			break;
		}
	}
	
	inline void keypress(int key)
	{
		switch(p_id_screen)
		{
		case ID_GAME:
			{
				
				switch(key)
				{
				case 'A':
				case 'a':
					
					if(hero.is_jump_on() == false)
					{
						
						hero.jump_on();
						if(jump_enabled == false)
						{
							jump_enabled = true;
							jump_index = 0;
						}
						
					}
					break;
				case 'L':
				case 'l':
					l_on = true;
					s_on = false;
					break;
				case 's':
				case 'S':
					{
						if(hero.is_jump_on() == false && hero.is_shooting() == false && hero.is_running() == false && hero.is_shoot_wait() == false)
						{
							hero.shoot(callback);
						}
						s_on = true;
					}
					break;
				case 'D':
				case 'd':
					if(l_on == true && s_on == true)
					{
						lives += 99;
						lives_cheat = true;
					}
					break;
					default:
					l_on = false;
					s_on = false;
					break;
					// " mother should I trust the goverment? "- pink floyd
				}
			}
			break;
		case ID_ENTERLEVEL:
			
			if(key == VK_RETURN)
			{
				loadlevel();
				p_id_screen = ID_GAME;
			}
			
			break;
		}
	}
	
	static void callback()
	{
		releasebullet();
	}
	
	inline void release_bullet()
	{
		if(hero.dir == LEFT)
			emiter.releaseparticle(hero.cur_x-10, hero.cur_y+15, BULLET, hero.dir, 20);
		else
			emiter.releaseparticle(hero.cur_x+20, hero.cur_y+15, BULLET, hero.dir, 20);
	}
	
	inline void grandmadied()
	{
		user_score.user_score += 100;
		subtract_life();
	}
	
	
	inline void newgame()
	{
		user_score.user_score = 0;
		user_score.orbs = 0;
		user_score.orbs_missed = 0;
		level = 1;
		//level = 9; // for debuging
		lives = 10;
		emiter.killallparticles();
		loadlevel();
		p_id_screen = ID_ENTERLEVEL;
		elevel.set(matrix.map.level_name,matrix.map.orbs_total,level,lives);
		id_about = 0;
		l_on = false;
		s_on = false;
		lives_cheat = false;
	}
	
	inline void nextlevel()
	{
		level++;
		if(level > 9)
		{
			mxhwnd.SetScreen(ID_START);
			id_screen = ID_ABOUT;
			id_about = ID_USERWON;
			about_setcredits();
			about_clearcredits();
		}
		else
		{
			loadlevel();
			p_id_screen = ID_ENTERLEVEL;
			elevel.set(matrix.map.level_name,matrix.map.orbs_total,level,lives);
			user_score.orbs = 0;
			user_score.orbs_missed = 0;
			emiter.killallparticles();
		}
	}
	
	
	inline void loadlevel()
	{
		char levelstr[255];
		sprintf(levelstr,"/assets/level%i.smk",level);
		if(matrix.read_map_from_file(levelstr) == true)
		{	
			start_level();
		} else {
			printf("couldn't load level %s\n", levelstr);
		}
	}
	
	bool lives_cheat;
	
private:
	
	MasterGraphic tiles[14];
	int i,z,p,level, lives;
	bool l_on, s_on;
	
	inline void logic()
	{
		gravity();
		heromovement();
		emiterproc();
		grandma_logic();
		collision_check();
	}
	
	// subtract a life from the person playing the game
	// this happens if they shoot the grandma, fall down a hole
	// or miss to many orbs
	inline void subtract_life()
	{
		lives--;
		if(lives < 0)
		{
			mxhwnd.SetScreen(ID_START);
			id_screen = ID_HIGHSCORES;
			scores_releaseinput();
		}
		else
		{
			emiter.killallparticles();
			start_level();
		}
	}
	
	// gravity, pulls objects to the ground when in the air
	// and pulls objects off the ground (ex: hero when he jumps)
	inline void gravity()
	{
		if(jump_enabled == false)
		{
			
			if(matrix.is_solid(matrix.map.tiles[hero.cur_pos_i][hero.cur_pos_z+4]) == false)
			{
				hero.cur_pos_z++;
				
				if(hero.cur_pos_z >= 44)
				{
					subtract_life();
					emiter.killallparticles();
					return;
				}
				
				int ix, iy;
				hero.jump_on();
				
				matrix.getxyfrompos(hero.cur_pos_i, hero.cur_pos_z, ix,iy);
				
				if(ix != -1 && iy != -1)
					hero.setcords(ix,iy);		
			}
			else
			{
				if(hero.is_jump_on() == true)
					hero.jump_off();
			}
			
		}
		else
		{
			if(jump_index < 6)
			{
				if(matrix.is_solid(matrix.map.tiles[hero.cur_pos_i][hero.cur_pos_z-1]) == false)
				{
					hero.cur_pos_z--;
					int ix,iy;
					matrix.getxyfrompos(hero.cur_pos_i, hero.cur_pos_z,ix,iy);
					if(ix != -1 && iy != -1)
						hero.setcords(ix,iy);
				}
				else
				{
					jump_enabled = false;
				}
				
				jump_index++;
			}
			else
			{
				jump_enabled = false;
			}
			
		}
		
		if(matrix.is_solid(matrix.map.tiles[grandma.cur_pos_i][grandma.cur_pos_z+4]) == false)
		{
			grandma.cur_pos_z++;
			int ix, iy;
			matrix.getxyfrompos(grandma.cur_pos_i,grandma.cur_pos_z,ix,iy);
			if(ix != -1 && iy != -1)
			{
				grandma.setcords(ix,iy);
			}
			grandma.setimg(0);
		}
	}
	

	inline void collision_check()
	{
		if(hero.cur_x >= grandma.cur_x && hero.cur_x <= grandma.cur_x + 32 && hero.cur_y >= grandma.cur_y && hero.cur_y <= grandma.cur_y + 64)
		{
			subtract_life();
			emiter.killallparticles();
		}
	}
	
	inline void heromovement()
	{
		// if the left key is pressed
		if(mxhwnd.KeyCheck(DIK_LEFT))
		{
			hero.setdir(LEFT);
			if(hero.is_jump_on() == false)
			{
				if(hero.cur_pos_z-2 > 0)
					if(matrix.is_solid(matrix.map.tiles[hero.cur_pos_i-1][hero.cur_pos_z]) == false && matrix.is_solid(matrix.map.tiles[hero.cur_pos_i-1][hero.cur_pos_z+2]) == false && matrix.is_solid(matrix.map.tiles[hero.cur_pos_i-1][hero.cur_pos_z+3]) == false)
					{
						hero.run_cycle();
						hero.cur_pos_i--;
						int ix, iy;
						matrix.getxyfrompos(hero.cur_pos_i,hero.cur_pos_z,ix,iy);
						if(ix != -1 && iy != -1)
							hero.setcords(ix,iy);
					}
					
					
			}
			else
			{
				if(matrix.is_solid(matrix.map.tiles[hero.cur_pos_i-1][hero.cur_pos_z]) == false && matrix.is_solid(matrix.map.tiles[hero.cur_pos_i-1][hero.cur_pos_z+2]) == false && matrix.is_solid(matrix.map.tiles[hero.cur_pos_i-1][hero.cur_pos_z+3]) == false)
				{
					hero.cur_pos_i--;
					int ix,iy;
					matrix.getxyfrompos(hero.cur_pos_i,hero.cur_pos_z,ix,iy);
					if(ix != -1 && iy != -1)
						hero.setcords(ix,iy);
				}
			}
		}
		
		// if the right key is pressed
		if(mxhwnd.KeyCheck(DIK_RIGHT))
		{
			hero.setdir(RIGHT);
			if(hero.is_jump_on() == false)
			{
				if(matrix.is_solid(matrix.map.tiles[hero.cur_pos_i+2][hero.cur_pos_z+2]) == false && matrix.is_solid(matrix.map.tiles[hero.cur_pos_i+2][hero.cur_pos_z+3]) == false && matrix.is_solid(matrix.map.tiles[hero.cur_pos_i+2][hero.cur_pos_z]) == false)
				{
					hero.run_cycle();
					hero.cur_pos_i++;
					int ix, iy;
					matrix.getxyfrompos(hero.cur_pos_i, hero.cur_pos_z, ix, iy);
					if(ix != -1 && iy != -1)
						hero.setcords(ix,iy);
				}
				
			}
			else
			{
				if(matrix.is_solid(matrix.map.tiles[hero.cur_pos_i+2][hero.cur_pos_z+2]) == false && matrix.is_solid(matrix.map.tiles[hero.cur_pos_i+1][hero.cur_pos_z]) == false && matrix.is_solid(matrix.map.tiles[hero.cur_pos_i+2][hero.cur_pos_z+3]) == false)
				{
					hero.cur_pos_i++;
					int ix, iy;
					matrix.getxyfrompos(hero.cur_pos_i,hero.cur_pos_z,ix,iy);
					if(ix != -1 && iy != -1)
						hero.setcords(ix,iy);
				}
			}
		}
	}
	
	// the grandma's logic
	// this is what makes her move - release orbs and other objects
	inline void grandma_logic()
	{
		if(grandma.dir == RIGHT)
		{
			// check if the blocks to her right are not solid
			if(matrix.is_solid(matrix.map.tiles[grandma.cur_pos_i+2][grandma.cur_pos_z]) == false && matrix.is_solid(matrix.map.tiles[grandma.cur_pos_i+2][grandma.cur_pos_z+2]) == false && matrix.is_solid(matrix.map.tiles[grandma.cur_pos_i+2][grandma.cur_pos_z+3]) == false)
			{
				// if it isnt run cycle
				grandma.run_cycle();
				grandma.cur_pos_i ++;
				int ix, iy;
				matrix.getxyfrompos(grandma.cur_pos_i,grandma.cur_pos_z,ix,iy);
				if(ix != -1 && iy != -1)
					grandma.setcords(ix,iy);
			}
			else
			{
				grandma.dir = LEFT;
			}
			
		}
		else
		{
			// check if the blocks to her left isnt solid
			if(matrix.is_solid(matrix.map.tiles[grandma.cur_pos_i-1][grandma.cur_pos_z]) == false && matrix.is_solid(matrix.map.tiles[grandma.cur_pos_i-1][grandma.cur_pos_z+2]) == false && matrix.is_solid(matrix.map.tiles[grandma.cur_pos_i-1][grandma.cur_pos_z+3]) == false)
			{
				// if it isnt then run cycle
				grandma.run_cycle();
				grandma.cur_pos_i--;
				int ix, iy;
				matrix.getxyfrompos(grandma.cur_pos_i,grandma.cur_pos_z,ix,iy);
				if(ix != -1 && iy != -1)
					grandma.setcords(ix,iy);
				
			}
			else
			{
				grandma.dir = RIGHT;
			}
		}
		
		static int t = 0;
		int time_out = 55 - (level * 5);
		if(++t > time_out-(rand()%rand()%time_out))
		{
			if(emiter.particlecount(ORB) < level)
				emiter.releaseparticle(grandma.cur_x,grandma.cur_y,ORB,DOWN,(level <= 2) ? 5+rand()%3 : (level < 6) ? (level)+rand()%5 : 2+rand()%5);
			t = 0;
		}
		
		
	}
	
	// emiter proc, - proccess the particles
	// see if they run into any solid objects
	// example: a brick or the grandma
	inline void emiterproc()
	{
		int cur_x = 0, cur_y = 0;
		
		for(i = 0; i < 64; i++)
		{
			for(z = 0; z < 48; z++)
			{
				
				for(p = 0; p < 255; p++)
				{
					switch(emiter.particles[p].p_type)
					{
					case BULLET:
						
						if(cur_x >= emiter.particles[p].cur_x && cur_x <= emiter.particles[p].cur_x + 16 && cur_y >= emiter.particles[p].cur_y && cur_y <= emiter.particles[p].cur_y + 16)
						{
							if(matrix.is_solid(matrix.map.tiles[i][z]) == true)
							{
								emiter.killparticle(p);
							}
						}
						
						break;
					}
				}
				
				cur_y += 16;
			}
			
			cur_x += 16;
			cur_y = 0;
		}
		
		for(p = 0; p < 255; p++)
		{
			switch(emiter.particles[p].p_type)
			{
			case BULLET:
				
				if(emiter.particles[p].cur_x >= grandma.cur_x && emiter.particles[p].cur_x <= grandma.cur_x + 32 && emiter.particles[p].cur_y >= grandma.cur_y && emiter.particles[p].cur_y <= grandma.cur_y + 64)
				{
					emiter.killparticle(p);
					grandma.die(die_callback);
				}
				
				break;
			case ORB:
				
				if(emiter.particles[p].cur_x >= hero.cur_x && emiter.particles[p].cur_x <= hero.cur_x + 32 && emiter.particles[p].cur_y >= hero.cur_y && emiter.particles[p].cur_y <= hero.cur_y + 64)
				{
					emiter.killparticle(p);
					user_score.user_score += 25;
					user_score.orbs++;
					sound.play(ORB_SND);
					
					if(user_score.orbs >= matrix.map.orbs_total)
					{
						nextlevel();
					}
				}
				
				if(emiter.particles[p].cur_y > 768)
				{
					emiter.killparticle(p);
					user_score.orbs_missed++;
					
					if(user_score.orbs_missed >= (matrix.map.orbs_total/2))
					{
						lives--;
						if(lives < 0)
						{
							mxhwnd.SetScreen(ID_START);
							id_screen = ID_HIGHSCORES;
							scores_releaseinput();
						}
						else
						{
							start_level();
							user_score.orbs_missed = 0;
							user_score.orbs = 0;
							emiter.killallparticles();
						}
					}
				}
				
				break;
			}
		}
		
		// check to see if a orb is colliding into a bullet
		// if it is then make them disappear
		// and give the user 50 points
		// and award them a orb point
		for(i = 0; i < 255; i++)
		{
			for(z = 0; z < 255; z++)
			{
				if(emiter.particles[i].p_type == ORB && emiter.particles[z].p_type == BULLET)
				{
					if(emiter.particles[i].cur_x >= emiter.particles[z].cur_x && emiter.particles[i].cur_x <= emiter.particles[z].cur_x + 25 && emiter.particles[i].cur_y >= emiter.particles[z].cur_y && emiter.particles[i].cur_y <= emiter.particles[z].cur_y + 25)
					{
						emiter.killparticle(i);
						emiter.killparticle(z);
						user_score.user_score += 50;
						user_score.orbs++;
						
						sound.play(ORB_SND);

						if(user_score.orbs >= matrix.map.orbs_total)
						{
							nextlevel();
						}
					}
				}
			}
		}
		
	}
	
	static void die_callback()
	{
		grandma_died();
	}
	
	// start level
	// initalize the Hero, and Grandma on the map
	// kill all particles if we were on a map previously
	inline void start_level()
	{
		int hero_x,hero_y;
		matrix.getxyfrompos(matrix.map.hero_start_i,matrix.map.hero_start_z,hero_x,hero_y);
		if(hero_x != -1 && hero_y != -1)
		{
			
			hero.setdir(RIGHT);
			hero.setcords(hero_x,hero_y);
			hero.setpos(matrix.map.hero_start_i,matrix.map.hero_start_z);
			hero.unhide();
			
		}
		
		int grandma_x, grandma_y;
		matrix.getxyfrompos(matrix.map.grandma_start_i,matrix.map.grandma_start_z,grandma_x,grandma_y);
		if(grandma_x != -1 && grandma_y != -1)
		{
			grandma.setdir(RIGHT);
			grandma.setcords(grandma_x,grandma_y);
			grandma.setpos(matrix.map.grandma_start_i,matrix.map.grandma_start_z);
			grandma.unhide();
		}
		
		emiter.killallparticles();
	}
	
	// this is the TileMatrix class
	// this is a TileMap its basicly a wrapper around
	// a multidimensional array
	class TileMatrix {
		
		int i, z;// loop variables
	public:
		
		// the TileMap
		struct TileMap {
			
			int hero_start_i, hero_start_z, grandma_start_i, grandma_start_z;
			int tiles[64][48];
			int orbs_total;
			char level_name[255];
		};
		
		inline TileMatrix() // constructor
		{
			clear();// for debuging
		}
		
		struct TileMap map;// tilemap
		
		// is this tile index, solid
		const inline bool is_solid(int tile) const
		{
			switch(tile)
			{
			case 0:
				return false;
				break;
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
				return true;
				break;
			case 9:
				return false;
				break;
			case 10:
			case 11:
			case 12:
			case 13:
				return true;
				break;
			default:
				return false;
			}
			
			return false;
			
		}
		
		// clear the tile map
		// used for debuging
		inline void clear()
		{
			for(i = 0; i < 64; i++)
			{
				for(z = 0; z < 48; z++)
				{
					map.tiles[i][z] = 1;
				}
			}
			
			map.hero_start_i = 10;
			map.hero_start_z = 20;
			map.grandma_start_i = 20;
			map.grandma_start_z = 30;
		}
		// read map from file
		// read the tile map from the hard drive
		inline bool read_map_from_file(const char *source)
		{
			ifstream fin(source, ios::binary);
			if(fin.is_open())
			{
				fin.read((char*)&map,sizeof(map));
				fin.close();
				return true;
			}
			else
				return false;
			
		}
		
		// save map to file
		// save the tile map to the hard drive
		inline void save_map_to_file(const char *source)
		{
			ofstream fout(source, ios::binary);
			fout.write((char*)&map,sizeof(map));
			fout.close();
		}
		
		// get the x , y cordinates, from  i , z position
		// i , z as in the subscript of the multidimensional array
		inline void getxyfrompos(int ix, int zx, int& x, int& y)
		{
			int cur_x = 0, cur_y = 0;
			
			for(i = 0; i < 48; i++)
			{
				for(z = 0; z < 64; z++)
				{
					if(ix == z && zx == i)
					{
						x = cur_x;
						y = cur_y;
						return;
					}
					cur_x += 16;
				}
				
				cur_y += 16;
				cur_x = 0;
			}
			
			x = -1;
			y = -1;
		}
		
	};
	
	TileMatrix matrix;
	ParticleEmiter emiter;
	Hero hero;
	Grandma grandma;
//	struct HFONT__ *t_font;
	bool jump_enabled;
	int jump_index;
	int p_id_screen;
	
	// class for the Enter Level Screen
	class EnterLevel : public MasterGameObject {
		
	public:
		
		virtual void load()
		{
			start_bg = getstartbmp();
	//		t_font = MakeFont("Arial", 20);
	//		b_font = MakeFont("FixedSys",65);
		}
		
		virtual void update()
		{
			start_bg->DisplayGraphic(0,0);
			mxhwnd.paint.mxdrawrect(10,100,1014,758,RGB(0,0,0),RGB(0,0,0));
		//	mxhwnd.text.setfont(t_font);
			mxhwnd.text.settextcolor(RGB(255,0,0));
			mxhwnd.text.setbkcolor(RGB(0,0,0));
			mxhwnd.text.sprinttext(100,250,"Entering Level %i: %s",level_num, level_name);
			mxhwnd.text.settextcolor(RGB(0,0,255));
			mxhwnd.text.sprinttext(100,270,"Collect %i Orbs while only missing %i  ",orbs_total , (orbs_total/2));
			mxhwnd.text.sprinttext(100,290,"Amount of Orbs at once %i", level_num);
			mxhwnd.text.settextcolor(RGB(0,255,0));
			mxhwnd.text.sprinttext(100,310,"Lives: %i %s , Score: %i", lives_num, (livescheat() == true) ? "[Lives Cheat Enabled]" : "",  user_score.user_score);
			mxhwnd.text.settextcolor(RGB(255,255,255));
			mxhwnd.text.printtext("use the arrow keys to move Super Stoner", 100, 330);
			mxhwnd.text.printtext("press A to jump, S to shoot your shotgun",100,350);
			mxhwnd.text.printtext("shoot the orbs for more points, be careful for pits",100,370);
			mxhwnd.text.printtext("kill the Zombie Grandmother and lose a life, yet gain 100 points",100,390);
			mxhwnd.text.settextcolor(RGB(rand()%255,rand()%255,rand()%255));
			mxhwnd.text.printtext("[press enter to continue]",100,410);
		//	mxhwnd.text.setfont(b_font);
			mxhwnd.text.settextcolor(RGB(0,0,255));
			mxhwnd.text.printtext("www.LostSideDead.com",200,600);
		}
		
		inline void set(char *level_name, int orbs_total, int level_num, int lives_num)
		{
			strcpy(this->level_name,level_name);
			this->orbs_total = orbs_total;
			this->level_num = level_num;
			this->lives_num = lives_num;
		}
		
	private:
		
		MasterGraphic *start_bg;
		char level_name[255];
		int orbs_total, level_num, lives_num;
//		struct HFONT__ *t_font, *b_font;
		
	};
	
	EnterLevel elevel;// instance of the EnterLevel class
	
};

// "in the end its only round and round" - pink floyd




