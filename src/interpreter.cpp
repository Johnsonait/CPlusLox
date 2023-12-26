#include "../include/interpreter.hpp"

namespace Lox {

Value visitBinaryExpr(Binary* b) {
    return Value{std::monostate{}};
}

Value visitGroupingExpr(Grouping* g) {
    return Value{std::monostate{}};
}

Value visitUnaryExpr(Unary* u) {
    return Value{std::monostate{}};
}

Value visitLiteralExpr(Literal* l) {
    return Value{std::monostate{}};
}

} // Lox namespace