#include "../include/rapidjson/prettywriter.h"
#include "../include/rapidjson/reader.h"
#include <sstream>
#include <typeinfo>
#include "PackageParser.h"
#include <vector>
#include <iostream>
#include <any>
using namespace rapidjson;

double add(double lhs, double rhs)
{
    return lhs + rhs;
}

double multiple(double lhs, double rhs)
{
    return lhs * rhs;
}
using namespace std;

struct MyHandler {
    bool Null() { cout << "Null()" << endl; return true; }
    bool Bool(bool b) { cout << "Bool(" << boolalpha << b << ")" << endl; return true; }
    bool Int(int i) { cout << "Int(" << i << ")" << endl; return true; }
    bool Uint(unsigned u) { cout << "Uint(" << u << ")" << endl; return true; }
    bool Int64(int64_t i) { cout << "Int64(" << i << ")" << endl; return true; }
    bool Uint64(uint64_t u) { cout << "Uint64(" << u << ")" << endl; return true; }
    bool Double(double d) { cout << "Double(" << d << ")" << endl; return true; }
    bool RawNumber(const char* str, SizeType length, bool copy) { 
        cout << "Number(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
        return true;
    }
    bool String(const char* str, SizeType length, bool copy) { 
        cout << "String(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
        return true;
    }
    bool StartObject() { cout << "StartObject()" << endl; return true; }
    bool Key(const char* str, SizeType length, bool copy) {
        cout << "Key(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
        return true;
    }
    bool EndObject(SizeType memberCount) { cout << "EndObject(" << memberCount << ")" << endl; return true; }
    bool StartArray() { cout << "StartArray()" << endl; return true; }
    bool EndArray(SizeType elementCount) { cout << "EndArray(" << elementCount << ")" << endl; return true; }
};

int main(int argc, char const *argv[])
{
    // StringBuffer sb;
    // PrettyWriter<StringBuffer> writer(sb);
    // writer.StartObject();
    // 现在有点尬住，因为好像要先知道数据的type才能进行数据的存储
    // 欧克，现在知道那个func_str i(ii)是什么意思了，就是返回值是int 有两个参数，类型是int and int

    // writer.EndObject();
    // function name

    // function params

    // double d
    // int    i
    
    // const char* PKc
    // char        c

    // bool        b

    yi::PackageParser parser;
    parser.ParseParams("add", -100, 200, 400.5);
    // std::cout << parser.Serialize() << '\n';;
    auto str = parser.Serialize();
    std::cout << str << '\n';

    Document d;
    d.Parse(str);
    std::string param_header("param");
    // auto name = d["function_name"].GetString();
    // std::cout << name;
    // size_t size = 0;
    // auto temp = (param_header+std::to_string(size+1));
    // std::cout << temp.c_str() << '\n';
    // std::cout << d[temp.c_str()].GetDouble() << '\n';
    MyHandler handler;
    Reader reader;
    StringStream ss(str);
    reader.Parse(ss, handler);

    auto ite = d.FindMember("param3");
    std::cout << ite->value.GetType() << '\n';
    std::cout << ite->value.GetInt() << '\n';

    // auto [function_name, param_list] = parser.DeSerialize(str);

    // std::cout << std::any_cast<const char*>(function_name) << '\n';
    // std::cout << std::any_cast<int>(param_list[0]) << '\n';
    // std::cout << std::any_cast<uint>(param_list[1]) << '\n';
    // std::cout << std::any_cast<double>(param_list[2]) << '\n';

    // const char json[] = " { \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, 3, 4] } ";
        // std::string name1("123");
    // std::string name2("123");
    // std::string name3("abc");
    // bool a  = true;
    // std::cout << typeid(name1).name() << '\n'; 
    // std::cout << typeid(name2).name() << '\n'; 
    // std::cout << typeid(name3.c_str()).name() << '\n'; 
    // std::cout << typeid(a).name() << '\n'; 

    return 0;
}
