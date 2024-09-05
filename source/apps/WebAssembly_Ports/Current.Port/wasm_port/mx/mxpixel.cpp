/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/

#include "mxpixel.h"
#include<cstring>
#include<cstdlib>
#include<iostream>

namespace mx
{



	mxPixel::mxPixel() : dead(false), no_respawn(false)
	{
		x = 0, y = 0, speed = 0, direction = 0;
		alpha = 0, distance = 0, vibration = 0;
		color.value = 0;
		sx = sy = sw = sh  = 0;
	}

	mxPixel::~mxPixel()
	{

	}

	mxPixel::mxPixel(unsigned int color_value) : dead(false), no_respawn(false)
	{
		x = 0, y = 0, speed = 0, direction = 0;
		alpha = 0, distance = 0, vibration = 0;
		color.value = color_value;
		sx = sy = sw = sh  = 0;


	}

	mxPixel::mxPixel(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : dead(false), no_respawn(false)
	{

		x = 0, y = 0, speed = 0, direction = 0;
		alpha = 0, distance = 0, vibration = 0;
		sx = sy = sw = sh  = 0;

		color.rgb[0] = r;
		color.rgb[1] = g;
		color.rgb[2] = b;
		color.rgb[3] = a;
	}

	mxPixel::mxPixel(const mxPixel &p) : dead(false),  no_respawn(false)
	{
		setPixel(p);

	}

	void mxPixel::setPixel(const mxPixel &p)
	{
		direction = p.direction;
		color.value = p.color.value;
		x = p.x;
		y = p.y;
		speed = p.speed;
		alpha = p.alpha;
		distance = p.distance;
		vibration = p.vibration;
		sx = p.sx;
		sy = p.sy;
		sw = p.sw;
		sh = p.sh;
		no_respawn = p.no_respawn;
		dead = p.dead;
	}

	void mxPixel::setPixel(int x, int y, int direction)
	{

		this->x = x, this->y = y, this->direction = direction;

	}

	void mxPixel::clearPixel()
	{

		x = 0;
		y = 0;
		direction = 0;
		memset(&color, 0, sizeof(color));
		vibration = 0;
		speed = 0;
		alpha = 0;
		distance = 0;
		vibration = 0;
	}


	mxPixel &mxPixel::operator=(const mxPixel &p)
	{

		setPixel(p);

		return *this;
	}

	mxPixel &mxPixel::operator+=(const mxPixel &p)
	{
		color.value += p.color.value;
		vibration += p.vibration;
		return *this;
	}


	Uint32 mxPixel::GetMappedColor(SDL_PixelFormat *format)
	{
		return SDL_MapRGB(format, color.rgb[0], color.rgb[1], color.rgb[2]);
	}


	void mxPixel::SetMappedColor(SDL_PixelFormat *format, Uint32 color_i)
	{

		SDL_GetRGBA(color_i, format, &color.rgb[0], &color.rgb[1], &color.rgb[2], &color.rgb[3]);

	}

	void mxPixel::setColor(unsigned int color)
	{
		this->color.value = color;
	}

	void mxPixel::GetSDLColor(SDL_Color *col)
	{
		col->r = color.rgb[0];
		col->g = color.rgb[1];
		col->b = color.rgb[2];
	}

	void mxPixel::setDirection(int direction)
	{
		this->direction = direction;
	}

	void mxPixel::setAlpha(float alpha, bool on)
	{

		if(on) this->alpha = alpha;
		else alpha = 0.0f;

	}

	void mxPixel::setVibration(float alpha, bool on)
	{
		if(on) vibration = alpha;
		else vibration = 0.0f;

	}

	void mxPixel::setSpeed(int speed)
	{
		this->speed = speed;
	}


	mxPixel operator+(const mxPixel &p, const mxPixel &p2)
	{
		mxPixel ptemp(p);
		ptemp += p2;
		return ptemp;
	}


	mxPixel &mxPixel::operator-=(const mxPixel &p)
	{
		color.value -= p.color.value;
		vibration -= p.vibration;
		return *this;

	}

	mxPixel operator-(const mxPixel &p, const mxPixel &p2)
	{
		mxPixel temp(p);
		temp += p2;
		return temp;
	}

	mxPixel &mxPixel::operator*=(const mxPixel &p)
	{

		color.value *= p.color.value;
		vibration *= p.vibration;
		return *this;

	}

	mxPixel operator*(const mxPixel &p, const mxPixel &p2)
	{
		mxPixel temp(p);
		temp+=p2;
		return temp;
	}

	mxPixel &mxPixel::operator/=(const mxPixel &p)
	{

		if(p.color.value != 0 && color.value != 0)
			color.value /= p.color.value;

		if(p.vibration != 0 && vibration != 0)
			vibration /= p.vibration;

		return *this;


	}

 	mxPixel operator/(const mxPixel &p, const mxPixel &p2)
 	{
		mxPixel ptr(p);
		ptr += p2;
		return ptr;
	}



	const bool mxPixel::isActive() const
	{

		if(no_respawn == true && dead == true)
			return false;

		return true;
	}


	void mxPixel::moveByDirection()
	{

		if(dead == true) return;

		int &x_pos = this->x;
		int &y_pos = this->y;


		switch ( direction )
		{
                case 0: // across and up
                        x_pos += 2;
                        y_pos -= 2;
                        break;
                case 1:// across and down
                        x_pos++;
                        y_pos++;
                        break;
                case 2:// to the left
			// down
                        x_pos--;
                        y_pos++;
                        break;
                case 3:// to the right
			// and up
                        y_pos--;
                        x_pos--;
                        break;
                case 4:// across
			// up
                        x_pos++;
                        y_pos--;
                        break;
                case 5: // up
                        y_pos--;
                        break;
                case 6: // across
                        y_pos++;
                        break;

        	}

		boundsCheck();

	}

	void mxPixel::boundsCheck()
	{

		if(!(x > 0 && x < sw && y > 0 && y < sh))
		{

			resetPixel();

		}

	}

	void mxPixel::setBounds(const int bx, const int by, const int bw, const int bh)
	{
		sx = bx, sy = by, sw = bw, sh = bh;
	}

	void mxPixel::resetPixel()
	{
		if(no_respawn == true)
		{
			this->dead = true;
			x = 0;
			y = 0;
			return;
		}
		setPixel(rand()%sw, rand()%sh, rand()%6); // random respawn
	}


	void mxPixel::operator++(int)
	{
		moveByDirection();
	}

	void mxPixel::operator++()
	{
		moveByDirection();
	}

	void mxPixel::setNoRespawn(bool spawn)
	{
		no_respawn = spawn;
	}





 	mxPixelData::mxPixelData()
 	{
		data = 0;
		surf = 0;

	}

	mxPixelData::~mxPixelData()
	{
		if(data  != 0)
		{
			for(int z = 0; z < sx; z++)
			delete [] data[z];

			delete [] data;
		}
	}

	bool mxPixelData::initDataFromSurface(mx::mxSurface &surface)
	{

		sx = surface.getSurface()->w;
		sy = surface.getSurface()->h;

		surface.lockSurface();

		try
		{


			data = new  mxPixel*[surface.getSurface()->w];

			for(int x = 0; x < surface.getSurface()->w; x++)
			{
				data[x] = new mxPixel[surface.getSurface()->h];

				for(int y = 0; y < surface.getSurface()->h; y++)
				{
					Uint32 color;
					color = surface[static_cast<unsigned int>(x+(y*surface.width()))];
					data[x][y].setBounds(0, 0, surface.getSurface()->w, surface.getSurface()->h);
					data[x][y].SetMappedColor(surface, color);
					data[x][y].setPos(x,y);
					data[x][y].setDirection(rand()%6);

				}

			}

			surface.unlockSurface();

		}

		catch(...)
		{

			for(int z = 0; z < surface.width(); z++)
				delete [] data[z];

			delete [] data;
			std::cerr << "error converting image, prob out of memory.\n";
			throw;
		}


		return true;
	}



	void mxPixelData::copyDataToSurface(mx::mxSurface &surf, int amt)
	{

		surf.lockSurface();

		int index = 0;


		for(int i = 0; i < sx; i++)
		{
			for(int z = 0; z  < sy; z++)
			{

				if(((++index)%amt) != 0) continue;

				data[i][z]++;

				surf[static_cast<unsigned int>(data[i][z].x+(data[i][z].y*surf.getSurface()->w))] = data[i][z].GetMappedColor(surf);


			}

		}

		surf.unlockSurface();

	}


}



