
namespace __hidden__ {
    struct print {
        bool space;
        print() : space(false) {}
        ~print() { std::cout << std::endl; }

        template <typename T>
        print &operator , (const T &t) {
            if (space) std::cout << ' ';
            else space = true;
            std::cout << t;
            return *this;
        }
    };
}

#define print __hidden__::print(),

int main() {
    //Note: Empty keys ("") can be problematic for JSON output!

    using namespace PyData;

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
    data["M (null)"]        = NULL_DATA;
    data["N (bool)"]        = FALSE;
    data["O (bool)"]        = TRUE;
    print "Data: ", data.json();


    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    print "";
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


    data = 1;
    print "Data: ", data.json();

    data = "A";
    print "Data: ", data.json();

    data = dict();
    print "Data: ", data.json();

    data = list();
    print "Data: ", data.json();


    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    print "\n";
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
    print "Data: ", data.json();


    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    print "\n";
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


    // Direct Access (read)
    data["bruno"] = "silva";
        print data["bruno"].str;
        print data["bruno"].char_at(-2).str + "<";
        print data["bruno"].endswith("va") + "<";
        print data["bruno"].endswith("wa") + "<";
        print data["bruno"].startswith("sil") + "<";
        print data["bruno"].startswith("syl") + "<";
        print data["bruno"].find("lv") + "<";
        // print data.startswith("syl") + "<";

    data["bruno"] = 1983;
        print data["bruno"].num;

    data["bruno"] = dict();
    data["bruno"]["home"] = "setubal";
        print data["bruno"]["home"].str;
        print data["bruno"].get("home")->str;
        data["bruno"]["list"] = list();
        data["bruno"]["list"].append("bananas");
        data["bruno"]["list"].get(12);
    data["bruno"].get("listaaaa");
        print data["bruno"]["list"].json();

    print "data.get_data_at():", data.get_data_at("/bruno/home/")->str;
    print "data.get_data_at():", data.get_data_at("/bruno/list/0/")->str;
    print "data.get_data_at():", data.get_data_at("/bruno/list/1/");    //expect null pointer
    print "data.get_data_at():", data.get_data_at("/bruno/list/-1/");   //expect null pointer
    print "data.get_data_at():", data.get_data_at("/bruno/list/aaa/");  //expect null pointer
    print "data.get_data_at():", data.get_data_at("/bruno/asdsadds/");  //expect null pointer

    data.set_data_at("/bruno/home/", "lisbon");
    print "data.get_data_at():", data.get_data_at("/bruno/home/")->str;

    print data.get_data_at("/")->json();

    data["bruno"] = dict();

    data["bruno"] = list();
    data["bruno"].lst.push_back(Data(123));
    data["bruno"].lst.push_back(Data("ABC"));
    data["bruno"].append(Data(456));
        print data["bruno"][0].num;
        print data["bruno"][1].str;
        print data["bruno"][2].num;
        print data["bruno"][-1].num;

    print data.has_key("bruno") + "<";

    print "Datatype of data[\"bruno\"]: ", data["bruno"].get_datatype();

    data = " Hello World! ";
    print ">" + data.ltrim().str + "<";
    print ">" + data.rtrim().str + "<";
    print ">" + data.trim().str + "<";

    data = string();
    print "Is data(string) empty?", data.empty();

    data = list();
    print "Is data(list) empty?", data.empty();

    data = dict();
    print "Is data(dict) empty?", data.empty();

    data = 0;
    print "Is data(number) empty?", data.empty();


    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    print "\n";
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


    data = NULL_DATA;
    print "Empty data object:";
    print data.json();
    print "End of JSON output\n";

    data = dict();
    print "Empty root dict:";
    print data.json();
    print "End of JSON output\n";

    data = list();
    print "Empty root list:";
    print data.json();
    print "End of JSON output\n";

    print "Null value / null element / empty dict:";
    data = dict();
    data["Null"] = NULL_DATA;
    data["List"] = list();
    data["List"].append(NULL_DATA);
    data["Dict"] = dict();
    print data.json();


    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    print "\n";
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


    data = TRUE;
    print data.json();
    data = FALSE;
    print data.json();

    return 0;
}
