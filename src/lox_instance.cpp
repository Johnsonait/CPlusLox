#include "../include/lox_instance.hpp"

namespace Lox {

LoxInstance::LoxInstance(LoxClass* klass) : klass{klass}
{}

std::string LoxInstance::LoxInstance::name() {
    return this->klass->name;
}

} // Lox namespace