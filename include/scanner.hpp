#ifndef SCANNER_HPP
#define SCANNER_HPP

#include "token.hpp"
#include "token_type.hpp"
#include "lox.hpp"
#include "value.hpp"

#include <list>
#include <variant>
#include <unordered_map>

namespace Lox {

class Scanner {
public:
    Scanner() = default;
    Scanner(std::string& source);
    ~Scanner() = default;

    std::list<Token> scanTokens();

private:
    void scanToken();
    bool isAtEnd();
    char advance();
    char peek();
    char peekNext();
    void addToken(const TokenType&);
    void addToken(const TokenType&, Value);
    bool match(char);
    void string();
    bool isDigit(char);
    void number();
    bool isAlpha(char);
    bool isAlphaNumeric(char);
    void identifier();

    static std::unordered_map<std::string, TokenType> keywords;
    
    std::string _source;
    std::list<Token> _tokens;

    int _start = 0;
    int _current = 0;
    int _line = 1;

};

} // Lox namespace

#endif