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

#pragma once

/*
#include <cassert>
#include <vector>
#include <set>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <heapapi.h>
*/

// #include "tokenizer.h"
#include "tokens.h"

/**
 * A command is only one command, without ':' separator. It's possible to have many command in a line.
 **/
class Command : private std::vector<Token*> {
	public:
		Command(const std::vector<Token*>& aTokens) : std::vector<Token*>(aTokens) {}

		unsigned execute() const {
			auto itToken = std::vector<Token*>::cbegin();

			while (itToken != std::vector<Token*>::cend()) {
				if (const auto pTC = dynamic_cast<const TokenComment*>(*itToken)) {
					++itToken;
					continue;
				} else if (const auto pTI = dynamic_cast<const TokenInstruction*>(*itToken)) {
					switch (pTI->getId()) {
						case 20:
													
							
						default:
							std::cerr << "Instruction " << pTI->getString() << " inconnue !" << std::endl;
							exit(-1);
					}
				} else {
					std::cerr << "Token " << **itToken << " inconnue !" << std::endl;
					exit(-1);
				}

			}

		}

		friend std::ostream& operator<<(std::ostream&, const Command&);
};

/*
std::ostream& operator<<(std::ostream& out, const Command& aCommand)
{
	for (auto&& token : aCommand) {
		out << (token == *aCommand.begin() ? "" : " ") << *token;
	}
	return out;
}
*/

std::ostream& operator<<(std::ostream& out, const Command& aCommand) {
	return out << static_cast<std::vector<Token*> >(aCommand);
}

std::ostream& operator<<(std::ostream& out, const std::vector<Command>& aCommands) {
	for (auto&& command : aCommands)	{
		out << (&command == &(*aCommands.cbegin()) ? "" : " : ") << command;
	}
	return out;
}
