#include "../include/parser.hpp"

namespace Lox {

//==============================================================================
// Constructors
//==============================================================================
Parser::Parser(const std::list<Token>& t) : tokens{t.begin(),t.end()}
{}
Parser::Parser(const std::vector<Token>& t) : tokens{t}
{}
//==============================================================================
// Expression handling
//==============================================================================

std::unique_ptr<Expr> Parser::parse() {
    try {
        return expression();
    } catch (ParseError& e)
    {
        return nullptr;
    }
    
}

std::unique_ptr<Expr> Parser::expression() {
    return equality();
}

std::unique_ptr<Expr> Parser::equality() {
    std::unique_ptr<Expr> expr = comparison();
    while(match({TokenType::BANG_EQUAL,TokenType::EQUAL_EQUAL})) {
        Token op = previous();
        std::unique_ptr<Expr> right = comparison();
        expr = std::make_unique<Binary>(std::move(expr),op,std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::comparison() {
    std::unique_ptr<Expr> expr = term();

    while(match({
                    TokenType::GREATER,
                    TokenType::GREATER_EQUAL,
                    TokenType::LESS,
                    TokenType::LESS_EQUAL
                })) {
        Token op = previous();
        std::unique_ptr<Expr> right = term();
        expr = std::make_unique<Binary>(std::move(expr),op,std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::term() {
    std::unique_ptr<Expr> expr = factor();

    while(match({TokenType::MINUS,TokenType::PLUS})) {
        Token op = previous();
        std::unique_ptr<Expr> right = factor();
        expr = std::make_unique<Binary>(std::move(expr),op,std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::factor() {
    std::unique_ptr<Expr> expr = unary();

    while(match({TokenType::SLASH,TokenType::STAR})) {
        Token op = previous();
        std::unique_ptr<Expr> right = unary();
        expr = std::make_unique<Binary>(std::move(expr),op,std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::unary() {
    if (match({TokenType::BANG,TokenType::MINUS})) {
        Token op = previous();
        std::unique_ptr<Expr> right = unary();
        return std::make_unique<Unary>(op,std::move(right));
    }
    return primary();
}

std::unique_ptr<Expr> Parser::primary() {
    if (match({TokenType::FALSE})) return std::make_unique<Literal>(false);
    if (match({TokenType::TRUE})) return std::make_unique<Literal>(true);
    if (match({TokenType::NIL})) return std::make_unique<Literal>(std::monostate{});

    if (match({TokenType::NUMBER,TokenType::STRING})) {
        return std::make_unique<Literal>(Value{previous().literal});
    }
    
    if (match({TokenType::LEFT_PAREN})) {
        std::unique_ptr<Expr> expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_unique<Grouping>(std::move(expr));
    }

    throw error(peek(), "Expect expression.");

}

//==============================================================================
// Error handling
//==============================================================================
Token Parser::consume(const TokenType& type, const std::string& message) {
    if (check(type)) return advance();
    throw error(peek(),message);
}

ParseError Parser::error(const Token& token, const std::string& message) {
    Lox::error(token,message);
    return ParseError{};
}

void Parser::synchronize() {
    advance();
    while(!isAtEnd()) {
        if (previous().type == TokenType::SEMICOLON) return;

        switch (peek().type)
        {
        case TokenType::CLASS:
        case TokenType::FOR:
        case TokenType::FUN:
        case TokenType::IF:
        case TokenType::PRINT:
        case TokenType::RETURN:
        case TokenType::VAR:
        case TokenType::WHILE:
            return;
        default:
            break;
        }

        advance();
    }


}

//==============================================================================
// Utility functions
//==============================================================================
bool Parser::match(std::initializer_list<TokenType> types) {
    for (auto& type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

bool Parser::check(const TokenType& type) {
    if (isAtEnd()) return false;
    return peek().type == type;
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

Token Parser::peek() {
    return tokens[current];
}

Token Parser::previous() {
    return tokens[current-1];
}

bool Parser::isAtEnd() {
    return peek().type == TokenType::END;
}

} // Lox namespace