
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

	Labels use ASCII letters or '_' followed by letters, digits, or '_'. Macros are '#' plus a
	label, including '#'. Numbers are signed or unsigned decimal literals with an optional
	fraction and e/E exponent (for example -0.5, .25, 1., and 1e-9).
	Whitespace, brackets, operators (= , .), and semicolons are separate single-character tokens.
	A // comment extends up to, but excludes, the next CR/LF or end of input.
	Single/double quotes and backticks delimit text; backticks support preformatted model code.
	Delimited text can span lines. Its token covers only the contents, which may be empty.
	A backslash shields the following character from delimiter recognition; escapes are retained
	verbatim, not decoded. Unknown characters, including a bare '#', yield TOKEN_UNDEFINED.

	\throws std::invalid_argument for unterminated delimited text or an exponent without digits;
	the diagnostic includes the zero-based source position.
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
		TOKEN_MACRO,          ///< #label token, including '#'
		TOKEN_LABEL,          ///< label/name token
		TOKEN_NUMBER,         ///< number token
		TOKEN_QUOTE,          ///< string quote token, with "" '' delimiters stripped out
		TOKEN_LEFT_BRACKET,   ///< left bracket ({[< token
		TOKEN_RIGHT_BRACKET,  ///< right bracket )}]> token
		TOKEN_OPERATOR,       ///< operator token
		TOKEN_STATEMENT_END,  ///< statement end ; token
		TOKEN_WHITESPACE,     ///< whitespace token
		TOKEN_COMMENT,        ///< // comment token, including '//', excluding CR/LF
		TOKEN_PREFORMATTED    ///< backtick-delimited text, with backticks stripped out
	};

//==================================================================================================

	/**
		\brief embodies a token within an UDC definition source
	**/
	struct Token
	{
		std::size_t position; ///< zero-based byte offset; delimited text starts after its opening delimiter
		std::size_t length;   ///< byte length; delimited text excludes delimiters and may have zero length
		TokenType type;       ///< type of token

		/**
			\brief views character string of token from stream source
			\details Offsets are relative to the stream's current cursor, which is restored along
			with its state and exception mask. Requires a good, seekable stream with byte offsets.
			After tokenizeStream, clear EOF and seek back to its original starting cursor first.
			\throws std::out_of_range for invalid bounds; std::runtime_error for stream errors.
		**/
		std::string
		viewFromStream(std::istream& strm);

		/**
			\brief views character string of token from string source
			\throws std::out_of_range if the full token does not fit in the string.
		**/
		std::string
		viewFromString(const std::string& str);

		/**
			\brief views character string of token from file source
			\details Reads in binary mode so offsets match tokenizeFile, including CR/LF bytes.
			\throws std::out_of_range for invalid bounds; std::runtime_error for file errors.
		**/
		std::string
		viewFromFile(const std::string& filename);
	};

//==================================================================================================

	/**
		\brief Tokenizes from the current stream cursor to EOF without requiring seeking.
		\details Token offsets are relative to that starting cursor. The stream is consumed.
		Use binary mode for files to preserve byte offsets across all entry points.
		\throws std::runtime_error for read failures other than normal EOF.
	**/
	static
	std::vector<UserDefinedComponentSourceTokenizer::Token>
	tokenizeStream(std::istream& strm);

	static
	std::vector<UserDefinedComponentSourceTokenizer::Token>
	tokenizeString(const std::string& str);

	/**
		\brief Tokenizes a file in binary mode; offsets are relative to its beginning.
		\throws std::runtime_error if the file cannot be opened or read.
	**/
	static
	std::vector<UserDefinedComponentSourceTokenizer::Token>
	tokenizeFile(const std::string& file);

//==================================================================================================

private:

	const static std::string WHITESPACE_CHARS;
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

