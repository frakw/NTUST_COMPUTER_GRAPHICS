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
#include "all_glm.h"
class Vertex {
	public:
		Vertex() {}
		// Constructor
		Vertex(glm::vec4& in,int x_index,int y_index) {
			posn[0] = in[x_index];
			posn[1] = in[y_index];
		}
		Vertex(const Vertex& copy) {
			posn[0] = copy.posn[0];
			posn[1] = copy.posn[1];
			index = copy.index;
		}
		Vertex(float x, float y);
		Vertex(int index, float x, float y);
		Vertex& operator=(const Vertex& i) {
			posn[0] = i.posn[0];
			posn[1] = i.posn[1];
			index = i.index;
			return *this;
		}
	public:
		// Index constant for x, y
		static const char X;
		static const char	Y;
		int	  index = -1;		// An identifier
		float   posn[2] = {0};	// The location (x and y)

};


#endif
