///////////////////////////////////////////////////////////////////////////////
//
//      TargaImage.cpp                          Author:     Stephen Chenney
//                                              Modified:   Eric McDaniel
//                                              Date:       Fall 2004
//
//      Implementation of TargaImage methods.  You must implement the image
//  modification functions.
//
///////////////////////////////////////////////////////////////////////////////
#define _USE_MATH_DEFINES //要放在iostream與cmath前面
#include "Globals.h"
#include "TargaImage.h"
#include "libtarga.h"
#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include <math.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <array>
#include <limits.h>
#include <cstdlib>
#include <time.h>
#include <cmath>
#define sqare(x) ((x)*(x))
#define avoid_overflow(x) ((x) > 255 ? 255:((x) < 0 ? 0: (x)))
using namespace std;

// constants
const int           RED             = 0;                // red channel
const int           GREEN           = 1;                // green channel
const int           BLUE            = 2;                // blue channel
const int           ALPHA           = 3;                // alpha channel
const unsigned char BACKGROUND[3]   = { 0, 0, 0 };      // background color


// Computes n choose s, efficiently
double Binomial(int n, int s)
{
    double        res;

    res = 1;
    for (int i = 1 ; i <= s ; i++)
        res = (n - i + 1) * res / i ;

    return res;
}// Binomial


///////////////////////////////////////////////////////////////////////////////
//
//      Constructor.  Initialize member variables.
//
///////////////////////////////////////////////////////////////////////////////
TargaImage::TargaImage() : width(0), height(0), data(NULL)
{}// TargaImage

///////////////////////////////////////////////////////////////////////////////
//
//      Constructor.  Initialize member variables.
//
///////////////////////////////////////////////////////////////////////////////
TargaImage::TargaImage(int w, int h) : width(w), height(h)
{
   data = new unsigned char[width * height * 4];
   ClearToBlack();
}// TargaImage



///////////////////////////////////////////////////////////////////////////////
//
//      Constructor.  Initialize member variables to values given.
//
///////////////////////////////////////////////////////////////////////////////
TargaImage::TargaImage(int w, int h, unsigned char *d)
{
    int i;

    width = w;
    height = h;
    data = new unsigned char[width * height * 4];

    for (i = 0; i < width * height * 4; i++)
	    data[i] = d[i];
}// TargaImage

///////////////////////////////////////////////////////////////////////////////
//
//      Copy Constructor.  Initialize member to that of input
//
///////////////////////////////////////////////////////////////////////////////
TargaImage::TargaImage(const TargaImage& image) 
{
   width = image.width;
   height = image.height;
   data = NULL; 
   if (image.data != NULL) {
      data = new unsigned char[width * height * 4];
      memcpy(data, image.data, sizeof(unsigned char) * width * height * 4);
   }
}


///////////////////////////////////////////////////////////////////////////////
//
//      Destructor.  Free image memory.
//
///////////////////////////////////////////////////////////////////////////////
TargaImage::~TargaImage()
{
    if (data)
        delete[] data;
}// ~TargaImage


///////////////////////////////////////////////////////////////////////////////
//
//      Converts an image to RGB form, and returns the rgb pixel data - 24 
//  bits per pixel. The returned space should be deleted when no longer 
//  required.
//
///////////////////////////////////////////////////////////////////////////////
unsigned char* TargaImage::To_RGB(void)
{
    unsigned char   *rgb = new unsigned char[width * height * 3];
    int		    i, j;

    if (! data)
	    return NULL;

    // Divide out the alpha
    for (i = 0 ; i < height ; i++)
    {
	    int in_offset = i * width * 4;
	    int out_offset = i * width * 3;

	    for (j = 0 ; j < width ; j++)
        {
	        RGBA_To_RGB(data + (in_offset + j*4), rgb + (out_offset + j*3));
	    }
    }

    return rgb;
}// TargaImage


///////////////////////////////////////////////////////////////////////////////
//
//      Save the image to a targa file. Returns 1 on success, 0 on failure.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Save_Image(const char *filename)
{
    TargaImage	*out_image = Reverse_Rows();

    if (! out_image)
	    return false;

    if (!tga_write_raw(filename, width, height, out_image->data, TGA_TRUECOLOR_32))
    {
	    cout << "TGA Save Error: %s\n", tga_error_string(tga_get_last_error());
	    return false;
    }

    delete out_image;

    return true;
}// Save_Image


