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

#ifndef TIGNUMBER_H
#define TIGNUMBER_H
// ******* TIGNumber Stuff *******

// ************* For debugging Purposes *************
// The stack will show the freed data but it won't show up as freed or NULLed in the TIGNumber pointer
// Also make sure any TIGNumber pointer equals NULL before using it otherwise it will not equal NULL and not use the "TIGNumberCreate" function
TIGValue *TIGNumberNumberStack(void);
TIGValue *TIGNumberStringStack(void);
// ************* For debugging Purposes *************

void TIGNumberStartStack(const char *startStackString);
void TIGNumberEndStack(const char *endStackString);

TIGValue *TIGNumberCreate(TIGValue *tigNumber, TIGBool useStack);
TIGValue *TIGNumberDestroy(TIGValue *tigNumber);

TIGValue *TIGNumberBooleanInput(TIGValue *tigNumber, TIGBool booleanValue);
TIGValue *TIGNumberBooleanStackInput(TIGValue *tigNumber, TIGBool booleanValue, TIGBool useStack);

TIGValue *TIGNum(TIGScalar number);
TIGValue *TIGNumberInput(TIGValue *tigNumber, TIGScalar number);
TIGValue *TIGNumberStackInput(TIGValue *tigNumber, TIGScalar number, TIGBool useStack);

TIGScalar TIGNumberOutput(TIGValue *tigNumber);
TIGScalar TIGNumberFromString(TIGValue *tigString);

TIGBool TIGNumberEqualsNumber(TIGValue *tigNumber1, TIGValue *tigNumber2);

#endif // TIGARRAY_H