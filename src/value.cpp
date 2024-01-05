#include "../include/value.hpp"

namespace Lox {

Value::Value() {
    this->item = std::monostate{};
}

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

Value::Value(std::shared_ptr<LoxCallable>& v) {
    this->item = v;
}

Value::Value(std::shared_ptr<LoxClass>& v) {
    this->item = v;
}

Value::Value(std::shared_ptr<LoxInstance>& v) {
    this->item = v;
}

Value::Value(const std::variant<
    double,
    bool,
    std::string,
    std::monostate, 
    std::shared_ptr<LoxCallable>,
    std::shared_ptr<LoxClass>,
    std::shared_ptr<LoxInstance>
    >& v) {
    this->item = v;
}


// Arithmetic operations
Value Value::operator+(const Value& rhs) const {
    if (std::holds_alternative<double>(this->item) && std::holds_alternative<double>(rhs.item)) {
        return Value{std::get<double>(this->item) + std::get<double>(rhs.item)};
    }
    if (std::holds_alternative<std::string>(this->item) && std::holds_alternative<std::string>(rhs.item)) {
        return Value{std::get<std::string>(this->item) + std::get<std::string>(rhs.item)};
    }
    return Value{std::monostate{}};
}

Value Value::operator-(const Value& rhs) const {
    if (std::holds_alternative<double>(this->item) && std::holds_alternative<double>(rhs.item)) {
        return Value{std::get<double>(this->item) - std::get<double>(rhs.item)};
    }
    return Value{std::monostate{}};
}

Value Value::operator*(const Value& rhs) const {
    if (std::holds_alternative<double>(this->item) && std::holds_alternative<double>(rhs.item)) {
        return Value{std::get<double>(this->item) * std::get<double>(rhs.item)};
    }
    return Value{std::monostate{}};
}

Value Value::operator/(const Value& rhs) const {
    if (std::holds_alternative<double>(this->item) && std::holds_alternative<double>(rhs.item)) {
        return Value{std::get<double>(this->item) / std::get<double>(rhs.item)};
    }
    return Value{std::monostate{}};
}

// Relational operators
bool Value::operator>(const Value& rhs) const {
    if (std::holds_alternative<double>(this->item) && std::holds_alternative<double>(rhs.item)) {
        return std::get<double>(this->item) > std::get<double>(rhs.item);
    }
    return false;
}

bool Value::operator>=(const Value& rhs) const {
    if (std::holds_alternative<double>(this->item) && std::holds_alternative<double>(rhs.item)) {
        return std::get<double>(this->item) >= std::get<double>(rhs.item);
    }
    return false;
}

bool Value::operator<(const Value& rhs) const {
    return rhs > (*this);
}

bool Value::operator<=(const Value& rhs) const {
    return rhs >= (*this);
}

bool Value::operator==(const Value& rhs) const {
    if (std::holds_alternative<bool>(this->item) && std::holds_alternative<bool>(rhs.item)) {
        return std::get<bool>(this->item) == std::get<bool>(rhs.item);
    }
    if (std::holds_alternative<double>(this->item) && std::holds_alternative<double>(rhs.item)) {
        return std::get<double>(this->item) == std::get<double>(rhs.item);
    }
    if (std::holds_alternative<std::string>(this->item) && std::holds_alternative<std::string>(rhs.item)) {
        return std::get<std::string>(this->item) == std::get<std::string>(rhs.item);
    }
    if (std::holds_alternative<std::monostate>(this->item) && std::holds_alternative<std::monostate>(rhs.item)) {
        return true;
    }
    
    return false;
}

bool Value::operator!=(const Value& rhs) const {
    return !((*this) == rhs);
}


} // Lox namespace