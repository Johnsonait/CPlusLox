#ifndef STMT_HPP
#define STMT_HPP

#include "expr.hpp"

#include <list>

namespace Lox {

class Expression;
class Function;
class Return;
class Var;
class Print;
class Block;
class If;
class While;

//==============================================================================
// Statement visitor interface
//==============================================================================
template<typename T>
class StmtVisitor {
public:
    virtual ~StmtVisitor(){}

    virtual T visitExpressionStmt(Expression*) = 0;
    virtual T visitFunctionStmt(Function*) = 0;
    virtual T visitReturnStmt(Return*) = 0;
    virtual T visitVarStmt(Var*) = 0;
    virtual T visitPrintStmt(Print*) = 0;
    virtual T visitBlockStmt(Block*) = 0;
    virtual T visitIfStmt(If*) = 0;
    virtual T visitWhileStmt(While*) = 0;
};

//==============================================================================
// Base statement (interface)
//==============================================================================
class Stmt {
public:
    virtual ~Stmt(){};

    virtual void accept(StmtVisitor<void>*) = 0;

};

//==============================================================================
// Concrete statement(s)
//==============================================================================
class Expression : public Stmt {
public:
    explicit Expression(std::unique_ptr<Expr>& expr) : expr{std::move(expr)}
    {}
    virtual ~Expression() override = default;

    virtual void accept(StmtVisitor<void>* visitor) override {
        visitor->visitExpressionStmt(this);
    }

    std::unique_ptr<Expr> expr;
};

class Var : public Stmt {
public: 
    explicit Var(const Token& name, std::unique_ptr<Expr>& init) : name{name} ,initializer{std::move(init)}
    {}
    virtual ~Var() override = default;

    virtual void accept(StmtVisitor<void>* visitor) override {
        visitor->visitVarStmt(this);
    }

    Token name;
    std::unique_ptr<Expr> initializer;
};

class Print : public Stmt {
public:
    explicit Print(std::unique_ptr<Expr>& value) : value{std::move(value)}
    {}
    virtual ~Print() override = default;

    virtual void accept(StmtVisitor<void>* visitor) override {
        visitor->visitPrintStmt(this);
    }
    
    std::unique_ptr<Expr> value;
};

class Block : public Stmt {
public:
    explicit Block(std::list<std::unique_ptr<Stmt>>&& statements) : statements{std::move(statements)} 
    {}
    virtual ~Block() override = default;

    virtual void accept(StmtVisitor<void>* visitor) override {
        visitor->visitBlockStmt(this);
    }

    std::list<std::unique_ptr<Stmt>> statements;
};

class If : public Stmt {
public: 
    If(std::unique_ptr<Expr>& condition, std::unique_ptr<Stmt>& thenBranch, std::unique_ptr<Stmt>& elseBranch) :
    condition{std::move(condition)}, thenBranch{std::move(thenBranch)}, elseBranch{std::move(elseBranch)}
    {}
    virtual ~If() override = default;

    virtual void accept(StmtVisitor<void>* visitor) override {
        visitor->visitIfStmt(this);
    }

    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> thenBranch;
    std::unique_ptr<Stmt> elseBranch;
};

class While : public Stmt {
public:
    While(std::unique_ptr<Expr>& expr, std::unique_ptr<Stmt>& body) 
    : expr{std::move(expr)}, body{std::move(body)}
    {}
    virtual ~While(){};

    void accept(StmtVisitor<void>* visitor) {
        visitor->visitWhileStmt(this);
    }

    std::unique_ptr<Expr> expr;
    std::unique_ptr<Stmt> body;
};

class Function : public Stmt {
public:
    explicit Function(const Token& name, std::list<Token>& params, std::list<std::unique_ptr<Stmt>>& body) 
    : name{name}, params{params}, body{std::move(body)}
    {}
    virtual ~Function(){}

    virtual void accept(StmtVisitor<void>* visitor) override {
        visitor->visitFunctionStmt(this);
    }

    Token name;
    std::list<Token> params;
    std::list<std::unique_ptr<Stmt>> body;

};

class Return : public Stmt {
public:

    explicit Return(const Token& keyword, std::unique_ptr<Expr>& value) 
    : keyword{keyword}, value{std::move(value)}
    {}
    virtual ~Return(){}

    virtual void accept(StmtVisitor<void>* visitor) override {
        visitor->visitReturnStmt(this);
    }

    Token keyword;
    std::unique_ptr<Expr> value;

};

}

#endif