#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include "token.h"

class Tokenizer
{
    public:
        std::vector<Token*> tokenize(const std::string& aLine) const {
            static const std::regex space("^\\s.*");

            std::vector<Token*> list;
            auto pos = aLine.begin();
            const auto end = aLine.end();

            while (pos != end) {
//                for (auto it=pos; it!=end; ++it) std::cerr << *it;
//                std::cerr << std::endl;

                if (std::regex_match(pos, end, space)) {
                    pos++;
                    continue;
                }

                auto pTCom = TokenComment::create(pos, end);
                if (pTCom) {
                    list.push_back(pTCom);
                    continue;
                }

                auto pTIn = TokenInstruction::create(pos, end);
                if (pTIn) {
                    list.push_back(pTIn);
                    continue;
                }

                auto pTId = TokenIdentifier::create(pos, end);
                if (pTId) {
                    list.push_back(pTId);
                    continue;
                }

                auto pTOp = TokenOperator::create(pos, end);
                if (pTOp) {
                    list.push_back(pTOp);
                    continue;
                }

                auto pTCo = TokenConstant::create(pos, end);
                if (pTCo) {
                    list.push_back(pTCo);
                    continue;
                }

                auto pTSe = TokenSeparator::create(pos, end);
                if (pTSe) {
                    list.push_back(pTSe);
                    continue;
                }

                std::cerr << "Syntax Error in line" << std::endl;
                std::cerr << aLine << std::endl;
                for (auto i = aLine.begin(); i != pos; ++i)
                    std::cerr << " ";
                std::cerr << "^" << std::endl;
                break;

            }

            return list;
        }

    protected:

    private:
};
