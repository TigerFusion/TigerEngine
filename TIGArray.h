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

#ifndef TIGARRAY_H
#define TIGARRAY_H
// ******* TIGArray Stuff *******

// ************* For debugging Purposes *************
// The stack will show the freed data but it won't show up as freed or NULLed in the TIGArray pointer
// Also make sure any TIGArray pointer equals NULL before using it otherwise it will not equal NULL and not use the "TIGArrayCreate" function
TIGValue *TIGArrayNumberStack(void);
TIGValue *TIGArrayStringStack(void);
// ************* For debugging Purposes *************

void TIGArrayStartStack(const char *startStackString);
void TIGArrayEndStack(const char *endStackString);

TIGValue *TIGArrayCreate(TIGValue *tigArray, TIGBool useStack);
TIGValue *TIGArrayDestroy(TIGValue *tigArray);

TIGValue *TIGArr(TIGValue *tigValue);
TIGValue *TIGArrayAddValue(TIGValue *tigArray, TIGValue *tigValue);
TIGValue *TIGArrayInsertValueAtIndex(TIGValue *tigArray, TIGValue *tigValue, int tigIndex);
TIGValue *TIGArrayStackInsertValueAtIndex(TIGValue *tigArray, TIGValue *tigValue, int tigIndex, TIGBool useStack);

TIGValue *TIGArrayValueAtIndex(TIGValue *tigArray, int tigArrayIndex);
void TIGArrayRemoveValueAtIndex(TIGValue *tigArray, int tigArrayIndex);

TIGInteger TIGArrayCount(TIGValue *tigArray);
void TIGArrayRemoveAllValues(TIGValue *tigArray);

TIGValue *TIGArrayReplaceValueAtIndex(TIGValue *tigArray, TIGValue *tigValue, int tigArrayIndex);

TIGValue *TIGArrayOfObjectStrings(TIGValue *tigObject);
TIGValue *TIGArrayOfObjectValues(TIGValue *tigObject);

TIGValue *TIGArrayInsertValuesFromArrayAtIndex(TIGValue *tigOldArray, TIGValue *tigNewArray, int index);
TIGValue *TIGArrayAddValuesFromArray(TIGValue *tigOldArray, TIGValue *tigNewArray);

TIGValue *TIGArrayFirstValue(TIGValue *tigArray);
TIGValue *TIGArrayLastValue(TIGValue *tigArray);

TIGBool TIGArrayEqualsArray(TIGValue *tigArray1, TIGValue *tigArray2);

#endif // TIGARRAY_H