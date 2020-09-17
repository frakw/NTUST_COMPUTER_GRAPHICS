///////////////////////////////////////////////////////////////////////////////
//
//      ImageWidget.h                           Author:     Stephen Chenney
//                                              Modified:   Eric McDaniel
//                                              Date:       Spring 2002
//
//      Widget to display image and enter commands to modify image.  You do not
//  need to modify this file.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _IMAGE_WIDGET_H_
#define _IMAGE_WIDGET_H_

#include <Fl/Fl.h>
#include <Fl/Fl_Widget.h>

class Fl_Box;
class Fl_Input;
class TargaImage;

class ImageWidget : public Fl_Widget
{
    // methods
    public:
	    ImageWidget(int, int, int, int, const char*);
        ~ImageWidget();

	    void draw();	                    // FLTK draw function draws the current image.
	    TargaImage* Get_Image();            // get the current image
	    void Redraw();                      // redraw the image in the window


    private:
        static void CommandCallback(Fl_Widget* pWidget, void* pData);           // command entered callback


    // members
    private:
        TargaImage* m_pImage;	                // The image to display (current image).
        Fl_Box*     m_pStaticTextBox;           // static text
        Fl_Input*   m_pCommandInput;            // input box
};


#endif


