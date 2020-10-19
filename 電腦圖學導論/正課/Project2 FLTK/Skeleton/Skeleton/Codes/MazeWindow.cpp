///************************************************************************
//     File:        MazeWindow.cpp
//
//     Author:     
//                  Stephen Chenney, schenney@cs.wisc.edu
//     Modifier
//                  Yu-Chi Lai, yu-chi@cs.wisc.edu
//
//     Comment:    
//						(c) 2001-2002 Stephen Chenney, University of Wisconsin at Madison
//
//						Class header file for the MazeWindow class. The MazeWindow is
//						the window in which the viewer's view of the maze is displayed.
//		
//
//     Platform:    Visio Studio.Net 2003 (converted to 2005)
//
//*************************************************************************/
//#include "MazeWindow.h"
//#include <Fl/math.h>
//#include <Fl/gl.h>
//#include <GL/glu.h>
//#include <stdio.h>
//#include <array>
//#include <glm/glm.hpp>
//#include <glm/mat4x4.hpp>
//#include <glm/matrix.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtx/transform.hpp>
//#include<glm/vec4.hpp>
//#include<glm/vec3.hpp>
//#include<glm/geometric.hpp>
//#define square(x) ((x)*(x))
//
////*************************************************************************
////
//// * Constructor
////=========================================================================
//MazeWindow::
//MazeWindow(int x, int y, int width, int height, const char *label,Maze *m)
//	: Fl_Gl_Window(x, y, width, height, label)
////=========================================================================
//{
//	maze = m;
//	// The mouse button isn't down and there is no key pressed.
//	down = false;
//	z_key = 0;
//}
//
//
////*************************************************************************
////
//// * Set the maze. Also causes a redraw.
////=========================================================================
//void MazeWindow::
//Set_Maze(Maze *m)
////=========================================================================
//{
//	// Change the maze
//	maze = m;
//	maze->matrix = &matrix;
//	// Force a redraw
//	redraw();
//}
//void glhFrustumf2(float* matrix, float left, float right, float bottom, float top,
//	float znear, float zfar);
//void Perspectivef(glm::mat4x4& m,float fovyInDegrees, float aspectRatio,
//	float znear, float zfar)
//{
//	float matrix[16];
//	float ymax, xmax;
//	float temp, temp2, temp3, temp4;
//	ymax = znear * tanf(fovyInDegrees * M_PI / 360.0);
//	xmax = ymax * aspectRatio;
//	glhFrustumf2(matrix, -xmax, xmax, -ymax, ymax, znear, zfar);
//	m *= matrix;
//	//glMultMatrixf(matrix);//乘上最上面的矩陣並儲存 loadmatrix則是直接取代
//}
//void glhFrustumf2(float* matrix, float left, float right, float bottom, float top,
//	float znear, float zfar)
//{
//	float temp, temp2, temp3, temp4;
//	temp = 2.0 * znear;
//	temp2 = right - left;
//	temp3 = top - bottom;
//	temp4 = zfar - znear;
//	matrix[0] = temp / temp2;
//	matrix[1] = 0.0;
//	matrix[2] = 0.0;
//	matrix[3] = 0.0;
//	matrix[4] = 0.0;
//	matrix[5] = temp / temp3;
//	matrix[6] = 0.0;
//	matrix[7] = 0.0;
//	matrix[8] = (right + left) / temp2;
//	matrix[9] = (top + bottom) / temp3;
//	matrix[10] = (-zfar - znear) / temp4;
//	matrix[11] = -1.0;
//	matrix[12] = 0.0;
//	matrix[13] = 0.0;
//	matrix[14] = (-temp * zfar) / temp4;
//	matrix[15] = 0.0;
//}
//
//void NormalizeVector(float x[3]) {
//	float Norm = sqrt(square(x[0]) + square(x[1]) + square(x[2]));
//	x[0] /= Norm;
//	x[1] /= Norm;
//	x[2] /= Norm;
//}
//
//void ComputeNormalOfPlane(float result[3],float A[3],float B[3]) {
//	result[0] = A[1] * B[2] - A[2] * B[1];
//	result[1] = A[2] * B[0] - A[0] * B[2];
//	result[2] = A[0] * B[1] - A[1] * B[0];
//}
//
//void look_at(glm::mat4x4& matrix,float eyePosition3DX, float eyePosition3DY, float eyePosition3DZ,
//				 float center3DX,	   float center3DY,		 float center3DZ,
//				 float upVector3DX,	   float upVector3DY,	 float upVector3DZ)
//{
//	float forward[3], side[3], up[3];
//	float matrix2[16];
//	// --------------------
//	forward[0] = center3DX - eyePosition3DX;
//	forward[1] = center3DY - eyePosition3DY;
//	forward[2] = center3DZ - eyePosition3DZ;
//	NormalizeVector(forward);
//	// --------------------
//	// Side = forward x up    向量外積
//	float tmp[3] = { upVector3DX ,upVector3DY, upVector3DZ };
//	ComputeNormalOfPlane(side, forward, tmp);
//	NormalizeVector(side);
//	//--------------------
//	// Recompute up as: up = side x forward    向量外積
//	ComputeNormalOfPlane(up, side, forward);
//	// --------------------
//	matrix2[0] = side[0];
//	matrix2[4] = side[1];
//	matrix2[8] = side[2];
//	matrix2[12] = 0.0;
//	// --------------------
//	matrix2[1] = up[0];
//	matrix2[5] = up[1];
//	matrix2[9] = up[2];
//	matrix2[13] = 0.0;
//	// --------------------
//	matrix2[2] = -forward[0];
//	matrix2[6] = -forward[1];
//	matrix2[10] = -forward[2];
//	matrix2[14] = 0.0;
//	// --------------------
//	matrix2[3] = matrix2[7] = matrix2[11] = 0.0;
//	matrix2[15] = 1.0;
//	// --------------------
//	matrix *= matrix2;
//	//glMultMatrixf(matrix2);//乘上最上面的矩陣並儲存 loadmatrix則是直接取代
//	matrix = glm::translate(matrix, glm::vec3(-eyePosition3DX, -eyePosition3DY, -eyePosition3DZ));
//	//glTranslatef(-eyePosition3DX, -eyePosition3DY, -eyePosition3DZ);
//}
//
////*************************************************************************
////
//// * draw() method invoked whenever the view changes or the window
////   otherwise needs to be redrawn.
////=========================================================================
//void MazeWindow::
//draw(void)
////=========================================================================
//{
//	float   focal_length;
//
//	if ( ! valid() ) {
//		// The OpenGL context may have been changed
//		// Set up the viewport to fill the window.
//		glViewport(0, 0, w(), h());
//
//		// We are using orthogonal viewing for 2D. This puts 0,0 in the
//		// middle of the screen, and makes the image size in view space
//		// the same size as the window.
//		gluOrtho2D(-w() * 0.5, w() * 0.5, -h() * 0.5, h() * 0.5);
//
//		// Sets the clear color to black.
//		glClearColor(0.0, 0.0, 0.0, 1.0);
//	}
//
//	// Clear the screen.
//	glClear(GL_COLOR_BUFFER_BIT);
//	glBegin(GL_QUADS);
//		// Draw the "floor". It is an infinite plane perpendicular to
//		// vertical, so we know it projects to cover the entire bottom
//		// half of the screen. Walls of the maze will be drawn over the top
//		// of it.
//		glColor3f(0.2f, 0.2f, 0.2f);
//		glVertex2f(-w() * 0.5f, -h() * 0.5f);
//		glVertex2f( w() * 0.5f, -h() * 0.5f);
//		glVertex2f( w() * 0.5f, 0.0       );
//		glVertex2f(-w() * 0.5f, 0.0       );
//
//		// Draw the ceiling. It will project to the entire top half
//		// of the window.
//		glColor3f(0.4f, 0.4f, 0.4f);
//		glVertex2f( w() * 0.5f,  h() * 0.5f);
//		glVertex2f(-w() * 0.5f,  h() * 0.5f);
//		glVertex2f(-w() * 0.5f, 0.0       );
//		glVertex2f( w() * 0.5f, 0.0       );
//	glEnd();
//	
//	if ( maze ) {
//		glClear(GL_DEPTH_BUFFER_BIT);
//		float aspect = (float)w() / h();
//		Perspectivef(matrix,maze->viewer_fov, aspect, 0.01, 200);
//		float viewer_pos[3] = { maze->viewer_posn[Maze::Y], 0.0f, maze->viewer_posn[Maze::X] };
//		look_at(matrix,viewer_pos[Maze::X], viewer_pos[Maze::Y], viewer_pos[Maze::Z],
//			viewer_pos[Maze::X] + sin(Maze::To_Radians(maze->viewer_dir)),
//			viewer_pos[Maze::Y],
//			viewer_pos[Maze::Z] + cos(Maze::To_Radians(maze->viewer_dir)),
//			0.0, 1.0, 0.0);
//		maze->Draw_View(focal_length);
//	}
//	glLoadIdentity();//回歸單位矩陣，避免被上次影響
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//}
//
//
////*************************************************************************
////
//// *
////=========================================================================
//bool MazeWindow::
//Drag(float dt)
////=========================================================================
//{
//	float   x_move, y_move, z_move;
//
//	if ( down ) {
//		int	dx = x_down - x_last;
//		int   dy = y_down - y_last;
//		float dist;
//
//		// Set the viewing direction based on horizontal mouse motion.
//		maze->Set_View_Dir(d_down + 360.0f * dx / (float)w());
//
//		// Set the viewer's linear motion based on a speed (derived from
//		// vertical mouse motion), the elapsed time and the viewing direction.
//		dist = 10.0f * dt * dy / (float)h();
//		x_move = dist * (float)cos(Maze::To_Radians(maze->viewer_dir));
//		y_move = dist * (float)sin(Maze::To_Radians(maze->viewer_dir));
//	}
//	else {
//		x_move = 0.0;
//		y_move = 0.0;
//	}
//
//	// Update the z posn
//	z_move = z_key * 0.1f;
//	z_key = 0;
//
//	// Tell the maze how much the view has moved. It may restrict the motion
//	// if it tries to go through walls.
//	maze->Move_View_Posn(x_move, y_move, z_move);
//
//	return true;
//}
//
//
////*************************************************************************
////
//// *
////=========================================================================
//bool MazeWindow::
//Update(float dt)
////=========================================================================
//{
//	// Update the view
//
//	if ( down || z_key ) // Only do anything if the mouse button is down.
//		return Drag(dt);
//
//	// Nothing changed, so no need for a redraw.
//	return false;
//}
//
//
////*************************************************************************
////
//// *
////=========================================================================
//int MazeWindow::
//handle(int event)
////=========================================================================
//{
//	if (!maze)
//		return Fl_Gl_Window::handle(event);
//
//	// Event handling routine.
//	switch ( event ) {
//		case FL_PUSH:
//			down = true;
//			x_last = x_down = Fl::event_x();
//			y_last = y_down = Fl::event_y();
//			d_down = maze->viewer_dir;
//			return 1;
//		case FL_DRAG:
//			x_last = Fl::event_x();
//			y_last = Fl::event_y();
//			return 1;
//			case FL_RELEASE:
//			down = false;
//			return 1;
//		case FL_KEYBOARD:
//			/*
//			if ( Fl::event_key() == FL_Up )	{
//				z_key = 1;
//				return 1;
//			}
//			if ( Fl::event_key() == FL_Down ){
//				z_key = -1;
//				return 1;
//			}
//			*/
//			return Fl_Gl_Window::handle(event);
//		case FL_FOCUS:
//		case FL_UNFOCUS:
//			return 1;
//	}
//
//	// Pass any other event types on the superclass.
//	return Fl_Gl_Window::handle(event);
//}
//
//



