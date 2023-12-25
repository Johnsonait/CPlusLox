#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "token_type.hpp"

#include <string>
#include <iostream>
#include <variant>

namespace Lox {

class Token {
public:
    Token() = default;
    Token(
        const TokenType& type,
        const std::string& lexeme,
        std::variant<double,std::string,std::monostate> literal,
        int line
    ) : _type{type}, _lexeme{lexeme}, _literal{literal}, _line{line}
    {}

    ~Token() = default;

    // We can print out info about tokens in a fairly elegant way by making it
    // work with the C++ stream objects
    friend std::ostream& operator<<(std::ostream& stream, const Token& token) {
        return stream << token._lexeme << std::endl;
    }

    TokenType _type;
    std::string _lexeme;
    int _line;
    // An alleged equivalent to the Java Object type for storing literals for Lox.
    // Definitely not the most elegant solution but it allows for the two "base" 
    // types of literal as well as a null equivalent (monostate)
    std::variant<double,std::string,std::monostate> _literal;

    
}; 

} // namespace Lox


#endif