#include "../include/lox_function.hpp"

namespace Lox {

LoxFunction::LoxFunction(Function* declaration, std::shared_ptr<Environment>& closure, bool isInitializer) 
: declaration{declaration}, closure{closure}, isInitializer{isInitializer}
{}

std::shared_ptr<LoxFunction> LoxFunction::bind(LoxInstance* instance) {
    auto env = std::make_shared<Environment>(closure);
    env->define("this", Value{std::make_shared<LoxInstance>(*instance)});
    return std::make_shared<LoxFunction>(declaration,env,isInitializer);
}

int LoxFunction::arity() {
    return declaration->params.size();
}

Value LoxFunction::call(Interpreter* interpreter, std::list<Value>& arguments) {
    auto env = std::make_shared<Environment>(closure);

    // Messy-ish way to "zip" the list of parameters and args together so we can
    // easily operate on them together
    auto _null = std::list<bool>{};
    std::transform(
        declaration->params.begin(), declaration->params.end(),
        arguments.begin(),
        std::back_inserter(_null),
        [&](const auto& param, const auto& arg){
            env->define(param.lexeme,arg);
            return true;
        }
    );

    try {
        interpreter->executeBlock(declaration->body,env);
    } catch(const ReturnValue& return_value) {
        if (isInitializer) return closure->getAt(0,"this");
        return return_value.value;
    }

    if (isInitializer) return closure->getAt(0,"this");
    
    return Value{std::monostate{}};
}


} // Lox namespace