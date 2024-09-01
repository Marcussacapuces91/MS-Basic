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

#include <vector>
#include <set>
#include <iostream>
#include <sstream>

#include "tokenizer.h"
#include "tokens.h"

/*
class Context {
};
*/

/*
class CommandLine {
	public:
		CommandLine(const unsigned aLabel, const std::vector<Token*>& aTokens) : label(aLabel), tokens(aTokens) {}

		void execute(Context& context) const {
		}

	protected:

	private:
		const unsigned label;
		std::vector<Token*> tokens;
};
*/

class Interpreter {
	public:
		enum error_t {
			OK,
			SYNTAX_ERROR
		};
		
		Interpreter(std::istream& aIn = std::cin, std::ostream& aOut = std::cout, std::ostream& aErr = std::cerr) :
			in(aIn),
			out(aOut),
			err(aErr) {
		}

		error_t interpret(std::string& aCommand) {
			static const Tokenizer tokenizer;

			if (!aCommand.length()) return OK;

			bool error;
			int pos;
			const auto tokens = tokenizer.tokenize(aCommand, error, pos);		// need to free later all takens!!
			if (error) {
				err << "Syntax Error in:" << std::endl;
				err << aCommand << std::endl;
				err << std::string(pos, ' ') << '^' << std::endl;
				return SYNTAX_ERROR;
			}
//             out << tokens << std::endl;

			auto token = tokens.begin();
			if (const auto pTC = dynamic_cast<TokenConstant*>(*token)) {
				err << "Constant " << *pTC << " " << pTC->getType() << std::endl;
				++token;
			}
			while (token != tokens.end()) {



				++token;
			}



/*
			if (const auto pTC = dynamic_cast<TokenConstant*>(tokens[0])) {
				err << "Constant " << *pTC << std::endl;

				return 0;
			} else if (const auto pTIn = dynamic_cast<TokenInstruction*>(tokens[0])) {
				err << "Instruction " << *pTIn << std::endl;

				return 0;
			} else if (const auto pTId = dynamic_cast<TokenIdentifier*>(tokens[0])) {
				err << "Instruction " << *pTId << std::endl;

				return 0;
			} else {
				err << "An interactive command shall start with a Statement or a line number" << std::endl;
				return 1;
			}
*/
		}

		std::string toString() const {
			std::ostringstream s;
			s << "MS-Basic - Copyright (C) 2024 by M. SIBERT" << std::endl;
			s << "Ready." << std::endl;

			return  s.str();
		}

	protected:


	private:
		std::istream& in;
		std::ostream& out;
		std::ostream& err;
//    Context context;
//		std::set<CommandLine> program;
};

std::ostream& operator<<(std::ostream& out, const Interpreter& aInterpreter) {
	out << aInterpreter.toString() << std::endl;
	return out;
}

std::istream& operator>>(std::istream& in, Interpreter& aInterpreter) {
	std::string s;
	std::getline(in, s);
	const auto err = aInterpreter.interpret(s);
	if (err) std::cerr << "Error " << err << " in command line!" << std::endl;
	return in;
}
