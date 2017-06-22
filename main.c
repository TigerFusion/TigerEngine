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

// **** The PRINTFs activate the printf() in each example function below ****
#define TIGARRAY_PRINTF
#define TIGNUMBER_PRINTF
#define TIGOBJECT_PRINTF
#define TIGSTRING_PRINTF
// **** The PRINTFs activate the printf() in each example function below ****

void arrayFunctions(void);
void numberFunctions(void);
void objectFunctions(void);
void stringFunctions(void);

int main(int argc, const char *argv[])
{
	// ********************************************************************************************************************
	// The four functions below show how to use the functions in the Tiger Engine.
	// Some Tiger Engine functions are not used in the examples below.
	// It is recommended that you use only the functions listed in this file.
	// ********************************************************************************************************************
	
	arrayFunctions();
	numberFunctions();
	objectFunctions();
	stringFunctions();
	
	// ********************************************************************************************************************
	// Below is all about stacks
	// ********************************************************************************************************************
	
	// Start and end stacks must be used before and after functions in the Tiger Engine.
	// TIGArray, TIGNumber, TIGObject, and TIGString all have their own stacks.
	// If the function has the name TIGString as its prefix then TIGStringStartStack() and TIGStringEndStack() must be used before and after the function.
	
	// Every start stack that uses "NULL" as the argument increments an internal index number for the next start stack.
	// Once the end stacks are called the stack number deincrements.
	// The TIGValues are destroyed when the end stack gets to the same increment number that was generated internally with the start stack.
	
	// Usually using "NULL" as the argument for the stacks is fine.
	// For more advanced memory management you can use a C string for the argument.
	
	TIGStringStartStack("String Stack");
	
		TIGValue *theString1 = TIGStringInput(NULL, "String Stack 1");
	
		TIGStringStartStack(NULL);
	
			TIGValue *theString2 = TIGStringInput(NULL, "String Stack 2");
			printf("*** All About Stacks 1 *** : %s\n", TIGStringOutput(theString1));
	
		// theString1 is destroyed here.
		TIGStringEndStack("String Stack");

		// Uncomment this and it will generate an error since the theString1 has been destroyed in the end stack above.
		//printf("*** All About Stacks 1 *** : %s\n", TIGStringOutput(theString1));
	
		printf("*** All About Stacks 2 *** : %s\n", TIGStringOutput(theString2));
	
	// theString2 is destroyed here
	TIGStringEndStack(NULL);

	// ********************************************************************************************************************
	// Above is all about stacks
	// ********************************************************************************************************************

	// ********************************************************************************************************************
	// Below is all about structs and TIGObjects
	// ********************************************************************************************************************

	// TIGObjects and TIGArrays can be very useful.
	// They can be used to generate JSON files or used as a bridge between C arrays and arrays in object oriented languages.

	TIGVector vector[2];
	
	vector[0].x = 1.0;
	vector[0].y = 2.0;
	vector[0].z = 3.0;

	vector[1].x = 4.0;
	vector[1].y = 5.0;
	vector[1].z = 6.0;

	TIGObjectStartStack(NULL);
		TIGArrayStartStack(NULL);
			TIGStringStartStack(NULL);
				TIGNumberStartStack(NULL);
	
					// The TIGValues must equal "NULL" before starting.
					TIGValue *theArray1 = NULL;
					TIGValue *theObject1 = NULL;
	
					// TIGArr() TIGNum() TIGObj() TIGStr() are not list in the examples.
					// They are used as short cuts for creating TIGValues.
					for (int i = 0; i < 2; i++)
					{
						theObject1 = TIGObjectAddStringWithValue(theObject1, TIGStr("X"), TIGNum(vector[i].x));
						theObject1 = TIGObjectAddStringWithValue(theObject1, TIGStr("Y"), TIGNum(vector[i].y));
						theObject1 = TIGObjectAddStringWithValue(theObject1, TIGStr("Z"), TIGNum(vector[i].z));
						
						theArray1 = TIGArrayAddValue(theArray1, theObject1);
						theObject1 = NULL;
					}

					theObject1 = TIGObjectAddStringWithValue(theObject1, TIGStr("Vectors"), theArray1);
		
					// TIGStringFromObject() Converts theObject1 to a string that can be used as a JSON file.
					printf("*** All About Structs And Objects *** : %s\n", TIGStringOutput(TIGStringFromObject(theObject1)));
	
				TIGNumberEndStack(NULL);
			TIGStringEndStack(NULL);
		TIGArrayEndStack(NULL);
	TIGObjectEndStack(NULL);

	// ********************************************************************************************************************
	// Above is all about structs and TIGObjects
	// ********************************************************************************************************************
	
	return 0;
}

