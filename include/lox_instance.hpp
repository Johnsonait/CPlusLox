#ifndef LOX_INSTANCE_HPP
#define LOX_INSTANCE_HPP

#include "lox_class.hpp"

#include <string>
#include <unordered_map>

namespace Lox {

class LoxInstance {
public:
    explicit LoxInstance(LoxClass*);

    std::string name();

private:
    LoxClass* klass;

};

} // Lox namespace 

#endif