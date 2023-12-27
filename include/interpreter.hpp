#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "lox.hpp"
#include "value.hpp"
#include "token_type.hpp"
#include "expr.hpp"
#include "errors.hpp"

#include <memory>
#include <variant>
#include <string>
#include <sstream>
#include <iostream>

namespace Lox {

class Interpreter : public ExprVisitor<Value> {
public:
    virtual ~Interpreter() override = default;

    void interpret(std::unique_ptr<Expr>&);

    virtual Value visitBinaryExpr(Binary*) override;
    virtual Value visitGroupingExpr(Grouping*) override;
    virtual Value visitUnaryExpr(Unary*) override;
    virtual Value visitLiteralExpr(Literal*) override;

private:
    Value evaluate(Expr*);
    Value evaluate(std::unique_ptr<Expr>&);
    bool isTruthy(const Value&);
    std::string stringify(const Value&);
    
    void checkNumberOperand(const Token&, const Value&);
    void checkNumberOperands(const Token&, const Value&, const Value&);
    void checkAdditionOperation(const Token&, const Value&, const Value&);


};

} // Lox namespace

#endif