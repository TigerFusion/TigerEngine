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
TIGValue *TIGArrayNumberStack(void)
{
	return theNumberStack;
}

TIGValue *TIGArrayStringStack(void)
{
	return theStringStack;
}
// ************* For debugging Purposes *************

void TIGArrayStartStack(const char *startStackString)
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

void TIGArrayEndStack(const char *endStackString)
{
	if (endStackString != NULL)
	{
		while (theStringStack != NULL)
		{
			TIGValue *theNextStack = theStringStack->nextStack;
			
			// 0 means both strings are the same
			if (strcmp(theStringStack->stackString, endStackString) == 0)
			{
				theStringStack = TIGArrayDestroy(theStringStack);
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
				theNumberStack = TIGArrayDestroy(theNumberStack);
			}
			
			theNumberStack = theNextStack;
		}
	}
	
	// free the string used to end the stack
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

TIGValue *TIGArrayCreate(TIGValue *tigArray, TIGBool useStack)
{
	tigArray = (TIGValue *)malloc(1 * sizeof(TIGValue));
	
	if (tigArray == NULL)
	{
#ifdef TIG_DEBUG
		printf("ERROR Function:TIGArrayCreate() Variable:tigArray Equals:NULL\n");
#endif
		return NULL;
	}
	
	if (useStack)
	{
		if (stackString != NULL)
		{
			if (theStringStack == NULL)
			{
				tigArray->nextStack = NULL;
			}
			// Add the last added TIGArray to the new tigArray's ->nextStack
			else
			{
				tigArray->nextStack = theStringStack;
			}
			
			tigArray->stackNumber = -1;
			tigArray->stackString = (char *)malloc((strlen(stackString) + 1) * sizeof(char));
			
			if (tigArray->stackString != NULL)
			{
				strcpy(tigArray->stackString, stackString);
			}
			else
			{
#ifdef TIG_DEBUG
				printf("ERROR Function:TIGArrayCreate() Variable:tigArray->stackString Equals:NULL\n");
#endif
			}
			
			// This adds the tigArray to the global TIGArray stack
			theStringStack = tigArray;
		}
		else
		{
			if (theNumberStack == NULL)
			{
				tigArray->nextStack = NULL;
			}
			// Add the last added TIGArray to the new tigArray's ->nextStack
			else
			{
				tigArray->nextStack = theNumberStack;
			}
			
			tigArray->stackNumber = stackNumber;
			tigArray->stackString = NULL;
			
			// This adds the tigArray to the global TIGArray stack
			theNumberStack = tigArray;
		}
	}
	else
	{
		tigArray->nextStack = NULL;
		
		tigArray->stackString = NULL;
		tigArray->stackNumber = -2;
	}

	tigArray->nextLevel = NULL;
	tigArray->thisLevel = NULL;
	
	// Sets the TIGObject's string to an empty string
	tigArray->string = NULL;
	
	// object type
	tigArray->type = "Array";
	
	//printf("%d", (*tigArray) != NULL);
	return tigArray;
}

TIGValue *TIGArrayDestroy(TIGValue *tigArray)
{
	// If the "tigArray" pointer has already been used free it
	if (tigArray != NULL)
	{
		TIGValue *theCurrentValue = tigArray->nextLevel;
		
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
		
		if (tigArray->string != NULL)
		{
			free(tigArray->string);
			tigArray->string = NULL;
		}
		
		if (tigArray->stackString != NULL)
		{
			free(tigArray->stackString);
			tigArray->stackString = NULL;
		}

		tigArray->number = 0.0;
		tigArray->stackNumber = -1;
		tigArray->type = NULL;
		
		tigArray->nextStack = NULL;
		tigArray->nextLevel = NULL;
		tigArray->thisLevel = NULL;
		
		free(tigArray);
		tigArray = NULL;
	}
	
	return tigArray;
}

TIGValue *TIGArr(TIGValue *tigValue)
{
	return TIGArrayAddValue(NULL, tigValue);
}

TIGValue *TIGArrayAddValue(TIGValue *tigArray, TIGValue *tigValue)
{
	return TIGArrayStackInsertValueAtIndex(tigArray, tigValue, TIGArrayCount(tigArray), TIGYes);
}

TIGValue *TIGArrayInsertValueAtIndex(TIGValue *tigArray, TIGValue *tigValue, int tigIndex)
{
	return TIGArrayStackInsertValueAtIndex(tigArray, tigValue, tigIndex, TIGYes);
}

TIGValue *TIGArrayStackInsertValueAtIndex(TIGValue *tigArray, TIGValue *tigValue, int tigIndex, TIGBool useStack)
{
	if (tigArray != NULL && (tigIndex < 0 || tigIndex > TIGArrayCount(tigArray)))
	{
#ifdef TIG_DEBUG
		printf("ERROR Function:TIGArrayStackInsertValueAtIndex() Variable:tigIndex Equals:%d Valid:0 to %d\n", tigIndex, TIGArrayCount(tigArray));
#endif
		return NULL;
	}
	
	if (tigArray == NULL)
	{
		tigArray = TIGArrayCreate(tigArray, useStack);
		
		if (tigArray == NULL)
		{
#ifdef TIG_DEBUG
			printf("ERROR Function:TIGArrayStackInsertValueAtIndex() Variable:tigArray Equals:NULL\n");
#endif
			return NULL;
		}
	}
	
	if (tigValue == NULL || strcmp(tigArray->type, "Array") != 0)
	{
#ifdef TIG_DEBUG
		if (strcmp(tigArray->type, "Array") != 0)
		{
			printf("ERROR Function:TIGArrayStackInsertValueAtIndex() Variable:tigArray->type Equals:%s Valid:\"Array\"\n", tigArray->type);
		}
		
		if (tigValue == NULL)
		{
			printf("ERROR Function:TIGArrayStackInsertValueAtIndex() Variable:tigValue Equals:NULL\n");
		}
#endif
		return tigArray;
	}
	else
	{
		TIGValue *tigNewValue = NULL;
		
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
				printf("ERROR1 Function:TIGArrayStackInsertValueAtIndex() Variable:tigNewValue Equals:NULL\n");
#endif
				return tigArray;
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
				printf("ERROR2 Function:TIGArrayStackInsertValueAtIndex() Variable:tigNewValue Equals:NULL\n");
#endif
				return tigArray;
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
			printf("ERROR3 Function:TIGArrayStackInsertValueAtIndex() Variable:tigNewValue Equals:NULL\n");
#endif
			return tigArray;
		}
		
		TIGValue *theCurrentValue = tigArray;
		int index = 0;
		
		while (1)
		{
			if (tigIndex == index)
			{
				if (theCurrentValue->thisLevel == NULL)
				{
					theCurrentValue->thisLevel = tigNewValue;
				}
				else
				{
					TIGValue *theNextValue = theCurrentValue->thisLevel;
					TIGValue *thePreviousValue = theCurrentValue;
					
					thePreviousValue->thisLevel = tigNewValue;
					tigNewValue->thisLevel = theNextValue;
				}
				
				break;
			}
			else if (theCurrentValue->thisLevel != NULL)
			{
				theCurrentValue = theCurrentValue->thisLevel;
			}
			else
			{
#ifdef TIG_DEBUG
				printf("ERROR Function:TIGArrayStackInsertValueAtIndex() Variable:index Equals:%d Valid:0 to %d\n", index, TIGArrayCount(tigArray));
#endif
				return tigArray;
			}
			
			index++;
		}
	}
	
	return tigArray;
}

