#ifndef LOX_INSTANCE_HPP
#define LOX_INSTANCE_HPP

#include "lox_class.hpp"
#include "runtime_error.hpp"

#include <string>
#include <unordered_map>

namespace Lox {

class LoxInstance {
public:
    explicit LoxInstance(LoxClass*);

    std::string name();
    Value& get(const Token&);
    void set(const Token&, Value&);

private:
    LoxClass* klass;
    std::unordered_map<std::string, Value> fields;
};

} // Lox namespace 

#endif