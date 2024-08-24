#include <iostream>
#include <fstream>
#include <string>

#include "interpreter.h"

int main()
{
    Interpreter interpreter;
    std::cout << interpreter;

/*
    while (std::cin) std::cin >> interpreter;
*/

	Tokenizer tokenizer;

    std::ifstream file;
    file.open("C:\\Users\\marc\\Documents\\GitHub\\MS-Basic\\eliza.bas", std::ios::in);
    
    if (!file) {
    	std::cerr << "Error opening file!" << std::endl;
    	exit(-1);
	}

    std::string line;
    while (std::getline(file, line)) {
        interpreter.interpret(line);
    }
    file.close();

    return 0;
}
