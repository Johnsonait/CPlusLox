#ifndef SCANNER_HPP
#define SCANNER_HPP

#include "token.hpp"

#include <list>

namespace Lox {

class Scanner {
public:
    Scanner() = default;
    Scanner(std::string& source);
    ~Scanner() = default;

    std::list<Token> scanTokens();

private:
    std::string _source;

};

} // Lox namespace

#endif