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

#ifndef EXPRPAR_EXPRESSIONPARSER_HPP
#define EXPRPAR_EXPRESSIONPARSER_HPP

#include <string>
#include <cstddef>
#include <vector>
#include <stack>

#include "exprpar/Expression.hpp"

namespace ortis
{

/**
	\brief Parses infix math expressions which can be evaluated

	The expression parser and expression objects support infix math expressions with +*-/ operators
	and parentheses, including unary +-.
	Higher precedence operators (exponents), assignments (=), and functions are not (yet) supported.
	Both variable and literal values in the expressions are supported.

	Presently, the expression parser only supports expressions containing only: the math operators
	+*-/, parentheses grouping (), literal numerical values (i.e. 1.2e3), and alphanumerical
	variable names.
	The expressions must be expressed in infix notation and not contain any unsupported operators
	or characters.
	Assignments to variables is not yet supported.
	Any malformed expression string will cause the expression parser to raise a syntax or other
	error.

	Example math expressions supported by the parser:\n
	2.0 + 3.0\n
	a + b\n
	a+(b*c)/(b+c)*1.0e3

	\author Matthew Milton

	\date June 2020
**/
class ExpressionParser
{
public:

	const static std::string WHITESPACE_CHARS; ///< white space characters
	const static std::string BAD_START_CHARS;  ///< invalid characters for start of a label
	const static std::string VALID_NAME_CHARS; ///< valid label/name characters
	const static std::string VALID_NUMBER_CHARS; ///< valid number characters
	const static std::string DIGIT_CHARS; ///< numerical digit characters
	const static std::string OPERATOR_CHARS; ///< supported operator characters
	const static std::string BRACKET_CHARS; ///< bracket characters
	const static std::string VALUE_CHARS; ///< value characters

	const static std::map<std::string,unsigned char> OPERATOR_PRECEDENCE; ///< internal map for operator precendence

	/**
		\brief tokenizes an infix math expression with infix order of the tokens

		\param expr_str infix expression string to tokenize

		\return vector of expression tokens in infix order
	**/
	std::vector<ExpressionToken>
	tokenize(const std::string& expr_str) const;

	/**
		\brief tokenizes an infix math expression with postfix order of the tokens

		\param expr_str infix expression string to tokenize

		\return vector of expression tokens in postfix order
	**/
	std::vector<ExpressionToken>
	tokenizeToPostfix(const std::string& expr_str) const;

	/**
		\brief expresses an infix math expression in postfix notation

		\param expr_str infix expression string to tokenize

		\return string of the math expression in postfix notation
	**/
	std::string
	expressAsPostfix(const std::string& expr_str) const;

	/**
		\brief parses an infix math expression and returns an expression syntax tree

		\param expr_str infix expression string to parse

		\return Expression object containing the expression's syntax tree

	**/
	Expression
	parse(const std::string& expr_str) const;

private:

	void
	addNode(std::stack<ExpressionNode>& nodes, const ExpressionToken& token) const;

	bool
	isWhitespace(char c) const;

	bool
	isBracket(char c) const;

	bool
	isOperator(char c) const;

	bool
	isValueChar(char c) const;



};

} //namespace ortis

#endif // EXPRPAR_EXPRESSIONPARSER_HPP
