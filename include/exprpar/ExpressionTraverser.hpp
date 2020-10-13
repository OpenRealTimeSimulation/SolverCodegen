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

#ifndef EXPRPAR_EXPRESSIONTRAVERSER_HPP
#define EXPRPAR_EXPRESSIONTRAVERSER_HPP

#include "exprpar/Expression.hpp"
#include "exprpar/ExpressionNode.hpp"
#include "exprpar/ExpressionSymbolTable.hpp"

namespace ortis
{

/**
	\brief pure abstract base class template for traversers of Expression syntax trees

	\author Matthew Milton

	\date Created July 26, 2020

	Children of this abstract class can be implemented to traverse over Expression syntax trees to
	evaluate/analyze/interpret/etc. the trees and their inner nodes.

	\tparam T data type returned from traversing expression syntax trees
**/
template<typename T>
class ExpressionTraverser
{
public:

	/**
		\brief traverses the inner tree of an Expression object
	**/
	virtual
	T
	operator()(const Expression& expr) = 0;

	/**
		\brief traverses a syntax tree, starting from given tree node
	**/
	virtual
	T
	operator()(const ExpressionNode& tree) = 0;

	/**
		\brief traverses a syntax tree, starting from given tree node, using symbol table as
		reference for traversal
	**/
	virtual
	T
	operator()(const ExpressionNode& tree, const ExpressionSymbolTable::SymbolValueMap& symbols) = 0;

	/**
		\brief traverses a syntax tree, starting from given tree node, using symbol-value mappings
		as reference for traversal
	**/
	virtual
	T
	operator()(const ExpressionNode& tree, const ExpressionSymbolTable& symbol_value_table) = 0;

};

} //namespace ortis

#endif // EXPRPAR_EXPRESSIONTRAVERSER_HPP
