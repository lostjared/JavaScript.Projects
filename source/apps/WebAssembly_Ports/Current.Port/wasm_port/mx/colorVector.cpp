/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/


#include "colorVector.h"
#include<stdlib.h>
#include<memory.h>

namespace color
{


unsigned int Color3D::toInteger() const
{

	union {
	unsigned char values[4];
	unsigned int value;
	} val;

 	val.values[0] = static_cast<unsigned char>(color.rgb[0]);
	val.values[1] = static_cast<unsigned char>(color.rgb[1]);
	val.values[2] = static_cast<unsigned char>(color.rgb[2]);
	val.values[3] = static_cast<unsigned char>(color.rgb[3]);

	return val.value;

}


std::ostream& operator<<(std::ostream& out, const Color3D &c)
{

	out << "[";
	unsigned int i;
	for(i = 0; i < 3; i++)
	{

		out << c.color.rgb[i] << ((i < 2) ? "," : "");

	}
	out << "]";

	return out;

}


Color3D::Color3D()
{


	memset(&color, 0, sizeof(color));

}

Color3D::Color3D(const Color3D &c)
{

	for(unsigned int i = 0; i < 4; i++)
	{

		color.rgb[i] = c.color.rgb[i];

	}


}


void Color3D::setcolor(float r, float g, float b, float a)
{

	color.rgb[0] = r, color.rgb[1] = g, color.rgb[2] = b, color.rgb[3] = a;

}


const union _colorVertex &Color3D::getColor() const
{
	return color;
}


void Color3D::add(const Color3D &c)
{

	color.rgb[0]+=c.color.rgb[0];
	color.rgb[1]+=c.color.rgb[1];
	color.rgb[2]+=c.color.rgb[2];
	color.rgb[3]+=c.color.rgb[3];


}
void Color3D::sub(const Color3D &c)
{


	color.rgb[0]-=c.color.rgb[0];
	color.rgb[1]-=c.color.rgb[1];
	color.rgb[2]-=c.color.rgb[2];
	color.rgb[3]-=c.color.rgb[3];


}
void Color3D::mul(const Color3D &c)
{

	color.rgb[0] *= c.color.rgb[0];
	color.rgb[1] *= c.color.rgb[1];
	color.rgb[2] *= c.color.rgb[2];
	color.rgb[3] *= c.color.rgb[3];


}
void Color3D::div(const Color3D &c)
{

	if(c.color.rgb[0] != 0)
		color.rgb[0] /= c.color.rgb[0];
	if(c.color.rgb[1] != 0)
		color.rgb[1] /= c.color.rgb[1];
	if(c.color.rgb[2] != 0)
		color.rgb[2] /= c.color.rgb[2];
	if(c.color.rgb[3] != 0)
		color.rgb[3] /= c.color.rgb[3];

}


Color3D &Color3D::operator=(const Color3D &c)
{

	for(unsigned int i = 0; i < 4; i++)
	{
		color.rgb[i] = c.color.rgb[i];
	}

	return *this;

}


}


