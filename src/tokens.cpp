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

#include "tokens.h"

#include <iostream>

TokenComment::TokenComment(const std::string& aText) : text(aText) {}

TokenComment* TokenComment::create(std::string::const_iterator& aStart, const std::string::const_iterator& aStop)
{
	static const std::regex exp("^REM(.*)", std::regex_constants::icase);

	std::smatch sm;
	if (std::regex_match(aStart, aStop, sm, exp)) {
		aStart += sm[0].length();
		return new TokenComment(sm[1]);
	}
	return nullptr; // No instruction found!
}

std::string TokenComment::toString() const
{
	return "REM" + text;
}


TokenInstruction::TokenInstruction(const unsigned aId) : id(aId) {}

TokenInstruction* TokenInstruction::create(std::string::const_iterator& aStart, const std::string::const_iterator& aStop)
{
	for (unsigned i = 0; i < sizeof(tokens) / sizeof(tokens[0]) ; ++i) {
		const std::regex exp("^" + tokens[i] + ".*", std::regex_constants::icase);
		if (std::regex_match(aStart, aStop, exp)) {
			aStart += tokens[i].size();
			return new TokenInstruction(i);
		}
	}
	return nullptr; // No instruction found!
}

const std::string& TokenInstruction::getString() const
{
	return tokens[id];
}

std::string TokenInstruction::toString() const
{
	return getString();
}

const std::string TokenInstruction::tokens[] = {
	"AUTO",
	"BEEP", "BLOAD", "BSAVE",
	"CALL", "CHAIN", "CHDIR", "CIRCLE", "CLEAR", "CLOSE", "CLS", "COLOR", "COM", "COMMON", "CONT",
	"DATA", "DEF", "FNSEG", "FNUSR", "DELETE", "DIM", "DRAW",
	"EDIT", "ELSE", "END", "ERASE", "ERROR",
	"FIELD", "FILES", "FOR", "TO", "STEP",
	"GET", "GOSUB", "GOTO",
	"IF", "INPUT",
	"KEY", "KILL",
	"LET", "LINE", "LIST", "LLIST", "LOAD", "LOCK", "LPRINT", "LSET",
	"MERGE", "MKDIR",
	"NAME", "NEXT", "NEW",
	"ON", "COM", "PLAY", "STRIG", "TIMER", "OPEN", "OPTION_BASE", "OUT",
	"PAINT", "PALETTE", "PEEK", "PEN", "PLAY", "PMAP", "POINT", "POKE", "PRESET", "PRINT", "PSET", "PUT",
	"RANDOMIZE", "READ", "RENUM", "RESET", "RESTORE", "RESUME", "RETURN", "RMDIR", "RSET", "RUN",
	"SAVE", "SCREEN", "SHELL", "SOUND", "STOP", "STRIG", "SYSTEM",
	"THEN", "TROFF", "TRON",
	"UNLOCK",
	"WAIT", "WEND", "WHILE", "WIDTH", "WINDOW", "WRITE"
};

TokenFunction::TokenFunction(const unsigned aId) : id(aId) {}

TokenFunction* TokenFunction::create(std::string::const_iterator& aStart, const std::string::const_iterator& aStop)
{
	for (unsigned i = 0; i < sizeof(tokens) / sizeof(tokens[0]) ; ++i) {
		const std::regex exp("^" + tokens[i] + ".*", std::regex_constants::icase);
		if (std::regex_match(aStart, aStop, exp)) {
			aStart += tokens[i].size();
			return new TokenFunction(i);
		}
	}
	return nullptr; // No instruction found!
}

std::string TokenFunction::toString() const
{
	return tokens[id];
}

const std::string TokenFunction::tokens[] = {
};


TokenIdentifier::TokenIdentifier(const std::string& aName, const type_t& aType) : name(aName), type(aType) {}

TokenIdentifier* TokenIdentifier::create(std::string::const_iterator& aStart, const std::string::const_iterator& aStop)
{
	static const std::regex exp("^([A-Z]+\\w*[\\$%]?).*", std::regex_constants::icase);

	std::smatch sm;
	if (std::regex_match(aStart, aStop, sm, exp)) {
		aStart += sm[1].length();
		const type_t t = sm[1].str()[sm[1].length() - 1] == '$' ? STRING : (sm[1].str()[sm[1].length() - 1] == '%' ? INTEGER : SINGLE);
		return new TokenIdentifier(sm[1], t);
	}
	return nullptr; // No identifier found!
}

