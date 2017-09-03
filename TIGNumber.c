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

#include "TigerEngine.h"

// Global variable so the objects are not lost
static TIGValue *theNumberStack;
static TIGValue *theStringStack;
static int stackNumber;
static char *stackString;

// ************* For debugging Purposes *************
TIGValue *TIGNumberNumberStack(void)
{
	return theNumberStack;
}

TIGValue *TIGNumberStringStack(void)
{
	return theStringStack;
}
// ************* For debugging Purposes *************

void TIGNumberStartStack(const char *startStackString)
{
	// If there is another start stack called before the end stack free it
	if (stackString != NULL)
	{
		free(stackString);
		stackString = NULL;
	}

	if (startStackString == NULL)
	{
		stackNumber++;
	}
	else
	{
		stackString = (char *)malloc((strlen(startStackString) + 1) * sizeof(char));
		
		if (startStackString != NULL)
		{
			strcpy(stackString, startStackString);
		}
	}
}

void TIGNumberEndStack(const char *endStackString)
{
	if (endStackString != NULL)
	{
		while (theStringStack != NULL)
		{
			TIGValue *theNextStack = theStringStack->nextStack;
			
			// 0 means both strings are the same
			if (strcmp(theStringStack->stackString, endStackString) == 0)
			{
				theStringStack = TIGNumberDestroy(theStringStack);
			}
			
			theStringStack = theNextStack;
		}
	}
	else
	{
		while (theNumberStack != NULL)
		{
			TIGValue *theNextStack = theNumberStack->nextStack;
			
			if (theNumberStack->stackNumber == stackNumber)
			{
				theNumberStack = TIGNumberDestroy(theNumberStack);
			}
			
			theNumberStack = theNextStack;
		}
	}
	
	// If there is another end or start stack string called before this end stack free it
	if (stackString != NULL)
	{
		free(stackString);
		stackString = NULL;
	}

	if (endStackString == NULL)
	{
		stackNumber--;
	}
}

TIGValue *TIGNumberCreate(TIGValue *tigNumber, TIGBool useStack)
{
	tigNumber = (TIGValue *)malloc(1 * sizeof(TIGValue));
	
	if (tigNumber == NULL)
	{
#ifdef TIG_DEBUG
		printf("ERROR Function:TIGNumberCreate() Variable:tigNumber Equals:NULL\n");

	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
		return NULL;
	}
	
	if (useStack)
	{
		if (stackString != NULL)
		{
			if (theStringStack == NULL)
			{
				tigNumber->nextStack = NULL;
			}
			// Add the last added TIGNumber to the new tigNumber's ->nextStack
			else
			{
				tigNumber->nextStack = theStringStack;
			}
			
			tigNumber->stackNumber = -1;
			tigNumber->stackString = (char *)malloc((strlen(stackString) + 1) * sizeof(char));
			
			if (tigNumber->stackString != NULL)
			{
				strcpy(tigNumber->stackString, stackString);
			}
			else
			{
#ifdef TIG_DEBUG
				printf("ERROR Function:TIGNumberCreate() Variable:tigNumber->stackString Equals:NULL\n");

	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
			}
			
			// This adds the tigNumber to the global TIGNumber stack
			theStringStack = tigNumber;
		}
		else
		{
			if (theNumberStack == NULL)
			{
				tigNumber->nextStack = NULL;
			}
			// Add the last added TIGNumber to the new tigNumber's ->nextStack
			else
			{
				tigNumber->nextStack = theNumberStack;
			}
			
			tigNumber->stackNumber = stackNumber;
			tigNumber->stackString = NULL;
			
			// This adds the tigNumber to the global TIGNumber stack
			theNumberStack = tigNumber;
		}
	}
	else
	{
		tigNumber->nextStack = NULL;
		
		tigNumber->stackString = NULL;
		tigNumber->stackNumber = -2;
	}
		
	tigNumber->nextLevel = NULL;
	tigNumber->thisLevel = NULL;
	
	tigNumber->number = 0.0;
	
	// Sets the TIGObject's string to an empty string
	tigNumber->string = NULL;
	
	// object type
	tigNumber->type = "Number";
	
	//printf("%d", (*tigString) != NULL);
	return tigNumber;
}

TIGValue *TIGNumberDestroy(TIGValue *tigNumber)
{
	// If the "tigString" pointer has already been used free it
	if (tigNumber != NULL)
	{		
		if (strcmp(tigNumber->type, "Number") != 0)
		{
#ifdef TIG_DEBUG
			printf("ERROR Function:TIGNumberDestroy() Variable:tigNumber->type Equals:%s Valid:\"Number\"\n", tigNumber->type);
	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
			return tigNumber;
		}

		if (tigNumber->string != NULL)
		{
			if (strcmp(tigNumber->string, "false") != 0 && strcmp(tigNumber->string, "true") != 0 && strcmp(tigNumber->type, "Number") != 0)
			{
				free(tigNumber->string);
			}
			
			tigNumber->string = NULL;
		}
		
		if (tigNumber->stackString != NULL)
		{
			free(tigNumber->stackString);
			tigNumber->stackString = NULL;
		}

		tigNumber->number = 0.0;
		tigNumber->stackNumber = 0;
		
		tigNumber->type = NULL;
		
		tigNumber->nextStack = NULL;
		tigNumber->nextLevel = NULL;
		tigNumber->thisLevel = NULL;
		
		free(tigNumber);
		tigNumber = NULL;
	}
	
	return tigNumber;
}

