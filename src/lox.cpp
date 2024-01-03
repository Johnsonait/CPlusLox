#include "../include/lox.hpp"

namespace Lox {

bool Lox::hadError = false;
bool Lox::hadRuntimeError = false;

Interpreter Lox::interpreter{};

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
        std::exit(65);
    }
    if (Lox::hadRuntimeError) {
        std::exit(70);
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

    // Parse those tokens into statements
    Parser parser{tokens};
    auto statements = parser.parse();

    if (hadError) return;
    // Static analysis
    auto resolver = Resolver{&Lox::interpreter};
    resolver.resolve(statements);

    if (hadError) return;

    // Run the expression to generate side-effects
    Lox::interpreter.interpret(statements);


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

void Lox::runtimeError(RuntimeError& error) {
    std::cerr<<error.what()<<"\n["<<error.op.line<<"]";
    hadRuntimeError = true;
}

void Lox::report(const int line, const std::string& where,const std::string& message) {
    std::cerr << "[line " << line << "] Error " << where << ": " << message << std::endl;
    Lox::hadError = true;
}

} // namespace Lox