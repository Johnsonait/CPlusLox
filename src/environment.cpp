#include "../include/environment.hpp"

namespace Lox {

void Environment::define(std::string name, Value value) {
    this->values.emplace(name,value);
}

Value& Environment::get(const Token& name) {
    if (values.find(name.lexeme) != values.end()) {
        return values.at(name.lexeme);
    }

    throw RuntimeError{name,"Undefined variable '" + name.lexeme + "'."};
}

} // Lox namespace