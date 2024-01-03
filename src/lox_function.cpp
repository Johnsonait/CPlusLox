#include "../include/lox_function.hpp"

namespace Lox {

LoxFunction::LoxFunction(Function* declaration, std::shared_ptr<Environment>& closure) 
: declaration{declaration}, closure{closure}
{}

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
        return return_value.value;
    }
    
    return Value{std::monostate{}};
}


} // Lox namespace