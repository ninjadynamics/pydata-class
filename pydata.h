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

#if __cplusplus < 199711L
#define dct _dct[0]
#endif

#define NULL_DATA       Data()
#define NEW_VMAP        vmap(); _dct.push_back(dict())
#define PAUSE(x)        std::cout << x << std::flush; getchar()
#define IF_IT_SURVIVES  if (it_survives())

namespace PyData {
    const size_t  ROOT  = 0;
    const bool    SOFT  = false;
    const bool    HARD  = true;
    const char    FALSE = '\0';
    const char    TRUE  = '\1';

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
    typedef  std::map    <std::string, Data> dict;
    typedef  std::vector <Data             > list;
    typedef  std::vector <dict             > vmap;

    class Data {
        public:
        bool   bln;
        double num;
        string str;
        #if __cplusplus < 199711L
        vmap  _dct;
        #else
        dict   dct;
        #endif
        list   lst;

        Data();
        Data(const string &s);
        Data(const double &n);
        Data(const int &n);
        Data(const long &n);
        Data(char b);
        Data(const dict &m);
        Data(const list &v);
        Datatype get_datatype() const;
        string json();
        void append(const Data &data);
        void append(const double &n);
        void append(const string &s);
        Data *get(const int &index);
        Data *get(const string &key);
        Data *get_data_at(const string &path);
        bool set_data_at(const string &path, Data new_data);
        bool set_data_at(const string &path, string new_data);
        bool endswith(const string &substr);
        bool startswith(const string &substr);
        int find(const string &substr);
        int find(const char &c);
        Data char_at(const int &index);
        Data split();
        Data split(const string &splitter);
        Data rtrim();
        Data ltrim();
        Data trim();
        bool empty();
        bool has_key(string key);
        void operator= (const string &s);
        void operator= (const double &n);
        void operator= (const int &n);
        void operator= (const long &n);
        void operator= (char b);
        void operator= (dict m);
        void operator= (list v);
        Data & operator[] (const string &key);
        Data & operator[] (const int &index);

        private:
        bool is_error;
        bool is_fatal;
        Datatype datatype;
        string output_buffer;

        void initialize();
        void crash_and_burn_if(const bool &condition, const string &error_message);
        bool it_survives();
        long floor(const double &n);
        bool is_number(const string &s);
        string number_to_string(const double &n);
        void remove_trailing_comma();
        void indent(const int &level);
        void print_key(const string &key);
        void print_value(Data &value);
        void print_content(const string &key, Data &value, const int &level);
        bool iterate_dictionary(dict &m, const int &level);
        bool iterate_list(list &v, const int &level);
        bool json_recursive(Data &object, const int &level);
        string _rtrim(string s);
        string _ltrim(string s);
        void datatype_must_be(const Datatype &d_type);
        void datatypes_must_be(const Datatype &d_type1, const Datatype &d_type2);
    };

    void set_error_handling(bool mode);
    void set_path_separator(const string &s);
}

#endif /* End of include guard: PYDATA_HEADER */
