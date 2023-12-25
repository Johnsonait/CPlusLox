#ifndef LOX_HPP
#define LOX_HPP

#include "scanner.hpp"
#include "token.hpp"
#include "expr_visitor.hpp"
#include "ast_printer.hpp"

#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <list>

namespace Lox 
{
class Lox {
public:
    Lox() = default;
    ~Lox() = default;

    void main(std::vector<std::string>& args);
    static void error(const int line, const std::string& message);

    static bool hadError;
private:
    void runFile(std::string& path);
    void runPrompt();
    void run(std::string& source);
    static void report(const int line,const std::string& where,const std::string& message);

};

} // namespace Lox

#endif