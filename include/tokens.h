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

#include <ostream>
#include <string>
#include <regex>

class Token {
	public:
		friend std::ostream& operator<<(std::ostream&, const Token&);

		///< To distinguish between String or Number identifier (with $ terminator).
		enum type_t { STRING, INTEGER, SINGLE, DOUBLE, HEXADECIMAL, OCTAL, CHANEL };

	protected:
		virtual const std::string str() const = 0;
};

/**
 * TokenComment
 */
class TokenComment : public Token {
	public:
		/**
		 * Constructor initializing text value.
		 */
		TokenComment(const std::string& aText);

		/**
		 * Factory building the token from the passed text or returning a nullptr if none recognized.
		 */
		static TokenComment* create(std::string::const_iterator& aStart, const std::string::const_iterator& aStop);

	protected:
		virtual const std::string str() const;

	private:
///< Comment content.
		const std::string text;
};


/**
 * TokenInstruction
 */
class TokenInstruction : public Token {
	public:
		/**
		 * Constructor initializing id value.
		 */
		TokenInstruction(const unsigned aId);

		/**
		 * Factory building the token from the passed text or returning a nullptr if none recognized.
		 */
		static TokenInstruction* create(std::string::const_iterator& aStart, const std::string::const_iterator& aStop);

	protected:
		virtual const std::string str() const;

	private:
		///< Token id.
		const unsigned id;

		///< List of all tokens allowed for instructions.
		static const std::string tokens[99];
};

/**
 * TokenIdentifier
 */
class TokenIdentifier : public Token {
	public:
		/**
		 * Constructor.
		 */
		TokenIdentifier(const std::string& aName, const type_t& aType);

		/**
		 * Factory.
		 */
		static TokenIdentifier* create(std::string::const_iterator& aStart, const std::string::const_iterator& aStop);

		const std::string& getName() const;

		const type_t& getType() const;

	protected:
		virtual const std::string str() const;

	private:
		const std::string name;
		const type_t type;
};

/**
 * TokenOperator
 */
class TokenOperator : public Token {
	public:
		/**
		 * Constructor.
		 */
		TokenOperator(const std::string& aId);

		/**
		 * Factory.
		 */
		static TokenOperator* create(std::string::const_iterator& aStart, const std::string::const_iterator& aStop);

	protected:
		virtual const std::string str() const;

	private:
		const std::string id;
};

/**
 * TokenConstant
 */
class TokenConstant : public Token {
	public:
		/**
		 * Constructor.
		 */
		TokenConstant(const std::string& aId, const type_t& aType);

		/**
		 * Factory.
		 */
		static TokenConstant* create(std::string::const_iterator& aStart, const std::string::const_iterator& aStop);

		/**
		 * Return the type of constant.
		 */
		const type_t& getType() const;

	protected:
		virtual const std::string str() const;

	private:
		const std::string id;
		const type_t type;
};

/**
 * TokenSeparator
 */
class TokenSeparator : public Token {
	public:
		/**
		 * Constructor.
		 */
		TokenSeparator(const std::string& aId);

		/**
		 * Factory.
		 */
		static TokenSeparator* create(std::string::const_iterator& aStart, const std::string::const_iterator& aStop);

	protected:
		virtual const std::string str() const;

	private:
		const std::string id;
};


/**
 * Operator<< for token instances.
 */
std::ostream& operator<<(std::ostream&, const Token&);

std::ostream& operator<<(std::ostream&, const std::vector<Token*>);
