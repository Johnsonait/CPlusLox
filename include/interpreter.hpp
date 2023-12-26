#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "value.hpp"
#include "expr.hpp"

namespace Lox {

class Interpreter : public ExprVisitor<Value> {
public:
    virtual ~Interpreter() override = default;

    virtual Value visitBinaryExpr(Binary*) override;
    virtual Value visitGroupingExpr(Grouping*) override;
    virtual Value visitUnaryExpr(Unary*) override;
    virtual Value visitLiteralExpr(Literal*) override;


private:


};

} // Lox namespace

#endif