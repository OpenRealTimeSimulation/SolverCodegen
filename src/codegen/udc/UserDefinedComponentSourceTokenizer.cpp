
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

#include <string>
#include <vector>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <stdexcept>

namespace lblmc
{

const std::string UserDefinedComponentSourceTokenizer::WHITESPACE_CHARS(" \n\r\t\f\v");
const std::string UserDefinedComponentSourceTokenizer::LEFT_BRACKET_CHARS("({[<");
const std::string UserDefinedComponentSourceTokenizer::RIGHT_BRACKET_CHARS(")}]>");
const std::string UserDefinedComponentSourceTokenizer::QUOTE_DELIMITER_CHARS("\"\'");
const std::string UserDefinedComponentSourceTokenizer::OPERATOR_CHARS("=,.");
const std::string UserDefinedComponentSourceTokenizer::STATEMENT_END_CHARS(";");
const std::string UserDefinedComponentSourceTokenizer::COMMENT_START_CHARS("//");
const std::string UserDefinedComponentSourceTokenizer::MACRO_START_CHARS("#");

//==================================================================================================

namespace
{

bool isDigit(char ch)
{
	return ch >= '0' && ch <= '9';
}

bool isLabelStart(char ch)
{
	return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_';
}

bool isLabelChar(char ch)
{
	return isLabelStart(ch) || isDigit(ch);
}

// Token views temporarily seek the source, including when bounds checks or reads fail.
class StreamPositionGuard
{
private:
	std::istream& stream;
	std::ios::iostate state;
	std::ios::iostate exceptions;
	std::istream::pos_type position;
	bool restored;

public:
	explicit StreamPositionGuard(std::istream& source) :
		stream(source), state(source.rdstate()), exceptions(source.exceptions()), position(-1), restored(false)
	{
		stream.exceptions(std::ios::goodbit);
		position = stream.tellg();
	}

	~StreamPositionGuard()
	{
		try
		{
			restore();
		}
		catch(...)
		{
			// Preserve the original exception if restoration also fails during unwinding.
		}
	}

	void restore()
	{
		if(restored)
		{
			return;
		}
		stream.clear();
		if(position != std::istream::pos_type(-1))
		{
			stream.seekg(position);
		}
		const bool position_restored = static_cast<bool>(stream);
		stream.clear(state);
		restored = true;
		stream.exceptions(exceptions);
		if(!position_restored)
		{
			throw std::runtime_error("Token::viewFromStream(*) -- failed to restore source cursor");
		}
	}