/************************************************************************
	 File:        MazeWindow.cpp
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
#include "MazeWindow.h"
#include <Fl/math.h>
#include <Fl/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <array>

//#include <glm/glm.hpp>
//#include <glm/mat4x4.hpp>
//#include <glm/matrix.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtx/transform.hpp>
//#include<glm/vec4.hpp>
//#include<glm/vec3.hpp>
//#include<glm/geometric.hpp>

#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#define square(x) ((x)*(x))
//extern glm::mat4x4 mmm;
//*************************************************************************
//
// * Constructor
//=========================================================================
MazeWindow::
MazeWindow(int x, int y, int width, int height, const char* label, Maze* m)
	: Fl_Gl_Window(x, y, width, height, label)
	//=========================================================================
{
	maze = m;

	// The mouse button isn't down and there is no key pressed.
	down = false;
	z_key = 0;
}


//*************************************************************************
//
// * Set the maze. Also causes a redraw.
//=========================================================================
void MazeWindow::
Set_Maze(Maze* m)
//=========================================================================
{
	// Change the maze
	maze = m;

	// Force a redraw
	redraw();
}
void glhFrustumf2(float* matrix, float left, float right, float bottom, float top,
	float znear, float zfar);
// Matrix will receive the calculated perspective matrix.
// You would have to upload to your shader
// or use glLoadMatrixf if you aren't using shaders.
void glhPerspectivef2(float fovyInDegrees, float aspectRatio,
	float znear, float zfar)
{
	float matrix[16];
	float ymax, xmax;
	float temp, temp2, temp3, temp4;
	ymax = znear * tanf(fovyInDegrees * M_PI / 360.0);
	xmax = ymax * aspectRatio;
	glhFrustumf2(matrix, -xmax, xmax, -ymax, ymax, znear, zfar);
	glMultMatrixf(matrix);//乘上最上面的矩陣並儲存 loadmatrix則是直接取代
	//m *= matrix;
}
void glhFrustumf2(float* matrix, float left, float right, float bottom, float top,
	float znear, float zfar)
{
	float temp, temp2, temp3, temp4;
	temp = 2.0 * znear;
	temp2 = right - left;
	temp3 = top - bottom;
	temp4 = zfar - znear;
	matrix[0] = temp / temp2;
	matrix[1] = 0.0;
	matrix[2] = 0.0;
	matrix[3] = 0.0;
	matrix[4] = 0.0;
	matrix[5] = temp / temp3;
	matrix[6] = 0.0;
	matrix[7] = 0.0;
	matrix[8] = (right + left) / temp2;
	matrix[9] = (top + bottom) / temp3;
	matrix[10] = (-zfar - znear) / temp4;
	matrix[11] = -1.0;
	matrix[12] = 0.0;
	matrix[13] = 0.0;
	matrix[14] = (-temp * zfar) / temp4;
	matrix[15] = 0.0;
}

void NormalizeVector(float x[3]) {
	float Norm = sqrt(square(x[0]) + square(x[1]) + square(x[2]));
	x[0] /= Norm;
	x[1] /= Norm;
	x[2] /= Norm;
}

void ComputeNormalOfPlane(float result[3], float A[3], float B[3]) {
	result[0] = A[1] * B[2] - A[2] * B[1];
	result[1] = A[2] * B[0] - A[0] * B[2];
	result[2] = A[0] * B[1] - A[1] * B[0];
}

void glhLookAtf2(float eyePosition3DX, float eyePosition3DY, float eyePosition3DZ,
	float center3DX, float center3DY, float center3DZ,
	float upVector3DX, float upVector3DY, float upVector3DZ)
{
	float forward[3], side[3], up[3];
	float matrix2[16];
	
	// --------------------
	forward[0] = center3DX - eyePosition3DX;
	forward[1] = center3DY - eyePosition3DY;
	forward[2] = center3DZ - eyePosition3DZ;
	NormalizeVector(forward);
	// --------------------
	// Side = forward x up    向量外積
	float tmp[3] = { upVector3DX ,upVector3DY, upVector3DZ };
	ComputeNormalOfPlane(side, forward, tmp);
	NormalizeVector(side);
	//--------------------
	// Recompute up as: up = side x forward    向量外積
	ComputeNormalOfPlane(up, side, forward);
	// --------------------
	matrix2[0] = side[0];
	matrix2[4] = side[1];
	matrix2[8] = side[2];
	matrix2[12] = 0.0;
	// --------------------
	matrix2[1] = up[0];
	matrix2[5] = up[1];
	matrix2[9] = up[2];
	matrix2[13] = 0.0;
	// --------------------
	matrix2[2] = -forward[0];
	matrix2[6] = -forward[1];
	matrix2[10] = -forward[2];
	matrix2[14] = 0.0;
	// --------------------
	matrix2[3] = matrix2[7] = matrix2[11] = 0.0;
	matrix2[15] = 1.0;
	// --------------------
	//glm::mat4 matrix_t = glm::make_mat4(matrix);
	//mmm = mmm * matrix2;
	glMultMatrixf(matrix2);//乘上最上面的矩陣並儲存 loadmatrix則是直接取代
	//m = glm::translate(glm::mat4x4(), glm::vec3(-eyePosition3DX, -eyePosition3DY, -eyePosition3DZ));
	glTranslatef(-eyePosition3DX, -eyePosition3DY, -eyePosition3DZ);
}

//*************************************************************************
//
// * draw() method invoked whenever the view changes or the window
//   otherwise needs to be redrawn.
//=========================================================================
void MazeWindow::
draw(void)
//=========================================================================
{
	float   focal_length;

	if (!valid()) {
		// The OpenGL context may have been changed
		// Set up the viewport to fill the window.
		glViewport(0, 0, w(), h());

		// We are using orthogonal viewing for 2D. This puts 0,0 in the
		// middle of the screen, and makes the image size in view space
		// the same size as the window.
		gluOrtho2D(-w() * 0.5, w() * 0.5, -h() * 0.5, h() * 0.5);

		// Sets the clear color to black.
		glClearColor(0.0, 0.0, 0.0, 1.0);
	}

	// Clear the screen.
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_QUADS);
	// Draw the "floor". It is an infinite plane perpendicular to
	// vertical, so we know it projects to cover the entire bottom
	// half of the screen. Walls of the maze will be drawn over the top
	// of it.
	glColor3f(0.2f, 0.2f, 0.2f);
	glVertex2f(-w() * 0.5f, -h() * 0.5f);
	glVertex2f(w() * 0.5f, -h() * 0.5f);
	glVertex2f(w() * 0.5f, 0.0);
	glVertex2f(-w() * 0.5f, 0.0);

	// Draw the ceiling. It will project to the entire top half
	// of the window.
	glColor3f(0.4f, 0.4f, 0.4f);
	glVertex2f(w() * 0.5f, h() * 0.5f);
	glVertex2f(-w() * 0.5f, h() * 0.5f);
	glVertex2f(-w() * 0.5f, 0.0);
	glVertex2f(w() * 0.5f, 0.0);
	glEnd();

	if (maze) {
		glClear(GL_DEPTH_BUFFER_BIT);
		float aspect = (float)w() / h();
		glhPerspectivef2(maze->viewer_fov, aspect, 0.01, 200);
		float viewer_pos[3] = { maze->viewer_posn[Maze::Y], 0.0f, maze->viewer_posn[Maze::X] };
		glhLookAtf2(viewer_pos[Maze::X], viewer_pos[Maze::Y], viewer_pos[Maze::Z],
			viewer_pos[Maze::X] + sin(Maze::To_Radians(maze->viewer_dir)),
			viewer_pos[Maze::Y],
			viewer_pos[Maze::Z] + cos(Maze::To_Radians(maze->viewer_dir)),
			0.0, 1.0, 0.0);
		maze->Draw_View(focal_length);
	}
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();//回歸單位矩陣，避免被上次影響
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


//*************************************************************************
//
// *
//=========================================================================
bool MazeWindow::
Drag(float dt)
//=========================================================================
{
	float   x_move, y_move, z_move;

	if (down) {
		int	dx = x_down - x_last;
		int   dy = y_down - y_last;
		float dist;

		// Set the viewing direction based on horizontal mouse motion.
		maze->Set_View_Dir(d_down + 360.0f * dx / (float)w());

		// Set the viewer's linear motion based on a speed (derived from
		// vertical mouse motion), the elapsed time and the viewing direction.
		dist = 10.0f * dt * dy / (float)h();
		x_move = dist * (float)cos(Maze::To_Radians(maze->viewer_dir));
		y_move = dist * (float)sin(Maze::To_Radians(maze->viewer_dir));
	}
	else {
		x_move = 0.0;
		y_move = 0.0;
	}

	// Update the z posn
	z_move = z_key * 0.1f;
	z_key = 0;

	// Tell the maze how much the view has moved. It may restrict the motion
	// if it tries to go through walls.
	maze->Move_View_Posn(x_move, y_move, z_move);

	return true;
}


//*************************************************************************
//
// *
//=========================================================================
bool MazeWindow::
Update(float dt)
//=========================================================================
{
	// Update the view

	if (down || z_key) // Only do anything if the mouse button is down.
		return Drag(dt);

	// Nothing changed, so no need for a redraw.
	return false;
}


//*************************************************************************
//
// *
//=========================================================================
int MazeWindow::
handle(int event)
//=========================================================================
{
	if (!maze)
		return Fl_Gl_Window::handle(event);

	// Event handling routine.
	switch (event) {
	case FL_PUSH:
		down = true;
		x_last = x_down = Fl::event_x();
		y_last = y_down = Fl::event_y();
		d_down = maze->viewer_dir;
		return 1;
	case FL_DRAG:
		x_last = Fl::event_x();
		y_last = Fl::event_y();
		return 1;
	case FL_RELEASE:
		down = false;
		return 1;
	case FL_KEYBOARD:
		/*
		if ( Fl::event_key() == FL_Up )	{
			z_key = 1;
			return 1;
		}
		if ( Fl::event_key() == FL_Down ){
			z_key = -1;
			return 1;
		}
		*/
		return Fl_Gl_Window::handle(event);
	case FL_FOCUS:
	case FL_UNFOCUS:
		return 1;
	}

	// Pass any other event types on the superclass.
	return Fl_Gl_Window::handle(event);
}
