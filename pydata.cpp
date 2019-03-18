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

#include "pydata.h"

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

using namespace PyData;

bool error_survival = true;
string separator    = "/";
string datatypes[]  = {
    "null", "bool", "number",
    "string", "dict", "list"
};

Data::Data() {
    initialize();
}

Data::Data(const string &s) {
    initialize();
    str = s;
    datatype = t_string;
}

Data::Data(const double &n) {
    initialize();
    num = n;
    datatype = t_number;
}

Data::Data(const int &n) {
    initialize();
    num = (double)n;
    datatype = t_number;
}

Data::Data(const long &n) {
    initialize();
    num = (double)n;
    datatype = t_number;
}

Data::Data(char b) {
    initialize();
    bln = (bool)b;
    datatype = t_bool;
}

Data::Data(const dict &m) {
    initialize();
    dct = m;
    datatype = t_dict;
}

Data::Data(const list &v) {
    initialize();
    lst = v;
    datatype = t_list;
}

Datatype Data::get_datatype() const {
    return datatype;
}

string Data::json() {
    output_buffer = "";
    json_recursive(*this, ROOT);
    remove_trailing_comma();
    return Data(output_buffer).trim().str;
}

void Data::append(const Data &data) {
    datatype_must_be(t_list);
    lst.push_back(data);
}

void Data::append(const double &n) {
    datatype_must_be(t_list);
    lst.push_back(n);
}

void Data::append(const string &s) {
    datatype_must_be(t_list);
    lst.push_back(s);
}

Data *Data::get(const int &index) {
    crash_and_burn_if(
        datatype != t_list,
        "Datatype must be 'list' (invalid get)"
    );
    crash_and_burn_if(
        index < 0 || index >= (int)lst.size(),
        "Index error (out of bounds): "
        + number_to_string(index)
    );
    IF_IT_SURVIVES {
        return NULL;
    }
    return &lst[index];
}

Data *Data::get(const string &key) {
    crash_and_burn_if(
        datatype != t_dict,
        "Datatype must be 'dict' (invalid get)"
    );
    IF_IT_SURVIVES {
        return NULL;
    }
    dict::iterator it;
    it = dct.find(key);
    if (it == dct.end()) {
        return NULL;
    }
    return &dct[key];
}

Data *Data::get_data_at(const string &path) {
    Data *data = this;
    char *cstr = new char[path.size() + 1];
    char *sepr = new char[separator.size() + 1];
    strcpy(cstr, path.c_str());
    strcpy(sepr, separator.c_str());

    int index;
    char *key = strtok(cstr, sepr);
    while (key != 0)
    {
        if (data->datatype == t_dict) {
            dict::iterator it;
            it = data->dct.find(key);
            crash_and_burn_if(
                it == data->dct.end(),
                "Key error (not found): " + string(key)
            );
            IF_IT_SURVIVES {
                data = NULL;
                break;
            }
            data = &(*data)[key];
        }
        else if (data->datatype == t_list) {
            crash_and_burn_if(
                !is_number(key),
                "Index error (NaN+): " + string(key)
            );
            IF_IT_SURVIVES {
                data = NULL;
                break;
            }
            index = atoi(key);
            crash_and_burn_if(
                index >= (int)data->lst.size(),
                "Index error (out of bounds): " + string(key)
            );
            IF_IT_SURVIVES {
                data = NULL;
                break;
            }
            data = &(*data)[index];
        }
        key = strtok(NULL, sepr);
    }
    delete[] cstr;
    delete[] sepr;
    return data;
}

bool Data::set_data_at(const string &path, Data new_data) {
    Data *data = this;
    char *cstr = new char[path.size() + 1];
    char *sepr = new char[separator.size() + 1];
    strcpy(cstr, path.c_str());
    strcpy(sepr, separator.c_str());

    int index;
    char *key = strtok(cstr, sepr);
    while (key != 0)
    {
        if (data->datatype == t_dict) {
            dict::iterator it;
            it = data->dct.find(key);
            crash_and_burn_if(
                it == data->dct.end(),
                "Key error (not found): " + string(key)
            );
            IF_IT_SURVIVES {
                data = NULL;
                break;
            }
            data = &(*data)[key];
        }
        else if (data->datatype == t_list) {
            crash_and_burn_if(
                !is_number(key),
                "Index error (NaN+): " + string(key)
            );
            IF_IT_SURVIVES {
                data = NULL;;
                break;
            }
            index = atoi(key);
            crash_and_burn_if(
                index >= (int)data->lst.size(),
                "Index error (out of bounds): " + string(key)
            );
            IF_IT_SURVIVES {
                data = NULL;
                break;
            }
            data = &(*data)[index];
        }
        key = strtok(NULL, sepr);
    }
    delete[] cstr;
    delete[] sepr;
    if (!data) {
        return false;
    }
    *data = new_data;
    return true;
}

