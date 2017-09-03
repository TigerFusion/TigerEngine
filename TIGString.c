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
TIGValue *TIGStringNumberStack(void)
{
	return theNumberStack;
}

TIGValue *TIGStringStringStack(void)
{
	return theStringStack;
}
// ************* For debugging Purposes *************

void TIGStringStartStack(const char *startStackString)
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

void TIGStringEndStack(const char *endStackString)
{
	if (endStackString != NULL)
	{
		while (theStringStack != NULL)
		{
			TIGValue *theNextStack = theStringStack->nextStack;
			
			// 0 means both strings are the same
			if (strcmp(theStringStack->stackString, endStackString) == 0)
			{
				theStringStack = TIGStringDestroy(theStringStack);
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
				theNumberStack = TIGStringDestroy(theNumberStack);
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

TIGValue *TIGStringCreate(TIGValue *tigString, TIGBool useStack)
{
	tigString = (TIGValue *)malloc(1 * sizeof(TIGValue));
	
	if (tigString == NULL)
	{
#ifdef TIG_DEBUG
		printf("ERROR Function:TIGStringCreate() Variable:tigString Equals:NULL\n");

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
				tigString->nextStack = NULL;
			}
			// Add the last added TIGString to the new tigString's ->nextStack
			else
			{
				tigString->nextStack = theStringStack;
			}
			
			tigString->stackNumber = -1;
			tigString->stackString = (char *)malloc((strlen(stackString) + 1) * sizeof(char));
			
			if (tigString->stackString != NULL)
			{
				strcpy(tigString->stackString, stackString);
			}
			else
			{
#ifdef TIG_DEBUG
				printf("ERROR Function:TIGStringCreate() Variable:tigString->stackString Equals:NULL\n");

	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
			}
			
			// This adds the new tigString to the global TIGString stack
			theStringStack = tigString;
		}
		else
		{
			if (theNumberStack == NULL)
			{
				tigString->nextStack = NULL;
			}
			// Add the last added TIGString to the new tigString's ->nextStack
			else
			{
				tigString->nextStack = theNumberStack;
			}
			
			tigString->stackNumber = stackNumber;
			tigString->stackString = NULL;
			
			// This adds the tigString to the global TIGString stack
			theNumberStack = tigString;
		}
	}
	else
	{
		tigString->nextStack = NULL;
		
		tigString->stackString = NULL;
		tigString->stackNumber = -2;
	}
	
	tigString->nextLevel = NULL;
	tigString->thisLevel = NULL;
	
	tigString->number = 0.0;
	
	// Sets the TIGObject's string to an empty string
	tigString->string = NULL;
	
	// object type
	tigString->type = "String";
	
	return tigString;
}

TIGValue *TIGStringDestroy(TIGValue *tigString)
{
	// If the "tigString" pointer has already been used free it
	if (tigString != NULL)
	{		
		if (strcmp(tigString->type, "String") != 0)
		{
#ifdef TIG_DEBUG
			printf("ERROR Function:TIGStringDestroy() Variable:tigString->type Equals:%s Valid:\"String\"\n", tigString->type);
	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
			return tigString;
		}

		if (tigString->string != NULL)
		{
			free(tigString->string);
			tigString->string = NULL;
		}
		
		if (tigString->stackString != NULL)
		{
			free(tigString->stackString);
			tigString->stackString = NULL;
		}

		tigString->number = 0.0;
		tigString->stackNumber = 0;
		
		tigString->type = NULL;
		
		tigString->nextStack = NULL;
		tigString->nextLevel = NULL;
		tigString->thisLevel = NULL;
		
		free(tigString);
		tigString = NULL;
	}
	
	return tigString;
}

TIGValue *TIGStr(const char *string)
{
	return TIGStringInput(NULL, string);
}

TIGValue *TIGStringInput(TIGValue *tigString, const char *string)
{
	return TIGStringStackInput(tigString, string, TIGYes);
}

TIGValue *TIGStringStackInput(TIGValue *tigString, const char *string, TIGBool useStack)
{
	if (tigString == NULL)
	{
		tigString = TIGStringCreate(tigString, useStack);
		
		if (tigString == NULL)
		{
#ifdef TIG_DEBUG
			printf("ERROR Function:TIGStringStackInput() Variable:tigString Equals:NULL\n");

	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
			return NULL;
		}
	}
	else if (strcmp(tigString->type, "String") != 0)
	{
#ifdef TIG_DEBUG
		printf("ERROR Function:TIGStringStackInput() Variable:tigString->type Equals:%s Valid:\"String\"\n", tigString->type);

	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
		return NULL;
	}
	
	// If there is already a string free it
	if (tigString->string != NULL)
	{
		free(tigString->string);
		tigString->string = NULL;
	}
	
	tigString->string = (char *)malloc((strlen(string) + 1) * sizeof(char));

	if (tigString->string == NULL || string == NULL)
	{
#ifdef TIG_DEBUG
		if (string == NULL)
		{
			printf("ERROR Function:TIGStringStackInput() Variable:string Equals:NULL\n");
		}
		
		if (tigString->string == NULL)
		{
			printf("ERROR Function:TIGStringStackInput() Variable:tigString->string Equals:NULL\n");
		}

	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
		return NULL;
	}
	else
	{
		strcpy(tigString->string, string);
	}
	
	return tigString;
}

char *TIGStringOutput(TIGValue *tigString)
{
	if (tigString == NULL || tigString->string == NULL || strcmp(tigString->type, "String") != 0)
	{
#ifdef TIG_DEBUG
		if (tigString == NULL)
		{
			printf("ERROR Function:TIGStringOutput() Variable:tigString Equals:NULL\n");
		}
		else
		{
			if (tigString->string == NULL)
			{
				printf("ERROR Function:TIGStringOutput() Variable:tigString->string Equals:NULL\n");
			}
			
			if (strcmp(tigString->type, "String") != 0)
			{
				printf("ERROR Function:TIGStringOutput() Variable:tigString->string Equals:%s Valid:\"String\"\n", tigString->type);
			}
		}

	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
		return NULL;
	}
	else
	{
		return tigString->string;
	}
}

TIGInteger TIGStringLength(TIGValue *tigString)
{
	if (tigString == NULL || tigString->string == NULL || strcmp(tigString->type, "String") != 0)
	{
#ifdef TIG_DEBUG
		if (tigString == NULL)
		{
			printf("ERROR Function:TIGStringLength() Variable:tigString Equals:NULL\n");
		}
		else
		{
			if (tigString->string == NULL)
			{
				printf("ERROR Function:TIGStringLength() Variable:tigString->string Equals:NULL\n");
			}
			
			if (strcmp(tigString->type, "String") != 0)
			{
				printf("ERROR Function:TIGStringLength() Variable:tigString->type Equals:%s Valid:\"String\"\n", tigString->type);
			}
		}

	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
		return -1;
	}
	//if (tigString != NULL && tigString->string != NULL && strcmp(tigString->type, "String") == 0)
	else
	{
		return (int)strlen(tigString->string);
	}
}

TIGValue *TIGStringInsertStringAtIndex(TIGValue *tigString1, TIGValue *tigString2, int index)
{
	if (tigString1 == NULL || tigString2 == NULL || strcmp(tigString1->type, "String") != 0 || strcmp(tigString2->type, "String") != 0
		|| index < 0 || index > TIGStringLength(tigString1))
	{
#ifdef TIG_DEBUG
		if (tigString1 == NULL)
		{
			printf("ERROR Function:TIGStringInsertStringAtIndex() Variable:tigString1 Equals:NULL\n");
		}
		else if (strcmp(tigString1->type, "String") != 0)
		{
			printf("ERROR Function:TIGStringInsertStringAtIndex() Variable:tigNumber Equals:%s Valid:\"String\"\n", tigString1->type);
		}

		if (tigString2 == NULL)
		{
			printf("ERROR Function:TIGStringInsertStringAtIndex() Variable:tigString2 Equals:NULL\n");
		}
		else if (strcmp(tigString2->type, "String") != 0)
		{
			printf("ERROR Function:TIGStringInsertStringAtIndex() Variable:tigNumber Equals:%s Valid:\"String\"\n", tigString2->type);
		}

		if (index < 0 || index > TIGStringLength(tigString1))
		{
			printf("ERROR Function:TIGStringInsertStringAtIndex() Variable:index Equals:%d Valid:0 to %d\n", index, TIGStringLength(tigString1));
		}

	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
		return NULL;
	}

	char *newString = (char *)malloc(strlen(tigString1->string) + strlen(tigString2->string) + 1);
	
	if (index == strlen(tigString1->string))
	{
		strcat(newString, tigString1->string);
		strcat(newString, tigString2->string);
	}
	else
	{
		char character[2];
		int i;
		
		for (i = 0; i < strlen(tigString1->string); i++)
		{
			character[0] = tigString1->string[i];
			character[1] = '\0';
			
			if (index == i)
			{
				strcat(newString, tigString2->string);
			}
			
			strcat(newString, character);
		}
	}
	
	TIGValue *theString = TIGStringInput(NULL, newString);

	free(newString);
	newString = NULL;

	return theString;
}

TIGValue *TIGStringCharacterAtIndex(TIGValue *tigString, int index)
{
	if (tigString == NULL || index < 0 || index >= TIGStringLength(tigString))
	{
#ifdef TIG_DEBUG
		if (tigString == NULL)
		{
			printf("ERROR Function:TIGStringCharacterAtIndex() Variable:tigString Equals:NULL\n");
		}
		
		if (index < 0 || index >= TIGStringLength(tigString))
		{
			printf("ERROR Function:TIGStringCharacterAtIndex() Variable:index Equals:%d Valid:0 to %d\n", index, TIGStringLength(tigString) - 1);
		}
	
	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
		return NULL;
	}
	
	char *character = TIGStringOutput(tigString);
	return TIGStringWithFormat(NULL, "%c", character[index]);
}

void TIGStringRemoveCharacterAtIndex(TIGValue *tigString, int index)
{
	if (tigString == NULL || index < 0 || index >= TIGStringLength(tigString))
	{
#ifdef TIG_DEBUG
		if (tigString == NULL)
		{
			printf("ERROR Function:TIGStringRemoveCharacterAtIndex() Variable:tigString Equals:NULL\n");
		}
		
		if (index < 0 || index >= TIGStringLength(tigString))
		{
			printf("ERROR Function:TIGStringRemoveCharacterAtIndex() Variable:index Equals:%d Valid:0 to %d\n", index, TIGStringLength(tigString) - 1);
		}

	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
		return;
	}
	
	int length = TIGStringLength(tigString);
	
	char *characters = TIGStringOutput(tigString);
	// Since a character is being removed don't add +1 to the malloc length
	char *newCharacters = (char *)malloc(length * sizeof(char));
	int newIndex = 0;
	int i;
	
	for (i = 0; i < length; i++)
	{
		if (index != i)
		{
			newCharacters[newIndex] = characters[i];
			newIndex++;
		}
	}
	
	TIGStringInput(tigString, newCharacters);
	
	free(newCharacters);
	newCharacters = NULL;
}

TIGValue *TIGStringFromNumber(TIGValue *tigNumber)
{
	if (tigNumber == NULL || strcmp(tigNumber->type, "Number") != 0)
	{
#ifdef TIG_DEBUG
		if (tigNumber == NULL)
		{
			printf("ERROR Function:TIGStringFromNumber() Variable:tigNumber Equals:NULL\n");
		}
		else if (strcmp(tigNumber->type, "Number") != 0)
		{
			printf("ERROR Function:TIGStringFromNumber() Variable:tigNumber->type Equals:%s Valid:\"Number\"\n", tigNumber->type);
		}

	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
		return NULL;
	}
	else
	{
		int stringLength = snprintf( NULL, 0, "%f", tigNumber->number) + 1;
		char *stringBuffer = (char *)malloc(stringLength * sizeof(char));
		
		if (stringBuffer == NULL)
		{
#ifdef TIG_DEBUG
			printf("ERROR Function:TIGStringFromNumber() Variable:stringBuffer Equals:NULL\n");

	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
			return NULL;
		}
		
		snprintf(stringBuffer, stringLength, "%f", tigNumber->number);
		
		TIGValue *tigString = TIGStringInput(NULL, stringBuffer);
	
		free(stringBuffer);
		stringBuffer = NULL;
	
		return tigString;
	}
}

TIGBool TIGStringEqualsString(TIGValue *tigString1, TIGValue *tigString2)
{
	if (tigString1 != NULL && strcmp(tigString1->type, "String") == 0 && tigString2 != NULL && strcmp(tigString2->type, "String") == 0
		&& strcmp(tigString1->string, tigString2->string) == 0)
	{
		return TIGYes;
	}
	
	return TIGNo;
}

TIGValue *TIGStringObjectType(TIGValue *tigObject)
{
	if (tigObject == NULL || tigObject->type == NULL)
	{
#ifdef TIG_DEBUG
		if (tigObject == NULL)
		{
			printf("ERROR Function:TIGStringObjectType() Variable:tigObject Equals:NULL\n");
		}
		else if (tigObject->type == NULL)
		{
			printf("ERROR Function:TIGStringObjectType() Variable:tigObject->type Equals:NULL\n");
		}

	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
		return NULL;
	}
	
	return TIGStringInput(NULL, tigObject->type);
}

TIGValue *TIGStringAddEscapeCharacters(TIGValue *tigString)
{
	if (tigString == NULL || strcmp(tigString->type, "String") != 0)
	{
#ifdef TIG_DEBUG
		if (tigString == NULL)
		{
			printf("ERROR Function:TIGStringAddEscapeCharacters() Variable:tigString Equals:NULL\n");
		}
		else if (strcmp(tigString->type, "String") != 0)
		{
			printf("ERROR Function:TIGStringAddEscapeCharacters() Variable:tigString->type Equals:%s Valid:\"String\"\n", tigString->type);
		}

	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
		return NULL;
	}
	
	char *string = tigString->string;

	int extraCount = 0;
	int i;
	
	for (i = 0; i < strlen(string); i++)
	{
		switch (string[i])
		{
			case '"':
			case '\\':
			case '/':
			case '\b':
			case '\f':
			case '\n':
			case '\r':			
			case '\t':
				extraCount++;
			break;
		}
	}
	
	if (extraCount > 0)
	{
		char *newString = (char *)malloc((strlen(string) + extraCount + 1) * sizeof(char));
		int index = 0;
	
		if (newString == NULL)
		{
#ifdef TIG_DEBUG
			printf("ERROR Function:TIGStringAddEscapeCharacters() Variable:newString Equals:NULL\n");
	
	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
			return NULL;
		}
	
		for (i = 0; i < strlen(string); i++)
		{
			switch (string[i])
			{
				case '\"':
					newString[index] = '\\';
					newString[index + 1] = '"';
					index += 2;
				break;
				
				case '\\':
					newString[index] = '\\';
					newString[index + 1] = '\\';
					index += 2;
				break;
				
				case '/':
					newString[index] = '\\';
					newString[index + 1] = '/';
					index += 2;
				break;
				
				case '\b':
					newString[index] = '\\';
					newString[index + 1] = 'b';
					index += 2;
				break;
				
				case '\f':
					newString[index] = '\\';
					newString[index + 1] = 'f';
					index += 2;
				break;
				
				case '\n':
					newString[index] = '\\';
					newString[index + 1] = 'n';
					index += 2;
				break;

				case '\r':
					newString[index] = '\\';
					newString[index + 1] = 'r';
					index += 2;
				break;
				
				case '\t':
					newString[index] = '\\';
					newString[index + 1] = 't';
					index += 2;
				break;
				
				default:
					newString[index] = string[i];
					index++;
				break;
			}
		}
		
		TIGValue *theNewTIGString = TIGStringInput(NULL, newString);
		
		free(newString);
		newString = NULL;
		
		return theNewTIGString;
	}
	
	return tigString;
}

TIGValue *TIGStringRemoveEscapeCharacters(TIGValue *tigString)
{
	if (tigString == NULL || strcmp(tigString->type, "String") != 0)
	{
#ifdef TIG_DEBUG
		if (tigString == NULL)
		{
			printf("ERROR Function:TIGStringRemoveEscapeCharacters() Variable:tigString Equals:NULL\n");
		}
		else if (strcmp(tigString->type, "String") != 0)
		{
			printf("ERROR Function:TIGStringRemoveEscapeCharacters() Variable:tigObject->type Equals:%s Valid:\"String\"\n", tigString->type);
		}

	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
		return NULL;
	}
	
	char *string = tigString->string;
	int extraCount = 0;
	int i;
	
	for (i = 0; i < strlen(string); i++)
	{
		if (string[i] == '\\')
		{
			switch (string[i + 1])
			{
				case '"':
				case '\\':
				case '/':
				case 'b':
				case 'f':
				case 'n':
				case 'r':
				case 't':
					extraCount++;
					// Below makes sure it is not read as something like \\t instead of \\ and \t
					i++;
				break;
			}
		}
	}
	
	//printf("extraCount %d\n", extraCount);

	if (extraCount > 0)
	{
		char *newString = (char *)malloc(((strlen(string) - extraCount) + 1) * sizeof(char));
		int index = 0;
	
		if (newString == NULL)
		{
#ifdef TIG_DEBUG
			printf("ERROR Function:TIGStringRemoveEscapeCharacters() Variable:newString Equals:NULL\n");

	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
			return NULL;
		}
	
		for (i = 0; i < strlen(string); i++)
		{
			if (string[i] == '\\')
			{
				switch (string[i + 1])
				{
					case '\"':
						newString[index] = '"';
						index++;
						i++;
					break;
					
					case '\\':
						newString[index] = '\\';
						index++;
						i++;
					break;

					case '/':
						newString[index] = '/';
						index++;
						i++;
					break;

					case 'b':
						newString[index] = '\b';
						index++;
						i++;
					break;

					case 'f':
						newString[index] = '\f';
						index++;
						i++;
					break;

					case 'n':
						newString[index] = '\n';
						index++;
						i++;
					break;

					case 'r':
						newString[index] = '\r';
						index++;
						i++;
					break;
					
					case 't':
						newString[index] = '\t';
						index++;
						i++;
					break;
				}
			}
			else
			{
				newString[index] = string[i];
				index++;
			}
		}
		
		newString[index] = '\0';
		
		TIGValue *theNewTIGString = TIGStringInput(NULL, newString);
		if (newString != NULL)
		{
			free(newString);
			newString = NULL;
		}
		
		return theNewTIGString;
	}
	
	return tigString;
}

TIGValue *TIGStringWithFormat(TIGValue *tigString, const char *format, ...)
{
	va_list arguments;
	
	// Find out how long the string is when the arguments are converted to text
	va_start(arguments, format);
	int stringLength = vsnprintf( NULL, 0, format, arguments) + 1;
	va_end(arguments);

	// Create the new buffer with the new string length
	char *stringBuffer = (char *)malloc(stringLength * sizeof(char));
	
	if (stringBuffer == NULL)
	{
#ifdef TIG_DEBUG
		printf("ERROR Function:TIGStringWithFormat() Variable:stringBuffer Equals:NULL\n");

	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
		return NULL;
	}
	else
	{
		// Use the new length of text and add the arguments to the new string buffer
		va_start(arguments, format);
		vsnprintf(stringBuffer, stringLength, format, arguments);
		va_end(arguments);
		
		if (stringBuffer != NULL)
		{
			if (tigString == NULL)
			{
				tigString = TIGStringInput(tigString, stringBuffer);
			}
			else if (tigString->string != NULL && strcmp(tigString->type, "String") == 0)
			{
				//printf("Length: %d\n", (int)(strlen(tigString->string) + stringLength));
				
				// stringLength already has +1 added to it for the '\0' so adding another +1 below is not necessary
				tigString->string = (char *)realloc(tigString->string, (strlen(tigString->string) + stringLength) * sizeof(char));
				strcat(tigString->string, stringBuffer);
			}
		}
		else
		{
	#ifdef TIG_DEBUG
			if (tigString->string == NULL)
			{
				printf("ERROR Function:TIGStringWithFormat() Variable:tigString->string Equals:NULL\n");
			}
			
			if (strcmp(tigString->type, "String") != 0)
			{
				printf("ERROR Function:TIGStringWithFormat() Variable:tigString->type Equals:%s Valid:\"String\"\n", tigString->type);
			}

		#ifdef TIG_DEBUG_ASSERT
			assert(0);
		#endif
	#endif
			return NULL;
		}

		free(stringBuffer);
		stringBuffer = NULL;
	}
	
	return tigString;
}

TIGValue *TIGStringWithAddedString(TIGValue *oldTigString, TIGValue *newTigString)
{	
	if (oldTigString == NULL || newTigString == NULL || oldTigString->string == NULL)
	{
#ifdef TIG_DEBUG
		if (oldTigString == NULL)
		{
			printf("ERROR Function:TIGStringWithAddedString() Variable:oldTigString Equals:NULL\n");
		}
		else if (oldTigString->string == NULL)
		{
			printf("ERROR Function:TIGStringWithAddedString() Variable:oldTigString->string Equals:NULL\n");
		}
		
		if (newTigString == NULL)
		{
			printf("ERROR Function:TIGStringWithAddedString() Variable:newTigString Equals:NULL\n");
		}

	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
		return NULL;
	}
	
	oldTigString = TIGStringInsertStringAtIndex(oldTigString, newTigString, (int)strlen(oldTigString->string));
	
	return oldTigString;
}

TIGValue *TIGStringFromObject(TIGValue *tigObject)
{
	if (tigObject == NULL || strcmp(tigObject->type, "Object") != 0)
	{
#ifdef TIG_DEBUG
		if (tigObject == NULL)
		{
			printf("ERROR Function:TIGStringFromObject() Variable:tigObject Equals:NULL\n");
		}
		else if (strcmp(tigObject->type, "Object") != 0)
		{
			printf("ERROR Function:TIGStringFromObject() Variable:tigObject->type Equals:%s Valid:\"Object\"\n", tigObject->type);
		}

	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
		return NULL;
	}
	
	TIGObjectStartStack(NULL);
	TIGArrayStartStack(NULL);
	TIGNumberStartStack(NULL);

	TIGValue *theString = TIGStringFromObjectWithLevel(NULL, tigObject, 1, TIGYes);

	TIGNumberEndStack(NULL);
	TIGArrayEndStack(NULL);
	TIGObjectEndStack(NULL);
	
	return theString;
}

TIGValue *TIGStringFromObjectForNetwork(TIGValue *tigObject)
{
	if (tigObject == NULL || strcmp(tigObject->type, "Object") != 0)
	{
#ifdef TIG_DEBUG
		if (tigObject == NULL)
		{
			printf("ERROR Function:TIGStringFromObjectForNetwork() Variable:tigObject Equals:NULL\n");
		}
		else if (strcmp(tigObject->type, "Object") != 0)
		{
			printf("ERROR Function:TIGStringFromObjectForNetwork() Variable:tigObject->type Equals:%s Valid:\"Object\"\n", tigObject->type);
		}

	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
		return NULL;
	}
	
	TIGObjectStartStack(NULL);
	TIGArrayStartStack(NULL);
	TIGNumberStartStack(NULL);

	TIGValue *theString = TIGStringFromObjectWithLevel(NULL, tigObject, 1, TIGNo);

	TIGNumberEndStack(NULL);
	TIGArrayEndStack(NULL);
	TIGObjectEndStack(NULL);
	
	return theString;
}

TIGValue *TIGStringFromArray(TIGValue *tigArray)
{
	if (tigArray == NULL || strcmp(tigArray->type, "Array") != 0)
	{
#ifdef TIG_DEBUG
		if (tigArray == NULL)
		{
			printf("ERROR Function:TIGStringFromArray() Variable:tigArray Equals:NULL\n");
		}
		else if (strcmp(tigArray->type, "Array") != 0)
		{
			printf("ERROR Function:TIGStringFromArray() Variable:tigArray->type Equals:%s Valid:\"Array\"\n", tigArray->type);
		}

	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
		return NULL;
	}

	TIGObjectStartStack(NULL);
	TIGArrayStartStack(NULL);
	TIGNumberStartStack(NULL);

	TIGValue *theString = TIGStringFromObjectWithLevel(NULL, tigArray, 1, TIGYes);

	TIGNumberEndStack(NULL);
	TIGArrayEndStack(NULL);
	TIGObjectEndStack(NULL);
	
	return theString;
}

TIGValue *TIGStringFromArrayForNetwork(TIGValue *tigArray)
{
	if (tigArray == NULL || strcmp(tigArray->type, "Array") != 0)
	{
#ifdef TIG_DEBUG
		if (tigArray == NULL)
		{
			printf("ERROR Function:TIGStringFromArrayForNetwork() Variable:tigArray Equals:NULL\n");
		}
		else if (strcmp(tigArray->type, "Array") != 0)
		{
			printf("ERROR Function:TIGStringFromArrayForNetwork() Variable:tigArray->type Equals:%s Valid:\"Array\"\n", tigArray->type);
		}
	
	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
		return NULL;
	}

	TIGObjectStartStack(NULL);
	TIGArrayStartStack(NULL);
	TIGNumberStartStack(NULL);

	TIGValue *theString = TIGStringFromObjectWithLevel(NULL, tigArray, 1, TIGNo);

	TIGNumberEndStack(NULL);
	TIGArrayEndStack(NULL);
	TIGObjectEndStack(NULL);
	
	return theString;
}

// The JSON string outputs a TIGString but the functions below have their own stack
TIGValue *TIGStringFromObjectWithLevel(TIGValue *tigString, TIGValue *tigValue, int level, TIGBool useEscapeCharacters)
{
	int i;
	
	if (strcmp(tigValue->type, "Array") == 0)
	{
		TIGValue *theTIGStringTabs = NULL;
		TIGValue *theTIGStringEndTab = NULL;
		
		if (useEscapeCharacters)
		{
			for (i = 0; i < level; i++)
			{
				theTIGStringTabs = TIGStringWithFormat(theTIGStringTabs, "\t");
				
				if (i < level - 1)
				{
					theTIGStringEndTab = TIGStringWithFormat(theTIGStringEndTab, "\t");
				}
			}
		
			tigString = TIGStringWithFormat(tigString, "[\n");
		}
		else
		{
			tigString = TIGStringWithFormat(tigString, "[");
		}
		
		for (i = 0; i < TIGArrayCount(tigValue); i++)
		{
			TIGValue *theTIGValue = TIGArrayValueAtIndex(tigValue, i);
			
			if (useEscapeCharacters)
			{
				tigString = TIGStringWithAddedString(tigString, theTIGStringTabs);
			}
			
			tigString = TIGStringFromObjectWithLevel(tigString, theTIGValue, level + 1, useEscapeCharacters);

			if (useEscapeCharacters)
			{
				if (i < TIGArrayCount(tigValue) - 1)
				{
					tigString = TIGStringWithFormat(tigString, ",\n");
				}
				else
				{
					tigString = TIGStringWithFormat(tigString, "\n");
				}
			}
			else
			{
				if (i < TIGArrayCount(tigValue) - 1)
				{
					tigString = TIGStringWithFormat(tigString, ",");
				}
			}
		}
	
		if (level > 1 && useEscapeCharacters)
		{
			tigString = TIGStringWithAddedString(tigString, theTIGStringEndTab);
		}
		
		tigString = TIGStringWithFormat(tigString, "]");
	}
	else if (strcmp(tigValue->type, "Number") == 0)
	{
		if (tigValue->string != NULL)
		{
			if (strcmp(tigValue->string, "false") == 0 || strcmp(tigValue->string, "true") == 0)
			{
				tigString = TIGStringWithAddedString(tigString, TIGStringInput(NULL, tigValue->string));
			}
		}
		else
		{
			tigString = TIGStringWithAddedString(tigString, TIGStringFromNumber(tigValue));
		}
	}
	else if (strcmp(tigValue->type, "Object") == 0)
	{
		TIGValue *theTIGArrayStrings = TIGArrayOfObjectStrings(tigValue);
		TIGValue *theTIGArrayValues = TIGArrayOfObjectValues(tigValue);
		
		TIGValue *theTIGStringTabs = NULL;
		TIGValue *theTIGStringEndTab = NULL;

		if (useEscapeCharacters)
		{
			for (i = 0; i < level; i++)
			{
				theTIGStringTabs = TIGStringWithFormat(theTIGStringTabs, "\t");
				
				if (i < level - 1)
				{
					theTIGStringEndTab = TIGStringWithFormat(theTIGStringEndTab, "\t");
				}
			}
		
			tigString = TIGStringWithFormat(tigString, "{\n");
		}
		else
		{
			tigString = TIGStringWithFormat(tigString, "{");
		}

		for (i = 0; i < TIGArrayCount(theTIGArrayStrings); i++)
		{
			TIGValue *theTIGString = TIGArrayValueAtIndex(theTIGArrayStrings, i);
			TIGValue *theTIGValue = TIGArrayValueAtIndex(theTIGArrayValues, i);
			
			if (useEscapeCharacters)
			{
				tigString = TIGStringWithAddedString(tigString, theTIGStringTabs);
				tigString = TIGStringWithFormat(tigString, "\"%s\": ", TIGStringOutput(TIGStringAddEscapeCharacters(theTIGString)));
			}
			else
			{
				tigString = TIGStringWithFormat(tigString, "\"%s\":", TIGStringOutput(TIGStringAddEscapeCharacters(theTIGString)));
			}
			
			tigString = TIGStringFromObjectWithLevel(tigString, theTIGValue, level + 1, useEscapeCharacters);
			
			if (useEscapeCharacters)
			{
				if (i < TIGArrayCount(theTIGArrayStrings) - 1)
				{
					tigString = TIGStringWithFormat(tigString, ",\n");
				}
				else
				{
					tigString = TIGStringWithFormat(tigString, "\n");
				}
			}
			else
			{
				if (i < TIGArrayCount(theTIGArrayStrings) - 1)
				{
					tigString = TIGStringWithFormat(tigString, ",");
				}
			}
		}
		
		if (level > 1 && useEscapeCharacters)
		{
			tigString = TIGStringWithAddedString(tigString, theTIGStringEndTab);
		}
		
		tigString = TIGStringWithFormat(tigString, "}");
	}
	else if (strcmp(tigValue->type, "String") == 0)
	{
		tigString = TIGStringWithFormat(tigString, "\"%s\"", TIGStringOutput(TIGStringAddEscapeCharacters(tigValue)));
	}
	
	return tigString;
}

void TIGStringWriteWithFilename(TIGValue *tigString, TIGValue *filenameString)
{
	if (tigString == NULL || filenameString == NULL || strcmp(tigString->type, "String") != 0 || strcmp(filenameString->type, "String") != 0)
	{
#ifdef TIG_DEBUG
		if (tigString == NULL)
		{
			printf("ERROR Function:TIGStringWriteWithFilename() Variable:tigString Equals:NULL\n");
		}
		else if (strcmp(tigString->type, "String") != 0)
		{
			printf("ERROR Function:TIGStringWriteWithFilename() Variable:tigString->type Equals:%s Valid:\"String\"\n", tigString->type);
		}
		
		if (filenameString == NULL)
		{
			printf("ERROR Function:TIGStringWriteWithFilename() Variable:filenameString Equals:NULL\n");
		}
		else if (strcmp(filenameString->type, "String") != 0)
		{
			printf("ERROR Function:TIGStringWriteWithFilename() Variable:filenameString->type Equals:%s Valid:\"String\"\n", filenameString->type);
		}

	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
	}
	else
	{
		FILE *theFile = fopen(filenameString->string, "w");
		
		if (theFile != NULL)
		{
			fprintf(theFile, "%s", tigString->string);
		}
		else
		{
#ifdef TIG_DEBUG
			printf("ERROR Function:TIGStringWriteWithFilename() Variable:theFile Equals:NULL\n");

	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
		}
		
		fclose(theFile);
	}
}

TIGValue *TIGStringReadFromFilename(TIGValue *filenameString)
{
	if (filenameString == NULL || filenameString->string == NULL || strcmp(filenameString->type, "String") != 0)
	{
		if (filenameString == NULL)
		{
			printf("ERROR Function:TIGStringWriteWithFilename() Variable:tigString Equals:NULL\n");
		}
		else
		{
			if (strcmp(filenameString->type, "String") != 0)
			{
				printf("ERROR Function:TIGStringWriteWithFilename() Variable:filenameString->type Equals:%s Valid:\"String\"\n", filenameString->type);
			}
			
			if (filenameString->string == NULL)
			{
				printf("ERROR Function:TIGStringWriteWithFilename() Variable:filenameString->string Equals:NULL\n");
			}
		}
		
		return NULL;
	}
	
	FILE *theFile = fopen(filenameString->string, "r");
	int index = 0, block = 1, maxBlockLength = 100;
	char *newString = NULL;
	
	char *buffer = malloc(maxBlockLength * sizeof(char));
	
	if (theFile == NULL)
	{
#ifdef TIG_DEBUG
		printf("ERROR Function:TIGStringReadFromFilename() Variable:theFile Equals:NULL\n");

	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
		if (buffer != NULL)
		{
			free(buffer);
			buffer = NULL;
		}
		
		fclose(theFile);
		return NULL;
	}
	
	if (buffer == NULL)
	{
#ifdef TIG_DEBUG
		printf("ERROR Function:TIGStringReadFromFilename() Variable:buffer Equals:NULL\n");

	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
		fclose(theFile);
		return NULL;
	}

	
	while (1)
	{
		buffer[index] = fgetc(theFile);
		
		if ((buffer[index] != EOF && index >= maxBlockLength - 2) || buffer[index] == EOF)
		{
			int stringLength;
			buffer[index + 1] = '\0';
			
			if (newString == NULL)
			{
				stringLength = 0;
			}
			else
			{
				stringLength = (int)strlen(newString);
			}

			if (buffer[index] == EOF)
			{
				//printf("END Buffer: %d String Length: %d\n", (int)strlen(buffer), stringLength);
				// Since the "buffer" variable already has '\0' +1 is not needed
				newString = realloc(newString, (strlen(buffer) + stringLength) * sizeof(char));
			}
			else
			{
				//printf("Buffer: %d String Length: %d\n", (int)strlen(buffer), stringLength);
				newString = realloc(newString, (strlen(buffer) + stringLength) * sizeof(char));
			}
			
			if (newString == NULL)
			{
#ifdef TIG_DEBUG
				printf("ERROR Function:TIGStringReadFromFilename() Variable:newString Equals:NULL\n");

	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
				free(buffer);
				buffer = NULL;
				
				fclose(theFile);
				return NULL;
			}
			
			strcat(newString, buffer);
			
			if (buffer[index] == EOF)
			{
				//printf("Total String Length: %d", (int)strlen(newString));
				
				// Since the "buffer" always uses the same size block '\0' with -1 is needed for the last index number
				newString[strlen(newString) - 1] = '\0';
				
				free(buffer);
				buffer = NULL;

				break;
			}
			else
			{
				free(buffer);
				buffer = NULL;
				
				buffer = malloc(maxBlockLength * sizeof(char));
				
				index = -1;
				block++;
			}
		}
		
		index++;
	}
	
	fclose(theFile);
	
	TIGValue *theString = TIGStringInput(NULL, newString);
	
	free(newString);
	newString = NULL;
	
	return theString;
}

TIGBool TIGStringPrefix(TIGValue *tigString, TIGValue *tigStringPrefix)
{
	if (tigString == NULL || strcmp(tigString->type, "String") != 0 || tigStringPrefix == NULL || strcmp(tigStringPrefix->type, "String") != 0)
	{
#ifdef TIG_DEBUG
		if (tigString == NULL)
		{
			printf("ERROR Function:TIGStringPrefix() Variable:tigString Equals:NULL\n");
		}
		else if (strcmp(tigString->type, "String") != 0)
		{
			printf("ERROR Function:TIGStringPrefix() Variable:tigString->type Equals:%s Valid:\"String\"\n", tigString->type);
		}
		
		if (tigStringPrefix == NULL)
		{
			printf("ERROR Function:TIGStringPrefix() Variable:tigStringPrefix Equals:NULL\n");
		}
		else if (strcmp(tigStringPrefix->type, "String") != 0)
		{
			printf("ERROR Function:TIGStringPrefix() Variable:tigStringPrefix->type Equals:%s Valid:\"String\"\n", tigStringPrefix->type);
		}

	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
		return TIGNo;
	}

	if (strlen(tigString->string) > 0 && strlen(tigStringPrefix->string) > 0)
	{
		int i;
		
		for (i = 0; i < strlen(tigString->string); i++)
		{
			if (tigString->string[i] == tigStringPrefix->string[i])
			{
				// The prefix has been found
				if (i >= strlen(tigStringPrefix->string) - 1)
				{
					return TIGYes;
				}
			}
			else
			{
				return TIGNo;
			}
		}
	}
	
	return TIGNo;
}

TIGBool TIGStringSuffix(TIGValue *tigString, TIGValue *tigStringSuffix)
{
	if (tigString == NULL || strcmp(tigString->type, "String") != 0 || tigStringSuffix == NULL || strcmp(tigStringSuffix->type, "String") != 0)
	{
#ifdef TIG_DEBUG
		if (tigString == NULL)
		{
			printf("ERROR Function:TIGStringSuffix() Variable:tigString Equals:NULL\n");
		}
		else if (strcmp(tigString->type, "String") != 0)
		{
			printf("ERROR Function:TIGStringSuffix() Variable:tigString->type Equals:%s Valid:\"String\"\n", tigString->type);
		}
		
		if (tigStringSuffix == NULL)
		{
			printf("ERROR Function:TIGStringSuffix() Variable:tigStringSuffix Equals:NULL\n");
		}
		else if (strcmp(tigStringSuffix->type, "String") != 0)
		{
			printf("ERROR Function:TIGStringSuffix() Variable:tigStringSuffix->type Equals:%s Valid:\"String\"\n", tigStringSuffix->type);
		}

	#ifdef TIG_DEBUG_ASSERT
		assert(0);
	#endif
#endif
		return TIGNo;
	}
	
	if (strlen(tigString->string) > 0 && strlen(tigStringSuffix->string) > 0)
	{
		int suffixIndex = 0, suffixTotal = (int)strlen(tigStringSuffix->string), index = 0, total = (int)strlen(tigString->string);
		
		while (1)
		{
			if (tigString->string[total - index - 1] == tigStringSuffix->string[suffixTotal - suffixIndex - 1])
			{
				// The suffix was found
				if (suffixIndex >= suffixTotal - 1)
				{
					return TIGYes;
				}
				
				suffixIndex++;
				index++;
			}
			else
			{
				return TIGNo;
			}
		}
	}
	
	return TIGNo;
}
