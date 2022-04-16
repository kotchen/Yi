#include "../include/rapidjson/prettywriter.h"
#include "../include/rapidjson/document.h"
#include <typeinfo>
#include <string>
#include <tuple>
#include <vector>
#include <any>
using namespace rapidjson;
namespace yi
{
    class PackageParser
    {
    private:
        /* data */
        StringBuffer _sb;
        PrettyWriter<StringBuffer> _writer;
        Document _document;
        std::string _params_header;
        void _parseParams()
        {
            _writer.EndObject();
        }
        template <typename First, typename... Args>
        void _parseParams(First first, Args... args)
        {
            constexpr int params_size = sizeof...(args) + 1;
            char type;
            memcpy(&type, typeid(first).name(), 1);
            switch (type)
            {
            // 数值 double  int
            case 'd':
                _writer.String((_params_header + std::to_string(params_size)).c_str());
                _writer.Double(first);
                break;
            case 'i':
                _writer.String((_params_header + std::to_string(params_size)).c_str());
                _writer.Int(first);
                break;
            // 字符串 std::string
            default:
                break;
            }
            _parseParams(std::forward<Args>(args)...);
        }

    public:
        PackageParser(/* args */) : _writer(_sb), _params_header("param")
        {
            _writer.StartObject();
        }
        ~PackageParser()
        {
        }
        template <typename... Args>
        void ParseParams(const std::string &function_name, Args &&...params)
        {
            _writer.String("function_name");
            _writer.String(function_name.c_str(), static_cast<SizeType>(function_name.length()));
            _writer.String("params_size");
            _writer.Int(sizeof...(params));
            _parseParams(std::forward<Args>(params)...);
        }

        const char *Serialize()
        {
            return _sb.GetString();
        }

        std::tuple<std::string, std::vector<std::any>>
        DeSerialize(const char *str)
        {
            _document.Parse(str);
            auto function_name = _document["function_name"].GetString();
            auto params_size = _document["params_size"].GetInt();
            std::string params_header("param");
            std::vector<std::any> params_list;

            for (int i = params_size; i >= 1; i++)
            {
                // params_list.emplace_back(param);
            }
            return {function_name, params_list};
        }
    };

} // namespace yi
