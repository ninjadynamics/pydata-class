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
We need to use VMAP (Vector of Maps) in order to trick
older compilers into accepting a similar construction to
Class Data { map<string, Data> dct; ... };
*/

#define NULL_DATA       Data()
#define NEW_VMAP        vmap(); _dct.push_back(dict())
#define PAUSE(x)        std::cout << x << std::flush; getchar()
#define IF_IT_SURVIVES  if (it_survives())

#if __cplusplus <= 199711L
#include <stdio.h>
#include <stdlib.h>
std::string to_string(double value)
{
    char numbuffer[32];
    sprintf(numbuffer, "%f", value);
    return numbuffer;
}

std::string to_string(long value)
{
    char numbuffer[32];
    sprintf(numbuffer, "%ld", value);
    return numbuffer;
}
#endif

namespace PyData {
    const size_t  ROOT  = 0;
    const bool    SOFT  = false;
    const bool    HARD  = true;
    const char    FALSE = '\0';
    const char    TRUE  = '\1';

    #if __cplusplus < 199711L
    #define dct _dct[0]
    #endif

    typedef std::string string;
    typedef enum Datatype {
        t_null,
        t_bool,
        t_number,
        t_string,
        t_dict,
        t_list
    } Datatype;

    class    Data;
    Datatype datatype;
    string   separator = "/";
    bool     error_survival = true;
    string   datatypes[] = {"null", "bool", "number", "string", "dict", "list"};

    typedef  std::map    <std::string, Data> dict;
    typedef  std::vector <Data             > list;
    typedef  std::vector <dict             > vmap;

    void set_error_handling(bool mode);
    void set_path_separator(const string &s);
}

#endif /* End of include guard: PYDATA_HEADER */
