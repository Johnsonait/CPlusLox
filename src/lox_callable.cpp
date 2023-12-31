#include "../include/lox_callable.hpp"

namespace Lox {

Value LoxCallable::operator()(Interpreter* interpreter, std::list<Value>& args) {
    return this->call(interpreter,args);
}

Value LoxCallable::call(Interpreter* interpreter, std::list<Value>& args) {
    
}

}