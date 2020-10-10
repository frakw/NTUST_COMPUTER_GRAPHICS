/************************************************************************
     File:        MapWidget.cpp

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

#include <stdio.h>
#include "MapWidget.h"
#include <Fl/fl_draw.h>
#include <Fl/Fl_Window.h>

//*****************************************************************************
//
// * Constructor
//=============================================================================
MapWidget::
MapWidget(int x, int y, int width, int height, const char *label, Maze *m)
	: Fl_Widget(x, y, width, height, label)
//=============================================================================
{
    maze = m;
}


//*****************************************************************************
//
// * Set the maze
//=============================================================================
void MapWidget::
Set_Maze(Maze *m)
//=============================================================================
{
	maze = m;

	redraw();
}


//*****************************************************************************
//
// * The draw() method. For efficiency, this should only be called when the
//   display has been damaged, or when the maze changes. It should not be
//   called if only the view frustum has changed.
//=============================================================================
void MapWidget::
draw(void)
//=============================================================================
{
	// Clear the background
	fl_color(FL_WHITE);
	fl_rectf(x(), y(), w(), h());

	if ( maze )  {
		// Push a clip region to make sure that we don't draw outside the
		// widget.
		fl_push_clip(x(), y(), w(), h());

		// The maze knows how to draw its own map
		maze->Draw_Map(x(), y(), x() + w() - 1, y() + h() - 1);

		// Draw the frustum in red
		fl_color(FL_RED);
		maze->Draw_Frustum(x(), y(), x() + w() - 1, y() + h() - 1);

		// Restore the original clip region
		fl_pop_clip();
	}
}


//*****************************************************************************
//
// * Draws the view frustum on the map
//=============================================================================
void MapWidget::
Draw_Frustum(void)
//=============================================================================
{
	if (maze && window()->visible())	{
		// Make sure we draw in the right place, since this method is
		// not generally called inside a draw() method.
		window()->make_current();

		// Make sure we only draw inside the widget.
		fl_push_clip(x(), y(), w(), h());

		// The frustum is drawn on in red.
		fl_color(FL_RED);

		// The maze knows how to draw its frustum if you give it the
		// location and dimension of the window to draw into.
		maze->Draw_Frustum(x(), y(), x() + w() - 1, y() + h() - 1);

		// Restore the clip region.
		fl_pop_clip();
	}
}


//*****************************************************************************
//
// * Gets rid of the view frustum, and cleans up
//=============================================================================
void MapWidget::
Undraw_Frustum(void)
//=============================================================================
{
	// See all the comments in Draw_Frustum above.
	if (maze && window()->visible()) {
		window()->make_current();
		fl_push_clip(x(), y(), w(), h());

		// To undraw, we draw in the background color (white).
		fl_color(FL_WHITE);

		maze->Draw_Frustum(x(), y(), x() + w() - 1, y() + h() - 1);

		// This method draws the edges of the view cell and its neighbors.
		// It is necessary because drawing the frustum in white may have
		// drawn over some edges. This redraws those without redrawing
		// everything.
		maze->Draw_Neighbors(x(), y(), x()+w()-1, y()+h()-1);

		fl_pop_clip();
	}
}
