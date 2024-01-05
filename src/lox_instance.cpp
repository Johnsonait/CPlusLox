#include "../include/lox_instance.hpp"

namespace Lox {

LoxInstance::LoxInstance(LoxClass* klass) : klass{klass}
{}

Value LoxInstance::get(const Token& name) {
    if (fields.find(name.lexeme) != fields.end()) {
        return fields.at(name.lexeme);
    }
    
    auto method = klass->findMethod(name.lexeme);
    if (method != nullptr) return Value{method->bind(this)};

    throw RuntimeError{name, "Undefined property '" + name.lexeme + "'."};
}

void LoxInstance::set(const Token& name, Value value) {
    fields.emplace(name.lexeme,value);
}

std::string LoxInstance::name() {
    return this->klass->name;
}

} // Lox namespace