///////////////////////////////////////////////////////////////////////////////
//
//      Load a targa image from a file.  Return a new TargaImage object which 
//  must be deleted by caller.  Return NULL on failure.
//
///////////////////////////////////////////////////////////////////////////////
TargaImage* TargaImage::Load_Image(char *filename)
{
    unsigned char   *temp_data;
    TargaImage	    *temp_image;
    TargaImage	    *result;
    int		        width, height;

    if (!filename)
    {
        cout << "No filename given." << endl;
        return NULL;
    }// if

    temp_data = (unsigned char*)tga_load(filename, &width, &height, TGA_TRUECOLOR_32);
    if (!temp_data)
    {
        cout << "TGA Error: %s\n", tga_error_string(tga_get_last_error());
	    width = height = 0;
	    return NULL;
    }
    temp_image = new TargaImage(width, height, temp_data);
    free(temp_data);

    result = temp_image->Reverse_Rows();

    delete temp_image;

    return result;
}// Load_Image

//#define index_of_pixel(x,y,color) (((y) * width + (x)) * 4 + (color))
int TargaImage::index_of_pixel(int x, int y, int color) {
    return (y * width + x) * 4 + color;
}
unsigned char TargaImage::to_gray(int x, int y) {
    return  0.299 * (double)data[index_of_pixel(x, y, RED)] + 0.587 * (double)data[index_of_pixel(x, y, GREEN)] + 0.114 * (double)data[index_of_pixel(x, y, BLUE)];
}
bool TargaImage::legal_index(int x, int y) {
    return x > 0 && x < width && y> 0 && y < height;
}
void TargaImage::replace_rgb(int x,int y,int val) {
    for (int k = 0;k < 3;k++) {
        data[index_of_pixel(x, y, k)] = val;
    }
}
///////////////////////////////////////////////////////////////////////////////
//
//      Convert image to grayscale.  Red, green, and blue channels should all 
//  contain grayscale value.  Alpha channel shoould be left unchanged.  Return
//  success of operation.
//
///////////////////////////////////////////////////////////////////////////////


bool TargaImage::To_Grayscale()
{
    //ClearToBlack();
    for (int i = 0;i < height;i++) {
        for (int j = 0;j < width;j++) {
            unsigned char g = to_gray(j,i);
            for (int k = 0;k < 3;k++) {
                data[index_of_pixel(j, i, k)] = g;
            }
        }
    }
    return true;
}// To_Grayscale

int Quant(int val,int level) {
    return val - val % level;
}
///////////////////////////////////////////////////////////////////////////////
//
//  Convert the image to an 8 bit image using uniform quantization.  Return 
//  success of operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Quant_Uniform()
{
    //ClearToBlack();
    for (int i = 0;i < height;i++) {
        for (int j = 0;j < width;j++) {
            data[index_of_pixel(j, i, RED)] -= data[index_of_pixel(j, i, RED)] % 32;
            data[index_of_pixel(j, i, GREEN)] -= data[index_of_pixel(j, i, GREEN)] % 32;
            data[index_of_pixel(j, i, BLUE)] -= data[index_of_pixel(j, i, BLUE)] % 64;
        }
    }
    return true;
}// Quant_Uniform


///////////////////////////////////////////////////////////////////////////////
//
//      Convert the image to an 8 bit image using populosity quantization.  
//  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Quant_Populosity()//38秒
{
    //ClearToBlack();
    unsigned int color_count[32][32][32] = { 0 };
    for (int i = 0;i < height;i++) {
        for (int j = 0;j < width;j++) {
            data[index_of_pixel(j, i, RED)] -= data[index_of_pixel(j, i, RED)] % 8;
            data[index_of_pixel(j, i, GREEN)]-=data[index_of_pixel(j, i, GREEN)] % 8;
            data[index_of_pixel(j, i, BLUE)] -= data[index_of_pixel(j, i, BLUE)] % 8;
            ++color_count[data[index_of_pixel(j, i, RED)]/8][data[index_of_pixel(j, i, GREEN)]/8][data[index_of_pixel(j, i, BLUE)]/8];
        }
    }
    array<tuple<int,int,int,int>,256 > popular;
    popular.fill(make_tuple(0, 0, 0, 0));
    for (int i = 0;i < 32;i++) {
        for (int j = 0;j < 32;j++) {
            for (int k = 0;k < 32;k++) {
                int g = 0;
                for (;;) {
                    if (color_count[i][j][k] < get<0>(popular[g])) {
                        --g;
                        break;
                    }
                    if (g == 255) break;
                    else ++g;
                }
                if (g == -1) continue;
                for (int h = 0;h < g;h++) {
                    popular[h] = popular[h + 1];
                }
                popular[g] = make_tuple(color_count[i][j][k], i*8, j*8, k*8);                
            }
        }
    }
    for (int i = 0;i < height;i++) {
        for (int j = 0;j < width;j++) {
            tuple<int, int, int, int> change;
            int min_dis = INT_MAX;
            for (int g = 0;g < 256;g++) {
                int tmp = sqrt(
                    sqare(get<1>(popular[g]) - data[index_of_pixel(j, i, RED)]) +
                    sqare(get<2>(popular[g]) - data[index_of_pixel(j, i, GREEN)]) +
                    sqare(get<3>(popular[g]) - data[index_of_pixel(j, i, BLUE)]));
                if (tmp < min_dis)
                {
                    min_dis = tmp;
                    change = popular[g];
                }
            }
            data[index_of_pixel(j, i, RED)] = get<1>(change);
            data[index_of_pixel(j, i, GREEN)] = get<2>(change);
            data[index_of_pixel(j, i, BLUE)] = get<3>(change);
        }
    }
    return true;
}// Quant_Populosity


