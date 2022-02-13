#include "../include/rapidjson/prettywriter.h"
#include <typeinfo>

#include <iostream>
using namespace rapidjson;
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

    std::cout << typeid("hello").name() << '\n';

    return 0;
}