TIGValue *TIGArrayValueAtIndex(TIGValue *tigArray, int tigArrayIndex)
{
	if (tigArray == NULL || strcmp(tigArray->type, "Array") != 0 || tigArrayIndex < 0 || tigArrayIndex >= TIGArrayCount(tigArray))
	{
#ifdef TIG_DEBUG
		if (tigArray == NULL)
		{
			printf("ERROR Function:TIGArrayValueAtIndex() Variable:tigArray Equals:NULL\n");
		}
		else
		{
			if (strcmp(tigArray->type, "Array") != 0)
			{
				printf("ERROR Function:TIGArrayValueAtIndex() Variable:tigArray->type Equals:%s Valid:\"Array\"\n", tigArray->type);
			}
			else if (tigArrayIndex < 0 || tigArrayIndex >= TIGArrayCount(tigArray))
			{
				printf("ERROR Function:TIGArrayValueAtIndex() Variable:tigArrayIndex Equals:%d Valid:0 to %d\n", tigArrayIndex, TIGArrayCount(tigArray) - 1);
			}
		}
#endif
		return NULL;
	}

	TIGValue *theCurrentValue = tigArray;
	int index = 0;
	
	while (1)
	{
		if (tigArrayIndex == index)
		{
			if (theCurrentValue->thisLevel != NULL)
			{
				TIGValue *theTigValue = theCurrentValue->thisLevel;
				
				if (strcmp(theTigValue->type, "Array") == 0)
				{
					TIGValue *theNewCurrentValue = theTigValue->nextLevel;
					TIGValue *theNewValue = NULL;
					
					while (theNewCurrentValue != NULL)
					{
						theNewValue = TIGArrayAddValue(theNewValue, theNewCurrentValue);
						theNewCurrentValue = theNewCurrentValue->thisLevel;
					}
					
					return theNewValue;
				}
				else if (strcmp(theTigValue->type, "Number") == 0)
				{
					if (theTigValue->string != NULL && strcmp(theTigValue->string, "false") == 0)
					{
						return TIGNumberBooleanInput(NULL, TIGNo);
					}
					else if (theTigValue->string != NULL && strcmp(theTigValue->string, "true") == 0)
					{
						return TIGNumberBooleanInput(NULL, TIGYes);
					}
					else
					{
						return TIGNumberInput(NULL, theTigValue->number);
					}
				}
				else if (strcmp(theTigValue->type, "Object") == 0)
				{
					TIGValue *theNewCurrentValue = theTigValue->nextLevel;
					TIGValue *theNewValue = NULL;

					while (theNewCurrentValue != NULL)
					{
						theNewValue = TIGObjectAddStringWithValue(theNewValue, theNewCurrentValue, theNewCurrentValue->nextLevel);
						theNewCurrentValue = theNewCurrentValue->thisLevel;
					}
					
					return theNewValue;
				}
				else if (strcmp(theTigValue->type, "String") == 0)
				{
					if (theTigValue->string != NULL)
					{
						return TIGStringInput(NULL, theTigValue->string);
					}
				}
			}
#ifdef TIG_DEBUG
			printf("ERROR1 Function:TIGArrayValueAtIndex() Variable:theCurrentValue->thisLevel Equals:NULL\n");
#endif
			return NULL;
		}
		else if (theCurrentValue->thisLevel != NULL)
		{
			theCurrentValue = theCurrentValue->thisLevel;
		}
		else
		{
#ifdef TIG_DEBUG
			printf("ERROR2 Function:TIGArrayValueAtIndex() Variable:theCurrentValue->thisLevel Equals:NULL\n");
#endif
			return NULL;
		}
		
		index++;
	}
}

