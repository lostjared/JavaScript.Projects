// particle handler
// written by jared bruni
// www.lostsidedead.com

#include "thehead.h"


int ParticleHandler::getoffparticle()
{
	for(int i = 0; i <= MAX_PARTICLE;i++)
	{
		if(particles[i].on == false)
		{
			return i;
		}
	}
	return 0;
}


void ParticleHandler::drawparticles()
{
	
	for(int i = 0; i <= MAX_PARTICLE;i++)
	{
		if(particles[i].on == true)
		{
			switch(particles[i].ptype)
			{
			case BULLET:
				{
					int px;
					int py;
					level.GetPointFromPos(particles[i].pos,px,py);
					if(px == 0 && py == 0)
					{
						// invalid particle
						killparticle(i);
						continue;
					}

					mxhwnd.paint.mxdrawellipse(px,py,px+5,py+5,SDL_MapRGB(front->format,180,180,180),SDL_MapRGB(front->format,200,200,200));
				}
				break;
			case BALL:
				{
					int px;
					int py;
					level.GetPointFromPos(particles[i].pos,px,py);
					if(px == 0 && py == 0)
					{
						killparticle(i);
					}

					COLORREF rcolor = SDL_MapRGB(front->format,rand()%255,rand()%255,rand()%255);
					
					mxhwnd.paint.mxdrawellipse(px,py,px+15,py+15,rcolor,rcolor);

				}
				break;
			}
		}
	}
	
}

void ParticleHandler::killparticle(int pos)
{
	particles[pos].on = false;
	particles[pos].ptype = BULLET;
}

void ParticleHandler::procparticles()
{
	for(int i = 0; i <= MAX_PARTICLE;i++)
	{
		if(particles[i].on == true)
		{
			switch(particles[i].ptype)
			{
			case BULLET:
				{
					if(particles[i].dir == true)
					{

					particles[i].pos += 24;

					}
					else
					{
						particles[i].pos -= 24;
					}

					// check and see if the particles dead

					if (level.blocks[particles[i].pos].solid == true)
					{
						killparticle(i);
						continue;
					}

					// see if this particle is clipping the grandma.

					Grandma* grandma = GetGrandma();
					int gpos;
					int ppos;
					ppos = particles[i].pos;
					gpos = grandma->pos;

					if(gpos == ppos || gpos + 1 == ppos || gpos + 2 == ppos || gpos + 3 == ppos || gpos + 4 == ppos || gpos - 24 == ppos || gpos - 24 + 1 == ppos || gpos - 24 + 2 == ppos || gpos - 24 + 3 == ppos || gpos -24 +4 == ppos)
					{
						grandma->die();
						killparticle(i);
					}
					
					if(gpos - rows(1) == ppos || gpos - rows(1) + 1 == ppos || gpos - rows(1) + 2 == ppos || gpos - rows(1) + 3 == ppos || gpos - rows(1) + 4 == ppos)
					{
						grandma->die();
						killparticle(i);
					}
				 
				}
				break;
			case BALL:
				{
					if(particles[i].dir == true)
					{
						particles[i].pos += 24;
					}
					else
					{
						particles[i].pos -= 24;
					}

			
					if(level.blocks[particles[i].pos].solid == true)
					{
						killparticle(i);
					}

					// see if the particle is clipping into the hero

					Hero* hero = GetHero();

		    		int gpos;
					int ppos;
					ppos = particles[i].pos;
					gpos = hero->hero_pos;

					if ( (gpos == ppos || gpos + 1 == ppos || gpos + 2 == ppos || gpos + 3 == ppos || gpos + 4 == ppos) || (gpos == ppos + rows(1) || gpos + 1 + rows(1) == ppos || gpos + 2 + rows(1) == ppos || gpos + 3 + rows(1) == ppos || gpos + 4 + rows(1) == ppos) )
					{
						hero->die();
						killparticle(i);
					}			
				 
				}
				break;
			}

		}
	}
}



void ParticleHandler::releaseparticle(int pos,bool dir, PARTICLETYPE ptype)
{
	int off;
	off = getoffparticle();
	particles[off].on = true;
	particles[off].ptype = ptype;
	particles[off].pos = pos;
	particles[off].dir = dir;
}


void ParticleHandler::releaseparticle(int pos,bool dir,bool idir, PARTICLETYPE ptype)
{
	int off;
	off = getoffparticle();
	particles[off].on = true;
	particles[off].ptype = ptype;
	particles[off].pos = pos;
	particles[off].dir = dir;
	particles[off].udir = idir;
}



// *********************************************************************** www.lostsidedead.com