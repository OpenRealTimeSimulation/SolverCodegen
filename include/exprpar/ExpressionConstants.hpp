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

#ifndef EXPRPAR_EXPRESSIONCONSTANTS_HPP
#define EXPRPAR_EXPRESSIONCONSTANTS_HPP

#include <string>

namespace ortis
{

/**
	\brief namespace containing common constants for expression parsing

	\author Matthew Milton

	\date July 24, 2020
**/
namespace ExpressionConstants
{

	// operator symbol constants

	extern const std::string OPERATOR_UNARY_PLUS_SYMBOL;   ///< unary plus (keep sign) operator u+
	extern const std::string OPERATOR_BINARY_PLUS_SYMBOL;  ///< binary plus (add) operator +
	extern const std::string OPERATOR_UNARY_MINUS_SYMBOL;  ///< unary minus (negation) operator u-
	extern const std::string OPERATOR_BINARY_MINUS_SYMBOL; ///< binary minus (sub) operator -
	extern const std::string OPERATOR_MULTIPLY_SYMBOL;     ///< binary multiply (mul) operator *
	extern const std::string OPERATOR_DIVIDE_SYMBOL;       ///< binary divide (div) operator /
	extern const std::string OPERATOR_ASSIGNMENT_SYMBOL;   ///< assignment operator =

	// bracket symbol constants

	extern const std::string BRACKET_LEFT_SYMBOL;  ///< left bracket symbol (
	extern const std::string BRACKET_RIGHT_SYMBOL; ///< right bracket symbol )

	// misc. symbol constants

	extern const std::string STATEMENT_END_SYMBOL; ///< statement end symbol ;

} //namespace ExpressionConstants

} //namespace ortis

#endif // EXPRPAR_EXPRESSIONCONSTANTS_HPP