///////////////////////////////////////////////////////////////////////////////
//
//      Dither the image using a threshold of 1/2.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Dither_Threshold()
{
    for (int i = 0;i < height;i++) {
        for (int j = 0;j < width;j++) {
            unsigned char g = to_gray(j, i);
            if (g > 127) g = 255;
            else g = 0;
            for (int k = 0;k < 3;k++) {
                data[index_of_pixel(j, i, k)] = g;
            }
        }
    }
    return true;
}// Dither_Threshold


///////////////////////////////////////////////////////////////////////////////
//
//      Dither image using random dithering.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Dither_Random()
{
    //ClearToBlack();
    for (int i = 0;i < height;i++) {
        for (int j = 0;j < width;j++) {
            unsigned char g = to_gray(j, i);
            if (g > 76+(rand()%103)) g = 255;
            else g = 0;
            for (int k = 0;k < 3;k++) {
                data[index_of_pixel(j, i, k)] = g;
            }
        }
    }
    return true;
}// Dither_Random


///////////////////////////////////////////////////////////////////////////////
//
//      Perform Floyd-Steinberg dithering on the image.  Return success of 
//  operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Dither_FS()
{
    To_Grayscale();
    for (int i = 0;i < height;i++) {
        for (int j = 0;j < width;j++) {
            unsigned char g = data[index_of_pixel(j,i,0)];
            int quant_error = g > 127 ? g - 255 : g;
            for (int k = 0;k < 3;k++) {
                data[index_of_pixel(j, i, k)] = g > 127 ? 255 : 0;
            }
            if (legal_index(j + 1, i)) {
                int result = data[index_of_pixel(j + 1, i, 0)] + quant_error * 7 / 16;
                if (result > 255) result = 255;
                else if (result < 0) result = 0;
                for (int k = 0;k < 3;k++) {
                    data[index_of_pixel(j + 1, i, k)] = result;
                }
            }
            if (legal_index(j - 1, i + 1)) {
                int result = data[index_of_pixel(j - 1, i + 1, 0)] + quant_error * 3 / 16;
                if (result > 255) result = 255;
                else if (result < 0) result = 0;
                for (int k = 0;k < 3;k++) {
                    data[index_of_pixel(j - 1, i + 1, k)] = result;
                }
            }
            if (legal_index(j, i + 1)) {
                int result = data[index_of_pixel(j, i + 1, 0)] + quant_error * 5 / 16;
                if (result > 255) result = 255;
                else if (result < 0) result = 0;
                for (int k = 0;k < 3;k++) {
                    data[index_of_pixel(j, i + 1, k)] = result;
                }
            }
            if (legal_index(j + 1, i + 1)) {
                int result = data[index_of_pixel(j + 1, i + 1, 0)] + quant_error * 1 / 16;
                if (result > 255) result = 255;
                else if (result < 0) result = 0;
                for (int k = 0;k < 3;k++) {
                    data[index_of_pixel(j + 1, i + 1, k)] = result;
                }
            }
        }
    }
    return true;
}// Dither_FS


///////////////////////////////////////////////////////////////////////////////
//
//      Dither the image while conserving the average brightness.  Return 
//  success of operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Dither_Bright()
{
    unsigned long long int all_brightness = 0;
    int min_diff = INT_MAX;
    int averge_brightness = 0;
    int best_black_count = 0;
    int best_threshold = 0;
    for (int i = 0;i < height;i++) {
        for (int j = 0;j < width;j++) {
            all_brightness += to_gray(j, i);
        }
    }
    best_black_count = all_brightness / 255;
    for (int threshold = 0;threshold < 256;threshold++) {
        int black_count = 0;
        for (int i = 0;i < height;i++) {
            for (int j = 0;j < width;j++) {
                if (to_gray(j, i) > threshold) black_count++;
            }
        }
        if (abs(best_black_count - black_count) < min_diff) {
            min_diff = abs(best_black_count - black_count);
            best_threshold = threshold;
        }
    }
    for (int i = 0;i < height;i++) {
        for (int j = 0;j < width;j++) {
            unsigned char g = to_gray(j, i);
            if (g > best_threshold) g = 255;
            else g = 0;
            for (int k = 0;k < 3;k++) {
                data[index_of_pixel(j, i, k)] = g;
            }
        }
    }

    return true;
}// Dither_Bright


