#include "../include/parser.hpp"

namespace Lox {

int Parser::MAXIMUM_FUNCTION_ARGS = 255;
//==============================================================================
// Constructors
//==============================================================================
Parser::Parser(const std::list<Token>& t) : tokens{t.begin(),t.end()}
{}
Parser::Parser(const std::vector<Token>& t) : tokens{t}
{}

//==============================================================================
// Statement handling
//==============================================================================
std::vector<std::unique_ptr<Stmt>> Parser::parse() {
    auto statements = std::vector<std::unique_ptr<Stmt>>{};
    while(!isAtEnd()) {
        statements.push_back(declaration());
    }

    return statements;
}

std::unique_ptr<Stmt> Parser::statement() {
    if (match({TokenType::IF})) return ifStatement();
    if (match({TokenType::PRINT})) return printStatement();
    if (match({TokenType::FOR})) return forStatement();
    if (match({TokenType::WHILE})) return whileStatement();
    if (match({TokenType::LEFT_BRACE})) return std::make_unique<Block>(block());

    return expressionStatement();
}

std::unique_ptr<Stmt> Parser::ifStatement() {
    consume(TokenType::LEFT_PAREN,"Expect '(' after 'if'.");
    std::unique_ptr<Expr> condition = expression();
    consume (TokenType::RIGHT_PAREN, "Expect ')' after if condition.");

    auto thenBranch = statement();
    auto elseBranch = std::unique_ptr<Stmt>{};
    if (match({TokenType::ELSE})) {
        elseBranch = statement();
    }

    return std::make_unique<If>(
        condition,
        thenBranch,
        elseBranch
    );

}

std::unique_ptr<Stmt> Parser::whileStatement() {
    consume(TokenType::LEFT_PAREN,"Expect '(' after 'while'.");
    auto condition = expression();

    consume(TokenType::RIGHT_PAREN,"Expect ')' after condition");
    auto body = statement();

    return std::make_unique<While>(condition,body);
}

std::unique_ptr<Stmt> Parser::forStatement() {
    consume(TokenType::LEFT_PAREN,"Expect '(' after 'for'.");
    // Basic structure
    // for(initializer; condition; increment;) {
    //        body
    //  }

    // Initializer part of FOR loop
    std::unique_ptr<Stmt> initializer{};
    if (match({TokenType::SEMICOLON})) {
        // Immediate semicolon means initializer is omitted
    } else if (match({TokenType::VAR})) {
        initializer = varDeclaration();
    } else {
        initializer = expressionStatement();
    }

    // Condition part
    std::unique_ptr<Expr> condition{};
    if (!check(TokenType::SEMICOLON)) {
        condition = expression();
    }
    consume(TokenType::SEMICOLON, "Expect ';' after loop condition.");

    // Increment part
    std::unique_ptr<Expr> increment{};
    if (!check(TokenType::RIGHT_PAREN)) {
        increment = expression();
    }
    consume(TokenType::RIGHT_PAREN, "Expect ')' after for clauses.");

    // Body
    std::unique_ptr<Stmt> body = statement();
    std::list<std::unique_ptr<Stmt>> stmts;
    // Desugaring for-loop syntax into lox AST nodes
    if (increment.get() != nullptr) {
        stmts.push_back(std::move(body));
        stmts.push_back(std::make_unique<Expression>(increment));
        body = std::make_unique<Block>(std::move(stmts));
    }
    if (condition.get() == nullptr) {
        condition = std::make_unique<Literal>(true);
    }
    stmts = std::list<std::unique_ptr<Stmt>>{};
    body = std::make_unique<While>(condition,body);
    if (initializer.get() != nullptr) {
        stmts.push_back(std::move(initializer));
        stmts.push_back(std::move(body));
        body = std::make_unique<Block>(std::move(stmts));
    }

    return body;
}

std::unique_ptr<Stmt> Parser::declaration() {
    try {
        if (match({TokenType::VAR})) return varDeclaration();
        return statement();
    } catch (ParseError& error) {
        synchronize();
        return nullptr;
    }

}

std::unique_ptr<Stmt> Parser::varDeclaration() {
    // Once we reconginze a var, we need to find the identifier or throw
    auto name = consume(TokenType::IDENTIFIER, "Expect variable name");
    
    // If there is some kind of initializer, get it after the equals sign.
    std::unique_ptr<Expr> initializer{};
    if (match({TokenType::EQUAL})) {
        initializer = expression();
    }

    // Search for the end of the statement or throw
    consume(TokenType::SEMICOLON, "Expect ';' after variable declaration");
    return std::make_unique<Var>(name,initializer);
}

std::unique_ptr<Stmt> Parser::printStatement() {
    auto value = expression();
    consume(TokenType::SEMICOLON,"Expect ';' after value.");
    return std::make_unique<Print>(value);
}
std::unique_ptr<Stmt> Parser::expressionStatement() {
    auto expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after expression.");
    return std::make_unique<Expression>(expr);
}

std::list<std::unique_ptr<Stmt>> Parser::block() {
    auto statements = std::list<std::unique_ptr<Stmt>>{};

    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        statements.push_back(declaration());
    }

