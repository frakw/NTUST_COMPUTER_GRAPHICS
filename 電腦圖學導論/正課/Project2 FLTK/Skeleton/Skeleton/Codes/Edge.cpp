/************************************************************************
     File:        Edge.cpp

     Author:     
                  Stephen Chenney, schenney@cs.wisc.edu
     Modifier
                  Yu-Chi Lai, yu-chi@cs.wisc.edu

     Comment:    
						(c) 2001-2002 Stephen Chenney, University of Wisconsin at Madison
						Class header file for Edge class. Stores data about edges in the maze.
		

     Platform:    Visio Studio.Net 2003 (converted to 2005)

*************************************************************************/

#include <stdio.h>
#include "Edge.h"
#include "Cell.h"


const char  Edge::LEFT		= 0;
const char  Edge::RIGHT		= 1;
const char  Edge::ON			= 2;
const char  Edge::NEITHER	= 3;

const char  Edge::START		= 0;
const char  Edge::END		= 1;


//***********************************************************************
//
// * Constructor to set up the start and end and the color of the edge
//=======================================================================
Edge::
Edge(int i, Vertex *start, Vertex *end, float r, float g, float b)
//=======================================================================
{
	index = i;
	endpoints[START] = start;
	endpoints[END] = end;

	neighbors[0] = neighbors[1] = NULL;

	opaque = true;

	color[0] = r;
	color[1] = g;
	color[2] = b;
}

//***********************************************************************
//
// * Returns which side of the edge a cell lies. Valid return values are
//	  LEFT, RIGHT or NEITHER if the cell is not a neighbor of the edge.
//   The left side of the edge is the side that would be on the left
//   if you were standing at the START of the edge and looking along it,
//   with your head in the z direction.
//=======================================================================
char Edge::
Cell_Side(Cell *cell)
//=======================================================================
{
	if ( cell == neighbors[LEFT] )
		return LEFT;
	else if ( cell == neighbors[RIGHT] )
		return RIGHT;

	return NEITHER;
}


//***********************************************************************
//
// * Returns which side of the edge the line (x,y) is on. The return value
//   is one of the constants defined above (LEFT, RIGHT, ON). See above
//   for a discussion of which side is left and which is right.
//=======================================================================
char Edge::
Point_Side(float x, float y)
//=======================================================================
{
	// Compute the determinant: | xs ys 1 |
	//                          | xe ye 1 |
	//                          | x  y  1 |
	// Use its sign to get the answer.

	float   det;

	det = endpoints[START]->posn[Vertex::X] * 
			( endpoints[END]->posn[Vertex::Y] - y ) - 
			endpoints[START]->posn[Vertex::Y] * 
			( endpoints[END]->posn[Vertex::X] - x ) +
			endpoints[END]->posn[Vertex::X] * y	- 
			endpoints[END]->posn[Vertex::Y] * x;
    
	if ( det == 0.0 )
		return ON;
	else if ( det > 0.0 )
		return LEFT;
	else
		return RIGHT;
}


