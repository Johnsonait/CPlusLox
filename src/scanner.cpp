#include "../include/scanner.hpp"

namespace Lox {

std::unordered_map<std::string, TokenType> Scanner::keywords{
    {"and", TokenType::AND},
    {"class", TokenType::CLASS},
    {"else", TokenType::ELSE},
    {"false", TokenType::FALSE},
    {"for", TokenType::FOR},
    {"fun", TokenType::FUN},
    {"if", TokenType::IF},
    {"nil", TokenType::NIL},
    {"or", TokenType::OR},
    {"print", TokenType::PRINT},
    {"return", TokenType::RETURN},
    {"super", TokenType::SUPER},
    {"this", TokenType::THIS},
    {"true", TokenType::TRUE},
    {"var", TokenType::VAR},
    {"while", TokenType::WHILE}
};

Scanner::Scanner(std::string& source) : _source{source}, _tokens{}
{}

bool Scanner::isAtEnd() {
    return _current >= _source.length();
}

char Scanner::advance() {
    return _source[_current++];
}

char Scanner::peek() {
    if (isAtEnd()) return '\0';
    return _source[_current];
}

char Scanner::peekNext() {
    if (_current + 1 >= _source.length()) return '\0';
    return _source[_current+1];

}

bool Scanner::isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool Scanner::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Scanner::isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
}

void Scanner::addToken(const TokenType& type) {
    addToken(type, std::monostate{});
}

void Scanner::addToken(const TokenType& type, std::variant<double,std::string,std::monostate> literal) {
    std::string text = _source.substr(_start, _current-_start);
    _tokens.push_back(
        Token{type,text,literal,_line}
    );
}

bool Scanner::match(char expected) {
    if (isAtEnd()) return false;
    if (_source[_current] != expected) return false;

    _current++;
    return true;
}

void Scanner::number() {
    while(isDigit(peek())) advance();

    // Look for a fractional (decimal) part
    if (peek() == '.' && isDigit(peekNext())) {
        advance();

        while (isDigit(peek())) advance();
    }

    addToken(TokenType::NUMBER,std::stod(_source.substr(_start,_current-_start)));

}

void Scanner::identifier() {
    while(isAlphaNumeric(peek())) advance();

    addToken(TokenType::IDENTIFIER);
}

void Scanner::string() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') _line++;
        advance();
    }

    if (isAtEnd()) {
        Lox::error(_line, "Unterminated string.");
        return;
    }

    // Closing part of string
    advance();

    // Remove surrounding quote symbols 
    std::string value = _source.substr(_start + 1, _current -_start - 2);
    addToken(TokenType::STRING, value);
}

std::list<Token> Scanner::scanTokens() {

    while (!isAtEnd()) {
        // Beginning of next lexeme
        _start = _current;
        scanToken();
    }
    Token final_token{TokenType::END, "", std::monostate{}, _line};
    _tokens.push_back(final_token);

    return _tokens;
}

void Scanner::scanToken() {
    char c = advance();
    switch (c)
    {
    case '(': addToken(TokenType::LEFT_PAREN);break;
    case ')': addToken(TokenType::RIGHT_PAREN); break; 
    case '{': addToken(TokenType::LEFT_BRACE); break; 
    case '}': addToken(TokenType::RIGHT_BRACE); break; 
    case ',': addToken(TokenType::COMMA); break;
    case '.': addToken(TokenType::DOT); break;
    case '-': addToken(TokenType::MINUS); break; 
    case '+': addToken(TokenType::PLUS); break;
    case ';': addToken(TokenType::SEMICOLON); break; 
    case '*': addToken(TokenType::STAR); break;
    case '!': addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG); break;
    case '=': addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL); break;
    case '<': addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS); break;
    case '>': addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER); break;
    case '/': {
        if (match('/')) {
            while(peek() != '\n' && !isAtEnd()) advance();
        } else {
            addToken(TokenType::SLASH);
        }
        break;
    }
    case ' ': 
    case '\r':
    case '\t':
        break;
    case '\n': _line++; break;
    case '"': string(); break;
    default:
        if (isDigit(c)) {
            number();
        } else if (isAlpha(c)) {
            identifier();
        } else {
            Lox::error(_line,"Unexpected character.");
        }
        break;
    }
}

} // Lox namespace