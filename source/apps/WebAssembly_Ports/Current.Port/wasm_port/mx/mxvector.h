/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/


#ifndef __MX__TEMPX_H_
#define __MX__TEMPX_H_

#include<iostream>
#include<cstdlib>
#include<cmath>
#include "mx_exception.h"

// written by jared bruni
// email me at jared@lostsidedead.biz


namespace mx
{
	// expandable being, any type, with as many elements as allowed by vector_size

	template<typename Type, size_t vector_size>
	class mxExpandableVector {

	public:
		typedef struct _vsize {
			Type data[vector_size];
		} data_vector;

		enum { VECTOR_SIZE = vector_size };

		mxExpandableVector();
		~mxExpandableVector() { } // no throw


		mxExpandableVector(const mxExpandableVector<Type, vector_size> &t);
		mxExpandableVector<Type, vector_size> &operator=(const mxExpandableVector<Type, vector_size> &t);
		void scaleVector(Type amount);
		void setVector(const mxExpandableVector<Type, vector_size> &value);
		mxExpandableVector<Type, vector_size> scaleNewVector(Type amount);
		data_vector &getVector() const { return vector_data; }

		mxExpandableVector<Type, vector_size> &operator+=(const mxExpandableVector<Type, vector_size> &);

mxExpandableVector<Type, vector_size> &operator+(const mxExpandableVector<Type, vector_size> &);



		friend mxExpandableVector<Type, vector_size> operator+(const mxExpandableVector<Type, vector_size> &x1, const mxExpandableVector<Type, vector_size> &x2)
		{
			mxExpandableVector<Type, vector_size> temp(x1);
			temp+=x2;
			return  temp;
		}

		mxExpandableVector<Type, vector_size> &operator-=(const mxExpandableVector<Type, vector_size> &);
		mxExpandableVector<Type, vector_size> &operator-(const mxExpandableVector<Type, vector_size> &);

		friend mxExpandableVector<Type, vector_size> operator-(const mxExpandableVector<Type, vector_size> &x1, const mxExpandableVector<Type, vector_size> &x2)
		{
			mxExpandableVector<Type, vector_size> temp(x1);
			temp-=x2;
			return  temp;
		}


		mxExpandableVector<Type, vector_size> &operator*=(const mxExpandableVector<Type, vector_size> &);
		mxExpandableVector<Type, vector_size> &operator*(const mxExpandableVector<Type, vector_size> &);

		friend mxExpandableVector<Type, vector_size> operator*(const mxExpandableVector<Type, vector_size> &x1, const mxExpandableVector<Type, vector_size> &x2)
		{
			mxExpandableVector<Type, vector_size> temp(x1);
			temp*=x2;
			return  temp;
		}
		mxExpandableVector<Type, vector_size> &operator/(const mxExpandableVector<Type, vector_size> &);


		friend mxExpandableVector<Type, vector_size> operator/(const mxExpandableVector<Type, vector_size> &x1, const mxExpandableVector<Type, vector_size> &x2)
		{
			mxExpandableVector<Type, vector_size> temp(x1);
			temp/=x2;
			return  temp;
		}


		mxExpandableVector<Type, vector_size> &operator/=(const mxExpandableVector<Type, vector_size> &);

		Type &operator[](unsigned int index);


		float DotProduct(const mxExpandableVector<Type, vector_size> &v);
		float Length();
		void Normalize();
		float Cos(const mxExpandableVector<Type, vector_size> &v);
		void Build(const mxExpandableVector<Type, vector_size> &v);
		mxExpandableVector<Type, vector_size> Build(mxExpandableVector<Type, vector_size> &v1, mxExpandableVector<Type, vector_size> v2);

		friend std::ostream &operator<<(std::ostream &out, const mxExpandableVector<Type, vector_size> &v)
		{

			out << "<";
			unsigned int i;
			for(i=0; i < mxExpandableVector<Type, vector_size>::VECTOR_SIZE-1; i++) out << v.vector_data.data[i] << ",";
			out << v.vector_data.data[i] << ">";
			return out;
		}

	protected:
		data_vector vector_data;


	};


	// size is 4 for this inherited type
	template<typename Type, size_t size>
	class vector4D : public mxExpandableVector<Type, size> {
	public:
		vector4D(const vector4D<Type,size> &v);
		vector4D();

		Type &x,&y,&z,&w;

	};

