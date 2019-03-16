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

class Data {
    public:
        typedef enum Datatype {
            t_null,      // 0
            t_number,    // 1
            t_string,    // 2
            t_dict,      // 3
            t_list       // 4
        } Datatype;

        double num;
        string str;
        #if __cplusplus < 199711L
        VMAP  _dct;
        #else
        DICT   dct;
        #endif
        LIST   lst;

        Data() {
            initialize();
        }

        Data(const string &s) {
            initialize();
            str = s;
            datatype = t_string;
        }

        Data(const double &n) {
            initialize();
            num = n;
            datatype = t_number;
        }

        Data(const int &n) {
            initialize();
            num = (double)n;
            datatype = t_number;
        }

        Data(const long &n) {
            initialize();
            num = (double)n;
            datatype = t_number;
        }

        Data(const DICT &m) {
            initialize();
            dct = m;
            datatype = t_dict;
        }

        Data(const LIST &v) {
            initialize();
            lst = v;
            datatype = t_list;
        }

        Datatype get_datatype() const {
            return datatype;
        }

        string json() {
            output_buffer = "";
            json_recursive(*this, ROOT);
            remove_trailing_comma();
            return output_buffer;
        }

        void append(const Data &data) {
            if (datatype != t_list) {
                return;
            }
            lst.push_back(data);
        }

        void append(const double &n) {
            if (datatype != t_list) {
                return;
            }
            lst.push_back(n);
        }

        void append(const string &s) {
            if (datatype != t_list) {
                return;
            }
            lst.push_back(s);
        }

        Data *get(const int &index) {
            crash_and_burn_if(
                datatype != t_list,
                "Datatype must be 'list' (invalid get)"
            );
            crash_and_burn_if(
                index < 0 || index >= lst.size(),
                "Index error (out of bounds): "
                + number_to_string(index)
            );
            IF_IT_SURVIVES {
                return NULL;
            }
            return &lst[index];
        }

        Data *get(const string &key) {
            crash_and_burn_if(
                datatype != t_dict,
                "Datatype must be 'dict' (invalid get)"
            );
            IF_IT_SURVIVES {
                return NULL;
            }
            DICT::iterator it;
            it = dct.find(key);
            if (it == dct.end()) {
                return NULL;
            }
            return &dct[key];
        }

        Data *get_data_at(const string &path) {
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
                    DICT::iterator it;
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
                        index >= data->lst.size(),
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

        bool set_data_at(const string &path, Data new_data) {
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
                    DICT::iterator it;
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
                        index >= data->lst.size(),
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

        bool set_data_at(const string &path, string new_data) {
            return set_data_at(path, Data(new_data));
        }

        Data split() {
            return split(" ");
        }

        Data split(const string &splitter) {
            is_fatal = true;
            crash_and_burn_if(
                datatype != t_string,
                "Datatype must be 'string' (type error)"
            );
            is_fatal = false;
            Data result = LIST();
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

        void operator= (const string &s) {
            initialize();
            str = s;
            datatype = t_string;
        }

        void operator= (const double &n) {
            initialize();
            num = n;
            datatype = t_number;
        }

        void operator= (const int &n) {
            initialize();
            num = (double)n;
            datatype = t_number;
        }

        void operator= (const long &n) {
            initialize();
            num = (double)n;
            datatype = t_number;
        }

        void operator= (DICT m) {
            initialize();
            dct = m;
            datatype = t_dict;
        }

        void operator= (LIST v) {
            initialize();
            lst = v;
            datatype = t_list;
        }

        Data & operator[] (const string &key) {
            is_fatal = true;
            crash_and_burn_if(
                datatype != t_dict,
                "Datatype must be 'dict' (invalid access)"
            );
            is_fatal = false;
            return dct[key];
        }

        Data & operator[] (const int &index) {
            is_fatal = true;
            crash_and_burn_if(
                datatype != t_list,
                "Datatype must be 'list' (invalid access)"
            );
            is_fatal = false;
            if (index < 0) {
                return lst[lst.size() + index];
            }
            return lst[index];
        }

    private:
        bool is_error;
        bool is_fatal;
        Datatype datatype;
        string output_buffer;

        void initialize() {
            num      = 0;
            str      = "";
            is_error = false;
            is_fatal = false;
            datatype = t_null;
            #if __cplusplus < 199711L
            _dct     = NEW_VMAP;
            #endif
        }

        void crash_and_burn_if(const bool &condition, const string &error_message) {
            is_error = false;
            if (condition) {
                is_error = true;
                cerr << "> PyData ERROR [ " << error_message << " ]" << endl;
                if (error_survival && !is_fatal) return;
                exit(EXIT_FAILURE);
            }
        }

        bool it_survives() {
            if (!is_error) return false;
            is_error = false;
            return error_survival;
        }

        long floor(const double &n) {
            int i = n - 2;
            while (++i <= n - 1);
            return i;
        }

        bool is_number(const string &s)
        {
            string::const_iterator it = s.begin();
            while (it != s.end() && isdigit(*it)) ++it;
            return !s.empty() && it == s.end();
        }

        string number_to_string(const double &n) {
            string number = to_string(n);
            int i = number.size();
            if (n == floor(n)) {
                return to_string((long)n);
            }
            while (number[--i] == '0');
            return number.substr(0, i + 1);
        }

        void remove_trailing_comma() {
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

        void indent(const int &level) {
            for (int i = 0; i < level * 4; ++i) {
                output_buffer += " ";
            }
        }

        void print_key(const string &key) {
            output_buffer += "\"" + key + "\": ";
        }

        void print_value(Data &value) {
            switch (value.get_datatype()) {
                case t_string:
                    output_buffer += "\"" + value.str + "\",\n";
                    break;

                case t_number:
                    output_buffer += number_to_string(value.num) + ",\n";
                    break;
            }
        }

        void print_content(const string &key, Data &value, const int &level) {
            indent(level);
            if (!key.empty()) {
                print_key(key);
            }
            print_value(value);
            if (value.get_datatype() >= t_dict) {
                json_recursive(value, level);
            }
        }

        bool iterate_dictionary(DICT &m, const int &level) {
            DICT::iterator i;
            for (i = m.begin(); i != m.end(); ++i) {
                string  key = i->first;
                Data &value = i->second;
                print_content(key, value, level);
            }
            return !m.empty();
        }

        bool iterate_list(LIST &v, const int &level) {
            LIST::iterator i;
            for (i = v.begin(); i != v.end(); ++i) {
                string withoutKey; Data &value = *i;
                print_content(withoutKey, value, level);
            }
            return !v.empty();
        }

        bool json_recursive(Data &object, const int &level) {
            bool result = false;
            switch (object.get_datatype()) {
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
            }
            return result;
        }
};

DICT dict() {
    return DICT();
}

LIST list() {
    return LIST();
}

void set_error_handling(bool mode) {
    error_survival = !mode;
}

void set_path_separator(const string &s) {
    separator = s;
}
