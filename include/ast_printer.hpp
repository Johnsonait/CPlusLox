#ifndef AST_PRINTER_HPP
#define AST_PRINTER_HPP

#include "expr.hpp"
#include "value.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <variant>
#include <type_traits>

namespace Lox {

class AstPrinter : public ExprVisitor<std::string> {
public: 
    virtual ~AstPrinter() override = default;

    void main(const std::vector<std::string>& args) {
        std::unique_ptr<Expr> expr = std::make_unique<Binary>(
            std::make_unique<Unary>(
                Token{TokenType::MINUS,"-",std::monostate{},1},
                std::make_unique<Literal>(123)
            ),
            Token{TokenType::STAR,"*",std::monostate{},1},
            std::make_unique<Grouping>(
                std::make_unique<Literal>(45.67)
            )
        );

        std::cout<<print(expr);
    }

    std::string print(std::unique_ptr<Expr>& expr){
        return expr->accept(this);
    }

    std::string parenthesize(const std::string& name, std::unique_ptr<Expr>& expr) {
        std::stringstream builder{};

        builder << "(" << name;
        builder << " " << expr->accept(this);
        builder << ")";

        return builder.str();
    }

    std::string parenthesize(const std::string& name, std::unique_ptr<Expr>& expr1,std::unique_ptr<Expr>& expr2) {
        std::stringstream builder{};

        builder << "(" << name;
        builder << " " << expr1->accept(this);
        builder << " " << expr2->accept(this);
        builder << ")";

        return builder.str();
    }

    // Describers
    virtual std::string visitBinaryExpr(Binary* b) override {
        return parenthesize(b->op.lexeme,b->left,b->right);
    }

    virtual std::string visitGroupingExpr(Grouping* g) override {
        return parenthesize("group",g->expression);
    }

    virtual std::string visitUnaryExpr(Unary* u) override {
        return parenthesize(u->op.lexeme, u->expression);
    }

    virtual std::string visitLiteralExpr(Literal* l) override {
        auto& val = l->value.item;
        // Float
        if (std::holds_alternative<double>(val)) {
            return std::to_string(std::get<double>(val));
        // String
        } else if (std::holds_alternative<std::string>(val)) {
            return std::get<std::string>(val);
        // Bool
        } else if (std::holds_alternative<bool>(val)) {
            return std::to_string(std::get<bool>(val));
        // Null
        } else if (std::holds_alternative<std::monostate>(val)) {
            return "nil";
        // Error
        } else {
            throw std::runtime_error("Unexpected type in variant");
        }
    }

};

} // Lox namespace

#endif