/* MIT License (From https://choosealicense.com/ )

Copyright (c) 2017 Jonathan Burget support@solarfusionsoftware.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

#ifndef TIGER_ENGINE_H
#define TIGER_ENGINE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // This is for the string functions like strcpy() and strcat()
#include <stdarg.h> // This is for functions like: function(char *fmt, ...);
#include <math.h> // For sqrt()

// Turns the assert() error function checking on.
#include <assert.h>

// Comment out TIG_DEBUG to turn off internal TIGArray, TIGNumber, TIGObject, and TIGString printf errors when publishing an application.
#define TIG_DEBUG
// Comment this out to turn off assert(); inside TIG_DEBUG
//#define TIG_DEBUG_ASSERT

#define TIG_BUILD_VERSION 10b4

#define TIG_MAJOR_VERSION 1
#define TIG_MINOR_VERSION 1

typedef enum
{
	TIGNo = 0,
	TIGYes = 1
} TIGBool;

// ******* Precision *******
//#define USE_DOUBLE_PRECISION

#ifdef USE_DOUBLE_PRECISION
	typedef double TIGScalar;
#else
	typedef float TIGScalar;
#endif

typedef int TIGInteger;

struct _TIGValue
{
	char *string;
	TIGScalar number;
	
	// This sets where the TIGValue is in an object,
	struct _TIGValue *nextLevel;
	struct _TIGValue *thisLevel;

	char *stackString;
	int stackNumber;

	// This is used internally for the stack in each TIGValue,
	struct _TIGValue *nextStack;

	char *type;
};

typedef struct _TIGValue TIGValue;

typedef union
{
	struct
	{
		TIGScalar x, y, z;
	};
	
	TIGScalar array[3];
} TIGVector;

// m (Row) (Column)
// OpenGL is column major
typedef union
{
	struct
	{
		TIGScalar
			// Row 1
			m11, m12, m13,
			// Row 2
			m21, m22, m23,
			// Row 3
			m31, m32, m33;

//		TIGScalar
//			m11, m21, m31,
//			m12, m22, m32,
//			m13, m23, m33;
	};
	
	TIGScalar array[9];
} TIGMatrix3;

typedef union
{
	struct
	{
		TIGScalar
			// Row 1
			m11, m12, m13, m14,
			// Row 2
			m21, m22, m23, m24,
			// Row 3
			m31, m32, m33, m34,
			// Row 4
			m41, m42, m43, m44;

//		TIGScalar
//			m11, m21, m31, m41,
//			m12, m22, m32, m42,
//			m13, m23, m33, m43,
//			m14, m24, m34, m44;
	};
	
	TIGScalar array[16];
} TIGMatrix4;


#include "TIGArray.h"
#include "TIGNumber.h"
#include "TIGObject.h"
#include "TIGString.h"

#endif // TIGER_ENGINE_H