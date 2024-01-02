#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "lox_callable.hpp"
#include "value.hpp"
#include "token_type.hpp"

#include <string>
#include <iostream>
#include <variant>

namespace Lox {

class Token {
public:
    Token() = default;
    // Token(
    //     const TokenType& type,
    //     const std::string& lexeme,
    //     std::variant<double,bool,std::string,std::monostate, std::shared_ptr<LoxCallable>> literal,
    //     int line
    // ) : type{type}, lexeme{lexeme}, literal{literal}, line{line}
    // {}
    Token(const TokenType& type,const std::string& lexeme, Value literal, int line)
    : type{type}, lexeme{lexeme}, literal{literal}, line{line}
    {}

    ~Token() = default;

    // We can print out info about tokens in a fairly elegant way by making it
    // work with the C++ stream objects
    friend std::ostream& operator<<(std::ostream& stream, const Token& token) {
        return stream << token.lexeme << std::endl;
    }

    TokenType type;
    std::string lexeme;
    int line;
    // An alleged equivalent to the Java Object type for storing literals for Lox.
    // Definitely not the most elegant solution but it allows for the two "base" 
    // types of literal as well as a null equivalent (monostate)
    Value literal;
    // std::variant<
    //     double,
    //     bool,
    //     std::string,
    //     std::monostate,
    //     std::shared_ptr<LoxCallable>
    //     > literal;

    
}; 

} // namespace Lox


#endif