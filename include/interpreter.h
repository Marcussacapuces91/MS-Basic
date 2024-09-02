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

#include <cassert>
#include <vector>
#include <set>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <heapapi.h>

#include "tokenizer.h"
#include "tokens.h"

/**
 * A command is only on command, without ':' separator. It's possible to have many command in a line.
 **/
class Command : private std::vector<Token*> {
	public:
		Command(const std::vector<Token*>& aTokens) : std::vector<Token*>(aTokens) {}

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
std::ostream& operator<<(std::ostream& out, const Command& aCommand)
{
	return out << static_cast<std::vector<Token*> >(aCommand);
}
	
std::ostream& operator<<(std::ostream& out, const std::vector<Command>& aCommands)
{
	for (auto&& command : aCommands)	{
		out << (&command == &(*aCommands.begin()) ? "" : " : ") << command;
	}
	return out;
}


class Interpreter {
	public:
		enum error_t {
			OK,
			SYNTAX_ERROR
		};

		Interpreter(std::istream& aIn = std::cin, std::ostream& aOut = std::cout, std::ostream& aErr = std::cerr) :
			in(aIn),
			out(aOut),
			err(aErr)
		{
		}

		/**
		 * Load a file in program memory.
		 **/
		error_t load(std::ifstream& aFile)
		{
			program.clear();	// empty current program
			
			std::string line;
			while (std::getline(aFile, line)) {
				// Empty line?
				if (!line.length()) continue;

				Tokenizer tokenizer;
				bool error;
				int pos;
				const auto tokens = tokenizer.tokenize(line, error, pos);		// need to free later all takens!!
				if (error) {
					assert(tokens.size() == 0);
					err << "Syntax Error in:" << std::endl;
					err << line << std::endl;
					err << std::string(pos, ' ') << '^' << std::endl;
					return SYNTAX_ERROR;
				}

				auto token = tokens.begin();
				unsigned lineNumber = 0;
				if (const auto pTC = dynamic_cast<TokenConstant*>(*token))  {
					if (pTC->getType() == Token::INTEGER) {
						lineNumber = std::stoul(pTC->getValue());
						++token;
					}
					else {
						err << "Syntax Error: A line number must be an INTEGER!" << std::endl;
						err << line << std::endl;
						return SYNTAX_ERROR;
					}
				}
				else {
					err << "Syntax Error: A line number must be an INTEGER!" << std::endl;
					err << line << std::endl;
					return SYNTAX_ERROR;
				}
				assert(lineNumber);	// not 0

				std::vector<Command> commands;
				while (token != tokens.end()) {
					auto command = commandSlicer(token, tokens.end());
					commands.push_back(command);
				}
				const auto success = program[lineNumber] = commands;
//				out << std::setw(5) << lineNumber << " " << commands << std::endl;
			}
			return OK;
		}
		
		error_t list(const unsigned start=0, const unsigned stop=65535) const {
			for (auto&& line : program) {
				if ((line.first >= start) && (line.first <= stop))
					out << std::setw(5) << line.first << ' ' << line.second << std::endl;
			}
			return OK;
		}

		/**
		 * Slice each tokens' list in separate commands, using ':' separator.
		 * @param start Iterator on first token.
		 * @param stop Iterator after the last token.
		 * @return A vector of tokens.
		 **/
		Command commandSlicer(std::vector<Token*>::const_iterator& start, const std::vector<Token*>::const_iterator& stop)
		{
			std::vector<Token*> res;
			for (auto token = start; token != stop; ++token) {
				if (const auto pTSep = dynamic_cast<TokenSeparator*>(*token)) {
					if(pTSep->getId() == ":") {
						start = ++token;
						return res;
					}
				}
				res.push_back(*token);
			}
			start = stop;
			return Command(res);
		}


		/**
		 * Return a string describing the current interpreter.
		 **/
		std::string toString() const
		{
			const auto handle = GetProcessHeap();
			if (!handle) {
				err << "Error getting process heap handle in" << __FILE__ << ':' << __LINE__ << ", func:" << __PRETTY_FUNCTION__ << std::endl;
				exit(-1);
			}
			
			HEAP_SUMMARY summary = { sizeof(HEAP_SUMMARY), 0, 0, 0, 0 };
			
			if (!HeapSummary(handle, 0, &summary)) {
				err << "Error getting heap summary in" << __FILE__ << ':' << __LINE__ << ", func:" << __PRETTY_FUNCTION__ << std::endl;
				exit(-1);
			}
			
			
			std::ostringstream s;
			s << PRODUCT_NAME << ' ' << PRODUCT_VERSION << std::endl
			  << "(C) Copyright M. SIBERT 2024" << std::endl
			  << summary.cbReserved << " Bytes free" << std::endl
			  << "Ok" << std::endl;

			return s.str();
		}

	protected:


	private:
		std::istream& in;
		std::ostream& out;
		std::ostream& err;

		std::map<unsigned, std::vector<Command> > program;
};

std::ostream& operator<<(std::ostream& out, const Interpreter& aInterpreter)
{
	return out << aInterpreter.toString() << std::endl;
}
