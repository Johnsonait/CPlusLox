#include "../include/lox_class.hpp"

namespace Lox {

LoxClass::LoxClass(const std::string& name, std::unordered_map<std::string, std::shared_ptr<LoxFunction>>& methods) 
: name{name}, methods{std::move(methods)}
{}

std::shared_ptr<LoxFunction> LoxClass::findMethod(const std::string& name) {
    if (methods.find(name) != methods.end()) {
        return methods.at(name);
    }

    return nullptr;
}

int LoxClass::arity() {
    auto initializer = findMethod("init");
    if (initializer == nullptr) return 0;
    return initializer->arity();
}

Value LoxClass::call(Interpreter* interpreter, std::list<Value>& args) {
    auto instance = std::make_shared<LoxInstance>(this);
    auto initializer = findMethod("init");
    if (initializer != nullptr) {
        initializer->bind(instance.get())->call(interpreter,args);
    }
    return Value{instance};
}

} // Lox namespace