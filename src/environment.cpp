#include "../include/environment.hpp"

namespace Lox {

Environment::Environment(std::shared_ptr<Environment> env) : enclosing{env}, values{}
{}

void Environment::define(std::string name, Value value) {
    values[name] = value;
}

void Environment::assign(const Token& name, const Value& value) {
    if (values.find(name.lexeme) != values.end()) {
        values[name.lexeme] = value;
        return;
    }

    if (enclosing != nullptr) {
        enclosing->assign(name,value);
        return;
    }

    throw RuntimeError{name, "Undefined variable " + name.lexeme + "."};
}

Value& Environment::get(const Token& name) {
    if (values.find(name.lexeme) != values.end()) {
        return values.at(name.lexeme);
    }

    if (enclosing != nullptr) return enclosing->get(name); 

    throw RuntimeError{name,"Undefined variable '" + name.lexeme + "'."};
}

Value& Environment::getAt(int distance, const std::string& name) {
    auto* env = ancestor(distance);
    if (env->values.find(name) == env->values.end()) {
        throw Error{"Value not in scope."};
    } else {
        return env->values.at(name);
    }
}

void Environment::assignAt(int distance, const Token& name, Value value) {
    ancestor(distance)->values[name.lexeme] =  value;
}

Environment* Environment::ancestor(int distance) {
    Environment* env = this;
    for (int i = 0; i < distance; i++) {
        env = env->enclosing.get();
    }
    return env;
}



} // Lox namespace