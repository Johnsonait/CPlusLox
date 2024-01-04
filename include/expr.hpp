#ifndef EXPR_HPP
#define EXPR_HPP

#include "token.hpp"
#include "value.hpp"

#include <string>
#include <list>
#include <memory>

namespace Lox {

class Binary;
class Grouping;
class Unary;
class Variable;
class Assign;
class Literal;
class Logical;
class Call;
class Get;
class Set;

//==============================================================================
// Abstract Visitor
//==============================================================================
template<typename T>
class ExprVisitor {
public:
    virtual ~ExprVisitor(){}

    virtual T visitBinaryExpr(Binary*) = 0;
    virtual T visitGroupingExpr(Grouping*) = 0;
    virtual T visitUnaryExpr(Unary*) = 0;
    virtual T visitLiteralExpr(Literal*) = 0;
    virtual T visitVariableExpr(Variable*) = 0;
    virtual T visitAssignExpr(Assign*) = 0;
    virtual T visitLogicalExpr(Logical*) = 0;
    virtual T visitCallExpr(Call*) = 0;
    virtual T visitGetExpr(Get*) = 0;
    virtual T visitSetExpr(Set*) = 0;

};

//==============================================================================
// Abstract expression
//==============================================================================

class Expr {

public:
    virtual ~Expr(){}

    virtual void accept(ExprVisitor<void>*) = 0;
    virtual std::string accept(ExprVisitor<std::string>*) = 0;
    virtual Value accept(ExprVisitor<Value>*) = 0;

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

    virtual void accept(ExprVisitor<void>* visitor) override {
        visitor->visitBinaryExpr(this);
    }

    virtual std::string accept(ExprVisitor<std::string>* visitor) override {
        return visitor->visitBinaryExpr(this);
    }

    virtual Value accept(ExprVisitor<Value>* visitor) override {
        return visitor->visitBinaryExpr(this);
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

    virtual void accept(ExprVisitor<void>* visitor) override {
        visitor->visitGroupingExpr(this);
    }

    virtual std::string accept(ExprVisitor<std::string>* visitor) override {
        return visitor->visitGroupingExpr(this);
    }

    virtual Value accept(ExprVisitor<Value>* visitor) override {
        return visitor->visitGroupingExpr(this);
    }

    std::unique_ptr<Expr> expression;
};

class Unary : public Expr {
public:
    Unary(const Token& op, std::unique_ptr<Expr> expr)
    : op{op}, expression{std::move(expr)}
    {}
    virtual ~Unary() override = default;

    virtual void accept(ExprVisitor<void>* visitor) override {
        visitor->visitUnaryExpr(this);
    }

    virtual std::string accept(ExprVisitor<std::string>* visitor) override {
        return visitor->visitUnaryExpr(this);
    }

    virtual Value accept(ExprVisitor<Value>* visitor) override {
        return visitor->visitUnaryExpr(this);
    }

    Token op;
    std::unique_ptr<Expr> expression;

}; 

class Literal : public Expr {
public:
    explicit Literal(const double& v) : value{v}
    {}
    explicit Literal(const bool& v) : value{v}
    {}
    explicit Literal(const std::string& v) : value{v}
    {}
    explicit Literal(const std::monostate& v) : value{v}
    {}
    explicit Literal(const std::shared_ptr<LoxCallable>& v) : value{v}
    {}
    Literal(const Value& v) : value{v}
    {}
    virtual ~Literal() override = default;

    virtual void accept(ExprVisitor<void>* visitor) override {
        visitor->visitLiteralExpr(this);
    }

    virtual std::string accept(ExprVisitor<std::string>* visitor) override {
        return visitor->visitLiteralExpr(this);
    }

    virtual Value accept(ExprVisitor<Value>* visitor) override {
        return visitor->visitLiteralExpr(this);
    }

    Value value;

};

class Variable : public Expr {
public:
    explicit Variable(const Token& name) : name{name}
    {}
    virtual ~Variable() override = default;

    virtual void accept(ExprVisitor<void>* visitor) override {
        visitor->visitVariableExpr(this);
    }

    virtual std::string accept(ExprVisitor<std::string>* visitor) override {
        return visitor->visitVariableExpr(this);
    }

    virtual Value accept(ExprVisitor<Value>* visitor) override {
        return visitor->visitVariableExpr(this);
    }

    Token name;
};

class Assign : public Expr {
public:
    Assign(const Token& name, std::unique_ptr<Expr>& value) : name{name}, value{std::move(value)}
    {}
    virtual ~Assign() override = default;

    virtual void accept(ExprVisitor<void>* visitor) override {
        visitor->visitAssignExpr(this);
    }

    virtual std::string accept(ExprVisitor<std::string>* visitor) override {
        return visitor->visitAssignExpr(this);
    }

    virtual Value accept(ExprVisitor<Value>* visitor) override {
        return visitor->visitAssignExpr(this);
    }

    Token name;
    std::unique_ptr<Expr> value;
};

class Logical : public Expr {
public:
    Logical(std::unique_ptr<Expr>& left, const Token& op, std::unique_ptr<Expr>& right) 
        : left{std::move(left)}, op{op}, right{std::move(right)}
    {}
    virtual ~Logical() = default;

    virtual void accept(ExprVisitor<void>* visitor) override {
        return visitor->visitLogicalExpr(this);
    }

    virtual std::string accept(ExprVisitor<std::string>* visitor) override {
        return visitor->visitLogicalExpr(this);
    }

    virtual Value accept(ExprVisitor<Value>* visitor) override {
        return visitor->visitLogicalExpr(this);
    }

    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;

};

class Call : public Expr {

public:
    Call(std::unique_ptr<Expr>& callee, const Token paren, std::list<std::unique_ptr<Expr>>& args)
    : callee{std::move(callee)}, paren{paren}, arguments{std::move(args)}
    {}
    virtual ~Call() = default;

    void accept(ExprVisitor<void>* visitor) override {
        visitor->visitCallExpr(this);
    }

    std::string accept(ExprVisitor<std::string>* visitor) override {
        return visitor->visitCallExpr(this);
    }

    Value accept(ExprVisitor<Value>* visitor) override {
        return visitor->visitCallExpr(this);
    }

    std::unique_ptr<Expr> callee;
    Token paren;
    std::list<std::unique_ptr<Expr>> arguments;
};

class Get : public Expr {

public:
    Get(std::unique_ptr<Expr>& object, const Token& name) 
    : object{std::move(object)}, name{name}
    {}

    virtual ~Get() = default;

    void accept(ExprVisitor<void>* visitor) override {
        visitor->visitGetExpr(this);
    }

    std::string accept(ExprVisitor<std::string>* visitor) override {
        return visitor->visitGetExpr(this);
    }

    Value accept(ExprVisitor<Value>* visitor) override {
        return visitor->visitGetExpr(this);
    }

    std::unique_ptr<Expr> object;
    Token name;
};

class Set : public Expr {
public:
    Set(std::unique_ptr<Expr>& object, const Token& name, std::unique_ptr<Expr>& value)
    : object{std::move(object)}, name{name}, value{std::move(value)}
    {}

    virtual ~Set() = default;

    void accept(ExprVisitor<void>* visitor) override {
        visitor->visitSetExpr(this);
    }

    std::string accept(ExprVisitor<std::string>* visitor) override {
        return visitor->visitSetExpr(this);
    }

    Value accept(ExprVisitor<Value>* visitor) override {
        return visitor->visitSetExpr(this);
    }

    std::unique_ptr<Expr> object;
    Token name;
    std::unique_ptr<Expr> value;

};

} // Lox namespace

#endif