#include "../include/lox_callable.hpp"

namespace Lox {

Value LoxCallable::operator()(Interpreter* callable, std::list<Value>& args) {
    return this->call(callable,args);
}

Value LoxCallable::call(Interpreter* callable, std::list<Value>& args) {
    
}

}