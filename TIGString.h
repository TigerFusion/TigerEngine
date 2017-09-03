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

#ifndef TIGSTRING_H
#define TIGSTRING_H

// ******* TIGString Stuff *******

// ************* For debugging Purposes *************
// The stack will show the freed data but it won't show up as freed or NULLed in the TIGString pointer
// Also make sure any TIGString pointer equals NULL before using it otherwise it will not equal NULL and not use the "TIGStringCreate" function
TIGValue *TIGStringNumberStack(void);
TIGValue *TIGStringStringStack(void);
// ************* For debugging Purposes *************

void TIGStringStartStack(const char *startStackString);
void TIGStringEndStack(const char *endStackString);

TIGValue *TIGStringCreate(TIGValue *tigString, TIGBool useStack);
TIGValue *TIGStringDestroy(TIGValue *tigString);

TIGValue *TIGStr(const char *string);
TIGValue *TIGStringInput(TIGValue *tigString, const char *string);
TIGValue *TIGStringStackInput(TIGValue *tigString, const char *string, TIGBool useStack);

char *TIGStringOutput(TIGValue *tigString);
TIGInteger TIGStringLength(TIGValue *tigString);

TIGValue *TIGStringInsertStringAtIndex(TIGValue *tigString1, TIGValue *tigString2, int index);
TIGValue *TIGStringCharacterAtIndex(TIGValue *tigString, int index);
void TIGStringRemoveCharacterAtIndex(TIGValue *tigString, int index);

TIGValue *TIGStringFromNumber(TIGValue *tigNumber);
TIGBool TIGStringEqualsString(TIGValue *tigString1, TIGValue *tigString2);
TIGValue *TIGStringObjectType(TIGValue *tigObject);

TIGValue *TIGStringAddEscapeCharacters(TIGValue *tigstring);
TIGValue *TIGStringRemoveEscapeCharacters(TIGValue *tigstring);

TIGValue *TIGStringWithFormat(TIGValue *tigString, const char *format, ...);
TIGValue *TIGStringWithAddedString(TIGValue *oldTigString, TIGValue *newTigString);

TIGValue *TIGStringFromObject(TIGValue *tigObject);
TIGValue *TIGStringFromObjectForNetwork(TIGValue *tigObject);

TIGValue *TIGStringFromArray(TIGValue *tigArray);
TIGValue *TIGStringFromArrayForNetwork(TIGValue *tigArray);

TIGValue *TIGStringFromObjectWithLevel(TIGValue *tigString, TIGValue *tigValue, int level, TIGBool useEscapeCharacters);

void TIGStringWriteWithFilename(TIGValue *tigString, TIGValue *filenameString);
TIGValue *TIGStringReadFromFilename(TIGValue *filenameString);

TIGBool TIGStringPrefix(TIGValue *tigString, TIGValue *tigStringPrefix);
TIGBool TIGStringSuffix(TIGValue *tigString, TIGValue *tigStringSuffix);

#endif //TIGSTRING_H