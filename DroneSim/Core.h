#pragma once


// We use lookup into dependent base class for static polymorphism.
// (This could be done with getters and setters in base class as well.)
#pragma clang diagnostic ignored "-Wmicrosoft-template"
#ifndef _MSC_VER
    #error "Lookup into dependent base class is a Microsoft extension."
#endif



#define CL_TARGET_OPENCL_VERSION 120
#define _CRT_SECURE_NO_WARNINGS
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define _USE_MATH_DEFINES


#include <cassert>
#include <math.h>

#include "Typedefs.h"
#include "Terminate.h"
#include "Utility.h"