void arrayFunctions(void)
{
	// For every start stack there should be an end stack. I indent every new stack but it is just a preference.
	TIGArrayStartStack(NULL);
		TIGStringStartStack(NULL);
	
			TIGValue *theString1 = TIGStringInput(NULL, "String 1");
			// When starting a new TIGArray Use NULL for the first argument. Add Value adds theString1 to the end of theArray1's list
			TIGValue *theArray1 = TIGArrayAddValue(NULL, theString1);
#ifdef TIGARRAY_PRINTF
			printf("TIGArray theArray1's first added value %s\n", TIGStringOutput(TIGStringFromArray(theArray1)));
#endif

			TIGValue *theString2 = TIGStringInput(NULL, "String 2");
			// This inserts theString2 into the first slot of the array.
			theArray1 = TIGArrayInsertValueAtIndex(theArray1, theString2, 0);
#ifdef TIGARRAY_PRINTF
			printf("TIGArray theArray1 value inserted at index 0 %s\n", TIGStringOutput(TIGStringFromArray(theArray1)));
#endif

			// This outputs a value in the TIGArray at the specified index number.
			TIGValue *theValue1 = TIGArrayValueAtIndex(theArray1, 0);
			// This prints out a TIGString which is at index 0 in the TIGArray.
#ifdef TIGARRAY_PRINTF
			printf("TIGArray value at index 0: %s\n", TIGStringOutput(theValue1));
#endif

			// This removes a value in the TIGArray at the specified index number.
			TIGArrayRemoveValueAtIndex(theArray1, 0);
			// This prints out the value that was not removed. Note: "String 1" has been removed.
#ifdef TIGARRAY_PRINTF
			printf("TIGArray theArray1 values %s\n", TIGStringOutput(TIGStringFromArray(theArray1)));
#endif

			// This prints out an integer value of how many values are in the TIGArray.
			int count = TIGArrayCount(theArray1);
#ifdef TIGARRAY_PRINTF
			printf("TIGArray total values in theArray1 %d\n", count);
#endif

			// This removes everything in theArray1.
			TIGArrayRemoveAllValues(theArray1);
#ifdef TIGARRAY_PRINTF
			printf("TIGArray theArray1 values after remove all %s\n", TIGStringOutput(TIGStringFromArray(theArray1)));
#endif

			// Add some data back to theArray1.
			theArray1 = TIGArrayAddValue(theArray1, theString1);
			theArray1 = TIGArrayAddValue(theArray1, theString2);
			// This replaces theString1 with another copy of theString2.
			TIGArrayReplaceValueAtIndex(theArray1, theString2, 0);
#ifdef TIGARRAY_PRINTF
			printf("TIGArray theString2 replaces theString1 %s\n", TIGStringOutput(TIGStringFromArray(theArray1)));
#endif
	
			// Start an object stack.
			TIGObjectStartStack(NULL);
				// Add some data to an object.
				TIGValue *theObject1 = TIGObjectAddStringWithValue(NULL, theString1, theString2);
				// This outputs an array of all of the strings in an object.
				theArray1 = TIGArrayOfObjectStrings(theObject1);
#ifdef TIGARRAY_PRINTF
				printf("TIGArray theString1 is the string %s\n", TIGStringOutput(TIGStringFromArray(theArray1)));
#endif
	
				// This outputs an array of all of the values in an object.
				theArray1 = TIGArrayOfObjectValues(theObject1);
#ifdef TIGARRAY_PRINTF
				printf("TIGArray theString2 is the value %s\n", TIGStringOutput(TIGStringFromArray(theArray1)));
#endif
			// End the object stack.
			TIGObjectEndStack(NULL);

			// Since the Tiger Engine copies its values you can insert the same values into the same array.
			// This inserts theArray1 into the first slot of theArray1.
			theArray1 = TIGArrayInsertValuesFromArrayAtIndex(theArray1, theArray1, 0);
#ifdef TIGARRAY_PRINTF
			printf("TIGArray inserts theArray1 into theArray1 %s\n", TIGStringOutput(TIGStringFromArray(theArray1)));
#endif

			// This adds theArray1 to the end of theArray1.
			theArray1 = TIGArrayAddValuesFromArray(theArray1, theArray1);
#ifdef TIGARRAY_PRINTF
			printf("TIGArray adds theArray1 to theArray1 %s\n", TIGStringOutput(TIGStringFromArray(theArray1)));
#endif
	
			// Outputs the first value in theArray1.
			TIGValue *theString3 = TIGArrayFirstValue(theArray1);
#ifdef TIGARRAY_PRINTF
			printf("TIGArray the first value in theArray1: %s\n", TIGStringOutput(theString3));
#endif

			// Outputs the last value in theArray1.
			TIGValue *theString4 = TIGArrayLastValue(theArray1);
#ifdef TIGARRAY_PRINTF
			printf("TIGArray the last value in theArray1: %s\n", TIGStringOutput(theString4));
#endif

			// Outputs TIGYes if the arrays are the same or TIGNo if not.
			TIGBool theBoolean1 = TIGArrayEqualsArray(theArray1, theArray1);
			// Prints out "1" for TIGYes and "0" for TIGNo.
#ifdef TIGARRAY_PRINTF
			printf("TIGArray decides if the arrays are equal to eachother: %d\n", theBoolean1);
#endif
	
		TIGStringEndStack(NULL);
	TIGArrayEndStack(NULL);
}

