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
#include <stdlib.h>
#include <math.h>
#include "Maze.h"
#include <Fl/Fl.h>
#include <Fl/fl_ask.h>
#include "MapWidget.h"
#include <Fl/Fl_Button.h>
#include <Fl/Fl_Counter.h>
#include <Fl/Fl_Window.h>

// Window system (W, H)
static const int    WINDOW_W	= 800;
static const int    WINDOW_H	= 800;

// COunter (H, W)
static const int    COUNTER_H = 50;
static const int    COUNTER_W = 150;
static const int    BUTTON_H	= 25;
static const int    GAP			= 5;

static MapWidget    *map_window;
static Fl_Counter   *nx_counter;
static Fl_Counter   *ny_counter;
static Fl_Counter   *sx_counter;
static Fl_Counter   *sy_counter;
static Fl_Counter   *vx_counter;
static Fl_Counter   *vy_counter;
static Fl_Counter   *vz_counter;
static Fl_Counter   *vd_counter;
static Fl_Counter   *vf_counter;
static Maze         *maze = NULL;


//****************************************************************************
//
// *
//============================================================================
static void Build_Callback(Fl_Widget *widget, void *data)
//============================================================================
{
	int	    nx = (int)floor(nx_counter->value());
	int	    ny = (int)floor(ny_counter->value());
	float   sx = (float)sx_counter->value();
	float   sy = (float)sy_counter->value();

	if ( maze )
		delete maze;
	maze = new Maze(nx, ny, sx, sy);

	try {
		maze->Set_View_Posn((float)vx_counter->value(),
								 (float)vy_counter->value(),
								 (float)vz_counter->value());
	} catch ( MazeException *e) {
		fl_alert("%s: POV %g %g %g\n", 
					e->Message(),
					(float)vx_counter->value(),
					(float)vy_counter->value(),
					(float)vz_counter->value());
		exit(1);
	}
	maze->Set_View_Dir((float)vd_counter->value());
	maze->Set_View_FOV((float)vf_counter->value());

	map_window->Set_Maze(maze);
}


//****************************************************************************
//
// *
//============================================================================
static void Save_Callback(Fl_Widget *widget, void *data)
//============================================================================
{
	const char	*filename;

	if ( ! maze )  {
		fl_alert("No maze to save.");
		return;
	}

	filename = fl_input("Enter the filename:", "maze-1x1");

	if ( ! filename )
		return;

	if ( ! maze->Save(filename) )
		fl_alert("Could not save the maze to %s", filename);
}


//****************************************************************************
//
// *
//============================================================================
void Load_Callback(Fl_Widget *widget, void *data)
//============================================================================
{
	const char	*filename;
	Maze	*new_maze;

	filename = fl_input("Enter the filename:", "maze.maze");

	if ( ! filename )
		return;

	try {
		new_maze = new Maze(filename);
	} catch ( MazeException *e) {
		fl_alert("%s: file %s\n", e->Message(), filename);
		new_maze = NULL;
	}

	if ( new_maze ) {
		if ( maze )
			delete maze;
		maze = new_maze;
		map_window->Set_Maze(maze);
	}
}


//****************************************************************************
//
// *
//============================================================================
static void Quit_Callback(Fl_Widget *widget, void *data)
//============================================================================
{
	exit(0);
}

//****************************************************************************
//
// *
//============================================================================
static void Build_Interface(Fl_Window &window)
//============================================================================
{
	Fl_Button	*go_button;
	Fl_Button	*save_button;
	Fl_Button	*load_button;
	Fl_Button	*quit_button;

	window.begin();

	nx_counter = new Fl_Counter(GAP, GAP,
										 COUNTER_W, COUNTER_H / 2, 
										 "Cells in X");
	nx_counter->value(10);
	nx_counter->step(1, 10);
	nx_counter->minimum(1);

	ny_counter = new Fl_Counter(GAP, COUNTER_H + 2 * GAP,
										 COUNTER_W, COUNTER_H / 2, 
										 "Cells in Y");
	ny_counter->value(10);
	ny_counter->step(1, 10);
	ny_counter->minimum(1);

	sx_counter = new Fl_Counter(GAP, 2 * COUNTER_H + 3 * GAP,
										 COUNTER_W, COUNTER_H / 2, 
										 "Cell X Size");
	sx_counter->value(5.0);
	sx_counter->step(0.2, 1);
	sx_counter->minimum(1);

	sy_counter = new Fl_Counter(GAP, 3 * COUNTER_H + 4 * GAP,
										 COUNTER_W, COUNTER_H / 2, 
										 "Cell Y Size");
	sy_counter->value(5.0);
	sy_counter->step(0.2, 1);
	sy_counter->minimum(1);

	vx_counter = new Fl_Counter(GAP, 4 * COUNTER_H + 5 * GAP,
										 COUNTER_W, COUNTER_H / 2, 
										 "Viewer X");
	vx_counter->value(2.5);
	vx_counter->step(0.1, 1);

	vy_counter = new Fl_Counter(GAP, 5 * COUNTER_H + 6 * GAP,
										 COUNTER_W, COUNTER_H / 2, 
										 "Viewer Y");
	vy_counter->value(2.5);
	vy_counter->step(0.1, 1);

	vz_counter = new Fl_Counter(GAP, 6 * COUNTER_H + 7 * GAP,
										 COUNTER_W, COUNTER_H / 2, 
										 "Viewer Z");
	vz_counter->value(0.0);
	vz_counter->step(0.1, 1);
	vz_counter->minimum(-0.8);
	vz_counter->maximum(0.8);

	vd_counter = new Fl_Counter(GAP, 7 * COUNTER_H + 8 * GAP,
										 COUNTER_W, COUNTER_H / 2, 
										 "Viewer Dir");
	vd_counter->value(0.0);
	vd_counter->step(1, 5);

	vf_counter = new Fl_Counter(GAP, 8 * COUNTER_H + 9 * GAP,
										 COUNTER_W, COUNTER_H / 2, 
										 "Viewer FOV");
	vf_counter->value(45.0);
	vf_counter->step(1.0, 5);
	vf_counter->minimum(1.0);
	vf_counter->maximum(179.0);

	go_button = new Fl_Button(GAP, 9 * COUNTER_H + 10 * GAP,
										COUNTER_W, BUTTON_H, "Build Maze");
	go_button->callback(Build_Callback, NULL);

	save_button = new Fl_Button(GAP, 9 * COUNTER_H + BUTTON_H + 11 * GAP,
										 COUNTER_W, BUTTON_H, "Save Maze");
	save_button->callback(Save_Callback, NULL);

	load_button = new Fl_Button(GAP, 9 * COUNTER_H + 2 * BUTTON_H + 12 * GAP,
										 COUNTER_W, BUTTON_H, "Load Maze");
	load_button->callback(Load_Callback, NULL);

	quit_button = new Fl_Button(GAP, 9 * COUNTER_H + 3 * BUTTON_H + 13 * GAP,
										COUNTER_W, BUTTON_H, "Quit");
	quit_button->callback(Quit_Callback, NULL);

	map_window = new MapWidget(COUNTER_W + 2 * GAP, GAP,
										WINDOW_W - COUNTER_W - 3 * GAP,
										WINDOW_H - 2 * GAP,
										"Maze Map", maze);

	window.end();
}


int main(int argc, char *argv[])
{
    Fl_Window	window(WINDOW_W, WINDOW_H, "Maze Builder");

    Fl::visual(FL_RGB);

    Build_Interface(window);

    window.show();
    return Fl::run();
}


