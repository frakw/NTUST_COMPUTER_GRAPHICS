/************************************************************************
     File:        TrainWindow.H

     Author:     
                  Michael Gleicher, gleicher@cs.wisc.edu

     Modifier
                  Yu-Chi Lai, yu-chi@cs.wisc.edu
     
     Comment:     
						this class defines the window in which the project 
						runs - its the outer windows that contain all of 
						the widgets, including the "TrainView" which has the 
						actual OpenGL window in which the train is drawn

						You might want to modify this class to add new widgets
						for controlling	your train

						This takes care of lots of things - including installing 
						itself into the FlTk "idle" loop so that we get periodic 
						updates (if we're running the train).


     Platform:    Visio Studio.Net 2003/2005

*************************************************************************/

#include <FL/fl.h>
#include <FL/Fl_Box.h>

// for using the real time clock
#include <time.h>

#include "TrainWindow.H"
#include "TrainView.H"
#include "CallBacks.H"



//************************************************************************
//
// * Constructor
//========================================================================
TrainWindow::
TrainWindow(const int x, const int y) 
	: Fl_Double_Window(x,y,800,800,"Project 4: Water Surface")
//========================================================================
{
	// make all of the widgets
	begin();	// add to this widget
	{
		int pty=5;			// where the last widgets were drawn

		trainView = new TrainView(5,5,590,790);
		trainView->tw = this;
		trainView->m_pTrack = &m_Track;
		this->resizable(trainView);

		// to make resizing work better, put all the widgets in a group
		widgets = new Fl_Group(600,5,190,790);
		widgets->begin();

		/*
		runButton = new Fl_Button(605,pty,60,20,"Run");
		togglify(runButton);

		Fl_Button* fb = new Fl_Button(700,pty,25,20,"@>>");
		fb->callback((Fl_Callback*)forwCB,this);
		Fl_Button* rb = new Fl_Button(670,pty,25,20,"@<<");
		rb->callback((Fl_Callback*)backCB,this);
		
		arcLength = new Fl_Button(730,pty,65,20,"ArcLength");
		togglify(arcLength,1);
  
		pty+=25;
		speed = new Fl_Value_Slider(655,pty,140,20,"speed");
		speed->range(0,10);
		speed->value(2);
		speed->align(FL_ALIGN_LEFT);
		speed->type(FL_HORIZONTAL);

		pty += 30;
		*/
		// camera buttons - in a radio button group
		Fl_Group* camGroup = new Fl_Group(600,pty,195,20);
		camGroup->begin();
		worldCam = new Fl_Button(605, pty, 90, 20, "World");
        worldCam->type(FL_RADIO_BUTTON);		// radio button
        worldCam->value(1);			// turned on
        worldCam->selection_color((Fl_Color)3); // yellow when pressed
		worldCam->callback((Fl_Callback*)damageCB,this);
		//trainCam = new Fl_Button(670, pty, 60, 20, "Train");
		//trainCam->type(FL_RADIO_BUTTON);
		//trainCam->value(0);
		//trainCam->selection_color((Fl_Color)3);
		//trainCam->callback((Fl_Callback*)damageCB,this);
		topCam = new Fl_Button(700, pty, 90, 20, "Top");
        topCam->type(FL_RADIO_BUTTON);
        topCam->value(0);
        topCam->selection_color((Fl_Color)3);
		topCam->callback((Fl_Callback*)damageCB,this);
		camGroup->end();

		pty += 30;
		/*
		// browser to select spline types
		// TODO: make sure these choices are the same as what the code supports
		splineBrowser = new Fl_Browser(605,pty,120,75,"Spline Type");
		splineBrowser->type(2);		// select
		splineBrowser->callback((Fl_Callback*)damageCB,this);
		splineBrowser->add("Linear");
		splineBrowser->add("Cardinal Cubic");
		splineBrowser->add("Cubic B-Spline");
		splineBrowser->select(2);

		pty += 110;

		// add and delete points
		Fl_Button* ap = new Fl_Button(605,pty,80,20,"Add Point");
		ap->callback((Fl_Callback*)addPointCB,this);
		Fl_Button* dp = new Fl_Button(690,pty,80,20,"Delete Point");
		dp->callback((Fl_Callback*)deletePointCB,this);

		pty += 25;
		// reset the points
		resetButton = new Fl_Button(735,pty,60,20,"Reset");
		resetButton->callback((Fl_Callback*)resetCB,this);
		Fl_Button* loadb = new Fl_Button(605,pty,60,20,"Load");
		loadb->callback((Fl_Callback*) loadCB, this);
		Fl_Button* saveb = new Fl_Button(670,pty,60,20,"Save");
		saveb->callback((Fl_Callback*) saveCB, this);

		pty += 25;
		// roll the points
		Fl_Button* rx = new Fl_Button(605,pty,30,20,"R+X");
		rx->callback((Fl_Callback*)rpxCB,this);
		Fl_Button* rxp = new Fl_Button(635,pty,30,20,"R-X");
		rxp->callback((Fl_Callback*)rmxCB,this);
		Fl_Button* rz = new Fl_Button(670,pty,30,20,"R+Z");
		rz->callback((Fl_Callback*)rpzCB,this);
		Fl_Button* rzp = new Fl_Button(700,pty,30,20,"R-Z");
		rzp->callback((Fl_Callback*)rmzCB,this);
		pty += 30;
		*/

		// browser to select spline types
		// TODO: make sure these choices are the same as what the code supports
		waveBrowser = new Fl_Browser(605, pty, 120, 50, "wave Type");
		waveBrowser->type(2);		// select
		waveBrowser->callback((Fl_Callback*)damageCB, this);
		waveBrowser->add("sin");
		waveBrowser->add("height map");
		waveBrowser->select(1);

		toon = new Fl_Button(735, pty, 60, 60, "toon");
		togglify(toon);
		pty += 70;
		tiles = new Fl_Button(605, pty, 90, 20, "tiles");
		togglify(tiles);
		rain = new Fl_Button(700, pty, 90, 20, "rain");
		togglify(rain);
		pty += 30;
		rain_frequency = new Fl_Value_Slider(660, pty, 140, 20, "rain freq");
		rain_frequency->range(0,20);
		rain_frequency->value(10);
		rain_frequency->align(FL_ALIGN_LEFT);
		rain_frequency->type(FL_HORIZONTAL);
		pty += 30;
		height_map_flat = new Fl_Button(605, pty, 180, 20, "height map flat");
		togglify(height_map_flat,1);
		pty += 30;
		dir_L = new Fl_Button(605, pty, 60, 20, "dir");
		togglify(dir_L);
		point_L = new Fl_Button(670, pty, 60, 20, "point");
		togglify(point_L);
		spot_L = new Fl_Button(735, pty, 60, 20, "spot");
		togglify(spot_L);

		pty += 30;
		y_axis = new Fl_Value_Slider(635, pty, 140, 20, "y axis");
		y_axis->range(-100, 100);
		y_axis->value(0);
		y_axis->align(FL_ALIGN_LEFT);
		y_axis->type(FL_HORIZONTAL);
		pty+=30;
		scale = new Fl_Value_Slider(635, pty, 140, 20, "scale");
		scale->range(0, 50);
		scale->value(25);
		scale->align(FL_ALIGN_LEFT);
		scale->type(FL_HORIZONTAL);
		pty += 50;
		amplitude = new Fl_Value_Slider(660, pty, 140, 20, "Amplitude");
		amplitude->range(0, 0.5);
		amplitude->value(0.15);
		amplitude->align(FL_ALIGN_LEFT);
		amplitude->type(FL_HORIZONTAL);
		pty += 30;
		wavelength = new Fl_Value_Slider(660, pty, 140, 20, "wave len");
		wavelength->range(0.1, 2);
		wavelength->value(1);
		wavelength->align(FL_ALIGN_LEFT);
		wavelength->type(FL_HORIZONTAL);
		pty += 30;
		wavespeed = new Fl_Value_Slider(660, pty, 140, 20, "speed");
		wavespeed->range(0, 2);
		wavespeed->value(1.0f);
		wavespeed->align(FL_ALIGN_LEFT);
		wavespeed->type(FL_HORIZONTAL);
		pty += 50;
		interactive_amplitude = new Fl_Value_Slider(660, pty, 140, 20, "interact\nAmplitude");
		interactive_amplitude->range(0, 0.5);
		interactive_amplitude->value(0.15);
		interactive_amplitude->align(FL_ALIGN_LEFT);
		interactive_amplitude->type(FL_HORIZONTAL);
		pty += 30;
		interactive_wavelength = new Fl_Value_Slider(660, pty, 140, 20, "interact\nwave len");
		interactive_wavelength->range(0.1, 1);
		interactive_wavelength->value(0.5);
		interactive_wavelength->align(FL_ALIGN_LEFT);
		interactive_wavelength->type(FL_HORIZONTAL);
		pty += 30;
		interactive_wavespeed = new Fl_Value_Slider(660, pty, 140, 20, "interact\nspeed");
		interactive_wavespeed->range(0.0, 20);
		interactive_wavespeed->value(10);
		interactive_wavespeed->align(FL_ALIGN_LEFT);
		interactive_wavespeed->type(FL_HORIZONTAL);
		pty += 50;
		Eta = new Fl_Value_Slider(660, pty, 140, 20, "Eta");
		Eta->range(0, 1);
		Eta->value(0.95);
		Eta->align(FL_ALIGN_LEFT);
		Eta->type(FL_HORIZONTAL);
		pty += 30;
		ratio_of_reflect_refract = new Fl_Value_Slider(700, pty, 100, 20, "reflect/refract");
		ratio_of_reflect_refract->range(0, 1);
		ratio_of_reflect_refract->value(0.5);
		ratio_of_reflect_refract->align(FL_ALIGN_LEFT);
		ratio_of_reflect_refract->type(FL_HORIZONTAL);
		pty += 30;
		frame_buffer_type = new Fl_Browser(605, pty, 120, 70, "frame buffer Type");
		frame_buffer_type->type(2);		// select
		frame_buffer_type->callback((Fl_Callback*)damageCB, this);
		frame_buffer_type->add("default");
		frame_buffer_type->add("pixel");
		frame_buffer_type->add("offset");
		frame_buffer_type->add("sobel");
		frame_buffer_type->select(1);
		// TODO: add widgets for all of your fancier features here
#ifdef EXAMPLE_SOLUTION
		makeExampleWidgets(this,pty);
#endif

		//// we need to make a little phantom widget to have things resize correctly
		//Fl_Box* resizebox = new Fl_Box(600,595,200,5);
		//widgets->resizable(resizebox);

		//widgets->end();
	}
	end();	// done adding to this widget

	// set up callback on idle
	animating = true;
	//Fl::add_idle((void (*)(void*))runButtonCB, this);
	Fl::add_idle((void (*)(void*))keeping_water,this);
}

