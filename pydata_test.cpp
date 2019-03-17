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
            datatype_must_be(t_list);
            lst.push_back(data);
        }

        void append(const double &n) {
            datatype_must_be(t_list);
            lst.push_back(n);
        }

        void append(const string &s) {
            datatype_must_be(t_list);
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

        bool endswith(const string &substr) {
            datatype_must_be(t_string);
            return str.rfind(substr) == str.size() - substr.size();
        }

        bool startswith(const string &substr) {
            datatype_must_be(t_string);
            return str.find(substr) == 0;
        }

        size_t find(const string &substr) {
            datatype_must_be(t_string);
            size_t index = str.find(substr);
            if (index == string::npos) {
                return -1;
            }
            return index;
        }

        size_t find(const char &c) {
            datatype_must_be(t_string);
            size_t index = str.find(c);
            if (index == string::npos) {
                return -1;
            }
            return index;
        }

        Data char_at(const int &index) {
            datatype_must_be(t_string);
            if (index < 0) {
                return string(1, str[str.size() + index]);
            }
            return string(1, str[index]);
        }

        Data split() {
            datatype_must_be(t_string);
            return split(" ");
        }

        Data split(const string &splitter) {
            datatype_must_be(t_string);
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

        Data rtrim() {
            datatype_must_be(t_string);
            return _rtrim(str);
        }

        Data ltrim() {
            datatype_must_be(t_string);
            return _ltrim(str);
        }

        Data trim() {
            datatype_must_be(t_string);
            return _ltrim(_rtrim(str));
        }

        bool empty() {
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

            }
        }

        bool has_key(string key) {
            DICT::iterator it;
            it = dct.find(key);
            return it != dct.end();
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
            datatype_must_be(t_dict);
            return dct[key];
        }

        Data & operator[] (const int &index) {
            datatype_must_be(t_list);
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

        string _rtrim(string s) {
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

        string _ltrim(string s) {
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

        void datatype_must_be(const Datatype &d_type) {
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

        void datatypes_must_be(const Datatype &d_type1, const Datatype &d_type2) {
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

int main() {
    //Note: Empty keys ("") can be problematic for JSON output!

    Data data = dict();
    data["A (string)"]      = "str_value";
    data["B (zero int)"]    = 0;
    data["C (zero long)"]   = 0l;
    data["D (zero float)"]  = 0.0f;
    data["E (zero double)"] = 0.0;
    data["F (num int)"]     = 5;
    data["G (num long)"]    = 5l;
    data["H (num float)"]   = 5.5f;
    data["I (num double)"]  = 5.5;
    data["J (dict)"]        = dict();
    data["K (zero string)"] = "";
    data["L (list)"]        = list();
    cout << "Data: " << data.json() << endl;


    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    cout << endl;
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


    data = 1;
    cout << "Data: " << data.json();

    data = "A";
    cout << "Data: " << data.json();

    data = dict();
    cout << "Data: " << data.json();

    data = list();
    cout << "Data: " << data.json();


    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    cout << endl << endl;
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


    // Direct Access (write)
    data = dict();
    data["bruno"] = dict();
    data["bruno"]["home"]    = "Setubal";
    data["bruno"]["country"] = "Portugal";
    data["bruno"]["name"]    = "Bruno";
    data["bruno"]["surname"] = "Silva";
    data["bruno"]["age"]     = 35;
    data["bruno"]["zero_i"]  = 0;
    data["bruno"]["zero_l"]  = 0l;
    data["bruno"]["zero_f"]  = 0.0f;
    data["bruno"]["zero_d"]  = 0.0;
    data["bruno"]["stuff"]   = dict();
        data["bruno"]["stuff"]["phone"] = "iPhone";
        data["bruno"]["stuff"]["numbers"] = list();
            data["bruno"]["stuff"]["numbers"].append(1);
            data["bruno"]["stuff"]["numbers"].append(2);
            data["bruno"]["stuff"]["numbers"].append(3.01);
            data["bruno"]["stuff"]["numbers"].append("hello");

            // Alternative: data["bruno"]["stuff"]["numbers"][3] = dict();
            // Alternative: data["bruno"]["stuff"]["numbers"][-1] = dict();
            data["bruno"]["stuff"]["numbers"].append(dict());
                data["bruno"]["stuff"]["numbers"][4]["new key"] = "hello";

    // Alternative: data["bruno"]["stuff"]["numbers"][4] = list();
    // Alternative: data["bruno"]["stuff"]["numbers"][-1] = list();
        data["bruno"]["stuff"]["numbers"].append(list());
            data["bruno"]["stuff"]["numbers"][5].append(999);
            data["bruno"]["stuff"]["numbers"][5].append(777);
            data["bruno"]["stuff"]["numbers"][5].append(dict());
            data["bruno"]["stuff"]["numbers"][5].append(list());
    cout << "Data: " << data.json();


    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    cout << endl << endl;
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


    // Direct Access (read)
    data["bruno"] = "silva";
        cout << data["bruno"].str << "\n";
        cout << data["bruno"].char_at(-2).str << "<\n";
        cout << data["bruno"].endswith("va") << "<\n";
        cout << data["bruno"].endswith("wa") << "<\n";
        cout << data["bruno"].startswith("sil") << "<\n";
        cout << data["bruno"].startswith("syl") << "<\n";
        cout << data["bruno"].find("lv") << "<\n";
        // cout << data.startswith("syl") << "<\n";

    data["bruno"] = 1983;
        cout << data["bruno"].num << "\n";

    data["bruno"] = dict();
    data["bruno"]["home"] = "setubal";
        cout << data["bruno"]["home"].str << "\n";
        cout << data["bruno"].get("home")->str << "\n";
        data["bruno"]["list"] = list();
        data["bruno"]["list"].append("bananas");
        data["bruno"]["list"].get(12);
    data["bruno"].get("listaaaa");
        cout << data["bruno"]["list"].json();

    cout << "data.get_data_at(): " << data.get_data_at("/bruno/home/")->str << "\n";
    cout << "data.get_data_at(): " << data.get_data_at("/bruno/list/0/")->str << "\n";
    cout << "data.get_data_at(): " << data.get_data_at("/bruno/list/1/") << "\n";    //expect null pointer
    cout << "data.get_data_at(): " << data.get_data_at("/bruno/list/-1/") << "\n";   //expect null pointer
    cout << "data.get_data_at(): " << data.get_data_at("/bruno/list/aaa/") << "\n";  //expect null pointer
    cout << "data.get_data_at(): " << data.get_data_at("/bruno/asdsadds/") << "\n";  //expect null pointer

    data.set_data_at("/bruno/home/", "lisbon");
    cout << "data.get_data_at(): " << data.get_data_at("/bruno/home/")->str << "\n";

    cout << data.get_data_at("/")->json() << "\n";

    data["bruno"] = dict();

    data["bruno"] = list();
    data["bruno"].lst.push_back(Data(123));
    data["bruno"].lst.push_back(Data("ABC"));
    data["bruno"].append(Data(456));
        cout << data["bruno"][0].num << "\n";
        cout << data["bruno"][1].str << "\n";
        cout << data["bruno"][2].num << "\n";
        cout << data["bruno"][-1].num << "\n";

    cout << data.has_key("bruno") << "<\n";

    cout << "Datatype of data[\"bruno\"]: "<< data["bruno"].get_datatype() << "\n";

    data = " Hello World! ";
    cout << ">" << data.ltrim().str << "<\n";
    cout << ">" << data.rtrim().str << "<\n";
    cout << ">" << data.trim().str << "<\n";

    data = string();
    cout << data.empty() << "<\n";

    data = list();
    cout << data.empty() << "<\n";

    data = dict();
    cout << data.empty() << "<\n";

    data = 0;
    cout << data.empty() << "<\n";

    return 0;
}