///////////////////////////////////////////////////////////////////////////////
//
//      Perform clustered differing of the image.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Dither_Cluster()
{
    double matrix[4][4] = {
        {0.7059, 0.3529, 0.5882, 0.2353 },
        {0.0588, 0.9412, 0.8235, 0.4118 },
        {0.4706, 0.7647, 0.8824, 0.1176},
        {0.1765, 0.5294, 0.2941, 0.6471} };
    for (int i = 0;i < height;i++) {
        for (int j = 0;j < width;j++) {
            unsigned char g = to_gray(j, i);
            if (g > 255*matrix[j%4][i%4]) g = 255;
            else g = 0;
            for (int k = 0;k < 3;k++) {
                data[index_of_pixel(j, i, k)] = g;
            }
        }
    }
    return true;
}// Dither_Cluster


///////////////////////////////////////////////////////////////////////////////
//
//  Convert the image to an 8 bit image using Floyd-Steinberg dithering over
//  a uniform quantization - the same quantization as in Quant_Uniform.
//  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Dither_Color()
{
    
    for (int i = 0;i < height;i++) {
        for (int j = 0;j < width;j++) {
            int r = data[index_of_pixel(j, i, RED)];
            int g = data[index_of_pixel(j, i, GREEN)];
            int b = data[index_of_pixel(j, i, BLUE)];
            //int nr = (int)(r / 8) * 8;
            //int ng = (int)(g / 8) * 8;
            //int nb = (int)(b / 4) * 4;
            int nr = (7 * r / 255) * (255 / 7);
            int ng = (7 * g / 255) * (255 / 7);
            int nb = (3 * b / 255) * (255 / 3);
            int err[3];
            err[0] = r - nr;
            err[1] = g - ng;
            err[2] = b - nb;
            data[index_of_pixel(j, i, RED)] = nr;
            data[index_of_pixel(j, i, GREEN)] = ng;
            data[index_of_pixel(j, i, BLUE)] = nb;
            if (legal_index(j + 1, i)) {
                for (int k = 0;k < 3;k++) {
                    int result =  err[k] * 7 / 16 + data[index_of_pixel(j + 1, i, k)];
                    if (result > 255) result = 255;
                    else if (result < 0) result = 0;
                    data[index_of_pixel(j + 1, i, k)] = result;
                }
            }
            if (legal_index(j - 1, i + 1)) {
                for (int k = 0;k < 3;k++) {
                    int result = err[k] * 3 / 16 + data[index_of_pixel(j - 1, i + 1, k)];
                    if (result > 255) result = 255;
                    else if (result < 0) result = 0;
                    data[index_of_pixel(j - 1, i + 1, k)] = result;
                }
            }
            if (legal_index(j, i + 1)) {
                for (int k = 0;k < 3;k++) {
                    int result = err[k] * 5 / 16 + data[index_of_pixel(j, i + 1, k)];
                    if (result > 255) result = 255;
                    else if (result < 0) result = 0;
                    data[index_of_pixel(j , i + 1, k)] = result;
                }
            }
            if (legal_index(j + 1, i + 1)) {
                for (int k = 0;k < 3;k++) {
                    int result = err[k] * 1 / 16 + data[index_of_pixel(j + 1, i + 1, k)];
                    if (result > 255) result = 255;
                    else if (result < 0) result = 0;
                    data[index_of_pixel(j + 1, i + 1, k)] = result;
                }
            }
        }
    }
    return true;
}// Dither_Color


///////////////////////////////////////////////////////////////////////////////
//
//      Composite the current image over the given image.  Return success of 
//  operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Comp_Over(TargaImage* pImage)
{
    if (width != pImage->width || height != pImage->height)
    {
        cout <<  "Comp_Over: Images not the same size\n";
        return false;
    }

    ClearToBlack();
    return false;
}// Comp_Over


///////////////////////////////////////////////////////////////////////////////
//
//      Composite this image "in" the given image.  See lecture notes for 
//  details.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Comp_In(TargaImage* pImage)
{
    if (width != pImage->width || height != pImage->height)
    {
        cout << "Comp_In: Images not the same size\n";
        return false;
    }

    ClearToBlack();
    return false;
}// Comp_In


///////////////////////////////////////////////////////////////////////////////
//
//      Composite this image "out" the given image.  See lecture notes for 
//  details.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Comp_Out(TargaImage* pImage)
{
    if (width != pImage->width || height != pImage->height)
    {
        cout << "Comp_Out: Images not the same size\n";
        return false;
    }

    ClearToBlack();
    return false;
}// Comp_Out


