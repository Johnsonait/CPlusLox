#ifndef CLOCK_CALLABLE_HPP
#define CLOCK_CALLABLE_HPP

#include "lox_callable.hpp"

#include <chrono>

namespace Lox {

class ClockCallable : public LoxCallable {
public:
    ClockCallable() = default;
    virtual ~ClockCallable() override = default;

    virtual int arity() override;
    virtual Value call(Interpreter*, std::list<Value>&) override;
private:
};


} // Lox namespace

#endif