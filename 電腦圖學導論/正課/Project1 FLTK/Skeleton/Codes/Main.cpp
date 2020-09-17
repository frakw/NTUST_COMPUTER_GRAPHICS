///////////////////////////////////////////////////////////////////////////////
//
//      Main.cpp                                Author:     Stephen Chenney
//                                              Modified:   Eric McDaniel
//                                              Date:       Spring 2002
//
//      Main function and soem helper functions of this app.  Handle command 
//  line arguments such as scripts.
//
//  You need to add your name in MakeNames.  Otherwise, you should not need
//  to modify this file.
//  
//
///////////////////////////////////////////////////////////////////////////////


#include <Fl/Fl.h>
#include <Fl/Fl_Window.h>
#include <string.h>
#include <iostream>
#include <vector>
#include "TargaImage.h"
#include "ImageWidget.h"
#include "ScriptHandler.h"


using namespace std;

// constants
const char      c_sNames[]          = "-names";             // display student names command line switch
const char      c_sHeadless[]       = "-headless";          // headless command line switch

// globals
std::vector<char*>  vsStudentNames;



///////////////////////////////////////////////////////////////////////////////
//
//      Write student names to standard out.
//
///////////////////////////////////////////////////////////////////////////////
void MakeNames()
{
    // ************ ADD YOUR NAME HERE ****************************************
    vsStudentNames.push_back((char*)"B10815057¹ù¸t°q");
}// MakeNames


///////////////////////////////////////////////////////////////////////////////
//
//      Write student names to standard out.
//
///////////////////////////////////////////////////////////////////////////////
void DisplayNames()
{
    for (std::vector<char*>::iterator i = vsStudentNames.begin(); i != vsStudentNames.end(); ++i)
        cout << *i << endl;
}// DisplayNames


///////////////////////////////////////////////////////////////////////////////
//
//      Argument processing callback. Does nothing at this point.
//
///////////////////////////////////////////////////////////////////////////////
static int Arg_Callback(int argc, char *argv[], int &i)
{
    return 0;
}// Arg_Callback


///////////////////////////////////////////////////////////////////////////////
//
//      Main function.  Handle command line arguments.  If running headless, 
//  handle the scripts given on the command line.  Otherwise create our window.
//
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
    int script_arg;

    // Do argument processing. At the end of this, script_arg contains
    // the first non-switch argument, which if not 0 or argc is the
    // location of the script file name in the argument list.
    Fl::args(argc, argv, script_arg, Arg_Callback);
    if ( script_arg == 0 )
    {
        cout <<  "Error: Unrecognised argument.\n";
	    return 1;
    }

    script_arg = 1;

    // fill in the names
    MakeNames();

    // check command line arguments
    TargaImage* pImage = NULL;
    bool bHeadless = false;

    for (int i = script_arg; i < argc; ++i)
    {
        if (!strcmp(argv[i], c_sNames))                                 // display names
            DisplayNames();
        else if (!bHeadless && !strcmp(argv[i], c_sHeadless))           // go headless
            bHeadless = true;
        else if (bHeadless && strcmp(argv[i], c_sHeadless))             // run script file
            CScriptHandler::HandleScriptFile(argv[i], pImage);
        else
        {
            cerr << "Usage:" << endl << "Project1 [-names] [-headless scriptFilenames . . .]" << endl;
            return 0;
        }// else
    }// for

    // print name reminder
//    if (vsStudentNames.empty())
//        cout << "This project has no author names.  Please add your" << endl
//             << "name in MakeNames in Main.cpp.  If you do not" << endl
//             << "comply, you will not be in compliance.  Have a nice day." << endl;

    // run the gui if we're not headless
    if (!bHeadless)
    {
        // using the gui so create our window and the image widget
        Fl_Window   window(350, 100, "CS559 Project 1");
        Fl::visual(FL_RGB);
        window.begin();
            ImageWidget* pWidget = new ImageWidget(0, 0, 350, 100, "Image");
            window.add(pWidget);
        window.end();

        window.show(argc, argv);

        return Fl::run();
    }// else

    return 0;
}// main