    consume(TokenType::RIGHT_BRACE,"Expect '}' after block.");
    return statements;
}

//==============================================================================
// Expression handling
//==============================================================================
std::unique_ptr<Expr> Parser::expression() {
    return assignment();
}

std::unique_ptr<Expr> Parser::assignment() {
    auto expr = or_expression();

    if (match({TokenType::EQUAL})) {
        auto equals = previous();
        auto value = assignment();

        if (Variable* v = dynamic_cast<Variable*>(expr.get())) {
            auto name = v->name;
            return std::make_unique<Assign>(name,value);
        }

        Lox::error(equals,"Invalid assignment target.");
    }

    return expr;
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

std::unique_ptr<Expr> Parser::or_expression() {
    auto expr = and_expression();

    while (match({TokenType::OR})) {
        Token op = previous();
        auto right = and_expression();
        expr = std::make_unique<Logical>(expr,op,right);
    }

    return expr;

}

std::unique_ptr<Expr> Parser::and_expression() {
    auto expr = equality();
    
    while(match({TokenType::AND})) {
        Token op = previous();
        auto right = equality();
        expr = std::make_unique<Logical>(expr,op,right);
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
    return call();
}

std::unique_ptr<Expr> Parser::primary() {
    if (match({TokenType::FALSE})) return std::make_unique<Literal>(false);
    if (match({TokenType::TRUE})) return std::make_unique<Literal>(true);
    if (match({TokenType::NIL})) return std::make_unique<Literal>(std::monostate{});

    if (match({TokenType::NUMBER,TokenType::STRING})) {
        return std::make_unique<Literal>(Value{previous().literal});
    }

    if (match({TokenType::IDENTIFIER})) {
        return std::make_unique<Variable>(previous());
    }
    
    if (match({TokenType::LEFT_PAREN})) {
        std::unique_ptr<Expr> expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_unique<Grouping>(std::move(expr));
    }

    throw error(peek(), "Expect expression.");

}

std::unique_ptr<Expr> Parser::call() {
    auto expr = primary();

    while(true) {
        if (match({TokenType::LEFT_PAREN})) {
            expr = finishCall(expr);
        } else {
            break;
        }
    }

    return expr;
}

std::unique_ptr<Expr> Parser::finishCall(std::unique_ptr<Expr>& callee) {
    std::list<std::unique_ptr<Expr>> args{};

    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            if (args.size() > Parser::MAXIMUM_FUNCTION_ARGS) {
                error(peek(),"Can't have more than "+std::to_string(Parser::MAXIMUM_FUNCTION_ARGS)+" arguments.");
            }
            args.push_back(expression());
        } while (match({TokenType::COMMA}));
    }

    Token paren = consume(TokenType::RIGHT_PAREN, "Expect ')' after arguments");

    return std::make_unique<Call>(callee,paren,args);
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