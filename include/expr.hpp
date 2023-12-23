#ifndef EXPR_HPP
#define EXPR_HPP

#include "token.hpp"

#include <list>
#include <string>
#include <variant>
#include <memory>

namespace Lox {

class Expr {

public:
	Expr() = default;
	virtual ~Expr() = 0;

	template<typename T>
	T accept(std::shared_ptr<Visitor<T>> visitor) {
	}
};

class Binary : public Expr {
public:
	Binary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right) {
		 this->left = left; 
		 this->op = op; 
		 this->right = right; 
	}
	virtual ~Binary() override = default;

	template<typename T>
	T accept(std::shared_ptr<Visitor<T>> visitor) {
		return visitor.visitBinaryExpr(this);
	}
	std::shared_ptr<Expr> left;
	Token op;
	std::shared_ptr<Expr> right;
};

class Grouping : public Expr {
public:
	Grouping(std::shared_ptr<Expr> expression) {
		 this->expression = expression; 
	}
	virtual ~Grouping() override = default;

	template<typename T>
	T accept(std::shared_ptr<Visitor<T>> visitor) {
		return visitor.visitGroupingExpr(this);
	}
	std::shared_ptr<Expr> expression;
};

class Literal : public Expr {
public:
	Literal(std::variant<double,std::string,std::monostate> value) {
		 this->value = value; 
	}
	virtual ~Literal() override = default;

	template<typename T>
	T accept(std::shared_ptr<Visitor<T>> visitor) {
		return visitor.visitLiteralExpr(this);
	}
	std::variant<double,std::string,std::monostate> value;
};

class Unary : public Expr {
public:
	Unary(Token op, std::shared_ptr<Expr> right) {
		 this->op = op; 
		 this->right = right; 
	}
	virtual ~Unary() override = default;

	template<typename T>
	T accept(std::shared_ptr<Visitor<T>> visitor) {
		return visitor.visitUnaryExpr(this);
	}
	Token op;
	std::shared_ptr<Expr> right;
};

template<typename T>
class Visitor {
	virtual T visitBinaryExpr(Binary expr) = 0;
	virtual T visitGroupingExpr(Grouping expr) = 0;
	virtual T visitLiteralExpr(Literal expr) = 0;
	virtual T visitUnaryExpr(Unary expr) = 0;
};

} // namespace Lox

#endif