const std::string& TokenIdentifier::getName() const
{
	return name;
}

const Token::type_t& TokenIdentifier::getType() const
{
	return type;
}

std::string TokenIdentifier::toString() const
{
	return name;
}


TokenOperator::TokenOperator(const std::string& aId) : id(aId) {}

TokenOperator* TokenOperator::create(std::string::const_iterator& aStart, const std::string::const_iterator& aStop)
{
	static const std::regex exp("^([+\\-*\\/<>=()\\[\\]\\%\\^]+).*", std::regex_constants::icase);

	std::smatch sm;
	if (std::regex_match(aStart, aStop, sm, exp)) {
		aStart += sm[1].length();
		return new TokenOperator(sm[1]);
	}
	return nullptr; // No identifier found!
}

std::string TokenOperator::toString() const
{
	return id;
}


TokenConstant::TokenConstant(const std::string& aValue, const type_t& aType) : value(aValue), type(aType) {}

TokenConstant* TokenConstant::create(std::string::const_iterator& aStart, const std::string::const_iterator& aStop)
{
	static const std::regex expString("^\"(.*?[^\\\\])\".*");
	static const std::regex expChanel("^(#\\d+).*");

	static const std::regex expSingle("^(\\d*[.]\\d+(E[+-]?\\d+)?!?|\\d+E[+-]?\\d+!?|\\d+!).*", std::regex_constants::icase);
	static const std::regex expDouble("^(\\d*[.]\\d+(D[+-]?\\d+)?#?|\\d+D[+-]?\\d+#?|\\d+#).*", std::regex_constants::icase);
	static const std::regex expInt("^(\\d+).*");
	static const std::regex expHexa("^&H([\\dA-F]+).*");
	static const std::regex expOctal("^&O?([1-8]+).*");

	std::smatch sm;

	if (std::regex_match(aStart, aStop, sm, expString)) {
		aStart += sm[1].length() + 2; // 2 quotes char.
		return new TokenConstant(sm[1], STRING);
	}
	if (std::regex_match(aStart, aStop, sm, expChanel)) {
		aStart += sm[1].length();
		return new TokenConstant(sm[1], CHANEL);
	}

	if (std::regex_match(aStart, aStop, sm, expSingle)) {
		aStart += sm[1].length();
		return new TokenConstant(sm[1], SINGLE);
	}
	if (std::regex_match(aStart, aStop, sm, expDouble)) {
		aStart += sm[1].length();
		return new TokenConstant(sm[1], DOUBLE);
	}
	if (std::regex_match(aStart, aStop, sm, expInt)) {
		aStart += sm[1].length();
		return new TokenConstant(sm[1], INTEGER);
	}
	if (std::regex_match(aStart, aStop, sm, expHexa)) {
		aStart += sm[1].length();
		return new TokenConstant(sm[1], HEXADECIMAL);
	}
	if (std::regex_match(aStart, aStop, sm, expOctal)) {
		aStart += sm[1].length();
		return new TokenConstant(sm[1], OCTAL);
	}
	return nullptr; // No identifier found!
}

const Token::type_t& TokenConstant::getType() const
{
	return type;
}

const std::string& TokenConstant::getValue() const
{
	return value;
}

std::string TokenConstant::toString() const
{
	return (type == STRING ? '"' + value + '"' : value);
}


TokenSeparator::TokenSeparator(const std::string& aId) : id(aId) {}

TokenSeparator* TokenSeparator::create(std::string::const_iterator& aStart, const std::string::const_iterator& aStop)
{
	static const std::regex exp("^([:;,]).*");

	std::smatch sm;
	if (std::regex_match(aStart, aStop, sm, exp)) {
		aStart += sm[1].length();
		return new TokenSeparator(sm[1]);
	}
	return nullptr; // No token found!
}

const std::string& TokenSeparator::getId() const
{
	return id;
}

std::string TokenSeparator::toString() const
{
	return id;
}

std::ostream& operator<<(std::ostream& out, const Token& t)
{
	return out << t.toString();
}

std::ostream& operator<<(std::ostream& out, const std::vector<Token*> list)
{
	for (auto&& token : list)
//		out << (token == *list.begin() ? "" : " ") << *token;
		out << *token;
	return out;
}
