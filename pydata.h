/*
MIT License

PyData - Pythonic Datatypes for C++
Copyright (c) 2019 Bruno Silva / Ninja Dynamics

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
SOFTWARE.
*/

#ifndef PYDATA_HEADER
#define PYDATA_HEADER
#if __cplusplus < 199711L
#include <iostream.h>
#else
#include <iostream>
#endif
#include <cstring>
#include <string>
#include <vector>
#include <map>

/*
Notes:

We need to use VMAP (Vector of Maps) in order to trick
older compilers into accepting a similar construction to
Class Data { map<string, Data> dct; ... };

Regarding the extra whitespace placed around the angled
brackets (</>), this is entirely necessary due to the
fact that those same older compilers will confuse a double
closing (...ing, Data>>) with a bitshift operation (>>).
*/

#define ROOT            0
#define SOFT            false
#define HARD            true
#define DICT            map    < string, Data >
#define LIST            vector < Data         >
#define VMAP            vector < DICT         >
#define NEW_VMAP        VMAP(); _dct.push_back(DICT())
#define IF_IT_SURVIVES  if (it_survives())
#define PAUSE(x)        cout << x << endl; getchar()

#if __cplusplus < 199711L
#define dct _dct[0]
#endif

using namespace std;

#if __cplusplus <= 199711L
#include <stdio.h>
#include <stdlib.h>
string to_string(double value)
{
    char numbuffer[32];
    sprintf(numbuffer, "%f", value);
    return numbuffer;
}

string to_string(long value)
{
    char numbuffer[32];
    sprintf(numbuffer, "%ld", value);
    return numbuffer;
}
#endif

class  Data;
string separator = "/";
bool   error_survival = true;

#endif /* End of include guard: PYDATA_HEADER */
