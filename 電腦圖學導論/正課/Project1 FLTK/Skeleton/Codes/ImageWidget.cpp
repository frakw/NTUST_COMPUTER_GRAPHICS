///////////////////////////////////////////////////////////////////////////////
//
//      ImageWidget.cpp                         Author:     Stephen Chenney
//                                              Modified:   Eric McDaniel
//                                              Date:       Spring 2002
//
//      Implementation of ImageWidget methods.  You do not need to modify this
//  file.
//
///////////////////////////////////////////////////////////////////////////////

#include "Globals.h"
#include "ImageWidget.h"
#include <Fl/Fl_Window.h>
#include <Fl/Fl_Input.h>
#include <Fl/Fl_Box.h>
#include <Fl/fl_draw.h>
#include "libtarga.h"
#include <string.h>
#include "TargaImage.h"
#include "ScriptHandler.h"

// constants
const int   c_border                = 10;                                       // border width between window elements in pixels
const int   c_buttonHeight          = 30;                                       // button height in pixels
const int   c_commandInputBoxWidth  = 200;                                      // input box width
const int   c_commandTextWidth      = 120;                                      // static text width
const int   c_buttonPaneHeight      = 2 * c_border + c_buttonHeight;            // height of pane for buttons in pixels
const int   c_minWindowWidth        = 350;                                      // minimum window width in pixels
const int   c_minWindowHeight       = 100;                                      // minimum windoe height in pixels


///////////////////////////////////////////////////////////////////////////////
//
//      Constructor.  Add the buttons to the window.
//
///////////////////////////////////////////////////////////////////////////////
ImageWidget::ImageWidget(int x, int y, int w, int h, const char *title) : Fl_Widget(x, y, Max(w, c_minWindowWidth), Max(h, c_minWindowHeight), title), m_pImage(NULL)
{
    // add controls-
    int horizontalCenter = Max(w, c_minWindowWidth) / 2;
    int halfControlWidth = (c_commandInputBoxWidth + c_commandTextWidth + c_border) / 2;
    int verticalButtonPos = c_border;

    // add label
    m_pStaticTextBox = new Fl_Box(horizontalCenter - halfControlWidth, verticalButtonPos, c_commandTextWidth, c_buttonHeight, "Enter Command:");

    // add input box
    m_pCommandInput = new Fl_Input(horizontalCenter - halfControlWidth + c_commandTextWidth, verticalButtonPos, c_commandInputBoxWidth, c_buttonHeight, "");
    m_pCommandInput->callback(CommandCallback, this);
    m_pCommandInput->when(FL_WHEN_ENTER_KEY|FL_WHEN_NOT_CHANGED);
}// ImageWidget


///////////////////////////////////////////////////////////////////////////////
//
//      Destructor.  Free targa imaeg object.
//
///////////////////////////////////////////////////////////////////////////////
ImageWidget::~ImageWidget()
{
    delete m_pImage;
}// ~ImageWidget


///////////////////////////////////////////////////////////////////////////////
//
//      Get the current image.
//
///////////////////////////////////////////////////////////////////////////////
TargaImage* ImageWidget::Get_Image()
{
    return m_pImage;
}// Get_Image


///////////////////////////////////////////////////////////////////////////////
//
//      Draw the window contents.
//
///////////////////////////////////////////////////////////////////////////////
void ImageWidget::draw()
{
    if (!m_pImage)          // Don't do anything if the image is empty.
    	return;
    
    unsigned char   *rgb;
    rgb = m_pImage->To_RGB(); // Convert the pre-multiplied RGBA image into RGB.
    unsigned int imageX = x() + (w() > m_pImage->width) ? (w() - m_pImage->width) / 2 : 0;
    fl_draw_image(rgb, imageX, y() + c_border * 2 + c_buttonHeight, m_pImage->width, m_pImage->height, 3);
    delete[] rgb;
}// draw


///////////////////////////////////////////////////////////////////////////////
//
//      Redraw the window.
//
///////////////////////////////////////////////////////////////////////////////
void ImageWidget::Redraw()
{
    if (m_pImage)
		parent()->size(Max(m_pImage->width, c_minWindowWidth), Max(m_pImage->height + c_buttonPaneHeight, c_minWindowHeight));
    else
        parent()->size(c_minWindowWidth, c_minWindowHeight);

    parent()->redraw();
}// Redraw


///////////////////////////////////////////////////////////////////////////////
//
//      Handle commands entered in input box.
//
///////////////////////////////////////////////////////////////////////////////
void ImageWidget::CommandCallback(Fl_Widget* pWidget, void* pData)
{
    ImageWidget* pImageWidget = static_cast<ImageWidget*>(pData);
    CScriptHandler::HandleCommand(static_cast<Fl_Input*>(pWidget)->value(), pImageWidget->m_pImage);
    pImageWidget->Redraw();
}// CommandCallback


