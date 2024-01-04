#include "../include/lox_instance.hpp"

namespace Lox {

LoxInstance::LoxInstance(LoxClass* klass) : klass{klass}
{}

Value& LoxInstance::get(const Token& name) {
    if (fields.find(name.lexeme) != fields.end()) {
        return fields.at(name.lexeme);
    }

    throw RuntimeError{name, "Undefined property '" + name.lexeme + "'."};
}

void LoxInstance::set(const Token& name, Value& value) {
    fields[name.lexeme] = value;
}

std::string LoxInstance::LoxInstance::name() {
    return this->klass->name;
}

} // Lox namespace