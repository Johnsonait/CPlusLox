#include "../include/lox_class.hpp"

namespace Lox {

LoxClass::LoxClass(const std::string& name) : name{name}
{}

int LoxClass::arity() {
    return 0;
}

Value LoxClass::call(Interpreter* interpreter, std::list<Value>& args) {
    auto instance = std::make_shared<LoxInstance>(this);
    return Value{instance};
}

} // Lox namespace