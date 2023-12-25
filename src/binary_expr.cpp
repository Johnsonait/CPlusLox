#include "../include/binary_expr.hpp"

namespace Lox {

Binary::Binary(
        std::unique_ptr<Expr> l, 
        Token op,
        std::unique_ptr<Expr> r
) : left{std::move(l)}, op{op}, right{std::move(r)}
{}

} // Lox namespace