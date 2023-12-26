#include "../include/value.hpp"

namespace Lox {

Value::Value(const double& v) {
    this->item = v;
}

Value::Value(const bool& v) {
    this->item = v;
}

Value::Value(const std::string& v) {
    this->item = v;
}

Value::Value(const std::monostate& v) {
    this->item = v;
}
Value::Value(const std::variant<double,bool,std::string,std::monostate>& v) {
    this->item = v;
}

} // Lox namespace