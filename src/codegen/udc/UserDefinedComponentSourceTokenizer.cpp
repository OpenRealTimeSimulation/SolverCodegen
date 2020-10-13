
/*

Copyright (C) 2020 Matthew Milton

This file is part of the LB-LMC Solver C++ Code Generation Library, included in the
Open Real-Time Simulation (ORTiS) Framework.

LB-LMC Solver C++ Code Generation Library is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

LB-LMC Solver C++ Code Generation Library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with LB-LMC Solver C++ Code Generation Library.  If not, see <https://www.gnu.org/licenses/>.

*/

#include "codegen/udc/UserDefinedComponentSourceTokenizer.hpp"
#include "codegen/Cpp.hpp"

#include <string>
#include <vector>
#include <cstddef>

namespace lblmc
{

const std::string UserDefinedComponentSourceTokenizer::WHITESPACE_CHARS(Cpp::WHITESPACE_CHARS);
const std::string UserDefinedComponentSourceTokenizer::LABEL_CHARS(Cpp::VALID_NAME_CHARS);
const std::string UserDefinedComponentSourceTokenizer::NUMBER_CHARS(Cpp::VALID_NUMBER_CHARS);
const std::string UserDefinedComponentSourceTokenizer::LEFT_BRACKET_CHARS("({[<");
const std::string UserDefinedComponentSourceTokenizer::RIGHT_BRACKET_CHARS(")}]>");
const std::string UserDefinedComponentSourceTokenizer::QUOTE_DELIMITER_CHARS("\"\'");
const std::string UserDefinedComponentSourceTokenizer::OPERATOR_CHARS("=,.");
const std::string UserDefinedComponentSourceTokenizer::STATEMENT_END_CHARS(";");
const std::string UserDefinedComponentSourceTokenizer::COMMENT_START_CHARS("//");
const std::string UserDefinedComponentSourceTokenizer::MACRO_START_CHARS("#");

//==================================================================================================

std::vector<UserDefinedComponentSourceTokenizer::Token>
UserDefinedComponentSourceTokenizer::tokenizeStream(std::istream& strm)
{

}

std::vector<UserDefinedComponentSourceTokenizer::Token>
UserDefinedComponentSourceTokenizer::tokenizeString(const std::string& str)
{
	std::vector<UserDefinedComponentSourceTokenizer::Token> tokens;

	TokenType last_type = TokenType::TOKEN_UNDEFINED;

	const std::size_t str_size = str.size();
	const static std::size_t npos = std::string::npos;

    std::size_t position = 0;
    std::size_t length = 0;

    std::size_t i      = 0;

    while(i < str_size)
	{
		const char ch = str[i];

		if(WHITESPACE_CHARS.find_first_of(ch) != npos)
		{
            last_type = TokenType::TOKEN_WHITESPACE;

            tokens.push_back(Token{i, 1, last_type});

            i++;
            continue;
		}

		if(LEFT_BRACKET_CHARS.find_first_of(ch) != npos)
		{
            last_type = TokenType::TOKEN_LEFT_BRACKET;

            tokens.push_back(Token{i, 1, last_type});

            i++;
            continue;
		}

		if(RIGHT_BRACKET_CHARS.find_first_of(ch) != npos)
		{
            last_type = TokenType::TOKEN_RIGHT_BRACKET;

            tokens.push_back(Token{i, 1, last_type});

            i++;
            continue;
		}

		if(OPERATOR_CHARS.find_first_of(ch) != npos)
		{
            last_type = TokenType::TOKEN_OPERATOR;

            tokens.push_back(Token{i, 1, last_type});

            i++;
            continue;
		}

		// !!! FINISH ME !!! 2020-07-19-18.21
	}

	return tokens;
}

std::vector<UserDefinedComponentSourceTokenizer::Token>
UserDefinedComponentSourceTokenizer::tokenizeFile(const std::string& file)
{

}

} //namespace lblmc
