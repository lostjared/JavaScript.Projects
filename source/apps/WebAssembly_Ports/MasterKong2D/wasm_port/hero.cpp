// the hero object
// written by *jared bruni*
// www.lostsidedead.com

// open source,  open mind ++

#include "thehead.h"



void Hero::ondraw()
{
	int hx;
	int hy;
	level.GetPointFromPos(hero_pos,hx,hy);
	// on drawing
	if(hdir == true)
	{
		herog[cur_hero].DisplayGraphic(hx,hy);
	}
	else
	{
		herogl[cur_hero].DisplayGraphic(hx,hy);
	}
}

void Hero::hlogic()
{
	if(ijump == true)
	{
		// check and see wazup
		bool check1;
		check1 = level.blocks[hero_pos-1].solid;
		bool check2;
		check2 = level.blocks[hero_pos+24-1].solid;
		
		if(check1 == true || check2 == true)
		{
			goto iover;
		}
		
		hero_pos--;
		jcount++;
		if(jcount > 6)
		{
iover:
		jcount = 0;
		ijump = false;
		cur_hero = 4;
		// falldown
		ifall = true;
		}
	}
	
	procblock();
}

// proccess blocks, see if hes standing on one, and move him down
void Hero::procblock()
{
	if(ijump == false)
	{
		if( level.blocks[hero_pos+4].solid  == false)
		{
			if( level.blocks[hero_pos+24+4].solid == true)
			{ 
				goto elsex;
			}
			
			hero_pos++;
		}
		else
		{
elsex:
		if(ifall == true)
		{
			ifall = false;
			cur_hero = 0; // we have landed
		}
		}
		
	}
}

void Hero::onlogic()
{
	
	if(ifire == true)
	{
		procfire();
		return;
	}
	
	hlogic();

	// synchonized logic, based on 'speed' of the hero
	speedc++;
	
	if(speedc > speed)
	{
		speedc = 0;
		// first check for keys then joystick
		
		if(mxhwnd.KeyCheck(DIK_LEFT))
		{
			moveleft();
		}
		
		if(mxhwnd.KeyCheck(DIK_RIGHT))
		{
			moveright();
		}
		
		if(mxhwnd.KeyCheck(DIK_A))
		{
			jump();
		}

		if(mxhwnd.KeyCheck(DIK_S))
		{
			fire();
		}
		
		/*if(player.joystick == true)
		{
			MasterJoyStick* stick = getstick();
			if(stick->JoyLeft())
			{
				moveleft();
			}
			if(stick->JoyRight())
			{
				moveright();
			}
			int b;
			b = stick->JoyButtonDown();
			switch(b)
			{
			case 0:
				jump();
				break;
			case 1:
				fire();
				break;
			}
		}*/
	}
	
	
}


void Hero::setpos(bool hdirx,int hero_posx)
{
	hdir = hdirx;
	hero_pos = hero_posx;
}


void Hero::fire()
{
	// FIRE FIRE FIRE
	// FIRE & FIRE & FIRE & FIRE++
	if(ifire == false && ifall == false && ijump == false)
	{
		ifire = true;
		firepos = 0;
	}
}

void Hero::procfire()
{
	switch(firepos)
	{
	case 0:
		cur_hero = 5;
		break;
	case 1:
		cur_hero = 6;
		break;
	case 2:
		cur_hero = 7;
		break;
	case 3:
		cur_hero = 8;
		break;
	case 4:
		cur_hero = 9; 
		break;
	case 5:// release bullet
		int bpos;
		bpos = hero_pos;
		if(hdir == true)
		{
			bpos += 24 + 24 + 2;
		}
		else
		{
			bpos -= 24 - 24 - 2;
		}
		hparticle.releaseparticle(bpos,hdir, BULLET);
		soundhandle.Play(FIRE);
		break;
	case 6:
		cur_hero = 8;
		break;
	case 7:
		cur_hero = 7;
		break;
	case 8:
		cur_hero = 6;
		break;
	case 9:
		cur_hero = 5;
		break;
	case 10:
		cur_hero = 0;
		break;
	}

	firepos++;

	if(firepos > 10)
	{
		cur_hero = 0;
		firepos = 0;
		ifire = false;
	}

}

void Hero::jump()
{
	if(ijump == false && ifall == false)
	{
		
		jcount = 0;
		ijump = true;
		cur_hero = 4;
		
	}
	
}


void Hero::movedown()
{
	
	
}

void Hero::moveup()
{
	
	
}

void Hero::moveleft()
{ 
	if(hero_pos - 24 > 0)
	{
		
		bool check1 = level.blocks[hero_pos-24].solid;
		bool check2 = level.blocks[hero_pos-23].solid;
		bool check3 = level.blocks[hero_pos-22].solid;
		bool check4 = level.blocks[hero_pos-21].solid;
		
		if(check1 == false && check2 == false && check3 == false && check4 == false)
		{
			
			hero_pos -= 24;
			cur_hero = getnextwalk();
			hdir = false;
			
		} else
		{
			goto returnx;
		}
		
	}
	else
	{
returnx:
	hdir = false;
	cur_hero = 0;
	}
}

void Hero::moveright()
{
	if(hero_pos + 24 < LEVEL_SIZE - 24)
	{
		
		bool check1;
		check1 = level.blocks[hero_pos + 27].solid;
		bool check2;
		check2 = level.blocks[hero_pos + 27 + 24].solid;
		bool check3;
		check3 = level.blocks[hero_pos + 27 + 23].solid;
		bool check4;
		check4 = level.blocks[hero_pos + 27 + 22].solid;
		
		if(check1 == false && check2 == false && check3 == false && check4 == false)
		{
			
			hero_pos += 24;
			cur_hero = getnextwalk();
			hdir = true;
			
		}
		else
		{
			goto returnx;
		}
		
	}
	else
	{
returnx:
	
	hdir = true;
	cur_hero = 0;
	}
}

// get the next walk image
int Hero::getnextwalk()
{
	if( ijump == false && ifall == false)
	{
		
		
		switch(cur_hero)
		{
		case 0:
			return 1;
			break;
		case 1:
			return 2;
			break;
		case 2:
			return 3;
			break;
		case 3:
			return 0;
			break;
			
		}
		
	}
	else
	{
		return 4;
	}
	return 0;
}

// keypress and key release

void Hero::keypress(unsigned int wParam)
{
	
}

void Hero::keyrelease(unsigned int wParam)
{
	if(ijump == false)
	{
		
		cur_hero = 0;
		
	}
}

void Hero::die()
{
	player.lives--;
	if(player.lives < 0 )
	{
		// game over
		thegameover(false);

	}
	else
	{
		cur_hero = 0;
		hdir = true;
		hero_pos = start_pos;
	}
}

/*************************************************************************** lostsidedead.com */
