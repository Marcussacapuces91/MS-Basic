#include <iostream>
#include <fstream>
#include <string>

#include "interpreter.h"

int main()
{
    Interpreter interpreter;

    std::cout << interpreter;
    while (std::cin) std::cin >> interpreter;


/*
    Tokenizer tokenizer;

    std::ifstream file;
    std::string line;
    file.open("eliza.bas", std::ios::in);
    while (std::getline(file, line)) {
        std::cout << tokenizer.tokenize(line) << std::endl;
    }
    file.close();
*/

    return 0;
}