bool Data::set_data_at(const string &path, string new_data) {
    return set_data_at(path, Data(new_data));
}

bool Data::endswith(const string &substr) {
    datatype_must_be(t_string);
    return str.rfind(substr) == str.size() - substr.size();
}

bool Data::startswith(const string &substr) {
    datatype_must_be(t_string);
    return str.find(substr) == 0;
}

int Data::find(const string &substr) {
    datatype_must_be(t_string);
    size_t index = str.find(substr);
    if (index == string::npos) {
        return -1;
    }
    return index;
}

int Data::find(const char &c) {
    datatype_must_be(t_string);
    size_t index = str.find(c);
    if (index == string::npos) {
        return -1;
    }
    return index;
}

Data Data::char_at(const int &index) {
    datatype_must_be(t_string);
    if (index < 0) {
        return string(1, str[str.size() + index]);
    }
    return string(1, str[index]);
}

Data Data::split() {
    datatype_must_be(t_string);
    return split(" ");
}

Data Data::split(const string &splitter) {
    datatype_must_be(t_string);
    Data result = list();
    char *cstr = new char[str.size() + 1];
    char *splt = new char[splitter.size() + 1];
    strcpy(cstr, str.c_str());
    strcpy(splt, splitter.c_str());
    char *element = strtok(cstr, splt);
    while (element != 0) {
        result.append(string(element));
        element = strtok(NULL, splt);
    }
    delete[] cstr;
    delete[] splt;
    return result;
}

Data Data::rtrim() {
    datatype_must_be(t_string);
    return _rtrim(str);
}

Data Data::ltrim() {
    datatype_must_be(t_string);
    return _ltrim(str);
}

Data Data::trim() {
    datatype_must_be(t_string);
    return _ltrim(_rtrim(str));
}

bool Data::empty() {
    switch (datatype) {
        case t_null:
            return true;

        case t_number:
            return num == 0;

        case t_string:
            return str.empty();

        case t_list:
            return lst.empty();

        case t_dict:
            return dct.empty();

        default:
            return true;
    }
}

bool Data::has_key(string key) {
    dict::iterator it;
    it = dct.find(key);
    return it != dct.end();
}

void Data::operator= (const string &s) {
    initialize();
    str = s;
    datatype = t_string;
}

void Data::operator= (const double &n) {
    initialize();
    num = n;
    datatype = t_number;
}

void Data::operator= (const int &n) {
    initialize();
    num = (double)n;
    datatype = t_number;
}

void Data::operator= (const long &n) {
    initialize();
    num = (double)n;
    datatype = t_number;
}

void Data::operator= (char b) {
    initialize();
    bln = (bool)b;
    datatype = t_bool;
}

void Data::operator= (dict m) {
    initialize();
    dct = m;
    datatype = t_dict;
}

void Data::operator= (list v) {
    initialize();
    lst = v;
    datatype = t_list;
}

Data & Data::operator[] (const string &key) {
    datatype_must_be(t_dict);
    return dct[key];
}

Data & Data::operator[] (const int &index) {
    datatype_must_be(t_list);
    if (index < 0) {
        return lst[lst.size() + index];
    }
    return lst[index];
}

void Data::initialize() {
    num      = 0;
    str      = "";
    bln      = false;
    is_error = false;
    is_fatal = false;
    datatype = t_null;
    #if __cplusplus < 199711L
    _dct     = NEW_VMAP;
    #endif
}

void Data::crash_and_burn_if(const bool &condition, const string &error_message) {
    is_error = false;
    if (condition) {
        is_error = true;
        using namespace std;
        cerr << "> PyData ERROR [ " << error_message << " ]" << endl;
        if (error_survival && !is_fatal) return;
        exit(EXIT_FAILURE);
    }
}

bool Data::it_survives() {
    if (!is_error) return false;
    is_error = false;
    return error_survival;
}

long Data::floor(const double &n) {
    int i = n - 2;
    while (++i <= n - 1);
    return i;
}

