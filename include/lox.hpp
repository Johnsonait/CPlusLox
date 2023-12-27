#ifndef LOX_HPP
#define LOX_HPP

#include "interpreter.hpp"
#include "scanner.hpp"
#include "token.hpp"
#include "expr.hpp"
#include "value.hpp"
#include "ast_printer.hpp"
#include "parser.hpp"

#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <list>

namespace Lox 
{

class Interpreter;

class Lox {
public:
    Lox() = default;
    ~Lox() = default;

    void main(std::vector<std::string>& args);
    static void error(const int line, const std::string& message);
    static void error(const Token& token, const std::string& message);
    static void runtimeError(RuntimeError& error);

    static bool hadError;
    static bool hadRuntimeError;
    static Interpreter interpreter;
private:
    void runFile(std::string& path);
    void runPrompt();
    void run(std::string& source);
    static void report(const int line,const std::string& where,const std::string& message);

};

} // namespace Lox

#endif