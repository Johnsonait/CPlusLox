#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include "value.hpp"
#include "errors.hpp"

#include <memory>
#include <map>
#include <unordered_map>
#include <string>

namespace Lox {

class Environment {
public:
    Environment() = default;
    explicit Environment(std::shared_ptr<Environment>);
    ~Environment() = default;

    void define(std::string, Value);
    void assign(const Token&, const Value&);

    Value& get(const Token&);
    Value& getAt(int, const std::string&);
    void assignAt(int, const Token&, Value);
    Environment* ancestor(int);

    std::shared_ptr<Environment> enclosing;


private:
    std::unordered_map<std::string,Value> values;
};

} // Lox namespace

#endif