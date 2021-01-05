/************************************************************************
     File:        TrainView.H

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

#pragma once
#include <vector>
#include <tuple>
#include "RenderUtilities/BufferObject.h"
#include "RenderUtilities/Shader.h"
#include "RenderUtilities/Texture.h"

// Preclarify for preventing the compiler error
class TrainWindow;
class CTrack;


//#######################################################################
// TODO
// You might change the TrainView in order to add different objects to
// be drawn, or adjust the lighting, or ...
//#######################################################################


// The TrainView also needs to handle its user interface events.
// since this is an FLTk Window, we need to include FlTk Headers
// the pragmas turn off the warnings from FlTk - our code should have no warnings,
// but their code doesn't meet that standard
#pragma warning(push)
#pragma warning(disable:4312)
#pragma warning(disable:4311)
#include <Fl/Fl_Gl_Window.h>
#pragma warning(pop)


// this uses the old ArcBall Code
#include "Utilities/ArcBallCam.H"
using std::vector;
using std::tuple;
class Model;

struct Drop{
	Drop(glm::vec2 p,float t,float r,float k):point(p),time(t),radius(r),keep_time(k){}
	glm::vec2 point;
	float time;
	float radius;
	float keep_time;
};

class TrainView : public Fl_Gl_Window
{
	public:
		// note that we keep the "standard widget" constructor arguments
		TrainView(int x, int y, int w, int h, const char* l = 0);

		// overrides of important window things
		virtual int handle(int);
		virtual void draw();

		void dir_light(Shader*);
		void point_light(Shader*);
		void spot_light(Shader* ,glm::vec3);
		// all of the actual drawing happens in this routine
		// it has to be encapsulated, since we draw differently if
		// we're drawing shadows (no colors, for example)
		void drawStuff(bool doingShadows=false);

		// setup the projection - assuming that the projection stack has been
		// cleared for you
		void setProjection();

		// Reset the Arc ball control
		void resetArcball();

		// pick a point (for when the mouse goes down)
		void doPick();
		void add_drop(float,float);
		//set ubo
		void setUBO();
	public:
		ArcBallCam		arcball;			// keep an ArcBall for the UI
		int				selectedCube;  // simple - just remember which cube is selected

		TrainWindow*	tw;				// The parent of this display window
		CTrack*			m_pTrack;		// The track of the entire scene

		Shader* sinwave		= nullptr;
		Shader* height_map = nullptr;
		Shader* skybox = nullptr;
		Shader* tiles = nullptr;
		Shader* screen = nullptr;
		Shader* interactive_frame = nullptr;
		GLuint cubemapTexture;//skybox
		GLuint tiles_cubemapTexture;//tiles
		GLuint skyboxVAO, skyboxVBO;
		GLuint tilesVAO, tilesVBO;

		Texture2D* texture	= nullptr;
		VAO* plane			= nullptr;
		UBO* commom_matrices= nullptr;

		//OpenAL
		glm::vec3 source_pos;
		glm::vec3 listener_pos;


		Model* wave = nullptr;
		Texture2D* height_map_image = nullptr;
		int height_index = 0;

		unsigned int screen_framebuffer;
		unsigned int screen_textureColorbuffer;
		unsigned int screen_rbo;
		unsigned int screen_quadVAO, screen_quadVBO;

		unsigned int interactive_framebuffer;
		unsigned int interactive_textureColorbuffer;
		unsigned int interactive_rbo;
		unsigned int interactive_quadVAO, interactive_quadVBO;

		unsigned int tiles_tex = -1;
		//vector<glm::vec2> drop_point;
		//vector<float> drop_time;
		vector<Drop> all_drop;
};
unsigned int loadCubemap(vector<const GLchar*> faces);