TIGValue *TIGNumberBooleanInput(TIGValue *tigNumber, TIGBool booleanValue)
{
	return TIGNumberBooleanStackInput(tigNumber, booleanValue, TIGYes);
}

TIGValue *TIGNumberBooleanStackInput(TIGValue *tigNumber, TIGBool booleanValue, TIGBool useStack)
{
	if (booleanValue == TIGNo)
	{
		tigNumber = TIGNumberStackInput(tigNumber, TIGNo, useStack);
		tigNumber->string = "false";
	}
	else
	{
		tigNumber = TIGNumberStackInput(tigNumber, TIGYes, useStack);
		tigNumber->string = "true";
	}
	
	return tigNumber;
}

TIGValue *TIGNum(TIGScalar number)
{
	return TIGNumberInput(NULL, number);
}

TIGValue *TIGNumberInput(TIGValue *tigNumber, TIGScalar number)
{
	return TIGNumberStackInput(tigNumber, number, TIGYes);
}

TIGValue *TIGNumberStackInput(TIGValue *tigNumber, TIGScalar number, TIGBool useStack)
{
	if (tigNumber == NULL)
	{
		tigNumber = TIGNumberCreate(tigNumber, useStack);
		
		if (tigNumber == NULL)
		{
#ifdef TIG_DEBUG
			printf("ERROR Function:TIGNumberStackInput() Variable:tigNumber Equals:NULL\n");
	
	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
			return NULL;
		}
	}
	else if (strcmp(tigNumber->type, "Number") != 0)
	{
#ifdef TIG_DEBUG
		printf("ERROR Function:TIGNumberStackInput() Variable:tigNumber->type Equals:%s Valid:\"Number\"\n", tigNumber->type);

	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
		return NULL;
	}
	
	tigNumber->number = number;
	
	return tigNumber;
}

TIGScalar TIGNumberOutput(TIGValue *tigNumber)
{
	if (tigNumber == NULL || strcmp(tigNumber->type, "Number") != 0)
	{
#ifdef TIG_DEBUG
		if (tigNumber == NULL)
		{
			printf("ERROR Function:TIGNumberOutput() Variable:tigNumber Equals:NULL\n");
		}
		else
		{
			if (strcmp(tigNumber->type, "Number") != 0)
			{
				printf("ERROR Function:TIGNumberOutput() Variable:tigNumber->type Equals:%s Valid:\"Number\"\n", tigNumber->type);
			}
		}

	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
		return -1;
	}
	else
	{
		return tigNumber->number;
	}
}

TIGScalar TIGNumberFromString(TIGValue *tigString)
{
	if (tigString == NULL || tigString->string == NULL || strcmp(tigString->type, "String") != 0)
	{
#ifdef TIG_DEBUG
		if (tigString == NULL)
		{
			printf("ERROR Function:TIGNumberFromString() Variable:tigString Equals:NULL\n");
		}
		else if (tigString->string == NULL)
		{
			printf("ERROR Function:TIGNumberFromString() Variable:tigString->string Equals:NULL\n");
		}
		else if (strcmp(tigString->type, "String") != 0)
		{
			printf("ERROR Function:TIGNumberFromString() Variable:tigString->type Equals:%s Valid:\"String\"\n", tigString->type);
		}

	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
		return -1;
	}
	else
	{
		return atof(tigString->string);
	}
}

TIGBool TIGNumberEqualsNumber(TIGValue *tigNumber1, TIGValue *tigNumber2)
{
	if (tigNumber1 == NULL || tigNumber2 == NULL || strcmp(tigNumber1->type, "Number") != 0 || strcmp(tigNumber2->type, "Number") != 0)
	{
#ifdef TIG_DEBUG
		if (tigNumber1 == NULL)
		{
			printf("ERROR Function:TIGNumberEqualsNumber() Variable:tigNumber1 Equals:NULL\n");
		}
		else if (strcmp(tigNumber1->type, "Number") != 0)
		{
			printf("ERROR Function:TIGNumberEqualsNumber() Variable:tigNumber1->type Equals:%s Valid:\"Number\"\n", tigNumber1->type);
		}

		if (tigNumber2 == NULL)
		{
			printf("ERROR Function:TIGNumberEqualsNumber() Variable:tigNumber2 Equals:NULL\n");
		}
		else if (strcmp(tigNumber2->type, "Number") != 0)
		{
			printf("ERROR Function:TIGNumberEqualsNumber() Variable:tigNumber2->type Equals:%s Valid:\"Number\"\n", tigNumber2->type);
		}

	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
		return TIGNo;
	}
	
	if (tigNumber1->number == tigNumber2->number)
	{
		return TIGYes;
	}
	
	return TIGNo;
}
