/************************************************************************
     File:        Vertex.h

     Author:     
                  Stephen Chenney, schenney@cs.wisc.edu
     Modifier
                  Yu-Chi Lai, yu-chi@cs.wisc.edu

     Comment:    
						(c) 2001-2002 Stephen Chenney, University of Wisconsin at Madison

						Class header file for Vertex class. Stores data about vertices in the maze.
		

     Platform:    Visio Studio.Net 2003 (converted to 2005)

*************************************************************************/

#ifndef _VERTEX_H_
#define _VERTEX_H_

class Vertex {
	public:
		// Constructor
		Vertex(int index, float x, float y);

	public:
		// Index constant for x, y
		static const char X;
		static const char	Y;
		int	  index;		// An identifier
		float   posn[2];	// The location (x and y)

};


#endif
