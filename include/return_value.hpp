#ifndef RETURN_VALUE_HPP
#define RETURN_VALUE_HPP

#include "value.hpp"
#include "token.hpp"
#include "runtime_error.hpp"

namespace Lox {

class ReturnValue : public RuntimeError{
public:
    explicit ReturnValue(const Value& value) : RuntimeError{Token{},""}, value{value}
    {}

    Value value;
};

} // Lox namespace

#endif