void numberFunctions(void)
{
	TIGNumberStartStack(NULL);

		// This allows you to insert a boolean. This function is used for JSON files that support the values true and false.
		TIGValue *theNumber1 = TIGNumberBooleanInput(NULL, TIGYes);
		// Prints out "1" for TIGYes and "0" for TIGNo.
#ifdef TIGNUMBER_PRINTF
		printf("TIGNumber a boolean %f\n", TIGNumberOutput(theNumber1));
#endif

		// Turns a number into a TIGValue number. Make sure to use NULL as the first argument.
		TIGValue *theNumber2 = TIGNumberInput(NULL, 10.0);
#ifdef TIGNUMBER_PRINTF
		printf("TIGNumber input a number %f\n", TIGNumberOutput(theNumber2));
#endif
	
		// This turns a TIGValue to a C float or double number.
		TIGScalar number1 = TIGNumberOutput(theNumber2);
#ifdef TIGNUMBER_PRINTF
		printf("TIGNumber output a number %f\n", number1);
#endif

		// The start of the string stack
		TIGStringStartStack(NULL);
			// Create the data for the string.
			TIGValue *theString1 = TIGStringInput(NULL, "20.0");
			// Output a number from a string.
			TIGScalar number2 = TIGNumberFromString(theString1);
#ifdef TIGNUMBER_PRINTF
			printf("TIGNumber output a number from a string %f\n", number2);
#endif
		// The end of the string stack
		TIGStringEndStack(NULL);
	
		// Test whether theNumber1 is the same number as theNumber2
		TIGBool equalsNumber = TIGNumberEqualsNumber(theNumber1, theNumber2);
		// Prints out "1" for TIGYes and "0" for TIGNo.
#ifdef TIGNUMBER_PRINTF
			printf("TIGNumber check to see if the numbers are equal %d\n", equalsNumber);
#endif

	TIGNumberEndStack(NULL);
}

void objectFunctions(void)
{
	TIGObjectStartStack(NULL);
		TIGStringStartStack(NULL);
	
			// Create data for theObject1.
			TIGValue *theString1 = TIGStringInput(NULL, "String 1");
			TIGValue *theString2 = TIGStringInput(NULL, "String 2");
			TIGValue *theString3 = TIGStringInput(NULL, "String 3");
			TIGValue *theString4 = TIGStringInput(NULL, "String 4");
			// Note that the last argument in TIGObjectAddStringWithValue() can be any type of TIGValue.
			// Also note that if the value of the second argument "String 1" is reused the value attached to it is replaced.
			TIGValue *theObject1 = TIGObjectAddStringWithValue(NULL, theString1, theString2);
			theObject1 = TIGObjectAddStringWithValue(theObject1, theString3, theString4);
#ifdef TIGOBJECT_PRINTF
			printf("TIGObject string and value %s\n", TIGStringOutput(TIGStringFromObject(theObject1)));
#endif
	
			// This outputs the value attached to the string in theObject1.
			TIGValue *theString5 = TIGObjectValueFromString(theObject1, theString1);
#ifdef TIGOBJECT_PRINTF
			printf("TIGObject the value attached to a string %s\n", TIGStringOutput(theString5));
#endif
	
			// This removes the value attached to the string "String 1".
			TIGObjectRemoveValueFromString(theObject1, theString1);
#ifdef TIGOBJECT_PRINTF
			printf("TIGObject String 1 and its value are removed %s\n", TIGStringOutput(TIGStringFromObject(theObject1)));
#endif
	
			// Outputs how many values are in theObject1.
			int totalObjects = TIGObjectCount(theObject1);
#ifdef TIGOBJECT_PRINTF
			printf("TIGObject total values %d\n", totalObjects);
#endif

			// Removes all values and their strings in theObject1.
			TIGObjectRemoveAllValues(theObject1);
#ifdef TIGOBJECT_PRINTF
			printf("TIGObject all values removed %s\n", TIGStringOutput(TIGStringFromObject(theObject1)));
#endif

			// Create data for theObject1.
			theObject1 = TIGObjectAddStringWithValue(theObject1, theString1, theString2);
			theObject1 = TIGObjectAddStringWithValue(theObject1, theString3, theString4);
			// Convert the object to a string
			TIGValue *theString6 = TIGStringFromObject(theObject1);
			// This can read objects that have been saved in the JSON text format.
			theObject1 = TIGObjectFromString(theString6);
#ifdef TIGOBJECT_PRINTF
			printf("TIGObject object converted from string %s\n", TIGStringOutput(TIGStringFromObject(theObject1)));
#endif
	
			// Both objects have the same strings which gets a little tricky. This means theObject1 strings and values replace the other theObject1.
			theObject1 = TIGObjectAddValuesFromObject(theObject1, theObject1);
#ifdef TIGOBJECT_PRINTF
			printf("TIGObject object added to another object %s\n", TIGStringOutput(TIGStringFromObject(theObject1)));
#endif
	
			// This tests whether two objects have the same data in them.
			TIGBool isEqual = TIGObjectEqualsObject(theObject1, theObject1);
			// Prints out "1" for TIGYes and "0" for TIGNo.
#ifdef TIGOBJECT_PRINTF
			printf("TIGObject is theObject1 equal to theObject1? %d\n", isEqual);
#endif
	
		TIGStringEndStack(NULL);
	TIGObjectEndStack(NULL);
}

