#include "../include/lox.hpp"

namespace Lox {

bool Lox::hadError = false;

void Lox::main(std::vector<std::string>& args) {
    AstPrinter printer{};
    printer.main(args);

    return;
    
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
    Scanner scanner{source};
    std::list<Token> tokens = scanner.scanTokens();

    std::cout<< tokens.size()<<std::endl;
    for (auto token : tokens) {
        std::cout << token << std::endl;
    }

}

void Lox::error(const int line,const std::string& message) {
    report(line, std::string(""), message);
}

void Lox::report(const int line, const std::string& where,const std::string& message) {
    std::cerr << "[line " << line << "] Error " << where << ": " << message << std::endl;
    Lox::hadError = true;
}

} // namespace Lox