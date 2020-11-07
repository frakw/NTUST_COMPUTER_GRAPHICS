/************************************************************************
     File:        TrainView.cpp

     Author:     
                  Michael Gleicher, gleicher@cs.wisc.edu

     Modifier
                  Yu-Chi Lai, yu-chi@cs.wisc.edu
     
     Comment:     
						The TrainView is the window that actually shows the 
						train. Its a
						GL display canvas (Fl_Gl_Window).  It is held within 
						a TrainWindow
						that is the outer window with all the widgets. 
						The TrainView needs 
						to be aware of the window - since it might need to 
						check the widgets to see how to draw

	  Note:        we need to have pointers to this, but maybe not know 
						about it (beware circular references)

     Platform:    Visio Studio.Net 2003/2005

*************************************************************************/

#include <iostream>
#include <tuple>
#include <utility>
#include <Fl/fl.h>
using std::tuple;
using std::make_tuple;
using std::get;
using std::pair;
using std::make_pair;
// we will need OpenGL, and OpenGL needs windows.h
#include <windows.h>
//#include "GL/gl.h"
//#include "../include/glad4.6/include/glad/glad.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "GL/glu.h"

#include "TrainView.H"
#include "TrainWindow.H"
#include "Utilities/3DUtils.H"


#ifdef EXAMPLE_SOLUTION
#	include "TrainExample/TrainExample.H"
#endif

Pnt3f GMT(const Pnt3f& p0, const Pnt3f& p1, const Pnt3f& p2, const Pnt3f& p3, int matrix_type, float t);
//************************************************************************
//
// * Constructor to set up the GL window
//========================================================================
TrainView::
TrainView(int x, int y, int w, int h, const char* l) 
	: Fl_Gl_Window(x,y,w,h,l)
//========================================================================
{
	mode( FL_RGB|FL_ALPHA|FL_DOUBLE | FL_STENCIL );

	resetArcball();
}

//************************************************************************
//
// * Reset the camera to look at the world
//========================================================================
void TrainView::
resetArcball()
//========================================================================
{
	// Set up the camera to look at the world
	// these parameters might seem magical, and they kindof are
	// a little trial and error goes a long way
	arcball.setup(this, 40, 250, .2f, .4f, 0);
}

//************************************************************************
//
// * FlTk Event handler for the window
//########################################################################
// TODO: 
//       if you want to make the train respond to other events 
//       (like key presses), you might want to hack this.
//########################################################################
//========================================================================
int TrainView::handle(int event)
{
	// see if the ArcBall will handle the event - if it does, 
	// then we're done
	// note: the arcball only gets the event if we're in world view
	if (tw->worldCam->value())
		if (arcball.handle(event)) 
			return 1;

	// remember what button was used
	static int last_push;

	switch(event) {
		// Mouse button being pushed event
		case FL_PUSH:
			last_push = Fl::event_button();
			// if the left button be pushed is left mouse button
			if (last_push == FL_LEFT_MOUSE  ) {
				doPick();
				damage(1);
				return 1;
			};
			break;

	   // Mouse button release event
		case FL_RELEASE: // button release
			damage(1);
			last_push = 0;
			return 1;

		// Mouse button drag event
		case FL_DRAG:

			// Compute the new control point position
			if ((last_push == FL_LEFT_MOUSE) && (selectedCube >= 0)) {
				ControlPoint* cp = &m_pTrack->points[selectedCube];

				double r1x, r1y, r1z, r2x, r2y, r2z;
				getMouseLine(r1x, r1y, r1z, r2x, r2y, r2z);

				double rx, ry, rz;
				mousePoleGo(r1x, r1y, r1z, r2x, r2y, r2z, 
								static_cast<double>(cp->pos.x), 
								static_cast<double>(cp->pos.y),
								static_cast<double>(cp->pos.z),
								rx, ry, rz,
								(Fl::event_state() & FL_CTRL) != 0);

				cp->pos.x = (float) rx;
				cp->pos.y = (float) ry;
				cp->pos.z = (float) rz;
				damage(1);
			}
			break;

		// in order to get keyboard events, we need to accept focus
		case FL_FOCUS:
			return 1;

		// every time the mouse enters this window, aggressively take focus
		case FL_ENTER:	
			focus(this);
			break;

		case FL_KEYBOARD:
		 		int k = Fl::event_key();
				int ks = Fl::event_state();
				if (k == 'p') {
					// Print out the selected control point information
					if (selectedCube >= 0) 
						printf("Selected(%d) (%g %g %g) (%g %g %g)\n",
								 selectedCube,
								 m_pTrack->points[selectedCube].pos.x,
								 m_pTrack->points[selectedCube].pos.y,
								 m_pTrack->points[selectedCube].pos.z,
								 m_pTrack->points[selectedCube].orient.x,
								 m_pTrack->points[selectedCube].orient.y,
								 m_pTrack->points[selectedCube].orient.z);
					else
						printf("Nothing Selected\n");

					return 1;
				};
				break;
	}

	return Fl_Gl_Window::handle(event);
}

