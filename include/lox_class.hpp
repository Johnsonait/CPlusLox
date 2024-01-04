#ifndef LOX_CLASS_HPP
#define LOX_CLASS_HPP

#include "lox_callable.hpp"
#include "lox_instance.hpp"

#include <string>
#include <list>
#include <vector>
#include <map>
#include <unordered_map>

namespace Lox {

class LoxClass : public LoxCallable {
public:
    explicit LoxClass(const std::string&);
    virtual ~LoxClass() = default;


    virtual int arity() override;
    virtual Value call(Interpreter*, std::list<Value>&) override;

    std::string name;
private:
    
};

} // Lox namespace

#endif