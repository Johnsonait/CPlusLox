#ifndef LOX_CLASS_HPP
#define LOX_CLASS_HPP

#include "lox_function.hpp"
#include "lox_callable.hpp"
#include "lox_instance.hpp"

#include <string>
#include <list>
#include <vector>
#include <map>
#include <unordered_map>

namespace Lox {

class LoxFunction;

class LoxClass : public LoxCallable {
public:
    explicit LoxClass(const std::string&, std::unordered_map<std::string, std::shared_ptr<LoxFunction>>&);

    virtual ~LoxClass() = default;

    std::shared_ptr<LoxFunction> findMethod(const std::string&);

    virtual int arity() override;
    virtual Value call(Interpreter*, std::list<Value>&) override;

    std::string name;
private:
    std::unordered_map<std::string, std::shared_ptr<LoxFunction>> methods;
    
};

} // Lox namespace

#endif