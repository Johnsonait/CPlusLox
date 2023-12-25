#include "../include/unary_expr.hpp"

namespace Lox {

Unary::Unary(
    Token op,
    std::unique_ptr<Expr> r
) : op{op}, right{std::move(r)}
{}

}