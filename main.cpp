#include "./include/lox.hpp"

#include <iostream>

int main(int argc, char** argv) {

    Lox::Lox lox{};
    // Basic setup to send args to main
    std::vector<std::string> args{};
    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        args.push_back(arg);
    }

    lox.main(args);


    return 0;
}