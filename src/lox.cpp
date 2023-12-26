#include "../include/lox.hpp"

namespace Lox {

bool Lox::hadError = false;

void Lox::main(std::vector<std::string>& args) {
    if(args.size() > 1){
        std::cout << "Usage: jlox [script]" << std::endl;
    } else if(args.size() == 1){
        Lox::runFile(args[0]);
    } else {
        Lox::runPrompt();
    }
}

void Lox::runFile(std::string& path) {
    std::fstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << path << std::endl;
        return;
    }
    std::string source((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    run(source);

    if (Lox::hadError) {
        std::exit(1);
    }
}

void Lox::runPrompt(){

    for (;;) {
        std::cout << "> ";
        std::string line;
        std::getline(std::cin, line);
        if (line.empty()) break;
        run(line);
        Lox::hadError = false;
    }
}

void Lox::run(std::string& source) {
    // Move through the provided source and create a list of tokens
    Scanner scanner{source};
    std::list<Token> tokens = scanner.scanTokens();

    // Parse those tokens into expressions
    Parser parser{tokens};
    auto expression = parser.parse();

    if (hadError) return;

    // Print out the Abstract Syntax Tree that defines the expression
    AstPrinter printer{};
    std::cout<<printer.print(expression);

    // std::cout<< tokens.size()<<std::endl;
    // for (auto token : tokens) {
        // std::cout << token << std::endl;
    // }

}

void Lox::error(const int line,const std::string& message) {
    report(line, std::string(""), message);
}

void Lox::error(const Token& token, const std::string& message) {
    if (token.type == TokenType::END) {
        report(token.line, " at end", message);
    } else {
        report(token.line, " at '" + token.lexeme + "'", message);

    }
}

void Lox::report(const int line, const std::string& where,const std::string& message) {
    std::cerr << "[line " << line << "] Error " << where << ": " << message << std::endl;
    Lox::hadError = true;
}

} // namespace Lox