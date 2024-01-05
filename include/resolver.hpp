#ifndef RESOLVER_HPP
#define RESOLVER_HPP

#include "expr.hpp"
#include "stmt.hpp"
#include "interpreter.hpp"
#include "lox.hpp"

#include <algorithm>
#include <memory>
#include <unordered_map>
#include <map>
#include <vector>
#include <list>
#include <stack>
#include <string>

namespace Lox {

enum class FunctionType {
    NONE,
    FUNCTION,
    INITIALIZER,
    METHOD
};

enum class ClassType {
    NONE,
    CLASS
};

class Resolver : public ExprVisitor<void>, public StmtVisitor<void>{
public:
    explicit Resolver(Interpreter*);

    void resolve(std::vector<std::unique_ptr<Stmt>>&);
    void resolve(std::list<std::unique_ptr<Stmt>>&);

    // ExprVisitor<void>
    virtual void visitBinaryExpr(Binary*) override;
    virtual void visitGroupingExpr(Grouping*) override;
    virtual void visitUnaryExpr(Unary*) override;
    virtual void visitLiteralExpr(Literal*) override;
    virtual void visitVariableExpr(Variable*) override;
    virtual void visitAssignExpr(Assign*) override;
    virtual void visitLogicalExpr(Logical*) override;
    virtual void visitCallExpr(Call*) override;
    virtual void visitGetExpr(Get*) override;
    virtual void visitSetExpr(Set*) override;
    virtual void visitThisExpr(This*) override;

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

private:
    Interpreter* interpreter;
    std::vector<std::unordered_map<std::string,bool>> scopes;

    FunctionType currentFunction;
    ClassType currentClass;

    void resolve(std::unique_ptr<Stmt>&);
    void resolve(std::unique_ptr<Expr>&);
    void resolveLocal(Expr*, const Token&);
    void resolveFunction(Function* function,const FunctionType&);
    void declare(const Token&);
    void define(const Token&);

    void beginScope();
    void endScope();

};

} // Lox namespace

#endif