//************************************************************************
//
// * this is the code that actually draws the window
//   it puts a lot of the work into other routines to simplify things
//========================================================================
void TrainView::draw()
{

	//*********************************************************************
	//
	// * Set up basic opengl informaiton
	//
	//**********************************************************************
	//initialized glad
	if (gladLoadGL())
	{
		//initiailize VAO, VBO, Shader...
	}
	else
		throw std::runtime_error("Could not initialize GLAD!");

	// Set up the view port
	glViewport(0,0,w(),h());

	// clear the window, be sure to clear the Z-Buffer too
	glClearColor(0,0,.3f,0);		// background should be blue

	// we need to clear out the stencil buffer since we'll use
	// it for shadows
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH);

	// Blayne prefers GL_DIFFUSE
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	// prepare for projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	setProjection();		// put the code to set up matrices here

	//######################################################################
	// TODO: 
	// you might want to set the lighting up differently. if you do, 
	// we need to set up the lights AFTER setting up the projection
	//######################################################################
	// enable the lighting
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// top view only needs one light
	if (tw->topCam->value()) {
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
	} else {
		glEnable(GL_LIGHT1);
		glEnable(GL_LIGHT2);
	}

	//*********************************************************************
	//
	// * set the light parameters
	//
	//**********************************************************************
	GLfloat lightPosition1[]	= {0,1,1,0}; // {50, 200.0, 50, 1.0};
	GLfloat lightPosition2[]	= {1, 0, 0, 0};
	GLfloat lightPosition3[]	= {0, -1, 0, 0};
	GLfloat yellowLight[]		= {0.5f, 0.5f, .1f, 1.0};
	GLfloat whiteLight[]			= {1.0f, 1.0f, 1.0f, 1.0};
	GLfloat blueLight[]			= {.1f,.1f,.3f,1.0};
	GLfloat grayLight[]			= {.3f, .3f, .3f, 1.0};

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, grayLight);

	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition2);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, yellowLight);

	glLightfv(GL_LIGHT2, GL_POSITION, lightPosition3);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, blueLight);



	//*********************************************************************
	// now draw the ground plane
	//*********************************************************************
	// set to opengl fixed pipeline(use opengl 1.x draw function)
	glUseProgram(0);

	setupFloor();
	glDisable(GL_LIGHTING);
	drawFloor(200,10);


	//*********************************************************************
	// now draw the object and we need to do it twice
	// once for real, and then once for shadows
	//*********************************************************************
	glEnable(GL_LIGHTING);
	setupObjects();

	drawStuff();

	// this time drawing is for shadows (except for top view)
	if (!tw->topCam->value()) {
		setupShadows();
		drawStuff(true);
		unsetupShadows();
	}
}