///////////////////////////////////////////////////////////////////////////////
//
//      Composite current image "atop" given image.  Return success of 
//  operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Comp_Atop(TargaImage* pImage)
{
    if (width != pImage->width || height != pImage->height)
    {
        cout << "Comp_Atop: Images not the same size\n";
        return false;
    }

    ClearToBlack();
    return false;
}// Comp_Atop


///////////////////////////////////////////////////////////////////////////////
//
//      Composite this image with given image using exclusive or (XOR).  Return
//  success of operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Comp_Xor(TargaImage* pImage)
{
    if (width != pImage->width || height != pImage->height)
    {
        cout << "Comp_Xor: Images not the same size\n";
        return false;
    }

    ClearToBlack();
    return false;
}// Comp_Xor


///////////////////////////////////////////////////////////////////////////////
//
//      Calculate the difference bewteen this imag and the given one.  Image 
//  dimensions must be equal.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Difference(TargaImage* pImage)
{
    if (!pImage)
        return false;

    if (width != pImage->width || height != pImage->height)
    {
        cout << "Difference: Images not the same size\n";
        return false;
    }// if

    for (int i = 0 ; i < width * height * 4 ; i += 4)
    {
        unsigned char        rgb1[3];
        unsigned char        rgb2[3];

        RGBA_To_RGB(data + i, rgb1);
        RGBA_To_RGB(pImage->data + i, rgb2);

        data[i] = abs(rgb1[0] - rgb2[0]);
        data[i+1] = abs(rgb1[1] - rgb2[1]);
        data[i+2] = abs(rgb1[2] - rgb2[2]);
        data[i+3] = 255;
    }

    return true;
}// Difference


///////////////////////////////////////////////////////////////////////////////
//
//      Perform 5x5 box filter on this image.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Filter_Box()
{
    //ClearToBlack();
    for (int i = 0;i < height;i++) {
        for (int j = 0;j < width;j++) {
            int resultR = 0;
            int resultG = 0;
            int resultB = 0;
            for (int k = -2;k < 3;k++) {
                for (int g = -2;g < 3;g++){
                    int x = j + g;
                    int y = i + k;
                    if (x < 0) x *= -1;
                    else if (x >= width) x = -1 * x + 2 * (width - 1);

                    if (y < 0) y *= -1;
                    else if (y >= height) y = -1 * y + 2 * (height - 1);

                    resultR += data[index_of_pixel(x, y, RED)];
                    resultG += data[index_of_pixel(x, y, GREEN)];
                    resultB += data[index_of_pixel(x, y, BLUE)];
                }
            }
            resultR /= 25;
            resultG /= 25;
            resultB /= 25;
            data[index_of_pixel(j, i, RED)] = avoid_overflow(resultR);
            data[index_of_pixel(j, i, GREEN)] = avoid_overflow(resultG);
            data[index_of_pixel(j, i, BLUE)] = avoid_overflow(resultB);
        }
    }
    return true;
}// Filter_Box


///////////////////////////////////////////////////////////////////////////////
//
//      Perform 5x5 Bartlett filter on this image.  Return success of 
//  operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Filter_Bartlett()
{
    int gk[5][5] = {
        {1,2,3,2,1},
        {2,4,6,4,2},
        {3,6,9,6,3},
        {2,4,6,4,2},
        {1,2,3,2,1},
    };
    for (int i = 0;i < height;i++) {
        for (int j = 0;j < width;j++) {
            double resultR = 0;//要用double存，否則會遺失很多小數點，圖片地板變一圈一圈的
            double resultG = 0;//要用double存，否則會遺失很多小數點，圖片地板變一圈一圈的
            double resultB = 0;//要用double存，否則會遺失很多小數點，圖片地板變一圈一圈的
            for (int k = 0;k < 5;k++) {
                for (int g = 0;g < 5;g++) {
                    int x = j + g - 2;
                    int y = i + k - 2;
                    if (x < 0) x *= -1;
                    else if (x >= width) x = 2 * (width - 1) - x;

                    if (y < 0) y *= -1;
                    else if (y >= height) y = 2 * (height - 1) - y;

                    resultR += (double)data[index_of_pixel(x, y, RED)] * ((double)gk[k][g] / 81.0f);//整數除法前要先強制轉型，不然會遺失小數點
                    resultG += (double)data[index_of_pixel(x, y, GREEN)] * ((double)gk[k][g] / 81.0f);//整數除法前要先強制轉型，不然會遺失小數點
                    resultB += (double)data[index_of_pixel(x, y, BLUE)] * ((double)gk[k][g] /81.0f);//整數除法前要先強制轉型，不然會遺失小數點
                }
            }
            data[index_of_pixel(j, i, RED)] = avoid_overflow(resultR);
            data[index_of_pixel(j, i, GREEN)] = avoid_overflow(resultG);
            data[index_of_pixel(j, i, BLUE)] = avoid_overflow(resultB);
        }
    }
    return true;
}// Filter_Bartlett

