#ifndef LOX_FUNCTION_HPP
#define LOX_FUNCTION_HPP

#include "interpreter.hpp"
#include "stmt.hpp"
#include "lox_callable.hpp"
#include "environment.hpp"
#include "return_value.hpp"

#include <memory>
#include <list>
#include <algorithm>

namespace Lox {

class LoxFunction : public LoxCallable {
public:
    explicit LoxFunction(Function*, std::shared_ptr<Environment>&, bool);
    virtual ~LoxFunction() override = default;

    std::shared_ptr<LoxFunction> bind(LoxInstance*);

    virtual int arity() override;
    virtual Value call(Interpreter*, std::list<Value>&) override;

private:
    Function* declaration;
    std::shared_ptr<Environment> closure;

    bool isInitializer;

};

} // Lox namespace

#endif