void TIGArrayRemoveValueAtIndex(TIGValue *tigArray, int tigArrayIndex)
{
	if (tigArray == NULL || strcmp(tigArray->type, "Array") != 0 || tigArrayIndex < 0 || tigArrayIndex >= TIGArrayCount(tigArray))
	{
#ifdef TIG_DEBUG
		if (tigArray == NULL)
		{
			printf("ERROR Function:TIGArrayRemoveValueAtIndex() Variable:tigArray Equals:NULL\n");
		}
		else
		{
			if (strcmp(tigArray->type, "Array") != 0)
			{
				printf("ERROR Function:TIGArrayRemoveValueAtIndex() Variable:tigArray->type Equals:%s Valid:\"Array\"\n", tigArray->type);
			}
			else if (tigArrayIndex < 0 || tigArrayIndex >= TIGArrayCount(tigArray))
			{
				printf("ERROR Function:TIGArrayRemoveValueAtIndex() Variable:tigArrayIndex Equals:%d Valid:0 to %d\n", tigArrayIndex, TIGArrayCount(tigArray) - 1);
			}
		}
#endif
	}
	else
	{
		TIGValue *theCurrentValue = tigArray;
		int index = 0;
		
		while (1)
		{
			if (tigArrayIndex == index)
			{
				TIGValue *thePreviousValue = theCurrentValue;
				theCurrentValue = theCurrentValue->thisLevel;
				
				// Remove the object at index from the linked list
				thePreviousValue->thisLevel = theCurrentValue->thisLevel;
				
				// Destroy the old object at index
				if (strcmp(theCurrentValue->type, "Array") == 0)
				{
					TIGArrayDestroy(theCurrentValue);
					return;
				}
				else if (strcmp(theCurrentValue->type, "Number") == 0)
				{
					TIGNumberDestroy(theCurrentValue);
					return;
				}
				else if (strcmp(theCurrentValue->type, "Object") == 0)
				{
					TIGObjectDestroy(theCurrentValue);
					return;
				}
				else if (strcmp(theCurrentValue->type, "String") == 0)
				{
					TIGStringDestroy(theCurrentValue);
					return;
				}
				else
				{
#ifdef TIG_DEBUG
					printf("ERROR Function:TIGArrayRemoveValueAtIndex() Variable:theCurrentValue->type Equals:%s\n", theCurrentValue->type);
#endif
					return;
				}
			}
			else if (theCurrentValue->thisLevel != NULL)
			{
				theCurrentValue = theCurrentValue->thisLevel;
			}
			else
			{
#ifdef TIG_DEBUG
				printf("ERROR Function:TIGArrayRemoveValueAtIndex() Variable:theCurrentValue->thisLevel Equals:NULL\n");
#endif
				return;
			}
			
			index++;
		}
	}
}

