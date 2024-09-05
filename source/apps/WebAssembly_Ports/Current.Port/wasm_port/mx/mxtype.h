/*
	MX library.

	http://lostsidedead.com/svn


	libmx written by jared bruni, it is wrappers around SDL
and other api's to provide a set of classes and functions
to produce multi media applications, using object oriented
techniques.

 	(C) 2008 LostSideDead
*/


#ifndef __MX_TYPE_X_H_
#define __MX_TYPE_X_H_

#include "SDL.h"

namespace mx {


	/*! \struct Point
	 *  structure to hold a pair of integers
	 */
	struct Point {
		/*! x coordinate */
		int x;
		/*! y coordinate */
		int y;
		/*! constructor to set x,y values */
		Point(int x=0, int y=0)
		{
			this->x = x, this->y = y;
		}
	};
	/*! \struct Size
	 *  structure to hold width and height variables for coordinates
	 */
	struct Size {

		/*! width in size*/
		int width;
		/*! height in size */
		int height;
		/*! default constructor */
		Size() { width = height = 0; }
		/*! constructor to hold width height values
		 * \param width sets the width parameter
		 * \param height sets the height parameter
		 */
		Size(int width, int height)
		{
			this->width = width;
			this->height = height;
		}
	};

	/*! \struct Rect
	 *  Rectangle structure, holds a Point and a Size, for x,y,width,height
	 */
	struct Rect {
		/*! Point variable for X,Y */
		Point point;
		/*! Size variable for size width,height */
		Size  rsize;
		/*! default constructor */
		Rect() { }
		/*! Constructor
		 * \param rc takes a SDL_Rect and fills in point and rsize
		 */
		Rect(SDL_Rect &rc) : point(rc.x, rc.y) , rsize(rc.w, rc.h) {}
		/*! Constructor
		 * \param rc takes a Pointer to a SDL_Rect and sets point and rsize
		 */
		Rect(SDL_Rect *rc)
		{
			if(rc == 0) return;
			point = mx::Point(rc->x, rc->y);
			rsize = mx::Size(rc->w, rc->h);
		}

		/*! Constructor sets the rect from four integer's.
		 *  \param x x coordinate
		 *  \param y y coordinate
		 *  \param width width size
		 *  \param height height size
		 */
		Rect(int x, int y, int width, int height) : point(x,y), rsize(width, height) {}
		/*! Copy constructor sets this Rect's values from another Rect
		 * \param r Rectangle to set from
		 */
		Rect( const Rect &r ) : point(r.point), rsize(r.rsize)  {}
		/*! Constructor taking a Point and a Size
		 * \param p Point for X,Y
		 * \param s for Size width,height
		 */
		Rect( const Point &p, const Size &s ) : point(p), rsize(s) {}

		/*! width of the Rect
		 * \return returns width
		 */
		const int width() const { return rsize.width; }
		/*! height of the Rect
		 * \return returns height
		 */
		const int height() const { return rsize.height; }
		/*! x of the Rect
		 * \return returns the x coordinate
		 */
		const int x() const { return point.x; }
		/*! y of the Rect
		 * \return returns the y coordinate
		 */
		const int y() const { return point.y; }

		/*! is this Point inside this Rect ?
		 * \param p the Point
		 * \return true if it is, false if is not
		 */
		const bool pointInRect(Point p) const
		{
			if(p.x >= point.x && p.x <= point.x+rsize.width && p.y >= point.y && p.y <= point.y+rsize.height) return true;
			return false;
		}
		/*! operator SDL_Rect so Rect can be used as a SDL_Rect in expressions
		 * \return returns SDL_Rect
		 */
		operator SDL_Rect()  {
			SDL_Rect rc = { point.x, point.y, rsize.width, rsize.height };
			return rc;
		}
	};


	/*! \struct Line
	 * structure to stand for a line, containing two points
	 */
	struct Line {
		/*! starting point */
		Point p1;
		/*! ending point */
		Point p2;
		/*! default constructor */
		Line () { }
		/*! constructor taking two points
		 * \param p_1 point one (starting)
		 * \param p_2 point two (stoping)
		 */
		Line(Point p_1 , Point p_2) : p1(p_1), p2(p_2) { }
		/*! constructor taking 2 points in four variables
		 * \param start_x x1 coordinate
		 * \param start_y y1 coordinate
		 * \param stop_x  x2 coordinate
		 * \param stop_y  y2 coordinate
		 */
		Line(int start_x, int start_y, int stop_x, int stop_y)
		{
			p1 = Point(start_x, start_y);
			p2 = Point(stop_x, stop_y);
		}
		/*! return x1 \return x1 */
		const int x1() const { return p1.x; }
		/*! return y1 \return y1 */
		const int y1() const { return p1.y; }
		/*! return x2 \return x2 */
		const int x2() const { return p2.x; }
		/*! return y2 \return y2 */
		const int y2() const { return p2.y; }
	};
}
#endif
