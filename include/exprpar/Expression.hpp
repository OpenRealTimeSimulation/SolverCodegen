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


#ifndef EXPRPAR_EXPRESSION_HPP
#define EXPRPAR_EXPRESSION_HPP

#include <string>
#include <vector>

#include "exprpar/ExpressionNode.hpp"
#include "exprpar/ExpressionToken.hpp"
#include "exprpar/ExpressionSymbolTable.hpp"

namespace ortis
{

/**
	\brief represents an expression as an abstract syntax tree

	\author Matthew Milton

	\date July 23, 2020

	Objects of this type can evaluated to return a numerical value.
**/
class Expression
{
private:

	ExpressionNode tree_root; ///< root of expression's syntax tree

public:

	/**
		\brief default constructor
	**/
	Expression();

	/**
		\brief constructs Expression that takes ownership of given tree root node

		\param tree_root root node of expression tree to be added to the constructed expression.
		This node's tree will be moved to the expression, leaving tree_root modified to be empty
	**/
	Expression(ExpressionNode&& tree_root);

	/**
		\brief move constructor

		Contents of base will be moved to constructed expression, leaving base modified to be empty
	**/
	Expression(Expression&& base);

	/**
		\brief casts expression as a string

		The string representation of the expression will be in normal polish notation with
		parentheses.  So for instance, the expression a*b+c will be expressed as +(*(a,b),c).

		\return string representation of the expression
	**/
	std::string
	asString() const;

	/**
		\brief cast operator to std::string

		same as calling asString().
	**/
	inline
	operator std::string() const
	{
		return asString();
	}

	/**
		\brief sets expression tree by moving one to the expression

		\param tree_root root node of expression tree to be added to the constructed expression.
		This node's tree will be moved to the expression, leaving tree_root modified to be empty
	**/
	void
	setTree(ExpressionNode&& tree);

	/**
		\brief gets the root tree node of the expression
	**/
	const
	ExpressionNode& getTree();

	/**
		\brief evaluates the expression to a numerical value

		This method internally uses recursion methods to evaluate the expression which may prevent
		massive expressions from being evaluated, depending on platform's stack memory.

		\param symbols expression symbol table that maps symbol variable names to values
	**/
	double
	evaluate(const ExpressionSymbolTable& symbols = ExpressionSymbolTable());

	/**
		\brief evaluates the expression to a numerical value

		This method internally uses recursion methods to evaluate the expression which may prevent
		massive expressions from being evaluated, depending on platform's stack memory.

		\param symbols symbol-value map that maps symbol variable names to values
	**/
	double
	evaluate(const ExpressionSymbolTable::SymbolValueMap& symbols);

private:

	/**
		\brief evalutes the expression to a numerical value by traversing the tree

		This method is recursive.
	**/
	double
	evaluate(const ExpressionNode& tree, const ExpressionSymbolTable::SymbolValueMap& symbols);

};

} //namespace ortis

#endif // EXPRPAR_EXPRESSION_HPP
