#ifndef LOX_CALLABLE_HPP
#define LOX_CALLABLE_HPP

#include "value.hpp"

#include <list>

namespace Lox {

class Interpreter;

class LoxCallable{
public:
    virtual ~LoxCallable(){}

    virtual int arity() = 0;
    virtual Value call(Interpreter*, std::list<Value>&) = 0;
};

}

#endif