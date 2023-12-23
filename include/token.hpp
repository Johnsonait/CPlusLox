#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "token_type.hpp"

#include <string>
#include <iostream>
#include <variant>

namespace Lox {

class Token {
public:
    Token() = delete;
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

private:
    const TokenType _type;
    const std::string _lexeme;
    const int _line;
    // An alleged equivalent to the Java Object type for storing literals for Lox.
    const std::variant<double,std::string,std::monostate> _literal;

    
}; 

} // namespace Lox


#endif