bool Data::is_number(const string &s)
{
    string::const_iterator it = s.begin();
    while (it != s.end() && isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

string Data::number_to_string(const double &n) {
    using namespace std;
    string number = to_string(n);
    int i = number.size();
    if (n == floor(n)) {
        return to_string((long)n);
    }
    while (number[--i] == '0');
    return number.substr(0, i + 1);
}

void Data::remove_trailing_comma() {
    size_t index_dct, index_lst, index_elm;
    string buffer = output_buffer;
    index_dct = buffer.rfind('{');
    index_lst = buffer.rfind('[');
    index_elm = buffer.rfind(',');
    bool dontRemoveComma = (
        (index_elm == string::npos)                          ||
        (index_dct != string::npos && index_dct > index_elm) ||
        (index_lst != string::npos && index_lst > index_elm)
    );
    if (dontRemoveComma) return;
    buffer.erase(buffer.rfind(','), 1);
    output_buffer = buffer;
}

void Data::indent(const int &level) {
    for (int i = 0; i < level * 4; ++i) {
        output_buffer += " ";
    }
}

void Data::print_key(const string &key) {
    output_buffer += "\"" + key + "\": ";
}

void Data::print_value(Data &value) {
    switch (value.get_datatype()) {
        case t_null:
            output_buffer += "null,\n";
            break;

        case t_bool:
            value.bln ? output_buffer += "true" : output_buffer += "false";
            output_buffer += ",\n";
            break;

        case t_string:
            output_buffer += "\"" + value.str + "\",\n";
            break;

        case t_number:
            output_buffer += number_to_string(value.num) + ",\n";
            break;

        default:
            break;
    }
}

void Data::print_content(const string &key, Data &value, const int &level) {
    indent(level);
    if (!key.empty()) {
        print_key(key);
    }
    print_value(value);
    if (value.get_datatype() >= t_dict) {
        json_recursive(value, level);
    }
}

bool Data::iterate_dictionary(dict &m, const int &level) {
    dict::iterator i;
    for (i = m.begin(); i != m.end(); ++i) {
        string  key = i->first;
        Data &value = i->second;
        print_content(key, value, level);
    }
    return !m.empty();
}

bool Data::iterate_list(list &v, const int &level) {
    list::iterator i;
    for (i = v.begin(); i != v.end(); ++i) {
        string withoutKey; Data &value = *i;
        print_content(withoutKey, value, level);
    }
    return !v.empty();
}

bool Data::json_recursive(Data &object, const int &level) {
    bool result = false;
    switch (object.get_datatype()) {
        case t_null:
        case t_bool:
        case t_number:
        case t_string:
            indent(level);
            print_value(object);
            result = true;
            break;

        case t_dict:
            output_buffer += "{";
            if (!object.dct.empty()) {
                output_buffer += "\n";
            }
            result = iterate_dictionary(object.dct, level + 1);
            if (result) {
                remove_trailing_comma();
                indent(level);
            }
            output_buffer += "},\n";
            break;

        case t_list:
            output_buffer += "[";
            if (!object.lst.empty()) {
                output_buffer += "\n";
            }
            result = iterate_list(object.lst, level + 1);
            if (result) {
                remove_trailing_comma();
                indent(level);
            }
            output_buffer += "],\n";
            break;

        default:
            break;
    }
    return result;
}

string Data::_rtrim(string s) {
    string whitespace(" \t\f\v\n\r");
    size_t found = s.find_last_not_of(whitespace);
    if (found != string::npos) {
        s.erase(found + 1);
    }
    else {
        s = "";
    }
    return s;
}

string Data::_ltrim(string s) {
    string whitespace(" \t\f\v\n\r");
    size_t found = s.find_first_not_of(whitespace);
    if (found != string::npos) {
        s.erase(0, found);
    }
    else {
        s = "";
    }
    return s;
}

void Data::datatype_must_be(const Datatype &d_type) {
    is_fatal = true;
    string \
    error_message  = "Datatype must be '";
    error_message += datatypes[d_type];
    error_message += "' (type error)";
    crash_and_burn_if(
        datatype != d_type,
        error_message
    );
    is_fatal = false;
}

void Data::datatypes_must_be(const Datatype &d_type1, const Datatype &d_type2) {
    is_fatal = true;
    string \
    error_message  = "Datatypes must be either '";
    error_message += datatypes[d_type1];
    error_message += "' or '";
    error_message += datatypes[d_type2];
    error_message += "' (type error)";
    crash_and_burn_if(
        datatype != d_type1 &&
        datatype != d_type2,
        error_message
    );
    is_fatal = false;
}

void PyData::set_error_handling(bool mode) {
    error_survival = !mode;
}

void PyData::set_path_separator(const string &s) {
    separator = s;
}
