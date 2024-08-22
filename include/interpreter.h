#pragma once

#include <vector>
#include <iostream>

#include "tokenizer.h"
#include "token.h"

class Context {
};

class Line {
public:
    Line(const unsigned aLabel, const std::vector<Token*>& aTokens) : label(aLabel), tokens(aTokens) {}

    void execute(Context& context) const {
    }

protected:

private:
    const unsigned label;
    std::vector<Token*> tokens;
};



class Interpreter {
public:
    Interpreter(std::ostream& aOut = std::cout, std::istream& aIn = std::cin) : out(aOut), in(aIn) {
        out << "MS Basic - Copyright (c) 2024 by M. SIBERT" << std::endl;
        out << std::endl;
        out << "Ready." << std::endl;
    }

    unsigned interactive(std::string& aCommand) {
        static const Tokenizer tokenizer;

        if (aCommand.length() > 0) {
            auto tokens = tokenizer.tokenize(aCommand);
            std::cout << "> " << tokens << std::endl;

//            std::cerr << typeid(tokens[0]).name() << std::endl;
//            std::cerr << typeid(*tokens[0]).name() << std::endl;


            if (typeid(*tokens[0]) == typeid(TokenInstruction)) {
                std::cerr << "Instruction " << *tokens[0] << std::endl;

                return 0;

            } else if (typeid(*tokens[0]) == typeid(TokenConstant)) {
                std::cerr << "Constant " << *tokens[0] << std::endl;

                return 0;

            } else {
                std::cerr << "An interactive command shall start with a Statement or a line number" << std::endl;
                return 1;
            }


            return 0;
        }

        return 0;
    }

protected:


private:
    std::ostream& out;
    std::istream& in;
    Context context;
    std::vector<Line> program;
};

std::ostream& operator<<(std::ostream& out, const Interpreter& aInterpreter) {
    out << "MS BASIC - Copyright (c) 2024 by M. SIBERT" << std::endl;
    return out;
}

std::istream& operator>>(std::istream& in, Interpreter& aInterpreter) {
    std::string s;
    std::getline(in, s);
    const auto err = aInterpreter.interactive(s);
    if (err) std::cerr << "Error " << err << " in command line!" << std::endl;
    return in;
}
