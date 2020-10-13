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

#ifndef EXPRPAR_EXPRESSIONTOKEN_HPP
#define EXPRPAR_EXPRESSIONTOKEN_HPP

#include <string>

namespace ortis
{

/**
	\brief Embodies an token within an expression

	\author Matthew Milton

	\date June 2020
**/
class ExpressionToken
{
public:

	/**
		\brief Type of the tokens
	**/
	enum Type
	{
		UNDEFINED = -1,   ///< undefined token
		VALUE = 0,        ///< variable/literal value token
		OPERATOR = 1,     ///< operator token
		LEFT_BRACKET = 2, ///< left bracket ( token
		RIGHT_BRACKET = 3 ///< right bracket ) token
	};

private:

	ExpressionToken::Type type; ///< type of token
	std::string symbol;         ///< symbol stored by the token
	unsigned char precedence;   ///< precedence of token if operator; ignored otherwise
	unsigned char num_operands; ///< number of operands taken if operator; ignored otherwise

public:

	ExpressionToken
	(
		ExpressionToken::Type type = ExpressionToken::UNDEFINED,
		const std::string& symbol = std::string(""),
		unsigned char precedence = 0,
		unsigned char num_operands = 0
	);

	ExpressionToken(const ExpressionToken& base);

	ExpressionToken(ExpressionToken&& base);

	ExpressionToken&
	operator=(const ExpressionToken& base);

	ExpressionToken&
	operator=(ExpressionToken&& base);

	void
	setType(ExpressionToken::Type type);

	const ExpressionToken::Type&
	getType() const;

    void
    setSymbol(const std::string& sym);

    void
    setSymbol(std::string&& sym);

    const std::string&
    getSymbol() const;

    void
    setPrecedence(unsigned char precedence);

    const unsigned char&
    getPrecedence() const;

    void
    setNumberOfOperands(unsigned char num_operands);

    const unsigned char&
    getNumberOfOperands() const;

    bool
    hasEqualPrecedence(const ExpressionToken& token);

    bool
	hasEqualPrecedence(unsigned char precedence);

	bool
	hasHigherPrecedence(const ExpressionToken& token);

	bool
	hasHigherPrecedence(unsigned char precedence);

};

} //namespace ortis

#endif // EXPRPAR_EXPRESSIONTOKEN_HPP
