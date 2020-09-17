///////////////////////////////////////////////////////////////////////////////
//
//      ScriptHandler.cpp                       Author:     Eric McDaniel
//                                              Modified:   Stephen Chenney
//                                              Date:       Fall 2004, Sept 29
//
//      Implementation of CScripthandler methods.  You should not need to
//  modify this file.
//
///////////////////////////////////////////////////////////////////////////////

#include "Globals.h"
#include "ScriptHandler.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include "TargaImage.h"

using namespace std;

// constants
const int       c_maxLineLength         = 1000;                         // maximum length of a command in a script
const char      c_sWhiteSpace[]         = " \t\n\r"; 
const char      c_asCommands[][32]      = { "load",                     // valid commands
                                            "save",
                                            "run",
                                            "gray",
                                            "quant-unif",
                                            "quant-pop",
                                            "dither-thresh",
                                            "dither-rand",
                                            "dither-fs",
                                            "dither-bright",
                                            "dither-cluster",
					    "dither-pattern",
					    "dither-color",
                                            "filter-box",
                                            "filter-bartlett",
                                            "filter-gauss",
                                            "filter-gauss-n",
                                            "filter-edge",
                                            "filter-enhance",
                                            "npr-paint",
                                            "half",
                                            "double",
                                            "scale",
                                            "comp-over",
                                            "comp-in",
                                            "comp-out",
                                            "comp-atop",
                                            "comp-xor",
                                            "diff",
                                            "rotate"
                                          };

enum ECommands          // command ids
{
    LOAD,
    SAVE,
    RUN,
    GRAY,
    QUANT_UNIF,
    QUANT_POP,
    DITHER_THRESH,
    DITHER_RAND,
    DITHER_FS,
    DITHER_BRIGHT,
    DITHER_CLUSTER,
    DITHER_PATTERN,
    DITHER_COLOR,
    FILTER_BOX,
    FILTER_BARTLETT,
    FILTER_GAUSS,
    FILTER_GAUSS_N,
    FILTER_EDGE,
    FILTER_ENHANCE,
    NPR_PAINT,
    HALF,
    DOUBLE,
    SCALE,
    COMP_OVER,
    COMP_IN,
    COMP_OUT,
    COMP_ATOP,
    COMP_XOR,
    DIFF,
    ROTATE,
    NUM_COMMANDS
};// ECommands


