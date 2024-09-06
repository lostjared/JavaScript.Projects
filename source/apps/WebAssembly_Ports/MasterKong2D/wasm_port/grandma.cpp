// grandma.cpp
// written by jared bruni
// www.lostsidedead.com

#include "thehead.h"


// proccess the blocks
void Grandma::procblock()
{
	if( level.blocks[pos+4].solid  == false)
	{
		pos++;
	}
}

// artiifical intelligence algorithm, for grandma bot
void Grandma::ai()
{
			if(bpos == true)
			{

			if(oldpos != pos)
			{
				oldpos = pos;
				oc = 0;
			}
			else
			{
				oc++;
				if(oc > 10)
				{
				
						if(gdir == true)
						{
							gdir = false;
						}
						else
						{
							gdir = true;
						}

						oc = 0;
				}
			}

			}


	st_count ++;

	if(st_count > 2)
	{
		st_count = 0;
		if(gdir == true)
		{
			bool check1;
			check1 = level.blocks[pos + rows(3) + 4].solid;
			if(check1 == false)
			{
				gdir = false;
				return;
			}

			moveright();
		}
		else
		{
			
			bool check1;
			check1 = level.blocks[pos - rows(3) + 4].solid;
			if(check1 == false)
			{
				gdir = true;
				return;
			}

			moveleft();
		}
	}

	rcount ++;

	if(rcount > 20-player.difficulty )
	{
		rcount = 0;
		releaseprojectile();
	}

}

// move left
void Grandma::moveleft()
{
	if(pos - 24 > 0)
	{
		
		bool check1 = level.blocks[pos-24].solid;
		bool check2 = level.blocks[pos-23].solid;
		bool check3 = level.blocks[pos-22].solid;
		bool check4 = level.blocks[pos-21].solid;
		
		if(check1 == false && check2 == false && check3 == false && check4 == false)
		{
			
			pos -= 24;
			cur_granny = getnextwalk();
			gdir = false;
			
		} else
		{
			goto returnx;
		}
		
	}
	else
	{
returnx:
	gdir = false;
	cur_granny = 0;
	}

}

void Grandma::moveright()
{
	if(pos + 24 < LEVEL_SIZE - 24)
	{
		
		bool check1;
		check1 = level.blocks[pos + 27].solid;
		bool check2;
		check2 = level.blocks[pos + 27 + 24].solid;
		bool check3;
		check3 = level.blocks[pos + 27 + 23].solid;
		bool check4;
		check4 = level.blocks[pos + 27 + 22].solid;
		
		if(check1 == false && check2 == false && check3 == false && check4 == false)
		{
			
			pos += 24;
			cur_granny = getnextwalk();
			gdir = true;
			
		}
		else
		{
			goto returnx;
		}
		
	}
	else
	{
returnx:
	
	gdir = true;
	cur_granny = 0;
	}
}

int Grandma::getnextwalk()
{
	switch(cur_granny)
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
		return 4;
		break;
	case 4:
		return 0;
		break;
	}

	return 0;
}

void Grandma::releaseprojectile()
{
		int bpos;
		bpos = pos;
		if(gdir == true)
		{
			bpos += 24 + 24 + 2;
		}
		else
		{
			bpos -= 24 - 24 - 2;
		}

		if ( bpos > LEVEL_SIZE  || bpos < 0 )
		{
			return;
		}

	hparticle.releaseparticle(bpos,gdir,true,BALL);
}


void Grandma::die()
{
	death = true;
}

void Grandma::birth()
{
	alive = true;
	bpos = true;
	cur_granny = 0;
}

int Grandma::getnextdie()
{
	switch(cur_granny)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
		return 5;
		break;
	case 5:
		return 6;
	case 6:
		return 7;
	case 7:
		return 8;
	case 8:
		return 9;
	case 9:
		return 10;
	case 10:
		return -1;
	}

	return 0;
}