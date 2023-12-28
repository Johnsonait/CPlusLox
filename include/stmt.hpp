#ifndef STMT_HPP
#define STMT_HPP

namespace Lox {

class Expression;
class Print;

//==============================================================================
// Statement visitor interface
//==============================================================================
template<typename T>
class StmtVisitor {
public:
    virtual ~StmtVisitor(){}

    virtual T visitExpressionStmt(Expression*) = 0;
    virtual T visitPrintStmt(Print*) = 0;
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


}

#endif