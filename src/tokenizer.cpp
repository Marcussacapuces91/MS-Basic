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

#include <vector>
#include <string>
#include <regex>

#include "tokenizer.h"
#include "tokens.h"


std::vector<Token*> Tokenizer::tokenize(const std::string& aLine, bool& err, int& pos) const
{
	static const std::regex space("^\\s.*$");

	err = false;

	std::vector<Token*> list;
	auto posit = aLine.begin();
	const auto end = aLine.end();

	while (posit != end) {

		if (std::regex_match(posit, end, space)) {
			posit++;
			continue;
		}

		auto pTCom = TokenComment::create(posit, end);
		if (pTCom) {
			list.push_back(pTCom);
			continue;
		}

		auto pTIn = TokenInstruction::create(posit, end);
		if (pTIn) {
			list.push_back(pTIn);
			continue;
		}

		auto pTFu = TokenFunction::create(posit, end);
		if (pTFu) {
			list.push_back(pTFu);
			continue;
		}

		auto pTId = TokenIdentifier::create(posit, end);
		if (pTId) {
			list.push_back(pTId);
			continue;
		}

		auto pTOp = TokenOperator::create(posit, end);
		if (pTOp) {
			list.push_back(pTOp);
			continue;
		}

		auto pTCo = TokenConstant::create(posit, end);
		if (pTCo) {
			list.push_back(pTCo);
			continue;
		}

		auto pTSe = TokenSeparator::create(posit, end);
		if (pTSe) {
			list.push_back(pTSe);
			continue;
		}

		err = true;
		pos = 0;
		for (auto i = aLine.begin(); i != posit; ++i) ++pos;
		for (auto it = list.begin(); it != list.end(); ++it) delete(*it);
		list.clear();
		return list;
	}

	return list;
}
