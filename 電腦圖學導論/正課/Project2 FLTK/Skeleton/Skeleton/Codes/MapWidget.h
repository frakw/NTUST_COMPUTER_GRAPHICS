/************************************************************************
     File:        MapWidget.h

     Author:     
                  Stephen Chenney, schenney@cs.wisc.edu
     Modifier
                  Yu-Chi Lai, yu-chi@cs.wisc.edu

     Comment:    
						(c) 2001-2002 Stephen Chenney, University of Wisconsin at Madison

						Class header file for the MapWigdet class. A widget class for
						displaying a map of the maze.
		

     Platform:    Visio Studio.Net 2003 (converted to 2005)

*************************************************************************/
#ifndef _MAPWIDGET_H_
#define _MAPWIDGET_H_

#include <Fl/Fl.h>
#include <Fl/Fl_Widget.h>

#include "Maze.h"

class MapWidget : public Fl_Widget {
	public:
		// Constructor: Takes window position and size, the widget label,
		// and a maze (which can be NULL.)
		MapWidget(int x, int y, int w, int h, const char *label, Maze *m);

	public:
		// draw() method for the widget
		void	draw(void);

		// Draws the view frustum on the map
		void	Draw_Frustum(void);

		// Gets rid of the view frustum, and cleans up
		void	Undraw_Frustum(void);

		// Tells the widget which maze to display. This causes a redraw.
		void	Set_Maze(Maze*);

	public:
		Maze	*maze;	// The maze to display
};


#endif