///////////////////////////////////////////////////////////////////////////////
//
//      Perform 5x5 Gaussian filter on this image.  Return success of 
//  operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Filter_Gaussian()
{
///////////////////////////////以下程式碼的出處:https://www.codewithc.com/gaussian-filter-generation-in-c/ ///////////////////////////
    double gk[5][5];
    double stdv = 1.0;
    double r, s = 2.0 * stdv * stdv;  // Assigning standard deviation to 1.0
    double sum = 0.0;   // Initialization of sun for normalization
    for (int x = -2; x <= 2; x++) // Loop to generate 5x5 kernelF
    {
        for (int y = -2; y <= 2; y++)
        {
            r = sqrt(x * x + y * y);
            gk[x + 2][y + 2] = (exp(-(r * r) / s)) / (M_PI * s);
            sum += gk[x + 2][y + 2];
        }
    }

    for (int i = 0; i < 5; ++i) // Loop to normalize the kernel
        for (int j = 0; j < 5; ++j)
            gk[i][j] /= sum;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (int i = 0;i < height;i++) {
        for (int j = 0;j < width;j++) {
            double resultR = 0;//要用double存，否則會遺失很多小數點，圖片地板變一圈一圈的
            double resultG = 0;//要用double存，否則會遺失很多小數點，圖片地板變一圈一圈的
            double resultB = 0;//要用double存，否則會遺失很多小數點，圖片地板變一圈一圈的
            for (int k = 0;k < 5;k++) {
                for (int g = 0;g < 5;g++) {
                    int x = j + g - 2;
                    int y = i + k - 2;
                    if (x < 0) x *= -1;
                    else if (x >= width) x = 2 * (width - 1) - x;

                    if (y < 0) y *= -1;
                    else if (y >= height) y = 2 * (height - 1) - y;

                    resultR += (double)data[index_of_pixel(x, y, RED)] * gk[k][g];
                    resultG += (double)data[index_of_pixel(x, y, GREEN)] * gk[k][g];
                    resultB += (double)data[index_of_pixel(x, y, BLUE)] * gk[k][g];
                }
            }
            data[index_of_pixel(j, i, RED)] = avoid_overflow(resultR);
            data[index_of_pixel(j, i, GREEN)] = avoid_overflow(resultG);
            data[index_of_pixel(j, i, BLUE)] = avoid_overflow(resultB);
        }
    }
    return true;
}// Filter_Gaussian

///////////////////////////////////////////////////////////////////////////////
//
//      Perform NxN Gaussian filter on this image.  Return success of 
//  operation.
//
///////////////////////////////////////////////////////////////////////////////

bool TargaImage::Filter_Gaussian_N( unsigned int N )
{
    ///////////////////////////////以下程式碼的出處:https://www.codewithc.com/gaussian-filter-generation-in-c/ ///////////////////////////
    double** gk = new double* [N] {};
    for (int i = 0;i < N;i++) {
        gk[i] = new double[N] {};
    }
    double stdv = 1.0;
    double r, s = 2.0 * stdv * stdv;  // Assigning standard deviation to 1.0
    double sum = 0.0;   // Initialization of sun for normalization
    int halfN = (N / 2);
    for (int x = -1* halfN; x <= halfN; x++) // 不可用(N/2)取代halfN，因為N是unsigned int，跟int比較會被視為很大的負數
    {
        for (int y = -1 * halfN; y <= halfN; y++)// 不可用(N/2)取代halfN，因為N是unsigned int，跟int比較會被視為很大的負數
        {
            r = sqrt(x * x + y * y);
            gk[x + halfN][y + halfN] = (exp(-(r * r) / s)) / (M_PI * s);
            sum += gk[x + halfN][y + halfN];
        }
    }
    for (int i = 0; i < N; ++i) // Loop to normalize the kernel
        for (int j = 0; j < N; ++j)
            gk[i][j] /= sum;
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (int i = 0;i < height;i++) {
        for (int j = 0;j < width;j++) {
            double resultR = 0;//要用double存，否則會遺失很多小數點，圖片地板變一圈一圈的
            double resultG = 0;//要用double存，否則會遺失很多小數點，圖片地板變一圈一圈的
            double resultB = 0;//要用double存，否則會遺失很多小數點，圖片地板變一圈一圈的
            for (int k = 0;k < N;k++) {
                for (int g = 0;g < N;g++) {
                    int x = j + g - halfN;
                    int y = i + k - halfN;
                    if (x < 0) x *= -1;
                    else if (x >= width) x = 2 * (width - 1) - x;

                    if (y < 0) y *= -1;
                    else if (y >= height) y = 2 * (height - 1) - y;

                    resultR += (double)data[index_of_pixel(x, y, RED)] * gk[k][g];
                    resultG += (double)data[index_of_pixel(x, y, GREEN)] * gk[k][g];
                    resultB += (double)data[index_of_pixel(x, y, BLUE)] * gk[k][g];
                }
            }
            data[index_of_pixel(j, i, RED)] = avoid_overflow(resultR);
            data[index_of_pixel(j, i, GREEN)] = avoid_overflow(resultG);
            data[index_of_pixel(j, i, BLUE)] = avoid_overflow(resultB);
        }
    }
    for (int i = 0;i < N;i++) {
        delete[] gk[i];
    }
    delete[] gk;
   return true;
}// Filter_Gaussian_N


