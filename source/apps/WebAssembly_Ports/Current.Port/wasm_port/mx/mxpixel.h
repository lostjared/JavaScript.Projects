/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/



#ifndef _FILE_PIXEL_H_
#define _FILE_PIXEL_H_
#include "SDL.h"
#include "mxsurface.h"


namespace mx
{

	class mxPixelData;


	class mxPixel {


	public:
		enum { DIR_UPRIGHT, DIR_DOWNRIGHT, DIR_DOWNLEFT, DIR_UPLEFT, DIR_UPRIGHT_EX, DIR_UP, DIR_RIGHT };

		mxPixel();
		mxPixel(unsigned int color_value);
		mxPixel(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
		mxPixel(const mxPixel &p);

		Uint32 GetMappedColor(SDL_PixelFormat *format);
		void   GetSDLColor(SDL_Color *col);
		void   SetMappedColor(SDL_PixelFormat *format, Uint32 color);
		void   setColor(unsigned int color);

		const bool isActive() const; // is this pixel dead or active?

		// must call this function
		void setBounds(const int bx, const int by, const int bw, const int bh);
		void setNoRespawn(bool spawn);
		void setPos(int x, int y) { this->x = x, this->y = y; }
		void setPixel(const mxPixel &p);
		void setPixel(int x, int y, int direction);
		void setDirection(int direction);
		void setAlpha(float alpha, bool on);
		void setVibration(float alpha, bool on);
		void setSpeed(int speed);
		void clearPixel();



		void resetPixel();


		mxPixel &operator=(const mxPixel &p);
		mxPixel &operator+=(const mxPixel &p);
		friend  mxPixel operator+(const mxPixel &p, const mxPixel &p2);
		mxPixel &operator-=(const mxPixel &p);
		friend  mxPixel operator-(const mxPixel &p, const mxPixel &p2);
		mxPixel &operator*=(const mxPixel &p);
		friend mxPixel operator*(const mxPixel &p, const mxPixel &p2);
		mxPixel &operator/=(const mxPixel &p);
		friend mxPixel operator/(const mxPixel &p, const mxPixel &p2);

		void operator++();// increase position
		void operator++(int); // increase position
		~mxPixel();

		const int getx() const { return x; }
		const int gety() const { return y; }


	protected:

		friend class mxPixelData;

		int x,y,speed,direction;
		int sx,sy,sw,sh;

		union {

			unsigned char rgb[4];
			unsigned int  value;

		} color;

		float alpha, distance, vibration;
		bool dead, no_respawn;

		void moveByDirection();
		void boundsCheck();
	};

	class mxPixelData {

	public:



		mxPixelData();
		~mxPixelData();
		bool initDataFromSurface(mx::mxSurface &surface);
		void copyDataToSurface(mx::mxSurface &surf, int amt);

		mxPixel **pixelData() { return data; }
		const int getX() const { return sx; }
		const int getY() const { return sy; }

	protected:
		mxPixel **data;
		int sx, sy;
		mxSurface surf;

	};

}






#endif



