#ifndef PARSER_HPP
#define PARSER_HPP

#include "lox.hpp"
#include "token_type.hpp"
#include "token.hpp"
#include "expr.hpp"
#include "errors.hpp"

#include <vector>
#include <list>
#include <string>

namespace Lox {

class Parser {
public:
    Parser(const std::list<Token>& t);
    Parser(const std::vector<Token>& t);

    std::unique_ptr<Expr> parse();

private:
    int current = 0;
    std::vector<Token> tokens;

    // Expression handling
    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> primary();

    // Error handling
    Token consume(const TokenType& type, const std::string& message);
    ParseError error(const Token& token, const std::string& message);
    void synchronize();

    // Utility methods
    bool match(std::initializer_list<TokenType> types);
    bool check(const TokenType& type);
    Token advance();
    Token peek();
    Token previous();
    bool isAtEnd();

};

} // Lox namespace

#endif