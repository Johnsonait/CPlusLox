#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include <iostream>

namespace Lox {

class Token {
public:
    Token() = default;
    ~Token() = default;

    friend std::ostream& operator<<(std::ostream& stream, const Token& token) {
        return stream << token._what;
    }

private:
    std::string _what = "";
}; 

} // namespace Lox


#endif