	template<typename Type, size_t size>
	class vector2D : public mxExpandableVector<Type, size> {
	public:
		vector2D(const vector2D<Type, size> &v);
		vector2D();

		Type &x, &y;
		void setPos(Type sx, Type sy) { x = sx, y = sy; }
		void getPos(Type &sx,Type &sy) { sx = x, sy = y; }

	};

	template<typename Type, size_t vector_size>
	vector2D<Type, vector_size>::vector2D() : x(this->vector_data.data[0]), y(this->vector_data.data[1])
	{
		this->vector_data.data[0] = 0;
		this->vector_data.data[1] = 0;
	}

	template<typename Type, size_t vsize>
	vector2D<Type, vsize>::vector2D(const vector2D<Type, vsize> &v) : vector2D()
	{
		setVector(v);
	}


	// implementation
	template<typename Type, size_t vector_size>
	vector4D<Type, vector_size>::vector4D() : x(this->vector_data.data[0]), y(this->vector_data.data[1]),z(this->vector_data.data[2]), w(this->vector_data.data[3])
	{
		this->vector_data.data[0] = 0;
		this->vector_data.data[1] = 0;
		this->vector_data.data[2] = 0;
		this->vector_data.data[3] = 0;

	}
	template<typename Type, size_t vector_size>
	vector4D<Type,vector_size>::vector4D(const vector4D<Type,vector_size> &v) : vector4D()
	{
		setVector(v);
	}


	template<typename Type, size_t vector_size>
	mxExpandableVector<Type, vector_size>::mxExpandableVector()
	{

		for(unsigned int i = 0; i < VECTOR_SIZE; i++) vector_data.data[i] = 0;

	}




	template<typename Type, size_t vector_size>
	mxExpandableVector<Type,vector_size>::mxExpandableVector(const mxExpandableVector<Type, vector_size> &t)
	{
			setVector(t);

	}


	template<typename Type, size_t vector_size>
	mxExpandableVector<Type, vector_size> &mxExpandableVector<Type, vector_size>::operator=(const mxExpandableVector<Type, vector_size> &t)
	{
			setVector(t);
			return *this;
	}



	template<typename Type, size_t vector_size>
	void mxExpandableVector<Type, vector_size>::setVector(const mxExpandableVector<Type, vector_size> &value)
	{
		for(unsigned int i = 0; i < VECTOR_SIZE; i++) vector_data.data[i] = value.vector_data.data[i];

	}

	template<typename Type, size_t vector_size>
	void mxExpandableVector<Type,vector_size>::scaleVector(Type amount)
	{
		for(unsigned int i = 0; i < VECTOR_SIZE; i++) vector_data.data[i] *= amount;
	}

	template<typename Type, size_t vector_size>
	mxExpandableVector<Type, vector_size> mxExpandableVector<Type,vector_size>::scaleNewVector(Type amount)
	{
		mxExpandableVector<Type, vector_size> v = *this;
		for(unsigned int i = 0; i < VECTOR_SIZE; i++) v.vector_data.data[i] *= amount;
		return v;

	}


	template<typename Type, size_t vector_size>
	float mxExpandableVector<Type, vector_size>::DotProduct(const mxExpandableVector<Type, vector_size> &value)
	{

		float values = 0;

		for(unsigned int i = 0; i < VECTOR_SIZE; i++)
		{
			float temp = (float) vector_data.data[i]*value.vector_data[i];
			values += temp;
		}

		return values;
	}


	template<typename Type, size_t vector_size>
	mxExpandableVector<Type, vector_size> &mxExpandableVector<Type, vector_size>::operator+=(const mxExpandableVector<Type, vector_size> &v)
	{

		for(unsigned int i = 0; i < VECTOR_SIZE; i++) vector_data.data[i] += v.vector_data[i];
		return *this;

	}


	template<typename Type, size_t vector_size>
	mxExpandableVector<Type, vector_size> &mxExpandableVector<Type, vector_size>::operator+(const mxExpandableVector<Type, vector_size> &v)
	{

		for(unsigned int i = 0; i < VECTOR_SIZE; i++) vector_data.data[i] += v.vector_data[i];
		return *this;

	}

	template<typename Type, size_t vector_size>
	mxExpandableVector<Type, vector_size> &mxExpandableVector<Type, vector_size>::operator-=(const mxExpandableVector<Type, vector_size> &v)
	{


		for(unsigned int i = 0; i < VECTOR_SIZE; i++) vector_data.data[i] -= v.vector_data[i];
		return *this;


	}

