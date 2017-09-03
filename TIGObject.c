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
TIGValue *TIGObjectNumberStack(void)
{
	return theNumberStack;
}

TIGValue *TIGObjectStringStack(void)
{
	return theStringStack;
}
// ************* For debugging Purposes *************

void TIGObjectStartStack(const char *startStackString)
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

void TIGObjectEndStack(const char *endStackString)
{
	if (endStackString != NULL)
	{
		while (theStringStack != NULL)
		{
			TIGValue *theNextStack = theStringStack->nextStack;
			
			// 0 means both strings are the same
			if (strcmp(theStringStack->stackString, endStackString) == 0)
			{
				theStringStack = TIGObjectDestroy(theStringStack);
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
				theNumberStack = TIGObjectDestroy(theNumberStack);
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

TIGValue *TIGObjectCreate(TIGValue *tigObject, TIGBool useStack)
{
	tigObject = (TIGValue *)malloc(1 * sizeof(TIGValue));
	
	if (tigObject == NULL)
	{
#ifdef TIG_DEBUG
		printf("ERROR Function:TIGObjectCreate() Variable:tigObject Equals:NULL\n");

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
				tigObject->nextStack = NULL;
			}
			// Add the last added TIGObject to the new tigObject's ->nextStack
			else
			{
				tigObject->nextStack = theStringStack;
			}
			
			tigObject->stackNumber = -1;
			tigObject->stackString = (char *)malloc((strlen(stackString) + 1) * sizeof(char));
			
			if (tigObject->stackString != NULL)
			{
				strcpy(tigObject->stackString, stackString);
			}
			else
			{
#ifdef TIG_DEBUG
				printf("ERROR Function:TIGObjectCreate() Variable:tigObject->stackString Equals:NULL\n");

	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
			}
			
			// This adds the tigObject to the global TIGObject stack
			theStringStack = tigObject;
		}
		else
		{
			if (theNumberStack == NULL)
			{
				tigObject->nextStack = NULL;
			}
			// Add the last added TIGObject to the new tigObject's ->nextStack
			else
			{
				tigObject->nextStack = theNumberStack;
			}
			
			tigObject->stackNumber = stackNumber;
			tigObject->stackString = NULL;
			
			// This adds the tigObject to the global TIGObject stack
			theNumberStack = tigObject;
		}
	}
	else
	{
		tigObject->nextStack = NULL;
		
		tigObject->stackString = NULL;
		tigObject->stackNumber = -2;
	}
	
	tigObject->nextLevel = NULL;
	tigObject->thisLevel = NULL;
	
	tigObject->number = 0.0;
	
	// Sets the TIGObject's string to an empty string
	tigObject->string = NULL;
	
	// object type
	tigObject->type = "Object";
	
	return tigObject;
}

TIGValue *TIGObjectDestroy(TIGValue *tigObject)
{
	if (tigObject != NULL)
	{
		if (strcmp(tigObject->type, "Object") != 0)
		{
#ifdef TIG_DEBUG
			printf("ERROR Function:TIGObjectDestroy() Variable:tigObject->type Equals:%s Valid:\"Object\"\n", tigObject->type);
	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
			return tigObject;
		}
		
		TIGValue *theCurrentValue = tigObject->nextLevel;
		
		while (theCurrentValue != NULL)
		{
			TIGValue *theNextValue = theCurrentValue->thisLevel;
			
			if (strcmp(theCurrentValue->type, "Array") == 0)
			{
				TIGArrayDestroy(theCurrentValue);
			}
			else if (strcmp(theCurrentValue->type, "Number") == 0)
			{
				TIGNumberDestroy(theCurrentValue);
			}
			else if (strcmp(theCurrentValue->type, "Object") == 0)
			{
				TIGObjectDestroy(theCurrentValue);
			}
			else if (strcmp(theCurrentValue->type, "String") == 0)
			{
				TIGStringDestroy(theCurrentValue);
			}
			
			theCurrentValue = theNextValue;
		}
		
		if (tigObject->string != NULL)
		{
			free(tigObject->string);
			tigObject->string = NULL;
		}
		
		if (tigObject->stackString != NULL)
		{
			free(tigObject->stackString);
			tigObject->stackString = NULL;
		}

		tigObject->number = 0.0;
		tigObject->stackNumber = -1;
		tigObject->type = NULL;
				
		tigObject->nextStack = NULL;
		tigObject->nextLevel = NULL;
		tigObject->thisLevel = NULL;
		
		free(tigObject);
		tigObject = NULL;
	}
	
	return tigObject;
}

TIGValue *TIGObj(TIGValue *tigString, TIGValue *tigValue)
{
	return TIGObjectAddStringWithValue(NULL, tigString, tigValue);
}

TIGValue *TIGObjectAddStringWithValue(TIGValue *tigObject, TIGValue *tigString, TIGValue *tigValue)
{
	return TIGObjectStackAddStringWithValue(tigObject, tigString, tigValue, TIGYes);
}

TIGValue *TIGObjectStackAddStringWithValue(TIGValue *tigObject, TIGValue *tigString, TIGValue *tigValue, TIGBool useStack)
{
	if (tigObject == NULL)
	{
		tigObject = TIGObjectCreate(tigObject, useStack);
		
		if (tigObject == NULL)
		{
#ifdef TIG_DEBUG
			printf("ERROR Function:TIGObjectStackAddStringWithValue() Variable:tigObject Equals:NULL\n");
	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
			return NULL;
		}
	}
	
	if (strcmp(tigObject->type, "Object") != 0 || tigValue == NULL)
	{
#ifdef TIG_DEBUG
		if (tigValue == NULL)
		{
			printf("ERROR Function:TIGObjectStackAddStringWithValue() Variable:tigValue Equals:NULL\n");
		}
		
		if (strcmp(tigObject->type, "Object") != 0)
		{
			printf("ERROR Function:TIGObjectStackAddStringWithValue() Variable:tigObject->type Equals:%s Valid:\"Object\"\n", tigObject->type);
		}
	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
		return tigObject;
	}
	else
	{
		TIGValue *tigNewValue = NULL, *tigNewString = NULL;
		
		if (tigString == NULL || strcmp(tigString->type, "String") != 0)
		{
#ifdef TIG_DEBUG
			if (tigString == NULL)
			{
				printf("ERROR Function:TIGObjectStackAddStringWithValue() Variable:tigString Equals:NULL\n");
			}
			else if (strcmp(tigString->type, "String") != 0)
			{
				printf("ERROR Function:TIGObjectStackAddStringWithValue() Variable:tigString->type Equals:%s Valid:\"String\"\n", tigString->type);
			}
	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
			return tigObject;
		}
		else
		{
			tigNewString = TIGStringStackInput(NULL, tigString->string, TIGNo);
		}
		
		if (strcmp(tigValue->type, "Array") == 0)
		{
			// This makes sure that the correct level is used
			TIGValue *theCurrentValue = NULL;

			// This is used if the object has been copied to another object in the stack
			if (tigValue->stackNumber == -2)
			{
				theCurrentValue = tigValue->nextLevel;
			}
			// This is used if the object is on the root/same level
			else
			{
				theCurrentValue = tigValue->thisLevel;
			}

			int index = 0;
			
			while (theCurrentValue != NULL)
			{
				tigNewValue = TIGArrayStackInsertValueAtIndex(tigNewValue, theCurrentValue, index, TIGNo);
				theCurrentValue = theCurrentValue->thisLevel;
				
				index++;
			}
			
			if (tigNewValue == NULL)
			{
#ifdef TIG_DEBUG
				printf("ERROR1 Function:TIGObjectStackAddStringWithValue() Variable:tigNewValue Equals:NULL\n");
	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
				return tigObject;
			}
			
			tigNewValue->nextLevel = tigNewValue->thisLevel;
			tigNewValue->thisLevel = NULL;
		}
		else if (strcmp(tigValue->type, "Number") == 0)
		{
			if (tigValue->string != NULL && strcmp(tigValue->string, "false") == 0)
			{
				tigNewValue = TIGNumberBooleanStackInput(NULL, TIGNo, TIGNo);
			}
			else if (tigValue->string != NULL && strcmp(tigValue->string, "true") == 0)
			{
				tigNewValue = TIGNumberBooleanStackInput(NULL, TIGYes, TIGNo);
			}
			else
			{
				tigNewValue = TIGNumberStackInput(NULL, tigValue->number, TIGNo);
			}
		}
		else if (strcmp(tigValue->type, "Object") == 0)
		{
			// This makes sure that the correct level is used
			TIGValue *theCurrentValue = NULL;

			// This is used if the object has been copied to another object in the stack
			if (tigValue->stackNumber == -2)
			{
				theCurrentValue = tigValue->nextLevel;
			}
			// This is used if the object is on the root/same level
			else
			{
				theCurrentValue = tigValue->thisLevel;
			}

			while (theCurrentValue != NULL)
			{
				tigNewValue = TIGObjectStackAddStringWithValue(tigNewValue, theCurrentValue, theCurrentValue->nextLevel, TIGNo);
				theCurrentValue = theCurrentValue->thisLevel;
			}
		
			if (tigNewValue == NULL)
			{
#ifdef TIG_DEBUG
				printf("ERROR2 Function:TIGObjectStackAddStringWithValue() Variable:tigNewValue Equals:NULL\n");
	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
				return tigObject;
			}
			
			tigNewValue->nextLevel = tigNewValue->thisLevel;
			tigNewValue->thisLevel = NULL;
		}
		else if (strcmp(tigValue->type, "String") == 0)
		{
			if (tigValue->string != NULL)
			{
				tigNewValue = TIGStringStackInput(NULL, tigValue->string, TIGNo);
			}
		}
		
		// Do not use "else if" and attach it to the block above
		if (tigNewValue == NULL)
		{
#ifdef TIG_DEBUG
			printf("ERROR3 Function:TIGObjectStackAddStringWithValue() Variable:tigNewValue Equals:NULL\n");
	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
			return tigObject;
		}
		
		TIGValue *theCurrentValue = tigObject;
		
		while (1)
		{
			// This looks for a link that already has the same name and replaces the value
			if (theCurrentValue->nextLevel != NULL && strcmp(tigString->string, theCurrentValue->string) == 0)
			{
				if (strcmp(theCurrentValue->nextLevel->type, "Array") == 0)
				{
					theCurrentValue->nextLevel = TIGArrayDestroy(theCurrentValue->nextLevel);
				}
				else if (strcmp(theCurrentValue->nextLevel->type, "Number") == 0)
				{
					theCurrentValue->nextLevel = TIGNumberDestroy(theCurrentValue->nextLevel);
				}
				else if (strcmp(theCurrentValue->nextLevel->type, "Object") == 0)
				{
					theCurrentValue->nextLevel = TIGObjectDestroy(theCurrentValue->nextLevel);
				}
				else if (strcmp(theCurrentValue->nextLevel->type, "String") == 0)
				{
					theCurrentValue->nextLevel = TIGStringDestroy(theCurrentValue->nextLevel);
				}
				
				theCurrentValue->nextLevel = tigNewValue;
				break;
			}
			// This adds a new link if there are no links with the same name
			else if (theCurrentValue->thisLevel == NULL)
			{
				tigNewString->nextLevel = tigNewValue;
				theCurrentValue->thisLevel = tigNewString;
				
				break;
			}
			
			theCurrentValue = theCurrentValue->thisLevel;
		}
	}

	return tigObject;
}

TIGValue *TIGObjectValueFromString(TIGValue *tigObject, TIGValue *tigString)
{
	if (tigObject == NULL || tigString == NULL || strcmp(tigObject->type, "Object") != 0 || strcmp(tigString->type, "String") != 0)
	{
#ifdef TIG_DEBUG
		if (tigObject == NULL)
		{
			printf("ERROR Function:TIGObjectValueFromString() Variable:tigObject Equals:NULL\n");
		}
		else if (strcmp(tigObject->type, "Object") != 0)
		{
			printf("ERROR Function:TIGObjectValueFromString() Variable:tigObject->type Equals:%s Valid:\"Object\"\n", tigObject->type);
		}
		
		if (tigString == NULL)
		{
			printf("ERROR Function:TIGObjectValueFromString() Variable:tigString Equals:NULL\n");
		}
		else if (strcmp(tigString->type, "String") != 0)
		{
			printf("ERROR Function:TIGObjectValueFromString() Variable:tigString->type Equals:%s\n Valid:\"String\"", tigString->type);
		}
		
		printf("object value from string error1\n");
	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
		return NULL;
	}

	TIGValue *theCurrentValue = tigObject;
	
	while (1)
	{
		theCurrentValue = theCurrentValue->thisLevel;
		
		if (theCurrentValue != NULL && theCurrentValue->string != NULL)
		{
			// Test to see if the strings match and make sure there is an object on the next level to return
			if (strcmp(tigString->string, theCurrentValue->string) == 0 && theCurrentValue->nextLevel != NULL)
			{
				if (strcmp(theCurrentValue->nextLevel->type, "Array") == 0)
				{
					TIGValue *theNewCurrentValue = theCurrentValue->nextLevel->nextLevel;
					TIGValue *theNewValue = NULL;
					
					while (theNewCurrentValue != NULL)
					{
						theNewValue = TIGArrayAddValue(theNewValue, theNewCurrentValue);
						theNewCurrentValue = theNewCurrentValue->thisLevel;
					}
					
					return theNewValue;
				}
				else if (strcmp(theCurrentValue->nextLevel->type, "Number") == 0)
				{
					if (theCurrentValue->nextLevel->string != NULL)
					{
						if (strcmp(theCurrentValue->nextLevel->string, "false") == 0)
						{
							return TIGNumberBooleanInput(NULL, TIGNo);
						}
						else if (strcmp(theCurrentValue->nextLevel->string, "true") == 0)
						{
							return TIGNumberBooleanInput(NULL, TIGYes);
						}
					}
					else
					{
						return TIGNumberInput(NULL, theCurrentValue->nextLevel->number);
					}
				}
				else if (strcmp(theCurrentValue->nextLevel->type, "Object") == 0)
				{
					TIGValue *theNewCurrentValue = theCurrentValue->nextLevel->nextLevel;
					TIGValue *theNewValue = NULL;

					while (theNewCurrentValue != NULL)
					{
						theNewValue = TIGObjectAddStringWithValue(theNewValue, theNewCurrentValue, theNewCurrentValue->nextLevel);
						theNewCurrentValue = theNewCurrentValue->thisLevel;
					}
					
					return theNewValue;
				}
				else if (strcmp(theCurrentValue->nextLevel->type, "String") == 0)
				{
					return TIGStringInput(NULL, theCurrentValue->nextLevel->string);
				}
				else
				{
#ifdef TIG_DEBUG
					printf("ERROR Function:TIGObjectValueFromString() Variable:theCurrentValue->nextLevel->type Equals:%s\n", theCurrentValue->nextLevel->type);

	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
					return NULL;
				}
			}
		}
		else if (theCurrentValue == NULL)
		{
#ifdef TIG_DEBUG
			printf("ERROR Function:TIGObjectValueFromString() Variable:theCurrentValue Equals:NULL\n");

	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
			return NULL;
		}
	}
}

void TIGObjectRemoveValueFromString(TIGValue *tigObject, TIGValue *tigString)
{
	if (tigObject == NULL || tigString == NULL || strcmp(tigObject->type, "Object") != 0 || strcmp(tigString->type, "String") != 0)
	{
#ifdef TIG_DEBUG
		if (tigObject == NULL)
		{
			printf("ERROR Function:TIGObjectRemoveValueFromString() Variable:tigObject Equals:NULL\n");
		}
		else if (strcmp(tigObject->type, "Object") != 0)
		{
			printf("ERROR Function:TIGObjectRemoveValueFromString() Variable:tigObject->type Equals:%s Valid:\"Object\"\n", tigObject->type);
		}
		
		if (tigString == NULL)
		{
			printf("ERROR Function:TIGObjectRemoveValueFromString() Variable:tigString Equals:NULL\n");
		}
		else if (strcmp(tigString->type, "String") != 0)
		{
			printf("ERROR Function:TIGObjectRemoveValueFromString() Variable:tigString->type Equals:%s Valid:\"String\"\n", tigString->type);
		}
	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
		return;
	}

	TIGValue *theCurrentValue = tigObject;
	TIGValue *thePreviousValue = NULL;
	
	while (1)
	{
		thePreviousValue = theCurrentValue;
		theCurrentValue = theCurrentValue->thisLevel;
	
		if (theCurrentValue == NULL)
		{
#ifdef TIG_DEBUG
			printf("ERROR Function:TIGObjectRemoveValueFromString() Variable:theCurrentValue Equals:NULL\n");
	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
			return;
		}
		// Find and compare the name of the object
		else if (strcmp(theCurrentValue->string, tigString->string) == 0)
		{
			thePreviousValue->thisLevel = theCurrentValue->thisLevel;
			
			// Note to self I am using "->nextLevel" not "->thisLevel"
			if (strcmp(theCurrentValue->nextLevel->type, "Array") == 0)
			{
				TIGArrayDestroy(theCurrentValue->nextLevel);
			}
			else if (strcmp(theCurrentValue->nextLevel->type, "Number") == 0)
			{
				TIGNumberDestroy(theCurrentValue->nextLevel);
			}
			else if (strcmp(theCurrentValue->nextLevel->type, "Object") == 0)
			{
				TIGObjectDestroy(theCurrentValue->nextLevel);
			}
			else if (strcmp(theCurrentValue->nextLevel->type, "String") == 0)
			{
				TIGStringDestroy(theCurrentValue->nextLevel);
			}
			else
			{
#ifdef TIG_DEBUG
				printf("ERROR Function:TIGObjectRemoveValueFromString() Variable:theCurrentValue->nextLevel->type Equals:%s\n", theCurrentValue->nextLevel->type);
	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
				return;
			}
			
			// This removes the *tigString that points to the object value above
			TIGStringDestroy(theCurrentValue);
			
			return;
		}
	}
}

TIGInteger TIGObjectCount(TIGValue *tigObject)
{
	if (tigObject == NULL)
	{
		return 0;
	}
	
	TIGValue *theCurrentValue = tigObject;
	int index = -1;
	
	while (theCurrentValue != NULL)
	{
		theCurrentValue = theCurrentValue->thisLevel;
		index++;
	}
	
	return index;
}

void TIGObjectRemoveAllValues(TIGValue *tigObject)
{
	if (tigObject == NULL || tigObject->thisLevel == NULL || strcmp(tigObject->type, "Object") != 0)
	{
#ifdef TIG_DEBUG
		if (tigObject == NULL)
		{
			printf("ERROR Function:TIGObjectRemoveAllValues() Variable:tigObject Equals:NULL\n");
		}
		else
		{
			if (strcmp(tigObject->type, "Object") != 0)
			{
				printf("ERROR Function:TIGObjectRemoveAllValues() Variable:tigObject->type Equals:%s Valid:\"Object\"\n", tigObject->type);
			}
			
			if (tigObject->thisLevel == NULL)
			{
				printf("ERROR Function:TIGObjectRemoveAllValues() Variable:tigObject->thisLevel Equals:NULL\n");
			}
		}
	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
	}
	else
	{
		TIGValue *theCurrentValue = tigObject->thisLevel;
		
		while (theCurrentValue != NULL)
		{
			TIGValue *theNextValue = theCurrentValue->thisLevel;
			
			if (strcmp(theCurrentValue->type, "Array") == 0)
			{
				TIGArrayDestroy(theCurrentValue);
			}
			else if (strcmp(theCurrentValue->type, "Number") == 0)
			{
				TIGNumberDestroy(theCurrentValue);
			}
			else if (strcmp(theCurrentValue->type, "Object") == 0)
			{
				TIGObjectDestroy(theCurrentValue);
			}
			else if (strcmp(theCurrentValue->type, "String") == 0)
			{
				TIGStringDestroy(theCurrentValue);
			}
			
			theCurrentValue = theNextValue;
		}
		
		tigObject->thisLevel = NULL;
	}
}

TIGValue *TIGObjectFromString(TIGValue *tigString)
{
	if (tigString == NULL || strcmp(tigString->type, "String") != 0)
	{
#ifdef TIG_DEBUG
		if (tigString == NULL)
		{
			printf("ERROR Function:TIGObjectFromString() Variable:tigString Equals:NULL\n");
		}
		else if (strcmp(tigString->type, "String") != 0)
		{
			printf("ERROR Function:TIGObjectFromString() Variable:tigString->type Equals:%s Valid:\"String\"\n", tigString->type);
		}
	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
		return NULL;
	}

	TIGArrayStartStack(NULL);
	TIGStringStartStack(NULL);
	TIGNumberStartStack(NULL);

	TIGValue *theObject = TIGObjectFromStringWithIndex(tigString, 0);
	
	TIGNumberEndStack(NULL);
	TIGStringEndStack(NULL);
	TIGArrayEndStack(NULL);

	return theObject;
}

TIGValue *TIGObjectFromStringWithIndex(TIGValue *tigString, int startIndex)
{
	char *string = tigString->string;
	TIGValue *theNewValue = NULL;
	static int endIndex = 0;
	TIGBool isTIGArray = TIGNo;
	TIGBool isTIGNumber = TIGNo;
	TIGBool isTIGObject = TIGNo;
	TIGBool isTIGString = TIGNo;
	int startString = -1;
	int endString = -1;
	int i;
	// Start with { } then " " strings then [ ] arrays and 0 to 9 numbers
	for (i = startIndex; i < strlen(string); i++)
	{
		if (string[i] == '[')
		{
			startIndex = i + 1;
			isTIGArray = TIGYes;
			break;
		}
		else if (string[i] == 't')
		{
			theNewValue = TIGNumberBooleanInput(theNewValue, TIGYes);
			
			return theNewValue;
		}
		else if (string[i] == 'f')
		{
			theNewValue = TIGNumberBooleanInput(theNewValue, TIGNo);
			
			return theNewValue;
		}
		else if (string[i] == '-' || string[i] == '.' || string[i] == '0'|| string[i] == '1' || string[i] == '2'|| string[i] == '3'
			|| string[i] == '4' || string[i] == '5' || string[i] == '6'|| string[i] == '7' || string[i] == '8'|| string[i] == '9')
		{
			startIndex = i;
			isTIGNumber = TIGYes;
			break;
		}
		else if (string[i] == '{')
		{
			startIndex = i + 1;
			isTIGObject = TIGYes;
			break;
		}
		else if (string[i] != '\\' && string[i + 1] == '"')
		{
			startIndex = i;
			isTIGString = TIGYes;
			break;
		}
		else if (string[i] == ']')
		{
			endIndex = i;
			return NULL;
		}
	}
	
	if (isTIGArray)
	{
		for (i = startIndex; i < strlen(string); i++)
		{
			TIGValue *theObjectValue = TIGObjectFromStringWithIndex(tigString, i);
			
			if (theObjectValue == NULL)
			{
				if (string[endIndex] == ']')
				{
					endIndex = i + 1;
					return theNewValue;
				}
				else
				{
#ifdef TIG_DEBUG
					printf("ERROR Function:TIGObjectFromStringWithIndex() Variable:string[endIndex] Equals:Missing a \"]\"\n");
	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
					return NULL;
				}
			}
			else
			{
				theNewValue = TIGArrayAddValue(theNewValue, theObjectValue);
				i = endIndex;
			}
		}
	}
	else if (isTIGNumber)
	{
		for (i = startIndex; i < strlen(string); i++)
		{
			if (string[i] == '-' || string[i] == '.' || string[i] == '0'|| string[i] == '1' || string[i] == '2'|| string[i] == '3'
				|| string[i] == '4' || string[i] == '5' || string[i] == '6'|| string[i] == '7' || string[i] == '8'|| string[i] == '9')
			{
				if (startString == -1)
				{
					startString = i;
				}
			}
			else if (startString > -1)
			{
				endString = i - 1;
				
				char *newString = (char *)malloc((endString - startString + 1) * sizeof(char));
				
				if (newString != NULL)
				{
					int stringIndex = 0;
					int j;
					//printf("start: %d end: %d\n", startString, endString);
					
					for (j = startString; j <= endString; j++)
					{
						newString[stringIndex] = string[j];
						stringIndex++;
					}
					
					theNewValue = TIGNumberInput(theNewValue, atof(newString));
					//printf("Number Start: %d End: %d String: %s\n", startString, endString, newString);
					
					free(newString);
					newString = NULL;
					
					endIndex = i;
					return theNewValue;
				}
				else
				{
#ifdef TIG_DEBUG
					printf("ERROR1 Function:TIGObjectFromStringWithIndex() Variable:newString Equals:NULL\n");
	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
					return NULL;
				}
			}
		}
	}
	else if (isTIGObject)
	{
		for (i = startIndex; i < strlen(string); i++)
		{
			if (string[i] != '\\' && string[i + 1] == '"')
			{
				if (startString == -1)
				{
					startString = i + 2;
				}
				else if (endString == -1)
				{
					endString = i;
				}
			}
			
			if (startString > -1 && endString > -1 && endString - startString > 0)
			{
				char *newString = (char *)malloc((endString - startString + 1) * sizeof(char));
				
				if (newString != NULL)
				{
					int stringIndex = 0;
					int j;
					//printf("start: %d end: %d\n", startString, endString);
					
					for (j = startString; j <= endString; j++)
					{
						newString[stringIndex] = string[j];
						stringIndex++;
					}
					
					TIGValue *theObjectValue = TIGObjectFromStringWithIndex(tigString, i + 2);
					TIGValue *theString = TIGStringRemoveEscapeCharacters(TIGStringInput(NULL, newString));
					theNewValue = TIGObjectAddStringWithValue(theNewValue, theString, theObjectValue);
					//printf("Object Start: %d End: %d String: %s\n", startString, endString, newString);
				
					i = endIndex;
				
					free(newString);
					newString = NULL;
				}
				else
				{
#ifdef TIG_DEBUG
					printf("ERROR2 Function:TIGObjectFromStringWithIndex() Variable:newString Equals:NULL\n");
	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
					return NULL;
				}
				
				startString = -1;
				endString = -1;
			}
		
			if (string[i] == '}' && startString == -1 && endString == -1)
			{
				//printf("isTIGObject2 tigIndex %d\n", i);
				endIndex = i + 1;
				return theNewValue;
			}
		}
	}
	else if (isTIGString)
	{
		for (i = startIndex; i < strlen(string); i++)
		{
			if (string[i] != '\\' && string[i + 1] == '"')
			{
				if (startString == -1)
				{
					startString = i + 2;
				}
				else if (endString == -1)
				{
					endString = i;
				}
			}
			
			if (startString > -1 && endString > -1 && endString - startString > 0)
			{
				char *newString = (char *)malloc((endString - startString + 1) * sizeof(char));
				
				if (newString != NULL)
				{
					//printf("start: %d end: %d\n", startString, endString);
					int stringIndex = 0;
					int j;
					
					for (j = startString; j <= endString; j++)
					{
						newString[stringIndex] = string[j];
						stringIndex++;
					}
					
					theNewValue = TIGStringRemoveEscapeCharacters(TIGStringInput(theNewValue, newString));
					
					free(newString);
					newString = NULL;
					
					endIndex = i + 2;
					return theNewValue;
				}
				else
				{
#ifdef TIG_DEBUG
					printf("ERROR3 Function:TIGObjectFromStringWithIndex() Variable:newString Equals:NULL\n");
	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
					return NULL;
				}
			}
		}
	}
	
#ifdef TIG_DEBUG
	printf("ERROR Function:TIGObjectFromStringWithIndex() Variable:tigString Equals:invalid tigString\n");
	
	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
	return NULL;
}

TIGValue *TIGObjectAddValuesFromObject(TIGValue *tigOldObject, TIGValue *tigNewObject)
{
	if (tigOldObject == NULL || tigNewObject == NULL || strcmp(tigOldObject->type, "Object") != 0 || strcmp(tigNewObject->type, "Object") != 0)
	{
#ifdef TIG_DEBUG
		if (tigOldObject == NULL)
		{
			printf("ERROR Function:TIGObjectAddValuesFromObject() Variable:tigOldObject Equals:NULL\n");
		}
		else if (strcmp(tigOldObject->type, "Object") != 0)
		{
			printf("ERROR Function:TIGObjectAddValuesFromObject() Variable:tigOldObject->type Equals:%s Valid:\"Object\"\n", tigOldObject->type);
		}
		
		if (tigNewObject == NULL)
		{
			printf("ERROR Function:TIGObjectAddValuesFromObject() Variable:tigNewObject Equals:NULL\n");
		}
		else if (strcmp(tigNewObject->type, "Object") != 0)
		{
			printf("ERROR Function:TIGObjectAddValuesFromObject() Variable:tigNewObject->type Equals:%s Valid:\"Object\"\n", tigNewObject->type);
		}

	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
		return NULL;
	}
	
	TIGArrayStartStack(NULL);
	TIGStringStartStack(NULL);
	TIGNumberStartStack(NULL);

	TIGValue *theNewArray = TIGArrayOfObjectStrings(tigNewObject);
	int i;
	
	for (i = 0; i < TIGArrayCount(theNewArray); i++)
	{
		TIGValue *theString = TIGArrayValueAtIndex( theNewArray, i);
		TIGValue *theValue = TIGObjectValueFromString(tigNewObject, theString);
		
		tigOldObject = TIGObjectAddStringWithValue(tigOldObject, theString, theValue);
	}
	
	TIGNumberEndStack(NULL);
	TIGStringEndStack(NULL);
	TIGArrayEndStack(NULL);
	
	return tigOldObject;
}

TIGBool TIGObjectEqualsObject(TIGValue *tigObject1, TIGValue *tigObject2)
{
	if (tigObject1 == NULL || tigObject2 == NULL || strcmp(tigObject1->type, "Object") != 0 || strcmp(tigObject2->type, "Object") != 0)
	{
#ifdef TIG_DEBUG
		if (tigObject1 == NULL)
		{
			printf("ERROR Function:TIGObjectEqualsObject() Variable:tigObject1 Equals:NULL\n");
		}
		else if (strcmp(tigObject1->type, "Object") != 0)
		{
			printf("ERROR Function:TIGObjectEqualsObject() Variable:tigObject1->type Equals:%s Valid:\"Object\"\n", tigObject1->type);
		}
		
		if (tigObject2 == NULL)
		{
			printf("ERROR Function:TIGObjectEqualsObject() Variable:tigObject2 Equals:NULL\n");
		}
		else if (strcmp(tigObject2->type, "Object") != 0)
		{
			printf("ERROR Function:TIGObjectEqualsObject() Variable:tigObject2->type Equals:%s Valid:\"Object\"\n", tigObject2->type);
		}
	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
		return TIGNo;
	}
	
	TIGStringStartStack(NULL);
		TIGBool equalTo = TIGStringEqualsString(TIGStringFromObjectForNetwork(tigObject1), TIGStringFromObjectForNetwork(tigObject2));
	TIGStringEndStack(NULL);
	
	return equalTo;
}
