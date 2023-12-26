#ifndef PARSER_HPP
#define PARSER_HPP

#include "token_type.hpp"
#include "token.hpp"
#include "expr.hpp"

#include <vector>
#include <list>

namespace Lox {

class Parser {
public:
    Parser(const std::list<Token>& t);
    Parser(const std::vector<Token>& t);

private:
    int current = 0;
    std::vector<Token> tokens;

    bool match(std::initializer_list<TokenType> types);
    bool check(const TokenType& type);
    Token advance();
    Token peek();
    Token previous();
    bool isAtEnd();

    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> primary();

};

} // Lox namespace

#endif