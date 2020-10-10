/************************************************************************
     File:        Vertex.cpp

     Author:     
                  Stephen Chenney, schenney@cs.wisc.edu
     Modifier
                  Yu-Chi Lai, yu-chi@cs.wisc.edu

     Comment:    
						(c) 2001-2002 Stephen Chenney, University of Wisconsin at Madison

						Class header file for Vertex class. Stores data about vertices in the maze.
		

     Platform:    Visio Studio.Net 2003 (converted to 2005)

*************************************************************************/
#include <stdio.h>
#include "Vertex.h"

const char  Vertex::X = 0;
const char  Vertex::Y = 1;

//*********************************************************************************
//
// * Constructor
//=================================================================================
Vertex::
Vertex(int i, float x, float y)
//=================================================================================
{
	index = i;
	posn[X] = x;
	posn[Y] = y;
}

