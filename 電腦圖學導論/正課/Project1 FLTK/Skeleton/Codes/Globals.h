///////////////////////////////////////////////////////////////////////////////
//
//      Globals.h                                   Author:  Eric McDaniel
//                                                  Email:   chate@cs.wisc.edu
//                                                  Date:    Summer 2001
//
//      Global variables, macros, etc for this application.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _GLOBALS
#define _GLOBALS


// windows specific
#ifdef _WIN32

    // fix for loop scoping problem in vc, this has no performance cost for a release build
    #define for if (false) {} else for

    // display todos in vc build window, gcc will just ignore them
    #define _QUOTE(x) # x
    #define QUOTE(x) _QUOTE(x)
    #define TODO(x) message(__FILE__"(" QUOTE(__LINE__) "): TODO: " #x"\n" )     

    // silence the "mangled name exceeds 255 characters" bug in vc
    #pragma warning(disable : 4786)
#endif


// global constants
const float c_epsilon   = 0.0001f;     // small value used to compare floating point values
const float c_pi        = 3.14159f;    // the constant pi

#include "Globals.inl"      // global functions and templates

#endif // _GLOBALS