void stringFunctions(void)
{
	TIGStringStartStack(NULL);
		// Create TIGValue strings.
		TIGValue *theString1 = TIGStringInput(NULL, "String 1");
		TIGValue *theString2 = TIGStringInput(NULL, "String 2");
#ifdef TIGSTRING_PRINTF
		printf("TIGString output of input %s\n", TIGStringOutput(theString1));
#endif

		// Output the input of the TIGValue string.
		char *string1 = TIGStringOutput(theString1);
#ifdef TIGSTRING_PRINTF
		printf("TIGString output %s\n", string1);
#endif

		// Output the TIGValue's string length.
		int length = TIGStringLength(theString1);
#ifdef TIGSTRING_PRINTF
		printf("TIGString total characters in string %d\n", length);
#endif
	
		// Insert theString2 into theString1 at index 2. The string can be inserted between the index number "0" to TIGStringLength().
		theString1 = TIGStringInsertStringAtIndex(theString1, theString2, 2);
#ifdef TIGSTRING_PRINTF
		printf("TIGString insert a string %s\n", TIGStringOutput(theString1));
#endif

		// Find the character at index 2 in theString1.
		theString1 = TIGStringCharacterAtIndex(theString1, 2);
#ifdef TIGSTRING_PRINTF
		printf("TIGString find a character at index %s\n", TIGStringOutput(theString1));
#endif
	
		// Recreate the first TIGValue string.
		theString1 = TIGStringInput(NULL, "String 1");
		TIGStringRemoveCharacterAtIndex(theString1, 2);
#ifdef TIGSTRING_PRINTF
		printf("TIGString removes a character at index %s\n", TIGStringOutput(theString1));
#endif

		TIGNumberStartStack(NULL);
			// Create some number data.
			TIGValue *theNumber1 = TIGNumberInput(NULL, 10.0);
			// Converts a TIGValue number to a TIGValue string.
			theString1 = TIGStringFromNumber(theNumber1);
#ifdef TIGSTRING_PRINTF
			printf("TIGString number converted to string %s\n", TIGStringOutput(theString1));
#endif
		TIGNumberEndStack(NULL);

		// Tests if the the two strings have the same text string.
		TIGBool isEqual = TIGStringEqualsString(theString1, theString2);
		// Prints out "1" for TIGYes and "0" for TIGNo.
#ifdef TIGSTRING_PRINTF
		printf("TIGString is theString1 equal to theString2? %d\n", isEqual);
#endif
	
		// The object type can be NULL, "Array", "Number", "Object", or "String".
		// This is useful when trying to determine the type of output from an array or object.
		TIGValue *theString3 = TIGStringObjectType(theString1);
#ifdef TIGSTRING_PRINTF
		printf("TIGString the TIGValue variable type is %s\n", TIGStringOutput(theString3));
#endif
		// This adds the special characters back. TIGStringRemoveEscapeCharacters() supports most escape characters that the JSON and printf() formats support.
		TIGValue *theString4 = TIGStringInput(NULL, "T \" H \\ E / S \b T \f R \n I \r N \n G \t");
		theString4 = TIGStringAddEscapeCharacters(theString4);
#ifdef TIGSTRING_PRINTF
		printf("TIGString Escape Characters Added %s\n", TIGStringOutput(theString4));
#endif
	
		// This removes the special characters. TIGStringRemoveEscapeCharacters() supports most escape characters that the JSON and printf() formats support.
		TIGValue *theString5 = TIGStringRemoveEscapeCharacters(theString4);
#ifdef TIGSTRING_PRINTF
		printf("TIGString Escape Characters Removed %s\n", TIGStringOutput(theString5));
#endif

		// Create some string data.
		theString1 = TIGStringInput(NULL, "String 1");
		// This outputs a string, float, and integer. TIGVTIGStringWithFormat() supports all characters that printf() does.
		// If a TIGString value is used instead of "NULL" the value of the TIGString is added to the beginning of the string.
		theString1 = TIGStringWithFormat(NULL, "%s %f %d", TIGStringOutput(theString1), 10.0, 1);
#ifdef TIGSTRING_PRINTF
		printf("TIGString format string %s\n", TIGStringOutput(theString1));
#endif

		// Create some string data.
		theString1 = TIGStringInput(NULL, "String 1");
		// This adds the second string to the end of the first string.
		theString1 = TIGStringWithAddedString(theString1, theString2);
#ifdef TIGSTRING_PRINTF
		printf("TIGString second string added to first string %s\n", TIGStringOutput(theString1));
#endif
		TIGObjectStartStack(NULL);
			// Create some object data.
			TIGValue *theObject1 = TIGObjectAddStringWithValue(NULL, theString1, theString2);
	
			// This converts a TIGObject to a TIGString in JSON format.
			TIGValue *theString6 = TIGStringFromObject(theObject1);
#ifdef TIGSTRING_PRINTF
			printf("TIGString the output of a JSON string %s\n", TIGStringOutput(theString6));
#endif

			// This condenses the object to a string without tabs, spaces, and return characters.
			theString6 = TIGStringFromObjectForNetwork(theObject1);
#ifdef TIGSTRING_PRINTF
			printf("TIGString condensed JSON string %s\n", TIGStringOutput(theString6));
#endif
		TIGObjectEndStack(NULL);
	
		TIGArrayStartStack(NULL);
			// Create some array data.
			TIGValue *theArray1 = TIGArrayAddValue(NULL, theString1);

			// This converts a TIGArray to a TIGString in JSON format.
			theString1 = TIGStringFromArray(theArray1);
#ifdef TIGSTRING_PRINTF
			printf("TIGString the output of a JSON array to a string %s\n", TIGStringOutput(theString1));
#endif
	
			// This condenses the array to a string without tabs, spaces, and return characters.
			theString1 = TIGStringFromArrayForNetwork(theArray1);
#ifdef TIGSTRING_PRINTF
			printf("TIGString the output of a JSON array to a string %s\n", TIGStringOutput(theString1));
#endif
		TIGArrayEndStack(NULL);

		// This is the filename for the string. Note that ~/ cannot be used but using a name with or without a / is valid.
		TIGValue *theFilename1 = TIGStringInput(NULL, "Tiger");
		// Note if you want to save a JSON file theString6 has to be a TIGObject converted from TIGStringFromObject().
		TIGStringWriteWithFilename(theString6, theFilename1);
#ifdef TIGSTRING_PRINTF
		printf("TIGString write a string to a file %s\n", TIGStringOutput(TIGStringReadFromFilename(theFilename1)));
#endif

		// This reads the file's text string and converts it to a TIGString.
		theString6 = TIGStringReadFromFilename(theFilename1);
#ifdef TIGSTRING_PRINTF
		printf("TIGString read the text string in a file %s\n", TIGStringOutput(theString6));
#endif
	
		// Create some string data.
		TIGValue *theString7 = TIGStringInput(NULL, "String");
		// Tests if the the first string has the second string as a prefix.
		isEqual = TIGStringPrefix(theString2, theString7);
		// Prints out "1" for TIGYes and "0" for TIGNo.
#ifdef TIGSTRING_PRINTF
		printf("TIGString is \"String\" a prefix? %d\n", isEqual);
#endif

		// Tests if the the first string has the second string as a suffix.
		isEqual = TIGStringSuffix(theString2, theString7);
		// Prints out "1" for TIGYes and "0" for TIGNo.
#ifdef TIGSTRING_PRINTF
		printf("TIGString is \"String\" a suffix? %d\n", isEqual);
#endif
	
	TIGStringEndStack(NULL);
}
