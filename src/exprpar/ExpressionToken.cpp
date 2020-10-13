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

#include "exprpar/ExpressionToken.hpp"

#include <utility>
#include <stdexcept>

namespace ortis
{

ExpressionToken::ExpressionToken
(
	ExpressionToken::Type type,
	const std::string& symbol,
	unsigned char precedence,
	unsigned char num_operands
) :
    type(type),
    symbol(symbol),
    precedence(precedence),
    num_operands(num_operands)
{}

ExpressionToken::ExpressionToken(const ExpressionToken& base) :
    type(base.type),
    symbol(base.symbol),
    precedence(base.precedence),
    num_operands(base.num_operands)
{}

ExpressionToken::ExpressionToken(ExpressionToken&& base) :
    type(std::move(base.type)),
    symbol(std::move(base.symbol)),
    precedence(std::move(base.precedence)),
    num_operands(std::move(base.num_operands))
{}

ExpressionToken&
ExpressionToken::operator=(const ExpressionToken& base)
{
	type = base.type;
	symbol = base.symbol;
	precedence = base.precedence;
	num_operands = base.num_operands;

	return *this;
}

ExpressionToken&
ExpressionToken::operator=(ExpressionToken&& base)
{
	type = std::move(base.type);
	symbol = std::move(base.symbol);
	precedence = std::move(base.precedence);
	num_operands = std::move(base.num_operands);

	return *this;
}

void
ExpressionToken::setType(ExpressionToken::Type type)
{
    this->type = type;
}

const ExpressionToken::Type&
ExpressionToken::getType() const
{
	return type;
}

void
ExpressionToken::setSymbol(const std::string& sym)
{
	symbol = sym;
}

void
ExpressionToken::setSymbol(std::string&& sym)
{
	symbol = std::move(sym);
}

const std::string&
ExpressionToken::getSymbol() const
{
	return symbol;
}

void
ExpressionToken::setPrecedence(unsigned char precedence)
{
	this->precedence = precedence;
}

const unsigned char&
ExpressionToken::getPrecedence() const
{
	return precedence;
}

void
ExpressionToken::setNumberOfOperands(unsigned char num_operands)
{
	this->num_operands = num_operands;
}

const unsigned char&
ExpressionToken::getNumberOfOperands() const
{
	return num_operands;
}

bool
ExpressionToken::hasEqualPrecedence(const ExpressionToken& token)
{
	return precedence == token.precedence;
}

bool
ExpressionToken::hasEqualPrecedence(unsigned char precedence)
{
	return this->precedence == precedence;
}

bool
ExpressionToken::hasHigherPrecedence(const ExpressionToken& token)
{
	return precedence > token.precedence;
}

bool
ExpressionToken::hasHigherPrecedence(unsigned char precedence)
{
	return this->precedence > precedence;
}

} //namespace ortis