///////////////////////////////////////////////////////////////////////////////
//
//      Execute the given command string on the given image.  If the command
//  string could not be parsed, an error message is displayed and false is 
//  returned.  Otherwise return true.
//  
///////////////////////////////////////////////////////////////////////////////
bool CScriptHandler::HandleCommand(const char* sCommand, TargaImage*& pImage)
{
    if (!sCommand || !strlen(sCommand))
        return true;

    char* sCommandLine = new char[strlen(sCommand) + 1];
    strcpy(sCommandLine, sCommand);
    char* sToken = strtok(sCommandLine, c_sWhiteSpace);

    // find command that was given
    int command;
    for (command = 0; command < NUM_COMMANDS; ++command)
        if (!strcmp(sToken, c_asCommands[command]))
            break;

    // if there's no image only a subset of commands are valid
    if (!pImage && command != LOAD && command != RUN && command != NUM_COMMANDS)
    {
        cout << "No image to operate on.  Use \"load\" command to load image." << endl;
        return false;
    }// if

    // handle the command
    bool bResult,
         bParsed = true;

    switch (command)
    {
        case LOAD:
        {
            if (pImage)
                delete pImage;
            char* sFilename = strtok(NULL, c_sWhiteSpace);
            bResult = (pImage = TargaImage::Load_Image(sFilename)) != NULL;

            if (!bResult)
            {
                if (!sFilename)
                    cout << "Unable to load image:  " << endl;
                else
                    cout << "Unable to load image:  " << sFilename << endl;
                
                bParsed = false;
            }// if
            break;
        }// LOAD

        case SAVE:
        {
            char* sFilename = strtok(NULL, c_sWhiteSpace);
            if (!sFilename)
                cout << "No filename given." << endl;

            bParsed = sFilename != NULL;
            bResult =  bParsed && pImage->Save_Image(sFilename);
            break;
        }// SAVE

        case RUN:
        {
            bResult = HandleScriptFile(strtok(NULL, c_sWhiteSpace), pImage);
            break;
        }// RUN

        case GRAY:
        {
            bResult = pImage->To_Grayscale();
            break;
        }// GREY

        case QUANT_UNIF:
        {
            bResult = pImage->Quant_Uniform();
            break;
        }// QUANT_UNIF

        case QUANT_POP:
        {
            bResult = pImage->Quant_Populosity();
            break;
        }// QUANT_POP

        case DITHER_THRESH:
        {
            bResult = pImage->Dither_Threshold();
            break;
        }// QUANT_THRESH

        case DITHER_RAND:
        {
            bResult = pImage->Dither_Random();
            break;
        }// DITHER_RAND

        case DITHER_FS:
        {
            bResult = pImage->Dither_FS();
            break;
        }// DITHER_FS

        case DITHER_BRIGHT:
        {
            bResult = pImage->Dither_Bright();
            break;
        }// DITHER_BRIGHT
        
        case DITHER_CLUSTER:
        {
            bResult = pImage->Dither_Cluster();
            break;
        }// DITHER_CLUSTER
        
        case DITHER_COLOR:
        {
            bResult = pImage->Dither_Color();
            break;
        }// DITHER_COLOR

        case FILTER_BOX:
        {
            bResult = pImage->Filter_Box();
            break;
        }// DITHER_BOX

        case FILTER_BARTLETT:
        {
            bResult = pImage->Filter_Bartlett();
            break;
        }// DITHER_BARTLETT

        case FILTER_GAUSS:
        {
            bResult = pImage->Filter_Gaussian();
            break;
        }// FILTER_GUASS

        case FILTER_GAUSS_N:
        {
            char *sN = strtok(NULL, c_sWhiteSpace);
            int N = atoi(sN);
            if (N % 2 != 1) {
               cout << "N \"" << N << "\" is not allowed; N must be an odd number." << endl;
               break;
            }
            bResult = pImage->Filter_Gaussian_N(N);
            break;
        }// FILTER_GUASS_N

        case FILTER_EDGE:
        {
            bResult = pImage->Filter_Edge();
            break;
        }// FILTER_EDGE

        case FILTER_ENHANCE:
        {
            bResult = pImage->Filter_Enhance();
            break;
        }// FILTER_ENHANCE

        case NPR_PAINT:
        {
            bResult = pImage->NPR_Paint();
            break;
        }// NPR_PAINT


        case HALF:
        {
            bResult = pImage->Half_Size();
            break;
        }// HALF

        case DOUBLE:
        {
            bResult = pImage->Double_Size();
            break;
        }// DOUBLE

        case SCALE:
        {
            char *sScale = strtok(NULL, c_sWhiteSpace);
            float scale;

            if (!sScale || !(scale = (float)atof(sScale)) || scale <= 0)
            {
                cout << "Invalid scaling factor." << endl;
                bParsed = bResult = false;
            }// if
            else
                bResult = pImage->Resize(scale);
            break;
        }// SCALE

        case COMP_OVER:
        {
            char* sFilename = strtok(NULL, c_sWhiteSpace);
            TargaImage* pNewImage = TargaImage::Load_Image(sFilename);
            if (!pNewImage)
            {
                if (sFilename)
                    cout << "Unable to load image:  " << sFilename << endl;
                else
                    cout << "No filename given." << endl;
                bParsed = false;
            }// if
            bResult = pNewImage && pImage->Comp_Over(pNewImage);
            delete pNewImage;
            break;
        }// COMP_OVER

        case COMP_IN:
        {
            char* sFilename = strtok(NULL, c_sWhiteSpace);
            TargaImage* pNewImage = TargaImage::Load_Image(sFilename);
            if (!pNewImage)
            {
                if (sFilename)
                    cout << "Unable to load image:  " << sFilename << endl;
                else
                    cout << "No filename given." << endl;

                bParsed = false;
            }// if
            bResult = pNewImage && pImage->Comp_In(pNewImage);
            delete pNewImage;
            break;
        }// COMP_IN

        case COMP_OUT:
        {
            char* sFilename = strtok(NULL, c_sWhiteSpace);
            TargaImage* pNewImage = TargaImage::Load_Image(sFilename);
            if (!pNewImage)
            {
                if (sFilename)
                    cout << "Unable to load image:  " << sFilename << endl;
                else
                    cout << "No filename given." << endl;

                bParsed = false;
            }// if
            bResult = pNewImage && pImage->Comp_Out(pNewImage);
            delete pNewImage;
            break;
        }// COMP_OUT

        case COMP_ATOP:
        {
            char* sFilename = strtok(NULL, c_sWhiteSpace);
            TargaImage* pNewImage = TargaImage::Load_Image(sFilename);
            if (!pNewImage)
            {
                if (sFilename)
                    cout << "Unable to load image:  " << sFilename << endl;
                else
                    cout << "No filename given." << endl;

                bParsed = false;
            }// if
            bResult = pNewImage && pImage->Comp_Atop(pNewImage);
            delete pNewImage;
            break;
        }// COMP_ATOP

        case COMP_XOR:
        {
            char* sFilename = strtok(NULL, c_sWhiteSpace);
            TargaImage* pNewImage = TargaImage::Load_Image(sFilename);
            if (!pNewImage)
            {
                if (sFilename)
                    cout << "Unable to load image:  " << sFilename << endl;
                else
                    cout << "No filename given." << endl;

                bParsed = false;
            }// if
            bResult = pNewImage && pImage->Comp_Xor(pNewImage);
            delete pNewImage;
            break;
        }// COMP_XOR

        case DIFF:
        {
            char* sFilename = strtok(NULL, c_sWhiteSpace);
            TargaImage* pNewImage = TargaImage::Load_Image(sFilename);
            if (!pNewImage)
            {
                if (sFilename)
                    cout << "Unable to load image:  " << sFilename << endl;
                else
                    cout << "Unable to load image:  " << endl;

                bParsed = false;
            }// if
            bResult = pNewImage && pImage->Difference(pNewImage);
            delete pNewImage;
            break;
        }// DIFF

        case ROTATE:
        {
            char *sAngle = strtok(NULL, c_sWhiteSpace);
            float angle;

            if (!sAngle || !(angle = (float)atof(sAngle)))
            {
                cout << "Invalid rotation angle." << endl;
                bResult = bParsed = false;
            }// if
            else
                bResult = pImage->Rotate(angle);
            break;

            break;
        }// ROTATE

        default:
        {
            cout << "Unable to parse command:  " << sCommand << endl;
            bResult = false;
            bParsed = false;
        }// default
    }// switch

    delete[] sCommandLine;

    return bParsed;
}// HandleCommand


///////////////////////////////////////////////////////////////////////////////
//
//      The given script file is executed on the given image.  If the file is 
//  not correctly parsed an error message is printed and false is returned.  
//  If all commands in the script execute correctly true is returned,
//  otherwise false is returned.
//
///////////////////////////////////////////////////////////////////////////////
bool CScriptHandler::HandleScriptFile(const char* sFilename, TargaImage*& pImage)
{
    if (!sFilename)
    {
        cout << "No filename given." << endl;
        return false;
    }// if

    ifstream inFile(sFilename);

    if (!inFile.is_open())
    {
        cout << "Unable to open file:  " << sFilename << endl;
        return false;
    }// if

    bool bResult = true;
    char sLine[c_maxLineLength + 1];
    while (!inFile.eof() && bResult)
    {
        inFile.getline(sLine, c_maxLineLength);

        if (!inFile.eof())
            bResult = HandleCommand(sLine, pImage);
    }// while

    inFile.close();
    return bResult;
}// CScriptHandler



