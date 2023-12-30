#ifndef RUNTIME_ERROR_HPP
#define RUNTIME_ERROR_HPP

#include "error.hpp"
#include "token.hpp"

#include <string>

namespace Lox {

class RuntimeError : public Error {
public:

    RuntimeError(const Token& op, const std::string& what) : Error(what), op{op}
    {}
    virtual ~RuntimeError() override = default;

    Token op;
};

} // Lox namespace

#endif