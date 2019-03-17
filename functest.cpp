
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
    data["M (null)"]        = NULL_DATA;
    data["N (bool)"]        = BOOL_FALSE;
    data["O (bool)"]        = BOOL_TRUE;
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


    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    cout << endl << endl;
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


    data = NULL_DATA;
    cout << "Empty data object:\n";
    cout << data.json();
    cout << "End of JSON output\n\n";

    data = dict();
    cout << "Empty root dict:\n";
    cout << data.json();
    cout << "End of JSON output\n\n";

    data = list();
    cout << "Empty root list:\n";
    cout << data.json();
    cout << "End of JSON output\n\n";

    cout << "Null value / null element / empty dict:\n";
    data = dict();
    data["Null"] = NULL_DATA;
    data["List"] = list();
    data["List"].append(NULL_DATA);
    data["Dict"] = dict();
    cout << data.json();


    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    cout << endl << endl;
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


    data = BOOL_TRUE;
    cout << data.json();
    data = BOOL_FALSE;
    cout << data.json();

    return 0;
}
