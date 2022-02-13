#include <functional>
#include <vector>
#include <any>
// that function we write to call, such as add
using Function = std::function<std::vector<std::any>(std::vector<std::any>)>;

using FunctionRet = std::vector<std::any>;
using FunctionParams = std::vector<std::any>;