/************************************************************************
     File:        RunMaze.cpp

     Author:     
                  Stephen Chenney, schenney@cs.wisc.edu
     Modifier
                  Yu-Chi Lai, yu-chi@cs.wisc.edu

     Comment:    
						(c) 2001-2002 Stephen Chenney, University of Wisconsin at Madison

						Main program
		

     Platform:    Visio Studio.Net 2003 (converted to 2005)

*************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Maze.h"
#include <Fl/Fl.h>
#include <Fl/fl_ask.h>

#include "MapWidget.h"
#include "MazeWindow.h"

#include <Fl/Fl_Button.h>
#include <Fl/Fl_Counter.h>
#include <Fl/Fl_Window.h>

// Constants related to the interface
static const int    MAP_WINDOW_W = 400;
static const int    MAP_WINDOW_H = 400;
static const int    MAZE_WINDOW_W = 600;
static const int    MAZE_WINDOW_H = 600;
static const int    BUTTON_W = 100;
static const int    BUTTON_H = 25;
static const int    GAP = 5;

// The time per frame, in seconds (enforced only by timeouts.)
static const float  frame_time = 0.05f;

// Global variables
static MapWidget    *map_window;  // The window with the map of the maze in it.
static MazeWindow   *maze_window; // The window with the 3d view in it.
static Maze         *maze = NULL; // The maze.


//*****************************************************************************
//
// * This callback is called every 20th of a second if the system is fast
//   enough. It should be possible to get this speed even on slow systems
//   with a correctly implemented algorithm.
//=============================================================================
static void Timeout_Callback(void *data)
//=============================================================================
{
	// Undraw the frustum on the map. We do this so we can animate the
	// viewer's motion on the map without redrawing the whole map, which
	// is too slow for large mazes.
	if (map_window)
		map_window->Undraw_Frustum();

	// Update the viewer's motion. This returns true only if the viewer
	// has moved and hence the view needs redrawing.
	if (maze_window->Update(frame_time) )
		maze_window->redraw();

	// Draw the frustum back on.
	if (map_window)
		map_window->Draw_Frustum();

	// Do the timeout again for the next frame.
	Fl::repeat_timeout(frame_time, Timeout_Callback);
}

//*****************************************************************************
//
// * Load in precreated maze
//=============================================================================
static void Load_Callback(Fl_Widget *widget, void *data)
//=============================================================================
{
	const char	*filename;
	Maze	*new_maze;

	filename = fl_input("Enter the filename:", "Sources/maze-1x1");

	if ( ! filename )
		return;

	// We use C++ exception handling here because there is no other sensible
	// way to return an error message from the constructor.
	try  {
		new_maze = new Maze(filename);
	}
	catch ( MazeException *e)  {
		fl_alert("%s: file %s\n", e->Message(), filename);
		new_maze = NULL;
	}

	if ( new_maze )  {
		// We have a new maze
		// Delete the old one
		delete maze;
		maze = new_maze;

		// Change the maze in the map and the 3D view.
		map_window->Set_Maze(maze);
		maze_window->Set_Maze(maze);

		// If we weren't running timeouts, start them. This allows the
		// view to be animated.
		if ( ! Fl::has_timeout(Timeout_Callback, NULL) )
			Fl::add_timeout(0.0, Timeout_Callback, NULL);
		}
}


int main(int argc, char *argv[])
{
	Fl_Window	*window;
	Fl_Button	*load_button;

	Fl::visual(FL_RGB);

	// Create the window that displays the 3D view.
	maze_window = 
		new MazeWindow(MAP_WINDOW_W + 10, 0, MAZE_WINDOW_W,
							MAZE_WINDOW_H, "Maze View", NULL);
	maze_window->show();

	// Create the map window.
	window = new Fl_Window(0, 0, MAP_WINDOW_W, MAP_WINDOW_H, "Map View");
	window->begin();

	load_button = new Fl_Button(GAP, GAP, BUTTON_W, BUTTON_H, "Load Maze");
	load_button->callback(Load_Callback, NULL);

	// This puts a maze map drawing widget in the map window.
	map_window = new MapWidget(GAP, 2 * GAP + BUTTON_H,
										MAP_WINDOW_W - 2 * GAP,
										MAP_WINDOW_H - BUTTON_H - 3 * GAP,
										"Maze Map", maze);

	window->end();
	window->show();

	return Fl::run();
}