int TIGArrayCount(TIGValue *tigArray)
{
	if (tigArray == NULL)
	{
		return 0;
	}
	
	TIGValue *theCurrentValue = tigArray;
	int index = -1;
	
	while (theCurrentValue != NULL)
	{
		theCurrentValue = theCurrentValue->thisLevel;
		index++;
	}
	
	return index;
}

void TIGArrayRemoveAllValues(TIGValue *tigArray)
{
	if (tigArray == NULL || strcmp(tigArray->type, "Array") != 0 || tigArray->thisLevel == NULL)
	{
#ifdef TIG_DEBUG
		if (tigArray == NULL)
		{
			printf("ERROR Function:TIGArrayRemoveAllValues() Variable:tigArray Equals:NULL\n");
		}
		else
		{
			if (strcmp(tigArray->type, "Array") != 0)
			{
				printf("ERROR Function:TIGArrayRemoveAllValues() Variable:tigArray->type Equals:%s Valid:\"Array\"\n", tigArray->type);
			}

			if (tigArray->thisLevel == NULL)
			{
				printf("ERROR Function:TIGArrayRemoveAllValues() Variable:tigArray->thisLevel Equals:NULL\n");
			}
		}
#endif
	}
	else
	{
		TIGValue *theCurrentValue = tigArray->thisLevel;
		
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
		
		tigArray->thisLevel = NULL;
	}
}

TIGValue *TIGArrayReplaceValueAtIndex(TIGValue *tigArray, TIGValue *tigValue, int tigArrayIndex)
{
	if (tigArray == NULL || tigValue == NULL || strcmp(tigArray->type, "Array") != 0 || tigArrayIndex < 0 || tigArrayIndex >= TIGArrayCount(tigArray))
	{
#ifdef TIG_DEBUG

		if (tigArray == NULL)
		{
			printf("ERROR Function:TIGArrayReplaceValueAtIndex() Variable:tigArray Equals:NULL\n");
		}
		else
		{
			if (strcmp(tigArray->type, "Array") != 0)
			{
				printf("ERROR Function:TIGArrayReplaceValueAtIndex() Variable:tigArray->type Equals:%s Valid:\"Array\"\n", tigArray->type);
			}
			else if (tigArrayIndex < 0 || tigArrayIndex >= TIGArrayCount(tigArray))
			{
				printf("ERROR Function:TIGArrayReplaceValueAtIndex() Variable:tigArrayIndex Equals:%d Valid:0 to %d\n", tigArrayIndex, TIGArrayCount(tigArray) - 1);
			}
		}
		
		if (tigValue == NULL)
		{
			printf("ERROR Function:TIGArrayReplaceValueAtIndex() Variable:tigValue Equals:NULL\n");
		}
#endif
		return NULL;
	}
	
	TIGArrayRemoveValueAtIndex(tigArray, tigArrayIndex);
	tigArray = TIGArrayInsertValueAtIndex(tigArray, tigValue, tigArrayIndex);

	return tigArray;
}

