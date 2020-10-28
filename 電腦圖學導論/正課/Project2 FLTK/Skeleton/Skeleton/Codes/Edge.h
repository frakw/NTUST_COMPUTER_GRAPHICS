/************************************************************************
     File:        Edge.h

     Author:     
                  Stephen Chenney, schenney@cs.wisc.edu
     Modifier
                  Yu-Chi Lai, yu-chi@cs.wisc.edu

     Comment:    
						(c) 2001-2002 Stephen Chenney, University of Wisconsin at Madison
						Class header file for Edge class. Stores data about edges in the maze.
		

     Platform:    Visio Studio.Net 2003 (converted to 2005)

*************************************************************************/
#ifndef _EDGE_H_
#define _EDGE_H_

#include "Vertex.h"
#include "all_cpp_std.h"
#include "all_fl_gl.h"
#include "all_glm.h"

class Edge {

	public:
		// Constructor takes the index, pointers to the start and end
		// vertices, and r, g and b for the color.
		Edge(Vertex* s, Vertex* e);
		Edge(int, Vertex*, Vertex*, float, float, float);
		//Edge(int,glm::vec3, glm::vec3, glm::vec3, glm::vec3, float, float, float);
		Edge(const Edge& x) {
			*this = x;
		}
	public:
		// Add a cell to the neighbors of this edge. Valid values for
		// which_one are Edge::LEFT or Edge::RIGHT.
		void	Add_Cell(class Cell *cell, const char which_one){ 
			neighbors[which_one] = cell; 
		};

		// Given a cell, return the neighboring cell across this edge.
		// This is very useful.
		class Cell*	Neighbor(class Cell *cell) { 
			return cell == neighbors[LEFT] ? neighbors[RIGHT] : neighbors[LEFT]; 
		};
		Edge& operator=(const Edge& copy) {
			endpoints[0] = copy.endpoints[0];
			endpoints[1] = copy.endpoints[1];
			neighbors[0] = copy.neighbors[0];
			neighbors[1] = copy.neighbors[1];
			opaque = copy.opaque;
			color[0] = copy.color[0];
			color[1] = copy.color[1];
			color[2] = copy.color[2];
			index = copy.index;
			return *this;
		}
		// Returns which side of the edge a cell lies. Valid return values are
		// LEFT, RIGHT or NEITHER if the cell is not a neighbor of the edge.
		// The left side of the edge is the side that would be on the left
		// if you were standing at the START of the edge and looking along it,
		// with your head in the z direction.
		char	Cell_Side(class Cell*);

		// Returns which side of the edge the line (x,y) is on. The return value
		// is one of the constants defined above (LEFT, RIGHT, ON). See above
		// for a discussion of which side is left and which is right.
		char	Point_Side(float x, float y);
  public:
		// Constants.
		// To access these outside the edge class, use Edge::LEFT etc.
		static const char	LEFT;
		static const char	RIGHT;
		static const char	ON;
		static const char	NEITHER;

		static const char	START;
		static const char	END;

		int			index;				// An identifier

		Vertex		*endpoints[2]; 	// The vertices at each end, indexed by
    											// Edge::START and Edge::END

		class Cell		*neighbors[2];	// The cells that this edge lies
    											// between, indexed by Edge::LEFT and
												// Edge::RIGHT. One or the other may
												// be 0 if the edge is on the
												// boundary of the maze

    bool		opaque;	// True is this edge cannot be seen or
    						// walked through, false otherwise.

    float	color[3]; // The color for this edge / wall.

};



#endif

