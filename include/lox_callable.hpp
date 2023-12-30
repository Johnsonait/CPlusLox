#ifndef LOX_CALLABLE_HPP
#define LOX_CALLABLE_HPP

#include "value.hpp"

#include <list>
#include <functional>

namespace Lox {

class Interpreter;

class LoxCallable{
public:
    LoxCallable() = default;
    ~LoxCallable() = default;

    Value operator()(Interpreter*, std::list<Value>&);

private:
    Value call(Interpreter*, std::list<Value>&);

    std::function<Value(Interpreter*, std::list<Value>&)>  func;
};

}

#endif