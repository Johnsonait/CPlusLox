#ifndef VALUE_HPP
#define VALUE_HPP

#include <string>
#include <variant>

namespace Lox {

class Value {
public:
    Value(const double& v);
    Value(const bool& v);
    Value(const std::string& v);
    Value(const std::monostate& v);
    Value(const Value&) = default;
    Value(const std::variant<double, bool,std::string,std::monostate>& v);

    std::variant<double,bool,std::string,std::monostate> item;
};

} // lox namespace

#endif