///////////////////////////////////////////////////////////////////////////////
//
//      Perform 5x5 edge detect (high pass) filter on this image.  Return 
//  success of operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Filter_Edge()
{
    ClearToBlack();
    return false;
}// Filter_Edge


///////////////////////////////////////////////////////////////////////////////
//
//      Perform a 5x5 enhancement filter to this image.  Return success of 
//  operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Filter_Enhance()
{
    ClearToBlack();
    return false;
}// Filter_Enhance


///////////////////////////////////////////////////////////////////////////////
//
//      Run simplified version of Hertzmann's painterly image filter.
//      You probably will want to use the Draw_Stroke funciton and the
//      Stroke class to help.
// Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::NPR_Paint()
{
    ClearToBlack();
    return false;
}



///////////////////////////////////////////////////////////////////////////////
//
//      Halve the dimensions of this image.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Half_Size()
{
    int filter[3][3] = {
        {1,2,1},
        {2,4,2},
        {1,2,1},
    };
    int new_height = (int)(height / 2);
    int new_width = (int)(width / 2);
    unsigned char* new_data = new unsigned char[new_width * new_height * 4]{};
    for (int i = 0;i < new_height;i++) {
        for (int j = 0;j < new_width;j++) {
            double resultR = 0;//要用double存，否則會遺失很多小數點，圖片地板變一圈一圈的
            double resultG = 0;//要用double存，否則會遺失很多小數點，圖片地板變一圈一圈的
            double resultB = 0;//要用double存，否則會遺失很多小數點，圖片地板變一圈一圈的
            double resultA = 0;//要用double存，否則會遺失很多小數點，圖片地板變一圈一圈的
            for (int k = 0;k < 3;k++) {
                for (int g = 0;g < 3;g++) {
                    int x = 2 * j + g - 1;
                    int y = 2 * i + k - 1;
                    if (x < 0) x *= -1;
                    else if (x >= width) x = 2 * (width - 1) - x;

                    if (y < 0) y *= -1;
                    else if (y >= height) y = 2 * (height - 1) - y;

                    resultR += (double)data[index_of_pixel(x, y, RED)] * (double)filter[k][g] / 16.0f;
                    resultG += (double)data[index_of_pixel(x, y, GREEN)] * (double)filter[k][g] / 16.0f;
                    resultB += (double)data[index_of_pixel(x, y, BLUE)] * (double)filter[k][g] / 16.0f;
                    resultA += (double)data[index_of_pixel(x, y, ALPHA)] * (double)filter[k][g] / 16.0f;
                }
            }
            new_data[(i * new_width + j) * 4 + RED] = avoid_overflow(resultR);
            new_data[(i * new_width + j) * 4 + GREEN] = avoid_overflow(resultG);
            new_data[(i * new_width + j) * 4 + BLUE] = avoid_overflow(resultB);
            new_data[(i * new_width + j) * 4 + ALPHA] = avoid_overflow(resultA);
        }
    }
    width  = new_width;
    height = new_height;
    delete[] data;
    data = new_data;
    return true;
}// Half_Size


///////////////////////////////////////////////////////////////////////////////
//
//      Double the dimensions of this image.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Double_Size()
{
    //ClearToBlack();
    return true;
}// Double_Size


///////////////////////////////////////////////////////////////////////////////
//
//      Scale the image dimensions by the given factor.  The given factor is 
//  assumed to be greater than one.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Resize(float scale)
{
    ClearToBlack();
    return false;
}// Resize


//////////////////////////////////////////////////////////////////////////////
//
//      Rotate the image clockwise by the given angle.  Do not resize the 
//  image.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Rotate(float angleDegrees)
{
    ClearToBlack();
    return false;
}// Rotate


