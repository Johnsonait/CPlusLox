#ifndef VALUE_HPP
#define VALUE_HPP

#include <list>
#include <string>
#include <variant>
#include <memory>

namespace Lox {

class LoxCallable;

class Value {
public:
    Value();
    explicit Value(const double& v);
    explicit Value(const bool& v);
    explicit Value(const std::string& v);
    explicit Value(const std::monostate& v);
    explicit Value(std::shared_ptr<LoxCallable>& v);
    Value(const Value&) = default;
    Value(
        const std::variant<double, 
            bool,
            std::string,
            std::monostate, 
            std::shared_ptr<LoxCallable>
            >& v
        );


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

    std::variant<
        double,
        bool,
        std::string,
        std::monostate,
        std::shared_ptr<LoxCallable>
        > item;
    
};

} // lox namespace

#endif