/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/


/*
 *
 * this source file is not very useful
 *
 */


#ifndef __COLOR_VECTOR___
#define __COLOR_VECTOR___

#include<cstdlib>
#include<iostream>

namespace color {


	union _colorVertex {
		float r,g,b,a;
		float rgb[4];
	};

	class Color3D {


	public:
		Color3D();
		Color3D(const Color3D &c);
		void setcolor(float r, float g, float b, float a);
		const union _colorVertex &getColor() const;
		void add(const Color3D &c);
		void sub(const Color3D &c);
		void mul(const Color3D &c);
		void div(const Color3D &c);
		unsigned int toInteger() const;
		Color3D &operator=(const Color3D &c);
		Color3D& operator/=(const Color3D &c) { div(c); return *this; }
		Color3D& operator*=(const Color3D &c) { mul(c); return *this; }
		Color3D& operator-=(const Color3D &c) { sub(c); return *this; }
		Color3D& operator+=(const Color3D &c) { add(c); return *this; }
		Color3D& operator+(const Color3D &c) { add(c); return *this; }
		Color3D& operator-(const Color3D &c) { sub(c); return *this; }
		Color3D& operator*(const Color3D &c) { mul(c); return *this; }
		Color3D& operator/(const Color3D &c) { div(c); return *this; }
		friend std::ostream& operator<<(std::ostream& out, const Color3D &c);
		union _colorVertex color;
	};


	template<std::size_t x, std::size_t y>
	class M4Color3D {

	public:
		Color3D row[x][y];
		M4Color3D()
		{
			w=x,h=y;

		}

		void randbuffer()
		{
			unsigned int i,z;
			for(i =0; i< w; i++)
				for(z = 0; z < h; z++)
				{
					row[i][z].color.rgb[0] = rand()%255;
					row[i][z].color.rgb[1] = rand()%255;
					row[i][z].color.rgb[2] = rand()%255;
					row[i][z].color.rgb[3] = rand()%255;
				}
		}

		void setp(int xvar, int yvar, Color3D &col)
		{
			row[xvar][yvar] = col;
		}

		void add(const M4Color3D  &c)
		{
			unsigned int i,z;

			for(i = 0; i < w; i++)
				for(z = 0; z < y; z++)
					row[i][z] += c.row[i][z];
		}

		void sub(const M4Color3D  &c)
		{
			unsigned int i,z;

			for(i = 0; i < w; i++)
				for(z = 0; z < y; z++)
					row[i][z] -= c.row[i][z];

		}

		void mul(const M4Color3D &c)
		{
			unsigned int i,z;

			for(i = 0; i < w; i++)
				for(z = 0; z < y; z++)
					row[i][z] *= c.row[i][z];

		}
		void div(const M4Color3D &c)
		{
			unsigned int i,z;

			for(i = 0; i < w; i++)
				for(z = 0; z < y; z++)
				if(c.row[i][z] != 0)
					row[i][z] /= c.row[i][z];

		}

		void addColor3D(const Color3D &c)
		{

			for(unsigned int i = 0; i < w; i++)
				for(unsigned int z = 0; z < h; z++)
					row[i][z] += c;

		}

		void mulColor3D(const Color3D &c)
		{
			for(unsigned int i = 0; i < w; i++)
				for(unsigned int z = 0; z < h; z++)
					row[i][z] *= c;
		}

		void subColor3D(const Color3D &c)
		{

			for(unsigned int i = 0; i < w; i++)
				for(unsigned int z = 0; z < h; z++)
					row[i][z] -= c;

		}


		void divColor3D(const Color3D &c)
		{
			for(unsigned int i = 0; i < w; i++)
				for(unsigned int z = 0; z < h; z++)
					row[i][z] /= c;

		}


		void printData() const
		{
			for(unsigned int i = 0; i < w; i++)
				for(unsigned int z = 0; z < h; z++)
					std::cout << row[i][z] << "\n";
		}

		void triblend()
		{

			unsigned int i,z;

			for(i = 1; i < w-1; i++)
				for(z = 1; z < h-1; z++)
				{
					Color3D c[3];
					// *
					c[0] = row[i][z];
					c[1] = row[i][z+1]; // *
					c[2] = row[i+1][z];
					// *
					row[i-1][z+1] = c[0] - c[1] * c[2];
				}
		}

		void upper_left()
		{
			unsigned int i,z;

			for(i = 1; i < w-1; i++)
				for(z = 1; z <  h-1; z++)
				{

					Color3D col = row[i][z];
					row[i-1][z-1] = col;

				}

		}


		void increase(float ramt, float gamt, float bamt)
		{
			unsigned int i,z;

			for(i = 0; i < w; i++)
				for(z = 0; z < h; z++)
				{

					row[i][z].color.rgb[0] += ramt;
					row[i][z].color.rgb[1] += gamt;
					row[i][z].color.rgb[2] += bamt;

				}

		}

		unsigned int w, h;
	};

}

 #endif


