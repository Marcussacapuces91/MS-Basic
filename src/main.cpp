/**
 * Copyright [2024] Marc SIBERT
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **/
 
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

	interpreter.load(file);

/*
    std::string line;
    while (std::getline(file, line)) {
        interpreter.interpret(line);
    }
*/

    file.close();

    return 0;
}