	std::istream::pos_type getPosition() const
	{
		return position;
	}
};

} //namespace

//==================================================================================================

std::string
UserDefinedComponentSourceTokenizer::Token::viewFromStream(std::istream& strm)
{
	if(!strm.good())
	{
		throw std::runtime_error("Token::viewFromStream(*) -- source stream is not readable");
	}

	StreamPositionGuard guard(strm);
	const auto origin = guard.getPosition();
	if(origin == std::istream::pos_type(-1))
	{
		throw std::runtime_error("Token::viewFromStream(*) -- source stream is not seekable");
	}

	strm.seekg(0, std::ios::end);
	const auto end = strm.tellg();
	if(!strm || end == std::istream::pos_type(-1) || end < origin)
	{
		throw std::runtime_error("Token::viewFromStream(*) -- failed to determine source size");
	}

	const auto available = static_cast<std::uintmax_t>(end - origin);
	if(position > available || length > available - position)
	{
		throw std::out_of_range("Token::viewFromStream(*) -- token is outside source bounds");
	}

	strm.seekg(origin + static_cast<std::streamoff>(position));
	if(!strm)
	{
		throw std::runtime_error("Token::viewFromStream(*) -- failed to seek to token");
	}

	std::string text(length, '\0');
	std::size_t offset = 0;
	while(offset < length)
	{
		const auto count = static_cast<std::streamsize>(length - offset > 1024 ? 1024 : length - offset);
		strm.read(&text[offset], count);
		if(!strm || strm.gcount() != count)
		{
			throw std::runtime_error("Token::viewFromStream(*) -- failed to read complete token");
		}
		offset += static_cast<std::size_t>(count);
	}
	guard.restore();
	return text;
}

std::string
UserDefinedComponentSourceTokenizer::Token::viewFromString(const std::string& str)
{
	if(position > str.size() || length > str.size() - position)
	{
		throw std::out_of_range("Token::viewFromString(*) -- token is outside source bounds");
	}
	return str.substr(position, length);
}

std::string
UserDefinedComponentSourceTokenizer::Token::viewFromFile(const std::string& filename)
{
	std::ifstream infile(filename, std::ios::in | std::ios::binary);
	if(!infile.is_open())
	{
		throw std::runtime_error("Token::viewFromFile(*) -- failed to open file: " + filename);
	}
	return viewFromStream(infile);
}

//==================================================================================================

std::vector<UserDefinedComponentSourceTokenizer::Token>
UserDefinedComponentSourceTokenizer::tokenizeStream(std::istream& strm)
{
	char buf[1024];
	std::string str;
	try
	{
		while(strm.read(buf, sizeof(buf)) || strm.gcount() > 0)
		{
			str.append(buf, static_cast<std::size_t>(strm.gcount()));
		}
	}
	catch(const std::ios_base::failure&)
	{
		if(strm.bad() || !strm.eof())
		{
			throw std::runtime_error("UserDefinedComponentSourceTokenizer::tokenizeStream(*) -- failed to read source");
		}
		// An exception-enabled stream can throw on the final partial read at EOF.
		str.append(buf, static_cast<std::size_t>(strm.gcount()));
	}
	if(strm.bad() || (strm.fail() && !strm.eof()))
	{
		throw std::runtime_error("UserDefinedComponentSourceTokenizer::tokenizeStream(*) -- failed to read source");
	}
	return UserDefinedComponentSourceTokenizer::tokenizeString(str);
}

std::vector<UserDefinedComponentSourceTokenizer::Token>
UserDefinedComponentSourceTokenizer::tokenizeString(const std::string& str)
{
	std::vector<UserDefinedComponentSourceTokenizer::Token> tokens;

	const std::size_t str_size = str.size();
	const static std::size_t npos = std::string::npos;

	std::size_t i = 0;

	while(i < str_size)
	{
		const char ch = str[i];
		const std::size_t start = i;

		if(WHITESPACE_CHARS.find_first_of(ch) != npos)
		{
			tokens.push_back(Token{i++, 1, TOKEN_WHITESPACE});
			continue;
		}

		if(str.compare(i, COMMENT_START_CHARS.size(), COMMENT_START_CHARS) == 0)
		{
			i = str.find_first_of("\r\n", i + COMMENT_START_CHARS.size());
			if(i == npos)
			{
				i = str_size;
			}
			tokens.push_back(Token{start, i - start, TOKEN_COMMENT});
			continue;
		}

		if(QUOTE_DELIMITER_CHARS.find_first_of(ch) != npos || ch == '`')
		{
			const auto type = ch == '`' ? TOKEN_PREFORMATTED : TOKEN_QUOTE;
			++i;
			const std::size_t content_start = i;
			while(i < str_size && str[i] != ch)
			{
				if(str[i] == '\\' && i + 1 < str_size)
				{
					++i;
				}
				++i;
			}
			if(i == str_size)
			{
				throw std::invalid_argument
				(
					"UserDefinedComponentSourceTokenizer::tokenizeString(*) -- unterminated " +
					std::string(ch == '`' ? "preformatted text" : "quote") +
					" at position " + std::to_string(start)
				);
			}
			tokens.push_back(Token{content_start, i - content_start, type});
			++i;
			continue;
		}

		if(MACRO_START_CHARS.find_first_of(ch) != npos && i + 1 < str_size && isLabelStart(str[i + 1]))
		{
			i += 2;
			while(i < str_size && isLabelChar(str[i]))
			{
				++i;
			}
			tokens.push_back(Token{start, i - start, TOKEN_MACRO});
			continue;
		}

		// Numbers must be considered before labels (which can contain digits) and '.'.
		std::size_t number_start = i;
		if(ch == '+' || ch == '-')
		{
			++number_start;
		}
		if(number_start < str_size &&
			(isDigit(str[number_start]) ||
			(str[number_start] == '.' && number_start + 1 < str_size && isDigit(str[number_start + 1]))))
		{
			i = number_start;
			while(i < str_size && isDigit(str[i]))
			{
				++i;
			}
			if(i < str_size && str[i] == '.')
			{
				++i;
				while(i < str_size && isDigit(str[i]))
				{
					++i;
				}
			}
			if(i < str_size && (str[i] == 'e' || str[i] == 'E'))
			{
				++i;
				if(i < str_size && (str[i] == '+' || str[i] == '-'))
				{
					++i;
				}
				if(i == str_size || !isDigit(str[i]))
				{
					throw std::invalid_argument
					(
						"UserDefinedComponentSourceTokenizer::tokenizeString(*) -- malformed exponent at position " +
						std::to_string(start)
					);
				}
				while(i < str_size && isDigit(str[i]))
				{
					++i;
				}
			}
			tokens.push_back(Token{start, i - start, TOKEN_NUMBER});
			continue;
		}

		if(isLabelStart(ch))
		{
			while(i < str_size && isLabelChar(str[i]))
			{
				++i;
			}
			tokens.push_back(Token{start, i - start, TOKEN_LABEL});
			continue;
		}

		if(LEFT_BRACKET_CHARS.find_first_of(ch) != npos)
		{
			tokens.push_back(Token{i++, 1, TOKEN_LEFT_BRACKET});
			continue;
		}

		if(RIGHT_BRACKET_CHARS.find_first_of(ch) != npos)
		{
			tokens.push_back(Token{i++, 1, TOKEN_RIGHT_BRACKET});
			continue;
		}

		if(OPERATOR_CHARS.find_first_of(ch) != npos)
		{
			tokens.push_back(Token{i++, 1, TOKEN_OPERATOR});
			continue;
		}

		if(STATEMENT_END_CHARS.find_first_of(ch) != npos)
		{
			tokens.push_back(Token{i++, 1, TOKEN_STATEMENT_END});
			continue;
		}

		tokens.push_back(Token{i++, 1, TOKEN_UNDEFINED});
	}

	return tokens;
}

std::vector<UserDefinedComponentSourceTokenizer::Token>
UserDefinedComponentSourceTokenizer::tokenizeFile(const std::string& file)
{
	std::ifstream infile(file, std::ios::in | std::ios::binary);
	if(!infile.is_open())
	{
		throw std::runtime_error("UserDefinedComponentSourceTokenizer::tokenizeFile(*) -- failed to open file: " + file);
	}
	return UserDefinedComponentSourceTokenizer::tokenizeStream(infile);
}

} //namespace lblmc
