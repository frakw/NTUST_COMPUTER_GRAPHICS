/************************************************************************
     File:        Maze.cpp

     Author:     
                  Stephen Chenney, schenney@cs.wisc.edu
     Modifier
                  Yu-Chi Lai, yu-chi@cs.wisc.edu

     Comment:    
						(c) 2001-2002 Stephen Chenney, University of Wisconsin at Madison

						A class for points.
		

     Platform:    Visio Studio.Net 2003 (converted to 2005)

*************************************************************************/

#ifndef _POINT_H_
#define _POINT_H_


class Point {
	public:
		// Constructor to set up x, y, z
		Point(const float x, const float y, const float z) {
			p[0] = x; p[1] = y; p[2] = z;
		}
		// Copy contructor
		Point(const Point &p_in) {
			p[0] = p_in.p[0]; p[1] = p_in.p[1]; p[2] = p_in.p[2];
		}

	public:
		// Assign operator
		Point&  operator=(const Point &p_in) {
			p[0] = p_in.p[0]; p[1] = p_in.p[1]; p[2] = p_in.p[2];
			return *this;
		}

		// Index operator
		float&  operator[](const int i) { return p[i]; }

		// Equal operator
		bool    operator==(const Point &other) {
			double x = p[0] - other.p[0];
			if ( x > 1.0e-8 )
				return false;

			double y = p[1] - other.p[1];
			if ( y > 1.0e-8 )
				return false;
			double z = p[2] - other.p[2];
			if ( z > 1.0e-8 )
				return false;
			return x * x + y * y + z * z < 1.0e-15;
		}

		// Inequal operator
		bool    operator!=(const Point &other) {
			double x = p[0] - other.p[0];
			if ( x > 1.0e-8 )
				return true;
			double y = p[1] - other.p[1];
			if ( y > 1.0e-8 )
				return true;

			double z = p[2] - other.p[2];
			if ( z > 1.0e-8 )
				return true;

			return x * x + y * y + z * z >= 1.0e-15;
		}
  private:
    float   p[3];
};

#endif
