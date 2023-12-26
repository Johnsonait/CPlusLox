#ifndef EXPR_HPP
#define EXPR_HPP

#include "token.hpp"
#include "value.hpp"

#include <string>
#include <memory>

namespace Lox {

class Binary;
class Grouping;
class Unary;
class Literal;

//==============================================================================
// Abstract Visitor
//==============================================================================

class ExprVisitor {
public:
    virtual ~ExprVisitor(){}

    virtual void visitBinaryExpr(Binary*) = 0;
    virtual void visitGroupingExpr(Grouping*) = 0;
    virtual void visitUnaryExpr(Unary*) = 0;
    virtual void visitLiteralExpr(Literal*) = 0;

    virtual std::string describeBinaryExpr(Binary*) = 0;
    virtual std::string describeGroupingExpr(Grouping*) = 0;
    virtual std::string describeUnaryExpr(Unary*) = 0;
    virtual std::string describeLiteralExpr(Literal*) = 0;
};

//==============================================================================
// Abstract expression
//==============================================================================

class Expr {

public:
    virtual ~Expr(){}

    virtual void accept(ExprVisitor*) = 0;
    virtual std::string describe(ExprVisitor*) = 0;

};

//==============================================================================
// Concrete expressions
//==============================================================================

class Binary : public Expr {
public:
    Binary(std::unique_ptr<Expr> l, const Token& op, std::unique_ptr<Expr> r) 
    : left{std::move(l)}, op{op},  right{std::move(r)}
    {}
    virtual ~Binary() override = default;

    virtual void accept(ExprVisitor* visitor) override {
        visitor->visitBinaryExpr(this);
    }

    virtual std::string describe(ExprVisitor* visitor) override {
        return visitor->describeBinaryExpr(this);
    }

    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;

};

class Grouping : public Expr {
public:
    Grouping(std::unique_ptr<Expr> expr)
    : expression{std::move(expr)}
    {}
    virtual ~Grouping() override = default;

    virtual void accept(ExprVisitor* visitor) override {
        visitor->visitGroupingExpr(this);
    }

    virtual std::string describe(ExprVisitor* visitor) override {
        return visitor->describeGroupingExpr(this);
    }

    std::unique_ptr<Expr> expression;
};

class Unary : public Expr {
public:
    Unary(const Token& op, std::unique_ptr<Expr> expr)
    : op{op}, expression{std::move(expr)}
    {}
    virtual ~Unary() override = default;

    virtual void accept(ExprVisitor* visitor) override {
        visitor->visitUnaryExpr(this);
    }

    virtual std::string describe(ExprVisitor* visitor) override {
        return visitor->describeUnaryExpr(this);
    }

    Token op;
    std::unique_ptr<Expr> expression;

}; 

class Literal : public Expr {
public:
    Literal(const double& v) : value{v}
    {}
    Literal(const std::string& v) : value{v}
    {}
    Literal(const std::monostate& v) : value{v}
    {}
    Literal(const Value& v) : value{v}
    {}
    virtual ~Literal() override = default;

    virtual void accept(ExprVisitor* visitor) override {
        visitor->visitLiteralExpr(this);
    }

    virtual std::string describe(ExprVisitor* visitor) override {
        return visitor->describeLiteralExpr(this);
    }

    Value value;

};

} // Lox namespace

#endif