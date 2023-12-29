#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "lox.hpp"
#include "value.hpp"
#include "token_type.hpp"
#include "expr.hpp"
#include "stmt.hpp"
#include "errors.hpp"
#include "environment.hpp"

#include <memory>
#include <variant>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

namespace Lox {

class Interpreter : public ExprVisitor<Value>, public StmtVisitor<void> {
public:
    Interpreter();
    virtual ~Interpreter() override = default;

    void interpret(std::unique_ptr<Expr>&);
    void interpret(std::vector<std::unique_ptr<Stmt>>&);

    // ExprVisitor<Value>
    virtual Value visitBinaryExpr(Binary*) override;
    virtual Value visitGroupingExpr(Grouping*) override;
    virtual Value visitUnaryExpr(Unary*) override;
    virtual Value visitLiteralExpr(Literal*) override;
    virtual Value visitVariableExpr(Variable*) override;
    virtual Value visitAssignExpr(Assign*) override;

    // StmtVisitor<void>
    virtual void visitExpressionStmt(Expression*) override;
    virtual void visitVarStmt(Var*) override;
    virtual void visitPrintStmt(Print*) override;

private:
    std::unique_ptr<Environment> _environment;

    Value evaluate(Expr*);
    Value evaluate(std::unique_ptr<Expr>&);
    void execute(std::unique_ptr<Stmt>&);
    bool isTruthy(const Value&);
    std::string stringify(const Value&);
    
    void checkNumberOperand(const Token&, const Value&);
    void checkNumberOperands(const Token&, const Value&, const Value&);
    void checkAdditionOperation(const Token&, const Value&, const Value&);


};

} // Lox namespace

#endif