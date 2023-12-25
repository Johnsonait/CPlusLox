#include "../include/grouping_expr.hpp"

namespace Lox {
    
Grouping::Grouping(
    std::unique_ptr<Expr> expr
) : expression{std::move(expr)}
{}

}