//************************************************************************
//
// * This sets up both the Projection and the ModelView matrices
//   HOWEVER: it doesn't clear the projection first (the caller handles
//   that) - its important for picking
//========================================================================
void TrainView::
setProjection()
//========================================================================
{
	// Compute the aspect ratio (we'll need it)
	float aspect = static_cast<float>(w()) / static_cast<float>(h());
	int cp_size = m_pTrack->points.size();
	// Check whether we use the world camp
	if (tw->worldCam->value())
		arcball.setProjection(false);
	// Or we use the top cam
	else if (tw->topCam->value()) {
		float wi, he;
		if (aspect >= 1) {
			wi = 110;
			he = wi / aspect;
		} 
		else {
			he = 110;
			wi = he * aspect;
		}

		// Set up the top camera drop mode to be orthogonal and set
		// up proper projection matrix
		glMatrixMode(GL_PROJECTION);
		glOrtho(-wi, wi, -he, he, 200, -200);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotatef(-90,1,0,0);
	} 
	// Or do the train view or other view here
	//####################################################################
	// TODO: 
	// put code for train view projection here!	
	//####################################################################
	else if(tw->trainCam->value()){
		glClear(GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60, aspect, 0.01f, 1000.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		ControlPoint& p0 = m_pTrack->points[(train_i - 1 + cp_size) % cp_size];
		ControlPoint& p1 = m_pTrack->points[train_i % cp_size];
		ControlPoint& p2 = m_pTrack->points[(train_i + 1) % cp_size];
		ControlPoint& p3 = m_pTrack->points[(train_i + 2) % cp_size];
		Pnt3f orient = GMT(p0.orient, p1.orient, p2.orient, p3.orient, tw->splineBrowser->value(), train_t);
		Pnt3f train_pos = GMT(p0.pos, p1.pos, p2.pos, p3.pos, tw->splineBrowser->value(), train_t);
		Pnt3f train_next = GMT(p0.pos, p1.pos, p2.pos, p3.pos, tw->splineBrowser->value(), train_t + 1.0f / DIVIDE_LINE);
		Pnt3f forward = train_next - train_pos;
		forward.normalize();
		Pnt3f cross = forward * orient;
		Pnt3f up = -1 * forward * cross;//不可用 forward *  forward * orient
		up.normalize();
		Pnt3f my_pos = train_pos + up * 5.0f;
		Pnt3f next_pos = train_next + up * 5.0f;
		gluLookAt(my_pos.x, my_pos.y, my_pos.z,
			next_pos.x, next_pos.y, next_pos.z, orient.x, orient.y, orient.z);

#ifdef EXAMPLE_SOLUTION
		trainCamView(this,aspect);
#endif
	}
}
void glvertex_vec(Pnt3f in) {
	glVertex3f(in.x,in.y,in.z);
}
Pnt3f GMT(const Pnt3f& p0, const Pnt3f& p1, const Pnt3f& p2, const Pnt3f& p3, int matrix_type, float t) {
	glm::mat4x4 M;
	switch (matrix_type)
	{
	case 1: {		
		M = { 0, 0, 0, 0,
			0, 0, -1, 1,
			0, 0, 1, 0,
			0, 0, 0, 0 
		};
	}break;
	case 2: {
		M = {
			-1,2,-1,0,
			3,-5,0,2,
			-3,4,1,0,
			1,-1,0,0
		};
		M /= 2.0f;
	}break;
	case 3: default: {
		M = {
			-1,3,-3,1,
			3,-6,0,4,
			-3,3,3,1,
			1,0,0,0
		};
		M /= 6.0f;
	}break;
	}
	M = glm::transpose(M);
	glm::mat4x4 G = {
		{p0.x, p0.y, p0.z, 1.0f},
		{p1.x, p1.y, p1.z, 1.0f},
		{p2.x, p2.y, p2.z, 1.0f},
		{p3.x, p3.y, p3.z, 1.0f} };
	glm::vec4 T = { t * t * t, t * t, t, 1.0f };
	glm::vec4 result = G * M * T;
	return Pnt3f(result[0],result[1],result[2]);
}
//************************************************************************
//
// * this draws all of the stuff in the world
//
//	NOTE: if you're drawing shadows, DO NOT set colors (otherwise, you get 
//       colored shadows). this gets called twice per draw 
//       -- once for the objects, once for the shadows
//########################################################################
// TODO: 
// if you have other objects in the world, make sure to draw them
//########################################################################
//========================================================================
void TrainView::drawStuff(bool doingShadows)
{
	// Draw the control points
	// don't draw the control points if you're driving 
	// (otherwise you get sea-sick as you drive through them)
	if (!tw->trainCam->value()) {
		for (size_t i = 0; i < m_pTrack->points.size(); ++i) {
			if (!doingShadows) {
				if (((int)i) != selectedCube)
					glColor3ub(240, 60, 60);
				else
					glColor3ub(240, 240, 30);
			}
			m_pTrack->points[i].draw();
		}
	}
	draw_track(doingShadows);
	if (!tw->trainCam->value()) {
		draw_train(doingShadows);
	}

#ifdef EXAMPLE_SOLUTION
	drawTrack(this, doingShadows);
#endif

	// draw the train
	//####################################################################
	// TODO: 
	//	call your own train drawing code
	//####################################################################
#ifdef EXAMPLE_SOLUTION
	// don't draw the train if you're looking out the front window
	if (!tw->trainCam->value())
		drawTrain(this, doingShadows);
#endif
}


void draw_sleeper(Pnt3f front,Pnt3f back,Pnt3f cross_t,Pnt3f up,bool doingShadows) {
	Pnt3f sleeper_up0 = front - up;
	Pnt3f sleeper_up1 = back - up;
	if (!doingShadows) glColor3ub(25, 25, 25);
	glLineWidth(1);
	glBegin(GL_LINES);
	glvertex_vec(front + cross_t);
	glvertex_vec(back + cross_t);

	glvertex_vec(back + cross_t);
	glvertex_vec(back - cross_t);

	glvertex_vec(back - cross_t);
	glvertex_vec(front - cross_t);

	glvertex_vec(front - cross_t);
	glvertex_vec(front + cross_t);

	//------------------------------
	glvertex_vec(front + cross_t);
	glvertex_vec(sleeper_up0 + cross_t);

	glvertex_vec(front - cross_t);
	glvertex_vec(sleeper_up0 - cross_t);

	glvertex_vec(back + cross_t);
	glvertex_vec(sleeper_up1 + cross_t);

	glvertex_vec(back - cross_t);
	glvertex_vec(sleeper_up1 - cross_t);

	//------------------------------
	glvertex_vec(sleeper_up0 + cross_t);
	glvertex_vec(sleeper_up1 + cross_t);

	glvertex_vec(sleeper_up1 + cross_t);
	glvertex_vec(sleeper_up1 - cross_t);

	glvertex_vec(sleeper_up1 - cross_t);
	glvertex_vec(sleeper_up0 - cross_t);

	glvertex_vec(sleeper_up0 - cross_t);
	glvertex_vec(sleeper_up0 + cross_t);
	glEnd();
	if (!doingShadows) glColor3ub(94, 38, 18);
	glBegin(GL_QUADS);
	glvertex_vec(front + cross_t);
	glvertex_vec(back + cross_t);
	glvertex_vec(back - cross_t);
	glvertex_vec(front - cross_t);

	glvertex_vec(sleeper_up0 + cross_t);
	glvertex_vec(sleeper_up1 + cross_t);
	glvertex_vec(sleeper_up1 - cross_t);
	glvertex_vec(sleeper_up0 - cross_t);

	glvertex_vec(sleeper_up0 - cross_t);
	glvertex_vec(front - cross_t);
	glvertex_vec(back - cross_t);
	glvertex_vec(sleeper_up1 - cross_t);

	glvertex_vec(sleeper_up0 + cross_t);
	glvertex_vec(front + cross_t);
	glvertex_vec(back + cross_t);
	glvertex_vec(sleeper_up1 + cross_t);

	glvertex_vec(front - cross_t);
	glvertex_vec(front + cross_t);
	glvertex_vec(sleeper_up0 + cross_t);
	glvertex_vec(sleeper_up0 - cross_t);

	glvertex_vec(back - cross_t);
	glvertex_vec(back + cross_t);
	glvertex_vec(sleeper_up1 + cross_t);
	glvertex_vec(sleeper_up1 - cross_t);
	glEnd();
}
void TrainView::draw_first_sleeper(bool doingShadows) {
	float percent = 1.0f / DIVIDE_LINE;
	Pnt3f prepreQ = GMT(m_pTrack->points[m_pTrack->points.size() - 2].pos, m_pTrack->points.back().pos, m_pTrack->points.front().pos, m_pTrack->points[1].pos, tw->splineBrowser->value(), 1.0f - percent);
	Pnt3f preQ = GMT(m_pTrack->points.back().pos, m_pTrack->points.front().pos, m_pTrack->points[1].pos, m_pTrack->points[2].pos, tw->splineBrowser->value(), 0.0f);
	Pnt3f firstO = GMT(m_pTrack->points.back().orient, m_pTrack->points.front().orient, m_pTrack->points[1].orient, m_pTrack->points[2].orient, tw->splineBrowser->value(), 0.0f);
	firstO.normalize();
	Pnt3f firstcross = (preQ - prepreQ) * firstO;
	firstcross.normalize();
	firstcross = firstcross * sleeper_width;
	Pnt3f firstbackward = (preQ - prepreQ);
	firstbackward.normalize();
	Pnt3f firstup = firstbackward * firstcross;
	firstup.normalize();
	Pnt3f first_pre_sleeper = preQ - firstbackward * sleeper_length;
	draw_sleeper(preQ, first_pre_sleeper, firstcross, firstup, doingShadows);
}
void TrainView::draw_track(bool doingShadows) {
	int cp_size = m_pTrack->points.size();
	arc_length = 0.0f;
	float T = 0.0f;
	float percent = 1.0f / DIVIDE_LINE;
	Pnt3f prepreQ = GMT(m_pTrack->points[cp_size - 2].pos, m_pTrack->points.back().pos, m_pTrack->points.front().pos, m_pTrack->points[1].pos, tw->splineBrowser->value(), 1.0f - percent);
	Pnt3f preQ = GMT(m_pTrack->points.back().pos, m_pTrack->points.front().pos, m_pTrack->points[1].pos, m_pTrack->points[2].pos, tw->splineBrowser->value(), 0.0f);
	draw_first_sleeper(doingShadows);//避免第一格空隙太大
	bool sleeper = false;
	bool get_train_pos = false;
	for (int i = 0;i < cp_size;i++) {
		ControlPoint& p0 = m_pTrack->points[(i - 1 + cp_size) % cp_size];
		ControlPoint& p1 = m_pTrack->points[i % cp_size];
		ControlPoint& p2 = m_pTrack->points[(i + 1) % cp_size];
		ControlPoint& p3 = m_pTrack->points[(i + 2) % cp_size];
		float t = percent;
		for (int j = 1;j < DIVIDE_LINE;j++) {
			Pnt3f Q = GMT(p0.pos, p1.pos, p2.pos, p3.pos, tw->splineBrowser->value(), t);
			Pnt3f O = GMT(p0.orient, p1.orient, p2.orient, p3.orient, tw->splineBrowser->value(), t);
			O.normalize();
			Pnt3f cross_t = (Q - preQ) * O;
			cross_t.normalize();
			Pnt3f backward = (Q - preQ);
			Pnt3f up = backward * cross_t;
			up.normalize();
			cross_t = cross_t * rail_width;
			glLineWidth(5);
			if (!doingShadows)glColor3ub(192, 192, 192);
			glBegin(GL_LINES);
			glvertex_vec(preQ + cross_t);
			glvertex_vec(Q + cross_t);
			glvertex_vec(preQ - cross_t);
			glvertex_vec(Q - cross_t);
			glvertex_vec(prepreQ - cross_t);//補掉外圈縫隙
			glvertex_vec(preQ - cross_t);//補掉外圈縫隙
			glEnd();

			arc_length += backward.length();
			if (!get_train_pos && arc_length > m_pTrack->trainU) {
				get_train_pos = true;
				train_i = i;
				train_t = t;
			}
			T += backward.length();
			if (sleeper && T >= sleeper_length) {
				backward.normalize();
				Pnt3f pre_sleeper = Q - backward * sleeper_length;
				draw_sleeper(Q, pre_sleeper, cross_t.normalize() * sleeper_width, up, doingShadows);
				T -= sleeper_length;
				sleeper = !sleeper;
			}
			else if (!sleeper && T >= space_length) {
				T -= space_length;
				sleeper = !sleeper;
			}
			prepreQ = preQ;
			preQ = Q;
			t += percent;
		}
	}
}
void TrainView::draw_train(bool doingShadows) {
	int cp_size = m_pTrack->points.size();
	ControlPoint& p0 = m_pTrack->points[(train_i - 1 + cp_size) % cp_size];
	ControlPoint& p1 = m_pTrack->points[train_i % cp_size];
	ControlPoint& p2 = m_pTrack->points[(train_i + 1) % cp_size];
	ControlPoint& p3 = m_pTrack->points[(train_i + 2) % cp_size];
	Pnt3f train_pos = GMT(p0.pos, p1.pos, p2.pos, p3.pos, tw->splineBrowser->value(), train_t);
	Pnt3f train_next = GMT(p0.pos, p1.pos, p2.pos, p3.pos, tw->splineBrowser->value(), train_t + 1.0f / DIVIDE_LINE);
	Pnt3f train_ori = GMT(p0.orient, p1.orient, p2.orient, p3.orient, tw->splineBrowser->value(), train_t);
	Pnt3f forward = train_next - train_pos;
	forward.normalize();//加這個才不會變彈跳床
	Pnt3f cross_t = forward * train_ori;
	cross_t.normalize();
	cross_t = cross_t * 10.0f;
	Pnt3f up = -1 * forward * cross_t;
	Pnt3f train_down_front = train_pos + forward * 10.0f;
	Pnt3f train_down_back = train_pos - forward * 10.0f;
	Pnt3f train_up_front = train_down_front + up * 2.0f;
	Pnt3f train_up_back = train_down_back + up * 2.0f;
	if (!doingShadows)glColor3ub(23, 122, 222);
	glBegin(GL_QUADS);//下
	glvertex_vec(train_down_front + cross_t);
	glvertex_vec(train_down_back + cross_t);
	glvertex_vec(train_down_back - cross_t);
	glvertex_vec(train_down_front - cross_t);
	glEnd();
	glBegin(GL_QUADS);//上
	glvertex_vec(train_up_front + cross_t);
	glvertex_vec(train_up_back + cross_t);
	glvertex_vec(train_up_back - cross_t);
	glvertex_vec(train_up_front - cross_t);
	glEnd();
	glBegin(GL_QUADS);//左
	glvertex_vec(train_down_front - cross_t);
	glvertex_vec(train_up_front - cross_t);
	glvertex_vec(train_up_back - cross_t);
	glvertex_vec(train_down_back - cross_t);
	glEnd();
	glBegin(GL_QUADS);//右
	glvertex_vec(train_down_front + cross_t);
	glvertex_vec(train_up_front + cross_t);
	glvertex_vec(train_up_back + cross_t);
	glvertex_vec(train_down_back + cross_t);
	glEnd();
	glBegin(GL_QUADS);//前
	glvertex_vec(train_down_front - cross_t);
	glvertex_vec(train_down_front + cross_t);
	glvertex_vec(train_up_front + cross_t);
	glvertex_vec(train_up_front - cross_t);
	glEnd();
	glBegin(GL_QUADS);//後
	glvertex_vec(train_down_back - cross_t);
	glvertex_vec(train_down_back + cross_t);
	glvertex_vec(train_up_back + cross_t);
	glvertex_vec(train_up_back - cross_t);
	glEnd();
}
// 
//************************************************************************
//
// * this tries to see which control point is under the mouse
//	  (for when the mouse is clicked)
//		it uses OpenGL picking - which is always a trick
//########################################################################
// TODO: 
//		if you want to pick things other than control points, or you
//		changed how control points are drawn, you might need to change this
//########################################################################
//========================================================================
void TrainView::
doPick()
//========================================================================
{
	// since we'll need to do some GL stuff so we make this window as 
	// active window
	make_current();		

	// where is the mouse?
	int mx = Fl::event_x(); 
	int my = Fl::event_y();

	// get the viewport - most reliable way to turn mouse coords into GL coords
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	// Set up the pick matrix on the stack - remember, FlTk is
	// upside down!
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity ();
	gluPickMatrix((double)mx, (double)(viewport[3]-my), 
						5, 5, viewport);

	// now set up the projection
	setProjection();

	// now draw the objects - but really only see what we hit
	GLuint buf[100];
	glSelectBuffer(100,buf);
	glRenderMode(GL_SELECT);
	glInitNames();
	glPushName(0);

	// draw the cubes, loading the names as we go
	for(size_t i=0; i<m_pTrack->points.size(); ++i) {
		glLoadName((GLuint) (i+1));
		m_pTrack->points[i].draw();
	}

	// go back to drawing mode, and see how picking did
	int hits = glRenderMode(GL_RENDER);
	if (hits) {
		// warning; this just grabs the first object hit - if there
		// are multiple objects, you really want to pick the closest
		// one - see the OpenGL manual 
		// remember: we load names that are one more than the index
		selectedCube = buf[3]-1;
	} else // nothing hit, nothing selected
		selectedCube = -1;

	printf("Selected Cube %d\n",selectedCube);
}


/*
void TrainView::draw_track(bool doingShadows) {
	float percent = 1.0f / DIVIDE_LINE;
	int cp_size = m_pTrack->points.size();//how much control point
	for (size_t i = 0; i < m_pTrack->points.size(); ++i) {
		float t = 0.0f;
		ControlPoint& p0 = m_pTrack->points[(i - 1 + cp_size) % cp_size];
		ControlPoint& p1 = m_pTrack->points[i % cp_size];
		ControlPoint& p2 = m_pTrack->points[(i + 1) % cp_size];
		ControlPoint& p3 = m_pTrack->points[(i + 2) % cp_size];
		for (size_t j = 0; j < DIVIDE_LINE; j++) {
			Pnt3f qt0 = GMT(p0.pos, p1.pos, p2.pos, p3.pos, tw->splineBrowser->value(), t);
			Pnt3f orient_t = GMT(p0.orient, p1.orient, p2.orient, p3.orient, tw->splineBrowser->value(), t);
			t += percent;
			Pnt3f qt1 = GMT(p0.pos, p1.pos, p2.pos, p3.pos, tw->splineBrowser->value(), t);
			orient_t.normalize();
			Pnt3f cross_t = (qt1 - qt0) * orient_t;
			cross_t.normalize();
			Pnt3f up = (qt1 - qt0) * cross_t;
			up.normalize();
			cross_t = cross_t * 2.5f;
			glLineWidth(5);
			glBegin(GL_LINES);
			if (!doingShadows)glColor3ub(192, 192, 192);
			glvertex_vec(qt0 + cross_t);
			glvertex_vec(qt1 + cross_t);
			glvertex_vec(qt0 - cross_t);
			glvertex_vec(qt1 - cross_t);
			glEnd();
			if (j % 2) {
				Pnt3f sleeper_up0 = qt0 - up;
				Pnt3f sleeper_up1 = qt1 - up;
				cross_t = cross_t * 2.0f;
				if (!doingShadows)
					glColor3ub(94, 38, 18);
				glBegin(GL_QUADS);
				glvertex_vec(qt0 + cross_t);
				glvertex_vec(qt1 + cross_t);
				glvertex_vec(qt1 - cross_t);
				glvertex_vec(qt0 - cross_t);
				glEnd();
				glBegin(GL_QUADS);
				glvertex_vec(sleeper_up0 + cross_t);
				glvertex_vec(sleeper_up1 + cross_t);
				glvertex_vec(sleeper_up1 - cross_t);
				glvertex_vec(sleeper_up0 - cross_t);
				glEnd();
				glBegin(GL_QUADS);
				glvertex_vec(sleeper_up0 - cross_t);
				glvertex_vec(qt0 - cross_t);
				glvertex_vec(qt1 - cross_t);
				glvertex_vec(sleeper_up1 - cross_t);
				glEnd();
				glBegin(GL_QUADS);
				glvertex_vec(sleeper_up0 + cross_t);
				glvertex_vec(qt0 + cross_t);
				glvertex_vec(qt1 + cross_t);
				glvertex_vec(sleeper_up1 + cross_t);
				glEnd();
				glBegin(GL_QUADS);
				glvertex_vec(qt0 - cross_t);
				glvertex_vec(qt0 + cross_t);
				glvertex_vec(sleeper_up0 + cross_t);
				glvertex_vec(sleeper_up0 - cross_t);
				glEnd();
				glBegin(GL_QUADS);
				glvertex_vec(qt1 - cross_t);
				glvertex_vec(qt1 + cross_t);
				glvertex_vec(sleeper_up1 + cross_t);
				glvertex_vec(sleeper_up1 - cross_t);
				glEnd();
			}
		}
	}
}
*/