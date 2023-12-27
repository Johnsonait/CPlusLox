#ifndef VALUE_HPP
#define VALUE_HPP

#include "errors.hpp"

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

    // Arithmetic
    Value operator+(const Value&) const;
    Value operator-(const Value&) const;
    Value operator*(const Value&) const;
    Value operator/(const Value&) const;

    // Relational
    bool operator>(const Value&) const;
    bool operator>=(const Value&) const;
    bool operator<(const Value&) const;
    bool operator<=(const Value&) const;
    bool operator==(const Value&) const;
    bool operator!=(const Value&) const;

    std::variant<double,bool,std::string,std::monostate> item;
    
};

} // lox namespace

#endif