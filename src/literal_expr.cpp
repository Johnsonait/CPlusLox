#include "../include/literal_expr.hpp"

namespace Lox {

Literal::Literal(const double& v) {
    value = v;
}

Literal::Literal(const std::string& v) {
    value = v;
}

Literal::Literal(std::monostate v) {
    value = v;
}

Literal::Literal(
    std::variant<double,std::string,std::monostate> val
) : value{val}
{}

}