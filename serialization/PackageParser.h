#include "../include/rapidjson/prettywriter.h"
#include <typeinfo>
#include <string>
using namespace rapidjson;
namespace yi
{
    class PackageParser
    {
    private:
        /* data */
        StringBuffer _sb;
        PrettyWriter<StringBuffer> _writer;
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
                _writer.String((_params_header+std::to_string(params_size)).c_str());
                _writer.Double(first);
                break;
            case 'i':
                _writer.String((_params_header+std::to_string(params_size)).c_str());
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
            _parseParams(std::forward<Args>(params)...);
        }

        const char *Serialize()
        {
            return _sb.GetString();
        }
    };

} // namespace yi
