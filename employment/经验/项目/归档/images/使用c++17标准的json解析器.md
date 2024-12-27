### 使用c++17标准的json解析器

> [!NOTE]
>
> 该文记录个人学习此项目的过程。
>
> 学习来源为：[三个比较小的C++项目与简历内容 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/612921777)



> 项目名称：使用C++17标准的json解析器
> 使用**variant** 管理json数据类型 ，string_view进行代理模式明确只读语义，**optional**处理非侵入式异常处理机制。
> 通过**递归下降**对json字符串进行parser，解析后支持动态修改，并重新输出json格式，接口设计便捷，使用方便。
>
> 提供了一个从JSON字符串到C++对象的解析器，以及从C++对象生成JSON字符串的生成器。通过`Node`对象，可以方便地操作JSON数据，例如访问、修改、添加、删除等。



**variant：**C++17的类模板，存储多个类型中的一种。管理json数据类型。[union](https://zhuanlan.zhihu.com/p/595288133)

**string_view：**C++17引入的轻量级对象，表示字符串的不可变视图，无需持有实际的字符串数据，不会带来额外的拷贝开销。在代理模式时，便于处理或者解析JSON字符串时不必要拷贝。

**optional：** C++17引入的模板类，表示一个值可能存在或不存在。是异常处理的一种替代方案，尤其在函数无法返回有效结果时。[optional上](https://zhuanlan.zhihu.com/p/64985296)



```c++
// JsonParse.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include<variant> // 单个变量中存储多种不同类型的数据
#include<vector> 
#include<map>
#include<optional>
#include<string>
#include<fstream>
#include <sstream>

// json解析器和生成器的命名空间
namespace json {

    // 基本数据类型的定义
    struct Node;
    using Null = std::monostate;
    using Bool = bool;
    using Int = int64_t;
    using Float = double;
    using String = std::string;
    using Array = std::vector<Node>;
    using Object = std::map<std::string, Node>;
    using Value = std::variant<Null, Bool, Int, Float, String, Array, Object>;
    
    // Node是对value的进一步封装，包含一些操作JSON节点的方法。
    struct Node {
        Value value;
        Node(Value _value) : value(_value) {}
        Node() : value(Null{}) {}

        // 重载[]运算符，用于访问Node对象中的object元素
        // get_if是c++17引入的一种安全访问variant内部值的方法。
        auto& operator[](const std::string& key) {
            if (auto object = std::get_if<Object>(&value)) {
                return (*object)[key];
            }
            throw std::runtime_error("not an object");
        }

        // 访问Array的元素
        auto operator[](size_t index) {
            if (auto array = std::get_if<Array>(&value)) {
                return array->at(index);
            }
            throw std::runtime_error("not an array");
        }

        // 添加数据元素
        void push(const Node& rhs) {
            if (auto array = std::get_if<Array>(&value)) {
                array->push_back(rhs);
            }
        }
    };

    // 解析JSON字符串
    struct JsonParser {
        // json字符串视图
        std::string_view json_str;
        size_t pos = 0;

        // 跳过空白字符
        void parse_whitespace() {
            while (pos < json_str.size() && std::isspace(json_str[pos])) {
                ++pos;
            }
        }

        // 解析null值
        auto parse_null() -> std::optional<Value> {
            if (json_str.substr(pos, 4) == "null") {
                pos += 4;
                return Null{};
            }
            return{};
        }

        // 解析true值
        auto parse_true() -> std::optional<Value> {
            if (json_str.substr(pos, 4) == "true") {
                pos += 4;
                return true;
            }
            return {};
        }

        // 解析false值
        auto parse_false() -> std::optional<Value> {
            if (json_str.substr(pos, 5) == "false") {
                pos += 5;
                return false;
            }
            return {};
        }

        // 返回类型后置
        // 解析数字
        auto parse_number() -> std::optional<Value> {
            // size_t是C++中用于表示对象大小或者数组索引的一种无符号整数类型
            size_t endpos = pos;
            // e和.是用来支持科学计数法和小数的
            while (endpos < json_str.size() && (
                std::isdigit(json_str[endpos]) ||
                json_str[endpos] == 'e' ||
                json_str[endpos] == '.')) {
                endpos++;
            }

            std::string number = std::string{ json_str.substr(pos, endpos - pos) };
            pos = endpos;

            // 看是否存在小数
            static auto is_Float = [](std::string& number) {
                return number.find('.') != number.npos ||
                    number.find('e') != number.npos;
            };

            // 字符串转换为数字
            if (is_Float(number)) {
                try {
                    Float ret = std::stod(number);
                    return ret;
                }
                catch (...) {
                    return {};
                }
            }
            else {
                try {
                    Int ret = std::stoi(number);
                    return ret;
                }
                catch (...) {
                    return {};
                }
            }
        }

        // 解析字符串
        auto parse_string() -> std::optional<Value> {
            pos++;// "
            
            size_t endpos = pos;
            while (pos < json_str.size() && json_str[endpos] != '"') {
                endpos++;
            }
            std::string str = std::string{ json_str.substr(pos, endpos - pos) };
            
            pos = endpos + 1;// "
            return str;
        }

        // 解析数组
        auto parse_array() -> std::optional<Value> {
            pos++;// [
            Array arr;
            while (pos < json_str.size() && json_str[pos] != ']') {
                auto value = parse_value();
                arr.push_back(value.value());

                parse_whitespace();
                if (pos < json_str.size() && json_str[pos] == ',') {
                    pos++;// ,
                }
                parse_whitespace();

            }
            pos++;// ]
            return arr;
        }

        // 解析object，也就是字典
        auto parse_object() -> std::optional<Value> {
            pos++;// {
            Object obj;

            while (pos < json_str.size() && json_str[pos] != '}') {
                auto key = parse_value();
                parse_whitespace();
                /*检查key.value()中是否保存了一个string，JSON中的object的键必须是字符串类型*/
                if (!std::holds_alternative<String>(key.value())) {
                    return {};
                }

                if (pos < json_str.size() && json_str[pos] == ':') {
                    pos++;// ,
                }
                parse_whitespace();

                auto val = parse_value();
                obj[std::get<String>(key.value())] = val.value();

                parse_whitespace();
                if (pos < json_str.size() && json_str[pos] == ',') {
                    pos++;// ,
                }
                parse_whitespace();
            }
            pos++;// }
            return obj;
        }

        // 解析值
        auto parse_value() -> std::optional<Value> {
            parse_whitespace();

            if (pos >= json_str.size()) {
                // 处理错误，可能抛出异常或返回一个错误状态
                throw std::out_of_range("JSON string position out of range");
            }

            switch (json_str[pos]) {
            case 'n':
                return parse_null();
            case 't':
                return parse_true();
            case 'f':
                return parse_false();
            case '"':
                return parse_string();
            case '[':
                return parse_array();
            case '{':
                return parse_object();
            default:
                return parse_number();
            }
        }

        // 解析，将上面内容串起来
        auto parse() -> std::optional<Node> {
            parse_whitespace();
            auto value = parse_value();
            if (!value) {
                return {};
            }
            return Node{ *value };
        }
    };

    /*JSON解析器的便捷入口*/
    auto parser(std::string_view json_str) -> std::optional<Node> {
        JsonParser p{ json_str };
        return p.parse();
    }

    // 将Node转换为JSON格式
    class JsonGenerator {
    public:
        static auto generate(const Node& node) -> std::string {
            /*visit
            * C++17引入的函数模板，用于访问std::variant对象中的值
            */
            return std::visit(
                [](auto&& arg) -> std::string {
                    /*将arg类型简化为基础类型*/
                    using T = std::decay_t<decltype(arg)>;
            /*编译器条件判断，根据T的类型选择操作*/
            if constexpr (std::is_same_v<T, Null>) {
                return "null";
            }
            else if constexpr (std::is_same_v<T, Bool>) {
                return arg ? "true" : "false";
            }
            else if constexpr (std::is_same_v<T, Int>) {
                return std::to_string(arg);
            }
            else if constexpr (std::is_same_v<T, Float>) {
                return std::to_string(arg);
            }
            else if constexpr (std::is_same_v<T, String>) {
                return generate_string(arg);
            }
            else if constexpr (std::is_same_v<T, Array>) {
                return generate_array(arg);
            }
            else if constexpr (std::is_same_v<T, Object>) {
                return generate_object(arg);
            }
                },
                node.value);
        }

        // Node字符串转换为JSON类型
        static auto generate_string(const String& str) -> std::string {
            std::string json_str = "\"";
            json_str += str;
            json_str += '"';
            return json_str;
        }

        //Node数组转换为JSON类型
        static auto generate_array(const Array& array) -> std::string {
            std::string json_str = "[";
            for (const auto& node : array) {
                json_str += generate(node);
                json_str += ',';
            }
            //去掉多余的逗号
            if (!array.empty()) json_str.pop_back();
            json_str += ']';
            return json_str;
        }

        // 
        static auto generate_object(const Object& object) -> std::string {
            std::string json_str = "{";
            for (const auto& [key, node] : object) {
                json_str += generate_string(key);
                json_str += ':';
                json_str += generate(node);
                json_str += ',';
            }
            if (!object.empty()) json_str.pop_back();
            json_str += '}';
            return json_str;
        }
    };

    /*生成一个简单的接口*/
    inline auto generate(const Node& node) -> std::string { 
        return JsonGenerator::generate(node); 
    }

    /*运算符重载，将Node对象转换为JSON字符串，输出到指定的输出流中*/
    auto  operator << (std::ostream& out, const Node& t) ->std::ostream& {
        out << JsonGenerator::generate(t);
        return out;
    }

}
using namespace json;

int main() {
    std::ifstream fin("json.txt");
    std::stringstream ss; ss << fin.rdbuf();
    std::string s{ ss.str() };
    auto x = parser(s).value();
    std::cout << x << "\n";
    x["configurations"].push({ true });
    x["configurations"].push({ Null {} });
    x["version"] = { 114514LL };
    std::cout << x << "\n";
}
```