	template<typename Type, size_t vector_size>
	mxExpandableVector<Type, vector_size> &mxExpandableVector<Type, vector_size>::operator-(const mxExpandableVector<Type, vector_size> &v)
	{

		for(unsigned int i = 0; i < VECTOR_SIZE; i++) vector_data.data[i] -= v.vector_data[i];
		return *this;

	}

	template<typename Type, size_t vector_size>
	mxExpandableVector<Type, vector_size> &mxExpandableVector<Type, vector_size>::operator*=(const mxExpandableVector<Type, vector_size> &v)
	{

		for(unsigned int i = 0; i < VECTOR_SIZE; i++) vector_data.data[i] *= v.vector_data[i];
		return *this;


	}

	template<typename Type, size_t vector_size>
	mxExpandableVector<Type, vector_size> &mxExpandableVector<Type, vector_size>::operator*(const mxExpandableVector<Type, vector_size> &v)
	{


		for(unsigned int i = 0; i < VECTOR_SIZE; i++) vector_data.data[i] *= v.vector_data[i];
		return *this;


	}

	template<typename Type, size_t vector_size>
	mxExpandableVector<Type, vector_size> &mxExpandableVector<Type, vector_size>::operator/(const mxExpandableVector<Type, vector_size> &v)
	{


		for(unsigned int i = 0; i < VECTOR_SIZE; i++) vector_data.data[i] /= v.vector_data[i];
		return *this;

	}


	template<typename Type, size_t vector_size>
	mxExpandableVector<Type, vector_size> &mxExpandableVector<Type, vector_size>::operator/=(const mxExpandableVector<Type, vector_size> &v)
	{

		for(unsigned int i = 0; i < VECTOR_SIZE; i++) vector_data.data[i] /= v.vector_data[i];
		return *this;

	}

	template<typename Type, size_t vector_size>
	float mxExpandableVector<Type, vector_size>::Length()
	{

		float total = 0;

		for(unsigned int i = 0; i < VECTOR_SIZE; i++)
		{

			float value = (float) vector_data.data[i]*vector_data.data[i];
			total += value;
		}

		return sqrtf(total);
	}

	template<typename Type, size_t vector_size>
	void mxExpandableVector<Type, vector_size>::Normalize()
	{

		float length = Length();
		if ( length < float(1E-5))
			return;
		float inv = 1/length;

		for(unsigned int i = 0; i < VECTOR_SIZE; i++) vector_data.data[i] *= inv;
	}


	template<typename Type, size_t vector_size>
	float mxExpandableVector<Type, vector_size>::Cos(const mxExpandableVector<Type, vector_size> &v)
	{

		float dot = DotProduct(v);
		return  (dot / Length() * v.Length());

	}


	// function may throw a mxException if user provides a out index which is out of bounds of the size
	// of the vector. Starts and 0 and ends with the vector size.

	template<typename Type, size_t vector_size>
	Type &mxExpandableVector<Type, vector_size>::operator[](unsigned int index)
	{
		if(!(index < VECTOR_SIZE))
			throw mx::mxException<std::string>(" Error out of vector bounds \n ");

		return vector_data.data[index];

	}

	template<typename Type, size_t vector_size>
	void mxExpandableVector<Type, vector_size>::Build(const mxExpandableVector<Type, vector_size> &v)
	{
		for(unsigned int i = 0; i < VECTOR_SIZE; i++) vector_data.data[i] = v.vector_data[i] - vector_data[i];

	}


	template<typename Type, size_t vector_size> mxExpandableVector<Type, vector_size>
	mxExpandableVector<Type, vector_size>::Build(mxExpandableVector<Type, vector_size> &v1, mxExpandableVector<Type, vector_size> v2)
	{
		mxExpandableVector<Type,vector_size> temp;

		for(unsigned int i = 0; i < VECTOR_SIZE; i++) temp.vector_data[i] = v2.vector_data[i] - v1.vector_data[i];

		return temp;
	}

	typedef vector4D<float, 4> mxfVector;
	typedef vector4D<int,   4> mxiVector;
	typedef vector2D<int,   2> mxiPoint;
	typedef vector2D<int,   2> mxfPoint;
}




#endif


