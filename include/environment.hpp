#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include "value.hpp"
#include "errors.hpp"

#include <map>
#include <unordered_map>
#include <string>

namespace Lox {

class Environment {
public:
    Environment() = default;
    ~Environment() = default;

    void define(std::string, Value);

    Value& get(const Token&);


private:
    std::map<std::string,Value> values;
};

} // Lox namespace

#endif