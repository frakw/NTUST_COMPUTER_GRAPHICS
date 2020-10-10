/************************************************************************
     File:        MazeWindow.h

     Author:     
                  Stephen Chenney, schenney@cs.wisc.edu
     Modifier
                  Yu-Chi Lai, yu-chi@cs.wisc.edu

     Comment:    
						(c) 2001-2002 Stephen Chenney, University of Wisconsin at Madison

						Class header file for the MazeWindow class. The MazeWindow is
						the window in which the viewer's view of the maze is displayed.
		

     Platform:    Visio Studio.Net 2003 (converted to 2005)

*************************************************************************/

#ifndef _MAZEWINDOW_H_
#define _MAZEWINDOW_H_

#include <Fl/Fl.h>
#include <Fl/Fl_Gl_Window.h>

#include "Maze.h"

// Subclass the Fl_Gl_Window because we want to draw OpenGL in here.
class MazeWindow : public Fl_Gl_Window {
	public:
		// Constructor takes window position and dimensions, the title,
		// and a maze (which can be 0).
		MazeWindow(int x, int y, int w, int h, const char *label, Maze *m);

	public:
		// draw() method invoked whenever the view changes or the window
		// otherwise needs to be redrawn.
		void	draw(void);

		// Event handling method for FLTK
		int	handle(int);

		// Set the maze. Also causes a redraw.
		void	Set_Maze(Maze*);

		// Update the view according to any events that have happened since
		// the last time this method was called.
		bool	Update(float);

	private:
		bool	Drag(float);	// The function to call for mouse drag events

	public:
		Maze	*maze;	// The maze to work with

	private:
		bool  down;		// True if the mouse button is down, false otherwise.
		int	x_last;	// The location of the most recent mouse event
		int	y_last;
		int	x_down;	// The location of the mouse when the button was pushed
		int	y_down;
		int   z_key;	// Whether the key to move the viewer up or down was
							// pressed. A value of 1 means move up, a value of -1
							// means move down. 0 means do nothing.
		float   d_down;// The view direction when the mouse button was pushed

};


#endif

