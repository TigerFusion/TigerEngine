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

#ifndef TIGOBJECT_H
#define TIGOBJECT_H

// ******* TIGObject Stuff *******

// ************* For debugging Purposes *************
// The stack will show the freed data but it won't show up as freed or NULLed in the TIGObject pointer
// Also make sure any TIGObject pointer equals NULL before using it otherwise it will not equal NULL and not use the "TIGObjectCreate" function
TIGValue *TIGObjectNumberStack(void);
TIGValue *TIGObjectStringStack(void);
// ************* For debugging Purposes *************

void TIGObjectStartStack(const char *startStackString);
void TIGObjectEndStack(const char *endStackString);

TIGValue *TIGStringCreate(TIGValue *tigObject, TIGBool useStack);
TIGValue *TIGObjectDestroy(TIGValue *tigObject);

TIGValue *TIGObj(TIGValue *tigString, TIGValue *tigValue);
TIGValue *TIGObjectAddStringWithValue(TIGValue *tigObject, TIGValue *tigString, TIGValue *tigValue);
TIGValue *TIGObjectStackAddStringWithValue(TIGValue *tigObject, TIGValue *tigString, TIGValue *tigValue, TIGBool useStack);
TIGValue *TIGObjectValueFromString(TIGValue *tigObject, TIGValue *tigString);
void TIGObjectRemoveValueFromString(TIGValue *tigObject, TIGValue *tigString);

int TIGObjectCount(TIGValue *tigObject);
void TIGObjectRemoveAllValues(TIGValue *tigObject);

TIGValue *TIGObjectFromString(TIGValue *tigString);
TIGValue *TIGObjectFromStringWithIndex(TIGValue *tigString, int startIndex);

TIGValue *TIGObjectAddValuesFromObject(TIGValue *tigOldObject, TIGValue *tigNewObject);
TIGBool TIGObjectEqualsObject(TIGValue *tigObject1, TIGValue *tigObject2);

#endif // TIGOBJECT_H