TIGValue *TIGArrayOfObjectStrings(TIGValue *tigObject)
{
	if (tigObject == NULL || strcmp(tigObject->type, "Object") != 0)
	{
#ifdef TIG_DEBUG
		if (tigObject == NULL)
		{
			printf("ERROR Function:TIGArrayOfObjectStrings() Variable:tigArrayIndex Equals:NULL\n");
		}
		else if (strcmp(tigObject->type, "Object") != 0)
		{
			printf("ERROR Function:TIGArrayOfObjectStrings() Variable:tigObject->type Equals:%s Valid:\"Object\"\n", tigObject->type);
		}
#endif
		return NULL;
	}

	TIGValue *tigArray = NULL;
	TIGValue *theCurrentValue = tigObject;
	
	while (1)
	{
		theCurrentValue = theCurrentValue->thisLevel;
		
		if (theCurrentValue == NULL)
		{
			break;
		}
		else
		{
			tigArray = TIGArrayAddValue(tigArray, theCurrentValue);
		}
	}
	
	return tigArray;
}

TIGValue *TIGArrayOfObjectValues(TIGValue *tigObject)
{
	if (tigObject == NULL || strcmp(tigObject->type, "Object") != 0)
	{
#ifdef TIG_DEBUG
		if (tigObject == NULL)
		{
			printf("ERROR Function:TIGArrayOfObjectValues() Variable:tigArrayIndex Equals:NULL\n");
		}
		else if (strcmp(tigObject->type, "Object") != 0)
		{
			printf("ERROR Function:TIGArrayOfObjectValues() Variable:tigObject->type Equals:%s Valid:\"Object\"\n", tigObject->type);
		}
#endif
		return NULL;
	}

	TIGValue *tigArray = NULL;
	TIGValue *theCurrentValue = tigObject;
	
	while (1)
	{
		theCurrentValue = theCurrentValue->thisLevel;
		
		if (theCurrentValue == NULL)
		{
			break;
		}
		else
		{
			tigArray = TIGArrayAddValue(tigArray, theCurrentValue->nextLevel);
		}
	}
	
	return tigArray;
}

TIGValue *TIGArrayInsertValuesFromArrayAtIndex(TIGValue *tigOldArray, TIGValue *tigNewArray, int index)
{
	if (tigOldArray == NULL || tigNewArray == NULL || strcmp(tigOldArray->type, "Array") != 0 || strcmp(tigNewArray->type, "Array") != 0
		|| index < 0 || index > TIGArrayCount(tigOldArray))
	{
#ifdef TIG_DEBUG
		if (tigOldArray == NULL)
		{
			printf("ERROR Function:TIGArrayInsertValuesFromArrayAtIndex() Variable:tigOldArray Equals:NULL\n");
		}
		else
		{
			if (strcmp(tigOldArray->type, "Array") != 0)
			{
				printf("ERROR Function:TIGArrayInsertValuesFromArrayAtIndex() Variable:tigOldArray->type Equals:%s Valid:\"Array\"\n", tigOldArray->type);
			}
			else if (index < 0 || index > TIGArrayCount(tigOldArray))
			{
				printf("ERROR Function:TIGArrayInsertValuesFromArrayAtIndex() Variable:index Equals:%d Valid:0 to %d\n", index, TIGArrayCount(tigOldArray));
			}
		}
		
		if (tigNewArray == NULL)
		{
			printf("ERROR Function:TIGArrayInsertValuesFromArrayAtIndex() Variable:tigNewArray Equals:NULL\n");
		}
		else if (strcmp(tigNewArray->type, "Array") != 0)
		{
			printf("ERROR Function:TIGArrayInsertValuesFromArrayAtIndex() Variable:tigNewArray->type Equals:%s Valid:\"Array\"\n", tigNewArray->type);
		}
#endif
		return NULL;
	}

	TIGObjectStartStack(NULL);
	TIGStringStartStack(NULL);
	TIGNumberStartStack(NULL);

	// total is needed because the array count changes once the for() statement starts
	int total = TIGArrayCount(tigNewArray) + index;

	for (int i = index; i < total; i++)
	{
		TIGValue *theValue = TIGArrayValueAtIndex(tigNewArray, i - index);
		tigOldArray = TIGArrayInsertValueAtIndex(tigOldArray, theValue, i);
	}
	
	TIGNumberEndStack(NULL);
	TIGStringEndStack(NULL);
	TIGObjectEndStack(NULL);
	
	return tigOldArray;
}

