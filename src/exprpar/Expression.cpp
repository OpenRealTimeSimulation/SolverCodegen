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

#include "exprpar/Expression.hpp"
#include "exprpar/ExpressionConstants.hpp"

#include <utility>
#include <stdexcept>
#include <string>
#include <iostream>

namespace ortis
{

Expression::Expression() :
	tree_root()
{}

Expression::Expression(ExpressionNode&& tree_root) :
	tree_root(std::move(tree_root))
{}

Expression::Expression(Expression&& base) :
	tree_root(std::move(base.tree_root))
{}

std::string
Expression::asString() const
{
	return tree_root.asString();
}

void
Expression::setTree(ExpressionNode&& tree)
{
	tree_root = std::move(tree);
}

const ExpressionNode&
Expression::getTree()
{
	return tree_root;
}

double
Expression::evaluate(const ExpressionSymbolTable& symbols)
{
	return evaluate(tree_root, symbols.getSymbolValueMap());
}

double
Expression::evaluate(const ExpressionSymbolTable::SymbolValueMap& symbols)
{
	return evaluate(tree_root, symbols);
}

double
Expression::evaluate(const ExpressionNode& tree, const ExpressionSymbolTable::SymbolValueMap& symbols)
{

	const auto& tree_token = tree.getToken();
	const auto& symbol     = tree_token.getSymbol();

	if(tree_token.getType() == ExpressionToken::VALUE)
	{
		if(ExpressionSymbolTable::containsSymbol(tree_token.getSymbol(), symbols))
		{
			return ExpressionSymbolTable::getSymbolValue(symbol, symbols);
		}
		else
		{
			try
			{
				return std::stod(symbol);
			}
			catch(const std::invalid_argument& error)
			{
				throw
				std::invalid_argument
				(
					std::string("Expression::evaluate(const ExpressionNode& tree, const ExpressionSymbolTable::SymbolValueMap& symbols) --")
					+ error.what()
				);
			}

		}
	}
	else if(tree_token.getType() == ExpressionToken::OPERATOR)
	{
		const auto left_node = tree.getLeftNode();
		const auto right_node = tree.getRightNode();

		#if 0
			std::cout
			<< "Current Tree Symbol: "
			<< symbol
			<< ", num operands: "
			<< int(tree_token.getNumberOfOperands())
			<< std::endl;
		#endif

		if(left_node == nullptr)
		{
			throw
			std::invalid_argument
			(
				"Expression::evaluate(const ExpressionNode& tree, const ExpressionSymbolTable::SymbolValueMap& symbols) --"
				"operator node has null left child node (no left operand)"
			);
		}

		if(symbol == ExpressionConstants::OPERATOR_UNARY_PLUS_SYMBOL)
		{
			return evaluate(*left_node, symbols);
		}

		if(symbol == ExpressionConstants::OPERATOR_UNARY_MINUS_SYMBOL)
		{
			return -evaluate(*left_node, symbols);
		}

		if(right_node == nullptr)
		{
			throw
			std::invalid_argument
			(
				"Expression::evaluate(const ExpressionNode& tree, const ExpressionSymbolTable::SymbolValueMap& symbols) --"
				"binary operator node has null right child node (no right operand)"
			);
		}

		if(symbol == ExpressionConstants::OPERATOR_BINARY_PLUS_SYMBOL)
		{
			return evaluate(*left_node, symbols) + evaluate(*right_node, symbols);
		}

		if(symbol == ExpressionConstants::OPERATOR_BINARY_MINUS_SYMBOL)
		{
			return evaluate(*left_node, symbols) - evaluate(*right_node, symbols);
		}

		if(symbol == ExpressionConstants::OPERATOR_MULTIPLY_SYMBOL)
		{
			return evaluate(*left_node, symbols) * evaluate(*right_node, symbols);
		}

		if(symbol == ExpressionConstants::OPERATOR_DIVIDE_SYMBOL)
		{
			return evaluate(*left_node, symbols) / evaluate(*right_node, symbols);
		}

	}
	else
	{
		throw
		std::invalid_argument
		(
			"Expression::evaluate(const ExpressionNode& tree, const ExpressionSymbolTable::SymbolValueMap& symbols) --"
			"expression syntax tree has invalid elements in it"
		);
	}

	return 0.0;

}

} //namespace ortis