//////////////////////////////////////////////////////////////////////////////
//
//      Given a single RGBA pixel return, via the second argument, the RGB
//      equivalent composited with a black background.
//
///////////////////////////////////////////////////////////////////////////////
void TargaImage::RGBA_To_RGB(unsigned char *rgba, unsigned char *rgb)
{
    const unsigned char	BACKGROUND[3] = { 0, 0, 0 };

    unsigned char  alpha = rgba[3];

    if (alpha == 0)
    {
        rgb[0] = BACKGROUND[0];
        rgb[1] = BACKGROUND[1];
        rgb[2] = BACKGROUND[2];
    }
    else
    {
	    float	alpha_scale = (float)255 / (float)alpha;
	    int	val;
	    int	i;

	    for (i = 0 ; i < 3 ; i++)
	    {
	        val = (int)floor(rgba[i] * alpha_scale);
	        if (val < 0)
		    rgb[i] = 0;
	        else if (val > 255)
		    rgb[i] = 255;
	        else
		    rgb[i] = val;
	    }
    }
}// RGA_To_RGB


///////////////////////////////////////////////////////////////////////////////
//
//      Copy this into a new image, reversing the rows as it goes. A pointer
//  to the new image is returned.
//
///////////////////////////////////////////////////////////////////////////////
TargaImage* TargaImage::Reverse_Rows(void)
{
    unsigned char   *dest = new unsigned char[width * height * 4];
    TargaImage	    *result;
    int 	        i, j;

    if (! data)
    	return NULL;

    for (i = 0 ; i < height ; i++)
    {
	    int in_offset = (height - i - 1) * width * 4;
	    int out_offset = i * width * 4;

	    for (j = 0 ; j < width ; j++)
        {
	        dest[out_offset + j * 4] = data[in_offset + j * 4];
	        dest[out_offset + j * 4 + 1] = data[in_offset + j * 4 + 1];
	        dest[out_offset + j * 4 + 2] = data[in_offset + j * 4 + 2];
	        dest[out_offset + j * 4 + 3] = data[in_offset + j * 4 + 3];
        }
    }

    result = new TargaImage(width, height, dest);
    delete[] dest;
    return result;
}// Reverse_Rows


///////////////////////////////////////////////////////////////////////////////
//
//      Clear the image to all black.
//
///////////////////////////////////////////////////////////////////////////////
void TargaImage::ClearToBlack()
{
    memset(data, 0, width * height * 4);
}// ClearToBlack


///////////////////////////////////////////////////////////////////////////////
//
//      Helper function for the painterly filter; paint a stroke at
// the given location
//
///////////////////////////////////////////////////////////////////////////////
void TargaImage::Paint_Stroke(const Stroke& s) {
   int radius_squared = (int)s.radius * (int)s.radius;
   for (int x_off = -((int)s.radius); x_off <= (int)s.radius; x_off++) {
      for (int y_off = -((int)s.radius); y_off <= (int)s.radius; y_off++) {
         int x_loc = (int)s.x + x_off;
         int y_loc = (int)s.y + y_off;
         // are we inside the circle, and inside the image?
         if ((x_loc >= 0 && x_loc < width && y_loc >= 0 && y_loc < height)) {
            int dist_squared = x_off * x_off + y_off * y_off;
            if (dist_squared <= radius_squared) {
               data[(y_loc * width + x_loc) * 4 + 0] = s.r;
               data[(y_loc * width + x_loc) * 4 + 1] = s.g;
               data[(y_loc * width + x_loc) * 4 + 2] = s.b;
               data[(y_loc * width + x_loc) * 4 + 3] = s.a;
            } else if (dist_squared == radius_squared + 1) {
               data[(y_loc * width + x_loc) * 4 + 0] = 
                  (data[(y_loc * width + x_loc) * 4 + 0] + s.r) / 2;
               data[(y_loc * width + x_loc) * 4 + 1] = 
                  (data[(y_loc * width + x_loc) * 4 + 1] + s.g) / 2;
               data[(y_loc * width + x_loc) * 4 + 2] = 
                  (data[(y_loc * width + x_loc) * 4 + 2] + s.b) / 2;
               data[(y_loc * width + x_loc) * 4 + 3] = 
                  (data[(y_loc * width + x_loc) * 4 + 3] + s.a) / 2;
            }
         }
      }
   }
}


///////////////////////////////////////////////////////////////////////////////
//
//      Build a Stroke
//
///////////////////////////////////////////////////////////////////////////////
Stroke::Stroke() {}

///////////////////////////////////////////////////////////////////////////////
//
//      Build a Stroke
//
///////////////////////////////////////////////////////////////////////////////
Stroke::Stroke(unsigned int iradius, unsigned int ix, unsigned int iy,
               unsigned char ir, unsigned char ig, unsigned char ib, unsigned char ia) :
   radius(iradius),x(ix),y(iy),r(ir),g(ig),b(ib),a(ia)
{
}

