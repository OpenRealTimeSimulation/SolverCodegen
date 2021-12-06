
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

#ifndef LBLMC_USERDEFINEDCOMPONENTSOURCETOKENIZER_HPP
#define LBLMC_USERDEFINEDCOMPONENTSOURCETOKENIZER_HPP

#include <istream>
#include <string>
#include <vector>
#include <cstddef>

namespace lblmc
{

/**
	\brief tokenizes source file for a User Defined Component for parsing and loading definitions

	\author Matthew Milton

	\date Created July 18, 2020
**/
class UserDefinedComponentSourceTokenizer
{

//==================================================================================================

public:

	/**
		\brief types of tokens found in UDC definition sources
	**/
	enum TokenType
	{
		TOKEN_UNDEFINED = -1, ///< undefined/unrecognized token
		TOKEN_MACRO,          ///< macro token
		TOKEN_LABEL,          ///< label/name token
		TOKEN_NUMBER,         ///< number token
		TOKEN_QUOTE,          ///< string quote token, with "" '' delimiters stripped out
		TOKEN_LEFT_BRACKET,   ///< left bracket ({[< token
		TOKEN_RIGHT_BRACKET,  ///< right bracket )}]> token
		TOKEN_OPERATOR,       ///< operator token
		TOKEN_STATEMENT_END,  ///< statement end ; token
		TOKEN_WHITESPACE,     ///< whitespace token
		TOKEN_COMMENT,        ///< code comment token
		TOKEN_PREFORMATTED    ///< preformatted string token
	};

//==================================================================================================

	/**
		\brief embodies a token within an UDC definition source
	**/
	struct Token
	{
		std::size_t position; ///< zeroth position in source where token starts; position is at first character in token
		std::size_t length;   ///< length of the token in source; includes all characters of token
		TokenType type;       ///< type of token

		/**
			\brief views character string of token from stream source
		**/
		std::string
		viewFromStream(std::istream& strm);

		/**
			\brief views character string of token from string source
		**/
		std::string
		viewFromString(const std::string& str);

		/**
			\brief views character string of token from file source
		**/
		std::string
		viewFromFile(const std::string& filename);
	};

//==================================================================================================

	static
	std::vector<UserDefinedComponentSourceTokenizer::Token>
	tokenizeStream(std::istream& strm);

	static
	std::vector<UserDefinedComponentSourceTokenizer::Token>
	tokenizeString(const std::string& str);

	static
	std::vector<UserDefinedComponentSourceTokenizer::Token>
	tokenizeFile(const std::string& file);

//==================================================================================================

private:

	const static std::string WHITESPACE_CHARS;
	const static std::string LABEL_CHARS;
	const static std::string NUMBER_CHARS;
	const static std::string LEFT_BRACKET_CHARS;
	const static std::string RIGHT_BRACKET_CHARS;
	const static std::string QUOTE_DELIMITER_CHARS;
	const static std::string OPERATOR_CHARS;
	const static std::string STATEMENT_END_CHARS;
	const static std::string COMMENT_START_CHARS;
	const static std::string MACRO_START_CHARS;

//==================================================================================================

};

} //namespace lblmc

#endif // LBLMC_USERDEFINEDCOMPONENTSOURCETOKENIZER_HPP

