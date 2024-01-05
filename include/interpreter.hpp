#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "lox_function.hpp"
#include "lox.hpp"
#include "value.hpp"
#include "return_value.hpp"
#include "lox_callable.hpp"
#include "lox_class.hpp"
#include "clock_callable.hpp"
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
#include <list>
#include <unordered_map>

namespace Lox {

class Interpreter : public ExprVisitor<Value>, public StmtVisitor<void> {
public:
    Interpreter();
    virtual ~Interpreter() override = default;

    void interpret(std::unique_ptr<Expr>&);
    void interpret(std::vector<std::unique_ptr<Stmt>>&);
    void execute(std::unique_ptr<Stmt>&);
    void resolve(Expr*,int);
    void executeBlock(std::list<std::unique_ptr<Stmt>>&, std::shared_ptr<Environment>);

    // ExprVisitor<Value>
    virtual Value visitBinaryExpr(Binary*) override;
    virtual Value visitGroupingExpr(Grouping*) override;
    virtual Value visitUnaryExpr(Unary*) override;
    virtual Value visitLiteralExpr(Literal*) override;
    virtual Value visitVariableExpr(Variable*) override;
    virtual Value visitAssignExpr(Assign*) override;
    virtual Value visitLogicalExpr(Logical*) override;
    virtual Value visitCallExpr(Call*) override;
    virtual Value visitGetExpr(Get*) override;
    virtual Value visitSetExpr(Set*) override;
    virtual Value visitThisExpr(This*) override;

    // StmtVisitor<void>
    virtual void visitExpressionStmt(Expression*) override;
    virtual void visitClassStmt(Class*) override;
    virtual void visitFunctionStmt(Function*) override;
    virtual void visitReturnStmt(Return*) override;
    virtual void visitVarStmt(Var*) override;
    virtual void visitPrintStmt(Print*) override;
    virtual void visitBlockStmt(Block*) override;
    virtual void visitIfStmt(If*) override;
    virtual void visitWhileStmt(While*) override;

    std::shared_ptr<Environment> globals;


private:
    std::shared_ptr<Environment>& _environment;
    std::unordered_map<Expr*, int> _locals;

    Value evaluate(Expr*);
    Value evaluate(std::unique_ptr<Expr>&);
    Value lookUpVariable(const Token&, Expr*);
    bool isTruthy(const Value&);
    std::string stringify(const Value&);
    
    void checkNumberOperand(const Token&, const Value&);
    void checkNumberOperands(const Token&, const Value&, const Value&);
    void checkAdditionOperation(const Token&, const Value&, const Value&);


};

} // Lox namespace

#endif