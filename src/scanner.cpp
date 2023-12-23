#include "../include/scanner.hpp"

namespace Lox {

Scanner::Scanner(std::string& source) : _source{source}
{}

std::list<Token> Scanner::scanTokens() {

    return std::list<Token>{};

}

} // Lox namespace