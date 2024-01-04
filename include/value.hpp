#ifndef VALUE_HPP
#define VALUE_HPP

// #include "lox_callable.hpp"
// #include "lox_class.hpp"
// #include "lox_instance.hpp"

#include <list>
#include <string>
#include <sstream>
#include <variant>
#include <memory>

namespace Lox {

class LoxCallable;
class LoxClass;
class LoxInstance;

class Value {
public:
    Value();
    explicit Value(const double& v);
    explicit Value(const bool& v);
    explicit Value(const std::string& v);
    explicit Value(const std::monostate& v);
    explicit Value(std::shared_ptr<LoxCallable>& v);
    explicit Value(std::shared_ptr<LoxClass>& v);
    explicit Value(std::shared_ptr<LoxInstance>& v);
    Value(const Value&) = default;
    Value(
        const std::variant<
            double, 
            bool,
            std::string,
            std::monostate, 
            std::shared_ptr<LoxCallable>,
            std::shared_ptr<LoxClass>,
            std::shared_ptr<LoxInstance>
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
        std::shared_ptr<LoxCallable>,
        std::shared_ptr<LoxClass>,
        std::shared_ptr<LoxInstance>
        > item;
    
};

} // lox namespace

#endif