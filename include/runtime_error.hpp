#ifndef RUNTIME_ERROR_HPP
#define RUNTIME_ERROR_HPP

#include "token.hpp"
#include "error.hpp"

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