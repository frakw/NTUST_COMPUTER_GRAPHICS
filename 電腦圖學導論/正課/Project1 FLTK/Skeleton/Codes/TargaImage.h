///////////////////////////////////////////////////////////////////////////////
//
//      TargaImage.h                            Author:     Stephen Chenney
//                                              Modified:   Eric McDaniel
//                                              Date:       Fall 2004
//
//      Class to manipulate targa images.  You must implement the image 
//  modification functions.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _TARGA_IMAGE_H_
#define _TARGA_IMAGE_H_

#include <Fl/Fl.h>
#include <Fl/Fl_Widget.h>
#include <stdio.h>

class Stroke;
class DistanceImage;

class TargaImage
{
    // methods
    public:
	    TargaImage(void);
            TargaImage(int w, int h);
	    TargaImage(int w, int h, unsigned char *d);
            TargaImage(const TargaImage& image);
	    ~TargaImage(void);

        unsigned char*	To_RGB(void);	            // Convert the image to RGB format,
        bool Save_Image(const char*);               // save the image to a file
        static TargaImage* Load_Image(char*);       // Load a file and return a pointer to a new TargaImage object.  Returns NULL on failure

        bool To_Grayscale();

        bool Quant_Uniform();
        bool Quant_Populosity();
        bool Quant_Median();

        bool Dither_Threshold();
        bool Dither_Random();
        bool Dither_FS();
        bool Dither_Bright();
        bool Dither_Cluster();
        bool Dither_Color();

        bool Comp_Over(TargaImage* pImage);
        bool Comp_In(TargaImage* pImage);
        bool Comp_Out(TargaImage* pImage);
        bool Comp_Atop(TargaImage* pImage);
        bool Comp_Xor(TargaImage* pImage);

        bool Difference(TargaImage* pImage);

        bool Filter_Box();
        bool Filter_Bartlett();
        bool Filter_Gaussian();
        bool Filter_Gaussian_N(unsigned int N);
        bool Filter_Edge();
        bool Filter_Enhance();

        bool NPR_Paint();

        bool Half_Size();
        bool Double_Size();
        bool Resize(float scale);
        bool Rotate(float angleDegrees);

    private:
	// helper function for format conversion
        void RGBA_To_RGB(unsigned char *rgba, unsigned char *rgb);

        // reverse the rows of the image, some targas are stored bottom to top
	TargaImage* Reverse_Rows(void);

	// clear image to all black
        void ClearToBlack();

	// Draws a filled circle according to the stroke data
        void Paint_Stroke(const Stroke& s);

    // members
    public:
        int		width;	    // width of the image in pixels
        int		height;	    // height of the image in pixels
        unsigned char	*data;	    // pixel data for the image, assumed to be in pre-multiplied RGBA format.

};

class Stroke { // Data structure for holding painterly strokes.
public:
   Stroke(void);
   Stroke(unsigned int radius, unsigned int x, unsigned int y,
          unsigned char r, unsigned char g, unsigned char b, unsigned char a);
   
   // data
   unsigned int radius, x, y;	// Location for the stroke
   unsigned char r, g, b, a;	// Color
};


#endif