//************************************************************************
//
// * handy utility to make a button into a toggle
//========================================================================
void TrainWindow::
togglify(Fl_Button* b, int val)
//========================================================================
{
	b->type(FL_TOGGLE_BUTTON);		// toggle
	b->value(val);		// turned off
	b->selection_color((Fl_Color)3); // yellow when pressed	
	b->callback((Fl_Callback*)damageCB,this);
}

//************************************************************************
//
// *
//========================================================================
void TrainWindow::
damageMe()
//========================================================================
{
	if (trainView->selectedCube >= ((int)m_Track.points.size()))
		trainView->selectedCube = 0;
	trainView->damage(1);
}

//************************************************************************
//
// * This will get called (approximately) 30 times per second
//   if the run button is pressed
//========================================================================
void TrainWindow::
advanceTrain(float dir)
//========================================================================
{
	//#####################################################################
	// TODO: make this work for your train
	//#####################################################################
#ifdef EXAMPLE_SOLUTION
	// note - we give a little bit more example code here than normal,
	// so you can see how this works

	if (arcLength->value()) {
		float vel = ew.physics->value() ? physicsSpeed(this) : dir * (float)speed->value();
		world.trainU += arclenVtoV(world.trainU, vel, this);
	} else {
		world.trainU +=  dir * ((float)speed->value() * .1f);
	}

	float nct = static_cast<float>(world.points.size());
	if (world.trainU > nct) world.trainU -= nct;
	if (world.trainU < 0) world.trainU += nct;
#endif
}