TIGValue *TIGArrayAddValuesFromArray(TIGValue *tigOldArray, TIGValue *tigNewArray)
{
	return TIGArrayInsertValuesFromArrayAtIndex(tigOldArray, tigNewArray, TIGArrayCount(tigOldArray));
}

TIGValue *TIGArrayFirstValue(TIGValue *tigArray)
{
	if (tigArray == NULL || strcmp(tigArray->type, "Array") != 0 || TIGArrayCount(tigArray) == 0)
	{
#ifdef TIG_DEBUG
		if (tigArray == NULL)
		{
			printf("ERROR Function:TIGArrayFirstValue() Variable:tigArray Equals:NULL\n");
		}
		else
		{
			if (strcmp(tigArray->type, "Array") != 0)
			{
				printf("ERROR Function:TIGArrayFirstValue() Variable:tigArray->type Equals:%s Valid:\"Array\"\n", tigArray->type);
			}
			else if (TIGArrayCount(tigArray) == 0)
			{
				printf("ERROR Function:TIGArrayFirstValue() Variable:TIGArrayCount(tigArray) Equals:0 Valid:1 or more\n");
			}

		}
#endif
		return NULL;
	}

	return TIGArrayValueAtIndex(tigArray, 0);
}

TIGValue *TIGArrayLastValue(TIGValue *tigArray)
{
	if (tigArray == NULL || strcmp(tigArray->type, "Array") != 0 || TIGArrayCount(tigArray) == 0)
	{
#ifdef TIG_DEBUG
		if (tigArray == NULL)
		{
			printf("ERROR Function:TIGArrayLastValue() Variable:tigArray Equals:NULL\n");
		}
		else
		{
			if (strcmp(tigArray->type, "Array") != 0)
			{
				printf("ERROR Function:TIGArrayLastValue() Variable:tigArray->type Equals:%s Valid:\"Array\"\n", tigArray->type);
			}
			else if (TIGArrayCount(tigArray) == 0)
			{
				printf("ERROR Function:TIGArrayLastValue() Variable:TIGArrayCount(tigArray) Equals:0 Valid:1 or more\n");
			}
		}
#endif
		return NULL;
	}
	
	return TIGArrayValueAtIndex(tigArray, TIGArrayCount(tigArray) - 1);
}

TIGBool TIGArrayEqualsArray(TIGValue *tigArray1, TIGValue *tigArray2)
{
	if (tigArray1 == NULL || tigArray2 == NULL || strcmp(tigArray1->type, "Array") != 0 || strcmp(tigArray2->type, "Array") != 0)
	{
#ifdef TIG_DEBUG
		if (tigArray1 == NULL)
		{
			printf("ERROR Function:TIGArrayEqualsArray() Variable:tigArray1 Equals:NULL\n");
		}
		else if (strcmp(tigArray1->type, "Array") != 0)
		{
			printf("ERROR Function:TIGArrayEqualsArray() Variable:tigArray1->type Equals:%s Valid:\"Array\"\n", tigArray1->type);
		}
		
		if (tigArray2 == NULL)
		{
			printf("ERROR Function:TIGArrayEqualsArray() Variable:tigArray2 Equals:NULL\n");
		}
		else if (strcmp(tigArray2->type, "Array") != 0)
		{
			printf("ERROR Function:TIGArrayEqualsArray() Variable:tigArray2->type Equals:%s Valid:\"Array\"\n", tigArray2->type);
		}
#endif
		return TIGNo;
	}

	TIGStringStartStack(NULL);
		TIGBool equalTo = TIGStringEqualsString(TIGStringFromArrayForNetwork(tigArray1), TIGStringFromArrayForNetwork(tigArray2));
	TIGStringEndStack(NULL);
	
	return equalTo;
}
